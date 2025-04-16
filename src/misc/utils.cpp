#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils.hpp>

/*
Returns home dir appended with /.rlg327

Name arg is the name of the file to save/load

Returns path to file
*/
char *get_save_path(const char *name) {
  char *home = getenv("HOME");
  if (!home) {
    printf("Error: Could not get home directory\n");
    return NULL;
  }

  char *path = (char *)(malloc(strlen(home) + strlen("/.rlg327/") +
                               strlen(name))); // Extra space for "/.rlg327"
  if (!path) {
    printf("Error: Memory allocation failed\n");
    return NULL;
  }

  snprintf(path, 64, "%s/.rlg327/%s", home, name);
  return path;
}
