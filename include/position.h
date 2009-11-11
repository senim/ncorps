#ifndef __POSITION_H__
#define __POSITION_H__

#include "parameters.h"



/*********************************************************************************************
**********************************************************************************************

   POSITION_T

**********************************************************************************************
*********************************************************************************************/

/* The position fields are stored as an array 
 * so that they are contiguous in memory: 
 * this MIGHT be necessary for the direct computation with BLAS, 
 * but this MIGHT be slower for the direct computation without BLAS!
 * We therefore let the two possibilities ...  
 *
 * After testing: the position fields stored as arrays are not slower 
 * for the direct computation without BLAS! */

#ifdef _POSITION_T_STORED_WITH_ARRAY_ 

#define POS_X 0
#define POS_Y 1
#define POS_Z 2
typedef struct {
  COORDINATES_T dat[3];
} position_t;

#else /* #ifdef _POSITION_T_STORED_WITH_ARRAY_ */

typedef struct {
  COORDINATES_T x;
  COORDINATES_T y;
  COORDINATES_T z;
} position_t;

#endif /* #ifdef _POSITION_T_STORED_WITH_ARRAY_ */








/*! For the spherical coordinates we use the "physical" notation \n
  for theta and phi. That is to say that phi is the longitudinal
  coordinate whereas theta is the colatitudinal coordinate
  (the colatitudinal coordinate "starts" from the z-axis 
  whereas the latitudinal coordinate "starts" from the x-y plan 
  (i.e. the equator for the earth). */
typedef struct { /* (r, theta, phi) */
  COORDINATES_T r;
  COORDINATES_T th; /* for "theta": theta is in the range [0, Pi] */
  COORDINATES_T ph; /* for "phi": phi is in the range ]-Pi, Pi] */
} spherical_position_t;







#ifdef _POSITION_T_STORED_WITH_ARRAY_
FMB_INLINE void position_Set_x(position_t *FMB_RESTRICT p, COORDINATES_T v){  
  p->dat[POS_X] = v;  
} 
FMB_INLINE COORDINATES_T position_Get_x(FMB_CONST position_t *FMB_RESTRICT p){ 
  return p->dat[POS_X]; 
} 
FMB_INLINE COORDINATES_T *position_Get_p_x(position_t *FMB_RESTRICT p){ 
  return p->dat + POS_X; 
} 

FMB_INLINE void position_Set_y(position_t *FMB_RESTRICT p, COORDINATES_T v){  
  p->dat[POS_Y] = v;  
}  
FMB_INLINE COORDINATES_T position_Get_y(FMB_CONST position_t *FMB_RESTRICT p){ 
  return p->dat[POS_Y]; 
} 
FMB_INLINE COORDINATES_T *position_Get_p_y(position_t *FMB_RESTRICT p){ 
  return p->dat + POS_Y; 
} 
#else /* #ifdef _POSITION_T_STORED_WITH_ARRAY_ */
FMB_INLINE void position_Set_x(position_t *FMB_RESTRICT p, COORDINATES_T v){  
  p->x = v;  
} 
FMB_INLINE COORDINATES_T position_Get_x(FMB_CONST position_t *FMB_RESTRICT p){ 
  return p->x; 
} 
FMB_INLINE COORDINATES_T *position_Get_p_x(position_t *FMB_RESTRICT p){ 
  return &(p->x); 
} 

FMB_INLINE void position_Set_y(position_t *FMB_RESTRICT p, COORDINATES_T v){  
  p->y = v;  
}  
FMB_INLINE COORDINATES_T position_Get_y(FMB_CONST position_t *FMB_RESTRICT p){ 
  return p->y; 
} 
FMB_INLINE COORDINATES_T *position_Get_p_y(position_t *FMB_RESTRICT p){ 
  return &(p->y); 
} 
#endif /* #ifdef _POSITION_T_STORED_WITH_ARRAY_ */


/*********************************************************************************************
   3D
*********************************************************************************************/

