/*	IN THIS EXAMPLE:
 *
 *      WE USE PF4 AS AN INPUT SW AND PF2 AS AN OUTPUT.
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





int main(){
      // seting the clk on 80 MHz
	  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	  // enable the portf peripheral
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF) ;

	  // delay for stable clk
	  SysCtlDelay(6) ;

	  // PF2 OUTPUT ... PF4 INPUT
	  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2) ;
	  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4) ;

	  // 2mA str .. pull up R enabled
	  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU) ;

	  bool Pressed  = 0 ;
	  uint8_t status = 0  ;



	  while(1){

	      status = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) ;
	      if(!(status == GPIO_PIN_4)){
	          SysTick80_Delay_10ms(5) ; // Delay to reduce the bouncing effect
	          if(!Pressed){
	          GPIO_PORTF_DATA_R  ^= 0x04  ;
	          Pressed = 1 ;
	      }

	      }
	      else
	          Pressed =  0 ;

}
}
