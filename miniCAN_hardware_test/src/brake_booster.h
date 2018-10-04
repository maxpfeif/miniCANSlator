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


// returns the state of the booster when passed the sensor value 
bool brake_booster(int boost_pressure); 

#endif 