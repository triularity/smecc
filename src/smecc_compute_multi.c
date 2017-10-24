/*
 * @(#) smecc_compute_multi.c
 *
 * Compute multiple ECC values.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */

#include <stddef.h>
#include <stdint.h>

#include "smecc.h"


smecc_status_t
smecc_compute_multi
(
	const uint8_t *data,
	size_t length,
	smecc_t *ecc,
	size_t ecc_count
)
{
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

	while(length != 0)
	{
		*ecc = smecc_compute(data);

		data += 256;
		length -= 256;
		ecc++;
	}

	return SMECC_STATUS_OK;
}
