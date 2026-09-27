# HW2: Pointers etc. E4741 NDF 2117

## 1. The Machine (Laptop), the Build, and the way of Benchmarking

### My Machine is ...
**Machine**
- CPU: Intel Core Ultra 5 226V
- CPU cores: 8 cores / 8 logical processors
- RAM: 16 GB
- OS: Microsoft Windows 11 Home, version 10.0.26200
- Architecture: x86-64

### And I build through ...
**Build**
- Compiler: g++ 16.1.0
- Language: C++17
- Compiler flags: `-std=c++17 -O2 -Wall`
- Release optimization: `-O2`

### My Benchmark Method is...
**Benchmark method**
- Clock: `std::chrono::steady_clock`
- Warm-up: yes, whole warm-up batches were run before measurements
- Dead-code prevention: `doNotOptimize()` was used by the provided benchmark harness
- Table 2: 1000 samples × 2000 calls per sample
- Table 3: 200 samples × 1 full traversal
- Reported statistics: p50, p99, p99.9, and mean

I ran the benchmarks using the optimized release build rather than the
`-O0` debug build. I also avoided intentionally running other heavy programs
while collecting the reported measurements.

#### Important to note: Windows 'long' note

For me on my Windows/MinGW environment, `long` is 32 bits. Therefore the program
reports `sizeof(Big) = 584 B`, although the starter's Table 2 heading says
648 bytes.

I believe that this also causes Table 3 sum to overflow a 32-bit `long`. The
mathematical sum of the integers from 0 through 1,048,575 does not fit in
a 32-bit signed `long`, so the program reports `-524288`. Both traversal
functions produce the same value as the starter's expected-value calculation,
so the provided correctness check still reports `OK`.

### 2. The four tables - Results

HW 2 ΓÇö pointers, references & the cost of a copy
elements N = 1048576   sizeof(Big) = 584 B   sizeof(Node) = 16 B

TABLE 1 ΓÇö swap correctness
  function           a before   b before    a after    b after   result
  ---------------------------------------------------------------------
  swap_ref                  3          9          9          3   OK
  swap_ptr                  3          9          9          3   OK
  swap_ptr(&a,&a)           5          5          5          5   OK

All correctness tests passed!

Worth noting that `sizeof(Big)` on my system is **584 B**


TABLE 2 ΓÇö pass a 648-byte struct: by value vs by const reference
  variant                             p50          p99        p99.9         mean
                                  ns/call      ns/call      ns/call      ns/call
  ---------------------------------------------------------------------------------
  sum_by_value(Big)                67.850      152.350      318.850       68.952
  sum_by_cref(const Big&)          46.600      138.750      509.550       50.295
  p50 ratio value/cref = 1.46x     (checksum 30202200000.0, 1000 samples x 2000 calls)
  correctness: sum_by_value=7191.0  sum_by_cref=7191.0  OK (equal, non-zero)

TABLE 3 ΓÇö traverse 1,048,576 ints: contiguous vector vs linked list
  variant                             p50          p99        p99.9         mean
                                  ns/elem      ns/elem      ns/elem      ns/elem
  ---------------------------------------------------------------------------------
  sum_vector (contiguous)           0.453        2.762        4.072        0.554
  sum_list   (pointer chase)      176.333      310.753      384.130      177.493
  p50 ratio list/vector = 389.01x    (200 samples x 1 full traversal)
  correctness: sum_vector=-524288  sum_list=-524288  expected=-524288  OK
  bytes touched: vector 4.0 MB, list 16.0 MB

TABLE 4 ΓÇö build & method (state your machine in the README)
  compiler               g++ 16.1.0
  optimisation           -O2/-O3 (release)  OK
  language               __cplusplus = 201703L
  arch                   x86-64
  clock                  std::chrono::steady_clock (monotonic)
  warm-up                yes ΓÇö whole batches before the first sample
  reported               p50 / p99 / p99.9 over samples, plus mean
  dead-code guard        doNotOptimize() on every result
  machine                TODO(4): put your CPU / RAM / OS in the README

  The negative result is due to the 32-bit `long` behavior described above.

#### What did I do about noise?
I tried  intentionally running no other heavy programs while collecting the reported measurements.


### 3. Explanation A — `swap_ref` vs `swap_ptr` (part of the 3 pts)

- What the callee actually receives in each case, and what it costs

In `swap_ref`, the parameters are references. A reference acts as an alias
for an existing object, so changing `a` or `b` changes the caller's original
integers. So the function does not need explicit dereferencing.

In `swap_ptr`, the parameters used are pointers. A pointer rather stores an address.
Therefore, I use `*a` and `*b` to access and modify the integers stored at
those addresses!

