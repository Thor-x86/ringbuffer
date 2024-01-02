/**
 * @file ringbuffer.h
 * Contains type definitions and functions from ringbuffer module which can be
 * applied on application source code.
 */
#ifndef __RINGBUFFER_DECLARED
#define __RINGBUFFER_DECLARED

#include <stdint.h>

// Support for Windows DLL
#if __has_include(<windows.h>)
	#if defined(__RINGBUFFER_INTERNAL)
		#if defined(TEST_UNIT) || defined(TEST_INTEGRATION)
			/// @private
			#define LINKED_FUNCTION extern
		#else
			/// @private
			#define LINKED_FUNCTION extern __declspec (dllexport)
		#endif
	#else
		/// @private
		#define LINKED_FUNCTION __declspec (dllimport)
	#endif
#else // __has_include(<windows.h>)
	/// @private
	#define LINKED_FUNCTION extern
#endif // __has_include(<windows.h>)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Private memory type declaration
 *
 * @details
 * This variable exists only for type enforcement. You shouldn't use
 * struct ringbuffer_context directly on your application source code.
 */
struct ringbuffer_context;

/**
 * @brief Points to an allocated memory for ringbuffer module
 *
 * @details
 * The reason why this library didn't (and won't) store global variables
 * because it would cause trouble if you want to do multiple independent
 * ringbuffer management. You **MUST** clean it with ringbuffer_destroy
 * function if not in use anymore.
 */
typedef struct ringbuffer_context *ringbuffer_context_t;

/**
 * @brief Initiate a single instance of ringbuffer management
 * @param target_array will be treated as a ring buffer with this library, set
 *                     this to NULL will cause ringbuffer_init returns NULL
 * @param element_size in bytes, defines element size of target_array, set this
 *                     to 0 will cause ringbuffer_init returns NULL
 * @param element_count represents element count of target_array, set this to 0
 *                      will cause ringbuffer_init returns NULL
 * @param initial_read_index allows the program to continue previous ring
 * 							 buffer, set to 0 if you don't need this
 * @param initial_write_index allows the program to continue previous ring
 * 							  buffer, set to 0 if you don't need this
 * @return ringbuffer_context_t if parameters are valid, otherwise NULL
 *
 * @details
 * This function allocates a memory region for internal operation of ringbuffer
 * management and references target_array into that memory region while also
 * stores additional informations such as element size and count. Remember to
 * always call ringbuffer_destroy function once if you're not using ringbuffer
 * anymore.
 *
 * **Brief Example**
 *
 * @code{.c}
 * #include <ringbuffer.h>
 *
 * unsigned long my_array[128];
 *
 * int main() {
 *     ringbuffer_context_t my_context = ringbuffer_init(my_array,
 *                                                       sizeof(unsigned long),
 *                                                       128,
 *                                                       0, 0);
 *
 *     //...
 * }
 * @endcode
 */
LINKED_FUNCTION ringbuffer_context_t ringbuffer_init (
	void *target_array, const unsigned int element_size,
	const unsigned int element_count, const unsigned int initial_read_index,
	const unsigned int initial_write_index);

/**
 * @brief Copy value from an element pointed by read index, to an intended
 *        variable
 * @param context came from ringbuffer_init
 * @param pointer_to_output points where to write the data, remember to always
 *                          use "&" symbol prefix (a.k.a. reference)
 *
 * @details
 * The ringbuffer_read function copies value from an element based on read index
 * which stored privately in the context, into a variable which you'd pointed
 * via pointer_to_output parameter. After called this function, the ringbuffer
 * module automatically shifts the read index for future use, unless your app
 * program read all elements in the array (or in other word, it's "empty"). When
 * it's already empty and you keep reading, it won't do anything. Internally,
 * the ringbuffer considered "empty" when read index equals write index.
 *
 * **Brief Example**
 *
 * @code{.c}
 * #include <ringbuffer.h>
 *
 * unsigned long my_array[128];
 *
 * int main() {
 *     ringbuffer_context_t my_context = ringbuffer_init(my_array,
 *                                                       sizeof(unsigned long),
 *                                                       128,
 *                                                       0, 0);
 *     // Write the ringbuffer first...
 *
 *     if (ringbuffer_is_empty(my_context)) {
 *         // It's empty and you can't read it.
 *         return 1;
 *     }
 *
 *     unsigned long my_var;
 *     ringbuffer_read(my_context, &my_var);
 *
 *     // my_var was modified, do something with it...
 * }
 * @endcode
 */
