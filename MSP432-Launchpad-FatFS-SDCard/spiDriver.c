/*
 * spi.c - msp430f5529 launchpad spi interface implementation
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/* DriverLib Includes */
#include "driverlib.h"
#include "spidriver.h"

#define ASSERT_CS()          (P4OUT &= ~BIT6)
#define DEASSERT_CS()        (P4OUT |= BIT6)

/* SPI Master Configuration Parameter */
const eUSCI_SPI_MasterConfig spiMasterConfig =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
        3000000,                                   // SMCLK 3Mhz
        500000,                                    // SPICLK = 500khz
        EUSCI_B_SPI_MSB_FIRST,                     // MSB First
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
        EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
};

int spi_Close(Fd_t fd)
{
    /* Disable WLAN Interrupt ... */
    //CC3100_InterruptDisable();

    return 0;//NONOS_RET_OK;
}

Fd_t spi_Open(void)
{

    /* Selecting P1.5 P1.6 and P1.7 in SPI mode */
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
			GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

	/* CS setup. */
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN6);

    /* Configuring SPI in 3wire master mode */
	SPI_initMaster(EUSCI_B0_MODULE, &spiMasterConfig);

	/* Enable SPI module */
	SPI_enableModule(EUSCI_B0_MODULE);

    return 0;//NONOS_RET_OK;
}


int spi_Write(Fd_t fd, unsigned char *pBuff, int len)
{
    int len_to_return = len;

    ASSERT_CS();
    while (len)
    {
        while (!(UCB0IFG&UCTXIFG));
        UCB0TXBUF = *pBuff;
        while (!(UCB0IFG&UCRXIFG));
        UCB0RXBUF;
        len --;
        pBuff++;
    }

    DEASSERT_CS();

    return len_to_return;
}


int spi_Read(Fd_t fd, unsigned char *pBuff, int len)
{
    int i = 0;

    ASSERT_CS();

    for (i = 0; i < len; i ++)
    {
        while (!(UCB0IFG&UCTXIFG));
        UCB0TXBUF = 0xFF;
        while (!(UCB0IFG&UCRXIFG));
        pBuff[i] = UCB0RXBUF;
    }

    DEASSERT_CS();

    return len;
}

