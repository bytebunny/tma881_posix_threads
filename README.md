[//]: # (To preview markdown file in Emacs type C-c C-c p)

# [Assignment 2: Threads](https://www.raum-brothers.eu/martin/Chalmers_TMA881_1920/assignments.html#threads)
We will use Newton's method to practice programming with POSIX threads.


## Timeline for Assignments 2
> Assignments 2 and 3 and include the implementation of one program and the writing of one report. Here is a suggested timeline:

> - [ ] Day 1: List concepts from the lecture and Assignments 0 and 1 that appear useful or related to the assignment. This can yield a sketch of the report.
> - [ ] Day 1 and 2: Discuss the program layout and split it into subtasks. This can contribute to the sketch of the report.
> - [ ] Day 2 and 3: Implement the layout. Do layout ideas work out?
> - [ ] Day 3 to 5: Implement subtasks and revisit layout accordingly.
> - [ ] Day 5: Achieve a functional program. What about performance?
> - [ ] Day 4 and 5: Improve report with everything learned so far. Submit!
> - [ ] Day 6: Read and assess reports from others. Any ideas that you have not yet thought of, but that would help to improve your program?
> - [ ] Day 7 to 10: Identify which parts of your program are slowest and improve them. Repeat until done. It is normal that “polishing” programs takes such a large share of the development time.


# Report

## Relevant concepts

- **Stack allocation**.  We have learned in Assignment 0 that there are two
  types of allocated memory: Stack and heap allocated memory. One acquires
  stack allocated memory by using array declarations inside of functions, as
  opposed to global declarations of arrays.

  Memory allocated on the stack is limited in size, but tends to be faster.
  Moreover, stack allocated memory is thread local and therefore provides an
  opportunity to untangle the mutual impact of parallel threads on one another.
  Consequentially, it is an important consideration to employ stack allocated
  memory in the innermost iteration steps, i.e., the Newton iteration for an
  individual point on the complex plane.

  We plan to test this concept by comparing runtimes of variants of our program
  using stack and heap allocated memory in the innermost iteration steps.

- **Avoiding power function**. Complicated mathematical operations should be
  avoided when possible. For example, raising a complex number to some power
  and comparing the square root of that to some tolerance can be replaced
  with multiplying out the components explicitly and comparing it to the
  square of the tolerance, which is a known number.

***CONTINUE HERE WITH FUTHER CONCEPTS***

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
