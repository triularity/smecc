/*
 * @(#) smecc_serialize.c
 *
 * Serialize an ECC to typical SmartMedia(tm) byte form.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */

#include "smecc.h"


/**
 * Serialize ECC data.
 *
 * @param	ecc		The ECC to serialize.
 * @param	buf		The serialization buffer (of
 *				{@link SMECC_SERIALIZED_SIZE} size).
 *
 * @see		smecc_deserialize(uint8_t *)
 */
void
smecc_serialize
(
	smecc_t ecc,
	uint8_t *buf
)
{
	buf[0] = (uint8_t) (SMECC_LP_GET(ecc) >> 8);
	buf[1] = (uint8_t) SMECC_LP_GET(ecc);
	buf[2] = (uint8_t) SMECC_CP_GET(ecc);
}
