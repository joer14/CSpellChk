//Graham Greving
//ggreving
//Joe Rowley
//jrowley

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#include "strhash.h"

hashcode_t strhash (char *string) {
   assert (string != NULL);
   hashcode_t hashcode = 0;
   for (int index = 0; string[index] != '\0'; ++index) {
      hashcode = hashcode * 31 + (unsigned char) string[index];
   }
   return hashcode;
}

