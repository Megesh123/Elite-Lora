#ifndef __FLASH_WRITE_1_H__
#define __FLASH_WRITE_1_H__

#include <Arduino.h>
#include <cstddef>

// On-flash record layout: [ magic u32 | version u16 | payload_len u16 | crc32 u32 | seq u32 | payload... ]
// commit() fills the header and CRCs the payload; _read() validates it and
// exposes the result via valid(). A blank or half-written sector fails
// validation, so callers fall back to defaults instead of loading garbage.
//
// Storage is ping-pong: two sector-aligned slots. commit() always erases and
// writes the slot that is NOT currently active, then only switches the active
// slot once that write succeeds. A reset/panic during erase or write leaves
// the other slot - which still holds the last good record - untouched, so
// _read() recovers it on the next boot instead of losing everything.
#define FLASH1_MAGIC 0xB10CF0C1UL
#define FLASH1_VERSION 3 // Bump whenever the EEPROMFUNCTION::read()/write() field order/count changes,
                          // or the header layout changes, so a record from the previous layout fails
                          // validation cleanly (-> factory reset) instead of being silently misread.
#define FLASH1_HEADER_SIZE 16

class FLASHWRITE1
{
public:
    FLASHWRITE1();
    ~FLASHWRITE1();

    void begin(size_t _size);
    void end();

    // True only when the last _read() found a valid magic/version/CRC record.
    bool valid() const { return _valid; }

    // Address-based template functions
    template <typename T>
    void write(const T &v)
    {
        if (!_write_buffer || _write_offset + sizeof(v) > _total_size) {
            Serial.println("Write buffer overflow!");
            return;
        }
        memcpy(_write_buffer + _write_offset, &v, sizeof(v));
        _write_offset += sizeof(v);
    }

    template <typename T>
    void read(T &v)
    {
        if (!_read_buffer || _read_offset + sizeof(v) > _total_size) {
            Serial.println("Read buffer overflow!");
            return;
        }
        memcpy(&v, _read_buffer + _read_offset, sizeof(v));
        _read_offset += sizeof(v);
    }

    // Address-based string functions
    void writeString(const String &str);
    void readString(String &str);

    

    void commit();
    void erase();

    size_t getWriteOffset() const { return _write_offset; }
    size_t getReadOffset() const { return _read_offset; }
    size_t getTotalSize() const { return _total_size; }

private:
    void _find_start_end_flash_address();
    void _read();
    void _cleanup();
    bool _validate_record(const uint8_t *buf, uint32_t &out_seq);

    size_t FLASH_TOTAL_CHAR;
    size_t FLASH_START_ADDRESS;
    size_t FLASH_END_ADDRESS;

    bool _flash_init;
    bool _valid = false;
    size_t _total_size;
    uint8_t *_write_buffer;
    uint8_t *_read_buffer;
    size_t _write_offset;
    size_t _read_offset;

    size_t _slot_stride;  // sector-aligned distance between slot 0 and slot 1
    int _active_slot;     // which slot _read()/commit() last confirmed valid: 0, 1, or -1
    uint32_t _seq;        // sequence number of the currently active slot

    static uint32_t _crc32(const uint8_t *data, size_t len);

        // Address-based template functions
    template <typename T>
    void write(const T &v, size_t address)
    {
        if (address + sizeof(v) > _total_size) {
            Serial.println("Write buffer overflow!");
            return;
        }
        memcpy(_write_buffer + address, &v, sizeof(v));
        // Don't update _write_offset for address-based writes
    }

    template <typename T>
    void read(T &v, size_t address)
    {
        if (address + sizeof(v) > _total_size) {
            Serial.println("Read buffer overflow!");
            return;
        }
        memcpy(&v, _read_buffer + address, sizeof(v));
        // Don't update _read_offset for address-based reads
    }
};

extern FLASHWRITE1 Flash1;
#endif