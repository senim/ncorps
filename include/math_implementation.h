#ifndef __MATH_IMPLEMENTATION_H__
#define __MATH_IMPLEMENTATION_H__


#include "position.h"




/*********************************************************************************************
**********************************************************************************************

   VARIOUS MATHEMATICAL FUNCTION

**********************************************************************************************
*********************************************************************************************/

/*! k can be negative */
FMB_INLINE long pow_of_minus_1(long k){
  /* Other methods: 
     - bit mask on the weak bit (portability problem : Big Endian, Little Endian)
     - static array containing all the values of (-1)^m for m from 0 to ...
  */
  return ((k%2) ? -1 : 1);
}

#define FMB_NEGATE_POW_OF_MINUS_1(v) (v = -v /* v *= -1.0 */)

#define FACT_T REAL_T

FACT_T fact(long k);


#define BINOMIAL_COEF_T REAL_T

/*! binomial_coef(k,n) = n! / k!(n-k)! = (n (n-1) (n-2) ... (n-k+1))/(k!) */
/* unsigned long binomial_coef(unsigned long k,unsigned long n); */
BINOMIAL_COEF_T binomial_coef(long k, long n);


























/*********************************************************************************************
**********************************************************************************************

   MATHEMATICAL IMPLEMENTATION OF COMPLEX NUMBERS AND FUNCTIONS

**********************************************************************************************
*********************************************************************************************/





/*********************************************************************************************

   COMPLEX DEFINED AS X+iY

*********************************************************************************************/


typedef struct {
  REAL_T dat[2];
} complex_t;

#define REAL 0
#define IMAG 1




/******************************* Basic functions: *******************************************/
FMB_INLINE void complex_Affect(complex_t *FMB_RESTRICT p_target, 
			       FMB_CONST complex_t *FMB_RESTRICT p_src){ 
  p_target->dat[REAL] = p_src->dat[REAL]; 
  p_target->dat[IMAG] = p_src->dat[IMAG]; 
}
FMB_INLINE void complex_Convert_real2complex(complex_t *FMB_RESTRICT p_res,
					     REAL_T real){ 
  p_res->dat[REAL] = real; p_res->dat[IMAG] = (REAL_T)0; 
}
FMB_INLINE void complex_Set_real(complex_t *FMB_RESTRICT p, REAL_T v){
  p->dat[REAL] = v; 
}
FMB_INLINE REAL_T complex_Get_real(FMB_CONST complex_t *FMB_RESTRICT p){
  return p->dat[REAL];
}
FMB_INLINE void complex_Set_imaginary(complex_t *FMB_RESTRICT p, REAL_T v){
  p->dat[IMAG] = v; 
}
FMB_INLINE REAL_T complex_Get_imaginary(FMB_CONST complex_t *FMB_RESTRICT p){
  return p->dat[IMAG];
}
FMB_INLINE void complex_Convert_position2complex(complex_t *FMB_RESTRICT p_complex, 
						 FMB_CONST position_t *FMB_RESTRICT p_pos){
  complex_Set_real(p_complex, position_Get_x(p_pos));
  complex_Set_imaginary(p_complex, position_Get_y(p_pos));
}




/******************************* Arithmetic and mathematical functions: *********************/
/*! *p1 = *p1 + *p2 */
/* Because of the FMB_RESTRICT, we can not have: p1==p2. */ 
FMB_INLINE void complex_Plus(complex_t *FMB_RESTRICT p1, 
			     FMB_CONST complex_t *FMB_RESTRICT p2){ 
#ifdef _DEBUG_
  if (p1 == p2){
    FMB_error("ERROR: in complex_Plus(p1, p2), p1 == p2.\n");
  }
#endif /* #ifdef _DEBUG_ */
  p1->dat[REAL] += p2->dat[REAL]; p1->dat[IMAG] += p2->dat[IMAG]; 
}

/*! *p1 = *p1 - *p2 : */
/* Because of the FMB_RESTRICT, we can not have: p1==p2. */ 
FMB_INLINE void complex_Minus(complex_t *FMB_RESTRICT p1, 
			      FMB_CONST complex_t *FMB_RESTRICT p2){
#ifdef _DEBUG_
  if (p1 == p2){
    FMB_error("ERROR: in complex_Minus(p1, p2), p1 == p2.\n");
  }
#endif /* #ifdef _DEBUG_ */
  p1->dat[REAL] -= p2->dat[REAL]; p1->dat[IMAG] -= p2->dat[IMAG]; 
}

