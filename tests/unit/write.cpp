#ifdef TEST_UNIT
	#include <gtest/gtest.h>
	#include <ringbuffer.h>
	#include <src/context.h>

	#include "shared_consts.hpp"

TEST (WRITE, SINGLE_ELEMENT)
{
	uint8_t plain_array[1] = { 0xFFU };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = 1,
										  .element_count = 1,
										  .read_index = 0,
										  .write_index = 0 };

	uint8_t example = 33U;
	ringbuffer_write (&context, &example);

	EXPECT_EQ (plain_array[0], example);
	EXPECT_EQ (context.read_index, 0);
	EXPECT_EQ (context.write_index, 0);
}

TEST (WRITE, SINGLE_ELEMENT_LOOP)
{
	struct ringbuffer_context context
		= { .target_array = NULL, // Will be added later
			.element_size = 1,
			.element_count = 1,
			.read_index = 0,
			.write_index = 0 };

	const uint8_t examples[8] = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U };

	uint8_t plain_array[1] = { 0xFFU };
	context.target_array = plain_array;

	for (unsigned int i = 0; i < 8; i++) {
		ringbuffer_write (&context, &(examples[i]));

		EXPECT_EQ (plain_array[0], examples[i])
			<< SharedConsts::Label::index_at << i;
		EXPECT_EQ (context.read_index, 0) << SharedConsts::Label::index_at << i;
		EXPECT_EQ (context.write_index, 0)
			<< SharedConsts::Label::index_at << i;
	}
}

TEST (WRITE, SINGLE_ELEMENT_SIZE)
{
	struct ringbuffer_context context
		= { .target_array = NULL, // Will be added later
			.element_size = 1,
			.element_count = 8,
			.read_index = 0,
			.write_index = 0 };

	const uint8_t examples[8] = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U };

	uint8_t plain_array[8]
		= { 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU };
	context.target_array = plain_array;

	for (unsigned int i = 0; i < 8; i++) {
		ringbuffer_write (&context, &(examples[i]));

		ASSERT_EQ (plain_array[i], examples[i])
			<< SharedConsts::Label::index_at << i;
		ASSERT_EQ (context.write_index, (i + 1) % 8)
			<< SharedConsts::Label::index_at << i;

		if (i < 7)
			ASSERT_EQ (context.read_index, 0)
				<< SharedConsts::Label::index_at << i;
		else
			ASSERT_EQ (context.read_index, 1)
				<< SharedConsts::Label::index_at << i;
	}
}

TEST (WRITE, SINGLE_ELEMENT_SIZE_LOOP)
{
	struct ringbuffer_context context
		= { .target_array = NULL, // Will be added later
			.element_size = 1,
			.element_count = 5,
			.read_index = 0,
			.write_index = 0 };

	const uint8_t examples[8] = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U };

	uint8_t plain_array[5] = { 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU };
	context.target_array = plain_array;

	unsigned int expected_read_index = 0;

	for (unsigned int i = 0; i < 1000; i++) {
		ringbuffer_write (&context, &(examples[i % 8]));

		// Overwrite event detection
		if (i >= 4) {
			expected_read_index++;
			expected_read_index %= 5;
		}

		ASSERT_EQ (plain_array[i % 5], examples[i % 8])
			<< SharedConsts::Label::index_at << i;
		ASSERT_EQ (context.read_index, expected_read_index)
			<< SharedConsts::Label::index_at << i;
		ASSERT_EQ (context.write_index, (i + 1) % 5)
			<< SharedConsts::Label::index_at << i;
	}
}

TEST (WRITE, SINGLE_ELEMENT_COUNT)
{
	uint32_t plain_array[1] = { 0xFFFFFFFFU };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = sizeof (uint32_t),
										  .element_count = 1,
										  .read_index = 0,
										  .write_index = 0 };

	uint32_t example = 0xBABECAFE;
	ringbuffer_write (&context, &example);

	EXPECT_EQ (plain_array[0], example);
	EXPECT_EQ (context.read_index, 0);
	EXPECT_EQ (context.write_index, 0);
}

TEST (WRITE, SINGLE_ELEMENT_COUNT_LOOP)
{
	struct ringbuffer_context context
		= { .target_array = NULL, // Will be added later
			.element_size = sizeof (uint32_t),
			.element_count = 1,
			.read_index = 0,
			.write_index = 0 };

	const uint32_t examples[8]
		= { 0xBABECAFE, 0xDEADBEEF, 0xF00DCAFE, 0xD0D0B0B0,
			0xDEAFBEEF, 0xB00Bbabe, 0xbeefF00D, 0xABCDEF01 };

	uint32_t plain_array[1] = { 0xFFFFFFFFU };
	context.target_array = plain_array;

	for (unsigned int i = 0; i < 8; i++) {
		ringbuffer_write (&context, &(examples[i]));

		EXPECT_EQ (plain_array[0], examples[i])
			<< SharedConsts::Label::index_at << i;
		EXPECT_EQ (context.read_index, 0) << SharedConsts::Label::index_at << i;
		EXPECT_EQ (context.write_index, 0)
			<< SharedConsts::Label::index_at << i;
	}
}

TEST (WRITE, MULTI_ELEMENTS)
{
	struct ringbuffer_context context
		= { .target_array = NULL, // Will be added later
			.element_size = sizeof (uint32_t),
			.element_count = 8,
			.read_index = 0,
			.write_index = 0 };

	const uint32_t examples[8]
		= { 0xBABECAFE, 0xDEADBEEF, 0xF00DCAFE, 0xD0D0B0B0,
			0xDEAFBEEF, 0xB00Bbabe, 0xbeefF00D, 0xABCDEF01 };

	uint32_t plain_array[8]
		= { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU,
			0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU };
	context.target_array = plain_array;

	for (unsigned int i = 0; i < 8; i++) {
		ringbuffer_write (&context, &(examples[i]));

		ASSERT_EQ (plain_array[i], examples[i])
			<< SharedConsts::Label::index_at << i;
		ASSERT_EQ (context.write_index, (i + 1) % 8)
			<< SharedConsts::Label::index_at << i;

		if (i < 7)
			ASSERT_EQ (context.read_index, 0)
				<< SharedConsts::Label::index_at << i;
		else
			ASSERT_EQ (context.read_index, 1)
				<< SharedConsts::Label::index_at << i;
	}
}

TEST (WRITE, MULTI_ELEMENTS_LOOP)
{
	struct ringbuffer_context context
		= { .target_array = NULL, // Will be added later
			.element_size = sizeof (uint32_t),
			.element_count = 5,
			.read_index = 0,
			.write_index = 0 };

	const uint32_t examples[8]
		= { 0xBABECAFE, 0xDEADBEEF, 0xF00DCAFE, 0xD0D0B0B0,
			0xDEAFBEEF, 0xB00Bbabe, 0xbeefF00D, 0xABCDEF01 };

	uint32_t plain_array[5]
		= { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU };
	context.target_array = plain_array;

	unsigned int expected_read_index = 0;

	for (unsigned int i = 0; i < 1000; i++) {
		ringbuffer_write (&context, &(examples[i % 8]));

		// Overwrite event detection
		if (i >= 4) {
			expected_read_index++;
			expected_read_index %= 5;
		}

		ASSERT_EQ (plain_array[i % 5], examples[i % 8])
			<< SharedConsts::Label::index_at << i;
		ASSERT_EQ (context.read_index, expected_read_index)
			<< SharedConsts::Label::index_at << i;
		ASSERT_EQ (context.write_index, (i + 1) % 5)
			<< SharedConsts::Label::index_at << i;
	}
}

#endif // TEST_UNIT
