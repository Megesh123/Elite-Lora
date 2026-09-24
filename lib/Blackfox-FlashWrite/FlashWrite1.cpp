#include "FlashWrite1.h"
#include "Arduino.h"
#include <esp_wifi.h>
#include <esp_flash.h>
#include <esp_err.h>
#include "esp_partition.h"

FLASHWRITE1 Flash1;

// Bytewise CRC-32 (poly 0xEDB88320), no table - a few hundred bytes per commit
// is trivial and this keeps the class dependency-free.
uint32_t FLASHWRITE1::_crc32(const uint8_t *data, size_t len)
{
    uint32_t crc = 0xFFFFFFFFUL;
    for (size_t i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (int b = 0; b < 8; b++)
            crc = (crc >> 1) ^ (0xEDB88320UL & (-(int32_t)(crc & 1)));
    }
    return crc ^ 0xFFFFFFFFUL;
}

FLASHWRITE1::FLASHWRITE1() : _write_buffer(nullptr),
                             _read_buffer(nullptr),
                             _write_offset(0),
                             _read_offset(0),
                             _total_size(0),
                             _slot_stride(0),
                             _active_slot(-1),
                             _seq(0)
{
}

FLASHWRITE1::~FLASHWRITE1()
{
    _cleanup();
}

void FLASHWRITE1::_cleanup()
{
    if (_write_buffer)
    {
        free(_write_buffer);
        _write_buffer = nullptr;
    }
    if (_read_buffer)
    {
        free(_read_buffer);
        _read_buffer = nullptr;
    }
    _write_offset = 0;
    _read_offset = 0;
    _active_slot = -1;
    _seq = 0;
}

void FLASHWRITE1::end()
{
    _cleanup();
}

void FLASHWRITE1::begin(size_t _size)
{
    _cleanup();

    _total_size = _size;
    _total_size = (_total_size + 3) & ~3;
    _write_buffer = (uint8_t *)malloc(_total_size);

    if (!_write_buffer)
    {
        Serial.println("Flash1 - Failed to allocate write buffer!");
        return;
    }

    memset(_write_buffer, 0, _total_size);
    _write_offset = FLASH1_HEADER_SIZE; // payload starts after the record header
    _find_start_end_flash_address();
    _read();
}

void FLASHWRITE1::_find_start_end_flash_address()
{
    const esp_partition_t *partition = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, NULL);

    if (partition != NULL)
    {
        FLASH_START_ADDRESS = partition->address;
        FLASH_TOTAL_CHAR = partition->size;

        // Two ping-pong slots, each rounded up to a full erase sector (4096B)
        // so writing the new slot's sector(s) never touches the other slot.
        _slot_stride = (_total_size + 4095) & ~((size_t)4095);
        FLASH_END_ADDRESS = FLASH_START_ADDRESS + (_slot_stride * 2);

        if (FLASH_TOTAL_CHAR < _slot_stride * 2)
        {
            Serial.println("Flash1 - SPIFFS partition too small for ping-pong slots!");
        }
    }
    else
    {
        Serial.println("Flash1 - No SPIFFS partition found!");
    }
}

// Original offset-based string functions (keep for backward compatibility)
void FLASHWRITE1::writeString(const String &str)
{
    uint16_t str_size = str.length();

    // Write the string size first
    write(str_size);

    // Then write the string content
    if (!_write_buffer || _write_offset + str_size > _total_size)
    {
        Serial.println("Flash1 - String write buffer overflow!");
        return;
    }

    memcpy(_write_buffer + _write_offset, str.c_str(), str_size);
    _write_offset += str_size;
}

void FLASHWRITE1::readString(String &str)
{
    uint16_t str_size = 0;

    // First read the string size
    read(str_size);

    if (!_read_buffer || _read_offset + str_size > _total_size)
    {
        Serial.println("Flash1 - String read buffer overflow!");
        str = "";
        return;
    }

    // Read the string content
    char temp[str_size + 1];
    memcpy(temp, _read_buffer + _read_offset, str_size);
    temp[str_size] = '\0';
    str = String(temp);
    _read_offset += str_size;
}

// // Address-based string writing with size prefix
// void FLASHWRITE1::writeString(const String &str, size_t address)
// {
//     uint16_t str_size = str.length();

//     // Write the string size first at the specified address
//     write(str_size, address);

//     // Then write the string content after the size
//     if (address + sizeof(str_size) + str_size > _total_size)
//     {
//         Serial.println("Flash1 - String write buffer overflow!");
//         return;
//     }

//     memcpy(_write_buffer + address + sizeof(str_size), str.c_str(), str_size);
// }

// // Address-based string reading with size prefix
// void FLASHWRITE1::readString(String &str, size_t address)
// {
//     uint16_t str_size;

//     // First read the string size from the specified address
//     read(str_size, address);

//     if (address + sizeof(str_size) + str_size > _total_size)
//     {
//         Serial.println("Flash1 - String read buffer overflow!");
//         str = "";
//         return;
//     }

//     // Read the string content after the size
//     char temp[str_size + 1];
//     memcpy(temp, _read_buffer + address + sizeof(str_size), str_size);
//     temp[str_size] = '\0';
//     str = String(temp);
// }

