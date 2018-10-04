#include "brakeBooster.h"

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

volatile bool booster = FALSE; 

bool brake_booster(uint8_t boost_pressure){
	if(boost_pressure < cut_in_threshold) {
		booster = TRUE;
	} 
	if(boost_pressure > cut_out_threshold){
		booster = FALSE;
	}
	return booster;
}
