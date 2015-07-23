# CRuntimeFunctionHooker
This is an example of how C functions can be hooked at runtime, similar to how [Cydia Substrate](http://www.cydiasubstrate.com/) and [Substitute](https://github.com/comex/substitute) work. It allows any function in a pre-compiled binary to be replaced at runtime with an arbitrary function written by the user. This doesn't modify the original binary at all, and doesn't require knowledge of the source code (although you do need to know the target function's signature, i.e. name, arguments, and return type).

This is mostly an experiment, if you need a function hooking library you should use Cydia Substrate or Substitute as they're much more robust.
