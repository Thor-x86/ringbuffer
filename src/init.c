#include <ringbuffer.h>

#include "context.h"

#include <stdlib.h>
#include <string.h>

ringbuffer_context_t
ringbuffer_init (void *target_array, const unsigned int element_size,
				 const unsigned int element_count,
				 const unsigned int initial_read_index,
				 const unsigned int initial_write_index)
{
	if (!target_array || !element_size || !element_count)
		return NULL;

	ringbuffer_context_t context
		= (ringbuffer_context_t)malloc (sizeof (struct ringbuffer_context));
	if (!context)
		return NULL;

	context->target_array = target_array;
	context->element_size = element_size;
	context->element_count = element_count;
	context->read_index = initial_read_index % element_count;
	context->write_index = initial_write_index % element_count;

	return context;
}
