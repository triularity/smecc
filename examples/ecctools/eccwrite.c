/*
 * @(#) eccwrite.c
 *
 * Write ECC data.
 * This file is in the Public Domain.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <smecc.h>

#include "eccfile.h"


int
main(int argc, char **argv)
{
	FILE *		ifp;
	FILE *		ofp;
	int		rc;
	smecc_t		ecc;
	eccfile_block_t	block;
	uint8_t		ecc_bytes[SMECC_SERIALIZED_SIZE];
	size_t		count;


	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s <infile> <outfile>\n", argv[0]);
		return 1;
	}

	if((ifp = fopen(argv[1], "rb")) == NULL)
	{
		fprintf(stderr, "Unable to open input: %s\n", argv[1]);
		return 2;
	}

	if((ofp = fopen(argv[2], "wb")) == NULL)
	{
		fprintf(stderr, "Unable to open output: %s\n", argv[2]);
		fclose(ifp);
		return 2;
	}

	rc = 0;

	while((count = fread(block.data, 1, sizeof(block.data), ifp)) != 0)
	{
		block.len = (uint8_t) count;
		memset(&block.data[count], 0, (sizeof(block.data) - count));

		ecc = smecc_compute((uint8_t *) &block);

		smecc_serialize(ecc, ecc_bytes);

		if(fwrite(&block, 1, sizeof(block), ofp) != sizeof(block))
		{
			fprintf(stderr, "Write error\n");
			rc = 3;
			break;
		}

		if(fwrite(ecc_bytes, 1, SMECC_SERIALIZED_SIZE, ofp)
		 != SMECC_SERIALIZED_SIZE)
		{
			fprintf(stderr, "Write error\n");
			rc = 3;
			break;
		}
	}

	if(ferror(ifp))
	{
		fprintf(stderr, "Read error\n");
		rc = 3;
	}

	fclose(ifp);

	if(fclose(ofp) != 0)
	{
		if(rc == 0)
		{
			fprintf(stderr, "Write error\n");
			rc = 3;
		}
	}

	return rc;
}
