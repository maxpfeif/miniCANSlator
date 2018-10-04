/*	The brake booster module should be passed the sense value and return 
*	the desired state of the pump. 
*
*	Cut-in and cut-out values should be hard-coded into this, and not
*	changed by any sort of config file. 
* 	
*	Logic is;
*		if sensor < cut_in_threshold, booster = on
*		if sensor > cut_out_threshold, booster = off 
*
*	Booster should be initialized to off. 
*/

#ifndef BRAKE_BOOSTER_H
#define BRAKE_BOOSTER_H

// FOR TESTING -- we will use 1.25V as the 'turn on' threshold, so 1024/2 = 512, should be cut-in. 

static int cut_out_threshold = 512;		// example value -- change in practice  
static int cut_in_threshold = 0; 		// example value -- change in practice 

// returns the state of the booster when passed the sensor value 
bool brake_booster(uint8_t boost_pressure); 

#endif 