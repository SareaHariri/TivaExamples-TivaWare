/*
 *  IN THIS EXAMPLE:
 *
 *      WE CONFIGURE THE UART0 MODULE FOR SERIAL COMMUNICATIONS WITH PC
 *      THE USER MAY INPUT r, g, or b FOR OUTPUT ON RGB LED RESPECTIVLY.
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
#include <PORTF_Init.h>

/**************************function prototype*******************************/
void UART0_Init(void) ;


int main(){
      /**************************set the clk on 80********************************/
	  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	  uint32_t data ;

	  /**************************initialize the URAT0*****************************/
	  UART0_Init() ;

	  /**************************initialize the PORTF*****************************/
	  PORTF_Init() ;

	  while(1){
	      /**************************WAIT FOR AVAILABLE DATA**********************/
	      while(!UARTCharsAvail(UART0_BASE)){}

	      /**************************READ ONE CHAR FOR FIFO***********************/
	      data = UARTCharGetNonBlocking(UART0_BASE) ;

	      /*
	       *  WE CAN USE THE FOLLOWING INSTEAD OF THE PREVIOS 2 STEPS
	       *  data = UARTCharGet(UART0_BASE) ;
	       */

	      /**************************OUTPUT ON LEDS*******************************/
	      if(data == 'r'){
	          GPIO_PORTF_DATA_R &= ~0x0e ;
	          GPIO_PORTF_DATA_R |= 0x02 ;
	      }
	      else if (data == 'b'){
	          GPIO_PORTF_DATA_R &= ~0x0e ;
	          GPIO_PORTF_DATA_R |= 0x04 ;
	      }
	      else if (data == 'g'){
	          GPIO_PORTF_DATA_R &= ~0x0e ;
	          GPIO_PORTF_DATA_R |= 0x08 ;
	      }

}
}


void UART0_Init(void) {
	
    /**************************ENABLE CLK ON PORTA*****************************/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA) ;

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){} ;


    /**************************CONFIGURE THE PINS FOR UART COMM****************/
    GPIOPinConfigure(GPIO_PA0_U0RX) ;

    GPIOPinConfigure(GPIO_PA1_U0TX) ;

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1) ;


    /**************************ENABLE CLK ON UART0*****************************/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0) ;

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){} ;

    /**************************DISABLE UART0 FOR CONFIGURATION*****************/
    UARTDisable(UART0_BASE) ;

    /**************************SETTING THE CLK SOURCE**************************/
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM) ;

    /**************************UART SETTING CONFIGURATIONS*********************/
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, UART_CONFIG_WLEN_8|UART_CONFIG_PAR_NONE|UART_CONFIG_STOP_ONE) ;


    /**************************ENABLE URAT0************************************/
    UARTEnable(UART0_BASE) ;

}