// Erases BOTH ping-pong slots (factory-reset style). Not used by the normal
// commit() path, which only ever erases the inactive slot.
void FLASHWRITE1::erase()
{
    size_t bytes_to_erase = _slot_stride * 2;

    if (bytes_to_erase == 0)
    {
        Serial.println("Flash1 - Nothing to erase");
        return;
    }

    esp_err_t ret1 = esp_flash_erase_region(esp_flash_default_chip, FLASH_START_ADDRESS, bytes_to_erase);
    if (ret1 != ESP_OK)
    {
        Serial.printf("Flash1 - erase failed: 0x%x\n", ret1);
    }

    _valid = false;
    _active_slot = -1;
    _seq = 0;
}

void FLASHWRITE1::commit()
{
    if (!_write_buffer)
        return;

    // Fill the record header over the payload that write()/writeString() staged.
    uint16_t payload_len = (uint16_t)(_write_offset - FLASH1_HEADER_SIZE);
    uint32_t magic = FLASH1_MAGIC;
    uint16_t version = FLASH1_VERSION;
    uint32_t next_seq = _seq + 1;
    uint32_t crc = _crc32(_write_buffer + FLASH1_HEADER_SIZE, payload_len);

    memcpy(_write_buffer + 0, &magic, 4);
    memcpy(_write_buffer + 4, &version, 2);
    memcpy(_write_buffer + 6, &payload_len, 2);
    memcpy(_write_buffer + 8, &crc, 4);
    memcpy(_write_buffer + 12, &next_seq, 4);

    // Always target the slot that is NOT currently active. If a reset/panic
    // hits mid-erase or mid-write here, the other slot - still holding the
    // last good record - is never touched, so _read() recovers it on the
    // next boot instead of the whole record coming back blank.
    int target_slot = (_active_slot == 0) ? 1 : 0;
    size_t target_address = FLASH_START_ADDRESS + ((size_t)target_slot * _slot_stride);

    esp_err_t eret = esp_flash_erase_region(esp_flash_default_chip, target_address, _slot_stride);
    if (eret != ESP_OK)
    {
        Serial.printf("Flash1 - erase failed: 0x%x\n", eret);
    }

    esp_err_t wret = esp_flash_write(esp_flash_default_chip, _write_buffer, target_address, _total_size);
    if (wret != ESP_OK)
    {
        Serial.printf("Flash1 - Flash write failed: 0x%x\n", wret);
    }
    else
    {
        // Only now does the new record become "active" - after it's fully on flash.
        _active_slot = target_slot;
        _seq = next_seq;
        _valid = true;
    }

    // Reset for next write operation
    free(_write_buffer);
    _write_offset = FLASH1_HEADER_SIZE;
    _write_buffer = (uint8_t *)malloc(_total_size);
    if (_write_buffer)
        memset(_write_buffer, 0, _total_size);
}

// Validates the header + CRC of a candidate record buffer. On success,
// out_seq receives the record's sequence number.
bool FLASHWRITE1::_validate_record(const uint8_t *buf, uint32_t &out_seq)
{
    uint32_t magic;
    uint16_t version, payload_len;
    uint32_t stored_crc, seq;
    memcpy(&magic, buf + 0, 4);
    memcpy(&version, buf + 4, 2);
    memcpy(&payload_len, buf + 6, 2);
    memcpy(&stored_crc, buf + 8, 4);
    memcpy(&seq, buf + 12, 4);

    if (magic != FLASH1_MAGIC || version != FLASH1_VERSION || payload_len > (_total_size - FLASH1_HEADER_SIZE))
        return false;

    uint32_t calc_crc = _crc32(buf + FLASH1_HEADER_SIZE, payload_len);
    if (calc_crc != stored_crc)
        return false;

    out_seq = seq;
    return true;
}

void FLASHWRITE1::_read()
{
    if (_read_buffer)
    {
        free(_read_buffer);
        _read_buffer = nullptr;
    }

    _read_buffer = (uint8_t *)malloc(_total_size);
    if (!_read_buffer)
    {
        Serial.println("Flash1 - Failed to allocate memory for flash read");
        _valid = false;
        _active_slot = -1;
        _seq = 0;
        return;
    }

    uint8_t *candidate = (uint8_t *)malloc(_total_size);
    if (!candidate)
    {
        Serial.println("Flash1 - Failed to allocate scratch buffer for flash read");
        free(_read_buffer);
        _read_buffer = nullptr;
        _valid = false;
        _active_slot = -1;
        _seq = 0;
        return;
    }

    // Read both slots and keep whichever validates, preferring the higher
    // sequence number if both do (the normal case is only one ever will,
    // since commit() only writes the inactive slot).
    _valid = false;
    _active_slot = -1;
    _seq = 0;

    for (int slot = 0; slot < 2; slot++)
    {
        size_t addr = FLASH_START_ADDRESS + ((size_t)slot * _slot_stride);
        esp_err_t ret1 = esp_flash_read(esp_flash_default_chip, candidate, addr, _total_size);
        if (ret1 != ESP_OK)
        {
            Serial.printf("Flash1 - Flash read failed: 0x%x\n", ret1);
            continue;
        }

        uint32_t seq = 0;
        if (_validate_record(candidate, seq) && (!_valid || (int32_t)(seq - _seq) > 0))
        {
            memcpy(_read_buffer, candidate, _total_size);
            _valid = true;
            _active_slot = slot;
            _seq = seq;
        }
    }

    free(candidate);

    _read_offset = FLASH1_HEADER_SIZE; // skip the record header

    if (!_valid)
    {
        // Neither slot checked out (blank sectors, half-written record, wrong
        // version) -> zero the payload so the caller restores defaults
        // instead of loading corruption.
        Serial.println("Flash1 - stored record invalid, using defaults");
        memset(_read_buffer, 0, _total_size);
    }
}