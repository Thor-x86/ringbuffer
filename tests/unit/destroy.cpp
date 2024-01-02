#ifdef TEST_UNIT
	#include <gtest/gtest.h>
	#include <ringbuffer.h>
	#include <src/context.h>

TEST (DESTROY, ONCE)
{
	uint32_t plain_array[64];
	ringbuffer_context_t context
		= (ringbuffer_context_t)malloc (sizeof (struct ringbuffer_context));

	context->target_array = plain_array;
	context->element_size = sizeof (uint32_t);
	context->element_count = 64;
	context->read_index = 0;
	context->write_index = 0;

	// Will crash if fail
	ringbuffer_destroy (&context);

	EXPECT_FALSE (context);
}

TEST (DESTROY, TWICE)
{
	uint32_t plain_array[64];
	ringbuffer_context_t context
		= (ringbuffer_context_t)malloc (sizeof (struct ringbuffer_context));

	context->target_array = plain_array;
	context->element_size = sizeof (uint32_t);
	context->element_count = 64;
	context->read_index = 0;
	context->write_index = 0;

	ringbuffer_destroy (&context);
	ASSERT_FALSE (context);

	ringbuffer_destroy (&context);
	EXPECT_FALSE (context);
}

#endif // TEST_UNIT
