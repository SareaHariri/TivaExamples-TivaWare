/*
 * IN THIS EXAMPLE:
 *      WE INTERFACE THE RGB LED AND THE SWITCH OF PORTF
 *      AND USE THE INTERRUPT CONCEPT TO DETECT THE SW STATUS.
 *
 *      Sarea Al Hariri
 */

#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include <string.h>
#include <SysTick_Init.h>



void GPIO_PORtF_Handler(void);



int main(){
	  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF) ;             // enable PORTF
	  SysCtlDelay(5) ;                                          // delay for stable clocking
	  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_1) ;      // PF2 output
	  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4) ;       // PF4 input
	  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU) ;
	  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4) ;          // interrupt enable
	  GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL) ; // low level interrupt
	  GPIOIntRegister(GPIO_PORTF_BASE, GPIO_PORtF_Handler) ;     // dynamic isr registering


	  while (1){
	      // you can UNcomment the following 4 lines instead of direct register access code
	      /*
	      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_3, GPIO_PIN_2) ;
	      SysTick80_Delay_10ms(100) ;
	      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_3, 0) ;
	      SysTick80_Delay_10ms(100) ;
	      */


	      /*************direct register accsessing************************/

	      GPIO_PORTF_DATA_R |= 0x04 ; // blue output
	      SysTick80_Delay_10ms(100) ; // delay 1 second
	      GPIO_PORTF_DATA_R &= ~0x0E ; // no output
	      SysTick80_Delay_10ms(100) ; // delay 1 second
	  }
}



void GPIO_PORtF_Handler(void) {
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4) ;
    GPIOPinWrite(GPIO_PORTF_BASE, 0x0e, 0x0e) ;

}
