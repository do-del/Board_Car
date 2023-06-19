#include "rf24.h"

unsigned char TX_ADDRESS[TX_ADR_WIDTH] = {0x0A,0x01,0x07,0x0E,0x01};  // ����һ����̬���͵�ַ

/********************************************************
�������ܣ�RF24���ų�ʼ��                
��ڲ�������
����  ֵ����
*********************************************************/
void RF24_Init(void)
{
	SCK_LOW();  //SCK = 0; 													//SPIʱ��������
	CSN_HIGH(); //CSN = 1;
	CE_LOW(); //CE 	= 0;
	//IRQ = 1;
}


/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
					value:�Ĵ�����ֵ
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
unsigned char RF24_Write_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;

	CSN_LOW();  //CSN = 0;                 
	status = SPI_RW(reg);				
	SPI_RW(value);
	CSN_HIGH(); //CSN = 1;  
	
	return(status);
}


/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
					pBuf:д�����׵�ַ
					bytes:д�����ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
unsigned char RF24_Write_Buf(unsigned char reg, const unsigned char *pBuf, unsigned char bytes)
{
	unsigned char status,byte_ctr;

  CSN_LOW();  //CSN = 0;                                  			
  status = SPI_RW(reg);                          
  for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)     
    SPI_RW(*pBuf++);
  CSN_HIGH(); //CSN = 1;                                      	

  return(status);       
}							  					   


/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��READ_REG��reg��
����  ֵ���Ĵ���ֵ
*********************************************************/
unsigned char RF24_Read_Reg(unsigned char reg)
{
 	unsigned char value;

	CSN_LOW();  //CSN = 0;    
	SPI_RW(reg);			
	value = SPI_RW(0);
	CSN_HIGH(); //CSN = 1;              

	return(value);
}


/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ��READ_REG��reg��
					pBuf:���ջ��������׵�ַ
					bytes:��ȡ�ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
unsigned char RF24_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
	unsigned char status,byte_ctr;

  CSN_LOW();  //CSN = 0;                                        
  status = SPI_RW(reg);                           
  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = SPI_RW(0);                   //��ȡ���ݣ����ֽ���ǰ
  CSN_HIGH(); //CSN = 1;                                        

  return(status);    
}


/********************************************************
�������ܣ�RF24����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void RF24_RX_Mode(void)
{
	CE_LOW(); //CE = 0;
	// RF24_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	// �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	// RF24_Write_Reg(WRITE_REG + EN_AA, 0x01);               						// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	// RF24_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);           						// ʹ�ܽ���ͨ��0
	// RF24_Write_Reg(WRITE_REG + RF_CH, 40);                 						// ѡ����Ƶͨ��0x40
	// RF24_Write_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	// RF24_Write_Reg(WRITE_REG + RF_SETUP, 0x0f);            						// ���ݴ�����2Mbps�����书��7dBm
	RF24_Write_Reg(WRITE_REG + CONFIG, 0x0f);              						// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	RF24_Write_Reg(WRITE_REG + STATUS, 0xff);  												//������е��жϱ�־λ
	CE_HIGH();  //CE = 1;                                            									// ����CE���������豸
}						


/********************************************************
�������ܣ�RF24����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void RF24_TX_Mode(void)
{
	CE_LOW(); //CE = 0;
	//RF24_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // д�뷢�͵�ַ
	//RF24_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ

	// RF24_Write_Reg(WRITE_REG + EN_AA, 0x01);       											// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	// RF24_Write_Reg(WRITE_REG + EN_RXADDR, 0x01);   											// ʹ�ܽ���ͨ��0
	// RF24_Write_Reg(WRITE_REG + SETUP_RETR, 0x0a);  											// �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	// RF24_Write_Reg(WRITE_REG + RF_CH, 40);         											// ѡ����Ƶͨ��0x40
	// RF24_Write_Reg(WRITE_REG + RF_SETUP, 0x0f);    											// ���ݴ�����2Mbps�����书��7dBm
	RF24_Write_Reg(WRITE_REG + CONFIG, 0x0e);      											// CRCʹ�ܣ�16λCRCУ�飬�ϵ�
	CE_HIGH();  //CE = 1;
}


/********************************************************
�������ܣ���ȡ��������                       
��ڲ�����rxbuf:�������ݴ���׵�ַ
����  ֵ��0:���յ�����
          1:û�н��յ�����
*********************************************************/
unsigned char RF24_RxPacket(unsigned char *rxbuf)
{
	unsigned char state;
	state = RF24_Read_Reg(STATUS);  			                 //��ȡ״̬�Ĵ�����ֵ    	  
	RF24_Write_Reg(WRITE_REG+STATUS,state);               //���RX_DS�жϱ�־

	if(state & RX_DR)								                           //���յ�����
	{
		RF24_Read_Buf(RD_RX_PLOAD,rxbuf,11); //TX_PLOAD_WIDTH);     //��ȡ����
		RF24_Write_Reg(FLUSH_RX,0xff);					              //���RX FIFO�Ĵ���
		return 0; 
	}	   
	return 1;                                                   //û�յ��κ�����
}


/********************************************************
�������ܣ�����һ�����ݰ�                      
��ڲ�����txbuf:Ҫ���͵�����
����  ֵ��0x10:�ﵽ����ط�����������ʧ�� 
          0x20:���ͳɹ�            
          0xff:����ʧ��                  
*********************************************************/
unsigned char RF24_TxPacket(unsigned char *txbuf)
{
	unsigned char state;
	CE_LOW();//CE=0;																										  //CE���ͣ�ʹ��RF24����
  RF24_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	    //д���ݵ�TX FIFO,32���ֽ�
 	CE_HIGH();  //CE=1;																										  //CE�øߣ�ʹ�ܷ���	   
	
	while(RF24_IRQ == 1);																				  //�ȴ��������
	state = RF24_Read_Reg(STATUS);  											  //��ȡ״̬�Ĵ�����ֵ	   
	RF24_Write_Reg(WRITE_REG+STATUS, state); 								//���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_RT)																			    //�ﵽ����ط�����
	{
		RF24_Write_Reg(FLUSH_TX,0xff);										    //���TX FIFO�Ĵ��� 
		return MAX_RT; 
	}
	if(state&TX_DS)																			      //�������
	{
		return TX_DS;
	}
	return 0XFF;																						  //����ʧ��
}

void RF24_Set_Power(unsigned char p)
{
	unsigned char temp;
	CE_LOW();
	temp = RF24_Read_Reg(RF_SETUP);
	temp &= ~0x07;
	temp |= p;
	RF24_Write_Reg(WRITE_REG+RF_SETUP,temp);
	CE_HIGH();
}

void RF24_Set_Channel(unsigned char ch)
{
	CE_LOW();
	RF24_Write_Reg(WRITE_REG + RF_CH,ch);
	CE_HIGH();
}

void RF24_Set_Rate(unsigned char rate)
{
	unsigned char temp;
	CE_LOW();
	temp = RF24_Read_Reg(RF_SETUP);
	temp &= 0xd7; //0b1101 0111
	temp |= (rate&0x01)<<3;
	temp |= (rate&0x02)<<4;
	RF24_Write_Reg(WRITE_REG+RF_SETUP,temp);
	CE_HIGH();
}

void RF24_Set_P0_Size(unsigned char len)
{
	CE_LOW();
	RF24_Write_Reg(WRITE_REG + RX_PW_P0,len);
	CE_HIGH();
}