In both cases, the function can access the callers existing integers and is not making copies of those integers just to perform the swap.

- Why `swap_ptr` needs `*` and `swap_ref` does not

Well, in `swap_ptr`, `a` is an address and `*a` gives the value located at that address.
So the pointer version has to dereference the pointer to change the callers int.

As already mentioned a reference is like an alias. So `a` in `swap_ref` can be used directly as the original integer itself.

- Why swapping the *pointers* inside `swap_ptr` does nothing to the caller

Swapping only the local pointers of `a` and `b`, we would onnly exchange the local copies of the addresses. But we want to exchange the integer values in the caller which would not happen if we do this.

So what we do is we swaps `*a` and `*b`, which are the actual pointed to values!

- `nullptr` is expressible for the pointer version and not for the reference
  version — say what that means for a caller, and whether you added a null
  check (either choice is fine; say which and why)

If a pointer containts  `nullptr` it means it points to no object. A reference is intended to refer to an existing object and does not have an equivalent normal null state. In my `swap_ptr` implementation I do not make a null check, so its caller must provide valid pointers. So I am not  adding a conditional branch to this small function, but it would also mean passing `nullptr` would  also be an invalid usage.


- Which one you would use in an HFT hot path and why

well, when null is not a meaningful input, I'd prefer the reference version.
Why? We have directly expressed that the functions expects existing integers and doesn't need explicit pointer dereferencing. However, both versions have their benefits. The key is rather interface semantic and not the pointer performance. 

### 4. Explanation B — contiguous vs pointer chasing (part of the 2 pts)

Same amount of arithmetic, same number of `int`s summed, very different time.
Explain *why*, in terms of the week-2 memory hierarchy:

- cache lines: how many `int`s ride along in one 64-byte line vs how many
  useful bytes you get from a 16-byte `Node`

As we discussed in class, locality does matter for the eventual performance. So what the memory-hierarchy here emphasizes for isntance is spatial locality: accessing neighboring data is beneficial because data is transferred through the cache hierarchy
in cache lines.

In this benchmark, the vector stores its integers contiguously. For reference an `int` is 4 bytes, so a 64-byte cache line can contain a total of 16 integers. When one cache line is loaded, it therefore contains several of the integers that the vector traversal will need next. The linked-list traversal, however behaves differently. `sizeof(Node)` is 16 bytes on my machine, but the nodes are followed in a shuffled order! Thus, the next node in the traversal is generally not the next node in memory. This provides much less useful spatial locality than the contiguous vector.

(In each 16 byte `Node`, only the 4 byte integer `v` is the value being summed and the rest is used for the pointer and padding)


- the hardware prefetcher: why it helps the vector and cannot help the chase


The CPU prefetches sequential access and that random access can defeat the prefetcher and cache.
This directly applies to the vector benchmark. Because `sum_vector` accesses consecutive integers, the CPU sees a predictable sequential access pattern and can prefetch upcoming data.
For `sum_list`, the access pattern is determined by the `next` pointer of the current node. Since the nodes are traversed in shuffled order, the next memory address is much less predictable, making hardware prefetching less effective.


- the dependent-load chain: each `p->next` must complete before the next hop
  can even start, so misses serialise instead of overlapping

For `sum_list` each step depends on the previous step. So the program first has to read the current node(s) `next` pointer before it knows wher the next node is located. Consequently, this would mean that if there is a chache miss, the CPU may has to wait for that memory acces to finish before it can continue to the next node. Due to those dependencies, those memory accesses cannot simply overlap and delays can add up.

- where this shows up in our course: it is the reason the order book (HW 7) is
  a flat array and not a `map` of nodes

Based on the results of this benchmark, a flat array provides contiguous and predictable memory access, while a node-based structure can involve pointer chasing and more cache misses. Overall, this makes the flat array more cache friendly, which is ultimately important to reduce latency in HFT. (As said above - this will appear in HW7)

Also say something about the **copy** result: is the by-value overhead roughly
what you would predict from `sizeof(Big)` and your machine's memory bandwidth,
or not? Order-of-magnitude reasoning is enough.

In our results the by-value benchmark also shows the cost of copying Big. For my machine, `sizeof(Big)` is 584 bytes. The p50 was 67.850 ns for pass by value and 46.600 ns for pass by const reference, so the additional time was about 21.25 ns. Taking a rough calculation we get 584 vytes / 21.25 ns which would be about 27.5 GB/s. I'd say that thi seems reasonable in terms of order magnitude, howver, dont interprate it as my RAM bandwith, as the data may already be in the CPU cache and the measurements also includes function call and argument passing effects. In sum, we still do see that copyin a large struct does come with measurable costs, while passsing it by const references avoids making that full copy.
