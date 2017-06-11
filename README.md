Building BugEngine
==================
[![Build Status](https://travis-ci.org/bugengine/BugEngine.svg?branch=master)](https://travis-ci.org/bugengine/BugEngine)

Supported platforms
-------------------

The following platforms are currently tested and known to build & work:

#### Win32 (Windows 7 to Windows 10)
You will need a properly configured compiler
* Visual Studio: from 2003 to 2017
* gcc: MingW32, MSys2, version 3.4 to 6
* gcc: MingW32, MSys2, version 3.4 minimum
* Intel compiler: version 12 and 13

#### Linux:
* gcc
* suncc
* clang
* intel compilers

#### MacOS X:
* gcc (and llvm-gcc)
* clang
* intel compilers

#### FreeBSD:
* gcc
* clang

#### Solaris:
* gcc
* suncc



## Building

You need one of the supported compilers installed and working
You need python installed (tested with python 2.7 and above)
You need Flex and Bison (for windows they are included in mak/win32)

waf is the build system that is used for building.
waf can also be used to generate projects for MSVC 2003 and above, including express versions, XCode, NetBeans, QtCreator


### Configuring

In the root folder of the project, run
```sh
> python waf configure
```
it will detect all the supported toolchains that it can find:
* GCC, sunCC and clang should be in the path (except for MacOSX, where they are shipped with Xcode).
* on Windows, MSVC, intel compilers and Platform SDK are detected automatically.

Run
```sh
> python waf --help
```
to print a list of options.


### Building from the command line


when you have decided which compiler to use, you can build by running
```sh
> python waf build:platform-architecture-compiler-version:variant
```
To list all supported combinations, run
```sh
> python waf build
```


### Bulding with IDEs

The other way of building is to create projects for the supported IDEs and build from the IDE.
The supported IDEs Visual Studio 2003 to 2013 in their complete or express editions, XCode, NetBeans, Eclipse and QtCreator.
To generate the project files, configure the project and run
```sh
> python waf [IDE] [IDEs...]
```
run
```sh
> python waf --help
```
to print a list of all supported toolchains and IDE.

For instance,
```sh
> python waf vs2005e vs2010
```
will create a set of projects and a solution for Visual Studio 2005 express, and another set of projects and solutions for Visual Studio 2010.
You don't need to have these installed to generate the projects, you just need them to actually open the projects.

In most IDE, you will be able to pick which toolchain to use with the "target" or "platform" setting of the IDE.

