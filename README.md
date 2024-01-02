# Quickstart

There are a lot of ring buffer implementation in C over the internet. However,
author cannot find the most portable and well tested for that implementation,
especially for microcontroller. Other than learning reasons, the author wrote
the implementation with some priorities:

-   🧳 Works on every platform, thanks to CMake
-   🍼 Simple to use, no exhaustive abstraction layers
-   🪖 Intensively tested for the best reliability
-   🛟 Runtime logic safety, so it won't halt the system on mis-usage
-   📏 Standard directory layout, according to [Pitchfork Convention][1]
-   📖 Documentation also a priority on this project + examples

---

## Requirement

-   C and C++ Toolchain
    -   [build-essentials][2] for Debian-based distro
    -   [base-devel][3] for Arch-based distro
    -   [Development Tools][4] for Redhat-based distro
    -   [Xcode][5] for MacOS
    -   [MSBuild][6] for Windows
    -   [MPLab][7] for Microchip MCUs
-   [CMake](https://cmake.org/download) (For Linux users, install via package
    manager)
-   Doxygen for documentation (For Linux users, install via package manager)

---

## Preparation

After you installed the requirement, run this command to let CMake prepare the
source code.

```bash
git submodule update --init --recursive
mkdir -p build out
cd build
cmake ..
```

### For Microchip project

If you're developing a microcontroller software with Microchip, the preparation
command is different. Follow this instead.

```bash
git submodule update --init --recursive
mkdir build
cd build
cmake -DMICROCHIP_MCU=<Model_Number> ..
```

Replace `<Model_Number>` to your microcontroller's model number as example
_ATSAMC21E18A_.

Special thanks to [Sam Hanes][8] to make this possible!

---

## Build the Source Code

First of all, you need to get inside the `build` directory with `cd` command.
Then run this:

```bash
cmake --build .
```

That's it! Now, the build output is inside `out` directory.

### Build Output on Linux

-   `libringbuffer.a` is static library
-   `libringbuffer.so` is dynamic-linking library
-   Run `ringbuffer-test-integration` to test the build output briefly
-   Run `ringbuffer-test-unit` to test the build output in detail

### Build Output on MacOS

-   `libringbuffer.a` is static library
-   `libringbuffer.dylib` is dynamic-linking library
-   Run `ringbuffer-test-integration` to test the build output briefly
-   Run `ringbuffer-test-unit` to test the build output in detail

### Build Output on Windows

-   `ringbuffer.lib` is static library
-   `ringbuffer.dll` is dynamic-linking library
-   Run `ringbuffer-test-integration.exe` to test the build output briefly
-   Run `ringbuffer-test-unit.exe` to test the build output in detail

---

## How to install and use?

If you are using this project for POSIX-compliant platform including Linux-based
operating system, follow the **For POSIX-compliant OS** guide. Otherwise, follow
**For non-POSIX platform**. For Microchip platform, you just have to integrate
this project to your project.

### For POSIX-compliant OS

CMake made it easy, just go into `build` directory in this project with `cd`
command then run this.

```bash
sudo cmake --install .
```

See the `examples` directory to figure out how to use this project.

### For non-POSIX platform

This involves manual method, which requires you to copy `include/ringbuffer.h`
file in this project to your source code and link either static library or
dynamic-linking library. In Microsoft Visual Studio, see [this guide][9].

### This Library seems not working for Microcontroller, what to do?

If you can't successfully automate with CMake, copy the entire `src` and
`include` directory to your project manually. It possibly work, but you can't
use unit test and integration test in this repository for that platform. So the
only way to test is trying on _physical_ microcontroller.

[1]: https://github.com/vector-of-bool/pitchfork#readme
[2]: https://packages.ubuntu.com/jammy/build-essential
[3]: https://archlinux.org/packages/core/any/base-devel
[4]: https://unix.stackexchange.com/a/1344/472035
[5]: https://stackoverflow.com/a/38086884/3730834
[6]: https://visualstudio.microsoft.com/downloads
[7]: https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide
[8]: https://github.com/Elemecca
[9]: https://stackoverflow.com/a/23882710/3730834
