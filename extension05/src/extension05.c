/** EX: E5.0

TASK:

Your task is to implement a two-tone siren using the QUTy. You must use
one of the Waveform Outputs of TCA0 to drive the buzzer with a 50 % duty
cycle signal.

Your siren must alternate between the following frequencies:

    f1 = 2AB0 Hz with f1 active for t1 = 3E0 ms
    f2 = 4CD0 Hz with f2 active for t2 = 6F0 ms

where A-F are the 2nd through 7th digits of your student number:

    n*ABCDEF*

EXAMPLE: If your student number was n12345678, then

A = 2, B = 3, C = 4, D = 5, E = 6, F = 7

f1 = 2230 Hz
f2 = 4450 Hz
t1 = 360 ms
t2 = 670 ms

NOTES:

Your program must use either TCA or TCB to time the duration of each
tone. Do not use software based delays.

Your program must be interrupt driven. The main() function expects
init() to return to demonstrate this.

You may define additional variables and functions that are used by
init(), in this file.
*/

// This function is called once by the main() function.
void init(void)
{

}

/** CODE: Write your code for Ex E5.0 above this line. */

/** CODE: Write your code for Ex E5.0 above this line. */
#include <avr/io.h>
#include <avr/interrupt.h>

// Define the student number digits
#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define F 7

// Calculate frequencies and durations
#define F1 ((2000 + A*1000 + B*100 + C*10 + D))  // 2AB0 Hz
#define F2 ((4000 + C*1000 + D*100 + E*10 + F))  // 4CD0 Hz
#define T1 ((300 + E*100))  // 3E0 ms
#define T2 ((600 + F*100))  // 6F0 ms

volatile uint8_t current_tone = 0;

void setup_timers(void) {

    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_FRQ_gc | TCA_SINGLE_CMP0EN_bm;
    TCA0.SINGLE.PER = (F_CPU / (2 * F1)) - 1;
    TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER / 2;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;

    TCB0.CTRLA = TCB_CLKSEL_DIV1_gc | TCB_ENABLE_bm;
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
    TCB0.CNT = 0;
    TCB0.CCMP = F_CPU / 1000 * T1;

    TCB0.INTCTRL = TCB_CAPT_bm;

    sei();
}

ISR(TCB0_INT_vect) {
    TCB0.INTFLAGS = TCB_CAPT_bm;

    if (current_tone == 0) {

        TCA0.SINGLE.PER = (F_CPU / (2 * F2)) - 1;
        TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER / 2;
        TCB0.CCMP = F_CPU / 1000 * T2;
        current_tone = 1;
    } else {

        TCA0.SINGLE.PER = (F_CPU / (2 * F1)) - 1;
        TCA0.SINGLE.CMP0 = TCA0.SINGLE.PER / 2;
        TCB0.CCMP = F_CPU / 1000 * T1;
        current_tone = 0;
    }

    TCB0.CNT = 0;
}

void init(void) {
    // Configure the buzzer pin as output
    PORTB.DIRSET = PIN0_bm;

    // Set up timers and interrupts
    setup_timers();
}
