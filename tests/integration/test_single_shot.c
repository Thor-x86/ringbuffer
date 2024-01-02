#include <ringbuffer.h>

#include "shared.h"
#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

errno_t
test_single_shot (void)
{
	uint64_t plain_array[10];
	ringbuffer_context_t context
		= ringbuffer_init (plain_array, sizeof (uint64_t), 10, 0, 0);

	unsigned int return_code = EXIT_FAILURE;
	unsigned int read_index;
	unsigned int write_index;

	ringbuffer_write (context, &(example_data[0]));

	read_index = ringbuffer_get_read_index (context);
	if (read_index != 0) {
		printf ("Wrong read index after write\r\n"
				"Now      : %u\r\n"
				"Expected : 0\r\n",
				read_index);
		goto cleanup_test_single_shot;
	}

	write_index = ringbuffer_get_write_index (context);
	if (write_index != 1) {
		printf ("Wrong write index after write\r\n"
				"Now      : %u\r\n"
				"Expected : 1\r\n",
				write_index);
		goto cleanup_test_single_shot;
	}

	if (plain_array[0] != example_data[0]) {
		printf ("Failed to write an element\r\n"
				"Retrieved: 0x%lX\r\n"
				"Expected : 0x%lX\r\n",
				plain_array[0], example_data[0]);
		goto cleanup_test_single_shot;
	}

	puts ("Single-shot write OK");

	uint64_t result = 0xFFFFFFFFFFFFFFFFULL;
	ringbuffer_read (context, &result);

	if (result != example_data[0]) {
		printf ("Failed to read an element\r\n"
				"Result   : 0x%lX\r\n"
				"Expected : 0x%lX\r\n",
				result, example_data[0]);
		goto cleanup_test_single_shot;
	}

	read_index = ringbuffer_get_read_index (context);
	if (read_index != 1) {
		printf ("Wrong read index after read\r\n"
				"Now      : %u\r\n"
				"Expected : 1\r\n",
				read_index);
		goto cleanup_test_single_shot;
	}

	write_index = ringbuffer_get_write_index (context);
	if (write_index != 1) {
		printf ("Wrong write index after read\r\n"
				"Now      : %u\r\n"
				"Expected : 1\r\n",
				write_index);
		goto cleanup_test_single_shot;
	}

	puts ("Single-shot read OK");
	return_code = EXIT_SUCCESS;

cleanup_test_single_shot:
	fflush (stdout);
	ringbuffer_destroy (&context);
	return return_code;
}
