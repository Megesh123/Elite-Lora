/**********************
 * RESERVOIR MONITORING HEADER
 * Defines interface for reservoir level monitoring and management
 * Handles refill percentage calculation and low level alerts
 **********************/

#ifndef __RESERVOIR_H__
#define __RESERVOIR_H__

/**********************
 * RESERVOIRFUNCTION CLASS
 * Manages reservoir level monitoring and refill tracking
 **********************/
class RESERVOIRFUNCTION
{
public:
    void read(); // Read reservoir level from sensor
};

extern RESERVOIRFUNCTION Reservoir; // Global instance of RESERVOIRFUNCTION

#endif /* __RESERVOIR_H__ */