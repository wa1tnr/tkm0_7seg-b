// Saturday, 11 Dec 2021  22:06:19z
// five LEDS  see hw_testing
// Decent port of the POV demo stuff.

const int latchPin = 2;  /* STCP */
const int clockPin = 3;  /* SHCP */
const int dataPin  = 4;  /* DS */

byte leds = 0;
byte uleds = 0;
byte pos = 15; // rightmost

byte slew = 5;

uint8_t ledval = 0;

void _digitSelect(void) {
    uleds = pos;
    shiftOut(dataPin, clockPin, MSBFIRST, uleds);
}

void updateShiftRegister(void) {
    // BS init of 'pos':
    pos = 0; // or 3
    digitalWrite(latchPin, LOW);
    _digitSelect(); // digit 0 1 2 or 3 using 'pos' as the index
     uleds = leds;   // A-F 0-9 and a few other glyphs

    shiftOut(dataPin, clockPin, MSBFIRST, uleds); // paint the character's glyph!

    digitalWrite(latchPin, HIGH);
}

#define EXPOSE_DIGIT_PAINTING -1
#define DURATION 2
#define REPETITIONS 1

void blankleds(void) {
    leds = 0;
    updateShiftRegister();
}

void setleds(void) {
    leds = ledval;
    updateShiftRegister();
    if (!EXPOSE_DIGIT_PAINTING) {
        delay(1); // CRITICAL - must be a finite, non-zero delay here
    } else {
        delay(400);
    }
}

void flash_digit(void) { // paint a single digit brightly, then immediately blank all LEDs
    if (EXPOSE_DIGIT_PAINTING) {
        // delay(122);
        delay(424); // to expose digit change
    }
    setleds();
    blankleds(); // waste no time in doing so!
}

void in_column_zero(void) {
    for (int i = REPETITIONS ; i>0; i--) {
        pos = 15 ; flash_digit();
    }
}

void encode_hw_testing(void) { // 3
    ledval = 1 + 2 + 4 + 8 +  16 +  0 + 0 +   0;
}

void encode_three(void) { // 3
    ledval = 1 + 2 + 4 + 8 +  0 +  0 + 64 +   0;
}

void msg_tttt(void) { // message: '3223'
    for (int j = 2;  j>0; j--) {
        for (int k = DURATION; k>0; k--) {
            //  columns 3 2 1 0  -- painted right to left!
//          encode_three();  in_column_zero();   // print '3' in column '0'
            encode_hw_testing(); in_column_zero();
        }
    }
    delay(1000);
}

int count = -1;
char buffer[8];
int line_reset;

void old_loop() {
}

void setup() {
  Serial1.begin(115200);
  Serial1.println("Begin.");
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);
}

void loop(void) {
    blankleds();
    delay(40);
    ledval = 0;
    int i = 0;
    delay(100);
    // Serial1.println("loop iteration above msg_tttt");
    msg_tttt();

    // hold display blank for a while:
    i = 128; // was 128 and is for blanking not duration.
    ledval = i; in_column_zero(); blankleds();
    delay(2 * (111 + slew));
}

/**********   d o c u m e n t a t i o n   **********/

#if 0

 [ http://www.cplusplus.com/reference/cstdio/snprintf/]

int snprintf ( char * s, size_t n, const char * format, ... );

Write formatted output to sized buffer
Composes a string with the same text that
would be printed if format was used on
printf, but instead of being printed, the
content is stored as a C string in the
buffer pointed by s (taking n as the maximum
buffer capacity to fill).

#endif

#if 0
3V3 is the main 3.3V supply to RP2040 and
its I/O, generated by the on-board SMPS.

This pin can be used to power external circuitry
(maximum output current will depend on
RP2040 load and VSYS voltage,
it is recommended to keep the load on
this pin less than 300mA).
#endif
