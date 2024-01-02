#ifdef TEST_INTEGRATION
	#include "tests.h"
	#include "shared.h"
	#include "strings.h"

	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>

int
main ()
{
	errno_t return_code;

	// Single Shot
	printf (template_starting, testname_single_shot);
	return_code = test_single_shot ();
	switch (return_code) {
		case EXIT_SUCCESS:
			printf (template_success, testname_single_shot);
			break;
		case EXIT_FAILURE:
			printf (template_fail, testname_single_shot);
			break;
		default:
			goto on_fatal_error;
	}

	fputs (newline, stdout);
	fflush (stdout);

	// Synchronized
	printf (template_starting, testname_synchronized);
	return_code = test_synchronized ();
	switch (return_code) {
		case EXIT_SUCCESS:
			printf (template_success, testname_synchronized);
			break;
		case EXIT_FAILURE:
			printf (template_fail, testname_synchronized);
			break;
		default:
			goto on_fatal_error;
	}

	fputs (newline, stdout);
	fflush (stdout);

	// Slow Write
	printf (template_starting, testname_slow_write);
	return_code = test_slow_write ();
	switch (return_code) {
		case EXIT_SUCCESS:
			printf (template_success, testname_slow_write);
			break;
		case EXIT_FAILURE:
			printf (template_fail, testname_slow_write);
			break;
		default:
			goto on_fatal_error;
	}

	fputs (newline, stdout);
	fflush (stdout);

	// Slow Read
	printf (template_starting, testname_slow_read);
	return_code = test_slow_read ();
	switch (return_code) {
		case EXIT_SUCCESS:
			printf (template_success, testname_slow_read);
			break;
		case EXIT_FAILURE:
			printf (template_fail, testname_slow_read);
			break;
		default:
			goto on_fatal_error;
	}

	fflush (stdout);
	return EXIT_SUCCESS;

on_fatal_error:
	fprintf (stderr, "Aborting the test because '%s (errno %i)' reason.\r\n",
			 strerror (return_code), return_code);
	return return_code;
}

#endif // TEST_INTEGRATION
