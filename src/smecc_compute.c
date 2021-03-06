/*
 * @(#) smecc_compute.c
 *
 * Compute an ECC value.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */

#include <stddef.h>
#include <stdint.h>

#include "smecc.h"
#include "smecc_impl.h"

/*
 * The data parity lookup table.
 * The CP bits line up with the CP part of the ECC.
 *
 * Table Generation:
 *
 *  CP0 = IS_ODD(B6 + B4 + B2 + B0)  [-B-B-B-B]
 *  CP1 = IS_ODD(B7 + B5 + B3 + B1)  [B-B-B-B-]
 *
 *  CP2 = IS_ODD(B5 + B4 + B1 + B0)  [--BB--BB]
 *  CP3 = IS_ODD(B7 + B6 + B3 + B2)  [BB--BB--]
 *
 *  CP4 = IS_ODD(B3 + B2 + B1 + B0)  [----BBBB]
 *  CP5 = IS_ODD(B7 + B6 + B5 + B4)  [BBBB----]
 *
 *  ALL = IS_ODD(B7 + B6 + B5 + B4 + B3 + B2 + B1 + B0)
 *
 *  7   6   5   4   3   2   1   0
 *  CP5 CP4 CP3 CP2 CP1 CP0 --- ALL
 */

#define	SMECC_PARITY_ALL	0x01
#define	SMECC_PARITY_CP		0xFC

static
const uint8_t
parity_table[256] =
{
	0x00, 0x55, 0x59, 0x0C, 0x65, 0x30, 0x3C, 0x69,
	0x69, 0x3C, 0x30, 0x65, 0x0C, 0x59, 0x55, 0x00,
	0x95, 0xC0, 0xCC, 0x99, 0xF0, 0xA5, 0xA9, 0xFC,
	0xFC, 0xA9, 0xA5, 0xF0, 0x99, 0xCC, 0xC0, 0x95,
	0x99, 0xCC, 0xC0, 0x95, 0xFC, 0xA9, 0xA5, 0xF0,
	0xF0, 0xA5, 0xA9, 0xFC, 0x95, 0xC0, 0xCC, 0x99,
	0x0C, 0x59, 0x55, 0x00, 0x69, 0x3C, 0x30, 0x65,
	0x65, 0x30, 0x3C, 0x69, 0x00, 0x55, 0x59, 0x0C,
	0xA5, 0xF0, 0xFC, 0xA9, 0xC0, 0x95, 0x99, 0xCC,
	0xCC, 0x99, 0x95, 0xC0, 0xA9, 0xFC, 0xF0, 0xA5,
	0x30, 0x65, 0x69, 0x3C, 0x55, 0x00, 0x0C, 0x59,
	0x59, 0x0C, 0x00, 0x55, 0x3C, 0x69, 0x65, 0x30,
	0x3C, 0x69, 0x65, 0x30, 0x59, 0x0C, 0x00, 0x55,
	0x55, 0x00, 0x0C, 0x59, 0x30, 0x65, 0x69, 0x3C,
	0xA9, 0xFC, 0xF0, 0xA5, 0xCC, 0x99, 0x95, 0xC0,
	0xC0, 0x95, 0x99, 0xCC, 0xA5, 0xF0, 0xFC, 0xA9,
	0xA9, 0xFC, 0xF0, 0xA5, 0xCC, 0x99, 0x95, 0xC0,
	0xC0, 0x95, 0x99, 0xCC, 0xA5, 0xF0, 0xFC, 0xA9,
	0x3C, 0x69, 0x65, 0x30, 0x59, 0x0C, 0x00, 0x55,
	0x55, 0x00, 0x0C, 0x59, 0x30, 0x65, 0x69, 0x3C,
	0x30, 0x65, 0x69, 0x3C, 0x55, 0x00, 0x0C, 0x59,
	0x59, 0x0C, 0x00, 0x55, 0x3C, 0x69, 0x65, 0x30,
	0xA5, 0xF0, 0xFC, 0xA9, 0xC0, 0x95, 0x99, 0xCC,
	0xCC, 0x99, 0x95, 0xC0, 0xA9, 0xFC, 0xF0, 0xA5,
	0x0C, 0x59, 0x55, 0x00, 0x69, 0x3C, 0x30, 0x65,
	0x65, 0x30, 0x3C, 0x69, 0x00, 0x55, 0x59, 0x0C,
	0x99, 0xCC, 0xC0, 0x95, 0xFC, 0xA9, 0xA5, 0xF0,
	0xF0, 0xA5, 0xA9, 0xFC, 0x95, 0xC0, 0xCC, 0x99,
	0x95, 0xC0, 0xCC, 0x99, 0xF0, 0xA5, 0xA9, 0xFC,
	0xFC, 0xA9, 0xA5, 0xF0, 0x99, 0xCC, 0xC0, 0x95,
	0x00, 0x55, 0x59, 0x0C, 0x65, 0x30, 0x3C, 0x69,
	0x69, 0x3C, 0x30, 0x65, 0x0C, 0x59, 0x55, 0x00
};