/*! *p = (*p) * real */
FMB_INLINE void complex_Mult_by_real(complex_t *FMB_RESTRICT p, REAL_T real){ 
  p->dat[REAL] *= real; p->dat[IMAG] *= real;
}

/*! *p1 = (*p1) * (*p2) \n
   We must have p1 != p2. */
FMB_INLINE void complex_Mult(complex_t *FMB_RESTRICT p1, 
			     FMB_CONST complex_t *FMB_RESTRICT p2){ 
  REAL_T aux = p1->dat[REAL]; 
#ifdef _DEBUG_
  if (p1 == p2){ 
    FMB_error("ERROR: in complex_Mult(p1, p2), p1 == p2!\n"); 
  } 
#endif /* #ifdef _DEBUG_ */
  p1->dat[REAL] = (aux * p2->dat[REAL]) - (p1->dat[IMAG] * p2->dat[IMAG]); 
  p1->dat[IMAG] = (aux * p2->dat[IMAG]) + (p1->dat[IMAG] * p2->dat[REAL]); 
}

/*! *p1 = (*p1) * (*p2) \n
   We can have p1 == p2. */
FMB_INLINE void complex_Mult_safe(complex_t *p1, FMB_CONST complex_t *p2){
  REAL_T aux1 = p1->dat[REAL] ; 
  REAL_T aux2 = p2->dat[REAL] ; 
  p1->dat[REAL] = (aux1 * aux2) - (p1->dat[IMAG] * p2->dat[IMAG]); 
  p1->dat[IMAG] = (aux1 * p2->dat[IMAG]) + (p1->dat[IMAG] * aux2); 
}


/*! *p1= (*p2) * (*p3) \n
  We must have: pi!=pj, for all i and j. */
FMB_INLINE void complex_Mult_with_res(complex_t *FMB_RESTRICT p1, 
				      FMB_CONST complex_t *FMB_RESTRICT p2, 
				      FMB_CONST complex_t *FMB_RESTRICT p3){ 
#ifdef _DEBUG_ 
  if (p1 == p2 || p1 == p3 || p2==p3){ 
    FMB_error("ERROR: in complex_Mult_with_res(p1, p2, p3), pi==pj!\n"); 
  } 
#endif /* #ifdef _DEBUG_ */ 
  p1->dat[REAL] = (p2->dat[REAL] * p3->dat[REAL]) - (p2->dat[IMAG] * p3->dat[IMAG]); 
  p1->dat[IMAG] = (p2->dat[REAL] * p3->dat[IMAG]) + (p2->dat[IMAG] * p3->dat[REAL]); 
}


/*! *p1 = *p1 / *p2 \n
   We must have p1 != p2 */
FMB_INLINE void complex_Div(complex_t *FMB_RESTRICT p1, 
			    FMB_CONST complex_t *FMB_RESTRICT p2){
  REAL_T r2 = (p2->dat[REAL] * p2->dat[REAL]) + (p2->dat[IMAG] * p2->dat[IMAG]); 
  REAL_T aux = p1->dat[REAL]; 
#ifdef _DEBUG_ 
  if (p1 == p2){ 
    FMB_error("ERROR: in complex_Div(p1, p2), p1==p2.\n"); 
  } 
#endif /* #ifdef _DEBUG_ */ 
  p1->dat[REAL] = ((aux * p2->dat[REAL]) + (p1->dat[IMAG] * p2->dat[IMAG])) / r2; 
  p1->dat[IMAG] = ((p1->dat[IMAG] * p2->dat[REAL]) - (aux * p2->dat[IMAG])) / r2; 
}

/*! *p = 1 / *p : */
FMB_INLINE void complex_Inverse(complex_t *FMB_RESTRICT p){
  REAL_T r = (p->dat[REAL] * p->dat[REAL]) + (p->dat[IMAG] * p->dat[IMAG]); 
  p->dat[REAL] = p->dat[REAL] / r; 
  p->dat[IMAG] = p->dat[IMAG] / ((REAL_T)0 - r);
}

/*! With c = r.exp(i.t) (where i^2 = -1), we have: \n
   log(c) = log(r) + i.t, where log(r) is the log with base e of r. \n
   So with cartesian coordinates : \n
   log((x,y) = log(sqrt(x^2 + y^2)) + i.arctan(y/x) \n
   \n
   *p = log(*p) */