#ifdef _POSITION_T_STORED_WITH_ARRAY_
/* For force_Direct_computation_[with_potential_]mutual() in 3D: */
#define POSITION_GET_X(p) ((p)->dat[POS_X]) 
#define POSITION_GET_Y(p) ((p)->dat[POS_Y]) 
#define POSITION_GET_Z(p) ((p)->dat[POS_Z]) 
FMB_INLINE void position_Set_z(position_t *FMB_RESTRICT p, COORDINATES_T v){  
  p->dat[POS_Z] = v;  
} 
FMB_INLINE COORDINATES_T position_Get_z(FMB_CONST position_t *FMB_RESTRICT p){ 
  return p->dat[POS_Z]; 
} 
FMB_INLINE COORDINATES_T *position_Get_p_z(position_t *FMB_RESTRICT p){ 
  return p->dat + POS_Z; 
} 
#else /* #ifdef _POSITION_T_STORED_WITH_ARRAY_ */
/* For force_Direct_computation_[with_potential_]mutual() in 3D: */
#define POSITION_GET_X(p) ((p)->x) 
#define POSITION_GET_Y(p) ((p)->y) 
#define POSITION_GET_Z(p) ((p)->z) 
FMB_INLINE void position_Set_z(position_t *FMB_RESTRICT p, COORDINATES_T v){  
  p->z = v;  
} 
FMB_INLINE COORDINATES_T position_Get_z(FMB_CONST position_t *FMB_RESTRICT p){ 
  return p->z; 
} 
FMB_INLINE COORDINATES_T *position_Get_p_z(position_t *FMB_RESTRICT p){ 
  return &(p->z); 
} 
#endif /* #ifdef _POSITION_T_STORED_WITH_ARRAY_ */


FMB_INLINE void position_Initialize(position_t *FMB_RESTRICT p){ 
  position_Set_x(p, (COORDINATES_T)0); 
  position_Set_y(p, (COORDINATES_T)0); 
  position_Set_z(p, (COORDINATES_T)0); 
} 

FMB_INLINE void position_Affect(position_t *FMB_RESTRICT p_to_affect, 
				FMB_CONST position_t *FMB_RESTRICT p_new_values){ 
  position_Set_x(p_to_affect, position_Get_x(p_new_values)); 
  position_Set_y(p_to_affect, position_Get_y(p_new_values));  
  position_Set_z(p_to_affect, position_Get_z(p_new_values)); 
} 

FMB_INLINE char position_Are_too_close(FMB_CONST position_t *FMB_RESTRICT p1, 
				       FMB_CONST position_t *FMB_RESTRICT p2){ 
  return ((COORDINATES_T__ARE_ALMOST_EQUAL(position_Get_x(p1), position_Get_x(p2), COORDINATES_T_MAX_RELATIVE_ERROR)  
 	   && COORDINATES_T__ARE_ALMOST_EQUAL(position_Get_y(p1), position_Get_y(p2), COORDINATES_T_MAX_RELATIVE_ERROR) 
 	   && COORDINATES_T__ARE_ALMOST_EQUAL(position_Get_z(p1), position_Get_z(p2), COORDINATES_T_MAX_RELATIVE_ERROR)) ?  
 	  TRUE :  
 	  FALSE); 
} 

FMB_INLINE void position_Negate(position_t *FMB_RESTRICT p){ 
  position_Set_x(p, - position_Get_x(p)); 
  position_Set_y(p, - position_Get_y(p)); 
  position_Set_z(p, - position_Get_z(p)); 
} 

/* Because of the FMB_RESTRICT, we can not have: p_target==p_src. */ 
FMB_INLINE void position_Add(position_t *FMB_RESTRICT p_target, 
			     FMB_CONST position_t *FMB_RESTRICT p_src){ 
#ifdef _DEBUG_
  if (p_target == p_src){
    FMB_error("ERROR: in position_Add(p_target, p_src), p_target == p_src.\n");
  }
#endif /* #ifdef _DEBUG_ */
  position_Set_x(p_target, position_Get_x(p_target) + position_Get_x(p_src)); 
  position_Set_y(p_target, position_Get_y(p_target) + position_Get_y(p_src));  
  position_Set_z(p_target, position_Get_z(p_target) + position_Get_z(p_src)); 
} 

