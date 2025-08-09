#ifndef _CORE_TYPES_H_
#define _CORE_TYPES_H_

/**
 * @brief Boolean type definition for C.
 *
 * This `typedef` defines a `bool` type for use in C, with values `true` and `false`.
 * It provides a simple representation of boolean logic in environments where the
 * standard `stdbool.h` header or built-in `bool` type is not available.
 *
 * Values:
 * - `false` (0): Represents the logical false state.
 * - `true` (1): Represents the logical true state.
 */
typedef enum {
    false = 0, ///< Logical false value.
    true = 1   ///< Logical true value.
} bool;

#endif
