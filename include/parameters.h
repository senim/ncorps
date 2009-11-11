#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include "agreements.h"



#ifdef _SINGLE_PREC_

/* Warning: this may be not safe to use different types for COORDINATES_T, VALUES_T and REAL_T... */
#define COORDINATES_T float
#define COORDINATES_T_MIN FLT_MIN
#define VALUES_T float
#define REAL_T float

/* See IO.h: */
#define FMB_IO_FREAD_COORDINATE()  FMB_IO_Fread_float()
#define FMB_IO_FWRITE_COORDINATE(x) FMB_IO_Fwrite_float(x)
#define FMB_IO_FREAD_REAL()  FMB_IO_Fread_float()
#define FMB_IO_FWRITE_REAL(x) FMB_IO_Fwrite_float(x)
#define FMB_IO_FREAD_VALUE()  FMB_IO_Fread_float()
#define FMB_IO_FWRITE_VALUE(x) FMB_IO_Fwrite_float(x)

#else /* #ifdef _SINGLE_PREC_ */

/* Warning: this may be not safe to use different types for COORDINATES_T, VALUES_T and REAL_T... */
#define COORDINATES_T double
#define COORDINATES_T_MIN DBL_MIN
#define VALUES_T double
#define REAL_T double

#define FMB_IO_FREAD_COORDINATE()  FMB_IO_Fread_double()
#define FMB_IO_FWRITE_COORDINATE(x) FMB_IO_Fwrite_double(x)
#define FMB_IO_FREAD_REAL()  FMB_IO_Fread_double()
#define FMB_IO_FWRITE_REAL(x) FMB_IO_Fwrite_double(x)
#define FMB_IO_FREAD_VALUE()  FMB_IO_Fread_double()
#define FMB_IO_FWRITE_VALUE(x) FMB_IO_Fwrite_double(x)

#endif /* #ifdef _SINGLE_PREC_ */



/************ Indexes: ************/
typedef short height_ind_t;

/* Type of the indexes of the expansions: */
typedef int exp_ind_t;


/* Thread indexes: */
typedef int thread_ind_t; /* so that it can be stored in a 'void *' */

/* MPI Processes indexes: */
typedef int proc_ind_t; /* so that it can be stored in a 'void *' */

/* bodies_ind_t: */
typedef long bodies_ind_t; /* Can NOT be unsigned (see bodies_it_t for example). */
#define FORMAT_BODIES_IND_T "%li"
#define FMB_IO_FREAD_BODIES_IND()   FMB_IO_Fread_long()
#define FMB_IO_FWRITE_BODIES_IND(x) FMB_IO_Fwrite_long(x)






/************ FMB_Info: ***********/
typedef struct{

  REAL_T dt;  /* 0.001 */
  REAL_T tend ; /*0.001 */ 

  long nb_bodies; /* for this process only */
  long total_nb_bodies; /* sum of 'FMB_Info.nb_bodies' over all MPI processes */


  REAL_T eps_soft; /* Softening parameter ('epsilon') in gravitational computations. */
  REAL_T eps_soft_square; /* eps_soft_square = eps_soft . eps_soft */ 
  
  bool save; /* If set to TRUE, we save position, mass, force and/or potential of all particles.
	      * Default value: FALSE. */

  bool sum;  /* If set to TRUE, we compute and display the sum of the forces and/or potential over all particles.
	      * Default value: FALSE. */

} FMB_Info_t;

extern FMB_Info_t FMB_Info;




/* Some default values for 'FMB_Info': */
#define DFLT_NB_LESS_S_MIN 0
#define DFLT_H_MIN_BLAS_WITHOUT_RECOPIES 5








/***** Equality between floating point numbers: (for position_Are_too_close()) *****/
/*** 1st method: ***
 * #define GAP_TOLERATED_FOR_POSITIONS 0.0000000001 /\* 10^(-10) *\/ 
 * And then test like: (FMB_FABS(position_Get_x(p1) - position_Get_x(p2)) < GAP_TOLERATED_FOR_POSITIONS)   */
/*** 2nd method: ***
 * WARNING: this is not optimal (see "Comparing floating point numbers", Bruce Dawson
 * (http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm)). */
#define COORDINATES_T_MAX_RELATIVE_ERROR 0.00001 /* default value for 'maxRelativeError' */
FMB_INLINE bool COORDINATES_T__ARE_ALMOST_EQUAL(COORDINATES_T A, COORDINATES_T B, COORDINATES_T maxRelativeError){
  COORDINATES_T relativeError;
  if (FMB_FABS(A - B) < COORDINATES_T_MIN){
    return TRUE;
  }
  if (FMB_FABS(B) > FMB_FABS(A)){
    relativeError = FMB_FABS((A - B) / B);
  }
  else {
    relativeError = FMB_FABS((A - B) / A);
  }
  if (relativeError <= maxRelativeError){
    return TRUE;
  }
  return FALSE;
}