/* Because of the FMB_RESTRICT, we can not have: p_target==p_src. */ 
FMB_INLINE void position_Substract(position_t *FMB_RESTRICT p_target, 
				   FMB_CONST position_t *FMB_RESTRICT p_src){ 
#ifdef _DEBUG_
  if (p_target == p_src){
    FMB_error("ERROR: in position_Substract(p_target, p_src), p_target == p_src.\n");
  }
#endif /* #ifdef _DEBUG_ */
  position_Set_x(p_target, position_Get_x(p_target) - position_Get_x(p_src)); 
  position_Set_y(p_target, position_Get_y(p_target) - position_Get_y(p_src));  
  position_Set_z(p_target, position_Get_z(p_target) - position_Get_z(p_src)); 
} 

FMB_INLINE void position_Mult_by_real(position_t *FMB_RESTRICT p_pos, 
				      COORDINATES_T real){ 
  position_Set_x(p_pos, position_Get_x(p_pos) * real);  
  position_Set_y(p_pos, position_Get_y(p_pos) * real);  
  position_Set_z(p_pos, position_Get_z(p_pos) * real); 
} 



FMB_INLINE COORDINATES_T position_Compute_square_distance(FMB_CONST position_t *FMB_RESTRICT p1, 
							  FMB_CONST position_t *FMB_RESTRICT p2){ 
  return ((position_Get_x(p1) - position_Get_x(p2)) * (position_Get_x(p1) - position_Get_x(p2)))  
    + ((position_Get_y(p1) - position_Get_y(p2)) * (position_Get_y(p1) - position_Get_y(p2)))  
    + ((position_Get_z(p1) - position_Get_z(p2)) * (position_Get_z(p1) - position_Get_z(p2))) ;  
} 


/* WARNING: the way this function is written may cause imbalance problems in terms
   of computation between leafs : there should be a better way of comparing positions 
   such that all leafs do roughly the same amount of computation in the direct computation
   phase ... */
/* return: *p1 "<" *p2 */
FMB_INLINE bool position_Is_lower(FMB_CONST position_t *FMB_RESTRICT p1, 
				  FMB_CONST position_t *FMB_RESTRICT p2){
  if (FMB_FABS(position_Get_x(p1) - position_Get_x(p2)) < EPSILON)
    if (FMB_FABS(position_Get_y(p1) - position_Get_y(p2)) < EPSILON)
      if (FMB_FABS(position_Get_z(p1) - position_Get_z(p2)) < EPSILON)
	return FALSE;
      else
	return position_Get_z(p1) < position_Get_z(p2);
    else 
      return position_Get_y(p1) < position_Get_y(p2);
  else 
    return position_Get_x(p1) < position_Get_x(p2);
}








/*! The "prec" parameter should be equal to "low" when we want to display positions 
  of bodies, and equal to "high" when we want to display computed force vectors of bodies. */
FMB_INLINE void position_Display(FMB_CONST position_t *FMB_RESTRICT p, 
				 FILE *f, 
				 precision_double_t prec){ 
  fprintf(f, "(");

  if (prec == low){
    fprintf(f, LOW_PRECISION_DOUBLE_FPRINTF, position_Get_x(p));
    fprintf(f, ", ");
    fprintf(f, LOW_PRECISION_DOUBLE_FPRINTF, position_Get_y(p));
    fprintf(f, ", ");
    fprintf(f, LOW_PRECISION_DOUBLE_FPRINTF, position_Get_z(p));
  }
  else {
    fprintf(f, HIGH_PRECISION_DOUBLE_FPRINTF, position_Get_x(p));
    fprintf(f, ", ");
    fprintf(f, HIGH_PRECISION_DOUBLE_FPRINTF, position_Get_y(p));
    fprintf(f, ", ");
    fprintf(f, HIGH_PRECISION_DOUBLE_FPRINTF, position_Get_z(p));
  }
  
  fprintf(f, ")");
} 





