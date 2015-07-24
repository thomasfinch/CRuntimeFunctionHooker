# CRuntimeFunctionHooker
This is an example of how C functions can be hooked at runtime, similar to [Cydia Substrate](http://www.cydiasubstrate.com/) and [Substitute](https://github.com/comex/substitute). It allows any function in a pre-compiled binary to be replaced at runtime with an arbitrary function written by the user. This doesn't modify the original binary at all, and doesn't require knowledge of the source code (although you do need to know the target function's signature, i.e. name, arguments, and return type).

This is mostly an experiment, if you need a function hooking library you should use Cydia Substrate or Substitute as they're much more robust.

**For a writeup about how this code works check out [this link](http://thomasfinch.me/blog/2015/07/24/Hooking-C-Functions-At-Runtime.html).**

## Running the Code

This code (currently) only works on 64 bit Mac OS X (tested on 10.10). Linux *almost* works, but when I tested it the dynamic library was loaded at a very high memory address out of range of a 32 bit offset from the main binary.

```
$ make
$ ./testProgram
Calling original function!
The number is: 5
$ DYLD_INSERT_LIBRARIES=inject.dylib ./testProgram
Calling replacement function!
The number is: 3
```

The makefile also includes a target for running the code with the dylib injected (`make run`) so you don't have to type it out every time.
