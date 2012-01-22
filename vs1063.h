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

// VS10xx SCI read and write commands
#define VS_WRITE_COMMAND    0x02
#define VS_READ_COMMAND     0x03

// VS10xx SCI Registers
#define SCI_MODE            0x0
#define SCI_STATUS          0x1
#define SCI_BASS            0x2
#define SCI_CLOCKF          0x3
#define SCI_DECODE_TIME     0x4
#define SCI_AUDATA          0x5
#define SCI_WRAM            0x6
#define SCI_WRAMADDR        0x7
#define SCI_HDAT0           0x8
#define SCI_HDAT1           0x9
#define SCI_AIADDR          0xa
#define SCI_VOL             0xb
#define SCI_AICTRL0         0xc
#define SCI_AICTRL1         0xd
#define SCI_AICTRL2         0xe
#define SCI_AICTRL3         0xf

// SCI_MODE
#define SM_DIFF		        0x0001
#define SM_LAYER12	        0x0002
#define SM_RESET	        0x0004
#define SM_CANCEL	        0x0008
#define SM_PDOWN	        0x0010  // ? undocumented
#define SM_TESTS	        0x0020
#define SM_STREAM	        0x0040  // ? undocumented
#define SM_PLUSV	        0x0080  // ? undocumented
#define SM_DACT		        0x0100
#define SM_SDIORD	        0x0200
#define SM_SDISHARE	        0x0400
#define SM_SDINEW	        0x0800
#define SM_ENCODE           0x1000
//#define SM_UNKNOWN        0x2000
#define SM_LINE1            0x4000
#define SM_CLK_RANGE        0x8000

// SCI_STATUS
#define SS_REFERENCE_SEL    0x0001
#define SS_AD_CLOCK         0x0002
#define SS_APDOWN1          0x0004
#define SS_APDOWN2          0x0008
#define SS_VER1             0x0010
#define SS_VER2             0x0020
#define SS_VER3             0x0040
#define SS_VER4             0x0080
//#define SS_UNKNOWN        0x0100
//#define SS_UNKNOWN        0x0200
#define SS_VCM_DISABLE      0x0400
#define SS_VCM_OVERLOAD     0x0800
#define SS_SWING1           0x1000
#define SS_SWING2           0x2000
#define SS_SWING3           0x4000
#define SS_DO_NOT_JUMP      0x8000

// parametric_x addresses translated to WRAMADDR
#define endFillByte         0xc0c6
#define ogg_gain_offset     0xc0ea

uint16_t vs_read_register(uint8_t address);
void vs_write_register(uint8_t address, uint8_t highbyte, uint8_t lowbyte);
void vs_write_register(uint8_t address, uint16_t value);

uint16_t vs_read_wramaddr(uint16_t address);
void vs_write_wramaddr(uint16_t address, uint16_t value);

void vs_wait();

void vs_select_control();
void vs_deselect_control();
void vs_select_data();
void vs_deselect_data();
void vs_assert_xreset();
void vs_deassert_xreset();

void vs_setup();
void vs_setup_i2s();
void vs_soft_reset();
void vs_set_volume(uint8_t leftchannel, uint8_t rightchannel);

#endif
