/***************************************************************************//**
 *   @file   ADF7024.h
 *   @brief  Header file of ADF7024 Driver.
********************************************************************************/
#ifndef __ADF7024_H__
#define __ADF7024_H__

/* Status Word */ //doubt
#define STATUS_SPI_READY  (0x1 << 7)
#define STATUS_IRQ_STATUS (0x1 << 6)
#define STATUS_CMD_READY  (0x1 << 5)
#define STATUS_FW_STATE   (0x1F << 0)

/* FW_STATE Description *///doubt
#define FW_STATE_INIT             0x0F
#define FW_STATE_BUSY             0x00
#define FW_STATE_PHY_OFF          0x11
#define FW_STATE_PHY_ON           0x12
#define FW_STATE_PHY_RX           0x13
#define FW_STATE_PHY_TX           0x14
#define FW_STATE_PHY_SLEEP        0x06
#define FW_STATE_GET_RSSI         0x05
#define FW_STATE_IR_CAL           0x07
#define FW_STATE_AES_DECRYPT_INIT 0x08
#define FW_STATE_AES_DECRYPT      0x09
#define FW_STATE_AES_ENCRYPT      0x0A

/* SPI Memory Access Commands */ //doubt
#define SPI_MEM_WR  0x1E // Write data to packet RAM sequentially.
#define SPI_MEM_RD  0x38 // Read data from packet RAM sequentially.
#define SPI_MEMR_WR 0x08 // Write data to packet RAM nonsequentially.
#define SPI_MEMR_RD 0x28 // Read data from packet RAM nonsequentially.
#define SPI_NOP     0xFF // No operation.

/* Radio Controller Commands */ //doubt
#define CMD_SYNC             0xA2 // This is an optional command. It is not necessary to use it during device initialization
#define CMD_PHY_OFF          0xB0 // Performs a transition of the device into the PHY_OFF state.
#define CMD_PHY_ON           0xB1 // Performs a transition of the device into the PHY_ON state.
#define CMD_PHY_RX           0xB2 // Performs a transition of the device into the PHY_RX state.
#define CMD_PHY_TX           0xB5 // Performs a transition of the device into the PHY_TX state.
#define CMD_PHY_SLEEP        0xBA // Performs a transition of the device into the PHY_SLEEP state.
#define CMD_CONFIG_DEV       0xBB // Configures the radio parameters based on the values.
#define CMD_GET_RSSI         0xBC // Performs an RSSI measurement.
#define CMD_BB_CAL           0xBE // Performs a calibration of the IF filter.
#define CMD_HW_RESET         0xC8 // Performs a full hardware reset. The device enters the PHY_SLEEP state.
#define CMD_RAM_LOAD_INIT    0xBF // Prepares the program RAM for a firmware module download.
#define CMD_RAM_LOAD_DONE    0xC7 // Performs a reset of the communications processor after download of a firmware module to program RAM.
#define CMD_IR_CAL           0xBD // Initiates an image rejection calibration routine.
#define CMD_AES_ENCRYPT      0xD0 // Performs an AES encryption on the transmit payload data stored in packet RAM.
#define CMD_AES_DECRYPT      0xD2 // Performs an AES decryption on the received payload data stored in packet RAM.
#define CMD_AES_DECRYPT_INIT 0xD1 // Initializes the internal variables required for AES decryption.
#define CMD_RS_ENCODE_INIT   0xD1 // Initializes the internal variables required for the Reed Solomon encoding.
#define CMD_RS_ENCODE        0xD0 // Calculates and appends the Reed Solomon check bytes to the transmit payload data stored in packet RAM.
#define CMD_RS_DECODE        0xD2 // Performs a Reed Solomon error correction on the received payload data stored in packet RAM.

/* Rolling Data Buffer */ //doubt
#define INTERRUPT_MASK_0                  			0x100
#define INTERRUPT_SOURCE_0                			0x336
#define RADIO_IMAGE_REJECT_CAL_PHASE            	0x118
#define RADIO_IMAGE_REJECT_CAL_AMPLITUDE        	0x119
#define RADIO_CONTROL                      			0x11A
#define PKT_TX_BASE_ADR                				0x124
#define PKT_RX_BASE_ADR                				0x125
#define MAX_ROLLING_BUFFER_LEN_TRX_BYTE_LOW         0x014
#define MAX_ROLLING_BUFFER_LEN_TRX_BYTE_HIGH        0x015
#define BB_RX_BUFFER_SIGNAL                			0x134
#define BB_RX_BUFFER_SIZE                			0x135
#define BB_TX_BUFFER_SIGNAL                			0x136
#define BB_TX_BUFFER_SIZE                			0x137
#define PKT_TESTMODES                				0x139
#define IMAGE_REJECT_CAL_CONFIG           			0x319