/*! The "prec" parameter should be equal to "low" when we want to display positions 
  of bodies, and equal to "high" when we want to display computed force vectors of bodies. */
FMB_INLINE void pos_xyz_Display(FMB_CONST COORDINATES_T pos_x, 
				FMB_CONST COORDINATES_T pos_y, 
				FMB_CONST COORDINATES_T pos_z, 
				FILE *f, 
				precision_double_t prec){ 
  fprintf(f, "(");

  if (prec == low){
    fprintf(f, LOW_PRECISION_DOUBLE_FPRINTF, pos_x);
    fprintf(f, ", ");
    fprintf(f, LOW_PRECISION_DOUBLE_FPRINTF, pos_y);
    fprintf(f, ", ");
    fprintf(f, LOW_PRECISION_DOUBLE_FPRINTF, pos_z);
  }
  else {
    fprintf(f, HIGH_PRECISION_DOUBLE_FPRINTF, pos_x);
    fprintf(f, ", ");
    fprintf(f, HIGH_PRECISION_DOUBLE_FPRINTF, pos_y);
    fprintf(f, ", ");
    fprintf(f, HIGH_PRECISION_DOUBLE_FPRINTF, pos_z);
  }
  
  fprintf(f, ")");
} 




FMB_INLINE bool position_Are_relative_corner_positions_correct(position_t *p_corner0_position,
							       position_t *p_corner1_position){
  if ((position_Get_x(p_corner0_position) > position_Get_x(p_corner1_position))
      || (position_Get_y(p_corner0_position) > position_Get_y(p_corner1_position))
      || (position_Get_z(p_corner0_position) > position_Get_z(p_corner1_position)))
    return FALSE;
  else
    return TRUE;
}









/*********************************************************************************************
   SPHERICAL POSITIONS
*********************************************************************************************/

/*! The two following functions are also used for the complex numbers implementation. */
/*! This function gives the value in the range ]-Pi, Pi] which equals "a" modulo 2*Pi. */ 
FMB_INLINE COORDINATES_T angle_Convert_in_MinusPi_Pi(COORDINATES_T a){
  COORDINATES_T res = FMB_FMOD(a, 2*M_PI);
  /* "res" is now in the range ]-2 Pi, 2 Pi[ */
  if (res > M_PI)
    res -= 2*M_PI;

  /* There might be something to do : "<=" is not very relevant for floating point numbers ....*/

  if (res < -M_PI + EPSILON) /* that is to say: if (res <= -M_PI) */
    res += 2*M_PI;
  /* "res" is now in the range ]Pi, Pi] */
  return res;
}
/*! Computes arctan(y/x) and returns a result in the range ]-Pi, Pi]: */
FMB_INLINE COORDINATES_T angle_Compute_from_cartesian(COORDINATES_T x, 
						      COORDINATES_T y){
  /* It might be better to avoid the call to angle_Convert_in_MinusPi_Pi() 
     against additional tests on the sign of y ... */

  /* Remark : the result will be in the range [-Pi, Pi] \n 
     and atan returns values in the range [-Pi/2, Pi/2]. */ 
/*   if (FMB_FABS(x) > (COORDINATES_T) EPSILON) */
/*     return angle_Convert_in_MinusPi_Pi(atan(y/x) + ((x > (COORDINATES_T) 0) ? 0 : M_PI)); */
/*   else */
/*     return ((y < (COORDINATES_T) 0) ? -M_PI_2 : M_PI_2); */

  return FMB_ATAN2(y, x);
}




