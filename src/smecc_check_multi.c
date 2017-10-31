/*
 * @(#) smecc_check_multi.c
 *
 * Check multiple ECC values.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */

#include <stddef.h>

#include "smecc.h"


/**
 * Check ECC(s).
 *
 * @param	data_ecc	The ECC(s) calculated from the data.
 * @param	check_ecc	The previously calculated ECC(s).
 * @param	ecc_count	The number of ECC(s).
 *
 * @return	{@link SMECC_STATUS_OK} if all the ECCs match,
 *		{@link SMECC_STATUS_CORRECTABLE} if the data has a correctable
 *		error, {@link SMECC_STATUS_CORRECTED} if an ECC has a
 *		correctable error, or {@link SMECC_STATUS_UNCORRECTABLE}
 *		if there is an uncorrectable error.
 */
smecc_status_t
smecc_check_multi
(
	const smecc_t *data_ecc,
	const smecc_t *check_ecc,
	size_t ecc_count
)
{
	smecc_status_t	status;
	size_t		i;


	status = SMECC_STATUS_OK;

	for(i = 0; i < ecc_count; i++)
	{
		switch(smecc_check(*data_ecc, *check_ecc))
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

		data_ecc++;
		check_ecc++;
	}

	return status;
}
