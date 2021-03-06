/*
 * @(#) smecc_check_correct_multi.c
 *
 * Check multiple ECC values, correcting the data if possible on error.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */

#include <stddef.h>
#include <stdint.h>

#include "smecc.h"


/**
 * Check ECC(s), correcting the data if necessary/possible.
 *
 * @param	data		The data buffer (of {@link SMECC_BLOCK_SIZE}
 *				multiple size).
 * @param	length		The length of the data.
 * @param	data_ecc	The ECC(s) calculated from the data.
 * @param	check_ecc	The previously calculated ECC(s).
 * @param	ecc_count	The number of ECC(s).
 *
 * @return	{@link SMECC_STATUS_BADLEN} if {@param length} isn't a
 *		multiple of {@link SMECC_BLOCK_SIZE} or {@param ecc_count}
 *		is too small for {@length}, {@link SMECC_STATUS_OK} if all
 *		the ECCs match, {@link SMECC_STATUS_CORRECTED} if the data
 *		or ECC has a correctable error,
 *		or {@link SMECC_STATUS_UNCORRECTABLE} if there is
 *		an uncorrectable error.
 */
smecc_status_t
smecc_check_correct_multi
(
	uint8_t *data,
	size_t length,
	const smecc_t *data_ecc,
	const smecc_t *check_ecc,
	size_t ecc_count
)
{
	smecc_status_t	status;


	/*
	 * Must be multiple of 256 (for now)
	 */
	if((length & 0xFF) != 0)
		return SMECC_STATUS_BADLEN;

	/*
	 * Must have enough ECCs
	 */
	if((length >> 8) > ecc_count)
		return SMECC_STATUS_BADLEN;


	status = SMECC_STATUS_OK;

	while(length != 0)
	{
		switch(smecc_check_correct(data, *data_ecc, *check_ecc))
		{
			case SMECC_STATUS_UNCORRECTABLE:
				return SMECC_STATUS_UNCORRECTABLE;

			case SMECC_STATUS_CORRECTED:
				status = SMECC_STATUS_CORRECTED;
				break;

			default:
				/* Make compiler happy */
				break;
		}

		data += 256;
		length -= 256;
		data_ecc++;
		check_ecc++;
	}

	return status;
}
