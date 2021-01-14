// Activate this, if you want to send from FLASH
#define VAR_IN_PROGMEM

#define NUMBER_OF_REPEATS 3U

#include <IRremote.h>

const char yamahaVolDown[]
#if defined(VAR_IN_PROGMEM) && HAS_FLASH_READ
PROGMEM
#endif
= "0000 006C 0022 0002 015B 00AD " /* Pronto header + start bit */
        "0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 " /* Lower address byte */
        "0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 " /* Upper address byte (inverted at 8 bit mode) */
        "0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 " /* command byte */
        "0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7 " /* inverted command byte + stop bit */
        "015B 0057 0016 0E6C"; /* NEC repeat pattern*/

IRsend irsend;

void setup() {
    Serial.begin(115200);
    while (!Serial)
        ;

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    Serial.print(F("Will send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
}

void loop() {

#if defined(VAR_IN_PROGMEM) && HAS_FLASH_READ
    Serial.println(F("Sending NEC from PROGMEM"));
    irsend.sendPronto_PF(yamahaVolDown, NUMBER_OF_REPEATS);
#else
    Serial.println(F("Sending from normal memory"));
    irsend.sendPronto(yamahaVolDown, NUMBER_OF_REPEATS);
#endif

    delay(2000);
#if HAS_FLASH_READ
    Serial.println(F("Sending the same NEC using the F()-form"));
    irsend.sendPronto(F("0000 006C 0022 0002 015B 00AD " /* Pronto header + start bit */
            "0016 0016 0016 0041 0016 0016 0016 0041 0016 0041 0016 0041 0016 0041 0016 0016 " /* Lower address byte */
            "0016 0041 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 " /* Upper address byte (inverted at 8 bit mode) */
            "0016 0041 0016 0041 0016 0016 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 " /* command byte */
            "0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 05F7 " /* inverted command byte + stop bit */
            "015B 0057 0016 0E6C"), /* NEC repeat pattern*/
    NUMBER_OF_REPEATS);
    delay(2000);
#endif

    // send Nec code aquired by IRreceiveDump.cpp
    Serial.println(F("Sending Nec: address 0xFF00, data 0x15"));
    // 006D -> 38029 Hz
    irsend.sendPronto("0000 006D 0022 0000 015C 00AB " /* Pronto header + start bit */
            "0017 0015 0017 0015 0017 0015 0017 0015 0017 0015 0017 0015 0017 0015 0017 0015 " /* Lower address byte */
            "0017 003F 0017 003E 0017 003F 0017 003E 0017 003F 0015 003F 0017 003F 0015 003F " /* Upper address byte (inverted at 8 bit mode) */
            "0017 003E 0017 0015 0017 003F 0017 0015 0017 003E 0017 0015 0017 0017 0015 0017 " /* command byte */
            "0017 0015 0017 003E 0017 0015 0017 003F 0015 0017 0017 003E 0017 003F 0015 003F 0017 0806" /* inverted command byte + stop bit */
    , 0); // No repeat possible, because of missing repeat pattern

    delay(5000);
}
