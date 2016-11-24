//To setup a digital pin as an output, set the output value HIGH, and then cleanup after you're done:
//
//GPIO.setup("P8_10", GPIO.OUT)
//GPIO.output("P8_10", GPIO.HIGH)
//GPIO.cleanup()


//GPIO.setup("P8_14", GPIO.IN)
//Once you've set pins as inputs, you can access the input value in a few different ways. The first, and easiest
// way is just polling the inputs, such as in a loop that keeps checking them:

//if GPIO.input("P8_14"):
//print("HIGH")
//else:
//print("LOW")

#include <xinu.h>

#define GPIO_INSTANCE_PIN_NUMBER        (12)

devcall	actwrite(struct dentry* devptr,char* buff, int32 count) {
    if(count!=1) {
        return 0;
    }

    unsigned int baseaddress =  *((unsigned int*)(&devptr->dvcsr)); //get the register start address

    /* Selecting GPIO1[pin] pin for use. */
    GPIO1PinMuxSetup(GPIO_INSTANCE_PIN_NUMBER);

    /* Setting the GPIO pin as an output pin. */
    GPIODirModeSet(baseaddress,
                   GPIO_INSTANCE_PIN_NUMBER,
                   GPIO_DIR_OUTPUT);


    if(buff[0]=='1') {
        GPIOPinWrite(baseaddress,
                     GPIO_INSTANCE_PIN_NUMBER,
                     GPIO_PIN_HIGH);
        return 1;
    }
    else if(buff[0]=='0') {
        GPIOPinWrite(baseaddress,
                     GPIO_INSTANCE_PIN_NUMBER,
                     GPIO_PIN_LOW);
        return 1;
    }

    return 0;
}

//
//devcall	ttywrite(
//        struct dentry	*devptr,	/* Entry in device switch table	*/
//        char	*buff,			/* Buffer of characters		*/
//        int32	count 			/* Count of character to write	*/
//)
//{
//    /* Handle negative and zero counts */
//
//    if(count!=1)    //1 bit at a time
//    {
//        return SYSERR;
//    }
//
//    if(GPIO_PIN_HIGH == pinValue)
//    {
//        HWREG(baseAdd + GPIO_SETDATAOUT) = (1 << pinNumber);
//    }
//    else
//    {
//        HWREG(baseAdd + GPIO_CLEARDATAOUT) = (1 << pinNumber);
//    }
//
//
//    return OK;
//}
