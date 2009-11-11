#include "math_implementation.h"



/*********************************************************************************************
**********************************************************************************************

   VARIOUS MATHEMATICAL FUNCTION

**********************************************************************************************
*********************************************************************************************/



/* Must compute also 0! and 1! */ 
FACT_T fact_aux(unsigned long k, FACT_T l){ 
  return ((k==0) ? l : fact_aux(k-1, l*k)); 
}  

FACT_T fact(long k){ 
#ifdef _TRACE_FACT_
  FACT_T res;
  FILE *f;
#endif /* #ifdef _TRACE_FACT_ */

#ifdef _DEBUG_
  if (k<0){
    FMB_error("ERROR: in fact(k), k<0.\n");
  }
#endif /* #ifdef _DEBUG_ */

#ifdef _TRACE_FACT_
  res = fact_aux(k,1);
  if (k>10){
    f=fopen("trace_fact.txt", "a");
    fprintf(f, "Result of fact(%ld): %f\n", k, res);
    fclose(f);
  }
  return res;
#else /* #ifdef _TRACE_FACT_ */
  /* Naive:  */
  /*    return ((k==0) ? 1 : k*fact(k-1)); */ 

  /* Without recursive stack: */
  return fact_aux(k,1);
#endif /* #ifdef _TRACE_FACT_ */
} 





/* unsigned long binomial_coef(unsigned long k,unsigned long n){ */
  
/*   if (k==0) */
/*     return 1; */
/*   if (k==n) */
/*     return 1; */

/*   if (k>(n/2)) */
/*     return binomial_coef(n-k, n); */
/*   else { */
/*     unsigned long n_aux = n-k+1; */
/*     unsigned long k_aux = 2; */
/*     unsigned long res = 1; */

/* #ifdef _TRACE_BINOMIAL_COEF_ */
/*     FILE *f; */
/* #endif /\* #ifdef _TRACE_BINOMIAL_COEF_ *\/ */
      
/*     /\* Remark: we can not afford that (n/2 +1)(n/2 +2)..(n-1)n be greater than the maximum */
/*        value that can be stored in an unsigned long : according to my testings we should be able */
/*        to use value of n up to 62 or 63. *\/ */
    
/*     while ((n_aux<=n) && (k_aux<=k)){ */
/*       res *= n_aux++; */
/*       if (!(res % k_aux)) */
/* 	/\* We can divide since "res" is, in the end, an "exact" positive integer. *\/ */
/* 	res /= k_aux++; */
/*     } */
    
/*     while (n_aux<=n) */
/*       res *= n_aux++; */
/*     while (k_aux<=k) */
/*       res /= k_aux++; */

/* #ifdef _TRACE_BINOMIAL_COEF_ */
/*     f = fopen("coef_bin.txt","a"); */
/*     fprintf(f,"C_{n=%li}^{k=%li} = %li\n", n, k, res); */
/*     fclose(f); */
/* #endif /\* #ifdef _TRACE_BINOMIAL_COEF_ *\/ */
    
/*     return res; */
/*   } */
/* }  */




  
BINOMIAL_COEF_T binomial_coef(long k, long n){ 

#ifdef _DEBUG_
  if (k<0 || n<0){
    FMB_error("ERROR: in binomial_coef(k,n), k<0 or n<0.\n");
  }
#endif /* #ifdef _DEBUG_ */
  
  if (k==0) 
    return (BINOMIAL_COEF_T) 1; 
  if (k==1)
    return (BINOMIAL_COEF_T) n;
  
  /* Not very useful (see the recursive call after): */
  /*   if (k==n)  */
  /*     return (BINOMIAL_COEF_T) 1;  */
  
  if (k>(n/2)) 
    return binomial_coef(n-k, n); 
  else { 
    BINOMIAL_COEF_T n_aux = n-k+1; 
    BINOMIAL_COEF_T k_aux = 1;
    /* We set k to 1 (and not 2) so that we have the same number of terms in 
       the "while ((n_aux<=n) && (k_aux<=k))" loop. */
    BINOMIAL_COEF_T res = 1; 
    
#ifdef _TRACE_BINOMIAL_COEF_ 
    FILE *f; 
#endif /* #ifdef _TRACE_BINOMIAL_COEF_ */ 
    
    /* It might be better to store the results in a static array ... */

    while ((n_aux<=n) && (k_aux<=k)) 
      res *= (n_aux++)/(k_aux++); 
    
#ifdef _TRACE_BINOMIAL_COEF_ 
    f = fopen("coef_bin.txt","a"); 
    fprintf(f,"C_{n=%li}^{k=%li} = %li\n", n, k, res); 
    fclose(f); 
#endif /* #ifdef _TRACE_BINOMIAL_COEF_ */ 
    
    return res; 
  }
}  













/*********************************************************************************************
**********************************************************************************************

   MATHEMATICAL IMPLEMENTATION OF COMPLEX NUMBERS AND FUNCTIONS

**********************************************************************************************
*********************************************************************************************/



void complex_Pow(complex_t *p, unsigned long k){
  
  if (k==0) {
    complex_Convert_real2complex(p, (REAL_T)1);
  }
  else if (k != 1){
    /* We do nothing for k==1. */
    
    if (k % 2){
      /* k is odd. */
      complex_t tmp;
      complex_Affect(&tmp, p);
      
      complex_Pow(&tmp, k / 2);
      complex_Mult_safe(&tmp, &tmp);
      
      complex_Mult(p, &tmp);
    }
    else {
      /* k is even. */
      complex_Pow(p, k/2);
      complex_Mult_safe(p, p);
    }
  }
}















