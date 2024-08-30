#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <SPI.h>

#define uint8_t uint8_t
#define uint16_t uint16_t
#define uint32_t uint32_t

//#define COLOR_3

#define SD_CS 16  //D0

#define EPD_BUSY_PIN 2   //D4
#define EPD_MOSI_PIN 13  //D7,SDI
#define EPD_SCK_PIN 14   //D5,SCL
#define EPD_CS_PIN 15    //D8

#define EPD_RST_PIN 10  //SD3
#define EPD_DC_PIN 0    //D3

#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0

#define DEV_Digital_Write(_pin, _value) digitalWrite(_pin, _value == 0 ? LOW : HIGH)
#define DEV_Digital_Read(_pin) digitalRead(_pin)
#define DEV_Delay_ms(__xms) delay(__xms)

/*------------------------------------------------------------------------------------------------------*/
uint8_t DEV_Module_Init(void);
void GPIO_Mode(uint16_t GPIO_Pin, uint16_t Mode);
void DEV_SPI_WriteByte(uint8_t data);
//uint8_t DEV_SPI_ReadByte();
void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t len);

#endif
