# Contributing

If you are actually want to understand this page, I would say thank you for your
participation! Basically, the way to contribute boils down into two ways:

1. Open an issue
2. Pull request

Any technical problems related to this project should goes to issue page. If you
know how to fix it, then go ahead fork the repo and pull request. However,
**issue page is not a Q&A page**. For question which has no solution in this
documentation, please ask it at available question platform including
StackOverflow. If you found a securityvulnerability, please contact the
maintainer directly through foss@athaariq.my.id.

---

## Directory Structure

The project is (almost) complying [Pitchfork Convention][1]. If you not really
familiar with the directory structure, here's the brief explanation.

```
ringbuffer
 |-- .cmake: Automatically generated if you are using Visual Studio Code. This
 |           directory contains CMake-generated build tools.
 |
 |-- .vscode: Configuration files for Visual Studio Code. Author chose this IDE
 |            because its intellisense is helpful.
 |
 |-- build: You create this manually to work with CMake without Visual Studio
 |          Code.
 |
 |-- docs: Contents and customization for this documentation. The layout is
 |    |    provided by Doxygen Awesome from jothepro.
 |    |-- page: Content of manual pages
 |     \- resource: Illustrations for the documentation, SVG is recommended.
 |
 |-- examples: Usage example of this project, you can copy and modify yourself
 |    |        to learn how to use this project.
 |    |-- simple: Bare-minimum example to let you get the idea how to use this
 |    |           project.
 |     \- pager: Practical example of ringbuffer implementation.
 |
 |-- external: Dependencies from someone's GitHub repository. It's empty if you
 |             are not prepared the project after clone. See the quickstart.
 |
 |-- include: Public header files. You can copy any files inside of this
 |            directory to your project in case your target device is not POSIX-
 |            compliant.
 |
 |-- out: Any build output, including source compilation and documentation. At
 |        first, you won't see this directory until you did compiled the source.
 |
 |-- src: Main source codes live here. If you want to contribute, take a look at
 |        this directory.
 |
  \- tests: Source codes for testing the main source code. It can be automated
      |     with CI/CD.
      |-- integration: Usually, you run this test after unit test to check the
      |                integrity.
       \- unit: C++ sources for testing each main source file, with help from
	            GoogleTest framework.
```

[1]: https://github.com/vector-of-bool/pitchfork#readme

---

## Coding Style

For Visual Studio Code users, the code styling is automatically formatted. For
other IDE users, you have to consider to learn our coding style. The only
purpose is to make the code readable and to make sure the way you code is not
conflicting with my automated formatter. Otherwise, there would be unnecessary
changes due to formatter complaining about inconsistent style. This project's
coding style is based [Linux kernel coding style standard][2] with some
modifications and remarks:

-   Tabs shouldn't replaced with spaces unless specific parts of documentation
-   Tab size is 4 columns (I know it's kind of defying the standard, but trust
    me, you won't read the code that too far shifted)
-   Maximum width is 80 columns
-   Naming must in `snake_case`, not `camelCase` or even `PascalCase`
-   Macro names must in `CAPITAL_SNAKE_CASE`
-   Omit braces for single-line conditional statement, if possible
-   Avoid abbreviation as possible

For better clarity, here's the list of Do and Don't:

### Indentation

Do:

```c
if (i > 10) {
	puts ("Exceed ten!");
	return 1;
}
```

Don't:

```c
if (i > 10) {
		puts ("Exceed ten!");
		return 1;
}
```

Definitely don't:

```c
if (i > 10) {
  puts ("Exceed ten!");
  return 1;
}
```

### Brace Position for Conditional and Loop Syntax

Do:

```c
for (int i = 0; i < 10; i++) {
	printf("i = %i\r\n", i);
	destination[i] = source[i];
}
```

Don't:

```c
for (int i = 0; i < 10; i++)
{
	printf("i = %i\r\n", i);
	destination[i] = source[i];
}
```

Definitely don't:

```c
for (int i = 0; i < 10; i++)
  {
    printf("i = %i\r\n", i);
	destination[i] = source[i];
  }
```

### Single line conditional statement

Do:

```c
if (!allowed)
	return;
```

Don't:

```c
if (!allowed) return;
```

Also don't:

```c
if (!allowed) {
	return;
}
```

Oh this is cursed, just don't.

```c
if (!allowed)
	puts ("Not allowed!"), return;
```

### Braces for Functions

Do:

```c
void
hello_world (const char **name)
{
	printf("Hello %s,\r\n", name);
	puts("Have a nice day!");
}
```

Don't:

```c
void
hello_world (const char **name) {
	printf("Hello %s,\r\n", name);
	puts("Have a nice day!");
}
```

### Function parameters

Do:

```c
void
print_full_name (const char *first_name,
				 const char *middle_name,
				 const char *family_name)
{
	puts("Name in citation:");
	printf("%s, %s %s\r\n", family_name, first_name, middle_name);
}
```

Don't:

```c
void
print_full_name (
	const char *first_name,
	const char *middle_name,
	const char *family_name
) {
	puts("Name in citation:");
	printf("%s, %s %s\r\n", family_name, first_name, middle_name);
}
```

Also don't:

```c
void
print_full_name (const char *first_name,
	const char *middle_name,
	const char *family_name)
{
	puts("Name in citation:");
	printf("%s, %s %s\r\n", family_name, first_name, middle_name);
}
```

Also don't, this is worse:

```c
void
print_full_name (const char *first_name,
	const char *middle_name,
	const char *family_name) {
	puts("Name in citation:");
	printf("%s, %s %s\r\n", family_name, first_name, middle_name);
}
```

### Function return type

Do:

```c
unsigned int
ringbuffer_get_read_index (const ringbuffer_context_t context)
{
	if (!context)
		return 0;

	return context->read_index;
}
```

Don't:

```c
unsigned int ringbuffer_get_read_index (const ringbuffer_context_t context)
{
	if (!context)
		return 0;

	return context->read_index;
}
```
