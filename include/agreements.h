#ifndef __AGREEMENTS_H__
#define __AGREEMENTS_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



/*** For "open/write/close": ***/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/*** For getopt(): ***/
#include <unistd.h>
#ifdef __LINUX__
#include <getopt.h>
#endif /* __LINUX__ */
#ifdef __AIX__
/* Note: xlc and gcc start searching the header files 
 * in the directories specified by the '-I' option. */
/* We use the following file since 'getopt_long' is not implemented on AIX. */
#include "gnugetopt.h" 
#endif 
extern char * optarg;
extern int optind, opterr, optopt;






/********* Coding conventions: ********/
/* Since this is the first file to be included (except 'config.h'),
 * I give here some coding conventions used in the rest of the code: 
 * - upper case words are usually reserved to constants or macros 
 *   (defined by the C preprocessor)
 * - 'p_' usually means "pointer to" 
 */
/***** End of coding conventions. *****/


#ifdef __AIX__
#undef FALSE
#undef TRUE
#endif /* #ifdef __AIX__ */

#include "config.h"


typedef enum bool {FALSE, TRUE} bool;
#define IS_EVEN(x) ( (x) % 2 ? FALSE : TRUE )

#define MIN(x,y) (((x) < (y))? (x) : (y))
#define MAX(x,y) (((x) > (y))? (x) : (y))

#define MEM_INIT(p_start,c,n){\
unsigned long i; for(i=0; i<n ;i++) p_start[i] = c; }



/*** For debugging in 'octree_Upward_pass_internal_cell_fifo()': ***/
#ifdef OCTREE_COMPUTE_MULTIPOLE_EXP_DEBUG
#define OCTREE_COMPUTE_MULTIPOLE_EXP_DEBUG_DISPLAY(i, x){ if (INFO_DISPLAY(i)){ x } } 
#else 
#define OCTREE_COMPUTE_MULTIPOLE_EXP_DEBUG_DISPLAY(i, x)
#endif 

/*** For debugging in 'octree_Compute_local_exp_L2L_from_father_fifo()': ***/
#ifdef OCTREE_COMPUTE_LOCAL_EXP_L2L_FROM_FATHER_FIFO_DEBUG 
#define OCTREE_COMPUTE_LOCAL_EXP_L2L_FROM_FATHER_FIFO_DEBUG_DISPLAY(i, x){ if (INFO_DISPLAY(i)){ x } } 
#else 
#define OCTREE_COMPUTE_LOCAL_EXP_L2L_FROM_FATHER_FIFO_DEBUG_DISPLAY(i, x)
#endif 

/*** For debugging in 'octree_Direct_computation_aux()': ***/
#ifdef OCTREE_DIRECT_COMPUTATION_AUX_DEBUG 
#define OCTREE_DIRECT_COMPUTATION_AUX_DEBUG_DISPLAY(i, x){ if (INFO_DISPLAY(i)){ x } } 
#else 
#define OCTREE_DIRECT_COMPUTATION_AUX_DEBUG_DISPLAY(i, x)
#endif 


/*** For debugging with multiple MPI processes: ***/
#ifdef SEVERAL_PROCS_DEBUG
#define SEVERAL_PROCS_DEBUG_DISPLAY(i, x){ if (INFO_DISPLAY(i)){ x } }
#else 
#define SEVERAL_PROCS_DEBUG_DISPLAY(i, x) 
#endif 





/* Definition of M_PI and M_PI_2: */
#ifndef __STRICT_ANSI__ 
/* In <math.h> M_PI is defined as: 3.14159265358979323846 
 (M_PI_2 is also defined there). */
#else /* #ifndef __STRICT_ANSI__ */
/* "-ansi" prevents the definition of M_PI in <math.h> */
# define M_PI           3.14159265358979323846  /* pi */
# define M_PI_2         1.57079632679489661923  /* pi/2 */
#endif /* #ifndef __STRICT_ANSI__ */

#define EPSILON (double) 0.00000000000000000001 /* 10^(-20): should be changed if we use "long double" ... */

/* This is for double floating point numbers.
   The "low precision" is for the positions and values of the bodies,
   whereas the high precision is for their force vector and their potential
   (the highest precision allowed by a "double" is about 13 digits). */
#define LOW_PRECISION_DOUBLE_FPRINTF "%.8f"
#define HIGH_PRECISION_DOUBLE_FPRINTF "%.15f"

