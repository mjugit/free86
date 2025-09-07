# SystemCore
The SystemCore Library provides the most commonly used type definitions. These are mainly abbreviations and fundamental data types.

## Types

#### Boolean
A classic Boolean value, as used in many other programming languages. Even though in C any value other than 0 is treated as true, it significantly improves readability. The value is implemented as an enum, which is converted to an integer by the compiler. This may seem wasteful at first glance, given the limited amount of memory available, but it noticeably improves performance.


| Type   | Min value     | Max value     |
|--------|---------------|---------------|
| `bool` | `false` (= 0) | `true` (<> 0) |


#### Signed integers

Integer numeric values with signs. Even though C provides its own types for this purpose, the defined type aliases are significantly shorter and follow the general naming convention used throughout this codebase.

| Type  | Min value                       | Max value                        |
|-------|---------------------------------|----------------------------------|
| `I8`  | `-128`                          | `127`                            |
| `I16` | `-32,768`                       | `32,767`                         |
| `I32` | `-2,147,483,648`                | `2,147,483,647`                  |
| `I64` | `-9,223,372,036,854,775,808`    | `9,223,372,036,854,775,807`      |


#### Unsigned integers

Integer numeric values without signs. These types are used when negative values are not required, allowing the full bit width to represent positive numbers. The type aliases are shorter and follow the same naming convention as their signed counterparts.

| Type  | Min value | Max value                        |
|-------|-----------|----------------------------------|
| `U8`  | `0`       | `255`                            |
| `U16` | `0`       | `65,535`                         |
| `U32` | `0`       | `4,294,967,295`                  |
| `U64` | `0`       | `18,446,744,073,709,551,615`     |