LINKED_FUNCTION void ringbuffer_read (ringbuffer_context_t context,
									  void *pointer_to_output);

/**
 * @brief Put value from a variable to an element which pointed by write index
 * @param context came from ringbuffer_init
 * @param pointer_to_input points where is the variable in the memory, remember
 *                         to always use "&" symbol prefix (a.k.a. reference)
 *
 * @details
 * When attempting to write ring buffer, you have to consider several things:
 *
 * 1. The variable size must equals element size of the array. Otherwise, you'll
 *    experience undefined behavior!
 * 2. pointer_to_input always represents a pointer to a variable, so you have to
 *    prepend "&" to the variable name. See the example for clear explanation.
 * 3. The new value will be written according to write index which privately
 *    located inside context.
 * 4. Write index always be shifted after called this function
 *
 * **Brief Example**
 *
 * @code{.c}
 * #include <ringbuffer.h>
 *
 * unsigned long my_array[128];
 *
 * int main() {
 *     ringbuffer_context_t my_context = ringbuffer_init(my_array,
 *                                                       sizeof(unsigned long),
 *                                                       128,
 *                                                       0, 0);
 *     unsigned long my_math_result = 0UL;
 *
 *     // Do something with my_math_result variable...
 *
 *     ringbuffer_write(my_context, &my_math_result);
 *
 *     // ...
 * }
 * @endcode
 */
LINKED_FUNCTION void ringbuffer_write (ringbuffer_context_t context,
									   const void *pointer_to_input);

/**
 * @brief Get the read index, which means position of read cursor of ring buffer
 * @param context came from ringbuffer_init
 * @return unsigned int in bytes unit
 */
LINKED_FUNCTION unsigned int
ringbuffer_get_read_index (const ringbuffer_context_t context);

/**
 * @brief Get the write index, which means position of write cursor of ring
 *        buffer
 * @param context came from ringbuffer_init
 * @return unsigned int in bytes unit
 */
LINKED_FUNCTION unsigned int
ringbuffer_get_write_index (const ringbuffer_context_t context);

/**
 * @brief Check if the ringbuffer is empty, which indicated by
 *        'read index == write index'
 * @param context came from ringbuffer_init
 * @return 1 if empty, 0 if has element to read
 */
LINKED_FUNCTION unsigned int
ringbuffer_is_empty (const ringbuffer_context_t context);

/**
 * @brief Clean-up ringbuffer context then NULLify it
 * @param pointer_to_context must has ampersand prefix, as example
 * 							 &my_context
 *
 * @details
 * This ensures the context was cleaned-up from memory for safety. So accidental
 * context usage won't cause segmentation fault if it's already cleaned up. Keep
 * in mind that your array won't be cleaned up while calling this function
 * because it's not created by this module. Thus, it's not ringbuffer module's
 * responsibility.
 *
 * **Brief Example**
 *
 * @code{.c}
 * #include <ringbuffer.h>
 *
 * // ...
 *
 * int main() {
 *     ringbuffer_context_t my_context = ringbuffer_init(
 *         // ...
 *     );
 *
 *     // ...
 *
 *     ringbuffer_destroy(&my_context);
 *     return 0;
 * }
 * @endcode
 */
LINKED_FUNCTION void
ringbuffer_destroy (ringbuffer_context_t *pointer_to_context);

#ifdef __cplusplus
}
#endif

#undef LINKED_FUNCTION

#endif // __RINGBUFFER_DECLARED
