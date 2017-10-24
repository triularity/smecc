/*
 * @(#) smecc_bitcount.c
 *
 * Get the number of set bits in an integer.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */
#include <stdint.h>

#include "smecc_impl.h"

#ifdef	SMECC_USE_LOOKUPS
static
const uint8_t	bitcount_table8[256] =
{
	0x00, 0x01, 0x01, 0x02, 0x01, 0x02, 0x02, 0x03,
	0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04,
	0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04,
	0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,
	0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04,
	0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,
	0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,
	0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,
	0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04,
	0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,
	0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,
	0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,
	0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,
	0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,
	0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,
	0x04, 0x05, 0x05, 0x06, 0x05, 0x06, 0x06, 0x07,
	0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04,
	0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,
	0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,
	0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,
	0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,
	0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,
	0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,
	0x04, 0x05, 0x05, 0x06, 0x05, 0x06, 0x06, 0x07,
	0x02, 0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05,
	0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,
	0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,
	0x04, 0x05, 0x05, 0x06, 0x05, 0x06, 0x06, 0x07,
	0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x05, 0x06,
	0x04, 0x05, 0x05, 0x06, 0x05, 0x06, 0x06, 0x07,
	0x04, 0x05, 0x05, 0x06, 0x05, 0x06, 0x06, 0x07,
	0x05, 0x06, 0x06, 0x07, 0x06, 0x07, 0x07, 0x08
};


int
_smecc_bitcount
(
	uint32_t value
)
{
	int	count;


#if	0
		count = bitcount_table8[(value >> 24) & 0xFF]
			+ bitcount_table8[(value >> 16) & 0xFF]
			+ bitcount_table8[(value >> 8) & 0xFF]
			+ bitcount_table8[value & 0xFF];
#else
	count = 0;

	while(value != 0)
	{
		count += bitcount_table8[value & 0xFF];
		value >>= 8;
	}
#endif

	return count;
}
#else	/* SMECC_USE_LOOKUPS */


#ifndef	_smecc_bitcount
/*
 * Fallback implementation, if not a macro
 */
static
const uint8_t	bitcount_table4[16] =
{
	0x00, 0x01, 0x01, 0x02, 0x01, 0x02, 0x02, 0x03,
	0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04
};


int
_smecc_bitcount
(
	uint32_t value
)
{
	int	count;


	count = 0;

	while(value != 0)
	{
		count += bitcount_table4[value & 0xF];
		value >>= 4;
	}

	return count;
}
#endif	/* !_smecc_bitcount */

#endif	/* SMECC_USE_LOOKUPS */
