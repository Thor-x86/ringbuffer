#include <ringbuffer.h>
#include <stdint.h>

const uint64_t example_data[16]
	= { 0xCAFEBABEBABECAFE, 0xBEEFDEADDEADBEEF, 0xCAFEF00DF00DCAFE,
		0xB0B0D0D0D0D0B0B0, 0xBEEFDEAFDEAFBEEF, 0xbabeB00BB00Bbabe,
		0xF00DbeefbeefF00D, 0xEF01ABCDABCDEF01, 0xCAFEF00DF00DCAFE,
		0xB0B0D0D0D0D0B0B0, 0xBEEFDEAFDEAFBEEF, 0xbabeB00BB00Bbabe,
		0xCAFEBABEBABECAFE, 0xBEEFDEADDEADBEEF, 0xF00DbeefbeefF00D,
		0xEF01ABCDABCDEF01 };

const char template_report[]
	= "\033[?25l" // Hides the cursor to make it looks better
	  "Iteration    = %u\033[0K\r\n"
	  "Source Index = %u\033[0K\r\n"
	  "Dest. Index  = %u\033[0K\r\n"
	  "Read Index   = %u\033[0K\r\n"
	  "Write Index  = %u\033[0K\r\n"
	  "\033[5A"	   // Moves the cursor back to initial position
	  "\033[?25h"; // Show the cursor again
