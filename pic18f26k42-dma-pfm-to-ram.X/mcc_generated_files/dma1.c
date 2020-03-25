/**
  DMA Generated Driver File
  
  @Company
    Microchip Technology Inc.

  @File Name
    dma1.c

  @Summary
    This is the generated driver implementation file for the DMA driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for DMA CHANNEL1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC18F26K42
        Driver Version    :  2.10
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB 	          :  MPLAB X 5.20
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 

    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "dma1.h"
#include "interrupt_manager.h"
#include "pin_manager.h"
#include "../app.h"

/**
  Section: Global Variables Definitions
*/

/**
  Section: DMA APIs
*/
void DMA1_Initialize(void)
{
    DMA1SSA = (__uint24)&PWM_DATA;          //set source start address
    DMA1DSA = (uint8_t)&PWM5DCH;           //set destination start address 
    DMA1CON1 = 0x0A;              //set control register1 
    DMA1SSZ = 0x00AC;             //set source size
    DMA1DSZ = 0x0001;             //set destination size
    DMA1SIRQ = 0x1F;              //set DMA Transfer Trigger Source
    DMA1AIRQ = 0x07;              //set DMA Transfer abort Source
    
    PIR2bits.DMA1DCNTIF = 0;      // clear Destination Count Interrupt Flag bit
    PIR2bits.DMA1SCNTIF = 0;      // clear Source Count Interrupt Flag bit
    PIR2bits.DMA1AIF = 0;         // clear abort Interrupt Flag bit
	PIR2bits.DMA1ORIF = 0;        // clear overrun Interrupt Flag bit
    
    PIE2bits.DMA1DCNTIE = 0;      // disable Destination Count 0 Interrupt
    PIE2bits.DMA1SCNTIE = 0;      // disable Source Count Interrupt
    PIE2bits.DMA1AIE = 1;         // enable abort Interrupt
    PIE2bits.DMA1ORIE = 0;        // disable overrun Interrupt 
	
	asm("BCF INTCON0,7");
	
	asm ("BANKSEL PRLOCK");
    asm ("MOVLW 0x55");
    asm ("MOVWF PRLOCK");
    asm ("MOVLW 0xAA");
    asm ("MOVWF PRLOCK");
    asm ("BSF PRLOCK, 0");
	
	asm("BSF INTCON0,7");
        
    DMA1CON0 = 0x80; //set control register0
}



void DMA1_DMAA_ISR(void)
{
    IOCBFbits.IOCBF4 = 0;   //clear the IOC flag for RB4
    PIR2bits.DMA1AIF=0;     //clear abort Interrupt Flag 
    // add your DMA channel 1 abort interrupt custom code
    IO_RA4_SetHigh();       //turn on LED on RA4
}

/**
  End of File
*/