// PRUSS program to flash 2 LEDs on P8_11 (pru0_pru_r30_15) and P8_12
// (pru0_pru_r30_14) alternating at a determined frequency
// When a button that is connected to P9_25 (pru0_pru_r31_7) is pressed
// the frequency increases. When a button that is connected to P9_27
// (pru0_pru_r31_5) is pressed the frequency decreases.

.origin 0			// start of program in PRU memory
.entrypoint MAIN		// program entry point (for a debugger)

#define INS_PER_US   200	// 5ns per instruction
#define INS_PER_DELAY_LOOP 2	// two instructions per delay loop
				// default delay is 50 ms
#define DELAYINC 10 * 1000 * (INS_PER_US / INS_PER_DELAY_LOOP) 
#define DELAY    50 * 1000 * (INS_PER_US / INS_PER_DELAY_LOOP)
#define DELAYMAX 1000 * 1000 * (INS_PER_US / INS_PER_DELAY_LOOP)

#define PRU0_R31_VEC_VALID 32    // allows notification of program completion
#define PRU_EVTOUT_0    3        // the event number that is sent back

MAIN:
	MOV	r1, DELAY	// store the default length of the delay in REG1
	MOV	r2, DELAYINC	// store the value of delay increment/decrement
	MOV	r3, DELAYMAX	// store the value of maximum delay
	SET	r30.t15		// turn off GREEN LED
	CLR	r30.t14		// turn on RED LED
START:
	QBBS	GREEN_ON, r30.t15
	SET	r30.t15		// turn off GREEN LED
RED_TOGGLE:
	QBBS	RED_ON, r30.t14
	SET	r30.t14		// turn off RED LED
TOGGLE_END:
	QBBS	FREQ_UP_CONT, r31.t7	// if button P9_25 pressed, increase frequency
	SUB	r1, r1, r2	// decrease delay DELAYINC
	QBLT	FREQ_UP_CONT, r1, r2	// make sure delay is greater than DELAYINC
	MOV	r1, r2
FREQ_UP_CONT:
	QBBS	FREQ_DOWN_CONT, r31.t5	// if button P9_27 pressed, decrease frequency
	ADD	r1, r1, r2	// increase delay 10ms
	QBGT	FREQ_DOWN_CONT, r1, r3	// make sure delay is lesser than DELAYMAX
	MOV	r1, r3
FREQ_DOWN_CONT:
	MOV	r0, r1		// store the length of the delay in REG0
DELAYLOOP:
	SUB	r0, r0, 1	// Decrement REG0 by 1
	QBNE	DELAYLOOP, r0, 0	// Loop to DELAYON, unless REG0=0
	QBA	START		// loop forever
END:				// notify the calling app that finished
	MOV	R31.b0, PRU0_R31_VEC_VALID | PRU_EVTOUT_0
	HALT			// halt the pru program

RED_ON:
	CLR	r30.t14		// turn on RED LED
	QBA	TOGGLE_END

GREEN_ON:
	CLR	r30.t15		// turn on GREEN LED
	QBA	RED_TOGGLE
