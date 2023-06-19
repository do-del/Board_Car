#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneButton.h>
#include "rf24.h"
#include "key.h"
#include "channel.h"

#define LED_PIN 4
#define BUZZ_PIN 6

#define LINE_LENGTH 48
#define CIRCLE_R 24
#define LEFT_CENTER_X 26
#define LEFT_CENTER_Y 38
#define RIGHT_CENTER_X 102
#define RIGHT_CENTER_Y 38

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void boot_page();
void home_page();

uint8_t tx[11] = {0};
uint8_t rx[11] = {0};
bool connect_flag = false;
#define CONNECT_TX 1
#define CONNECT_RX 0
uint8_t connect_state = CONNECT_TX; 
void connect_check();

uint16_t tim1_cnt = 0;
uint16_t rx_count = 0;
void timer1_config();
ISR(TIMER1_COMPA_vect)
{
  btn_left.tick();
  btn_right.tick();
  //rx_count--;
  // tim1_cnt++;
  if(connect_state)
  {
    // if(tim1_cnt>=20)
    // {
    //   tim1_cnt = 0;
      RF24_TX_Mode();
      tx[0] = 0x44;
      tx[1] = map(ch_value.left_ver,0,1023,0,255);
      tx[2] = map(ch_value.left_hor,0,1023,0,255);
      tx[3] = map(ch_value.right_ver,0,1023,0,255);
      tx[4] = map(ch_value.right_hor,0,1023,0,255);
      tx[5] = ch_value.left_key;
      tx[6] = ch_value.right_key;
      tx[7] = 'D';
      tx[8] = 'F';
      tx[9] = 'L';
      tx[10] = 'Y';
      CE_LOW();
      RF24_Write_Buf(WR_TX_PLOAD, tx, 11);	    //写数据到TX FIFO,32个字节
      CE_HIGH();
      //delay(1);
      rx_count = 50;
      RF24_RX_Mode();
      connect_state = CONNECT_RX;
    // }
  }
  else
  {
    if(digitalRead(IRQ_PIN) == LOW)
    {
      RF24_Read_Buf(RD_RX_PLOAD,rx,11);     //读取数据
      CE_LOW();
      RF24_Write_Reg(WRITE_REG+STATUS,0x40);
      CE_HIGH();
      if(rx[0] == 'O' && rx[1] == 'K')
      {
        connect_flag = true;        
        //digitalWrite(LED_PIN,LOW);
        //Serial.write(rx[0]);
        connect_state = CONNECT_TX;
      }
    }
    rx_count--;
    if(rx_count == 0)
    {
      connect_flag = false; 
      connect_state = CONNECT_TX;
    } 
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,LOW);
  pinMode(BUZZ_PIN,OUTPUT);

  key_init();
  channel_init();
  RF24_Init();
  RF24_Write_Reg(WRITE_REG + EN_AA,0x00);
	RF24_Write_Reg(WRITE_REG+EN_RXADDR,0x01);
	RF24_Write_Reg(WRITE_REG+SETUP_RETR,0x00);
	RF24_TX_Mode();
	RF24_Set_Channel(66);
	RF24_Set_Power(RF_PWR_0);
	RF24_Set_P0_Size(11);
  RF24_Set_Rate(RATE_1M_BPS);
	RF24_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
	RF24_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.fillRoundRect(14, 12, 100, 40,12,SSD1306_INVERSE);
  display.display();
  tim1_cnt = 49;
  while(tim1_cnt--)
  {
    display.clearDisplay();
    boot_page();
    display.display();
  }
  delay(200);
  Serial.println("wait for connect...");
  //connect_check();
  Serial.println("connect done");
  //RF24_RX_Mode();
  digitalWrite(LED_PIN,HIGH);
  timer1_config(); //Timer1配置1ms中断
  
}

void loop() {
  // put your main code here, to run repeatedly:

  channel_update();
  display.clearDisplay();
  home_page();
  display.display();

  // RF24_TX_Mode();
  // tx[0] = 0x44;
  // tx[1] = map(ch_value.left_ver,0,1023,0,255);
  // tx[2] = map(ch_value.left_hor,0,1023,0,255);
  // tx[3] = map(ch_value.right_ver,0,1023,0,255);
  // tx[4] = map(ch_value.right_hor,0,1023,0,255);
  // tx[5] = ch_value.left_key;
  // tx[6] = ch_value.right_key;
  // tx[7] = 'D';
  // tx[8] = 'F';
  // tx[9] = 'L';
  // tx[10] = 'Y';
  // CE_LOW();
  // RF24_Write_Buf(WR_TX_PLOAD, tx, 11);	    //写数据到TX FIFO,32个字节
  // CE_HIGH();
  // delay(1);
  // RF24_RX_Mode();
  // uint8_t n = 50;    
  // while(n--)
  // {
  //   delay(1);
  //   if(digitalRead(IRQ_PIN)==LOW)
  //   {
  //     digitalWrite(LED_PIN,LOW);
  //     RF24_Read_Buf(RD_RX_PLOAD,rx,11);     //读取数据
  //     CE_LOW();
  //     RF24_Write_Reg(WRITE_REG+STATUS,0x40);
  //     CE_HIGH();
  //     if(rx[0] == 'O' && rx[1] == 'K')
  //     {
  //       Serial.println("Send");
  //     }   
  //   }
  // }

}

