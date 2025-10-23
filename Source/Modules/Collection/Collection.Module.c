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


#include "../Include/Collection.h"


extern List*	_GenericList_CreateImplementation(HeapArea* heap);
extern void	_GenericList_DisposeImplementation(List* this);
extern void	_GenericList_AddImplementation(List* this, void *payload);
extern bool	_GenericList_RemoveImplementation(List* this, void* payload);
extern bool	_GenericList_ContainsImplementation(List* this, void* payload);
extern void	_GenericList_ClearImplementation(List* this);
extern void     _GenericList_ForEachImplementation(List* this, void (*callback)(void*));
extern void	_GenericList_ForEachImplementation(List* this, void (*callback)(void*));
extern bool	_GenericList_AnyImplementation(List* this, bool (*test)(void*));
extern bool	_GenericList_AllImplementation(List* this, bool (*test)(void*));
extern void*    _GenericList_FirstImplementation(List* this, bool (*test)(void*));


members(GenericList) {
    .Create   = _GenericList_CreateImplementation,
    .Dispose  = _GenericList_DisposeImplementation,
    .Add      = _GenericList_AddImplementation,
    .Remove   = _GenericList_RemoveImplementation,
    .Contains = _GenericList_ContainsImplementation,
    .Clear    = _GenericList_ClearImplementation,
    .ForEach  = _GenericList_ForEachImplementation,
    .Any      = _GenericList_AnyImplementation,
    .All      = _GenericList_AllImplementation,
    .First = _GenericList_FirstImplementation
};



members(Collection) {
    .List = GenericList
};
