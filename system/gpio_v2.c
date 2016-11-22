#include <xinu.h>

/**
 * \param  baseAdd    The memory address of the GPIO instance being used
 * \param  pinNumber     The number of the pin in the GPIO instance
 * \param  pinDirection  The required direction for the GPIO pin
 * \param  pinValue    This specifies whether a logic HIGH or a logic LOW
 *                     should be driven on the output pin
 *
 * 'pinNumber' can take one of the following values:
 * (0 <= pinNumber <= 31)\n
 *
 * 'pinDirection' can take one of the following values:
 * - GPIO_DIR_INPUT - to configure the pin as an input pin\n
 * - GPIO_DIR_OUTPUT - to configure the pin as an output pin\n

 * 'pinValue' can take one of the following values:
 * - GPIO_PIN_LOW - indicating to drive a logic LOW(logic 0) on the pin.
 * - GPIO_PIN_HIGH - indicating to drive a logic HIGH(logic 1) on the pin.
*/


/*
 * \brief  This API performs the module reset of the GPIO module. It also
 *         waits until the reset process is complete.
 * \return None
 */

void GPIOModuleReset(unsigned int baseAdd)
{
    /*
    ** Setting the SOFTRESET bit in System Configuration register.
    ** Doing so would reset the GPIO module.
    */
    HWREG(baseAdd + GPIO_SYSCONFIG) |= (GPIO_SYSCONFIG_SOFTRESET);

    /* Waiting until the GPIO Module is reset.*/
    while(!(HWREG(baseAdd + GPIO_SYSSTATUS) & GPIO_SYSSTATUS_RESETDONE));
}

/**
 * \brief  This API is used to enable the GPIO module. When the GPIO module
 *         is enabled, the clocks to the module are not gated.

 * \note   Enabling the GPIO module is a primary step before any other
 *         configurations can be done.
 */

void GPIOModuleEnable(unsigned int baseAdd)
{
    /* Clearing the DISABLEMODULE bit in the Control(CTRL) register. */
    HWREG(baseAdd + GPIO_CTRL) &= ~(GPIO_CTRL_DISABLEMODULE);
}

void GPIOModuleDisable(unsigned int baseAdd)
{
    /* Setting the DISABLEMODULE bit in Control(CTRL) register. */
    HWREG(baseAdd + GPIO_CTRL) |= (GPIO_CTRL_DISABLEMODULE);
}

/**
 * \brief  This API configures the direction of a specified GPIO pin as being
 *         either input or output.
 */

void GPIODirModeSet(unsigned int baseAdd,
                    unsigned int pinNumber,
                    unsigned int pinDirection)
{
    /* Checking if pin is required to be an output pin. */
    if(GPIO_DIR_OUTPUT == pinDirection)
    {
        HWREG(baseAdd + GPIO_OE) &= ~(1 << pinNumber);
    }
    else
    {
        HWREG(baseAdd + GPIO_OE) |= (1 << pinNumber);
    }
}

void GPIOPinWrite(unsigned int baseAdd,
                  unsigned int pinNumber,
                  unsigned int pinValue)
{
    if(GPIO_PIN_HIGH == pinValue)
    {
        HWREG(baseAdd + GPIO_SETDATAOUT) = (1 << pinNumber);
    }
    else
    {
        HWREG(baseAdd + GPIO_CLEARDATAOUT) = (1 << pinNumber);
    }
}

unsigned int GPIOPinRead(unsigned int baseAdd,
                         unsigned int pinNumber)
{
    return(HWREG(baseAdd + GPIO_DATAIN) & (1 << pinNumber));
}



//---------------------------------------------------------------------------------------------------------------------------------------
//https://github.com/embest-tech/AM335X_StarterWare_02_00_01_01/blob/master/platform/bbblack/gpio.c
//#include "hw_control_AM335x.h"
//#include "hw_cm_per.h"
//#include "soc_AM335x.h"

void GPIO1ModuleClkConfig(void)
{

    /* Writing to MODULEMODE field of CM_PER_GPIO1_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) |=
            CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) &
           CM_PER_GPIO1_CLKCTRL_MODULEMODE));
    /*
    ** Writing to OPTFCLKEN_GPIO_1_GDBCLK bit in CM_PER_GPIO1_CLKCTRL
    ** register.
    */
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) |=
            CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK;

    /*
    ** Waiting for OPTFCLKEN_GPIO_1_GDBCLK bit to reflect the desired
    ** value.
    */
    while(CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) &
           CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK));

    /*
    ** Waiting for IDLEST field in CM_PER_GPIO1_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_GPIO1_CLKCTRL_IDLEST_FUNC <<
                                            CM_PER_GPIO1_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) &
           CM_PER_GPIO1_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_GPIO_1_GDBCLK bit in CM_PER_L4LS_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_1_GDBCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
           CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_1_GDBCLK));
}

/**
 * \brief  This function does the Pin Multiplexing and selects GPIO pin
 *         GPIO1[23] for use. GPIO1[23] means 23rd pin of GPIO1 instance.
 *         This pin can be used to control the Audio Buzzer.
 *
 * \note   Either of GPIO1[23] or GPIO1[30] pins could be used to control the
 *         Audio Buzzer.
 */

void GPIO1Pin23PinMuxSetup(void)
{
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_A(7)) = CONTROL_CONF_MUXMODE(7);
}

/*
** This function enables GPIO1 pins
*/
void GPIO1PinMuxSetup(unsigned int pinNo)
{
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(pinNo)) =
            (CONTROL_CONF_GPMC_AD_CONF_GPMC_AD_SLEWCTRL | 	/* Slew rate slow */
             CONTROL_CONF_GPMC_AD_CONF_GPMC_AD_RXACTIVE |	/* Receiver enabled */
             (CONTROL_CONF_GPMC_AD_CONF_GPMC_AD_PUDEN & (~CONTROL_CONF_GPMC_AD_CONF_GPMC_AD_PUDEN)) | /* PU_PD enabled */
             (CONTROL_CONF_GPMC_AD_CONF_GPMC_AD_PUTYPESEL & (~CONTROL_CONF_GPMC_AD_CONF_GPMC_AD_PUTYPESEL)) | /* PD */
             (CONTROL_CONF_MUXMODE(7))	/* Select mode 7 */
            );
}

/**
 * \brief  This function does the pin multiplexing for any GPIO Pin.
 *
 * \param  offsetAddr   This is the offset address of the Pad Control Register
 *                      corresponding to the GPIO pin. These addresses are
 *                      offsets with respect to the base address of the
 *                      Control Module.
 * \param  padConfValue This is the value to be written to the Pad Control
 *                      register whose offset address is given by 'offsetAddr'.
 *
 * The 'offsetAddr' and 'padConfValue' can be obtained from macros defined
 * in the file 'include/armv7a/am335x/pin_mux.h'.\n
 *
 * \return  None.
 */
void GpioPinMuxSetup(unsigned int offsetAddr, unsigned int padConfValue)
{
    HWREG(SOC_CONTROL_REGS + offsetAddr) = (padConfValue);
}