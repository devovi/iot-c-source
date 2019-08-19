/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip
 * Support</a>
 */

#include <atmel_start.h>
#include "driver_init.h"
#include "hpl_dma.h"

/* global 8-bit array for the DGI communication */
uint8_t DataStream_buf[4] = {0x03, 0x00, 0x00, 0xFC};

/* ADC Result variable in RAM section */
SECTION_DMAC_DESCRIPTOR volatile uint16_t adc_result_store;

#define DMA_BITCOUNT_VALUE 10

/* *** Low Power Mode Definitions *** */

/* *** Application behvior's definitions *** */
#define IDLE_MODE_IRQ 0
#define STANDBY_MODE_IRQ 1
#define STANDBY_MODE_SLPWKG 2
#define STANDBY_MODE_SLPWKG_PWRGAT 3

#define MAX_MODE_NUMBER 3
#define MAX_TRANSFER 10

/* SPI DGI io descriptor */
static struct io_descriptor *io;

/* Global Variables */
volatile bool     DMA_Flag          = false;
volatile bool     read_light_sensor = true;
volatile bool     led0_state        = false;
volatile uint32_t app_mode          = IDLE_MODE_IRQ;
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

/* collect Light Sensor result make an average and send it to the data streamer*/
void Light_sensor_to_data_streammer(void)
{
	uint16_t i;
	uint16_t average = 0;
	uint32_t temp    = 0;
	uint32_t temp2   = 0;

	/* global 8-bit array for the DGI communication */
	DataStream_buf[1] = 0;
	DataStream_buf[2] = 0;
	/* Light Sensor data streamer formating data */
	for (i = 0; i < (MAX_TRANSFER / 2); i++) {
		temp  = *(unsigned short *)(&adc_result_store + (i * 2));
		temp2 = temp + temp2;
	}
	average = temp2 / (MAX_TRANSFER / 2);
	/* Send the average value of the last 10 ADC results */
	DataStream_buf[1] = (uint8_t)average;
	DataStream_buf[2] = (uint8_t)(average >> 8);

	io_write(io, DataStream_buf, 4); /* use the SPI to send the data */
}

static void SPI_DGI_Init(void)
{
	spi_m_sync_get_io_descriptor(&SPI_0, &io);
	spi_m_sync_enable(&SPI_0);
}

void optimize_Low_Power(void)
{
	/* Set Low power Mode Efficiency for the Low Power Voltage Regulator (LPVREG)  */
	hri_supc_set_VREG_LPEFF_bit(SUPC);

	// nLDO/Buck Regulator mode : Select Buck = 1
	hri_supc_write_VREG_SEL_bf(SUPC, SUPC_VREG_SEL_BUCK_Val);
	while (hri_supc_get_STATUS_VREGRDY_bit(SUPC) == false)
		;

	/* PDSW Power Domain domain will be put in retention when entering STANDBY sleep mode */
	hri_pm_write_STDBYCFG_PDCFG_bit(PM, PM_STDBYCFG_PDCFG_DEFAULT_Val);

	/* 4kB SRAM out of 16kB will be retained in STANDBY */
	PM->PWCFG.bit.RAMPSWC = PM_PWCFG_RAMPSWC_4KB_Val;
}

static void Enter_Mode_Standby(void)
{
	/* Enter STANDBY Sleep Mode */
	hri_pm_write_SLEEPCFG_reg(PM, PM_SLEEPCFG_SLEEPMODE_STANDBY);

	/* Ensure register is written before issuing wfi command */
	while (hri_pm_read_SLEEPCFG_SLEEPMODE_bf(PM) != PM_SLEEPCFG_SLEEPMODE_STANDBY_Val)
		;

	__DSB();
	__WFI();
}

static void enable_dynpower_gating(void)
{
	/**< \brief (PM_STDBYCFG) Dynamic Power Gating enabled */
	hri_pm_set_STDBYCFG_DPGPDSW_bit(PM);
}

static void disable_dynpower_gating(void)
{
	/**< \brief (PM_STDBYCFG) Dynamic Power Gating disabled */
	hri_pm_clear_STDBYCFG_DPGPDSW_bit(PM);
}

