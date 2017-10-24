/*
 * @(#) smecc_check.c
 *
 * Check ECC values.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */

#include "smecc.h"
#include "smecc_impl.h"


smecc_status_t
smecc_check
(
	smecc_t data_ecc,
	smecc_t check_ecc
)
{
	smecc_t		xor;


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
		return SMECC_STATUS_CORRECTABLE;

	/*
	 * Correctable ECC?
	 */
	if(_smecc_bitcount(xor) == 1)
		return SMECC_STATUS_CORRECTED;

	return SMECC_STATUS_UNCORRECTABLE;
}
