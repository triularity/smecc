/*
 * @(#) smecc.h
 *
 * SmartMedia(tm) compatible ECC (Error Correction Code) routines.
 *
 * Copyright (c) 2017, Chad M. Fraleigh.  All rights reserved.
 * http://www.triularity.org/
 */

#ifndef	__smecc_h
#define	__smecc_h

#include <stddef.h>
#include <stdint.h>

/*
 * DLL Export Modifier
 */
#if	defined(SMECC_LINK_SHARED) && defined(_WIN32)
#ifdef	smecc_EXPORTS
#define _SMECC_LIBAPI		__declspec(dllexport)
#else
#define _SMECC_LIBAPI		__declspec(dllimport)
#endif
#else	/* SMECC_LINK_SHARED && _WIN32 */
#define _SMECC_LIBAPI		/* */
#endif	/* SMECC_LINK_SHARED && _WIN32 */

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * ECC specification (v2.1)
 */
#define	SMARTMEDIA_ECC_SPEC		0x00020001

/*
 * Library version (v0.9.0)
 */
#define	SMECC_LIB_VERSION_MAJOR		0
#define	SMECC_LIB_VERSION_MINOR		9
#define	SMECC_LIB_VERSION_PATCH		0
#define	SMECC_LIB_VERSION		((SMECC_LIB_VERSION_MAJOR << 16) | (SMECC_LIB_VERSION_MINOR << 8) | SMECC_LIB_VERSION_PATCH)

/*
 * The per-ECC block size
 */
#define	SMECC_BLOCK_SIZE		256

/*
 * The size of ECC in byte form
 */
#define	SMECC_SERIALIZED_SIZE		3

typedef enum
{
	SMECC_STATUS_OK,		/* No errors */
	SMECC_STATUS_CORRECTABLE,	/* Data error and is correctable */
	SMECC_STATUS_CORRECTED,		/* Data error was corrected */
	SMECC_STATUS_UNCORRECTABLE,	/* Data is uncorrectable */
	SMECC_STATUS_BADLEN		/* Invalid data/ecc lengths */
} smecc_status_t;

/*
 * ECC data
 */
typedef uint32_t		smecc_t;

#define	SMECC_CP_SHIFT		0
#define	SMECC_CP_MASK		0x000000FF
#define	SMECC_CP_GET(ecc)	(((ecc) & SMECC_CP_MASK) >> SMECC_CP_SHIFT)
#define	SMECC_CP_MAKE(cp)	(((cp) << SMECC_CP_SHIFT) & SMECC_CP_MASK)

#define	SMECC_LP_SHIFT		8
#define	SMECC_LP_MASK		0x00FFFF00
#define	SMECC_LP_GET(ecc)	(((ecc) & SMECC_LP_MASK) >> SMECC_LP_SHIFT)
#define	SMECC_LP_MAKE(lp)	(((lp) << SMECC_LP_SHIFT) & SMECC_LP_MASK)

#define	SMECC_FILLER		0x00000003


_SMECC_LIBAPI
smecc_status_t			smecc_check(
					smecc_t data_ecc,
					smecc_t check_ecc);

_SMECC_LIBAPI
smecc_status_t			smecc_check_correct(
					uint8_t *data,
					smecc_t data_ecc,
					smecc_t check_ecc);

_SMECC_LIBAPI
smecc_status_t			smecc_check_correct_multi(
					uint8_t *data,
					size_t length,
					const smecc_t *data_ecc,
					const smecc_t *check_ecc,
					size_t ecc_count);

_SMECC_LIBAPI
smecc_status_t			smecc_check_multi(
					const smecc_t *data_ecc,
					const smecc_t *check_ecc,
					size_t ecc_count);

_SMECC_LIBAPI
smecc_t				smecc_compute(
					const uint8_t *data);

_SMECC_LIBAPI
smecc_status_t			smecc_compute_multi(
					const uint8_t *data,
					size_t length,
					smecc_t *ecc,
					size_t ecc_count);

_SMECC_LIBAPI
smecc_t				smecc_deserialize(
					const uint8_t *buf);

_SMECC_LIBAPI
void				smecc_serialize(
					smecc_t ecc,
					uint8_t *buf);

#ifdef	__cplusplus
}
#endif

#undef	_SMECC_LIBAPI

#endif	/* __smecc_h */