/* Entering Idle Mode */
static void Enter_Mode_Idle(void)
{
	/* Enter IDLE Sleep Mode */
	hri_pm_write_SLEEPCFG_reg(PM, PM_SLEEPCFG_SLEEPMODE_IDLE);

	/* Ensure register is written before issuing wfi command */
	while (hri_pm_read_SLEEPCFG_SLEEPMODE_bf(PM) != PM_SLEEPCFG_SLEEPMODE_IDLE_Val)
		;

	__DSB();
	__WFI();
}

/* reconfigure the RTC in interrupt when in IDLE_MODE_IRQ & STANDBY_MODE_IRQ */
static void rtc_reconfig_Interrupts(void)
{
	hri_rtcmode0_clear_CTRLA_ENABLE_bit(RTC);     /* RTC Disable. */
	RTC->MODE1.CTRLA.reg = RTC_MODE1_CTRLA_SWRST; /* Perform a Software reset on the RTC */
	while (RTC->MODE1.SYNCBUSY.bit.SWRST)
		; /* Wait for synchronization of the SWRST */
	hri_rtcmode0_write_CTRLA_reg(
	    RTC,
	    0 << RTC_MODE0_CTRLA_COUNTSYNC_Pos       /* COUNT Read Synchronization Enable: disabled */
	        | 0 << RTC_MODE0_CTRLA_PRESCALER_Pos /* Setting: 1 */
	        | 1 << RTC_MODE0_CTRLA_MATCHCLR_Pos  /* Clear on Match: enabled */
	        | 0 << RTC_MODE0_CTRLA_MODE_Pos      /* Operating Mode: 0 */
	        | 0 << RTC_MODE0_CTRLA_GPTRST_Pos);  /* GP Registers Reset On Tamper Enable: disabled */
	hri_rtcmode0_write_COMP_reg(RTC, 0, 0xccc);  /* Compare Value: 3276 */
	hri_rtcmode0_clear_EVCTRL_CMPEO0_bit(RTC);   /* Clear the RTC CMPEO0 event to disable event coming from the RTC */
	hri_rtcmode0_set_INTEN_CMP0_bit(
	    RTC);                 /* Set the  CMPEO0 Interrupt to enable interrupt coming from the RTC compare 0 */
	NVIC_EnableIRQ(RTC_IRQn); /* Enable the interrupt at the core level */
}

/* reconfigure the ADC to be started by from the RTC interrupt Handler when in IDLE_MODE_IRQ & STANDBY_MODE_IRQ */
static void adc_reconfig(void)
{
	/* Clear the ADC Event bit, now the ADC is started in The RTC Interrupt Handler */
	hri_adc_clear_EVCTRL_STARTEI_bit(ADC);
}

/**
 * \brief DMA Interrupt Handler function used in all state of the application
 */
void dmac_channel_0_callback(struct _dma_resource *resource)
{
	DMA_Flag = true;
}

void config_dma_channel_0(void)
{
	struct _dma_resource *dma_res;

	/* Set DMA CH-0 source address (SRAM: adc_result)*/
	_dma_set_source_address(0, (void *)(&(ADC->RESULT)));

	/* Set DMA CH-0 destination address (SRAM: adc_result_copy)*/
	_dma_set_destination_address(
	    0, (void *)((uint32_t)(&adc_result_store) + (DMA_BITCOUNT_VALUE * (DMAC_BTCTRL_STEPSIZE_X1_Val + 1))));

	/* Set DMA CH-0 block length */
	_dma_set_data_amount(0, (uint32_t)DMA_BITCOUNT_VALUE);

	/* Get DMA CH-0 resource to set the application callback */
	_dma_get_channel_resource(&dma_res, 0);

	/* Set application callback to handle the DMA CH-0 transfer done */
	dma_res->dma_cb.transfer_done = dmac_channel_0_callback;

	/* Set next descriptor */
	_dma_set_next_descriptor(0, 0);

	/* Enable DMA CH-0 transfer complete interrupt */
	_dma_set_irq_state(0, DMA_TRANSFER_COMPLETE_CB, true);

	/* Enable CH-0 DMA transaction */
	_dma_enable_transaction(0, false);
	while (DMAC->CHSTATUS.bit.BUSY)
		;
}

