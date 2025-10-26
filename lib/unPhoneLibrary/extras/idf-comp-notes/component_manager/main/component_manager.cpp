
#include <stdio.h>
extern "C" {
#include "cmp.h"
}
#include "Arduino.h"
// #include "unphone.h"

extern "C" void app_main(void)
{
    Serial.begin(2000000);
    cmp_hello();
}
