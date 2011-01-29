/***************************************************************************
 *   Copyright (C) 2010 by Christoph Thelen                                *
 *   doc_bacardi@users.sourceforge.net                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "main.h"

#include <string.h>

#include "netx_io_areas.h"
#include "serial_vectors.h"
#include "systime.h"
//#include "uartmon.h"
#include "uartmonitor_commands.h"
#include "uprintf.h"

/*-----------------------------------*/

#define MAX_PACKET_SIZE 256

#define UART_BUFFER_NO_TIMEOUT 0
#define UART_BUFFER_TIMEOUT 1

unsigned char aucPacketBuffer[MAX_PACKET_SIZE];

size_t sizBufferHead;
size_t sizBufferFill;


static unsigned int crc16(unsigned short usCrc, unsigned char ucData)
{
	unsigned int uiCrc;


	uiCrc  = (usCrc >> 8U) | ((usCrc & 0xffU) << 8U);
	uiCrc ^= ucData;
	uiCrc ^= (uiCrc & 0xffU) >> 4U;
	uiCrc ^= (uiCrc & 0x0fU) << 12U;
	uiCrc ^= ((uiCrc & 0xffU) << 4U) << 1U;

	return (unsigned short)uiCrc;
}


static void uart_buffer_init(void)
{
	sizBufferHead = 0;
	sizBufferFill = 0;
}


static int uart_buffer_fill(size_t sizRequestedFillLevel, unsigned int uiTimeoutFlag)
{
	size_t sizWritePosition;
	int iResult;
	unsigned long ulTimeout;
	unsigned int uiHasData;


	/* Get the write position. */
	sizWritePosition = sizBufferHead + sizBufferFill;
	if( sizWritePosition>=MAX_PACKET_SIZE )
	{
		sizWritePosition -= MAX_PACKET_SIZE;
	}

	iResult = 0;

	if( uiTimeoutFlag==0 )
	{
		/* Fillup the buffer to the requested level. */
		while( sizBufferFill<sizRequestedFillLevel )
		{
			/* Write the new byte to the buffer. */
			aucPacketBuffer[sizWritePosition] = SERIAL_GET();
			/* Increase the write position. */
			++sizWritePosition;
			if( sizWritePosition>=MAX_PACKET_SIZE )
			{
				sizWritePosition -= MAX_PACKET_SIZE;
			}
			/* Increase the fill level. */
			++sizBufferFill;
		}
	}
	else
	{
		/* Fillup the buffer to the requested level. */
		while( sizBufferFill<sizRequestedFillLevel )
		{
			/* Wait for new data. */
			ulTimeout = systime_get_ms();
			uiHasData = 0;
			do
			{
				if( systime_elapsed(ulTimeout, 500)!=0 )
				{
					break;
				}

				uiHasData = SERIAL_PEEK();
			} while( uiHasData==0 );

			if( uiHasData==0 )
			{
				iResult = -1;
				break;
			}

			/* Write the new byte to the buffer. */
			aucPacketBuffer[sizWritePosition] = SERIAL_GET();
			/* Increase the write position. */
			++sizWritePosition;
			if( sizWritePosition>=MAX_PACKET_SIZE )
			{
				sizWritePosition -= MAX_PACKET_SIZE;
			}
			/* Increase the fill level. */
			++sizBufferFill;
		}
	}

	return iResult;
}


static unsigned char uart_buffer_get(void)
{
	unsigned char ucByte;


	ucByte = aucPacketBuffer[sizBufferHead];

	++sizBufferHead;
	if( sizBufferHead>=MAX_PACKET_SIZE )
	{
		sizBufferHead -= MAX_PACKET_SIZE;
	}

	--sizBufferFill;

	return ucByte;
}


static unsigned char uart_buffer_peek(size_t sizOffset)
{
	size_t sizReadPosition;
	unsigned char ucByte;


	sizReadPosition = sizBufferHead + sizOffset;
	if( sizReadPosition>=MAX_PACKET_SIZE )
	{
		sizReadPosition -= MAX_PACKET_SIZE;
	}

	ucByte = aucPacketBuffer[sizReadPosition];

	return ucByte;
}






static void uart_loop(void)
{
	unsigned char ucByte;
	size_t sizPacket;
	unsigned short usCrc16;
	size_t sizCrcPosition;
	int iResult;


	/* Collect a complete packet. */

	/* Wait for the start character. */
	do
	{
		uart_buffer_fill(1, UART_BUFFER_NO_TIMEOUT);
		ucByte = uart_buffer_get();
	} while( ucByte!=UARTMON_PACKET_START );

	uprintf("Startchar\n");

	/* Get the size of the data packet in bytes. */
	uart_buffer_fill(2, UART_BUFFER_NO_TIMEOUT);
	sizPacket  = uart_buffer_peek(0);
	sizPacket |= uart_buffer_peek(1) << 8U;

	uprintf("Size: 0x%08x\n", sizPacket);

	/* Is the packet's size valid? */
	if( sizPacket>=4 && sizPacket<=MAX_PACKET_SIZE )
	{
		/* Yes, the packet size is valid. */

		iResult = uart_buffer_fill(sizPacket, UART_BUFFER_TIMEOUT);
		if( iResult==0 )
		{
			/* Loop over all bytes and build the crc16 checksum. */
			/* NOTE: the size information also contains the size itself and the checksum. */
			usCrc16 = 0;
			sizCrcPosition = 1;
			while( sizCrcPosition<sizPacket )
			{
				++sizCrcPosition;
				usCrc16 = crc16(usCrc16, uart_buffer_peek(sizCrcPosition));
			}

			if( usCrc16!=0 )
			{
				uprintf("crc failed: %04x\n", usCrc16);
			}
			else
			{
				/* Ok, the CRC matches! */

				/* TODO: process the packet. */

				uprintf("Received packet (%d bytes): ", sizPacket);
				sizCrcPosition = 1;
				while( sizCrcPosition<sizPacket )
				{
					++sizCrcPosition;
					uprintf(" %02x", uart_buffer_get());
				}
			}
		}
	}
}






void uart_monitor(void)
{
	systime_init();

	uprintf("Hallo!\n");

	uart_buffer_init();

	while(1)
	{
		uart_loop();
	}
}


/*-----------------------------------*/
