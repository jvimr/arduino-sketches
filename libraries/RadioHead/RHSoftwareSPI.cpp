// SoftwareSPI.cpp
// Author: Chris Lapa (chris@lapa.com.au)
// Copyright (C) 2014 Chris Lapa
// Contributed by Chris Lapa

#include <RHSoftwareSPI.h>

RHSoftwareSPI::RHSoftwareSPI(Frequency frequency, BitOrder bitOrder, DataMode dataMode)
    :
    RHGenericSPI(frequency, bitOrder, dataMode)
{
}


uint8_t RHSoftwareSPI::transfer(uint8_t data) 
{
    uint8_t readData;
    uint8_t writeData;
    uint8_t builtReturn;
    uint8_t mask;
    
    if (_bitOrder == BitOrderMSBFirst)
    {
	mask = 0x80;
    }
    else
    {
	mask = 0x01;
    }
    builtReturn = 0;
    readData = 0;

    for (uint8_t count=0; count<8; count++)
    {
	if (data & mask)
	{
	    writeData = HIGH;
	}
	else
	{
	    writeData = LOW;
	}


	if (_clockPhase == 1)
	{
	    // CPHA=1, miso/mosi changing state now
	    digitalWrite(_mosi, writeData);
	    digitalWrite(_sck, ~_clockPolarity);
	    delayPeriod();

	    // CPHA=1, miso/mosi stable now
	    readData = digitalRead(_miso);
	    digitalWrite(_sck, _clockPolarity);
	    delayPeriod();
	}
	else
	{
	    // CPHA=0, miso/mosi changing state now
	    digitalWrite(_mosi, writeData);
	    digitalWrite(_sck, _clockPolarity);
	    delayPeriod();

	    // CPHA=0, miso/mosi stable now
	    readData = digitalRead(_miso);
	    digitalWrite(_sck, ~_clockPolarity);
	    delayPeriod();
	}
			

	if (_bitOrder == BitOrderMSBFirst)
	{
	    mask >>= 1;
	    builtReturn |= (readData << (7 - count));
	}
	else
	{
	    mask <<= 1;
	    builtReturn |= (readData << count);
	}
    }

    digitalWrite(_sck, _clockPolarity);

    return builtReturn;
}

/// Initialise the SPI library
void RHSoftwareSPI::begin()
{
    if (_dataMode == DataMode0 ||
	_dataMode == DataMode1)
    {
	_clockPolarity = LOW;
    }
    else
    {
	_clockPolarity = HIGH;
    }
		
    if (_dataMode == DataMode0 ||
	_dataMode == DataMode2)
    {
	_clockPhase = 0;
    }
    else
    {
	_clockPhase = 1;
    }
    digitalWrite(_sck, _clockPolarity);

    uint8_t delayCounts;
    switch (_frequency)
    {
	case Frequency1MHz:
	    delayCounts = 8;
	    break;

	case Frequency2MHz:
	    delayCounts = 4;
	    break;

	case Frequency4MHz:
	    delayCounts = 2;
	    break;

	case Frequency8MHz:
	    delayCounts = 1;
	    break;

	case Frequency16MHz:
	    delayCounts = 0;
	    break;

    }
}

/// Disables the SPI bus usually, in this case
/// there is no hardware controller to disable.
void RHSoftwareSPI::end() { }

/// Sets the pins used by this SoftwareSPIClass instance.
/// \param[in] miso master in slave out pin used
/// \param[in] mosi master out slave in pin used
/// \param[in] sck clock pin used
void RHSoftwareSPI::setPins(uint8_t miso, uint8_t mosi, uint8_t sck)
{
    _miso = miso;
    _mosi = mosi;
    _sck = sck;

    pinMode(_miso, INPUT);
    pinMode(_mosi, OUTPUT);
    pinMode(_sck, OUTPUT);
    digitalWrite(_sck, _clockPolarity);
}


void RHSoftwareSPI::delayPeriod()
{
    for (volatile uint8_t count; count<_delayCounts; count++)
    {
	__asm__ __volatile__ ("nop");
    }
}

