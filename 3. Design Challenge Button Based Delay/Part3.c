/*
 * Part3.c
 *
 *  Created on: Feb 12, 2023
 *      Author: Thomas Rasa
 *
 *      YOU NEED TO FILL IN THIS AUTHOR BLOCK
 */


#include <msp430.h>
    unsigned int delay_count = 8000;

    void gpioInit();
    void timerInit();

    void main(){

        WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

        gpioInit();
        timerInit();

        PM5CTL0 &= ~LOCKLPM5;




        __bis_SR_register(LPM3_bits | GIE);

        while(1){
            P1OUT ^= BIT0;

        }

    }


    void gpioInit(){

        P1OUT &= ~BIT0; //RED LED
        P1DIR |= BIT0;  //Set as output

        ///BUTTONS
        //LEFT BUTTON - reset button
        P4OUT |= BIT1;                          // Configure P4.1 as pulled-up
        P4REN |= BIT1;                          // P2.3 pull-up register enable
        P4IES &= ~BIT1;                         // P2.3 Low --> High edge
        P4IE |= BIT1;                           // P2.3 interrupt enabled
        P4IFG &= 0;

        //RIGHT BUTTON- time capture button
        P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
        P2REN |= BIT3;                          // P2.3 pull-up register enable
        P2IES &= ~BIT3;                         // P2.3 Low --> High edge
        P2IE |= BIT3;                           // P2.3 interrupt enabled
        P2IFG &= 0;

    }

    void timerInit(){

        TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
        TB1CCR0 = delay_count;
        TB1CTL = TBSSEL_1 | MC_1;                 // ACLK, continuous mode

    }


    // Port 2 interrupt service routine
    #pragma vector=PORT2_VECTOR
    __interrupt void Port_2(void)
    {
        P2IFG &= ~BIT3;             // Clear P2.3 IFG
        P1OUT &= ~BIT0;             //turn off LED while capturing time

        while(!P2IN){
            delay_count++;
        }
    }


    // Port 4 interrupt service routine
    #pragma vector=PORT4_VECTOR
    __interrupt void Port_4(void)
    {
        P4IFG &= ~BIT1;                         // Clear P4.1 IFG
        P1OUT ^= BIT0;

    }


    // Timer B1 interrupt service routine
    #pragma vector = TIMER1_B0_VECTOR
    __interrupt void Timer1_B0_ISR(void)
    {
        // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.
        P1OUT ^= BIT0;


    }
