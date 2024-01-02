#ifdef TEST_UNIT
	#include <gtest/gtest.h>
	#include <ringbuffer.h>
	#include <src/context.h>

TEST (INIT, NORMAL)
{
	uint32_t plain_array[64];

	ringbuffer_context_t context
		= ringbuffer_init (plain_array, sizeof (uint32_t), 64, 0, 0);

	ASSERT_TRUE (context);
	EXPECT_EQ (context->target_array, plain_array);
	EXPECT_EQ (context->element_size, sizeof (uint32_t));
	EXPECT_EQ (context->element_count, 64);
	EXPECT_EQ (context->read_index, 0);
	EXPECT_EQ (context->write_index, 0);

	// WARNING: Manual deallocation here, because it's not integration test.
	free (context);
}

TEST (INIT, RESUME_READ_INDEX)
{
	uint32_t plain_array[64];

	ringbuffer_context_t context
		= ringbuffer_init (plain_array, sizeof (uint32_t), 64, 13, 0);

	ASSERT_TRUE (context);
	EXPECT_EQ (context->target_array, plain_array);
	EXPECT_EQ (context->element_size, sizeof (uint32_t));
	EXPECT_EQ (context->element_count, 64);
	EXPECT_EQ (context->read_index, 13);
	EXPECT_EQ (context->write_index, 0);

	// WARNING: Manual deallocation here, because it's not integration test.
	free (context);
}

TEST (INIT, RESUME_WRITE_INDEX)
{
	uint32_t plain_array[64];

	ringbuffer_context_t context
		= ringbuffer_init (plain_array, sizeof (uint32_t), 64, 0, 17);

	ASSERT_TRUE (context);
	EXPECT_EQ (context->target_array, plain_array);
	EXPECT_EQ (context->element_size, sizeof (uint32_t));
	EXPECT_EQ (context->element_count, 64);
	EXPECT_EQ (context->read_index, 0);
	EXPECT_EQ (context->write_index, 17);

	// WARNING: Manual deallocation here, because it's not integration test.
	free (context);
}

TEST (INIT, READ_INDEX_OVERLIMIT)
{
	uint32_t plain_array[64];

	ringbuffer_context_t context
		= ringbuffer_init (plain_array, sizeof (uint32_t), 64, 65, 0);

	ASSERT_TRUE (context);
	EXPECT_EQ (context->target_array, plain_array);
	EXPECT_EQ (context->element_size, sizeof (uint32_t));
	EXPECT_EQ (context->element_count, 64);
	EXPECT_EQ (context->read_index, 1);
	EXPECT_EQ (context->write_index, 0);

	// WARNING: Manual deallocation here, because it's not integration test.
	free (context);
}

TEST (INIT, WRITE_INDEX_OVERLIMIT)
{
	uint32_t plain_array[64];

	ringbuffer_context_t context
		= ringbuffer_init (plain_array, sizeof (uint32_t), 64, 0, 65);

	ASSERT_TRUE (context);
	EXPECT_EQ (context->target_array, plain_array);
	EXPECT_EQ (context->element_size, sizeof (uint32_t));
	EXPECT_EQ (context->element_count, 64);
	EXPECT_EQ (context->read_index, 0);
	EXPECT_EQ (context->write_index, 1);

	// WARNING: Manual deallocation here, because it's not integration test.
	free (context);
}

TEST (INIT, EMPTY)
{
	uint32_t plain_array[1] = { 0U }; // Placeholder

	ringbuffer_context_t context
		= ringbuffer_init (plain_array, sizeof (uint32_t), 0, 0, 0);

	EXPECT_FALSE (context);

	// WARNING: Manual deallocation here, because it's not integration test.
	free (context);
}

TEST (INIT, ARRAY_IS_NULL)
{
	ringbuffer_context_t context
		= ringbuffer_init (NULL, sizeof (uint32_t), 64, 0, 0);

	EXPECT_FALSE (context);

	// WARNING: Manual deallocation here, because it's not integration test.
	free (context);
}

TEST (INIT, ZERO_ELEMENT_SIZE)
{
	uint32_t plain_array[64];

	ringbuffer_context_t context = ringbuffer_init (plain_array, 0, 64, 0, 0);

	EXPECT_FALSE (context);

	// WARNING: Manual deallocation here, because it's not integration test.
	free (context);
}

TEST (INIT, ZERO_ELEMENT_COUNT)
{
	uint32_t plain_array[64];

	ringbuffer_context_t context
		= ringbuffer_init (plain_array, sizeof (uint32_t), 0, 0, 0);

	EXPECT_FALSE (context);

	// WARNING: Manual deallocation here, because it's not integration test.
	free (context);
}

#endif // TEST_UNIT