/* RDB_INTERRUPT_MASK_0 - 0x100 */ //doubt
#define RDB_INTERRUPT_MASK_0    					(0x1 << 6)
/* RDB_INTERRUPT_SOURCE_0 - 0x336 */ //doubt
#define RDB_INTERRUPT_SOURCE_0						(0x1 << 6)

//doubt
/* RBD_RADIO_CFG_0 - 0x10C */
#define RBD_RADIO_CFG_0_DATA_RATE(x)      ((x & 0xF) << 0) // for radio profile A
#define RBD_RADIO_CFG_0_FREQ_DEVIATION(x) ((x & 0xF) << 4)

/* RBD_RADIO_CFG_1 -0x10D */
#define RBD_RADIO_CFG_1_DATA_RATE(x)      ((x & 0xF) << 0) // for radio profile B
#define RBD_RADIO_CFG_1_FREQ_DEVIATION(x) ((x & 0xF) << 4)

/* RBD_RADIO_CFG_2 - 0x10E */
#define RBD_RADIO_CFG_2_DATA_RATE(x)      ((x & 0xF) << 0) // for radio profile C
#define RBD_RADIO_CFG_2_FREQ_DEVIATION(x) ((x & 0xF) << 4)

/* RBD_RADIO_CFG_3 -  0x10F */  // for radio profile D
#define RBD_RADIO_CFG_3_DATA_RATE(x)      ((x & 0xF) << 0)
#define RBD_RADIO_CFG_3_FREQ_DEVIATION(x) ((x & 0xF) << 4)

/* RBD_RADIO_CFG_4 -  0x110 */
#define RBD_RADIO_CFG_4_DATA_RATE(x)      ((x & 0xF) << 0) // for radio profile E
#define RBD_RADIO_CFG_4_FREQ_DEVIATION(x) ((x & 0xF) << 4)

/* RBD_RADIO_CFG_5 -0x111 */
#define RBD_RADIO_CFG_5_DATA_RATE(x)      ((x & 0xF) << 0) // for radio profile F
#define RBD_RADIO_CFG_5_FREQ_DEVIATION(x) ((x & 0xF) << 4)



struct ADF7024_RDB
{
	unsigned char interruptMask0;								// 0x100
	unsigned char interruptSource0;								// 0x336
	unsigned char channelFreq0;									// 0x109 doubt
	unsigned char channelFreq1;									// 0x10A doubt
	unsigned char channelFreq2;									// 0x10B doubt
	unsigned char radioCfg0;									// 0x10C
	unsigned char radioCfg1;									// 0x10D
	unsigned char radioCfg2;									// 0x10E
	unsigned char radioCfg3;									// 0x10F
	unsigned char radioCfg4;									// 0x110
	unsigned char radioCfg5;									// 0x111
	unsigned char imageRejectCalPhase;							// 0x118
    unsigned char imageRejectCalAmplitude;						// 0x119
	unsigned char txBaseAdr;									// 0x124
    unsigned char rxBaseAdr;									// 0x125
	unsigned char maximum_packet_length_control_trx_byte_low;	// 0x014
	unsigned char maximum_packet_length_control_trx_byte_high;	// 0x015
	unsigned char addressMatchOffset;							// 0x129
	unsigned char addressLength;								// 0x12A
	unsigned char rxBuffersignal;								// 0x134
	unsigned char rxBuffersize;									// 0x135
	unsigned char txBuffersignal;								// 0x136
	unsigned char txBuffersize;									// 0x137
	unsigned char testmodes;									// 0x139
};


#define ADF7024_PKT_TX_BASE_ADR 0x10
#define ADF7024_PKT_RX_BASE_ADR 0x10

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/

/* Initializes the ADF7024. */
char ADF7024_Init(void);

/* Reads the status word of the ADF7024. */
void ADF7024_GetStatus(unsigned char* status);

/* Initiates a command. */
void ADF7024_SetCommand(unsigned char command);

/* Sets a FW state and waits until the device enters in that state. */
void ADF7024(unsigned char fwState);

/* Reads data from the RAM. */
void ADF7024_GetRAM(unsigned long address,
                    unsigned long length,
                    unsigned char* data);

/* Writes data to RAM. */
void ADF7024_SetRAM(unsigned long address,
                    unsigned long length,
                    unsigned char* data);

/* Receives one packet. */
void ADF7024_ReceivePacket(unsigned char* packet, unsigned char* length);

/* Transmits one packet. */
void ADF7024_TransmitPacket(unsigned char* packet, unsigned char length);

/* Sets the channel frequency. */
void ADF7024_SetChannelFrequency(unsigned long chFreq);

/* Sets the data rate. */
void ADF7024_SetDataRate(unsigned long dataRate);

/* Sets the frequency deviation. */
void ADF7024_SetFrequencyDeviation(unsigned long freqDev);

/*! Initializes the SPI communication peripheral. */

#endif // __ADF7024_H__