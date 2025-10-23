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

#include "MinUnit.h"

#include "../Source/Modules/Include/Collection.h"


use(Collection);
use(Heap);


// Generic list tests

MU_TEST(GenericList_Create__HeapIsNull__ReturnsNull) {
  mu_assert(!Collection.List.Create(null), "Create returned not null.");
}

MU_TEST(GenericList_Create__EnoughSpace__CreatesInstance) {
  U8 testBuffer[128];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");

  List* list = Collection.List.Create(heap);

  mu_assert(!list->Head, "Head pointer was not reset.");
  mu_assert(!list->Tail, "Tail pointer was not reset.");
  mu_assert_int_eq(0, list->Count);
  mu_assert(list->Heap == heap, "Heap pointer was not set.");
}


MU_TEST(GenericList_Dispose__ListIsNull__ReturnsImmediately) {
  // Should cause a segmentation fault on error
  Collection.List.Dispose(null);
}

MU_TEST(GenericList_Dispose__EntriesExist__FreesAll) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  U32 bytesFreeBefore = heap->TotalBytesFree;
  
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 existing = 4321;
  Collection.List.Add(list, &existing);
  Collection.List.Add(list, &existing);
  Collection.List.Add(list, &existing);
  Collection.List.Add(list, &existing);
  mu_assert_int_eq(4, list->Count);
  
  // Method to test
  Collection.List.Dispose(list);

  mu_assert_int_eq(bytesFreeBefore, heap->TotalBytesFree);
}


MU_TEST(GenericList_Add__ListIsNull__ReturnsImmediately) {
  U8 dummy;

  // Should cause a segmentation fault on error
  Collection.List.Add(null, &dummy);
}

MU_TEST(GenericList_Add__ItemIsNull__AddsItem) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  Collection.List.Add(list, null);

  mu_assert_int_eq(1, list->Count);
  mu_assert(list->Head, "Head not set.");
  mu_assert(list->Tail, "Tail not set.");
  mu_assert(list->Head->Payload == null, "Payload not set correctly.");
}

MU_TEST(GenericList_Add__EmptyList__AddsItem) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 expected = 12345;

  Collection.List.Add(list, &expected);

  mu_assert_int_eq(1, list->Count);
  mu_assert(list->Head, "Head not set.");
  mu_assert(list->Tail, "Tail not set.");
  mu_assert(list->Head->Payload == &expected, "Payload not set correctly.");
}

MU_TEST(GenericList_Add__ListNotEmpty__AddsItem) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 existing = 4321;
  Collection.List.Add(list, &existing);

  U16 expected = 12345;
  Collection.List.Add(list, &expected);

  mu_assert_int_eq(2, list->Count);
  mu_assert(list->Head, "Head not set.");
  mu_assert(list->Tail, "Tail not set.");
  mu_assert(list->Tail->Payload == &expected, "List tail is wrong.");
  
  mu_assert(list->Head->Payload == &existing, "List head is wrong.");
  mu_assert(list->Head->Next == list->Tail, "New item not linked properly.");
}


MU_TEST(GenericList_Remove__ListIsNull__ReturnsFalse) {
  U8 dummy;
  mu_check(!Collection.List.Remove(null, &dummy));
}

MU_TEST(GenericList_Remove__ItemNotFound__ReturnsFalse) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 dummy = 12345;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(4, list->Count);

  U8 toRemove;
  mu_check(!Collection.List.Remove(list, &toRemove));
}

MU_TEST(GenericList_Remove__ItemFound__ReturnsTrue) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 dummy = 12345;
  U8 toRemove;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &toRemove);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(4, list->Count);

  mu_check(Collection.List.Remove(list, &toRemove));
  mu_assert_int_eq(3, list->Count);
}

MU_TEST(GenericList_Remove__LastItem__ReturnsTrue) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 dummy = 12345;
  U8 toRemove;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &toRemove);
  mu_assert_int_eq(4, list->Count);

  mu_check(Collection.List.Remove(list, &toRemove));
  mu_assert_int_eq(3, list->Count);
}

MU_TEST(GenericList_Remove__FirstItem__ReturnsTrue) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 dummy = 12345;
  U8 toRemove;
  Collection.List.Add(list, &toRemove);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(4, list->Count);

  mu_check(Collection.List.Remove(list, &toRemove));
  mu_assert_int_eq(3, list->Count);
}



MU_TEST(GenericList_Contains__ListIsNull__ReturnsFalse) {
  U8 dummy;
  mu_check(!Collection.List.Contains(null, &dummy));
}

