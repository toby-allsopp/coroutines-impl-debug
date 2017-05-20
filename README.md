# coroutines-impl-debug

This simple program show the sequence of functions called by the implementation when executing a simple coroutine.

The idea is to help you understand the ordering of the various calls relative to each other, and to compare different implementations.

For example, the implementation in MSVC 2017.2 converts the return object to the return value earlier than the work-in-progrrss clang implementation.
