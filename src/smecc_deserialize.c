/*
 * @(#) smecc_deserialize.c
 *
 * Deserialize an ECC from typical SmartMedia(tm) byte form.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */

#include "smecc.h"


smecc_t
smecc_deserialize
(
	const uint8_t *buf
)
{
	return SMECC_LP_MAKE((buf[0] << 8) | buf[1]) | SMECC_CP_MAKE(buf[2]);
}