void timer1_config()
{
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 1999;
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (1<<CS11);
  TIMSK1 |= (1<<OCIE1A);
}

void connect_check()
{
  uint8_t n = 0;

  tx[0] = 'D';
  tx[1] = 'e';
  tx[2] = 'l';
  tx[3] = '-';
  tx[4] = 'f';
  tx[5] = 'l';
  tx[6] = 'y';
  tx[7] = 'e';
  tx[8] = 'r';
  tx[9] = '-';
  tx[10] = '-';

  RF24_Set_Channel(66);
  RF24_Set_P0_Size(11);
  RF24_Set_TxAddress(TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
	RF24_Set_RxAddress(TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
  while(!connect_flag)
  {
    RF24_TX_Mode();
    RF24_FIFO_Write(tx, 11);
    delay(1);
    RF24_RX_Mode();   
    while(1)
    {
      delay(1);
      if(!RF24_RxPacket(rx))
      {
        //Serial.println("del");
        if(rx[0] == 'O' && rx[1] == 'K')
        {
          //connect_flag = 1;
          Serial.print(rx[0]);
          Serial.print(rx[1]);
        }
      }
      n++;
      if(n>50)
      {
        n = 0;
        break;
      }
    }
  }
}

void boot_page()
{
  display.fillRoundRect(14, 12, 100, 40,12,SSD1306_WHITE);
  display.setTextSize(3);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(21,22);
  display.cp437(true);
  display.print(F("D-FLY"));
  display.fillRect(114-tim1_cnt * 2, 12 , tim1_cnt * 2, 40,SSD1306_BLACK);
}

void home_page()
{
  display.fillRoundRect(0,0,37,12,3, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(4,2);
  display.print("D-FLY");
  display.setCursor(70, 2);
  display.setTextColor(SSD1306_WHITE);
  if(connect_flag)
  {
    display.print("connected");
  }
  else
  {
    display.print("unconnect");
  }

  uint8_t x1,y1,x2,y2,x3,y3;
  display.drawCircle(LEFT_CENTER_X, LEFT_CENTER_Y, CIRCLE_R, SSD1306_WHITE);
  x1 = LEFT_CENTER_X-CIRCLE_R;
  y1 = LEFT_CENTER_Y;
  x2 = x1+6;
  y2 = y1+3;
  x3 = x2;
  y3 = y1-3;
  display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
  x1 = LEFT_CENTER_X+CIRCLE_R;
  x2 = x1 - 6;
  x3 = x2;
  display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
  x1 = LEFT_CENTER_X;
  y1 = LEFT_CENTER_Y - CIRCLE_R;
  x2 = x1 - 3;
  y2 = y1 + 6;
  x3 = x1 + 3;
  y3 = y2;
  display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
  y1 = LEFT_CENTER_Y + CIRCLE_R;
  y2 = y1 - 6;
  y3 = y2;
  display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
  display.fillCircle(LEFT_CENTER_X, LEFT_CENTER_Y, 2, SSD1306_WHITE);

  display.drawCircle(RIGHT_CENTER_X, RIGHT_CENTER_Y, CIRCLE_R,SSD1306_WHITE);
  x1 = RIGHT_CENTER_X-CIRCLE_R;
  y1 = RIGHT_CENTER_Y;
  x2 = x1+6;
  y2 = y1+3;
  x3 = x2;
  y3 = y1-3;
  display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
  x1 = RIGHT_CENTER_X+CIRCLE_R;
  x2 = x1 - 6;
  x3 = x2;
  display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
  x1 = RIGHT_CENTER_X;
  y1 = RIGHT_CENTER_Y - CIRCLE_R;
  x2 = x1 - 3;
  y2 = y1 + 6;
  x3 = x1 + 3;
  y3 = y2;
  display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
  y1 = RIGHT_CENTER_Y + CIRCLE_R;
  y2 = y1 - 6;
  y3 = y2;
  display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
  display.fillCircle(RIGHT_CENTER_X, RIGHT_CENTER_Y, 2, SSD1306_WHITE);
  
  x1 = LEFT_CENTER_X;
  x1 += map(ch_value.left_hor,0,1023,CIRCLE_R-5,-CIRCLE_R+5);
  y1 = LEFT_CENTER_Y;
  y1 += map(ch_value.left_ver,0,1023,CIRCLE_R-5,-CIRCLE_R+5);
  display.drawCircle(x1, y1, 5, SSD1306_WHITE);

  x2 = RIGHT_CENTER_X;
  x2 += map(ch_value.right_hor,0,1023,-CIRCLE_R+5,CIRCLE_R-5);
  y2 = RIGHT_CENTER_Y;
  y2 += map(ch_value.right_ver,0,1023,CIRCLE_R-5,-CIRCLE_R+5);
  display.drawCircle(x2, y2, 5, SSD1306_WHITE);
}
