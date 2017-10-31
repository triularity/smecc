/*
 * @(#) smecc_check_correct.c
 *
 * Check ECC values, correcting the data if possible on error.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */

#include <stddef.h>
#include <stdint.h>

#include "smecc.h"
#include "smecc_impl.h"


/**
 * Check an ECC, correcting the data if necessary/possible.
 *
 * @param	data		The data buffer (of {@link SMECC_BLOCK_SIZE}
 *				size).
 * @param	data_ecc	The ECC calculated from the data.
 * @param	check_ecc	The previously calculated ECC.
 *
 * @return	{@link SMECC_STATUS_OK} if the ECCs match,
 *		{@link SMECC_STATUS_CORRECTED} if the data or ECC has a
 *		correctable error, or {@link SMECC_STATUS_UNCORRECTABLE}
 *		if there is an uncorrectable error.
 */
smecc_status_t
smecc_check_correct
(
	uint8_t *data,
	smecc_t data_ecc,
	smecc_t check_ecc
)
{
	smecc_t		xor;
	size_t		offset;
	uint32_t	bit;


	xor = data_ecc ^ check_ecc;

	/*
	 * No errors?
	 */
	if(xor == 0)
		return SMECC_STATUS_OK;

	/*
	 * Correctable Data?
	 */
	if(((xor ^ (xor >> 1)) & SMECC_VALIDATE_MASK) == SMECC_VALIDATE_MASK)
	{
		offset = ((xor >> (SMECC_LP01_SHIFT-0)) & 0x01)
			| ((xor >> (SMECC_LP03_SHIFT-1)) & 0x02)
			| ((xor >> (SMECC_LP05_SHIFT-2)) & 0x04)
			| ((xor >> (SMECC_LP07_SHIFT-3)) & 0x08)
			| ((xor >> (SMECC_LP09_SHIFT-4)) & 0x10)
			| ((xor >> (SMECC_LP11_SHIFT-5)) & 0x20)
			| ((xor >> (SMECC_LP13_SHIFT-6)) & 0x40)
			| ((xor >> (SMECC_LP15_SHIFT-7)) & 0x80);

		bit = ((xor >> (SMECC_CP1_SHIFT-0)) & 0x01)
			| ((xor >> (SMECC_CP3_SHIFT-1)) & 0x02)
			| ((xor >> (SMECC_CP5_SHIFT-2)) & 0x04);

		data[offset] ^= (uint8_t) (1 << bit);

		return SMECC_STATUS_CORRECTED;
	}

	/*
	 * Correctable ECC?
	 */
	if(_smecc_bitcount(xor) == 1)
		return SMECC_STATUS_CORRECTED;

	return SMECC_STATUS_UNCORRECTABLE;
}
