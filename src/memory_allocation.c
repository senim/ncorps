#include <errno.h>

#include "memory_allocation.h"
#include "parameters.h"
#include "IO.h"


char perror_mes[PERROR_MES_SIZE];



/*********************************************************************************************
   'total'
*********************************************************************************************/
#ifndef _FMB_MEM_TRACE_

/* 'total' contains the "total" sizes allocated (not considering free or realloc!):
 * this is a very rough upper bound ... */  
static long long total = 0.0;

#else 

/* 'total' contains the size of memory currently allocated, and 
 * 'max_total' contains the maximum size of memory allocated during the run of FMB:
 * precise, but can induce overhead in CPU times ... */
static long long total = 0.0;
static long long max_total = 0.0;
long long FMB_max_total_memory_allocated(){
  return total;
}

#endif 

long long FMB_total_memory_allocated(){
  return total;
}
void FMB_Add_to_total_memory_allocated(long long size){
  total += size;
}









#ifdef _FMB_MEM_TRACE_
/*********************************************************************************************
   with _FMB_MEM_TRACE_
*********************************************************************************************/

/* Should be ok, on 32 bit architectures as well as on 64 bit architectures: */
#define FMB_MEM_ALIGN   64 /* (sizeof(void *)) */
/* Remark: "#define FMB_MEM_ALIGN   16" may be ok on Intel (ix86) architectures ... */

typedef struct {
  int size;
} FMB_mem_header_t;

/* Computes the number of multiples of 'FMB_MEM_ALIGN' required to store 'FMB_mem_header_t': */
#define FMB_MEM_HEADER_SIZE  (((sizeof(FMB_mem_header_t))+(FMB_MEM_ALIGN-1)) & ~(FMB_MEM_ALIGN-1))



FMB_INLINE void *my_malloc(size_t size){
  char *ptr = (char *)malloc(FMB_MEM_HEADER_SIZE + size);

  if(ptr) {
    //  fprintf(f_output, " _FMB_MEM_TRACE_ : %i bytes allocated (@ = %p)\n", size, ptr);
    ((FMB_mem_header_t *)ptr)->size = size;
    ptr += FMB_MEM_HEADER_SIZE;
    total += size;
    if (total > max_total){ max_total = total; }
  }

  return ptr;
}

FMB_INLINE void my_free(void *ptr){
  if(ptr) {
    (char *)ptr -= FMB_MEM_HEADER_SIZE;
    
    //    fprintf(f_output, " _FMB_MEM_TRACE_ : free %i bytes (@ = %p)\n", ((FMB_mem_header_t *)ptr)->size, ptr);
    total -=  ((FMB_mem_header_t *)ptr)->size;
    free(ptr);
  }
}


FMB_INLINE void *my_calloc(size_t nmemb, size_t size){
  size_t size_to_allocate = size*nmemb; /* WARNING: may cause problem for too high value of 'size*nmemb' ... */
  char *ptr = (char *)calloc(1, FMB_MEM_HEADER_SIZE + size_to_allocate); 

  if(ptr) {
  //  fprintf(f_output, " _FMB_MEM_TRACE_ : %i bytes allocated (@ = %p)\n", size_to_allocate, ptr);
    ((FMB_mem_header_t *)ptr)->size = size_to_allocate;
    ptr += FMB_MEM_HEADER_SIZE;
    total += size_to_allocate;
    if (total > max_total){ max_total = total; }
  }

  return ptr;
}
FMB_INLINE void *my_realloc(void *ptr, size_t size){
  size_t previous_size;
  char *new_ptr;

  if(ptr) {
    (char *)ptr -= FMB_MEM_HEADER_SIZE;
    previous_size = ((FMB_mem_header_t *)ptr)->size;
  }
  else {
    previous_size = 0;
  }
  
  new_ptr = (char *)realloc(ptr, FMB_MEM_HEADER_SIZE + size); 

  if (new_ptr){
    if (size > previous_size){
      //    fprintf(f_output, " _FMB_MEM_TRACE_ : %i bytes (re)allocated (@ = %p)\n", size - previous_size, ptr);
      total +=  size - previous_size;
      if (total > max_total){ max_total = total; }
    }
    /* I do nothing if (size <= previous_size), but the libc may do something! 
     * This is however implementation dependant ... */

    new_ptr += FMB_MEM_HEADER_SIZE;
  }

  return new_ptr;
}







#else /* #ifdef _FMB_MEM_TRACE_ */