FMB_INLINE void complex_Log(complex_t *FMB_RESTRICT p){ 
  REAL_T r = FMB_SQRT((p->dat[REAL] * p->dat[REAL]) + (p->dat[IMAG] * p->dat[IMAG])); 
  p->dat[IMAG] = angle_Compute_from_cartesian(p->dat[REAL], p->dat[IMAG]);
/*   p->dat[IMAG] = ((p->dat[REAL] == 0.0) ?  */
/* 		    1.0 :  */
/* 		    ((p->dat[IMAG] < 0.0) ?  */
/* 		     atan(p->dat[IMAG] / p->dat[REAL]) + M_PI_2 :  */
/* 		     atan(p->dat[IMAG] / p->dat[REAL])));  */
  p->dat[REAL] = FMB_LOG(r); 
} 

/*! *p = (*p)^k   \n
  We use a function for recursivity. \n
  k is a positive (or null) integer. */
void complex_Pow(complex_t *FMB_RESTRICT p, unsigned long k);



/******************************* Conjugate: *************************************************/
FMB_INLINE void complex_Conjugate(complex_t *FMB_RESTRICT p){
  complex_Set_imaginary(p, - complex_Get_imaginary(p));
} 
FMB_INLINE void complex_Conjugate_with_res(FMB_CONST complex_t *FMB_RESTRICT p,
					   complex_t *FMB_RESTRICT p_res){
  complex_Set_real(p_res, complex_Get_real(p));
  complex_Set_imaginary(p_res, - complex_Get_imaginary(p));
}
/*! *p1 = (*p1) * \overline{(*p2)}, where \overline{z} is the conjugate of the complex z \n
   We must have p1 != p2. */
FMB_INLINE void complex_Mult_by_conjugate(complex_t *FMB_RESTRICT p1, 
					  FMB_CONST complex_t *FMB_RESTRICT p2){
  REAL_T aux = complex_Get_real(p1); 
#ifdef _DEBUG_
  if (p1 == p2){ 
    FMB_error("ERROR: in complex_Mult_by_conjugate(p1, p2), p1 == p2!\n"); 
  } 
#endif /* #ifdef _DEBUG_ */
  complex_Set_real(p1, 
		   (aux * complex_Get_real(p2)) + 
		   (complex_Get_imaginary(p1) * complex_Get_imaginary(p2))); 
  complex_Set_imaginary(p1, 
			(complex_Get_imaginary(p1) * complex_Get_real(p2)) - 
			(aux * complex_Get_imaginary(p2))); 
} 




/******************************* Argument & magnitude: **************************************/
/* #define EXPANSION_TERM_GET_ARGUMENT(p) complex_Get_argument(p) */
FMB_INLINE void complex_Set_magnitude_and_argument(complex_t *FMB_RESTRICT p, REAL_T m, REAL_T a){
  /* Nothing to be checked for the values of m and a. */
  REAL_T sin_a = FMB_SIN(a);
  REAL_T cos_a = FMB_SIN(a + M_PI_2); /* VECTORIZATION ???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????*/
  complex_Set_real(p, m * cos_a);
  complex_Set_imaginary(p, m * sin_a);
}




/******************************* Miscellaneous: *********************************************/
FMB_INLINE void complex_Display(FMB_CONST complex_t *FMB_RESTRICT p, FILE *f){
  fprintf(f, "(");
  fprintf(f, HIGH_PRECISION_DOUBLE_FPRINTF, complex_Get_real(p));
  fprintf(f, ", ");
  fprintf(f, HIGH_PRECISION_DOUBLE_FPRINTF, complex_Get_imaginary(p));
  fprintf(f, ")");
}
 















/*********************************************************************************************

   COMPLEX DEFINED AS r.e^{i.theta}

*********************************************************************************************/

/*! r in ]-\infty, +\infty[                                                             \n
    theta in ]-\infty, +\infty[                                                         \n
    If we want to compare (i.e., do they equal ?) two complex_exp_t variables we have   \n
    first to convert them as complex_t.                                                 */

typedef struct{
  REAL_T dat[2];
} complex_exp_t;

#define MAG 0
#define ARG 1


/******************************* Get and Set argument: **************************************/
FMB_INLINE REAL_T complex_exp_Get_argument(FMB_CONST complex_exp_t *FMB_RESTRICT p){
  return p->dat[ARG];
}

/*! The user has nothing to ensure on the value of v. */
FMB_INLINE void complex_exp_Set_argument(complex_exp_t *FMB_RESTRICT p, REAL_T v){
  p->dat[ARG] = v;
}



