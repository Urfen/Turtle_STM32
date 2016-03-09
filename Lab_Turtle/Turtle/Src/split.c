#include "split.h"

/** Code from: http://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c */

/*
Split a given string with every found delim
*/
int split(char *str, const char *delim, char ***array, int *length ) {
  int i=0;
  char *token;
  char **res = (char **) malloc(0 * sizeof(char *));

  /* get the first token */
   token = strtok(str, delim);
   while( token != NULL ) 
   {
        res = (char **) realloc(res, (i + 1) * sizeof(char *));
        res[i] = token;
        i++;
      token = strtok(NULL, delim);
   }
   *array = res;
   *length = i;
  return 1;
}