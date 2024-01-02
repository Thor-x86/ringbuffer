#include <ringbuffer.h>

#include "context.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void
ringbuffer_write (ringbuffer_context_t context, const void *pointer_to_input)
{
	// Safety first.
	if (!context || !pointer_to_input)
		return;

	const unsigned int next_write_index
		= (context->write_index + 1) % context->element_count;

	// Truncate the oldest element if we have to
	if (context->read_index == next_write_index) {
		context->read_index++;
		context->read_index %= context->element_count;
	}

	uint8_t *pointer_to_output = (uint8_t *)context->target_array;

	const unsigned int output_index
		= context->write_index * context->element_size;

	memcpy (&(pointer_to_output[output_index]), pointer_to_input,
			context->element_size);

	context->write_index++;
	context->write_index %= context->element_count;
}
