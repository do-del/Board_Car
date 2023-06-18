#ifndef RF24_H
#define RF24_H

#include <stc8h.h>
#include <intrins.h>

#define	MOSI      P00             // Master Out, Slave In pin (output)
#define	MISO      P27             // Master In, Slave Out pin (input)
#define	SCK       P01             // Serial Clock pin, (output)
#define	CSN       P02             // Slave Select pin, (output to CSN)
#define	CE        P03             // Chip Enable pin signal (output)
#define	IRQ       P32             // Interrupt signal, from nRF24L01 (input)

#define TX_ADR_WIDTH   5  				// 5�ֽڿ�ȵķ���/���յ�ַ
#define TX_PLOAD_WIDTH 32  				// ����ͨ����Ч���ݿ��
extern unsigned char TX_ADDRESS[];

//********************************************************************************************************************//
// SPI(RF24) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//********************************************************************************************************************//
// SPI(RF24) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define RSSI            0x09  // 'Received Signal Strength Indecator' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

//********************************************************************************************************************//
// STATUS Register 
#define RX_DR						0x40  /**/
#define TX_DS						0x20
#define MAX_RT					0x10

//�������ܣ�RF24���ų�ʼ��                
void RF24_Init(void);

//�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                
unsigned char RF24_Write_Reg(unsigned char reg, unsigned char value);

//�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                  
unsigned char RF24_Write_Buf(unsigned char reg, const unsigned char *pBuf, unsigned char bytes);						  					   

//�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
unsigned char RF24_Read_Reg(unsigned char reg);

//�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�
unsigned char RF24_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes);

//�������ܣ�RF24����ģʽ��ʼ��                      
void RF24_RX_Mode(void);			


//�������ܣ�RF24����ģʽ��ʼ��                      
void RF24_TX_Mode(void);

//�������ܣ���ȡ��������                       
unsigned char RF24_RxPacket(unsigned char *rxbuf);

//�������ܣ�����һ�����ݰ�                      
unsigned char RF24_TxPacket(unsigned char *txbuf);

#define	RF_PWR_N12 0x00
#define	RF_PWR_N6 0x01
#define RF_PWR_N4 0x02
#define RF_PWR_0 0x03
#define RF_PWR_P1 0x04
#define RF_PWR_P3 0x05
#define RF_PWR_P4 0x06
#define RF_PWR_P7 0x07
void RF24_Set_Power(unsigned char p);

void RF24_Set_Channel(unsigned char ch);

#define RATE_1M_BPS 0x00
#define RATE_2M_BPS 0x01
#define RATE_250K_BPS 0x02
#define RATE_RESERVED 0x03
void RF24_Set_Rate(unsigned char rate);

void RF24_Set_P0_Size(unsigned char len);

#endif