#ifdef	SMECC_USE_LOOKUPS
/*
 * Expand bits to the left, with a single bit gap between the original bits.
 *
 * B7 B6 B5 B4 B3 B2 B1 B0 -> 0 B7 0 B6 0 B5 0 B4 0 B3 0 B2 0 B1 0 B0
 */
static
const uint16_t
bitexpand_table[256] =
{
        0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015,
        0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055,
        0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115,
        0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155,
        0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415,
        0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455,
        0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515,
        0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555,
        0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015,
        0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055,
        0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115,
        0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155,
        0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415,
        0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455,
        0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515,
        0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555,
        0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015,
        0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055,
        0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115,
        0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155,
        0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415,
        0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455,
        0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515,
        0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555,
        0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015,
        0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055,
        0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115,
        0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155,
        0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415,
        0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455,
        0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515,
        0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555
};

#define	EXPAND_BITS(x)	bitexpand_table[x]

#else	/* SMECC_USE_LOOKUPS */

#define	EXPAND_BITS(x)	((((x) & 0x80) << (14-7)) \
			| (((x) & 0x40) << (12-6)) \
			| (((x) & 0x20) << (10-5)) \
			| (((x) & 0x10) << ( 8-4)) \
			| (((x) & 0x08) << ( 6-3)) \
			| (((x) & 0x04) << ( 4-2)) \
			| (((x) & 0x02) << ( 2-1)) \
			| (((x) & 0x01) << ( 0-0)))

#endif	/* SMECC_USE_LOOKUPS */


/**
 * Compute an ECC.
 *
 * @param	data		The data (of {@link SMECC_BLOCK_SIZE} size).
 *
 * @return	The computed ECC.
 */
smecc_t
smecc_compute
(
	const uint8_t *data
)
{
	uint8_t		cp;		/* CP{0,1,2,3,4,5} */
	uint8_t		lp_odd;		/* LP{01,03,05,...,13,15} */
	uint8_t		lp_even;	/* LP{00,02,04,...,12,14} */
	size_t		i;
	uint8_t		p;
	uint16_t	lp;		/* LP{00,01,02,...,14,15} */


	cp = 0;
	lp_odd = 0;
	lp_even = 0;

	for(i = 0; i < 256; i++)
	{
		p = parity_table[data[i]];

		cp ^= p;

		if(p & SMECC_PARITY_ALL)
		{
			lp_even ^= (uint8_t) ~i;
			lp_odd ^= (uint8_t) i;
		}
	}

	/*
	 * Interlace bits
	 */
	lp = (EXPAND_BITS(lp_odd) << 1) | EXPAND_BITS(lp_even);

	return (SMECC_CP_MAKE(~cp & SMECC_PARITY_CP) | SMECC_LP_MAKE(~lp))
		| SMECC_FILLER;
}
