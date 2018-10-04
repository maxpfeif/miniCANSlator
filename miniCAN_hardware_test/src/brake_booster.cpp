#include "brake_booster.h"

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

volatile bool booster = false; 
// FOR TESTING -- we will use 1.25V as the 'turn on' threshold, so 1024/2 = 512, should be cut-in. 

static int cut_out_threshold = 100;		// example value -- change in practice  
static int cut_in_threshold = 0; 		// example value -- change in practice 

bool brake_booster(int boost_pressure) {
	if(boost_pressure < cut_in_threshold) {
		return true;
	} 
	if(boost_pressure > cut_out_threshold){
		return false; 
	}
	return true;
}
