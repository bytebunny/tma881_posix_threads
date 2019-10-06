[//]: # (To preview markdown file in Emacs type C-c C-c p)

# [Assignment 2: Threads](https://www.raum-brothers.eu/martin/Chalmers_TMA881_1920/assignments.html#threads)
We will use Newton's method to practice programming with POSIX threads.

## Relevant concepts

- **Memory fragmentation**. In Assignment 0 we learned that it is advantageous
  to allocate memory in contiguous blocks. Since the **Parallel Computing**
  lecture proposed a layout for solving a similar problem where writing is
  done row by row, we will need to allocate memory for one row at a time.
  
- **Writing to file**. Assignment 0 allowed to get insight into difference
  in performance of varous ways of writing into file. It became evident that
  using `fwrite()` rather than `fprintf()` can save a lot of time.
  
- **Inlining**. In Assignment 1 we learned that providing additional
  information to the compiler about external (i.e. defined in separate files)
  functions can play a major role. Using `inline` keyword and/or
  **link-time optimizer** can facilitate optimization of the code by the compiler.

- **Elementary functions**. Complicated mathematical operations should be
  avoided when possible. For example, raising a complex number to some power
  and comparing the square root of that to some tolerance can be replaced
  with multiplying out the components explicitly and comparing it to the
  square of the tolerance, which is a known number.

- **Control locking**. If possible the `if-else` statements should be
  avoided. For example, one does not need to check the value of `d` (exponent)
  at each pixel by using **indirect function call** instead. This could
  potentially save us time. However, doing so will exclude the possibility
  of **inlining** the Newton routines.
  
  The comparison of the two approaches showed that both of them result in
  virtually same runtimes.

- **Data locality**. Perform as many operations with data loaded from memory
  as possible to avoid reloading it (e.g. in multiple loops). Having in mind
  the memory access pattern, the matrices should be traversed row by row, instead
  of column by column. Unfusing the loop can make prefetching faster.
  
  Our testing showed that unfusing the loop was not as fast as usage of
  `memcpy()` function when assigning colours to **attractor** (colourful image)
  and **convergence** (greyscale image) from arrays with precomputed colours.
  Also, further subdivision of computations into **blocks** did not enhance
  the perforamnce of our program.


## Intended program layout

Per instructions the program naturally splits into two subtasks: The
computation of the Newton iteration and the writing of results to the two
output files. Each of them will be implemented in a separate function, that are
intended to be run as the main function of corresponding POSIX threads.

The computation of the Newton iteration can be further split up into
***INSERT***

As for the writing to file, we have identified ***INSERT*** independent
subtasks. ***INSERT***

***CONTINUE BY FURTHER SPLITTING UP THE TWO TASKS***

# Questions

1. > Newton iteration does not converge for all points. To accomodate this, abort iteration if x_i is closer than 10^-3 to the origin, or if the absolute value of its real or imaginary part is bigger than 10^10. **In the pictures treat these cases as if there was an additional zero of f(x) to which these iterations converge.**???
1. > One might be tempted to implement the writing as
   `fprintf(attr_file, "%d %d %d ", COLOR_VALUE0, COLOR_VALUE1, COLOR_VALUE2);`
   `fprintf(conv_file, "%d %d %d ", GRAY_VALUE, GRAY_VALUE, GRAY_VALUE);`
   but recall that fprintf is slow; fwrite is a better solution.
   
   How to use `fwrite` to write ASCII?
