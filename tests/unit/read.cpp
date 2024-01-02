#ifdef TEST_UNIT
	#include <gtest/gtest.h>
	#include <ringbuffer.h>
	#include <src/context.h>

	#include "shared_consts.hpp"

TEST (READ, SINGLE_ELEMENT)
{
	uint8_t plain_array[1] = { 33U };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = 1,
										  .element_count = 1,
										  .read_index = 0,
										  .write_index = 0 };

	uint8_t result = 0xFFU;
	ringbuffer_read (&context, &result);

	EXPECT_EQ (result, 33U);
	EXPECT_EQ (context.read_index, 0);
	EXPECT_EQ (context.write_index, 0);
}

TEST (READ, SINGLE_ELEMENT_LOOP)
{
	uint8_t plain_array[1] = { 33U };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = 1,
										  .element_count = 1,
										  .read_index = 0,
										  .write_index = 0 };

	uint8_t result;
	for (unsigned int i = 0; i < 1000; i++) {
		result = 0xFFU;
		ringbuffer_read (&context, &result);

		ASSERT_EQ (result, 33U) << SharedConsts::Label::index_at << i;
		ASSERT_EQ (context.read_index, 0) << SharedConsts::Label::index_at << i;
		ASSERT_EQ (context.write_index, 0)
			<< SharedConsts::Label::index_at << i;
	}
}

TEST (READ, SINGLE_ELEMENT_SIZE)
{
	uint8_t plain_array[8] = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = 1,
										  .element_count = 8,
										  .read_index = 1,
										  .write_index = 0 };

	uint8_t result;
	for (unsigned int i = 1; i < 8; i++) {
		result = 0xFFU;
		ringbuffer_read (&context, &result);

		EXPECT_EQ (result, plain_array[i])
			<< SharedConsts::Label::index_at << i;
		EXPECT_EQ (context.read_index, (i + 1) % 8)
			<< SharedConsts::Label::index_at << i;
		EXPECT_EQ (context.write_index, 0)
			<< SharedConsts::Label::index_at << i;
	}
}

TEST (READ, SINGLE_ELEMENT_SIZE_LOOP)
{
	uint8_t plain_array[8] = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = 1,
										  .element_count = 8,
										  .read_index = 1,
										  .write_index = 0 };

	for (unsigned int i = 0; i < 1000; i++) {
		unsigned int index = (i + 1) % 8;
		if (index == 0) {
			// Ignore empty buffer for now, there's a separate test for that
			context.read_index++;
			continue;
		}

		uint8_t result = 0xFFU;
		ringbuffer_read (&context, &result);

		ASSERT_EQ (result, plain_array[index])
			<< SharedConsts::Label::index_at << i
			<< SharedConsts::Label::secondary_index_at << index;
		ASSERT_EQ (context.read_index, (index + 1) % 8)
			<< SharedConsts::Label::index_at << i
			<< SharedConsts::Label::secondary_index_at << index;
		ASSERT_EQ (context.write_index, 0)
			<< SharedConsts::Label::index_at << i
			<< SharedConsts::Label::secondary_index_at << index;
	}
}

TEST (READ, SINGLE_ELEMENT_SIZE_OVERREAD)
{
	uint8_t plain_array[8] = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = 1,
										  .element_count = 8,
										  .read_index = 1,
										  .write_index = 0 };

	for (unsigned int i = 1; i < 24; i++) {
		uint8_t result = 0xFFU;
		ringbuffer_read (&context, &result);

		if (i < 8) {
			ASSERT_EQ (result, plain_array[i])
				<< SharedConsts::Label::index_at << i;
			ASSERT_EQ (context.read_index, (i + 1) % 8)
				<< SharedConsts::Label::index_at << i;
		} else {
			ASSERT_EQ (result, 0xFFU) << SharedConsts::Label::index_at << i;
			ASSERT_EQ (context.read_index, 0)
				<< SharedConsts::Label::index_at << i;
		}

		ASSERT_EQ (context.write_index, 0)
			<< SharedConsts::Label::index_at << i;
	}
}

TEST (READ, SINGLE_ELEMENT_COUNT)
{
	uint32_t plain_array[1] = { 0xBABECAFE };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = sizeof (uint32_t),
										  .element_count = 1,
										  .read_index = 0,
										  .write_index = 0 };

	uint32_t result = 0xFFFFFFFFU;
	ringbuffer_read (&context, &result);

	EXPECT_EQ (result, 0xBABECAFE);
	EXPECT_EQ (context.read_index, 0);
	EXPECT_EQ (context.write_index, 0);
}

