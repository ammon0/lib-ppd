# Lib-PPD : Portable Program Data

Lib-PPD is meant to be a compiler middle-end supporting a programming language that can run in either a virtual machine or be compiled to native code.

## Project Components
*	ppd: internal representation
*	dyn: textual representation
*	pexe: serialized representation

## Important Classes
<!--*	`PPDModule`: A compilation unit, standalone or otherwise, Contains static memory information and a list of routines-->
<!--*	`PPDRoutine`: -->
<!--*	`PPDBlock`: Contains a list of PPDInst's with a single entrance label, and a single exit-->
<!--*	`PPDInst`: A data structure containing a PPDInstCode and 2 or 3 arguments-->
<!--*	`PPDArg`: Arguments are referenced by pointers and may include labels, and definitions.-->
<!--*	`PPDDefs`: a container for definitions-->
<!--*	`PPDDefinition`: contains information for determining the size of a memory location or operand. Definitions are unnamed at this level and are accessed by reference.-->

## Project Pages
*	[Latest Release](https://github.com/ammon0/MPL/releases/latest)
*	[Documentation](https://ammon0.github.io/MPL/)
*	[github](https://github.com/ammon0/MPL)

## MIT License
Copyright (c) 2016-2018 Ammon Dodson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
