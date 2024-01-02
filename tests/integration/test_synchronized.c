#include <ringbuffer.h>

#include "shared.h"
#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

errno_t
test_synchronized (void)
{
	uint64_t plain_array[11] = { 0 };
	ringbuffer_context_t context
		= ringbuffer_init (plain_array, sizeof (uint64_t), 11, 0, 0);

	unsigned int return_code = EXIT_FAILURE;
	unsigned int example_index, plain_index, read_index, write_index,
		expected_read_index, expected_write_index;

	for (unsigned int i = 0; i < 100000U; i++) {
		example_index = i % 16;
		plain_index = i % 11;

		ringbuffer_write (context, &(example_data[example_index]));
		read_index = ringbuffer_get_read_index (context);
		write_index = ringbuffer_get_write_index (context);
		expected_read_index = plain_index;
		expected_write_index = (i + 1) % 11;

		printf (template_report, i, example_index, plain_index, read_index,
				write_index);

		if (read_index != expected_read_index) {
			printf ("\033[5B"
					"Wrong read index after write! It should be %u\r\n",
					expected_read_index);
			goto cleanup_test_synchronized;
		}

		if (write_index != expected_write_index) {
			printf ("\033[5B"
					"Wrong write index after write! It should be %u\r\n",
					expected_write_index);
			goto cleanup_test_synchronized;
		}

		if (plain_array[plain_index] != example_data[example_index]) {
			printf ("\033[5B"
					"Faulty write! It should be 0x%lX not 0x%lX\r\n",
					example_data[example_index], plain_array[plain_index]);
			goto cleanup_test_synchronized;
		}

		uint64_t result = 0xFFFFFFFFFFFFFFFFULL;
		ringbuffer_read (context, &result);
		read_index = ringbuffer_get_read_index (context);
		write_index = ringbuffer_get_write_index (context);
		expected_read_index = expected_write_index;

		printf (template_report, i, example_index, plain_index, read_index,
				write_index);

		if (read_index != expected_read_index) {
			printf ("\033[5B"
					"Wrong read index after read! It should be %u\r\n",
					expected_read_index);
			goto cleanup_test_synchronized;
		}

		if (write_index != expected_write_index) {
			printf ("\033[5B"
					"Wrong write index after read! It should be %u\r\n",
					expected_write_index);
			goto cleanup_test_synchronized;
		}

		if (result != example_data[example_index]) {
			printf ("\033[5B"
					"Faulty read! It should be 0x%lX not 0x%lX\r\n",
					result, plain_array[plain_index]);
			goto cleanup_test_synchronized;
		}
	}

	fputs ("\033[5B", stdout);
	return_code = EXIT_SUCCESS;

cleanup_test_synchronized:
	fflush (stdout);
	ringbuffer_destroy (&context);
	return return_code;
}
