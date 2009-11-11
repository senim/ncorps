#ifndef __DIRECT_COMPUTATION_H__
#define __DIRECT_COMPUTATION_H__

#include "bodies.h"






/*********************************************************************************************
**********************************************************************************************

   Compute the interaction (forces and/or potentials)

**********************************************************************************************
*********************************************************************************************/



/*! If 'mutual' is set to TRUE, we use the mutual interaction principle to set the force vector of 
 all the bodies of *p_b_target AND of *p_b_src (and possibly the potential).
 Otherwise we do not modify the content of *p_b_src. */
void bodies_Compute_direct_interaction(bodies_t *FMB_RESTRICT p_b_target, 
				       bodies_t *FMB_RESTRICT p_b_src,
				       bool mutual);

/*! The mutual interaction principle is always used in 'bodies_Compute_own_interaction()'. */
void bodies_Compute_own_interaction(bodies_t *FMB_RESTRICT p_b);

void bodies_Compute_own_interaction_for_first_ones(bodies_t *FMB_RESTRICT p_b,
					      bodies_ind_t nmax);







#endif /* #ifdef __DIRECT_COMPUTATION_H__ */ 
