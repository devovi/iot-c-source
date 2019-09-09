/***************************************************************************//**
 *   @file   ADF7024.c
 *   @brief  Implementation of ADF7024 Driver.
********************************************************************************/
/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "ADF7024.h"
#include "ADF7024_Config.h"
#include "driver_init.h"
#include "hal_spi_m_sync.h"

/******************************************************************************/
/*************************** Macros Definitions *******************************/
/******************************************************************************/

#define CS_PIN_LOW          0
#define	CS_PIN_HIGH			1
//#define MISO_PIN			21
#define ADF7024_CS_ASSERT   CS_PIN_LOW
#define ADF7024_CS_DEASSERT CS_PIN_HIGH
//#define ADF7024_MISO        MISO_PIN//doubt


struct ADF7024_RDB ADF7024_RDBCurrent;


void ADF7024_WriteReadByte(unsigned char writeByte,
                           unsigned char* readByte)
{
    unsigned char data = 0;
    
    data = writeByte;
    //SPI_Read(0, &data, 1);
	_spi_m_sync_io_read(0, &data, 1);
    if(readByte)
    {
        *readByte = data;
    }
}

/***************************************************************************//**
 * @brief Initializes the ADF7024.
 *
 * @return retVal - Result of the initialization procedure.
 *                  Example: 0 - if initialization was successful;
 *                           -1 - if initialization was unsuccessful.
*******************************************************************************/
char ADF7024_Init(void)
{
	char           retVal  = 0;
    unsigned char  miso    = 0;
    unsigned short timeout = 0;
    unsigned char  status  = 0;
    
    ADF7024_RDBCurrent = ADF7024_RDBDefault;//doubt
    //SPI_Init(0,         // MSB first.
             //1000000,   // Clock frequency.
             //0,         // Idle state for clock is a high level; active state is a low level.
             //1);        // Serial output data changes on transition from idle clock state to active clock state.
	SPI_0_init();		 //doubt
    ADF7024_CS_ASSERT;//doubt
    while ((miso == 0) && (timeout < 1000))
    {
        //miso = ADF7024_MISO;//doubt
        timeout++;
    }
    if(timeout == 1000)
    {
        retVal = -1;
    }
    while(!(status & STATUS_CMD_READY))
    {
        ADF7024_GetStatus(&status);
    }
    ADF7024_SetRAM(0x100, 64, (unsigned char*)&ADF7024_RDBCurrent);//doubt
    ADF7024_SetCommand(CMD_CONFIG_DEV);
    
    return retVal;
}

/***************************************************************************//**
 * @brief Reads the status word of the ADF7024.
 *
 * @param status - Status word.
 *
 * @return None.
*******************************************************************************/
void ADF7024_GetStatus(unsigned char* status)
{
	ADF7024_CS_ASSERT;
	ADF7024_WriteReadByte(SPI_NOP, 0);
	ADF7024_WriteReadByte(SPI_NOP, status);
	ADF7024_CS_DEASSERT;
}

/***************************************************************************//**
 * @brief Initiates a command.
 *
 * @param command - Command.
 *
 * @return None.
*******************************************************************************/
void ADF7024_SetCommand(unsigned char command)
{
	ADF7024_CS_ASSERT;
	ADF7024_WriteReadByte(command, 0);
	ADF7024_CS_DEASSERT;
}

/***************************************************************************//**
 * @brief Sets a FW state and waits until the device enters in that state.
 *
 * @param fwState - FW state.
 *
 * @return None.
*******************************************************************************/
void ADF7024_SetFwState(unsigned char fwState)
{
    unsigned char status = 0;
    
    switch(fwState)
    {
        case FW_STATE_PHY_OFF:
            ADF7024_SetCommand(CMD_PHY_OFF);
            break;
        case FW_STATE_PHY_ON:
            ADF7024_SetCommand(CMD_PHY_ON);
            break;
        case FW_STATE_PHY_RX:
            ADF7024_SetCommand(CMD_PHY_RX);
            break;
        case FW_STATE_PHY_TX:
            ADF7024_SetCommand(CMD_PHY_TX);
            break;
        default:
            ADF7024_SetCommand(CMD_PHY_SLEEP);
    }
    while((status & STATUS_FW_STATE) != fwState)
    {
        ADF7024_GetStatus(&status);
    }
}

/***************************************************************************//**
 * @brief Reads data from the RAM.
 *
 * @param address - Start address.
 * @param length - Number of bytes to write.
 * @param data - Read buffer.
 *
 * @return None.
*******************************************************************************/
void ADF7024_GetRAM(unsigned long address,
                    unsigned long length,
                    unsigned char* data)
{
    ADF7024_CS_ASSERT;
    ADF7024_WriteReadByte(SPI_MEM_RD | ((address & 0x700) >> 8), 0);//doubt
    ADF7024_WriteReadByte(address & 0xFF, 0);//doubt
    ADF7024_WriteReadByte(SPI_NOP, 0);
    while(length--)
    {
        ADF7024_WriteReadByte(SPI_NOP, data++);
    }
    ADF7024_CS_DEASSERT;
}

