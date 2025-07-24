# Multiprecision for C++

## Benchmarks

All benchmarks are available in `/bench` folder and use **nanobench** as benchmarking library.

As primary target, I'll benchmark arithmetic operations: _incrementing, decrementing, addition, multiplication and division_.

### AMD Ryzen 9 9950X, 4.3 GHz

#### 256-bit integers

| Benchmark | Operations per second | ms per operation |
| --------- | --------------------- | ---------------- |
| `++x`     | 1,377,029,793         | 0.00000073       |
| `--x`     | 1,368,109,397         | 0.00000073       |
| `a + b`   | 111,209,073           | 0.00000899       |
| `a * b`   | 51,424,594            | 0.00001945       |
| `a / b`   | 1,255,010             | 0.00079681       |

#### 1024-bit integers

| Benchmark | Operations per second | ms per operation |
| --------- | --------------------- | ---------------- |
| `a + b`   | 20,408,274            | 0.00004900       |
| `a * b`   | 3,878,536             | 0.00025783       |
| `a / b`   | 531,018               | 0.00188317       |

#### 16384-bit integers

| Benchmark | Operations per second | ms per operation |
| --------- | --------------------- | ---------------- |
| `a + b`   | 1,283,494             | 0.00077912       |
| `a * b`   | 17,561                | 0.05694164       |
| `a / b`   | 5,461                 | 0.18308960       |
