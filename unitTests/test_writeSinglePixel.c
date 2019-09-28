#include <stdio.h>

int main() {
  //say we've converged to a root which we denote with RGB color (255,139,89)
  //based on this, we assign the attractor colors (could be hardcoded in vectors for all roots)
  int c[] = {255, 139, 89};

  //create a char array for the attractor colors (max 12 chars, e.g. 255 255 255, 9 chars from integers + 3 spaces)
  //could also think of adding an \n when the row is complete
  char output[13];
  //convert integers to strings
  sprintf(output, "%.3d %.3d %.3d \n", c[0], c[1], c[2]);
  //write to the file
  FILE *pfile;
  pfile = fopen("file.ppm","wb");
  fprintf(pfile, "P3\n");
  fprintf(pfile, "1 1\n");
  fprintf(pfile, "255\n");
  fwrite(output, sizeof(char), sizeof(output), pfile);
  fclose(pfile);

  return 0;
}
