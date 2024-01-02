/**
 * @example pager
 *
 * @details
 * In this example program, it will demonstrate how ringbuffer could be
 * implemented in a pager. User could take a sample text then point that text to
 * this program via command argument as below
 *
 * @code{.sh}
 * ./pager Blindsight.txt
 * @endcode
 *
 * Where "Blindsight.txt" is a sample text that supplied in this repository. You
 * can change it to any text you want.
 *
 * ---
 *
 * ## main.c
 *
 * @snippet pager/main.c Pager Main Source Code
 */

//! [Pager Main Source Code]

#include <errno.h>
#include <ringbuffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Sleep function in Windows is not POSIX compliant, so we need to wrap it
#if __has_include(<windows.h>)
	#include <Windows.h>
	#define SLEEP(t) Sleep (t)
#else
	#include <unistd.h>
	#define SLEEP(t) usleep (t * 1000)
#endif

int
main (int arg_count, char **args)
{
	if (arg_count != 2) {
		char *exec_name = "./pager";
		if (arg_count)
			exec_name = args[0];

		if (arg_count > 1)
			fputs ("Wrong usage!\r\n", stderr);
		else
			fputs ("Please provide <path_to_file>\r\n", stderr);

		printf ("USAGE:\r\n"
				"    %s <path_to_file>\r\n",
				exec_name);

		return EINVAL;
	}

	char plain_array[80];
	ringbuffer_context_t context;
	FILE *text_file;
	int return_code = EXIT_FAILURE;

	if (strcmp (args[1], "-")) {
		text_file = fopen (args[1], "rt");
	} else {
		text_file = stdin;
	}

	if (!text_file) {
		fprintf (stderr, "Cannot open the file because '%s' reason\r\n",
				 strerror (errno));
		return errno;
	}

	context = ringbuffer_init (plain_array, sizeof (char), sizeof (plain_array),
							   0, 0);
	if (!context) {
		return_code = errno;
		if (return_code) {
			fprintf (stderr, "Cannot open the file because '%s' reason\r\n",
					 strerror (return_code));
			goto cleanup_after_open;
		}

		return_code = EXIT_FAILURE;
		fputs ("Something went wrong in ringbuffer module, please check the "
			   "params\r\n",
			   stderr);
		goto cleanup_after_open;
	}

	memset (plain_array, ' ', sizeof (plain_array));

	do {
		int each_letter = fgetc (text_file);
		if (each_letter == EOF) {
			if (feof (text_file))
				break;

			return_code = errno;
			if (return_code < 1) {
				fprintf (stderr,
						 "Failed to read \"%s\" for unknown reason\033[0K\r\n",
						 args[1]);
				return_code = EXIT_FAILURE;
				break;
			}

			fprintf (stderr,
					 "Failed to read \"%s\" because '%s' reason\033[0K\r\n",
					 args[1], strerror (return_code));
			break;
		}

		if (each_letter == '\n')
			each_letter = ' ';
		else if (each_letter < 32 || each_letter > 126)
			continue;

		ringbuffer_write (context, &each_letter);

		plain_array[ringbuffer_get_write_index (context)] = '>';

		printf ("%.*s\033[0K\r", (int)sizeof (plain_array), plain_array);
		fflush (stdout);
		SLEEP (100);
	} while (!feof (text_file));

	ringbuffer_destroy (&context);

	return_code = EXIT_SUCCESS;

cleanup_after_open:
	if (text_file != stdin)
		fclose (text_file);

	fputs ("\033[0K", stdout);
	fflush (stdout);
	return return_code;
}

//! [Pager Main Source Code]