/* Margin added to the computational box in percentage of the original 
   half side length: => NOT USED ANYMORE: we use the lowest power of 2 
   that encloses all particles (see FMB_IO_Header()). */
/* #define COMPUTATIONAL_BOX_MARGIN 0.1  */
/* #define COMPUTATIONAL_BOX_MARGIN 0.0 /\* The body do not move (only 1 time step). *\/ */

/* For the octree's implementation: */
#define FIRST_CELL_INDEX_VS_LEAFS_ARRAY_SIZE_RATIO 2

/* Numbering of the cell's children : */
/* 3D : FRONT-BACK UP-DOWN LEFT-RIGHT */
/* Direct correspondence with Morton ordering: */
typedef enum children_number_t {
  BDL, BUL, BDR, BUR, FDL, FUL, FDR, FUR
} children_number_t;
#define FIRST_CHILDREN_NUMBER BDL
#define LAST_CHILDREN_NUMBER FUR


/* MISCELLANEOUS CONSTANTS: */
#define NB_CHILDREN 8
#define CHILDREN_BITS_NUMBER 3
#define NB_NEAREST_NEIGHBORS 26 /* for uniform distributions only */
#define SIZE_INTERACTION_LIST 189 
/* When we exclude the second nearest neighbors too: */
/* #define NB_NEAREST_AND_SECOND_NEAREST_NEIGHBORS 124 /\* for uniform distributions only *\/ */
/* #define SIZE_INTERACTION_LIST 875 /\* for uniform distributions only *\/ */  
/* #define SIZE_INTERACTION_LIST_WITH_SUPERNODES 198 /\* for uniform distributions only *\/ */







/* Numbering of the nearest neighbors: */
/* 3D : FACE-EDGE-VERTEX FRONT-BACK UP-DOWN LEFT-RIGHT */
/* Rules: F>E>V and F>B>U>D>L>R */
typedef enum neighbor_number_t {
  F_F, F_B, F_U, F_D, F_L, F_R, 
  E_FU, E_FD, E_BU, E_BD, E_FL, E_FR, E_BL, E_BR, E_UL, E_UR, E_DL, E_DR,
  V_FUL, V_FUR, V_FDL, V_FDR, V_BUL, V_BUR, V_BDL, V_BDR
} neighbor_number_t;
#define FIRST_NEIGHBOR_NUMBER F_F
#define LAST_NEIGHBOR_NUMBER V_BDR
/* #define F_F 0 */
/* #define F_B 1 */
/* #define F_U 2 */
/* #define F_D 3 */
/* #define F_L 4 */
/* #define F_R 5 */
/* #define E_FU 6 */
/* #define E_FD 7 */
/* #define E_BU 8 */
/* #define E_BD 9 */
/* #define E_FL 10 */
/* #define E_FR 11 */
/* #define E_BL 12 */
/* #define E_BR 13 */
/* #define E_UL 14 */
/* #define E_UR 15 */
/* #define E_DL 16 */
/* #define E_DR 17 */
/* #define V_FUL 18 */
/* #define V_FUR 19 */
/* #define V_FDL 20 */
/* #define V_FDR 21 */
/* #define V_BUL 22 */
/* #define V_BUR 23 */
/* #define V_BDL 24 */
/* #define V_BDR 25 */
FMB_INLINE bool is_Face_neighbor(neighbor_number_t nn){
  /* return ((nn <= F_R) ? TRUE : FALSE), => useless */
  return nn <= F_R;
}
FMB_INLINE bool is_Vertex_neighbor(neighbor_number_t nn){
  /* return ((nn >= V_FUL) ? TRUE : FALSE), => useless */
  return nn >= V_FUL;
}



/* Numbering of the exceptions for the nearest neighbors indexes: */
#define NOT_SET -1
#define BORDER -2 /* Only for "free-space boundaries conditions". */
#define NO_NEIGHBOR -3 /* Only for the non uniform case. */



/* BOUNDARY CONDITIONS: */
#define FREE_SPACE_BOUNDARY_CONDITIONS 1
#define PERIODIC_BOUNDARY_CONDITIONS 2
#define BOUNDARY_CONDITIONS FREE_SPACE_BOUNDARY_CONDITIONS

/* Direct computation between two bodies: */
#define NO_MUTUAL 0
#define MUTUAL 1


/***** To sort the "results" files: *****/
#ifdef __AIX__
/* The maximum number of fields of the 'sort' command on AIX 5.1 is 10, 
 * which is not enough: the 'sort command' fails for large files ... 
 * Use: ~/bin/gnu_sort.sh */
#define SORT_COMMANDE_FORMAT "sort -o %s -n -k 7,7 %s "
#endif /* __AIX__ */

#ifdef __LINUX__
/* The sorting fails with '-n' on pollux ... */
#define SORT_COMMANDE_FORMAT "sort -o %s -k 7,7 %s "
#endif /* __LINUX__ */ 






#endif 




