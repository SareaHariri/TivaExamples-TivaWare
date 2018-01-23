/*
 * IN THIS EXAMPLE:
 * WE CONNECT ONE LAUNCHPAD TO ANOTHER USING UART1 MODULE IN EACH.
 * UART1 MODULE:
 *  RX-PB0
 *  TX-PB1
 * THE CODE BELOW SHOULD BE FLASHED ON THE BOTH OF BOARDS.
 * THE USER CAN PUSH ANY OF 2 SWITCHS ON THE BOARD TO TURN ON THE LED ON THE OTHER BOARD.
 *
 *            WIRING:
 *            *****************************
 *            *      TIVA 1      TIVA 2   *
 *            *      PB0---------PB1      *
 *            *      PB1---------PB0      *
 *            *****************************
 *
 *      Sarea Al Hariri
 *
 *
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
#include <tm4c123gh6pm.h>

/**********************************function prototype****************************************/
void UART1_Init(void) ;

void PORTF_Init(void) ;

void PORTF_Isr(void) ;



int main(){
      /*******************************set clk on 80 MHZ****************************************/
	  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ) ;

	  /*******************************UART1 Initializations************************************/
	  UART1_Init() ;

	  /*******************************PORTF Initializations************************************/
	  PORTF_Init() ;

	  /* Declaring a variable to store the recieved char*/
	  uint32_t ui32Data ;


	  while (1) {
	      /* wait for available data*/
	      while(!UARTCharsAvail(UART1_BASE)){}

	      /* Get the recieved character*/
	      ui32Data = UARTCharGet(UART1_BASE) ;

	      /*
	       * if the recieved char is 'r' .. turn R led on
	       * if the recieved char is 'g' .. turn G led on
	       */

	      if(ui32Data == 'r'){
	          GPIO_PORTF_DATA_R &= ~0x0e ;
	          GPIO_PORTF_DATA_R |= 0x02 ;
	      }
	      else if (ui32Data == 'b'){
              GPIO_PORTF_DATA_R &= ~0x0e ;
              GPIO_PORTF_DATA_R |= 0x08 ;
	      }
	  }
}


/*******************************Functions Definitions************************************/
/****************************************************************************************/


/*******************************UART1 Initializations************************************/
void UART1_Init(void){

    /**************************ENABLE CLK ON PORTA*****************************/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB) ;

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){} ;


    /**************************CONFIGURE THE PINS FOR UART COMM****************/
    GPIOPinConfigure(GPIO_PB0_U1RX) ;

    GPIOPinConfigure(GPIO_PB1_U1TX) ;

    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1) ;


    /**************************ENABLE CLK ON UART0*****************************/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1) ;

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1)){} ;

    /**************************DISABLE UART0 FOR CONFIGURATION*****************/
    UARTDisable(UART1_BASE) ;

    /**************************SETTING THE CLK SOURCE**************************/
    UARTClockSourceSet(UART1_BASE, UART_CLOCK_SYSTEM) ;

    /**************************UART SETTING CONFIGURATIONS*********************/
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600, UART_CONFIG_WLEN_8|UART_CONFIG_PAR_NONE|UART_CONFIG_STOP_ONE) ;


    /**************************ENABLE URAT0************************************/
    UARTEnable(UART1_BASE) ;

}
/****************************************************************************************/


/*******************************PORTF Initializations************************************/
void PORTF_Init(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF) ;
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) ;

    GPIO_PORTF_LOCK_R = 0x4C4F434B ;
    GPIO_PORTF_CR_R |= 0x01 ;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4) ;

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3) ;

    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU) ;

    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_INT_PIN_0|GPIO_PIN_4, GPIO_FALLING_EDGE) ;

    GPIOIntRegister(GPIO_PORTF_BASE, PORTF_Isr) ;

    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4) ;

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0|GPIO_INT_PIN_4) ;


}
/****************************************************************************************/


/*******************************PORTF Handler********************************************/
void PORTF_Isr(void){
    uint32_t ui32IntFlags ;
    ui32IntFlags = GPIOIntStatus(GPIO_PORTF_BASE, true) ;

    if(ui32IntFlags == GPIO_INT_PIN_4){
        UARTCharPut(UART1_BASE, 'r') ;
    }
    else if (ui32IntFlags == GPIO_INT_PIN_0){
        UARTCharPut(UART1_BASE, 'b') ;
    }
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_0|GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4) ;
}
/****************************************************************************************/
