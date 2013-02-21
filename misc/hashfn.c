// $Id: hashfn.c,v 1.1 2012-11-16 18:04:00-08 - - $

//
// This program is not part of your project.  It exists just to 
// illustrate how to obtain and print hash values.  Each element
// of argv is hashed and printed along with its hashcode.
//

#include <stdio.h>
#include <stdlib.h>

#include "../code/strhash.h"

int main (int argc, char **argv) {
   for (int argi = 0; argi < argc; ++argi) {
      char *str = argv[argi];
      hashcode_t hashcode = strhash (str);
      printf ("%10u = strhash (\"%s\")\n", hashcode, str);
   }
   printf ("%10u = 0xFFFFFFFFu\n", 0xFFFFFFFFu);
   return EXIT_SUCCESS;
}

