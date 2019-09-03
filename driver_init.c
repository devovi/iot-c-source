/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>

#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE != 3U))
/* Weak Non-secure callable function. Real one should be in secure gateway library. */
WEAK int32_t nsc_periph_clock_init(uint32_t gclk_id, uint32_t gclk_src)
{
	(void)gclk_id;
	(void)gclk_src;
	return 0;
}
#endif

#include <hpl_adc_base.h>

	/* The channel amount for ADC */
#define ADC_0_CH_AMOUNT 1

/* The buffer size for ADC */
#define ADC_0_BUFFER_SIZE 16

/* The maximal channel number of enabled channels */
#define ADC_0_CH_MAX 0

struct adc_async_descriptor         ADC_0;
struct adc_async_channel_descriptor ADC_0_ch[ADC_0_CH_AMOUNT];
struct timer_descriptor             TIMER_0;
struct spi_m_sync_descriptor        SPI_0;

static uint8_t ADC_0_buffer[ADC_0_BUFFER_SIZE];
static uint8_t ADC_0_map[ADC_0_CH_MAX + 1];

struct rand_sync_desc RAND_0;

struct usart_sync_descriptor USART_0;

struct wdt_descriptor WDT_0;

/**
 * \brief ADC initialization function
 *
 * Enables ADC peripheral, clocks and initializes ADC driver
 */
void ADC_0_init(void)
{
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
	hri_mclk_set_APBCMASK_ADC_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, ADC_GCLK_ID, CONF_GCLK_ADC_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
#else
	nsc_periph_clock_init(ADC_GCLK_ID, CONF_GCLK_ADC_SRC);
#endif
	adc_async_init(&ADC_0, ADC, ADC_0_map, ADC_0_CH_MAX, ADC_0_CH_AMOUNT, &ADC_0_ch[0], (void *)NULL);
	adc_async_register_channel_buffer(&ADC_0, 0, ADC_0_buffer, ADC_0_BUFFER_SIZE);

	// Disable digital pin circuitry
	gpio_set_pin_direction(LIGHT_SENSOR, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(LIGHT_SENSOR, PINMUX_PA02B_ADC_AIN0);
}

void RAND_0_CLOCK_init(void)
{
	hri_mclk_set_APBCMASK_TRNG_bit(MCLK);
}

void RAND_0_init(void)
{
	RAND_0_CLOCK_init();
	rand_sync_init(&RAND_0, TRNG);
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_0_init(void)
{
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
	hri_mclk_set_APBCMASK_TC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC0_GCLK_ID, CONF_GCLK_TC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
#else
	nsc_periph_clock_init(TC0_GCLK_ID, CONF_GCLK_TC0_SRC);
#endif

	timer_init(&TIMER_0, TC0, _tc_get_timer());
}

/**
 * \brief Rtc initialization function
 *
 * Enables Rtc peripheral, clocks and initializes driver
 */
void RTC_CLOCK_init(void)
{
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
	hri_mclk_set_APBAMASK_RTC_bit(MCLK);
#endif
}

void SPI_0_PORT_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(SERCOM0_SPI_MISO, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SERCOM0_SPI_MISO,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SERCOM0_SPI_MISO, PINMUX_PA04D_SERCOM0_PAD0);

	gpio_set_pin_level(SERCOM0_SPI_MOSI,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SERCOM0_SPI_MOSI, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SERCOM0_SPI_MOSI, PINMUX_PA14D_SERCOM0_PAD2);

	gpio_set_pin_level(SERCOM0_SPI_SCK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SERCOM0_SPI_SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SERCOM0_SPI_SCK, PINMUX_PA15D_SERCOM0_PAD3);
	
	//doubt
	// pin configuration for SS -->rohini
	// Set pin direction to output
	gpio_set_pin_direction(SERCOM0_SPI_SS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SERCOM0_SPI_SS, PINMUX_PA05D_SERCOM0_PAD1);

	gpio_set_pin_level(SERCOM0_SPI_SS,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);
}

void SPI_0_CLOCK_init(void)
{
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBCMASK_SERCOM0_bit(MCLK);
#else
	nsc_periph_clock_init(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
	nsc_periph_clock_init(SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC);
#endif
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_m_sync_init(&SPI_0, SERCOM0);
	SPI_0_PORT_init();
}

void USART_0_PORT_init(void)
{

	gpio_set_pin_function(PA00, PINMUX_PA00D_SERCOM1_PAD0);

	gpio_set_pin_function(PA01, PINMUX_PA01D_SERCOM1_PAD1);
}

void USART_0_CLOCK_init(void)
{
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBCMASK_SERCOM1_bit(MCLK);
#else
	nsc_periph_clock_init(SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC);
	nsc_periph_clock_init(SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC);
#endif
}

void USART_0_init(void)
{
	USART_0_CLOCK_init();
	usart_sync_init(&USART_0, SERCOM1, (void *)NULL);
	USART_0_PORT_init();
}

void WDT_0_CLOCK_init(void)
{
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
	hri_mclk_set_APBAMASK_WDT_bit(MCLK);
#endif
}

void WDT_0_init(void)
{
	WDT_0_CLOCK_init();
	wdt_init(&WDT_0, WDT);
}

void EVENT_SYSTEM_0_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, EVSYS_GCLK_ID_0, CONF_GCLK_EVSYS_CHANNEL_0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBCMASK_EVSYS_bit(MCLK);

	event_system_init();
}

void EXTERNAL_IRQ_0_init(void)
{
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
	hri_gclk_write_PCHCTRL_reg(GCLK, EIC_GCLK_ID, CONF_GCLK_EIC_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBAMASK_EIC_bit(MCLK);
#else
	nsc_periph_clock_init(EIC_GCLK_ID, CONF_GCLK_EIC_SRC);
#endif

	// Set pin direction to input
	gpio_set_pin_direction(USER_BUTTON, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(USER_BUTTON,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(USER_BUTTON, PINMUX_PA27A_EIC_EXTINT5);

	ext_irq_init();
}

void system_init(void)
{
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U))
	/* Only initialize MCU clock when the project is TrustZone secure project  */
	init_mcu();
#endif

	// GPIO on PA07

	gpio_set_pin_level(USER_LED0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(USER_LED0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(USER_LED0, GPIO_PIN_FUNCTION_OFF);

	ADC_0_init();

	RAND_0_init();

	TIMER_0_init();

	RTC_CLOCK_init();
	RTC_init();

	SPI_0_init();

	USART_0_init();

	WDT_0_init();

	EVENT_SYSTEM_0_init();
	EXTERNAL_IRQ_0_init();
}
