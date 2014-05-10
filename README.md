# Gaen Concurrency Engine

A cross platform, [component based game engine](http://en.wikipedia.org/wiki/Entity_component_system)
that automates CPU concurrency.

Game designers create components and entities and designate data
dependencies. The engine load balances these entities across all
available cores. Running on a machine with more cores will
automatically improve performance without any code modifications.

Gaen is licensed under the terms of the
[zlib License](http://en.wikipedia.org/wiki/Zlib_License).


## Compose Scripting Language

To enable rapid prototyping and empower non-C++ programmers to create
with Gaen, there is Compose. It is a C-like language with lots of
syntactic sugar around components, entities and message passing. It
eliminates the boiler plate verbosity that comes with forcing C++ into
a message passing, component based model.

The language can be interpreted in a VM for rapid prototyping, or
compiled into C++ for maximum performance after the scripts have been
written and tested.


## Platforms

Active development is for Windows, but the engine is designed to run
on OSX and Linux as well. Development started on OSX, so support is
pretty well established there. Linux will require some porting work of
small chunks of isolated code. AMD64 is assumed, but it should not be
too hard to move to another 64 bit architecture.

[C++11](http://en.wikipedia.org/wiki/C++11) features are used
extensively, so you must have a recent compiler.  On Windows this
means VC 2013 or above. On OSX and Linux, GCC 4.8 is recommended.


## Building

The build system is CMake based, which makes possible the generation
of VS Projects on Windows and make files on OSX and Linux. To run
CMake, run either bootstrap.bat or bootstrap.sh from the gaen
directory and it will place generated files in gaen/build. To
completely clean the build and start from scratch, run clean.bat or
clean.sh.

OSX is using makefiles and not Xcode. I found it a bit obtuse to get
C++11 compiler support in Xcode, so I abandoned the effort. CMake
enables the generation of Xcode projects, but the compiler issues will
have to be solved. I also do not appreciate the cruft Xcode places in
the generated bundle. The package that comes out of the makefile
process is much cleaner.