/***************************************************************************//**
 * @brief Writes data to RAM.
 *
 * @param address - Start address.
 * @param length - Number of bytes to write.
 * @param data - Write buffer.
 *
 * @return None.
*******************************************************************************/
void ADF7024_SetRAM(unsigned long address,
                    unsigned long length,
                    unsigned char* data)
{
	ADF7024_CS_ASSERT;
	ADF7024_WriteReadByte(SPI_MEM_WR | ((address & 0x700) >> 8), 0);//doubt
	ADF7024_WriteReadByte(address & 0xFF, 0);//doubt
	while(length--)
	{
		ADF7024_WriteReadByte(*(data++), 0);//doubt
	}
	ADF7024_CS_DEASSERT;
}

/***************************************************************************//**
 * @brief Receives one packet.
 *
 * @param packet - Data buffer.
 * @param length - Number of received bytes.
 *
 * @return None.
*******************************************************************************/
void ADF7024_ReceivePacket(unsigned char* packet, unsigned char* length)
{
	unsigned char interruptReg = 0;
	
	ADF7024_SetFwState(FW_STATE_PHY_ON);
	ADF7024_SetFwState(FW_STATE_PHY_RX);
	while(!(interruptReg & RDB_INTERRUPT_MASK_0))//doubt
	{
		ADF7024_GetRAM(RDB_INTERRUPT_MASK_0, 0x1, &interruptReg);
	}
	ADF7024_GetRAM(INTERRUPT_SOURCE_0, 0x1, &interruptReg); //doubt
	ADF7024_GetRAM(0x10, 1, length);
	ADF7024_GetRAM(0x12, *length - 2, packet);
}

/***************************************************************************//**
 * @brief Transmits one packet.
 *
 * @param packet - Data buffer.
 * @param length - Number of bytes to transmit.
 *
 * @return None.
*******************************************************************************/
void ADF7024_TransmitPacket(unsigned char* packet, unsigned char length)
{
	unsigned char interruptReg = 0;
	unsigned char header[2]    = {0, 0};
	
	header[0] = 2 + length;
	//doubt to configure addressMatchOffset
	header[1] = ADF7024_RDBCurrent.addressMatchOffset; //need to be update
	ADF7024_SetRAM(0x10, 2, header);
	ADF7024_SetRAM(0x12, length, packet);
	ADF7024_SetFwState(FW_STATE_PHY_ON);
	ADF7024_SetFwState(FW_STATE_PHY_TX);
	while(!(interruptReg & RDB_INTERRUPT_MASK_0))//doubt
	{
		ADF7024_GetRAM(INTERRUPT_SOURCE_0,//doubt
		0x1,
		&interruptReg);
	}
}

/***************************************************************************//**
 * @brief Sets the channel frequency.
 *
 * @param chFreq - Channel frequency.
 *
 * @return None.
*******************************************************************************/
//doubt
void ADF7024_SetChannelFrequency(unsigned long chFreq)
{
	chFreq = (unsigned long)(((float)chFreq / 26000000) * 65535);
	ADF7024_RDBCurrent.channelFreq0 = (chFreq & 0x0000FF) >> 0;
	ADF7024_RDBCurrent.channelFreq1 = (chFreq & 0x00FF00) >> 8;
	ADF7024_RDBCurrent.channelFreq2 = (chFreq & 0xFF0000) >> 16;
	ADF7024_SetRAM(0x100, 64, (unsigned char*)&ADF7024_RDBCurrent);
}

/***************************************************************************//**
 * @brief Sets the data rate.
 *
 * @param dataRate - Data rate.
 *
 * @return None.
*******************************************************************************/
//doubt
void ADF7024_SetDataRate(unsigned long dataRate)
{
	 dataRate = (unsigned long)(dataRate / 100);
	 // for radio profile D Data rate: 100 kbps  Frequency deviation: 25 Hz
	 ADF7024_RDBCurrent.radioCfg3 = RBD_RADIO_CFG_3_DATA_RATE((dataRate & 0x00FF) >> 0);
	 ADF7024_RDBCurrent.radioCfg3 &= RBD_RADIO_CFG_3_DATA_RATE(0xF);
	 ADF7024_RDBCurrent.radioCfg3 |= RBD_RADIO_CFG_3_DATA_RATE((dataRate & 0x0F00) >> 8);
	 ADF7024_SetRAM(0x100, 64, (unsigned char*)&ADF7024_RDBCurrent);
	 ADF7024_SetFwState(FW_STATE_PHY_OFF);
	 ADF7024_SetCommand(CMD_CONFIG_DEV);
}

/***************************************************************************//**
 * @brief Sets the frequency deviation.
 *
 * @param freqDev - Frequency deviation.
 *
 * @return None.
*******************************************************************************/
void ADF7024_SetFrequencyDeviation(unsigned long freqDev)
{
	freqDev = (unsigned long)(freqDev / 100);
	// for radio profile D Data rate: 100 kbps  Frequency deviation: 25 Hz
	ADF7024_RDBCurrent.radioCfg3 &= RBD_RADIO_CFG_3_FREQ_DEVIATION(0xF);
	ADF7024_RDBCurrent.radioCfg3 |= RBD_RADIO_CFG_3_FREQ_DEVIATION((freqDev & 0x0F00) >> 8);
	ADF7024_RDBCurrent.radioCfg3 = RBD_RADIO_CFG_3_FREQ_DEVIATION((freqDev & 0x00FF) >> 0);
	ADF7024_SetRAM(0x100, 64, (unsigned char*)&ADF7024_RDBCurrent);
	ADF7024_SetFwState(FW_STATE_PHY_OFF);
	ADF7024_SetCommand(CMD_CONFIG_DEV);
}
