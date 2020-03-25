[![MCHP](https://cldup.com/U0qhLwBijF.png)](https://www.microchip.com)
# Introduction
The newer PIC18 family of devices showcase the Direct memory Access (DMA) module. This module can be used to move data within the micro-controller without the CPU. This frees up the CPU to attend to other tasks.

The DMA module on the new PIC micro-controllers allows the user to read data from the Flash memory/EEPROM and the user RAM area and write it to the user RAM area. The DMA module has configurable source and destination addresses and programmable hardware triggers to start and abort the transaction.

On devices that feature the DMA module, the priority of the data buses is decided by a system arbiter. the priorities are programmable by the user and help provide greater flexibility for different types of applications.

# Description
In this example, we will configure the DMA module to read data from a look-up table stored in the flash memory and write it to the PWM duty cycle register. We will also configure a hardware trigger using the TMR0 to initiate one byte transfer every time Timer0 overflows. We will also configure a hardware trigger using one of the pins as inputs to abort the DMA transfer.

# Setup
The DMA module is setup to transfer 172 bytes of data from a look-up table that is stored in the PFM at program time. Timer0 is setup for ~32 ms period and is also a trigger for the DMA to transfer one byte of data. The DMA is setup to continue running until an abort trigger is received. At this point the hardware will automatically clear the start and abort trigger. 

The PWM output is connected to 3 I/O pins (RA7, RA6 and RA5) using peripheral pin select feature of the PIC micro-controllers. These pins are connected to LEDs on the high pin count curiosity board. PIN RA4 is also made an output to indicate the state of the DMA.

We will also have 2 input switches on pin RB4 and RC5 to control the flow of the application.

> Note: For simplicity, I have ignored the lower 2 bits of the PWM duty cycle register and updated the PWMxDCH register to control the duty cycle. 

# MCC settings
Here are the setting in Microchip Code Configurator (MCC) for the DMA module. Open MCC to modify these settings if needed.

### DMA Interrupt settings
In this example we will use the DMA Abort Interrupt to set an I/O pin (RA4) high to indicate that the DMA operation has been aborted. Refer to the interruptmanager.c file for more details. 

![](https://i.imgur.com/oqJ0mpD.jpg){width=auto height=auto align=center}


### DMA Control registers
These are settings for the DMAxCON0 and DMAxCON1 registers. Look at the dma.c file to understand more about these selections.

![](https://i.imgur.com/wuWgr9W.jpg){width=auto height=auto align=center}


### DMA Source Address and Size registers
These are the settings for the source size and address location. The source size is 172 bytes (0x00AC) and we will modify dma.c file to provide the address of the look-up table.

![](https://i.imgur.com/Wfbs56r.jpg){width=auto height=auto align=center}

### DMA Destination Address and Size registers
These are the settings for the destination size and address location. The destination size is 1 byte i.e. PWM5DCH register. We will modify the generated dma.c file to provide the address of the destination register.

![](https://i.imgur.com/GxXzC8D.jpg){width=auto height=auto align=center}

### Other MCC settings
Open MCC to look at the settings for the Timers, PWM and I/O pins.

# Operation

Modify the main.c code as follows. This enables the start and abort triggers. The code in the while(1) loop checks for the SW2 press and re-enables the DMA triggers.

```
void main(void) {
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    DMA1CON0bits.DMA1SIRQEN = 1;
    DMA1CON0bits.DMA1AIRQEN = 1;

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    TMR0_StartTimer();
    DMA1CON0bits.DGO = 1;
    
    while (1) {
        if (!SW2_GetValue()) {
            DMA1CON0bits.DMA1SIRQEN = 1;
            DMA1CON0bits.DMA1AIRQEN = 1;
            IO_RA4_SetLow();

        }
        // Add your application code
    }
}
```

Once programmed LED D3,D4,D5 will start breathing. Pressing SW1 will trigger the DMA abort and stop the LEDs breathing and turn on LED D2 indicating the DMA is in abort state. Pressing SW2 will start the DMA again.

 
 
 


