#ifdef TEST_UNIT
	#include <gtest/gtest.h>
	#include <ringbuffer.h>
	#include <src/context.h>
	#include <string.h>

TEST (GETTERS, READ_INDEX)
{
	struct ringbuffer_context context;
	memset (&context, 0, sizeof (struct ringbuffer_context));
	context.read_index = 123U;

	unsigned int result = ringbuffer_get_read_index (&context);

	EXPECT_EQ (result, 123U);
}

TEST (GETTERS, WRITE_INDEX)
{
	struct ringbuffer_context context;
	memset (&context, 0, sizeof (struct ringbuffer_context));
	context.write_index = 210U;

	unsigned int result = ringbuffer_get_write_index (&context);

	EXPECT_EQ (result, 210U);
}

TEST (GETTERS, EMPTY_TRUE)
{
	struct ringbuffer_context context;
	memset (&context, 0, sizeof (struct ringbuffer_context));
	context.element_count = 29U;
	context.read_index = 13U;
	context.write_index = 13U;

	unsigned int result = ringbuffer_is_empty (&context);

	EXPECT_TRUE (result);
}

TEST (GETTERS, EMPTY_FALSE)
{
	struct ringbuffer_context context;
	memset (&context, 0, sizeof (struct ringbuffer_context));
	context.element_count = 29U;
	context.read_index = 13U;
	context.write_index = 17U;

	unsigned int result = ringbuffer_is_empty (&context);

	EXPECT_FALSE (result);
}

TEST (GETTERS, EMPTY_TRUE_ON_SINGLE_ELEMENT)
{
	struct ringbuffer_context context;
	memset (&context, 0, sizeof (struct ringbuffer_context));
	context.element_count = 1U;
	context.read_index = 0U;
	context.write_index = 0U;

	unsigned int result = ringbuffer_is_empty (&context);

	EXPECT_FALSE (result);
}

#endif // TEST_UNIT