/* Main Function: STATE MACHINE IMPLEMENTATION */
int main(void)
{
	/* Initializes MCU, drivers and middle ware */
	atmel_start_init();

	/* Optimize the SAM L10 Chip to Low Power */
	optimize_Low_Power();

	/* Enable IRQ on the PA27 */
	EXTERNAL_IRQ_0_enable();

	/* Initialize the SPI connection for DGI usage */
	SPI_DGI_Init();

	config_dma_channel_0();

	/* Replace with your application code */
	while (1) {

		if (SW0_flag) {
			SW0_flag = false;
			if (app_mode == MAX_MODE_NUMBER) {
				app_mode = 0;
			} else {
				app_mode++;
			}
		}
		switch (app_mode) {
		case IDLE_MODE_IRQ:
			/* reconfigure the RTC interrupt */
			rtc_reconfig_Interrupts();

			/* Reconfigure the ADC to work without event triggering */
			adc_reconfig();

			/* Enable ADC channel */
			adc_async_enable_channel(&ADC_0, 0);

			/* reset DMA flag */
			DMA_Flag = false;

			/* RTC Enable */
			hri_rtcmode0_set_CTRLA_ENABLE_bit(RTC);

			/* Wait for DMA transfer complete */
			while (!(DMA_Flag)) {
				/* No power gating in this mode */
				disable_dynpower_gating();

				/* Enter IDLE Sleep Mode */
				Enter_Mode_Idle();
			}

			/* WAKE-UP!!! */
			/* DGI Transfer to the Data Streamer */
			Light_sensor_to_data_streammer();
			break;

		case STANDBY_MODE_IRQ:
			/* reconfigure the RTC interrupt */
			rtc_reconfig_Interrupts();

			/* Reconfigure the ADC to work without event triggering */
			adc_reconfig();

			/* Enable ADC channel */
			adc_async_enable_channel(&ADC_0, 0);

			/* reset DMA flag */
			DMA_Flag = false;

			/* RTC Enable */
			hri_rtcmode0_set_CTRLA_ENABLE_bit(RTC);

			/* Wait for DMA transfer complete */
			while (!(DMA_Flag)) {
				/* No power gating in this mode */
				disable_dynpower_gating();

				/* Enter STANDBY Sleep Mode */
				Enter_Mode_Standby();
			}

			/* WAKE-UP!!! */
			/* DGI Transfer to the Data Streamer */
			Light_sensor_to_data_streammer();

			break;

		case STANDBY_MODE_SLPWKG:
			/* Initializes MCU, drivers and middle ware */
			ADC_0_init();

			/* RTC Initialization */
			RTC_init();

			/* Enable ADC channel */
			adc_async_enable_channel(&ADC_0, 0);

			/* reset DMA flag */
			DMA_Flag = false;

			/* Wait for DMA transfer complete*/
			while (!(DMA_Flag)) {
				/* No power gating in this mode */
				disable_dynpower_gating();

				/* Enter STANDBY Sleep Mode */
				Enter_Mode_Standby();
			}

			/* WAKE-UP!!! */
			Light_sensor_to_data_streammer();
			break;

		case STANDBY_MODE_SLPWKG_PWRGAT:
			/* Initializes MCU, drivers and middle ware */
			ADC_0_init();

			/* RTC Initialization */
			RTC_init();

			/* Enable ADC channel */
			adc_async_enable_channel(&ADC_0, 0);

			/* reset DMA flag */
			DMA_Flag = false;

			/* Wait for DMA transfer complete*/
			while (!(DMA_Flag)) {
				/* enable Power gating */
				enable_dynpower_gating();

				/* Enter STANDBY Sleep Mode */
				Enter_Mode_Standby();
			}

			/* WAKE-UP!!! */
			Light_sensor_to_data_streammer();
			break;

		} // end of switch (app_mode)
	}     // end of while(1)
} // end of main
