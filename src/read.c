#include <ringbuffer.h>

#include "context.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void
ringbuffer_read (ringbuffer_context_t context, void *pointer_to_output)
{
	// Safety first.
	if (!context || !pointer_to_output)
		return;

	// Detects empty buffer, it won't work on element_count = 1
	if (context->element_count != 1
		&& context->read_index == context->write_index)
		return;

	uint8_t *pointer_to_input = (uint8_t *)context->target_array;

	const unsigned int input_index
		= context->read_index * context->element_size;

	memcpy (pointer_to_output, &(pointer_to_input[input_index]),
			context->element_size);

	context->read_index++;
	context->read_index %= context->element_count;
}
