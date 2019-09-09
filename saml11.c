#include <atmel_start.h>
#include "driver_init.h"

/* global 8-bit array for the DGI communication */
uint8_t DataStream_buf[4] = {0x03, 0x00, 0x00, 0xFC};

/* SPI DGI io descriptor */
static struct io_descriptor *io;

/* Global Variables */
volatile bool     SW0_flag          = false;

static void button_on_PA27_used(void)
{
	SW0_flag = true;
}

/* RTC Interrupt Handler used in IDLE_MODE_IRQ & STANDBY_MODE_IRQ modes */
void RTC_Handler(void)
{
	hri_rtcmode0_clear_INTFLAG_CMP0_bit(RTC); /* clear the pending interrupts */
	adc_async_start_conversion(&ADC_0);
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_enable(void)
{
	ext_irq_register(PIN_PA27, button_on_PA27_used);
}

static void SPI_DGI_Init(void)
{
	spi_m_sync_get_io_descriptor(&SPI_0, &io);
	spi_m_sync_enable(&SPI_0);
}

/* saml11 init Function: STATE MACHINE IMPLEMENTATION */
int ATSAML11E16A_init()
{
	int status = 0;
	/* Initializes MCU, drivers and middle ware */
	atmel_start_init();
	
	/* Initialize the SPI connection for saml11 */
	status = SPI_0_init();
	
	/* Enable IRQ on the PA27 */
	EXTERNAL_IRQ_0_enable();

	/* Initialize the SPI connection for DGI usage */
	SPI_DGI_Init();
	
	 return status;

} // end of main
