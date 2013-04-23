
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>

typedef struct error_s {
  char* msg;
  struct error_s* prev;
} error_t;

static error_t* errors = NULL;

void ol_push_errorv(const char* fmt, va_list ap) {
  int size = vsprintf(NULL, fmt, ap) + 1;
  char* msg = (char*) malloc(size);  
  error_t* prev = errors;

  vsprintf(msg, fmt, ap);
  errors = malloc(sizeof(error_t));
  errors->prev = prev;
  errors->msg = msg;
}

void ol_push_error(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  ol_push_errorv(fmt, ap);
  va_end(ap);
}

// pop the most recent error (you own the memory for this message
// now). NULL if none.
char* ol_pop_error() {
  error_t* er = errors;
  const char* msg = NULL;
  
  if(!er)
    return NULL;
  else {
    errors = errors->prev;
    msg = er->msg;
    free(er);
    return (char*) msg;
  }  
}

// get the last error message, NULL if none.
const char* ol_last_error() {
  if(errors)
    return errors->msg;
  else
    return NULL;
}

// print a trace of all the errors (most recent to least recent) if
// any.
void ol_print_error_trace(FILE* out) {
  error_t* er = errors;
  int num = 0;
  while(er) {
    fprintf(out, "%s%s\n", (num > 0 ? "  " : ""), er->msg);
    num++;
    er = er->prev;
  }
}
