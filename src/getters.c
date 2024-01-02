#include <ringbuffer.h>

#include "context.h"

unsigned int
ringbuffer_get_read_index (const ringbuffer_context_t context)
{
	if (!context)
		return 0;

	return context->read_index;
}

unsigned int
ringbuffer_get_write_index (const ringbuffer_context_t context)
{
	if (!context)
		return 0;

	return context->write_index;
}

unsigned int
ringbuffer_is_empty (const ringbuffer_context_t context)
{
	if (!context)
		return 1;

	return (context->element_count != 1)
		   && (context->read_index == context->write_index);
}
