#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.


#define PB_DEBOUNCE_TIME 5 //timer ticks at 10ms, so 5 = 50ms
#define PRE_AUTO_TIMER_MAX 100 // hold button for 1s
#define AUTO_TIMER_MAX 50 // increment 2x per s
#define TIMER_SEC 100
#define NUMBER_OF_BUTTONS 5
// This is an example of an enumerated type I like to use for state machines. Here for reference...
enum buttonHandler_st_t {
	init_st,
	no_touch_st,
	wait_st,
	hold_st,
	auto_rate_st,
	final_st,
} buttonState[NUMBER_OF_BUTTONS] = {init_st, init_st, init_st, init_st, init_st};


const int BTN_MASKS[] = {0x01,0x02,0x04,0x08,0x10};
#define BTN_HR 3
#define BTN_MIN 0
#define BTN_SEC 1
#define BTN_DOWN 2
#define BTN_UP 4

#define MINSEC_MAX 59
#define HR_MAX 23


// Don't know if we are allowed to add any libraries so I manually define bool for kicks and giggles:
typedef int bool;
#define TRUE 1
#define FALSE 0



int timerCount = 0;
u32 buttonStateReg; // Read the button values with this variable


XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

int hour = 0;
int minute = 0;
int second = 0;
bool scrollDisable = FALSE;

void increment_clock() {
	if(!buttonStateReg) { // only increment time when not setting clock
		second++;
		if(second > MINSEC_MAX) {
			second = 0;
			minute++;
			if(minute > MINSEC_MAX) {
				minute = 0;
				hour++;
				if(hour > HR_MAX) {
					hour = 0;
				}
			}
		}
	}
}

void print_clock() {
	if(scrollDisable) {
		xil_printf("\b\b\b\b\b\b\b\b");
	}
	xil_printf("\r%02d:%02d:%02d",hour,minute,second);
}

void reset_time() {
	bool time_changed = FALSE;
	if(buttonStateReg & BTN_MASKS[BTN_HR]) { // SET HOUR
		if(buttonStateReg & BTN_MASKS[BTN_UP]) { // INCREMENT HOUR
			hour++;
			time_changed = TRUE;
			if(hour > HR_MAX) hour = 0; // wrap-around at 23 for military time
		}
		if(buttonStateReg & BTN_MASKS[BTN_DOWN]) { // DECREMENT HOUR
			hour--;
			time_changed = TRUE;
			if(hour < 0) hour = HR_MAX; // wrap-around to 23 for military time
		}
	}

	if(buttonStateReg & BTN_MASKS[BTN_MIN]) { // SET MINUTE
			if(buttonStateReg & BTN_MASKS[BTN_UP]) { // INCREMENT MINUTE
				minute++;
				time_changed = TRUE;
				if(minute > MINSEC_MAX) minute = 0; // wrap-around
			}
			if(buttonStateReg & BTN_MASKS[BTN_DOWN]) { // DECREMENT MINUTE
				minute--;
				time_changed = TRUE;
				if(minute < 0) minute = MINSEC_MAX; // wrap-around
			}
		}

	if(buttonStateReg & BTN_MASKS[BTN_SEC]) { // SET SECOND
			if(buttonStateReg & BTN_MASKS[BTN_UP]) { // INCREMENT SECOND
				second++;
				time_changed = TRUE;
				if(second > MINSEC_MAX) second = 0; // wrap-around
			}
			if(buttonStateReg & BTN_MASKS[BTN_DOWN]) { // DECREMENT SECOND
				second--;
				time_changed = TRUE;
				if(second < 0) second = MINSEC_MAX; // wrap-around to 23 for military time
			}
		}
	if(time_changed) print_clock();
}


// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void timer_interrupt_handler() {
	int i;
	bool time_set = FALSE;

	int debounce_timer[NUMBER_OF_BUTTONS];
	int pre_auto_timer[NUMBER_OF_BUTTONS];
	int auto_timer[NUMBER_OF_BUTTONS];

	if(timerCount>=TIMER_SEC) {
		increment_clock();
		print_clock();

		timerCount=0;
	} else {
		timerCount++;
	}

	for(i = 0; i < NUMBER_OF_BUTTONS; i++) {
		//print("*");
	// switch for state actions
	switch(buttonState[i]) {
	case init_st:
		break;
	case no_touch_st:
		break;
	case wait_st:
		debounce_timer[i]++;
		break;
	case hold_st:
		if(i == BTN_UP || i == BTN_DOWN) {
			pre_auto_timer[i]++;
		}
		break;
	case auto_rate_st:
		auto_timer[i]++;
		break;
	case final_st:
		//debounce_timer[i]++;
		break;
	}

	//switch for state transitions,
	// including mealy actions.
	switch(buttonState[i]) {
	case init_st:
		debounce_timer[i] = 0;
		buttonState[i] = no_touch_st;
		pre_auto_timer[i] = 0;
		auto_timer[i] = 0; // starting at max so that it will change time, then wait 0.5s
		break;
	case no_touch_st:
		if(buttonStateReg & BTN_MASKS[i]) // some button is pushed
			buttonState[i] = wait_st;
		break;
	case wait_st:

		if(debounce_timer[i] >= PB_DEBOUNCE_TIME) {// timer expired
			buttonState[i] = hold_st;
			debounce_timer[i] = 0;
			//xil_printf("%d",buttonStateReg);
			if(!time_set) {
				reset_time();
				time_set = TRUE; // makes sure that reset_time is called only once per interrupt cycle
			}
		// mealy action to increment/decrement values
		}
		else if(!(buttonStateReg & BTN_MASKS[i])) {
			debounce_timer[i] = 0;
			buttonState[i] = no_touch_st;
		}
		break;
	case hold_st:

		if(pre_auto_timer[i] >= PRE_AUTO_TIMER_MAX && (i == BTN_UP || i == BTN_DOWN)) { // only up and down should trigger hold actions.
			pre_auto_timer[i] = 0;
			buttonState[i] = auto_rate_st;
		}
		if(!(buttonStateReg & BTN_MASKS[i])) // the button is no longer pushed
			buttonState[i] = final_st;
		break;
	case auto_rate_st:
		print("auto rate\n");

		if(auto_timer[i] >= AUTO_TIMER_MAX) {

			auto_timer[i] = 0;
			if(!time_set) {
				reset_time();
				//xil_printf("change btn %d\n\r",i);
				time_set = TRUE;
			}
		}
		if(!(buttonStateReg & BTN_MASKS[i])) {
			buttonState[i] = final_st;
		}
	case final_st:
		debounce_timer[i] = 0;
		pre_auto_timer[i] = 0;
		auto_timer[i] = 0; // starting at max so that it will change time, then wait 0.5s
		buttonState[i] = no_touch_st;
		break;
	}
	}

}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
  // Clear the GPIO interrupt.
  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
  u32 currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
  // You need to do something here

  // Button implementation (Colt)
  /*
   *  buttonStateReg will store the button states in the lower 5 bits.
   *  If you just print the value you will get:
   *  1 - center button
   *  2 - right button
   *  4 - down button
   *  8 - left button
   *  16 - up button
   *
   */
   // shows which button is being pressed

  // The lab requires us to not directly poll the buttons. We will do this by storing the value of the states into
  // buttonStateReg. Maybe we should have a flag variable that indicates a change in the buttons...
  buttonStateReg = currentButtonState; // Save the button state and then do something with it

  XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
  XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
// Question: Why is the timer_interrupt_handler() called after ack'ing the interrupt controller
// but pb_interrupt_handler() is called before ack'ing the interrupt controller?
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		pb_interrupt_handler();
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
	}
}

int main (void) {
    init_platform();
    // Initialize the GPIO peripherals.
    int success;
    //print("dude, this does something\n\r");
    success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
    // Set the push button peripheral to be inputs.
    XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
    // Enable the global GPIO interrupt for push buttons.
    XGpio_InterruptGlobalEnable(&gpPB);
    // Enable all interrupts in the push button peripheral.
    XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

    microblaze_register_handler(interrupt_handler_dispatcher, NULL);
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
    		(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
    microblaze_enable_interrupts();

    while(1);  // Program never ends.

    cleanup_platform();

    return 0;
}