/*********************************************************************************************
   without _FMB_MEM_TRACE_
*********************************************************************************************/



FMB_INLINE void *my_malloc(size_t size){
  void *ptr = (void *)malloc(size);

  if(ptr) { total += size; }
  return ptr;
}

FMB_INLINE void my_free(void *ptr){
  free(ptr);
}


FMB_INLINE void *my_calloc(size_t nmemb, size_t size){
  char *ptr = (char *)calloc(nmemb, size); 
  
  if(ptr) {total += size; }
  return ptr;
}
FMB_INLINE void *my_realloc(void *ptr, size_t size){
  char *new_ptr = (char *)realloc(ptr, size); 

  if (new_ptr){ total += size; }
  return new_ptr;
}


#endif /* #ifdef _FMB_MEM_TRACE_ */









/*********************************************************************************************
   misc. functions
*********************************************************************************************/
void *malloc_with_check(size_t size){
  void *ptr = NULL;

  if (size == 0){ 
    FMB_error("In malloc_with_check(): size == 0\n");
  }

  if ((ptr = my_malloc(size)) == NULL){
    FMB_perror("In malloc_with_check(size=%i) (memory already allocated (WARNING! This value is not thread-safe!): %f Mo)", size, (REAL_T) total / (1024 * 1024));
  }
  SEVERAL_PROCS_DEBUG_DISPLAY(2, fprintf(f_output, "total_mem_allocated = %.1f MB\n", (REAL_T) total / (1024*1024)););

  return ptr;
}


void *calloc_with_check(size_t nmemb, size_t size){
  void *ptr = NULL;

  if ((size == 0) || (nmemb == 0)){ 
    FMB_error("In calloc_with_check(): size == 0 || nmemb == 0 \n");
  }

  if ((ptr = my_calloc(nmemb, size)) == NULL){
    FMB_perror("In calloc_with_check(size=%i) (memory already allocated (WARNING! This value is not thread-safe!): %f Mo)", size, (REAL_T) total / (1024 * 1024));
  }
  //  SEVERAL_PROCS_DEBUG_DISPLAY(2, fprintf(f_output, "total_mem_allocated = %.1f MB\n", (REAL_T) total / (1024*1024)););

  return ptr;
}


void *realloc_with_check(void *ptr, size_t size){
  void *auxiliary_ptr = NULL;

  if (size == 0){ /* we allow NULL value for 'ptr' */ 
    FMB_error("In realloc_with_check(): size == 0 \n");
  }

  if ((auxiliary_ptr = my_realloc(ptr,size)) == NULL){
    FMB_perror("In realloc_with_check() ");
  }
  //  SEVERAL_PROCS_DEBUG_DISPLAY(2, fprintf(f_output, "total_mem_allocated = %.1f MB\n", (REAL_T) total / (1024*1024)););

  return auxiliary_ptr;
}



void *FMB_malloc(size_t size){
  if (size == 0){ 
    FMB_error("In FMB_malloc(): size == 0\n");
  }
  return my_malloc(size);
}

void *FMB_malloc_with_check(size_t size){
  return malloc_with_check(size);
}

void *FMB_calloc_with_check(size_t nmemb, size_t size){
  return calloc_with_check(nmemb, size);
}

void *FMB_realloc_with_check(void *ptr, size_t size){
  return realloc_with_check(ptr, size);
}


void FMB_free(void *ptr){
  my_free(ptr);
}





void FMB_error(const char *error_format, ...){
  va_list  ap;

  fprintf(stderr, "\n\n\t FMB error message: ");

  va_start(ap, error_format);
  vfprintf(stderr, error_format, ap);
  va_end(ap);

  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
}




void FMB_perror(const char *error_format, ...){
  va_list  ap;

  va_start(ap, error_format);
  vsprintf(perror_mes, error_format, ap);
  va_end(ap);

  /* WARNING: 'perror()' is not thread-safe, which implies that DURING a call to 'perror()',
   * 'errno' can be changed by another thread (returning from another system call), and 
   * the "perror message" and the "errno value" displayed may not match!
   * 
   * In order to solve this problem, we use 'strerror()' with a mutex (here with FMB_thread_IO_lock())
   * (strerror_r() is not portable): this guarantees that the "perror message" and the "errno value" 
   * will match.   */ 
  fprintf(stderr,  "\n\n\t FMB error message: %s\n\t \"perror/errno\" message:%s\n", perror_mes, strerror(errno));
  
  exit(EXIT_FAILURE);
}