FMB_INLINE COORDINATES_T spherical_position_Get_ph(FMB_CONST spherical_position_t *FMB_RESTRICT p){  
  return p->ph;  
}  
/*! The user do not need to ensure that v is in the range ]-Pi, Pi].
  spherical_position_Set_ph() will automatically set phi to
  a value in the range ]-Pi, Pi]. */
FMB_INLINE void spherical_position_Set_ph(spherical_position_t *FMB_RESTRICT p, 
					  COORDINATES_T v){   
  p->ph = angle_Convert_in_MinusPi_Pi(v);
}   
/*! It is up to the user to ensure that v is in the range ]-Pi, Pi]. */
FMB_INLINE void spherical_position_Set_ph_not_safe(spherical_position_t *FMB_RESTRICT p,
						   COORDINATES_T v){   
#ifdef _DEBUG_ 
  if (v>M_PI || v<=-M_PI){
    FMB_error("ERROR: in spherical_position_Set_ph_not_safe() v is not in the range ]-Pi,Pi].\n");
  }
#endif /* #ifdef _DEBUG_ */
  p->ph = v;
}   


FMB_INLINE COORDINATES_T spherical_position_Get_th(FMB_CONST spherical_position_t *FMB_RESTRICT p){  
  return p->th;  
}  
FMB_INLINE void spherical_position_Set_th(spherical_position_t *FMB_RESTRICT p, 
					  COORDINATES_T v){   
  /* theta must be in the range [0, Pi]: */
  p->th = FMB_FMOD(v, 2*M_PI); /* theta is now in the range ]-2 Pi, 2 Pi[ */

  if (p->th < 0.0)
    p->th += 2*M_PI; /* theta is now in the range [0, 2 Pi[ */

  if (p->th > M_PI){
/* if (INFO_DISPLAY(3)){ */
/*     fprintf(stderr,  */
/* 	    "WARNING: p->th > Pi in spherical_position_Set_th => modifying p->ph too !!\n");  */
/* } */
    p->th = 2*M_PI - p->th;
    spherical_position_Set_ph(p, spherical_position_Get_ph(p) + M_PI);
  }
}   


FMB_INLINE COORDINATES_T spherical_position_Get_r(FMB_CONST spherical_position_t *FMB_RESTRICT p){  
  return p->r;  
}  
FMB_INLINE void spherical_position_Set_r(spherical_position_t *FMB_RESTRICT p, 
					 COORDINATES_T v){   
  if (v < (COORDINATES_T) 0){
/* if (INFO_DISPLAY(3)){ */
/*     fprintf(stderr,  */
/* 	    "WARNING: p->r < 0 in spherical_position_Set_r => modifying p->ph and p->th too !!\n");  */
/* } */
    p->r = -v;
    spherical_position_Set_ph(p, M_PI - spherical_position_Get_th(p));
    spherical_position_Set_th(p, spherical_position_Get_th(p) + M_PI);
  }
  else 
    p->r = v;   
}  


/*! Use the following function whenever it is possible (instead of 3 calls           \n
  to: spherical_position_Set_r(), spherical_position_Set_th() and                    \n
  spherical_position_Set_ph()) or be sure to call the 3 functions in the same        \n
  order as the one used in the definition of spherical_position_Set_r_th_ph()!       */ 
FMB_INLINE void spherical_position_Set_r_th_ph(spherical_position_t *FMB_RESTRICT p,
					       COORDINATES_T r_value,
					       COORDINATES_T th_value,
					       COORDINATES_T ph_value){
  /* We have to call spherical_position_Set_ph() before spherical_position_Set_th() 
     since spherical_position_Set_th() might modify the "ph" field, 
     and we have to call spherical_position_Set_r() at the end since it 
     might modify the fields "th" and "ph" ... */
  spherical_position_Set_ph(p, ph_value);
  spherical_position_Set_th(p, th_value);
  spherical_position_Set_r(p, r_value);
}
/*! "not_safe" is only for "ph". */
FMB_INLINE void spherical_position_Set_r_th_ph_not_safe(spherical_position_t *FMB_RESTRICT p,
							COORDINATES_T r_value,
							COORDINATES_T th_value,
							COORDINATES_T ph_value){
  /* We have to call spherical_position_Set_ph() before spherical_position_Set_th() 
     since spherical_position_Set_th() might modify the "ph" field, 
     and we have to call spherical_position_Set_r() at the end since it 
     might modify the fields "th" and "ph" ... */
  spherical_position_Set_ph_not_safe(p, ph_value);
  spherical_position_Set_th(p, th_value);
  spherical_position_Set_r(p, r_value);
}


