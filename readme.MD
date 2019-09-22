# Introduction
Yak libs is a collection of single-purpose, single-header libraries heavily inspired by Sean Barret's [STB](https://github.com/nothings/stb). These are tools which I personally use and have developed out of frustration that everything similar out there has to be _that complicated_. 

The libraries are developed in C but may use some C++ features (and will be compatible with modern C++ in the future).

Currently it houses the following libraries in very early stages of development:

* Mostly type defines and stuff that currently doesn't have an own library: [yak_defines.h](../blob/master/yak_defines.h)
* A lightweight unit testing framework (include your code in your test file and define your tests!): [yak_test.h](../blob/master/yak_test.h)
* String operations: [yak_string.h](../blob/master/yak_string.h)
* Memory management: [yak_memory.h](../blob/master/yak_memory.h)
* Win32 Platform layer: [yak_win32.h](../blob/master/yak_win32.h)

I'm committing to regularly update these libraries and new ones for the benefit of everyone.