#ifndef PARK_LOCK_H
#define PARK_LOCK_H

/*  This module captures the park lock logic for the CANslator
 *
 *  There are three digital states; two outputs, one input 
 *      SHIFT_OK // the volt shift ok signal, driven low (high) when acceptable to shift 
 *      SHIFT_DRIVE // the output that drives the shift-lock solenoid on the RAM shifter
 *      PARK_DRIVE // drives the park position signal to the BCM -- this is a LSD, driven high by Teensy 
 * 
 *  The SHIFT_OK signal is required to activate the SHIFT_DRIVE signal. Shift drive is the inverse of shift ok.
 *  The Park position sense signal is low when the shift release switch on the Volt shifter is not depressed.
 *  When the shift release switch on the volt shifter is depressed, the line floats, likely driving an internal 
 *  pull-up. For the sake of testing and development this signal need never be driven, and should set the state
 *  as always pressing the release unless this causes problems (i.e. shifting from drive to park) in which case
 *  a state machine would be added to control the signal relative to shift position signals reverse-engineered 
 *  from the RAM shifter.  
 * 
 *  
 */

bool park_lock(bool shift_bcm);

#endif 