//Graham Greving
//ggreving
//Joe Rowley
//jrowley

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "debugf.h"
#include "hashset.h"
#include "yyextern.h"

#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME "/usr/share/dict/words"
//#define DEFAULT_DICTNAME "./words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2

#define MAX_WORD_LENGTH 4096

void print_error (char *object, char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", Exec_Name, object, message);
   fflush (NULL);
   Exit_Status = 2;
}

FILE *open_infile (char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL) print_error (filename, strerror (errno));
   DEBUGF ('m', "filename = \"%s\", file = 0x%p\n", filename, file);
   return file;
}

void spellcheck (char *filename, hashset_ref hashset) {
   yylineno = 1;
   DEBUGF ('m', "filename = \"%s\", hashset = 0x%p\n",
                filename, hashset);
   for (;;) {
      int token = yylex ();
      if (token == 0) break;
      DEBUGF ('m', "line %d, yytext = \"%s\"\n", yylineno, yytext);
      if (has_hashset(hashset, yytext)) continue;
      else {
      printf("%s: %d: %s\n", filename, yylineno, yytext);
      if (Exit_Status == 0) Exit_Status++;
      }
   }
}

void load_dictionary (char *dictionary_name, hashset_ref hashset) {
   if (dictionary_name == NULL) return;
   DEBUGF ('m', "dictionary_name = \"%s\", hashset = %p\n",
           dictionary_name, hashset);
   //STUBPRINTF ("Open dictionary, load it, close it\n");
   char buffer[1024];
   FILE *dict = open_infile(dictionary_name);
   if (dict == NULL) {
   print_error(dictionary_name, "No such file or directory");
   return;
   }
   assert(dict != NULL);
   //words 
   int j = 0;
   for (int linenr = 1; ; ++linenr) {
      char *linepos = fgets (buffer, sizeof buffer, dict);
      if (linepos == NULL) break;
      linepos = strchr (buffer, '\n');
      if (linepos == NULL) {
         fflush (NULL);
         fprintf (stderr, "%s: %s[%d]: unterminated line\n",
                  Exec_Name, dictionary_name, linenr);
         fflush (NULL);
         Exit_Status = 2;
      }else {
         *linepos = '\0';
      }
      linepos = strdup (buffer);
      assert (linepos != NULL);
      //insert_queue (queue, linepos);
      put_hashset(hashset, linepos);
      free(linepos);
      //words added
      j++;
   }
   fclose(dict);
   //printf("Dictionary Loaded.\n");
   //printf("Words Added: %d\n", j);
}

int main (int argc, char **argv) {
   Exec_Name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
   hashset_ref hashset = new_hashset ();
   yy_flex_debug = false;
   int debug_dump = 0;

   // Scan the arguments and set flags.
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': user_dictionary = optarg;
                   break;
         case 'n': default_dictionary = NULL;
                   break;
         case 'x': debug_dump++;
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debugflags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }
   
   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);
   if (debug_dump > 1) {
   check_hashset(hashset, debug_dump);
   yycleanup ();
   free_hashset(hashset);
   return Exit_Status;
   }
   //if (has_hashset(hashset, "bitchface")) printf("a word\n");
   //else printf("not a word\n");
   
   // Read and do spell checking on each of the files.
   if (debug_dump == 1) check_hashset(hashset, debug_dump);
   if (optind >= argc) {
      yyin = stdin;
      spellcheck (STDIN_NAME, hashset);
   } else {
      int fileix = optind;
      for (; fileix < argc; ++fileix) {
         DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
         char *filename = argv[fileix];
         if (strcmp (filename, STDIN_NAME) == 0) {
            yyin = stdin;
            spellcheck (STDIN_NAME, hashset);
         }else {
            yyin = open_infile (filename);
            if (yyin == NULL) continue;
            spellcheck (filename, hashset);
            fclose (yyin);
         }
      }
   }
   
   free_hashset(hashset);
   yycleanup ();
   return Exit_Status;
}

