#include <ringbuffer.h>

#include "context.h"

#include <stdlib.h>

void
ringbuffer_destroy (ringbuffer_context_t *pointer_to_context)
{
	if (!pointer_to_context)
		return;

	ringbuffer_context_t context = *pointer_to_context;
	if (!context)
		return;

	free (context);
	*pointer_to_context = NULL;
}
