
#include <SPI.h>
#include "vs1063.h"

void SPIPutChar(unsigned char data)
{
	SPDR =data;
	while(!(SPSR & (1<<SPIF)));
}

unsigned char SPIGetChar()
{
    unsigned char returned = SPI.transfer(0xFF);
    return returned;            /* Return the received byte */
}

void vs_deassert_xreset()
{
   digitalWrite(VS_XRESET, HIGH);
}

void vs_assert_xreset()
{
   digitalWrite(VS_XRESET, LOW);
}

/** Pull the VS10xx Control Chip Select line Low */
void vs_SelectControl()
{
    digitalWrite(VS_XCS, LOW);
}

/** Pull the VS10xx Control Chip Select line High */
void vs_DeselectControl()
{
    digitalWrite(VS_XCS, HIGH);
}

/** Pull the VS10xx Data Chip Select line Low */
void vs_SelectData()
{
    digitalWrite(VS_XDCS, LOW);
}

/** Pull the VS10xx Data Chip Select line High */
void vs_DeselectData()
{
    digitalWrite(VS_XDCS, HIGH);
}

void vs_wait()
{
    while (!digitalRead(VS_DREQ)) {
    };
}

/** Write VS10xx register */
void vs_WriteRegister(uint8_t address, uint8_t highbyte, uint8_t lowbyte)
{
    vs_DeselectData();
    vs_SelectControl();
    delay(1);
    SPI.transfer(0x02);         //write command
    SPI.transfer(address);
    SPI.transfer(highbyte);
    SPI.transfer(lowbyte);
    vs_wait();
    vs_DeselectControl();
}

/** Read the 16-bit value of a VS10xx register */
uint16_t vs_ReadRegister(uint8_t address)
{
    uint16_t resultvalue = 0;
    uint16_t aux = 0;
    vs_DeselectData();
    vs_SelectControl();
    SPI.transfer(0x03);         //read command
    SPI.transfer(address);
    aux = SPI.transfer(0xff);
    vs_wait();
    resultvalue = aux << 8;
    SPI.transfer(0xff);
    vs_wait();
    resultvalue |= aux;
    vs_DeselectControl();
    return resultvalue;
}

/** Set VS10xx Volume Register */
void vs_SetVolume(uint8_t leftchannel, uint8_t rightchannel)
{
    vs_WriteRegister(SPI_VOL, leftchannel, rightchannel);
}

/** Soft Reset of VS10xx (Between songs) */
void vs_SoftReset()
{
    vs_deassert_xreset();
    // Newmode, Reset, No L1-2
    vs_WriteRegister(SPI_MODE, 0x08, 0x04);
    delay(1);
    // wait for startup
    while (!digitalRead(VS_DREQ)) ;
    // Set clock register, doubler etc
    vs_WriteRegister(SPI_CLOCKF, 0xb3, 0xfe);
    //vs_WriteRegister(SPI_CLOCKF, 0xb8, 0x00);
    delay(1);

    //setup I2S (see page77 of the datasheet of vs1053 )
    //set GPIO0 as output
    vs_WriteRegister(SPI_WRAMADDR, 0xc0, 0x17);
    vs_WriteRegister(SPI_WRAM, 0x00, 0xf0);
    //enable I2S (MCLK enabled, 48kHz sample rate)
    vs_WriteRegister(SPI_WRAMADDR, 0xc0, 0x40);
    vs_WriteRegister(SPI_WRAM, 0x00, 0x0C);

    while (!digitalRead(VS_DREQ)) ;
}

void vs_setup()
{
    pinMode(VS_DREQ, INPUT);
    pinMode(VS_XCS, OUTPUT);
    pinMode(VS_XDCS, OUTPUT);
    pinMode(VS_XRESET, OUTPUT);
    //Note that even if you're not using the SS pin, it must remain set as an output; 
    //otherwise, the SPI interface can be put into slave mode, rendering the library inoperative.
    pinMode(10, OUTPUT);        //the SS of the SPI interface on the arduino board

    vs_deassert_xreset();
}
