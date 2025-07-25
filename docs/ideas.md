# Multiprecision for C++

## Implementation ideas

First, I need to introduce to multiprecision's int structure.

I'm targeting various architectures and CPUs. My implementation should be fast yet memory-efficient.

As a requirement, n-bit int must be n-bits in size. So, basically we have a bitset with arithmetic upon it.

### Multiplication

Multiplying two large numbers is as complex as $`\mathcal{O}\left(n^2\right)`$. It's slow mainly due to huge amount of addition operations.

Naive approach is to perform multiplication bit-by-bit. This is painfuly slow.

Faster solution would be multiplying 32-bit chunks with each other. Cast them to 64-bit integers, so overflow goes to the next chunk.

How to handle overflow correclty?

Well, you can store each 32-bit chunk of multiplication result as 64-bit integer, thus keeping overflow information. Now each real 32-bit chunk of resuling number is a sum of low 32 bits of current chunk and high 32 bits of previous 64-bit integer ( previous chunk overflow ).

### Division

It's the most complex operation. Its complexity is $`\mathcal{O}\left(n^2 \mathrm{log}\left(n\right)\right)`$

Division is implemented as simple binary search algorithm. With $`x = \frac{a}{b}`$, your goal is to find biggest $`x`$, such that $`x \cdot b \le a`$.

However, problems may arise because of multiplication overflow. Binsearch's upper limit should be such number $`U`$, that does not overflow when multiplying by divisor.

Imagine dividing two 8-bit unsigned integers: $`227`$ and $`5`$, which are $`111001_2`$ and $`101_2`$ in binary. Integer division result is $`45`$ ( keep in mind that $`227`$ is not really divisible by $`5`$ ), and now we'll find it with binary search:

When $a$ is divided by $b$, division result is always somewhere between $`0`$ ( $`b > a`$ ) and $`a`$ ( $`b = 1`$ ), i.e. in the range $`\left[0; a\right]`$.

So, we're starting from the middle point:

| Range                     | Middle point $`c`$ | $`c \cdot 5`$ |
| ------------------------- | ------------------ | ------------- |
| $`\left[0; 228\right)`$   | $`114`$            | $`58`$        |
| $`\left[114; 228\right)`$ | $`171`$            | $`87`$        |
| $`\left[171; 228\right)`$ | $`199`$            | $`227`$       |

We've got answer $`199`$. But this is wrong! Why?
Well, if you mutliply $`199`$ and $`5`$ as 8-bit integers - you'll see $`227`$ as the result.

Overflow is what we need to get rid of.

Let's take a closer look: result overflows each time we try to multiply $`5`$ with anything bigger than $`51`$. Overflow means that multiplication is guaranteed to be greater $`227`$ ( and any other 8-bit integer ), therefore we need to relax binsearch upper limit.

But how to do this?

First, when you multiply by $`5`$, you actually multiply by $`101_2`$. From that point, we can try limit result ( for $`x > 0`$ ):
$$x \cdot 100_2 < x \cdot 101_2 < x \cdot 1000_2$$

Result is never bigger than bitwise shifting $`x`$ by 4 bits.
So, what's the maximum number that is guaranteed not to overflow with multiplication by 5? For `uint8`, it's $`255`$ bitwise shifted by 3 bits - $`31`$. The real result ( which is $`51`$ in this case: $`51 \cdot 5 = 255`$ ) will lay somewhere between $`31`$ and $`63`$.

Let's set upper limit to $`63`$ for binsearch and try original example one more time:

| Range                   | Middle point $`c`$ | $`c \cdot 5`$ |
| ----------------------- | ------------------ | ------------- |
| $`\left[0; 63\right)`$  | $`31`$             | $`155`$       |
| $`\left[32; 63\right)`$ | $`47`$             | $`235`$       |
| $`\left[32; 47\right)`$ | $`39`$             | $`195`$       |
| $`\left[40; 47\right)`$ | $`43`$             | $`215`$       |
| $`\left[44; 47\right)`$ | $`45`$             | $`225`$       |

Now we're good, answer is $`45`$, because $`225 \le 227 \le 230`$.

Take another example: $`255`$ / $`15`$:

$`15_{10} = 1111_2`$

Takes only 4 bits.

`255 >> 4 = 15`

`255 >> 3 = 31`

Result of integer division `255 / 15` will lay somewhere between $`15`$ and $`31`$.

Upper limit: `255 >> (4 - 1) = 31`.

| Range                   | Middle point $`c`$ | $`c \cdot 15`$ |
| ----------------------- | ------------------ | -------------- |
| $`\left[0; 31\right)`$  | $`15`$             | $`225`$        |
| $`\left[16; 31\right)`$ | $`23`$             | $`89`$         |

We've just overflowed. But it's actually easy to deal with it: if $`c \cdot b`$ ( in this case $`23 \cdot 15 = 89`$ ) is smaller than range start times $`b`$ ( in this case for range $`\left[16; 31\right)`$ is's $`16 \cdot 15 = 240`$ ), then we've got overflow. But why don't we used this approach earlier? Because it was not guaranteed that overflowed value would be less than range start times $`b`$.

However, this case is different:

> Result of integer division `255 / 15` will lay somewhere between $`15`$ and $`31`$.

So, $`15 \cdot 16`$ does not overflow. $`15 \cdot 31`$ does, but is smaller than $`15 \cdot 16`$. Therefore, every overflowed value will be smaller than $`15 \cdot 16`$:

| $`x`$ | $`x \cdot 15`$          |
| ----- | ----------------------- |
| 16    | 240                     |
|       | _overflow starts here!_ |
| 20    | 44                      |
| 24    | 104                     |
| 28    | 164                     |
| 31    | 209                     |

> Every overflowed value is less than $`240`$.

That's the point.

Here's how improved algo looks like:

| Range                   | Middle point $`c`$ | $`c \cdot 15`$ | Status                             |
| ----------------------- | ------------------ | -------------- | ---------------------------------- |
| $`\left[0; 31\right)`$  | $`15`$             | $`225`$        | _normal_: $`225 > 0 \cdot 15`$     |
| $`\left[16; 31\right)`$ | $`23`$             | $`89`$         | _overflowed_: $`89 < 16 \cdot 15`$ |
| $`\left[16; 23\right)`$ | $`19`$             | $`29`$         | _overflowed_: $`29 < 16 \cdot 15`$ |
| $`\left[16; 19\right)`$ | $`17`$             | $`255`$        | _normal_: $`255 > 16 \cdot 15`$    |
