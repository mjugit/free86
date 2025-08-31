/*
	
  Copyright © 2025 Maximilian Jung

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the “Software”), to deal in the Software without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following
  conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the
  Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY
  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#ifndef __C_MODULE_H__
#define __C_MODULE_H__

// Concatenate two raw tokens (e.g. A + B → AB).
// This macro performs no expansion — used as low-level building block.
#define __c_mod_concat(a, b) a##b

// Concatenate two tokens, with macro expansion of arguments before joining.
// Use this instead of __c_mod_concat(...) when arguments might themselves be macros.
#define __c_mod_expand(a, b) __c_mod_concat(a, b)

// Generate the internal typename of a module from its symbolic name.
// This ensures that each module struct type has a consistent and unique internal name.
#define __c_mod_typename(name) __c_mod_expand(__, __c_mod_expand(name, _Namespace__))




// Define the interface of a module.
#define module(name)					\
  typedef struct __c_mod_typename(name) __c_mod_typename(name); \
  struct __c_mod_typename(name)			

// Define the implementation of a module instance.
#define members(name)			\
  const __c_mod_typename(name) name = (__c_mod_typename(name)) 

// Import a module reference from another translation unit.
#define use(name)				\
  extern const __c_mod_typename(name) name

// Embed a module as a field within another module.
#define embed(name)				\
  const __c_mod_typename(name) name

#endif
