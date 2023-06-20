#ifndef RF24_H
#define RF24_h

#include "stc32g.h"
#include "intrins.h"
#include "spi.h"

#define RF24_CSN P26
#define RF24_IRQ P22
#define RF24_CE P27

#define MOSI_HIGH() (MOSI = 1)
#define MOSI_LOW() (MOSI = 0)
//#define MISO_HIGH() (PORTB |= 0x10)
//#define MISO_LOW() (PORTB &= ~0x10)
#define MISO_VALUE() (MISO)
#define SCK_HIGH() (SCK = 1)
#define SCK_LOW() (SCK = 0)
#define CSN_HIGH() (RF24_CSN = 1)
#define CSN_LOW() (RF24_CSN = 0)
#define CE_HIGH() (RF24_CE = 1) //CE�ߵ�ƽ��оƬ��Standbyģʽ���뵽Idle-TX��RXģʽ
#define CE_LOW() (RF24_CE = 0) //CE�͵�ƽ��оƬ����Standbyģʽ
#define IRQ_VALUE() (RF24_IRQ)

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
#define CONFIG          0x00  // ���üĴ���
#define EN_AA           0x01  // ʹ�����ݹܵ��Զ�ȷ��
#define EN_RXADDR       0x02  // ʹ�ܽ������ݹܵ���ַ
#define SETUP_AW        0x03  // ��ַ�������
#define SETUP_RETR      0x04  // �Զ��ط�����
#define RF_CH           0x05  // ��Ƶ�ŵ�������ͨ�ŵ�Ƶ��
#define RF_SETUP        0x06  // ��Ƶ����
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ������ͳ��
#define RSSI            0x09  // �����ź�ǿ�ȼ��
#define RX_ADDR_P0      0x0A  // ���ݹܵ�0�Ľ��յ�ַ
#define RX_ADDR_P1      0x0B  // ���ݹܵ�1�Ľ��յ�ַ
#define RX_ADDR_P2      0x0C  // ���ݹܵ�2�Ľ��յ�ַ
#define RX_ADDR_P3      0x0D  // ���ݹܵ�3�Ľ��յ�ַ
#define RX_ADDR_P4      0x0E  // ���ݹܵ�4�Ľ��յ�ַ
#define RX_ADDR_P5      0x0F  // ���ݹܵ�5�Ľ��յ�ַ
#define TX_ADDR         0x10  // ����˵ķ����ַ
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // FIFO״̬

//********************************************************************************************************************//
// ״̬�Ĵ����еı�־λ
#define RX_DR						0x40  // RX FIFO��ֵ��־λ��д��1�����
#define TX_DS						0x20  //��������ж�λ��д��1�����
#define MAX_RT					0x10  //�ﵽ����ط������ж�λ

/********************************************************
�������ܣ�RF24���ų�ʼ��                
��ڲ�������
����  ֵ����
*********************************************************/
void RF24_Init(void);

/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
					value:�Ĵ�����ֵ
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
unsigned char RF24_Write_Reg(unsigned char reg, unsigned char value);
#define RF24_RxFlag_Clr() RF24_Write_Reg(WRITE_REG+STATUS,0x40)

/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
					pBuf:д�����׵�ַ
					bytes:д�����ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
unsigned char RF24_Write_Buf(unsigned char reg, const unsigned char *pBuf, unsigned char bytes);						  					   
#define RF24_FIFO_Write(p,i) RF24_Write_Buf(WR_TX_PLOAD, p, i)
#define RF24_Set_TxAddress(p,i) RF24_Write_Buf(WRITE_REG + TX_ADDR, p, i)
#define RF24_Set_RxAddress(p,i) RF24_Write_Buf(WRITE_REG + RX_ADDR_P0, p, i)

/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��READ_REG��reg��
����  ֵ���Ĵ���ֵ
*********************************************************/
unsigned char RF24_Read_Reg(unsigned char reg);

/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ��READ_REG��reg��
					pBuf:���ջ��������׵�ַ
					bytes:��ȡ�ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
unsigned char RF24_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes);
#define RF24_FIFO_Read(p,i) RF24_Read_Buf(RD_RX_PLOAD,p,i)

/********************************************************
�������ܣ�RF24����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void RF24_RX_Mode(void);			

/********************************************************
�������ܣ�RF24����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void RF24_TX_Mode(void);

/********************************************************
�������ܣ���ȡ��������                       
��ڲ�����rxbuf:�������ݴ���׵�ַ
����  ֵ��0:���յ�����
          1:û�н��յ�����
*********************************************************/
unsigned char RF24_RxPacket(unsigned char *rxbuf);

/********************************************************
�������ܣ�����һ�����ݰ�                      
��ڲ�����txbuf:Ҫ���͵�����
����  ֵ��0x10:�ﵽ����ط�����������ʧ�� 
          0x20:���ͳɹ�            
          0xff:����ʧ��                  
*********************************************************/
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