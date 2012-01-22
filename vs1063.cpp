
#include <SPI.h>
#include "vs1063.h"

// read the 16-bit value of a VS10xx register
uint16_t vs_read_register(uint8_t address)
{
    uint16_t resultvalue = 0;
    uint16_t aux = 0;
    vs_deselect_data();
    vs_select_control();
    vs_wait();
    SPI.transfer(VS_READ_COMMAND);
    SPI.transfer(address);
    aux = SPI.transfer(0xff);
    resultvalue = aux << 8;
    aux = SPI.transfer(0xff);
    resultvalue |= aux;
    vs_deselect_control();
    vs_wait();
    return resultvalue;
}

// write VS10xx register
void vs_write_register(uint8_t address, uint8_t highbyte, uint8_t lowbyte)
{
    vs_deselect_data();
    vs_select_control();
    vs_wait();
    delay(2);
    SPI.transfer(VS_WRITE_COMMAND);
    SPI.transfer(address);
    SPI.transfer(highbyte);
    SPI.transfer(lowbyte);
    vs_deselect_control();
    vs_wait();
}

// write VS10xx 16-bit SCI registers
void vs_write_register(uint8_t address, uint16_t value)
{
    uint8_t highbyte;
    uint8_t lowbyte;

    highbyte = (value & 0xff00) >> 8;
    lowbyte = value & 0x00ff;
    vs_write_register(address, highbyte, lowbyte);
}

// read data rams
uint16_t vs_read_wramaddr(uint16_t address)
{
    uint16_t rv = 0;
    vs_write_register(SCI_WRAMADDR, address);
    rv = vs_read_register(SCI_WRAM);
    return rv;
}

// write to data rams
void vs_write_wramaddr(uint16_t address, uint16_t value)
{
    vs_write_register(SCI_WRAMADDR, address);
    vs_write_register(SCI_WRAM, value);
}

// wait for VS_DREQ to get HIGH before sending new data to SPI
void vs_wait()
{
    while (!digitalRead(VS_DREQ)) {
    };
}

// pull the VS10xx Control Chip Select line LOW
void vs_select_control()
{
    digitalWrite(VS_XCS, LOW);
}

// pull the VS10xx Control Chip Select line HIGH
void vs_deselect_control()
{
    digitalWrite(VS_XCS, HIGH);
}

// pull the VS10xx Data Chip Select line LOW
void vs_select_data()
{
    digitalWrite(VS_XDCS, LOW);
}

// pull the VS10xx Data Chip Select line HIGH
void vs_deselect_data()
{
    digitalWrite(VS_XDCS, HIGH);
}

// power up the VS10xx chip
void vs_deassert_xreset()
{
    digitalWrite(VS_XRESET, HIGH);
}

// shutdown the VS10xx chip
void vs_assert_xreset()
{
    digitalWrite(VS_XRESET, LOW);
}

// set up pins
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
    vs_wait();
}

// soft reset of VS10xx (between songs)
void vs_soft_reset()
{
    vs_write_register(SCI_MODE, SM_SDINEW | SM_RESET);
    // set SC_MULT=3.5x, SC_ADD=1.0x
    vs_write_register(SCI_CLOCKF, 0x8800);
    vs_wait();
}

// setup I2S (see page77 of the datasheet of vs1053 )
// also enables blinky lights on the simple dsp evaluation board
void vs_setup_i2s()
{
    //set GPIO0 as output
    vs_write_wramaddr(0xc017, 0x00f0);
    //enable I2S (MCLK enabled, 48kHz sample rate)
    vs_write_wramaddr(0xc040, 0x000c);
}

// Set VS10xx Volume Register
void vs_set_volume(uint8_t leftchannel, uint8_t rightchannel)
{
    vs_write_register(SCI_VOL, leftchannel, rightchannel);
}

