# Collection module
This module provides dynamic, heap-based data structures for storing and managing collections of elements at runtime. It offers a generic interface that can be used to create, traverse, and modify sets of objects without having to handle the underlying memory management manually.

## Introduction
Collections are a fundamental part of higher-level system behavior. While arrays or buffers are fixed in size and require explicit allocation, collections are dynamic - they can grow and shrink as elements are added or removed.

The free86 Collection module provides such dynamic data structures while maintaining full control over memory. All elements and management structures are allocated on the provided heap, keeping the system’s memory usage explicit and traceable.


## Using the module
Include the header and import the module instance with the `use(...)` macro:

```c
#include "Modules/Include/Collection.h"

use(Collection);
```

This makes the entire collection namespace available, including the List type and its operations. The user should not import `GenericList` directly - it is considered an internal implementation detail.


Example:

```c
// Create a new list on a given heap
List* myList = Collection.List.Create(myHeap);

// Add elements
Collection.List.Add(myList, somePointer);
Collection.List.Add(myList, anotherPointer);

// Iterate over elements
Collection.List.ForEach(myList, PrintElement);

// Dispose when done
Collection.List.Dispose(myList);
```

All internal allocations are managed automatically by the collection. The only requirement is that a valid heap is provided during creation.


## Data structures

### `ListItem`
Represents a single node within a list. Each item holds a payload pointer and maintains links to the previous and next elements.

```c
struct ListItem {
  void *Payload;
  ListItem* Next;
  ListItem* Previous;
};
```


### `List`
Represents the list container itself. Holds references to the heap, head and tail nodes, and keeps track of the total element count.

```c
struct List {
  HeapArea* Heap;
  ListItem* Head;
  ListItem* Tail;
  U32 Count;
};
```


## Function reference
The following functions form the generic collection interface. All operate on dynamically allocated structures residing in the specified heap.

### `List.Create`
Allocate a new list on the given heap.

```c
List* Create(HeapArea* heap);
```

| Parameter | Description                              |
| --------- | ---------------------------------------- |
| `heap`    | Heap on which to allocate the collection |

| Returns | Description                                             |
|---------|---------------------------------------------------------|
| `List*` | Pointer to a new collection instance or `null` on error |



### `List.Dispose`
Free all resources associated with the list. Note that this only concerns those related to the list itself, but *not* its actual item values. These are considered to be managed elsewhere.

```c
void Dispose(List* this);
```

| Parameter | Description               |
| --------- | ------------------------- |
| `this`    | Pointer to the collection |



### `List.Add`
Append a new element to the list.

```c
void Add(List* this, void* payload);
```

| Parameter | Description                  |
| --------- | ---------------------------- |
| `this`    | Pointer to the collection    |
| `payload` | Pointer to the stored object |



### `List.Remove`
Remove the first matching element from the list.

```c
bool Remove(List* this, void* payload);
```

| Parameter | Description                      |
| --------- | -------------------------------- |
| `this`    | Pointer to the collection        |
| `payload` | Pointer to the element to remove |

| Returns | Description         |
| ------- | ------------------- |
| `true`  | Element was removed |
| `false` | Element not found   |



### `List.Contains`
Check whether a given element exists in the list.

```c
bool Contains(List* this, void* payload);
```

| Parameter | Description               |
| --------- | ------------------------- |
| `this`    | Pointer to the collection |
| `payload` | Element to search for     |

| Returns | Description       |
| ------- | ----------------- |
| `true`  | Element exists    |
| `false` | Element not found |



### `List.Clear`
Remove all elements from the list.

```c
void Clear(List* this);
```

| Parameter | Description               |
| --------- | ------------------------- |
| `this`    | Pointer to the collection |



### `List.ForEach`
Iterate through all elements and execute a callback for each.

```c
void ForEach(List* this, void (*callback)(void*));
```

| Parameter  | Description                               |
| ---------- | ----------------------------------------- |
| `this`     | Pointer to the collection                 |
| `callback` | Function to call for each element payload |



### `List.Any`
Test whether any element satisfies a given condition.

```c
bool Any(List* this, bool (*test)(void*));
```

| Parameter | Description                                    |
| --------- | ---------------------------------------------- |
| `this`    | Pointer to the collection                      |
| `test`    | Predicate function returning `true` or `false` |

| Returns | Description                   |
| ------- | ----------------------------- |
| `true`  | At least one match found      |
| `false` | No element satisfied the test |



### `List.All`
Test whether all elements satisfy a given condition.

```c
bool All(List* this, bool (*test)(void*));
```

| Parameter | Description                                    |
| --------- | ---------------------------------------------- |
| `this`    | Pointer to the collection                      |
| `test`    | Predicate function returning `true` or `false` |

| Returns | Description                  |
| ------- | ---------------------------- |
| `true`  | All elements passed the test |
| `false` | At least one failed          |



### `List.First`
Return the first element that satisfies a given condition.

```c
void* First(List* this, bool (*test)(void*));
```

| Parameter | Description                                    |
| --------- | ---------------------------------------------- |
| `this`    | Pointer to the collection                      |
| `test`    | Predicate function returning `true` or `false` |

| Returns | Description                    |
| ------- | ------------------------------ |
| `void*` | Payload pointer of first match |
| `NULL`  | No element satisfied the test  |