/******************************* Get and Set magnitude: *************************************/
FMB_INLINE REAL_T complex_exp_Get_magnitude(FMB_CONST complex_exp_t *FMB_RESTRICT p){
  return p->dat[MAG];
}
FMB_INLINE void complex_exp_Set_magnitude(complex_exp_t *FMB_RESTRICT p, REAL_T v){
    p->dat[MAG] = v;
}




/******************************* Get and Set both: ******************************************/
FMB_INLINE void complex_exp_Set_magnitude_and_argument(complex_exp_t *FMB_RESTRICT p,
						       REAL_T mag_value,
						       REAL_T arg_value){
  complex_exp_Set_argument(p, arg_value);
  complex_exp_Set_magnitude(p, mag_value);
}




/*********************** Real & Imaginary parts, Conjugate: *********************************/
FMB_INLINE REAL_T complex_exp_Get_real(FMB_CONST complex_exp_t *FMB_RESTRICT p){
  return complex_exp_Get_magnitude(p) * FMB_COS(complex_exp_Get_argument(p));
}
FMB_INLINE REAL_T complex_exp_Get_imaginary(FMB_CONST complex_exp_t *FMB_RESTRICT p){
  return complex_exp_Get_magnitude(p) * FMB_SIN(complex_exp_Get_argument(p));
}

/*! *p_result = \overline{*p} 
  We cannot have: p==p_result (because of FMB_RESTRICT). */
FMB_INLINE void complex_exp_Conjugate_with_res(FMB_CONST complex_exp_t *FMB_RESTRICT p,
					       complex_exp_t *FMB_RESTRICT p_result){
#ifdef _DEBUG_ 
  if (p == p_result){ 
    FMB_error("ERROR: in complex_exp_Conjugate_with_res(p, p_result), p==p_result.\n"); 
  } 
#endif /* #ifdef _DEBUG_ */   
  complex_exp_Set_magnitude_and_argument(p_result,
					 complex_exp_Get_magnitude(p),
					 - complex_exp_Get_argument(p));
}
/*! *p = \overline{*p} */
FMB_INLINE void complex_exp_Conjugate(complex_exp_t *FMB_RESTRICT p){
  complex_exp_Set_argument(p, - complex_exp_Get_argument(p));
}



/***************************** complex_exp_t <-> complex_t: *********************************/
FMB_INLINE void complex_2complex_exp(FMB_CONST complex_t *FMB_RESTRICT p, 
				     complex_exp_t *FMB_RESTRICT p_exp){
  complex_exp_Set_magnitude_and_argument(p_exp,  
					 FMB_SQRT((complex_Get_real(p) * complex_Get_real(p)) + (complex_Get_imaginary(p) * complex_Get_imaginary(p))),
					 angle_Compute_from_cartesian(complex_Get_real(p), complex_Get_imaginary(p)));
}
FMB_INLINE void complex_exp_2complex(FMB_CONST complex_exp_t *FMB_RESTRICT p_exp, 
				     complex_t *FMB_RESTRICT p){
  complex_Set_real(p, complex_exp_Get_magnitude(p_exp) * FMB_COS(complex_exp_Get_argument(p_exp)));
  complex_Set_imaginary(p, complex_exp_Get_magnitude(p_exp) * FMB_SIN(complex_exp_Get_argument(p_exp)));
}


/*********************************** Arithmetic: ********************************************/
/*! *p1 = *p1 + *p2 */
/* surement moyen de faire (un peu) mieux ... */
/* Because of the FMB_RESTRICT, we can not have: p1==p2. */ 
FMB_INLINE void complex_exp_Plus(complex_exp_t *FMB_RESTRICT p1,
				 FMB_CONST complex_exp_t *FMB_RESTRICT p2){
  complex_t c1, c2;
#ifdef _DEBUG_ 
  if (p1 == p2){ 
    FMB_error("ERROR: in complex_exp_Plus(p1, p2), p1==p2.\n"); 
  } 
#endif /* #ifdef _DEBUG_ */   
  complex_exp_2complex(p1, &c1);
  complex_exp_2complex(p2, &c2);
  complex_Plus(&c1, &c2);
  complex_2complex_exp(&c1, p1);
}

/*! *p = (*p) * real */
FMB_INLINE void complex_exp_Mult_by_real(complex_exp_t *FMB_RESTRICT p, 
					 REAL_T real){
  complex_exp_Set_magnitude(p, real * complex_exp_Get_magnitude(p));
}

