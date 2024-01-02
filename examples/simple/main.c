/**
 * @example simple
 *
 * @details
 * Bare minimum implementation example. Probably you want to learn with this
 * first.
 *
 * ---
 *
 * ## main.c
 *
 * @snippet simple/main.c Simple Example
 */

//! [Simple Example]

#include <ringbuffer.h> // This project
#include <stdio.h>		// For printf, puts, fputs
#include <stdlib.h>		// For EXIT_SUCCESS, EXIT_FAILURE, and NULL
#include <string.h>		// For memset

int
main ()
{
	// We need a separate plain array to store the actual data, so you can have
	// full access to the data.
	char plain_array[64];

	// Let's fill the array with "=" to indicate the unwritten elements
	memset (plain_array, '=', sizeof (plain_array));

	// Context is an allocated region of RAM for ringbuffer internal operation.
	// The concept of context allows us to initiate multiple independent
	// ringbuffer instances.
	ringbuffer_context_t context = ringbuffer_init (
		plain_array,   // Where to store the data?
		sizeof (char), // Each element size, sizeof(char) equals 1
		64,			   // Element count, don't confuse it with byte count!
		0, 0); // Always set these to zero unless you have specific needs

	// Note that 'puts' function automatically add trailing "\r\n"
	puts ("To exit, just hit enter button without typing any letter");

	while (1) {
		// Whereas 'fputs' function won't add trailing "\r\n", unlike 'puts'
		fputs ("\r\nType something> ", stdout);

		// User input -> ringbuffer
		while (1) {
			const char each_letter = (char)getchar ();

			if (each_letter == '\n')
				break;
			else if (each_letter < 32 || each_letter > 126)
				continue;

			ringbuffer_write (context, &each_letter);
		}

		if (ringbuffer_is_empty (context))
			break;

		// Ringbuffer -> user's terminal
		while (!ringbuffer_is_empty (context)) {
			char each_letter;
			ringbuffer_read (context, &each_letter);
			putchar (each_letter);
		}

		// Our ringbuffer won't ever be NUL terminated. To print the plain
		// array, we have to do something special with printf.
		printf ("\r\n"
				"\r\n"
				"Ringbuffer: [%.*s]\r\n",
				(int)sizeof (plain_array), plain_array);
	}

	puts ("\r\nBye!");

	// Clean-up before exiting, it's like making bed after you sleep :)
	ringbuffer_destroy (&context);
	return EXIT_SUCCESS;
}

//! [Simple Example]
