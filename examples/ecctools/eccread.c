/*
 * @(#) eccread.c
 *
 * Read ECC data.
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
	smecc_t		check_ecc;
	eccfile_block_t	block;
	uint8_t		ecc_bytes[SMECC_SERIALIZED_SIZE];
	size_t		count;
	smecc_status_t	status;


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

	while((count = fread(&block, 1, sizeof(block), ifp)) != 0)
	{
		if(count != sizeof(block))
		{
			if(!ferror(ifp))
				fprintf(stderr, "Truncated input\n");

			rc = 3;
			break;
		}

		ecc = smecc_compute((uint8_t *) &block);

		if(fread(ecc_bytes, 1, SMECC_SERIALIZED_SIZE, ifp)
		 != SMECC_SERIALIZED_SIZE)
		{
			if(!ferror(ifp))
				fprintf(stderr, "Truncated input\n");

			rc = 3;
			break;
		}

		check_ecc = smecc_deserialize(ecc_bytes);

		status = smecc_check_correct(
			(uint8_t *) &block, ecc, check_ecc);

		if(status == SMECC_STATUS_CORRECTED)
		{
			fprintf(stderr, "Warning: Corrected Error\n");
		}
		else if(status == SMECC_STATUS_UNCORRECTABLE)
		{
			fprintf(stderr, "Data Error (Uncorrectable)\n");
			rc = 4;
			break;
		}
		else if(status != SMECC_STATUS_OK)
		{
			fprintf(stderr, "Unexpected error: code=%d\n", status);
			rc = 3;
			break;
		}

		if(fwrite(block.data, 1, block.len, ofp) != block.len)
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