FMB_INLINE void spherical_position_2cartesian(FMB_CONST spherical_position_t *FMB_RESTRICT p_sphe,  
 					      position_t *FMB_RESTRICT p_cart){ 
  position_Set_x(p_cart,  
 		 spherical_position_Get_r(p_sphe) * FMB_COS(spherical_position_Get_ph(p_sphe)) * FMB_SIN(spherical_position_Get_th(p_sphe))); 
  position_Set_y(p_cart,  
 		 spherical_position_Get_r(p_sphe) * FMB_SIN(spherical_position_Get_ph(p_sphe)) * FMB_SIN(spherical_position_Get_th(p_sphe))); 
  position_Set_z(p_cart,  
 		 spherical_position_Get_r(p_sphe) * FMB_COS(spherical_position_Get_th(p_sphe))); 
} 

FMB_INLINE void position_2spherical(FMB_CONST position_t *FMB_RESTRICT p_cart,  
 				    spherical_position_t *FMB_RESTRICT p_sphe){ 
  /* Remark: the use of spherical_position_Set_r_th_ph[_not_safe] is not very           \n
     convenient here, but we take care of calling the functions in the right order.     */ 

  /* We have to call spherical_position_Set_r() before spherical_position_Set_th but 
     there is no problem since sqrt(...) will always be positive! */
  spherical_position_Set_r(p_sphe,  
			   FMB_SQRT((position_Get_x(p_cart) * position_Get_x(p_cart)) + 
				(position_Get_y(p_cart) * position_Get_y(p_cart)) + 
				(position_Get_z(p_cart) * position_Get_z(p_cart))));
  
  spherical_position_Set_ph_not_safe(p_sphe, 
				     angle_Compute_from_cartesian(position_Get_x(p_cart),
								  position_Get_y(p_cart)));
  
  /* Remark : "th" (i.e. "theta") is in the range [0, Pi]   \n
     and acos returns values in this range.                 */
  /* What if r==0.0 ???????????????????????????????????????????? */
  spherical_position_Set_th(p_sphe, 
			    FMB_ACOS(position_Get_z(p_cart) / spherical_position_Get_r(p_sphe)));
}  


FMB_INLINE void position_2_r_cos_th_sin_th_ph(FMB_CONST position_t *FMB_RESTRICT p_cart,
					      REAL_T *p_r,
					      REAL_T *p_cos_th,
					      REAL_T *p_sin_th,
					      REAL_T *p_ph){
  COORDINATES_T x2_plus_y2 = (position_Get_x(p_cart) * position_Get_x(p_cart)) + 
    (position_Get_y(p_cart) * position_Get_y(p_cart));

  *p_r = FMB_SQRT( x2_plus_y2 + (position_Get_z(p_cart) * position_Get_z(p_cart)));
  *p_ph = angle_Compute_from_cartesian(position_Get_x(p_cart), position_Get_y(p_cart));
  /* cos_th = z/r */
  *p_cos_th = position_Get_z(p_cart) / *p_r;
  /* sin_th = sqrt(x^2 + y^2)/r */
  *p_sin_th = FMB_SQRT(x2_plus_y2) / *p_r;
}



/*! This function returns in *p_rel the relative spherical coordinates \n
  of the *p_abs absolute coordinates with regard to *p_new_origin. */ 
