# Examples of GCC diagnostics

## Setup and usage

The script `test.sh` compiles all provided examples. In order to use it you
will need to specify your GCC build directory in the script.

Note that the compiler proper (not the driver) is used for compilation (i.e.
`cc1` instead of `gcc`, etc). If you will ever need to run the compiler under
debugger, this will probably be the easiest way to run the compiler.

## Clang script

There is also a script `test-clang.sh` which runs Clang and Clang++ on the
same files (except `test.f95` - because Clang does not support Fortran and
`indent.c` because the corresponding warnging is only implemented in GCC).
Obviously, you will need to install Clang in order to use it. Note that Clang
has some sort of machine-pareseable fixit format.

Uncomment the line:

```bash
FLAGS='-c -fdiagnostics-parseable-fixits -fdiagnostics-print-source-range-info'
```

in `test-clang.sh` to see what it looks like.

## List of examples

- `simple.c` - a simple example with a single error.
- `include.c`, `header1.h`, `header2.h` - in this example, the error location
  is inside a header file.
- `include.i` - this file was obtained from the previous 3 by running the
  preprocessor: `gcc -E include.c -o include.i`,
  it should produce result equivalent to `include.c`
- `printf.c` - the warning message contains several data types (`double`,
  `int`) and a string literal (`'%f'`).
- `macro.c` - in this example the error comes from macro exapansion.
- `indent.c` - an example of a new warning, `-Wmisleading-indentation`. This
  example might be nontrivial for our XML format. Please keep it in mind when
  developing the format.
- `overload.cc` - this program is written in C++ (unlike the previous ones).
  The error occurs because two different overloads of `foo` are decalared
  and the call is ambiguous. The compiler lists candidates in the error
  message.
- `template.cc` - this is the example, which I sent you earilier in our private
  conversation. The error is essentially the same as in previous example, but
  there are several differences:
 - overloads are defined inside macro expansions. The compiler outputs a
   corresponding note.
 - the ambiguous call happends during `adder` template class instantiation, 
   which calls constexpr function `add_val` (in order to initialize a static
   constexpr data member). This chain of objects is visible in the diagnostic
   message.
 - another error "constexpr call flows off the end of the function" is in some
   sense a compiler's problem: it cannot correctly recover after the real error
   "call of overloaded 'add_overload(int&)' is ambiguous" and finish parsing
   the `add_val` function correctly.
- `test.f95` - this program is written in Fortran. Notice that diagnostics
  look slightly different.