/* "DOUBLE" is not meaningful here ... it means rather 'real'. */
#ifdef _SINGLE_PREC_
#define NO_PRECISION_DOUBLE_FSCANF "%f"
#define NO_PRECISION_DOUBLE_FSCANF_X2 "%f\t%f\n"
#define NO_PRECISION_DOUBLE_FSCANF_X3 "%f\t%f\t%f\n"
#define NO_PRECISION_DOUBLE_FSCANF_X4 "%f\t%f\t%f\t%f\n"
#define LOW_PRECISION_DOUBLE_FSCANF "%.8f"
#define HIGH_PRECISION_DOUBLE_FSCANF "%.15f"
#define NB_BYTES__FOR__NO_PRECISION_DOUBLE_FSCANF_X3 60 /* same as for double ...  */ 
#define NB_BYTES__FOR__NO_PRECISION_DOUBLE_FSCANF_X4 60 /* same as for double ...  */
#else /* #ifdef _SINGLE_PREC_ */
#define NO_PRECISION_DOUBLE_FSCANF "%lf"
#define NO_PRECISION_DOUBLE_FSCANF_X2 "%lf\t%lf\n"
#define NO_PRECISION_DOUBLE_FSCANF_X3 "%lf\t%lf\t%lf\n"
#define NO_PRECISION_DOUBLE_FSCANF_X4 "%lf\t%lf\t%lf\t%lf\n"
#define NB_BYTES__FOR__NO_PRECISION_DOUBLE_FSCANF_X3 60 /* for safety  */ 
#define NB_BYTES__FOR__NO_PRECISION_DOUBLE_FSCANF_X4 60 /* we can have for example: 
							 * "-3.704740e-01\t-1.311980e-01\t3.433600e-01\t1.000000e-06\n\0"  */


#define LOW_PRECISION_DOUBLE_FSCANF "%.8lf"
#define HIGH_PRECISION_DOUBLE_FSCANF "%.15lf"
#endif /* #ifdef _SINGLE_PREC_ */

typedef enum precision_double_t {low, high} precision_double_t;

#define FMB_CONST const





/* Mathematical functions: */
#include <math.h>
#include <float.h> /* for FLT_MIN and DBL_MIN */

/*** sqrtf slower than sqrt on Power5,
 * and as fast as sqrt on x86, so sqrt is
 * always used (and same for other functions). */
/* #ifdef _SINGLE_PREC_ */
/* #define FMB_COS(x)       cosf(x) */
/* #define FMB_ACOS(x)      acosf(x) */
/* #define FMB_SIN(x)       sinf(x) */
/* #define FMB_ATAN2(x, y)  atan2f(x, y) */
/* #define FMB_FABS(x)      fabsf(x) */
/* #define FMB_FMOD(x, y)   fmodf(x, y) */
/* #define FMB_POW(x, y)    powf(x, y) */
/* #define FMB_LOG(x)       logf(x) */
/* #define FMB_LOG2(x)      log2f(x) */
/* #define FMB_SQRT(x)      sqrtf(x) */
/* #define FMB_FLOOR(x)     floorf(x) */
/* #define FMB_CEIL(x)      ceilf(x) */
/* #else  /\* #ifdef _SINGLE_PREC_ *\/ */
#define FMB_COS(x)       cos(x)
#define FMB_ACOS(x)      acos(x)
#define FMB_SIN(x)       sin(x)
#define FMB_ATAN2(x, y)  atan2(x, y)
#define FMB_FABS(x)      fabs(x)
#define FMB_FMOD(x, y)   fmod(x, y)
#define FMB_POW(x, y)    pow(x, y)
#define FMB_LOG(x)       log(x)
#define FMB_LOG2(x)      log2(x)
#define FMB_SQRT(x)      sqrt(x)
#define FMB_FLOOR(x)     floor(x)
#define FMB_CEIL(x)      ceil(x)
/* #endif /\* #ifdef _SINGLE_PREC_ *\/ */

// static float my_sqrtf(float x){ return sqrtf(x);}




#define FMB_VOLATILE volatile /* Useless in fact for multi-threading... */


/*********************** __LINUX__ **********************/
#ifdef __LINUX__

#define FMB_RESTRICT __restrict__
#define FMB_INLINE static __inline__

#endif /* #ifdef __LINUX__ */





/*** For "htonl/ntohl": ***/
#ifdef __LINUX__ 
/* Depending on the system, we have to include <arpa/inet.h> or <netinet/in.h>. */
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdint.h> /* for 'uint64_t' */
/* From: http://www.codeproject.com/cpp/endianness.asp */
/* #define ntohll(x) (((uint64_t)(ntohl((int)((x << 32) >> 32))) << 32) | (unsigned int)ntohl(((int)(x >> 32)))) */
/* #define htonll(x) ntohll(x) */
/* #define ntohll(x) (x) */
/* #define htonll(x) (x) */



#endif /* __LINUX__ */
#ifdef __AIX__
#include <sys/types.h>
#include <netinet/in.h>
/* 'uint64_t', 'htonll' and 'ntohll' already defined. */
#endif /* __AIX__ */





#endif
