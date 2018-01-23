/*************** LED BLINKING ************/
/*
* 	LED blinking using the systick timer for delay 
*	
*	Sarea Al Hariri
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




int main(){
	  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF) ; // enable the port f clocking
	  SysCtlDelay(5) ;                              // delay for 5*3 cycle
	  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE , GPIO_PIN_3) ; // set portf1 as outut

	  while(1){
	      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3) ;
	      SysTick80_Delay_10ms(100) ;
	      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0) ;
	      SysTick80_Delay_10ms(100) ;
	  }

}
