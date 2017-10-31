/*
 * @(#) eccfile.h
 *
 * This file is in the Public Domain.
 */

#ifndef	__eccfile_h
#define	__eccfile_h

#include <stdint.h>

#include <smecc.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct eccfile_block
{
	uint8_t		len;
	uint8_t		data[SMECC_BLOCK_SIZE-1];
} eccfile_block_t;

#ifdef	__cplusplus
}
#endif

#endif	/* __eccfile_h */
