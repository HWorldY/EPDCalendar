
#include "DEV_Config.h"

void GPIO_Config(void) {
  pinMode(EPD_BUSY_PIN, INPUT);
  pinMode(EPD_RST_PIN, OUTPUT);
  pinMode(EPD_DC_PIN, OUTPUT);
  pinMode(EPD_CS_PIN, OUTPUT);
  digitalWrite(EPD_CS_PIN, HIGH);
}

void GPIO_Mode(uint16_t GPIO_Pin, uint16_t Mode) {
  if (Mode == 0) {
    pinMode(GPIO_Pin, INPUT);
  } else {
    pinMode(GPIO_Pin, OUTPUT);
  }
}
/******************************************************************************
function:	Module Initialize, the BCM2835 library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
uint8_t DEV_Module_Init(void) {
  GPIO_Config();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();
  return 0;
}

/******************************************************************************
function:
			SPI read and write
******************************************************************************/
void DEV_SPI_WriteByte(uint8_t data) {
  digitalWrite(EPD_CS_PIN, GPIO_PIN_RESET);
  SPI.transfer(data);
  digitalWrite(EPD_CS_PIN, GPIO_PIN_SET);
}
/*
uint8_t DEV_SPI_ReadByte()
{
    uint8_t j=0xff;
    GPIO_Mode(EPD_MOSI_PIN, 0);
    digitalWrite(EPD_CS_PIN, GPIO_PIN_RESET);
    for (int i = 0; i < 8; i++)
    {
        j = j << 1;
        if (digitalRead(EPD_MOSI_PIN))  j = j | 0x01;
        else                            j = j & 0xfe;
        
        digitalWrite(EPD_SCK_PIN, GPIO_PIN_SET);     
        digitalWrite(EPD_SCK_PIN, GPIO_PIN_RESET);
    }
    digitalWrite(EPD_CS_PIN, GPIO_PIN_SET);
    GPIO_Mode(EPD_MOSI_PIN, 1);
    return j;
}*/

void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t len) {
  for (int i = 0; i < len; i++)
    DEV_SPI_WriteByte(pData[i]);
}