/*! *p1 = (*p1) * (*p2) \n
  We can not have p1 == p2 (because of FMB_RESTRICT). */
FMB_INLINE void complex_exp_Mult(complex_exp_t *FMB_RESTRICT p1, 
				 FMB_CONST complex_exp_t *FMB_RESTRICT p2){
#ifdef _DEBUG_ 
  if (p1 == p2){ 
    FMB_error("ERROR: in complex_exp_Mult(p1, p2), p1==p2.\n"); 
  } 
#endif /* #ifdef _DEBUG_ */ 
  complex_exp_Set_magnitude_and_argument(p1, 
					 complex_exp_Get_magnitude(p1) * complex_exp_Get_magnitude(p2),
					 complex_exp_Get_argument(p1) + complex_exp_Get_argument(p2));
}

/*! *p1= (*p2) * (*p3) \n
  We must have pi != pj, for all i and j. */
FMB_INLINE void complex_exp_Mult_with_res(complex_exp_t *FMB_RESTRICT p1, 
					  FMB_CONST complex_exp_t *FMB_RESTRICT p2, 
					  FMB_CONST complex_exp_t *FMB_RESTRICT p3){ 
#ifdef _DEBUG_ 
  if (p1 == p2 || p1 == p3 || p2==p3){ 
    FMB_error("ERROR: in complex_exp_Mult_with_res(p1, p2, p3), pi==pj!\n"); 
  } 
#endif /* #ifdef _DEBUG_ */ 
  complex_exp_Set_magnitude_and_argument(p1, 
					 complex_exp_Get_magnitude(p2) * complex_exp_Get_magnitude(p3),
					 complex_exp_Get_argument(p2) + complex_exp_Get_argument(p3));
}

/*! *p1 = (*p1) * \overline{(*p2)} \n
  We can not have p1 == p2 (because of FMB_RESTRICT). */
FMB_INLINE void complex_exp_Mult_by_conjugate(complex_exp_t *FMB_RESTRICT p1, 
					      FMB_CONST complex_exp_t *FMB_RESTRICT p2){
#ifdef _DEBUG_ 
  if (p1 == p2){ 
    FMB_error("ERROR: in complex_exp_Mult_by_conjugate(p1, p2), p1==p2!\n"); 
  } 
#endif /* #ifdef _DEBUG_ */ 
  complex_exp_Set_magnitude_and_argument(p1, 
					 complex_exp_Get_magnitude(p1) * complex_exp_Get_magnitude(p2),
					 complex_exp_Get_argument(p1) - complex_exp_Get_argument(p2));
}




/*********************************** Miscellaneous: *****************************************/
FMB_INLINE void complex_exp_Affect(complex_exp_t *FMB_RESTRICT p_target, 
				   FMB_CONST complex_exp_t *FMB_RESTRICT p_src){ 
  complex_exp_Set_magnitude_and_argument(p_target, 
					 complex_exp_Get_magnitude(p_src),
					 complex_exp_Get_argument(p_src));
}

FMB_INLINE void complex_exp_Convert_real2complex(complex_exp_t *FMB_RESTRICT p_res, 
						 REAL_T real){ 
  complex_exp_Set_magnitude_and_argument(p_res, real, (REAL_T) 0.0);
}

FMB_INLINE void complex_exp_Display(FMB_CONST complex_exp_t *FMB_RESTRICT p, FILE *f){
  fprintf(f, "(r=");
  fprintf(f, HIGH_PRECISION_DOUBLE_FPRINTF, complex_exp_Get_magnitude(p));
  fprintf(f, ", arg=");
  fprintf(f, HIGH_PRECISION_DOUBLE_FPRINTF, complex_exp_Get_argument(p));
  fprintf(f, ")");
}

/*! *p_result = i^k                       \n
  k is an integer (positive or negative). */
FMB_INLINE void complex_exp_Pow_of_i(complex_exp_t *FMB_RESTRICT p_result, 
				     unsigned long k){
  complex_exp_Set_magnitude(p_result, (REAL_T) 1);
  if (! k%4)
    complex_exp_Set_argument(p_result, (REAL_T) 0);
  else 
    if (! k%2)
      complex_exp_Set_argument(p_result, (((k / 2) % 2) ? (REAL_T) M_PI : (REAL_T) 0 )); 
    else
      complex_exp_Set_argument(p_result, M_PI_2 *k); 
}







#endif
