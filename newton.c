#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  long int n_threads, pic_size, exponent;
  char *ptr, *ptr2;
  
  for ( size_t ix = 1; ix < argc - 1; ++ix ){ // starts with 1 because 0 is the program name.
    ptr = strchr( argv[ix], 't');
    if (ptr) n_threads = strtol(++ptr, &ptr2, 10);
    else{
      ptr = strchr( argv[ix], 'l');
      pic_size = strtol(++ptr, &ptr2, 10);
    }
  }
  exponent = strtol(argv[argc - 1], &ptr2, 10);
  printf("t is %ld, l is %ld, d is %ld\n", n_threads, pic_size, exponent);

  return(0);
}
