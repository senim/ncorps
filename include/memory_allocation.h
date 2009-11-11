#ifndef __MEMORY_ALLOCATION_H__
#define __MEMORY_ALLOCATION_H__

#include <stdlib.h>
#include <stdio.h>

#include <stdarg.h> /* for va_list */

long long FMB_total_memory_allocated();
void FMB_Add_to_total_memory_allocated(long long size);
#ifdef _FMB_MEM_TRACE_
long long FMB_max_total_memory_allocated();
#endif 


void *FMB_malloc(size_t size);
void *FMB_malloc_with_check(size_t size);
void *FMB_calloc_with_check(size_t nmemb, size_t size);
void *FMB_realloc_with_check(void *ptr, size_t size);
void FMB_free(void *ptr);
 
void FMB_Display_mem_allocated(FILE *f);



#define PERROR_MES_SIZE 300
extern char perror_mes[PERROR_MES_SIZE];

/* We also define here our own "error function": 
 * it can be used as 'printf' (variable number of arguments). */
void FMB_error(const char *error_format, ...);
/* Same as 'FMB_error' but also calls 'perror()': */
void FMB_perror(const char *error_format, ...);

/* And a useful macro: */
#define FMB_ERROR_BRIEF() {						\
  char message[PERROR_MES_SIZE];					\
  sprintf(message, "File: %s, line: %d\n", __FILE__ , __LINE__ );	\
  FMB_error(message);							\
}									\



#endif




