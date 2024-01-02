# Concept

## What is Ring Buffer?

![Fixed array with 12 elements](plain_array.svg)

A ring buffer is basically a fixed array with special steps, so we can do things
like **first-in first-out** operation (FIFO). This concept is required mostly
for low-level stuffs as example serial communication, task queue, and more. From
Wikipedia, there is a good illustration about how ring buffer works (or
"Circular Buffer" according to Wikipedia).

[![Previous fixed array bent into a ring shape with read index and write index running clockwise independently][1]][2]

Thanks to fixed array, we don't have to deal with dynamic array, linked list, or
any complicated stuffs. So the C program can be both predictable and performant.
To implement the ring buffer, we need several variables:

-   Fixed array, the element size is up to you
-   Read index
-   Write index

In source code of this repository, you (as a user) define the plain array
yourself. Then the rest handled by ringbuffer implementation module. There are
several scenarios which you need to know because other implementation might
treat that scenario differently.

### Scenario: Over-read

When the read index become faster than write index, it will reach a limit that
indicates the ring buffer is empty. To know whether the it reaches the limit or
not, we simply evaluate `read index == write index`. If it's true, then you
can't read the buffer because it's empty. In this implementation module, you can
use `ringbuffer_is_empty` function before reading each element to check if it's
empty or not.

### Scenario: Over-write

Writing too much elements to the buffer would causes the oldest elements to
disappear. So the data in the buffer always latest possible. It happens by
shifting both write and read index when the write index nears the read index.

[1]: https://upload.wikimedia.org/wikipedia/commons/f/fd/Circular_Buffer_Animation.gif
[2]: https://en.wikipedia.org/wiki/File:Circular_Buffer_Animation.gif

---

## What is Context?

Let's say you are going to use two arrays as two independent ring buffers. Now
the question is: _How do we store the indices and information about those
arrays?_ If you think "just put that somewhere in app source code" then it won't
look pleasant. Don't believe me? See this snippet.

```c
char each_letter;

ringbuffer_read(my_array, sizeof(char), sizeof(my_array),
                &read_index, &write_index, &each_letter);

printf("each_letter = %c\r\n", each_letter);
```

Yes, some parameters need to be references to a specific variable which
indicated with `&`. Other than that, you have to write same syntax for each read
operation, which quite painful because now you're managing a lot of variables!
In term of memory usage efficiency, passing a lot of parameter is not good.
Wouldn't it be nice if the parameters be fewer? The answer is: _by storing the
repetitive information in library space_. What could go wrong anyway…

![Diagram about access clash due to nature of C language](without_context.svg)

Well… access clash! A task shift the index and another task read the same index.
It has to be independent. In C language, we can't simply just declare an object
instance because our mindset is now about memory, and just memory, no objects,
no bananas, nada…. Note that we don't want to modify several states manually
including read and write indices. Otherwise, the ringbuffer would go mayhem! So
the solution is asking the library space to allocate a region of memory for its
internal operation.

![Context isolates states between separate instances](with_context.svg)

To do that, you would call `ringbuffer_init` to create context at very beginning
of your app program and clean it with `ringbuffer_destroy` after the app program
finished working with ring buffer. Basically, `ringbuffer_init` is a `malloc`
function with additional steps to initiate the ringbuffer. So it makes the
specific region of memory ready to be used as private part for ring buffer
operation. Also, same thing for `ringbuffer_destroy` with `free` function.