MU_TEST(GenericList_Contains__EmptyList__ReturnsFalse) {
  U8 testBuffer[512];
    
  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 dummy;

  mu_check(!Collection.List.Contains(list, &dummy));
}

MU_TEST(GenericList_Contains__NotFound__ReturnsFalse) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 dummy = 12345;
  U8 requested;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(4, list->Count);

  mu_check(!Collection.List.Contains(list, &requested));
}

MU_TEST(GenericList_Contains__Found__ReturnsTrue) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 dummy = 12345;
  U8 requested;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &requested);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(5, list->Count);

  mu_check(Collection.List.Contains(list, &requested));
}


MU_TEST(GenericList_Clear__Always__ResetsListItems) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");
  U32 bytesBeforeAdd = heap->TotalBytesFree;

  U16 dummy = 12345;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(4, list->Count);

  // Method to test
  Collection.List.Clear(list);

  mu_assert_int_eq(bytesBeforeAdd, heap->TotalBytesFree);
  mu_assert_int_eq(0, list->Count);
  mu_assert(!list->Head, "Head was not reset.");
  mu_assert(!list->Tail, "Tail was not reset.");
}


// These are used to verify, that the callback was invoked

U8 ForEachCallbackCounter;
void ForEachDummyCallback(void* item) {
  ForEachCallbackCounter++;
}

MU_TEST(GenericList_ForEach__ListIsNull__ReturnsImmediately) {
  Collection.List.ForEach(null, ForEachDummyCallback);
}

MU_TEST(GenericList_ForEach__CallbackIsNull__ReturnsImmediately) {
  U8 testBuffer[512];
    
  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  Collection.List.ForEach(list, null);
}

MU_TEST(GenericList_ForEach__ListIsEmpty__ReturnsImmediately) {
  U8 testBuffer[512];
    
  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  Collection.List.ForEach(list, ForEachDummyCallback);
}

MU_TEST(GenericList_ForEach__ListNotEmpty__InvokesCallbackForEachItem) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U16 dummy = 12345;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(4, list->Count);

  ForEachCallbackCounter = 0;
  Collection.List.ForEach(list, ForEachDummyCallback);

  mu_assert_int_eq(4, ForEachCallbackCounter);
}


bool AnyDummyTest(void* item) {
  U8* ptr = item;
  return *ptr == 0xff;
}

MU_TEST(GenericList_Any__ListIsNull__ReturnsFalse) {
  mu_check(!Collection.List.Any(null, AnyDummyTest));
}

MU_TEST(GenericList_Any__TestIsNull__ReturnsFalse) {
  U8 testBuffer[512];
    
  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  mu_check(!Collection.List.Any(list, null));
}

MU_TEST(GenericList_Any__NoneSucceeds__ReturnsFalse) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U8 dummy = 123;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(4, list->Count);

  mu_check(!Collection.List.Any(list, AnyDummyTest));
}

MU_TEST(GenericList_Any__AnySucceeds__ReturnsTrue) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U8 dummy = 123;
  U8 success = 0xff;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &success);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(4, list->Count);

  mu_check(Collection.List.Any(list, AnyDummyTest));
}


bool AllDummyTest(void* item) {
  U8* ptr = item;
  return *ptr == 0xff;
}

MU_TEST(GenericList_All__ListIsNull__ReturnsFalse) {
  mu_check(!Collection.List.All(null, AllDummyTest));
}

MU_TEST(GenericList_All__TestIsNull__ReturnsFalse) {
  U8 testBuffer[512];
    
  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  mu_check(!Collection.List.All(list, null));
}

MU_TEST(GenericList_All__NotAllSucceed__ReturnsFalse) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U8 dummy = 123;
  U8 success = 0xff;
  Collection.List.Add(list, &success);
  Collection.List.Add(list, &success);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &success);
  mu_assert_int_eq(4, list->Count);

  mu_check(!Collection.List.All(list, AllDummyTest));
}

MU_TEST(GenericList_All__AllSucceed__ReturnsTrue) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U8 success = 0xff;
  Collection.List.Add(list, &success);
  Collection.List.Add(list, &success);
  Collection.List.Add(list, &success);
  Collection.List.Add(list, &success);
  mu_assert_int_eq(4, list->Count);

  mu_check(Collection.List.All(list, AllDummyTest));
}


bool FirstDummyTest(void* item) {
  U8* ptr = item;
  return *ptr == 0xff;
}

MU_TEST(GenericList_First__ListIsNull__ReturnsNull) {
  mu_check(!Collection.List.First(null, FirstDummyTest));
}

