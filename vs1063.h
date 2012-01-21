#ifndef __vs1063_h_
#define __vs1063_h_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// digital pins

// Control Chip Select Pin (for accessing SPI Control/Status registers)
#ifndef VS_XCS
#define VS_XCS 8
#endif

// Data Chip Select / BSYNC Pin
#ifndef VS_XDCS
#define VS_XDCS 7
#endif

// Data Request Pin: Player asks for more data
#ifndef VS_DREQ
#define VS_DREQ 6
#endif

// analog pins

// XRESET pin
#ifndef VS_XRESET
#define VS_XRESET A0
#endif

// VS10xx SCI Write Command byte is 0x02
#define VS_WRITE_COMMAND 0x02

// VS10xx SCI Read Command byte is 0x03
#define VS_READ_COMMAND 0x03

/** VS10xx SCI Registers */
#define SPI_MODE 0x0
#define SPI_STATUS 0x1
#define SPI_BASS 0x2
#define SPI_CLOCKF 0x3
#define SPI_DECODE_TIME 0x4
#define SPI_AUDATA 0x5
#define SPI_WRAM 0x6
#define SPI_WRAMADDR 0x7
#define SPI_HDAT0 0x8
#define SPI_HDAT1 0x9
#define SPI_AIADDR 0xa
#define SPI_VOL 0xb
#define SPI_AICTRL0 0xc
#define SPI_AICTRL1 0xd
#define SPI_AICTRL2 0xe
#define SPI_AICTRL3 0xf

#define SM_DIFF		0x01
#define SM_JUMP		0x02
#define SM_RESET	0x04
#define SM_OUTOFWAV	0x08
#define SM_PDOWN	0x10
#define SM_TESTS	0x20
#define SM_STREAM	0x40
#define SM_PLUSV	0x80
#define SM_DACT		0x100
#define SM_SDIORD	0x200
#define SM_SDISHARE	0x400
#define SM_SDINEW	0x800
#define SM_ADPCM        0x1000
#define SM_ADPCM_HP     0x2000


void SPIPutChar(unsigned char data);
unsigned char SPIGetChar();
void vs_SelectControl();
void vs_DeselectControl();
void vs_SelectData();
void vs_DeselectData();
void vs_assert_xreset();
void vs_deassert_xreset();
void vs_wait();

void vs_WriteRegister(uint8_t address, uint8_t highbyte, uint8_t lowbyte);
uint16_t vs_ReadRegister(uint8_t address);
void vs_SetVolume(uint8_t leftchannel, uint8_t rightchannel);
void vs_SoftReset();

void vs_setup();

#endif
