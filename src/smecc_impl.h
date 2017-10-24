/*
 * @(#) smec_impl.h
 *
 * SmartMedia(tm) compatible ECC (Error Correction Code) implementation.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */

#ifndef	__smec_impl_h
#define	__smec_impl_h

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define	SMECC_LP00_SHIFT	(SMECC_LP_SHIFT+ 0)
#define	SMECC_LP01_SHIFT	(SMECC_LP_SHIFT+ 1)
#define	SMECC_LP02_SHIFT	(SMECC_LP_SHIFT+ 2)
#define	SMECC_LP03_SHIFT	(SMECC_LP_SHIFT+ 3)
#define	SMECC_LP04_SHIFT	(SMECC_LP_SHIFT+ 4)
#define	SMECC_LP05_SHIFT	(SMECC_LP_SHIFT+ 5)
#define	SMECC_LP06_SHIFT	(SMECC_LP_SHIFT+ 6)
#define	SMECC_LP07_SHIFT	(SMECC_LP_SHIFT+ 7)
#define	SMECC_LP08_SHIFT	(SMECC_LP_SHIFT+ 8)
#define	SMECC_LP09_SHIFT	(SMECC_LP_SHIFT+ 9)
#define	SMECC_LP10_SHIFT	(SMECC_LP_SHIFT+10)
#define	SMECC_LP11_SHIFT	(SMECC_LP_SHIFT+11)
#define	SMECC_LP12_SHIFT	(SMECC_LP_SHIFT+12)
#define	SMECC_LP13_SHIFT	(SMECC_LP_SHIFT+13)
#define	SMECC_LP14_SHIFT	(SMECC_LP_SHIFT+14)
#define	SMECC_LP15_SHIFT	(SMECC_LP_SHIFT+15)

#define	SMECC_LP00		(1<<SMECC_LP00_SHIFT)
#define	SMECC_LP01		(1<<SMECC_LP01_SHIFT)
#define	SMECC_LP02		(1<<SMECC_LP02_SHIFT)
#define	SMECC_LP03		(1<<SMECC_LP03_SHIFT)
#define	SMECC_LP04		(1<<SMECC_LP04_SHIFT)
#define	SMECC_LP05		(1<<SMECC_LP05_SHIFT)
#define	SMECC_LP06		(1<<SMECC_LP06_SHIFT)
#define	SMECC_LP07		(1<<SMECC_LP07_SHIFT)
#define	SMECC_LP08		(1<<SMECC_LP08_SHIFT)
#define	SMECC_LP09		(1<<SMECC_LP09_SHIFT)
#define	SMECC_LP10		(1<<SMECC_LP10_SHIFT)
#define	SMECC_LP11		(1<<SMECC_LP11_SHIFT)
#define	SMECC_LP12		(1<<SMECC_LP12_SHIFT)
#define	SMECC_LP13		(1<<SMECC_LP13_SHIFT)
#define	SMECC_LP14		(1<<SMECC_LP14_SHIFT)
#define	SMECC_LP15		(1<<SMECC_LP15_SHIFT)

#define	SMECC_CP0_SHIFT		(SMECC_CP_SHIFT+2+0)
#define	SMECC_CP1_SHIFT		(SMECC_CP_SHIFT+2+1)
#define	SMECC_CP2_SHIFT		(SMECC_CP_SHIFT+2+2)
#define	SMECC_CP3_SHIFT		(SMECC_CP_SHIFT+2+3)
#define	SMECC_CP4_SHIFT		(SMECC_CP_SHIFT+2+4)
#define	SMECC_CP5_SHIFT		(SMECC_CP_SHIFT+2+5)

#define	SMECC_CP0		(1<<SMECC_CP0_SHIFT)
#define	SMECC_CP1		(1<<SMECC_CP1_SHIFT)
#define	SMECC_CP2		(1<<SMECC_CP2_SHIFT)
#define	SMECC_CP3		(1<<SMECC_CP3_SHIFT)
#define	SMECC_CP4		(1<<SMECC_CP4_SHIFT)
#define	SMECC_CP5		(1<<SMECC_CP5_SHIFT)

#define	SMECC_VALIDATE_MASK	(SMECC_LP00|SMECC_LP02|SMECC_LP04|SMECC_LP06|SMECC_LP08|SMECC_LP10|SMECC_LP12|SMECC_LP14|SMECC_CP0|SMECC_CP2|SMECC_CP4)


int				_smecc_bitcount(uint32_t value);


#if	defined(__GNUC__) && !defined(SMCECC_USE_LOOKUPS)
#define	_smecc_bitcount(value)	__builtin_popcount(value)
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* __smec_impl_h */