TEST (READ, SINGLE_ELEMENT_COUNT_LOOP)
{
	uint32_t plain_array[1] = { 0xBABECAFE };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = sizeof (uint32_t),
										  .element_count = 1,
										  .read_index = 0,
										  .write_index = 0 };

	uint32_t result;

	for (unsigned int i = 0; i < 1000; i++) {
		result = 0xFFFFFFFFU;
		ringbuffer_read (&context, &result);

		ASSERT_EQ (result, 0xBABECAFE) << SharedConsts::Label::index_at << i;
		ASSERT_EQ (context.read_index, 0) << SharedConsts::Label::index_at << i;
		ASSERT_EQ (context.write_index, 0)
			<< SharedConsts::Label::index_at << i;
	}
}

TEST (READ, MULTI_ELEMENTS)
{
	uint32_t plain_array[8]
		= { 0xBABECAFE, 0xDEADBEEF, 0xF00DCAFE, 0xD0D0B0B0,
			0xDEAFBEEF, 0xB00Bbabe, 0xbeefF00D, 0xABCDEF01 };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = sizeof (uint32_t),
										  .element_count = 8,
										  .read_index = 1,
										  .write_index = 0 };

	uint32_t result;
	for (unsigned int i = 1; i < 8; i++) {
		result = 0xFFFFFFFFU;
		ringbuffer_read (&context, &result);
		EXPECT_EQ (result, plain_array[i])
			<< SharedConsts::Label::index_at << i;
		EXPECT_EQ (context.read_index, (i + 1) % 8)
			<< SharedConsts::Label::index_at << i;
		EXPECT_EQ (context.write_index, 0)
			<< SharedConsts::Label::index_at << i;
	}
}

TEST (READ, MULTI_ELEMENTS_LOOP)
{
	uint32_t plain_array[8]
		= { 0xBABECAFE, 0xDEADBEEF, 0xF00DCAFE, 0xD0D0B0B0,
			0xDEAFBEEF, 0xB00Bbabe, 0xbeefF00D, 0xABCDEF01 };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = sizeof (uint32_t),
										  .element_count = 8,
										  .read_index = 1,
										  .write_index = 0 };

	for (unsigned int i = 0; i < 1000; i++) {
		unsigned int index = (i + 1) % 8;
		if (index == 0) {
			// Ignore empty buffer for now, there's a separate test for that
			context.read_index++;
			continue;
		}

		uint32_t result = 0xFFFFFFFFU;
		ringbuffer_read (&context, &result);

		ASSERT_EQ (result, plain_array[index])
			<< SharedConsts::Label::index_at << i
			<< SharedConsts::Label::secondary_index_at << index;
		ASSERT_EQ (context.read_index, (index + 1) % 8)
			<< SharedConsts::Label::index_at << i
			<< SharedConsts::Label::secondary_index_at << index;
		ASSERT_EQ (context.write_index, 0)
			<< SharedConsts::Label::index_at << i
			<< SharedConsts::Label::secondary_index_at << index;
	}
}

TEST (READ, MULTI_ELEMENTS_OVERREAD)
{
	uint32_t plain_array[8]
		= { 0xBABECAFE, 0xDEADBEEF, 0xF00DCAFE, 0xD0D0B0B0,
			0xDEAFBEEF, 0xB00Bbabe, 0xbeefF00D, 0xABCDEF01 };
	struct ringbuffer_context context = { .target_array = plain_array,
										  .element_size = sizeof (uint32_t),
										  .element_count = 8,
										  .read_index = 1,
										  .write_index = 0 };

	for (unsigned int i = 1; i < 24; i++) {
		uint32_t result = 0xFFFFFFFFU;
		ringbuffer_read (&context, &result);

		if (i < 8) {
			ASSERT_EQ (result, plain_array[i])
				<< SharedConsts::Label::index_at << i;
			ASSERT_EQ (context.read_index, (i + 1) % 8)
				<< SharedConsts::Label::index_at << i;
		} else {
			ASSERT_EQ (result, 0xFFFFFFFFU)
				<< SharedConsts::Label::index_at << i;
			ASSERT_EQ (context.read_index, 0)
				<< SharedConsts::Label::index_at << i;
		}

		ASSERT_EQ (context.write_index, 0)
			<< SharedConsts::Label::index_at << i;
	}
}

#endif // TEST_UNIT
