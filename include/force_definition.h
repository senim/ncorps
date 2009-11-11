#ifndef __FORCE_DEFINITION_H__
#define __FORCE_DEFINITION_H__

/*! \file 
  We describe here all the features of the force field between the bodies
  (electrostatic, gravitational ...) */

/*! Universal gravitational constant: */
#define G ((VALUES_T) 6.67259E-11)
/* In 2D: */
/* #define POTENTIAL_SIGN  */
/* #define UNIT_VECTOR_COMPONENT(tgt_comp, src_comp) ((tgt_comp) - (src_comp))   */
/* In 3D: */
#define POTENTIAL_SIGN -
#define UNIT_VECTOR_COMPONENT(tgt_comp, src_comp) ((src_comp) - (tgt_comp))  


#define CONSTANT_INTERACTION_FACTOR G 













/*********************************************************************************************
**********************************************************************************************
**********************************************************************************************

   MACROS MUTUAL 

**********************************************************************************************
**********************************************************************************************
*********************************************************************************************/


/*********************************************************************************************
**********************************************************************************************

   Forces only

**********************************************************************************************
*********************************************************************************************/


/* With softening parameter. */
/* 23 flops 
 * (remark: there is another version which exposes rsqrt() but leads to 24 flops...). */
#define DIRECT_COMPUTATION_MUTUAL_SOFT(pxt, pyt, pzt,								\
				       pxs, pys, pzs,								\
				       v_target,								\
				       v_src,									\
				       fxt, fyt, fzt,								\
				       fxs, fys, fzs,	        					        \
				       unused1,									\
				       unused2,									\
                                       eps_soft_square){                                                        \
  COORDINATES_T dx = UNIT_VECTOR_COMPONENT(pxt, pxs);								\
  COORDINATES_T dy = UNIT_VECTOR_COMPONENT(pyt, pys);								\
  COORDINATES_T dz = UNIT_VECTOR_COMPONENT(pzt, pzs);								\
  COORDINATES_T inv_square_distance;										\
  COORDINATES_T inv_distance;											\
  COORDINATES_T fx, fy, fz;											\
														\
  /************ Compute the "distance" and the norm of the force: ***************************/			\
  inv_square_distance = 1.0/ (dx*dx + dy*dy + dz*dz + eps_soft_square);	        				\
  inv_distance = FMB_SQRT(inv_square_distance);									\
														\
  /* The "inv_square_distance" will now be equal to: distance^{-3} */						\
  inv_distance *= (v_target) * (v_src);										\
  inv_square_distance *= inv_distance;										\
														\
  /************ Compute the force vector contribution 								\
                and update the force vectors of the target AND the source: ******************/			\
  fx = dx * inv_square_distance;										\
  fy = dy * inv_square_distance;										\
  fz = dz * inv_square_distance;										\
  fxt += fx;													\
  fyt += fy;													\
  fzt += fz;													\
  fxs -= fx;													\
  fys -= fy;													\
  fzs -= fz;													\
														\
}









#endif /* #ifndef FORCE_DEFINITION_H */


