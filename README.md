# Multiprecision for C++

## Introduction

This is a library adding multiprecision support for `int`. I've intended this as yet another PET-project to improve my C++ skills and knowledge. Besides from that, you're free to use it in any cases, as the **MIT license** says.

Here, I target x86, ARM and AVR32 architectures, with `clang` and `gcc` the only supported compilers.

### Example usage

Pass include path to `/src/include` as compiler argument. `multiprecision.hpp` is the library _all-in-one_ include file.

Usage of multiprecision int is the same as builtin integers in C++.

#### Simple sum

```cpp
#include <iostream>
#include <multiprecision.hpp>

int main() {
    multiprecision::int_t<256> a;
    multiprecision::int_t<256> b;

    std::cin >> a >> b;

    std::cout << a + b << "\n";

    return 0;
}
```

| Input                                                    | Output                        |
| -------------------------------------------------------- | ----------------------------- |
| `234827839582374629384342756` `923478238745908324872346` | `235751317821120537709215102` |

#### Factorials

```cpp
#include <iostream>
#include <multiprecision.hpp>

int main() {
    uint64_t n;

    std::cin >> n;

    multiprecision::int_t<2048> factorial = 1;

    for (uint64_t i = 2; i <= n; ++i) {
        factorial *= int64_t(i);
    }

    std::cout << factorial << "\n";

    return 0;
}
```

| Input | Output                                                                                                                                                                                                    |
| ----- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `120` | `6689502913449127057588118054090372586752746333138029810295671352301633557244962989366874165271984981308157637893214090552534408589408121859898481114389650005964960521256960000000000000000000000000000` |

## Docs

In the docs you'll find all ideas I've used and everything I've learned from this PET-project. Benchmarks and comparisons with other implementations are included:

- [Implementation ideas](./docs/ideas.md)
- [Benchmarks](./docs/benchs.md)

Hope this will help someone else to better understand the internals of C++ and multiprecision arithmetic.

## Further development

As another C++ practise I wanted to implement multiprecision `float` with arbitrary _exponent_, _mantissa_ and _period_ for periodic fractions.

_I wish I had more time for this..._
