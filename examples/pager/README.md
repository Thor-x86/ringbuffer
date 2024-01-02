# Ringbuffer Implementation on Terminal Pager

In this example program, it will demonstrate how ringbuffer implemented in a
pager. User could take a sample text then point that text to this program via
command argument. The intention of this example is to promote the good practice
of implementing ringbuffer module (and POSIX functions) in practice.

Before trying the example program, you have to check whether the environment
variable was set properly.

```bash
echo $LD_LIBRARY_PATH
```

The output must include this:

```
/usr/local/lib
```

If it's not (or empty), run this command:

```bash
export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib
```

Then you can compile with this command:

```bash
cc main.c -l ringbuffer -o pager
```

...and run the compilation result with this command:

```bash
./pager Blindsight.txt
```

You can replace `Blindsight.txt` to any file you want. Other than reading files,
if you are using UNIX-based OS, you can read news directly from news server like
ticker bar on news television channels by entering this command:

```bash
clear &&
{ curl -s https://alterslash.org/        \
    || echo "No internet connection!"; } \
    | sed -z 's/^.*\(<body\)/<body/g' -  \
	| sed 's/&nbsp;/ /g' -               \
	| sed -n '/^$/!{s/<[^>]*>/ /g;p;}' - \
	| ./pager -
```