MU_TEST(GenericList_First__TestIsNull__ReturnsNull) {
  U8 testBuffer[512];
    
  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  mu_check(!Collection.List.First(list, null));
}

MU_TEST(GenericList_First__NotFound__ReturnsNull) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U8 dummy = 123;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(4, list->Count);

  mu_check(!Collection.List.First(list, FirstDummyTest));
}

MU_TEST(GenericList_First__Found__ReturnsItem) {
  U8 testBuffer[512];

  HeapArea* heap = Heap.Initialize(testBuffer, sizeof(testBuffer));
  mu_assert(heap, "Unable to initialize heap.");
  List* list = Collection.List.Create(heap);
  mu_assert(list, "Unable to create list.");

  U8 dummy = 123;
  U8 success = 0xff;
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &dummy);
  Collection.List.Add(list, &success);
  Collection.List.Add(list, &dummy);
  mu_assert_int_eq(4, list->Count);

  mu_check(Collection.List.First(list, FirstDummyTest) == &success);
}



MU_TEST_SUITE(GenericList) {
  // Create
  MU_RUN_TEST(GenericList_Create__HeapIsNull__ReturnsNull);
  MU_RUN_TEST(GenericList_Create__EnoughSpace__CreatesInstance);

  // Dispose
  MU_RUN_TEST(GenericList_Dispose__ListIsNull__ReturnsImmediately);
  MU_RUN_TEST(GenericList_Dispose__EntriesExist__FreesAll);

  // Add
  MU_RUN_TEST(GenericList_Add__ListIsNull__ReturnsImmediately);
  MU_RUN_TEST(GenericList_Add__ItemIsNull__AddsItem);
  MU_RUN_TEST(GenericList_Add__EmptyList__AddsItem);
  MU_RUN_TEST(GenericList_Add__ListNotEmpty__AddsItem);

  // Remove
  MU_RUN_TEST(GenericList_Remove__ListIsNull__ReturnsFalse);
  MU_RUN_TEST(GenericList_Remove__ItemNotFound__ReturnsFalse);
  MU_RUN_TEST(GenericList_Remove__ItemFound__ReturnsTrue);
  MU_RUN_TEST(GenericList_Remove__FirstItem__ReturnsTrue);
  MU_RUN_TEST(GenericList_Remove__LastItem__ReturnsTrue);

  // Contains
  MU_RUN_TEST(GenericList_Contains__ListIsNull__ReturnsFalse);
  MU_RUN_TEST(GenericList_Contains__EmptyList__ReturnsFalse);
  MU_RUN_TEST(GenericList_Contains__NotFound__ReturnsFalse);
  MU_RUN_TEST(GenericList_Contains__Found__ReturnsTrue);

  // Clear
  MU_RUN_TEST(GenericList_Clear__Always__ResetsListItems);

  // ForEach
  MU_RUN_TEST(GenericList_ForEach__ListIsNull__ReturnsImmediately);
  MU_RUN_TEST(GenericList_ForEach__CallbackIsNull__ReturnsImmediately);
  MU_RUN_TEST(GenericList_ForEach__ListIsEmpty__ReturnsImmediately);
  MU_RUN_TEST(GenericList_ForEach__ListNotEmpty__InvokesCallbackForEachItem);

  // Any
  MU_RUN_TEST(GenericList_Any__ListIsNull__ReturnsFalse);
  MU_RUN_TEST(GenericList_Any__TestIsNull__ReturnsFalse);
  MU_RUN_TEST(GenericList_Any__NoneSucceeds__ReturnsFalse);
  MU_RUN_TEST(GenericList_Any__AnySucceeds__ReturnsTrue);

  // All
  MU_RUN_TEST(GenericList_All__ListIsNull__ReturnsFalse);
  MU_RUN_TEST(GenericList_All__TestIsNull__ReturnsFalse);
  MU_RUN_TEST(GenericList_All__NotAllSucceed__ReturnsFalse);
  MU_RUN_TEST(GenericList_All__AllSucceed__ReturnsTrue);

  // First
  MU_RUN_TEST(GenericList_First__ListIsNull__ReturnsNull);
  MU_RUN_TEST(GenericList_First__TestIsNull__ReturnsNull);
  MU_RUN_TEST(GenericList_First__NotFound__ReturnsNull);
  MU_RUN_TEST(GenericList_First__Found__ReturnsItem);
}




int main(void) {
  MU_RUN_SUITE(GenericList);

  MU_REPORT();

  return MU_EXIT_CODE;
}


