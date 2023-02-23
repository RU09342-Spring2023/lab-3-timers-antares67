/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Russell Trafford
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>

char speedTracker = 0;
char offset = 0;

void gpioInit();
void timerInit();

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();


    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(GIE); //LPM3_bits |

}


void gpioInit(){
    // @TODO Initialize the Red or Green LED
        P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
        P6DIR |= BIT6;                          // Set P6.6 to output direction

    // @TODO Initialize Button 2.3
        P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
        P4REN |= BIT1;                          // P2.3 pull-up register enable
        P4IES &= ~BIT1;                         // P2.3 Low --> High edge
        P4IE |= BIT1;                           // P2.3 interrupt enabled
        P4IFG &= 0;



}

void timerInit(){
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on
        TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
        TB1CCR0 = 50000;
        TB1CTL = TBSSEL_1 | MC_2;                 // ACLK, continuous mode



}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.
    P4IFG &= 0;
    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.

    switch(speedTracker){
        case 0:
            speedTracker = 1;
            offset = 25000;
        break;
        case 0x01:
            speedTracker = 2;
            offset = 5000;
        break;
        case 0x02:
            speedTracker = 0;
            offset = 50000;
        break;

    }


}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.
    P6OUT ^= BIT6;
    TB1CCR0 += offset;

}


