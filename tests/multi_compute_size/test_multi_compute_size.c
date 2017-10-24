/*
 * @(#) test_multi_compute_size.c
 *
 * Unit test for multi ECC computation data sizes.
 * This file is in the Public Domain.
 */

#include <stdio.h>
#include <stdint.h>

#include <smecc.h>


uint8_t		test_buffer[SMECC_BLOCK_SIZE*3];
smecc_t		test_ecc[3];


int
main(int argc, char **argv)
{
	int		rc;
	size_t		i;
	smecc_status_t	status;


	rc = 0;

	/*
	 * Block multiples
	 */
	for(i = 0; i < (SMECC_BLOCK_SIZE*3); i++)
	{
		status = smecc_compute_multi(test_buffer, i, test_ecc, 3);

		if(status == SMECC_STATUS_OK)
		{
			if((i % SMECC_BLOCK_SIZE) != 0)
			{
				fprintf(stderr,
					"FAIL - Unexpected success: %zu / 3\n",
					i);

				rc = 1;
			}
		}
		else if(status == SMECC_STATUS_BADLEN)
		{
			if((i % SMECC_BLOCK_SIZE) == 0)
			{
				fprintf(stderr,
					"FAIL - Bas length: %zu / 3\n", i);

				rc = 1;
			}
		}
		else
		{
			fprintf(stderr,
				"FAIL - Unexpected error, status=%u\n",
				status);

			rc = 1;
			break;
		}
	}

	/*
	 * ECC count to data length ratio
	 */
	for(i = 0; i < (SMECC_BLOCK_SIZE*3); i++)
	{
		status = smecc_compute_multi(test_buffer, i, test_ecc, 2);

		if(status == SMECC_STATUS_OK)
		{
			if(((i % SMECC_BLOCK_SIZE) != 0)
			 || (i > (SMECC_BLOCK_SIZE*2)))
			{
				fprintf(stderr,
					"FAIL - Unexpected success: %zu / 2\n",
					i);

				rc = 1;
			}
		}
		else if(status == SMECC_STATUS_BADLEN)
		{
			if(((i % SMECC_BLOCK_SIZE) == 0)
			 && (i <= (SMECC_BLOCK_SIZE*2)))
			{
				fprintf(stderr,
					"FAIL - Bas length: %zu / 2\n", i);

				rc = 1;
			}
		}
		else
		{
			fprintf(stderr,
				"FAIL - Unexpected error, status=%u\n",
				status);

			rc = 1;
			break;
		}
	}

	if(rc == 0)
		fputs("PASSED\n", stderr);

	return rc;
}
