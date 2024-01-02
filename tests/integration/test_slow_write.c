#include <ringbuffer.h>

#include "shared.h"
#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

errno_t
test_slow_write (void)
{
	uint64_t plain_array[11] = { 0 };
	ringbuffer_context_t context
		= ringbuffer_init (plain_array, sizeof (uint64_t), 11, 0, 0);

	unsigned int return_code = EXIT_FAILURE;
	unsigned int example_index = 0, plain_index = 0;
	unsigned int read_index, write_index;

	for (unsigned int i = 0; i < 100000U; i++) {
		// Only write on odd index, not even index
		if (i % 2) {
			ringbuffer_write (context, &(example_data[example_index]));
			read_index = ringbuffer_get_read_index (context);
			write_index = ringbuffer_get_write_index (context);

			printf (template_report, i, example_index, plain_index, read_index,
					write_index);

			if (plain_array[plain_index] != example_data[example_index]) {
				printf ("\033[5B"
						"Faulty write! It should be 0x%lX not 0x%lX\r\n",
						example_data[example_index], plain_array[plain_index]);
				goto cleanup_test_slow_write;
			}
		}

		uint64_t result = 0xFFFFFFFFFFFFFFFFULL;
		ringbuffer_read (context, &result);
		read_index = ringbuffer_get_read_index (context);
		write_index = ringbuffer_get_write_index (context);

		printf (template_report, i, example_index, plain_index, read_index,
				write_index);

		if (i % 2) {
			if (result != example_data[example_index]) {
				printf ("\033[5B"
						"Faulty read! It should be 0x%lX not 0x%lX\r\n",
						result, plain_array[plain_index]);
				goto cleanup_test_slow_write;
			}

			example_index++;
			example_index %= 16;
			plain_index++;
			plain_index %= 11;
		} else {
			if (result != 0xFFFFFFFFFFFFFFFFULL) {
				puts ("\033[5B"
					  "While waiting write, ringbuffer_read supposed to not "
					  "modify the variable");
				goto cleanup_test_slow_write;
			}
		}
	}

	fputs ("\033[5B", stdout);
	return_code = EXIT_SUCCESS;

cleanup_test_slow_write:
	fflush (stdout);
	ringbuffer_destroy (&context);
	return return_code;
}
