/***************************************************************************//**
 *   @file   ADF7024.h
 *   @brief  Configuration file of ADF7024 Driver.
********************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "ADF7024.h"

/******************************************************************************/
/************************* Variables Declarations *****************************/
/******************************************************************************/
struct ADF7024_RDB ADF7024_RDBDefault =
{
	/* interruptMask0 - 0x100 */
	INTERRUPT_MASK_0,
	/* interruptSource0 - 0x336 */
	INTERRUPT_SOURCE_0,
	/* channelFreq0 - 0x109 */
	0x51, // Channel Frequency: 433 MHz
	/* channelFreq1 - 0x10A */
	0xA7, // Channel Frequency: 433 MHz
	/* channelFreq2 - 0x10B */
	0x10, // Channel Frequency: 433 MHz	
	//doubt
	/* radioCfg1 - 0x10D */
	RBD_RADIO_CFG_0_DATA_RATE(0x00)  | // Data rate: 9.6 kHz
	RBD_RADIO_CFG_0_FREQ_DEVIATION(0x03),       // Frequency deviation: 9.6 kbps // for radio profile A	
	/* radioCfg1 0x10D */
	RBD_RADIO_CFG_1_DATA_RATE(0x00) | // Data rate: 38.4 kHz
	RBD_RADIO_CFG_1_FREQ_DEVIATION(0x03),       // Frequency deviation: 20 kbps // for radio profile B
	/* radioCfg2   0x10E*/
	RBD_RADIO_CFG_2_DATA_RATE(0x00) | // Data rate: 50 kHz
	RBD_RADIO_CFG_2_FREQ_DEVIATION(0x03),       // Frequency deviation: 25 kbps // for radio profile C
	/* radioCfg3 0x10F */
	RBD_RADIO_CFG_3_DATA_RATE(0x00) | // Data rate: 100 kHz
	RBD_RADIO_CFG_3_FREQ_DEVIATION(0x03),       // Frequency deviation: 25 kbps // for radio profile D 
	/* radioCfg4  0x110*/
	RBD_RADIO_CFG_4_DATA_RATE(0x00) | // Data rate: 200 kHz
	RBD_RADIO_CFG_4_FREQ_DEVIATION(0x03),       // Frequency deviation: 50 kbps // for radio profile E
	/* radioCfg5  0x111*/
	RBD_RADIO_CFG_5_DATA_RATE(0x00) | // Data rate: 300 kHz
	RBD_RADIO_CFG_5_FREQ_DEVIATION(0x03),       // Frequency deviation: 75 kbps // for radio profile F
	/* imageRejectCalPhase - 0x118 */
    0x00,
    /* imageRejectCalAmplitude - 0x119 */
    0x00,
	/* txBaseAdr - 0x124 */
	PKT_TX_BASE_ADR,
	/* rxBaseAdr - 0x125 */
	PKT_RX_BASE_ADR,
	/* Maximum Packet Length Control for low byte -0x014 */
	MAX_ROLLING_BUFFER_LEN_TRX_BYTE_LOW,
	/* Maximum Packet Length Control for high byte -0x015 */
	MAX_ROLLING_BUFFER_LEN_TRX_BYTE_HIGH,
	 /* addressMatchOffset - 0x129 */
	 0x01,
	 /* addressLength - 0x12A */
	 0x02,
	/* rxBuffersignal - 0x134 */
	BB_RX_BUFFER_SIGNAL,
	/* rxBuffersize - 0x135 */
	BB_RX_BUFFER_SIZE,
	/* txBuffersignal - 0x136 */
	BB_TX_BUFFER_SIGNAL,
	/* txBuffersize - 0x137 */
	BB_TX_BUFFER_SIZE,
	/* testmodes - 0x139 */
	PKT_TESTMODES
};