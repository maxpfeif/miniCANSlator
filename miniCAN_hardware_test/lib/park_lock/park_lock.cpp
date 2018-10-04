#include <park_lock.h>

bool park_lock(bool shift_bcm){
    return ~shift_bcm;
}