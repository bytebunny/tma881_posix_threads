## Intended program layout

The problem consists of three main functions: 
the main function, the compute function, and the write function.

### main function
The main function has four main subtasks.

+ Receiving the total threads number, the pixels number, and the exponent number through the arguments.
+ Initializing the attractor, convergence, and item_done arrays.
+ Declaring and initializing mutex threads, and creating p_threads of compute fucntion and write function.
+ Joining p_threads and destoring mutex threads.

The subtasks are shown in the figure.
![flow chart](./flowchart.jpg) 

### newton iteration
For each of exponent number, there is a newton function to find the roots.
 The newton function receives real number (pixel coordinate y), image number (pixel coordinate x),
  and return attractor and convergence back. 
The analytical roots of the polynominal fuction are calculated before and hard coeded in to the function.

The newton iteration is then applied in a while loop.
while
&nbsp;&nbsp;      if the pixel is colse less than 0.001 to the origin
&nbsp;&nbsp;	&nbsp;&nbsp;	set attractor = 9, convergence to cap size
&nbsp;&nbsp;	end if
&nbsp;&nbsp;	break

&nbsp;&nbsp;	initialize solution to 0
&nbsp;&nbsp;	compute the increment of $-\frac{P(Z)}{P\prime(Z)}$
&nbsp;&nbsp;	update the solution 
&nbsp;&nbsp;	update the iteration number

&nbsp;&nbsp;	if the updated solution is converge to the true roots
&nbsp;&nbsp;	&nbsp;&nbsp;	set attractor to the number of the true roots
&nbsp;&nbsp;	&nbsp;&nbsp;	set convergence number as current iteration number
&nbsp;&nbsp;	&nbsp;&nbsp;	break
&nbsp;&nbsp;	end if

&nbsp;&nbsp;    continue ckecking with the next ture roots
end while


### compute function

### write function
