/*  main.c  - main */

/*
 *   author: Nimish
 */

#include <xinu.h>

#define GPIO_INSTANCE_ADDRESS           (SOC_GPIO_1_REGS)
#define GPIO_INSTANCE_PIN_NUMBER1        (12)   //P8_12
#define GPIO_INSTANCE_PIN_NUMBER2        (14)   //P8_16
#define LED 16


process main(void){
    recvclr();

    int i=5000,on_off=0,toggle=0;
    char onbuf[1] = {'1'}, offbuf[1]={'0'};
    int count=1;

    /* Enabling functional clocks for GPIO1 instance. */
    kprintf("Start: GPIO1ModuleClkConfig()");
    //GPIO1ModuleClkConfig();

    /* Selecting GPIO1[23] pin for use. */
    kprintf("Start: GPIO1Pin23PinMuxSetup()");
    //GPIO1Pin23PinMuxSetup();
    //GPIO1PinMuxSetup(GPIO_INSTANCE_PIN_NUMBER);

    /* Enabling the GPIO module. */
    kprintf("Start: GPIOModuleEnable()");
    //GPIOModuleEnable(GPIO_INSTANCE_ADDRESS);

    /* Resetting the GPIO module. */
    kprintf("Start: GPIOModuleReset()");
    //GPIOModuleReset(GPIO_INSTANCE_ADDRESS);

    /* Setting the GPIO pin as an output pin. */
    kprintf("Start: GPIODirModeSet()");
//    GPIODirModeSet(GPIO_INSTANCE_ADDRESS,
//                   GPIO_INSTANCE_PIN_NUMBER1,
//                   GPIO_DIR_OUTPUT);
//
//    GPIODirModeSet(GPIO_INSTANCE_ADDRESS,
//                   GPIO_INSTANCE_PIN_NUMBER2,
//                   GPIO_DIR_INPUT);

    while(i)
    {//        kprintf("%d",i);
        /* Driving a logic HIGH on the GPIO pin. */
        write(LED, onbuf, count);

 sleep(1);
        /* Driving a logic LOW on the GPIO pin. */
        write(LED, offbuf, count);
 sleep(1);
    }
    kprintf("\n\nEND\n");
    return OK;
}


/*Blink LED */
/*while(i)
    {
//        kprintf("%d",i);
        /* Driving a logic HIGH on the GPIO pin. */
//        GPIOPinWrite(GPIO_INSTANCE_ADDRESS,
//                     GPIO_INSTANCE_PIN_NUMBER1,
//                     GPIO_PIN_HIGH);

// sleep(1);
//        /* Driving a logic LOW on the GPIO pin. */
//        GPIOPinWrite(GPIO_INSTANCE_ADDRESS,
//                     GPIO_INSTANCE_PIN_NUMBER1,
//                     GPIO_PIN_LOW);
// sleep(1);
//}
// */


/*Blink LED in buttonpush*/
//while(i)
//{
//GPIOPinWrite(GPIO_INSTANCE_ADDRESS,
//GPIO_INSTANCE_PIN_NUMBER1,
//on_off);
//
//on_off = GPIOPinRead(GPIO_INSTANCE_ADDRESS, GPIO_INSTANCE_PIN_NUMBER2); //on=(1<<14)
//on_off>>=GPIO_INSTANCE_PIN_NUMBER2;
//kprintf("%d",on_off);
//}