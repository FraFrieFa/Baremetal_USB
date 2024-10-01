
#include "sercom.h"

#include "registers.h"

void sercom_init(sercom_unit* sercom) {
    // enable SERCOM 4
    PB_PMUX4 = 0x33;
    PB_PINCFG8 = 1;
    PB_PINCFG9 = 1;
}
