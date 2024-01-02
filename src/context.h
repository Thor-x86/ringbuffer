#ifndef __CONTEXT_DECLARED
#define __CONTEXT_DECLARED

struct ringbuffer_context {
	void *target_array;
	unsigned int element_size;
	unsigned int element_count;
	unsigned int read_index;
	unsigned int write_index;
};

#endif // __CONTEXT_DECLARED
