# Multiprecision for C++

## Benchmarks

All benchmarks are available in `/bench` folder and use **nanobench** as benchmarking library.

As primary target, I'll benchmark arithmetic operations: _incrementing, decrementing, addition, multiplication and division_.

### AMD Ryzen 9 9950X, 4.3 GHz

#### 256-bit integers

| Bench   | Operations per second | time per operation |
| ------- | --------------------- | ------------------ |
| `++x`   | 5,320,235,622         | 0.19 ns            |
| `--x`   | 2,661,642,904         | 0.38 ns            |
| `a + b` | 264,272,149           | 3.78 ns            |
| `a * b` | 98,310,678            | 10.17 ns           |
| `a / b` | 3,340,166             | 299.39 ns          |

#### 1024-bit integers

| Bench   | Operations per second | time per operation |
| ------- | --------------------- | ------------------ |
| `a + b` | 196,446,325           | 5.09 ns            |
| `a * b` | 13,785,549            | 72.54 ns           |
| `a / b` | 1,592,370             | 627.99 ns          |

#### 16384-bit integers

| Bench   | Operations per second | time per operation |
| ------- | --------------------- | ------------------ |
| `a + b` | 27,156,951            | 36.82 ns           |
| `a * b` | 705,637               | 1,417.16 ns        |
| `a / b` | 76,088                | 13,142.61 ns       |

## Comparison with `boost::multiprecision`

Here I compare results of my library with **boost**'s implementation. Keep in mind, that theese bechmarks cover cases, when most of the integer bits are used. In reality, **boost** optimizes calculations when most of the bits are zero, which my library don't use.

### Intel Core i7 7740X 4.3 GHz

#### 256-bit integers

| Bench   | ops/s ( boost ) | ops/s ( multiprecision ) | ns/op ( boost ) | ns/op ( multiprecision ) |
| ------- | --------------- | ------------------------ | --------------- | ------------------------ |
| `++x`   | 849,908,460     | 1,052,124,897            | 1.18            | 0.95                     |
| `--x`   | 899,667,545     | 975,316,019              | 1.13            | 1.03                     |
| `a + b` | 121,607,707     | 166,078,996              | 8.22            | 6.02                     |
| `a * b` | 40,907,726      | 41,268,835               | 24.45           | 24.23                    |
| `a / b` | 7,429,913       | 2,643,523                | 134.59          | 378.28                   |

#### 1024-bit integers

| Bench   | ops/s ( boost ) | ops/s ( multiprecision ) | ns/op ( boost ) | ns/op ( multiprecision ) |
| ------- | --------------- | ------------------------ | --------------- | ------------------------ |
| `a + b` | 88,586,729      | 117,690,777              | 11.29           | 8.50                     |
| `a * b` | 6,537,428       | 10,699,275               | 152.97          | 93.46                    |
| `a / b` | 1,631,633       | 570,242                  | 612.88          | 1,753                    |

#### 16384-bit integers

| Bench   | ops/s ( boost ) | ops/s ( multiprecision ) | ns/op ( boost ) | ns/op ( multiprecision ) |
| ------- | --------------- | ------------------------ | --------------- | ------------------------ |
| `a + b` | 9,835,071       | 10,071,651               | 101.68          | 99.29                    |
| `a * b` | 33,225          | 582,936                  | 30,097.09       | 1,715.45                 |
| `a / b` | 42,737          | 68,889                   | 23,398.43       | 14,516.04                |