FMB_INLINE void spherical_position_Compute_relative_coordinates(FMB_CONST spherical_position_t *FMB_RESTRICT p_abs,
								FMB_CONST spherical_position_t *FMB_RESTRICT p_new_origin,
								spherical_position_t *FMB_RESTRICT p_rel){
  /***** First method: *****/
  /* We convert to cartesian positions, compute the relative position in cartesian
     coordinates and convert back to spherical coordinates. */
  /*   position_t abs_cart, new_origin_cart, rel_cart; */
  /*   spherical_position_2cartesian(p_abs, &abs_cart); */
  /*   spherical_position_2cartesian(p_new_origin, &new_origin_cart); */
  /*   position_Negate(&new_origin_cart); */
  /*   position_Add(&abs_cart, &new_origin_cart); */
  /*   position_2spherical(&abs_cart, p_rel); */
  


  /***** Second method: *****/
  /* P = (r, theta, phi) (=*p_new_origin)
     Q = (rho, alpha, beta) (=*p_abs)
     We want to compute the relative coordinates (rho', alpha', beta') 
     (=*p_rel) of Q with regard to P:
     - rho' = sqrt(r^2 + rho^2 - 2*r*rho*cos(gamma)
       ou cos(gamma) = cos(theta)*cos(alpha) + sin(theta)*sin(alpha)*cos(phi-beta)
     - alpha' = arccos( (rho*cos(alpha) - r*cos(theta)) / rho' )
     - beta' = ... no easy way: I choose to use the cartesian coordinates ...
  */
  COORDINATES_T x_tmp, y_tmp;

  /* Remark: the use of spherical_position_Set_r_th_ph[_not_safe] is not very           \n
     convenient here, but we take care of calling the functions in the right order.     */ 

  /* Computing of rho': */
  COORDINATES_T cos_gamma = FMB_COS(spherical_position_Get_th(p_new_origin))*FMB_COS(spherical_position_Get_th(p_abs)) + FMB_SIN(spherical_position_Get_th(p_new_origin))*FMB_SIN(spherical_position_Get_th(p_abs))*FMB_COS(spherical_position_Get_ph(p_new_origin) - spherical_position_Get_ph(p_abs));

  /* We have to call spherical_position_Set_r() before spherical_position_Set_th but 
     there is no problem since sqrt(...) will always be positive! */
  spherical_position_Set_r(p_rel, 
			   FMB_SQRT(spherical_position_Get_r(p_new_origin)*spherical_position_Get_r(p_new_origin) + spherical_position_Get_r(p_abs)*spherical_position_Get_r(p_abs) - 2*spherical_position_Get_r(p_new_origin)*spherical_position_Get_r(p_abs)*cos_gamma));

  /* Computing of beta': */
  /* We do not use spherical_position_2cartesian() and position_2spherical() 
     since we do not need the coordinate along the z-axis. */
  x_tmp = spherical_position_Get_r(p_abs) * FMB_COS(spherical_position_Get_ph(p_abs)) * FMB_SIN(spherical_position_Get_th(p_abs)); 
  y_tmp = spherical_position_Get_r(p_abs) * FMB_SIN(spherical_position_Get_ph(p_abs)) * FMB_SIN(spherical_position_Get_th(p_abs));
  /* Computing relative cartesian coordinates along the x-axis and the y-axis: */
  x_tmp -= spherical_position_Get_r(p_new_origin) * FMB_COS(spherical_position_Get_ph(p_new_origin)) * FMB_SIN(spherical_position_Get_th(p_new_origin));
  y_tmp -= spherical_position_Get_r(p_new_origin) * FMB_SIN(spherical_position_Get_ph(p_new_origin)) * FMB_SIN(spherical_position_Get_th(p_new_origin));
  /* Back to spherical: */
  spherical_position_Set_ph_not_safe(p_rel, 
				     angle_Compute_from_cartesian(x_tmp, y_tmp));

  /* Computing of alpha': */
  /* Remark : alpha' is in the range [0, Pi] \n  
     and acos returns values in this range. */ 
  spherical_position_Set_th(p_rel, 
			    FMB_ACOS((spherical_position_Get_r(p_abs)*FMB_COS(spherical_position_Get_th(p_abs)) - spherical_position_Get_r(p_new_origin)*FMB_COS(spherical_position_Get_th(p_new_origin))) / spherical_position_Get_r(p_rel)));
  
}


