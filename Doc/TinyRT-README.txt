    Copyright (C) 2009 Joshua Barczak

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


What is TinyRT??
----------------

TRT is a template library which seeks to provide a set of modular, generic, and reasonably efficient implementations of common raytracing 
algorithms. 

Its main goals are:
 - To provide a flexible reference codebase for research, prototyping, and hobbyist experimentation
 - To provide baseline components which may be modified for use in a production renderer
 - To provide a ready-made solution for 'lightweight' raytracing applications (such as collision detection)




How can I use TinyRT??
----------------------

Simply add TinyRT/include to your include list, and #include TinyRT.h  
Everything in TinyRT is contained in the TinyRT namespace.  TinyRT is a source code library, there is nothing to link against.

Doxygen documentation is available under doc\html.

In addition, The 'examples' directory contains several sample projects:

 - TRTRenderTest
     The main test harness for TinyRT, which exercises most of its functionality
 - TRTPick  
     A Direct3D sample application modified to use TinyRT
 - TRTInstancing
     Shows how to nest data structures with TinyRT



What isn't TinyRT (FOR NOW)??
------------------------------

* A packet tracer
    Eventually, I may add some generic packet tracing functionality to TinyRT, but at the moment, it is single ray only.


What isn't TinyRT (EVER)??
------------------------------
* A complete renderer

    TRT provides many of the components needed to build a simple raytracer, but geometry loading, image sampling, shading, and output
    must all be implemented at the application level.  

* The fastest possible raytracer
    
    TRT's priority is to be generic, flexible, and modular.  Although every effort has been made to optimize TRT's raycasting functions, 
    they have been deliberately designed to use a very general interface, in order to support as many diverse data structure implementations
    as possible.  This generality is likely to result in worse performance than could be acheived by an implementation which is specialized 
    for a particular data structure.  If maximum performance is desired, it may be best to pair one of TRT's data structures with a
    custom raytracing kernel that is specialized for it.


Changes in Version 1.1
---------------------------

- Fixes to support compilation with GCC (tested with DevC++ 4.2)
- Reflect/Refract functions
- Added aligned allocation helpers 
- Scratch memory is now 16-byte aligned
- Uniform grid DDA now uses a templated cell index type.  
- added a static(compile-time) assert macro