/*! 
  - *p_center_of_local_base are the spherical coordinates of the center of the local 
  base of the spherical coordinates.
  - *p_pos_in_local_base are the cartesian coordinates of the vector in the local base 
  of the spherical coordinates.
  - *p_result will contain (at the return of the function) the cartesian coordinate of
  the vector in the "usual" base of the cartesian coordinates centred in the origin.
*/
FMB_INLINE void spherical_position_Compute_cartesian_from_local_base(FMB_CONST spherical_position_t *FMB_RESTRICT p_center_of_local_base,
								     FMB_CONST position_t *FMB_RESTRICT p_pos_in_local_base,
								     position_t *FMB_RESTRICT p_result){

  /* The local base is:
     - e_r = ( cos(phi).sin(theta), sin(phi).sin(theta), cos(theta))
     - e_theta = ( cos(phi).cos(theta), sin(phi).cos(theta), -sin(theta))  (i.e. theta -> theta + Pi/2 )
     - e_phi = ( -sin(phi), cos(phi), 0)  (i.e. theta=0 and phi -> phi + Pi/2 )
  */

  /* This can be written as a matrix product:
     
   [ f_x ]    [ cos(phi).sin(theta)  cos(phi).cos(theta)  -sin(phi) ]   [ f_r     ]
   [ f_y ]  = [ sin(phi).sin(theta)  sin(phi).cos(theta)  cos(phi)  ] . [ f_theta ]
   [ f_z ]    [ cos(theta)           -sin(theta)          0         ]   [ f_phi   ]
  
   with (f_x, f_y, f_z) = *p_result 
   and  (f_r, f_theta, f_phi) = *p_pos_in_local_base
   and theta = spherical_position_Get_th(p_center_of_local_base)
   and phi = spherical_position_Get_ph(p_center_of_local_base)
  */

  COORDINATES_T cos_theta = FMB_COS(spherical_position_Get_th(p_center_of_local_base));
  COORDINATES_T cos_phi = FMB_COS(spherical_position_Get_ph(p_center_of_local_base));
  COORDINATES_T sin_theta = FMB_SIN(spherical_position_Get_th(p_center_of_local_base));
  COORDINATES_T sin_phi = FMB_SIN(spherical_position_Get_ph(p_center_of_local_base));

  position_Set_x(p_result, 
		 cos_phi * sin_theta * position_Get_x(p_pos_in_local_base) + 
		 cos_phi * cos_theta * position_Get_y(p_pos_in_local_base) + 
		 (-sin_phi) * position_Get_z(p_pos_in_local_base));

  position_Set_y(p_result,
		 sin_phi * sin_theta * position_Get_x(p_pos_in_local_base) +
		 sin_phi * cos_theta * position_Get_y(p_pos_in_local_base) +
		 cos_phi * position_Get_z(p_pos_in_local_base));

  position_Set_z(p_result,
		 cos_theta * position_Get_x(p_pos_in_local_base) +
		 (-sin_theta) * position_Get_y(p_pos_in_local_base));
}


FMB_INLINE void spherical_position_Display(FMB_CONST spherical_position_t *FMB_RESTRICT p,
					   FILE *f){
  fprintf(f, "(r=");
  fprintf(f, LOW_PRECISION_DOUBLE_FPRINTF, spherical_position_Get_r(p));
  fprintf(f, ", th=");
  fprintf(f, LOW_PRECISION_DOUBLE_FPRINTF, spherical_position_Get_th(p));
  fprintf(f, ", ph=");
  fprintf(f, LOW_PRECISION_DOUBLE_FPRINTF, spherical_position_Get_ph(p));
  fprintf(f, ")");
}











#endif







