#ifndef __CONFIG_H__
#define __CONFIG_H__





/*********************************************************************************************
**********************************************************************************************

   General options:

**********************************************************************************************
*********************************************************************************************/

/* For comparison with falcON: do no set  _USE_CONSTANT_INTERACTION_FACTOR_ 
 * Moreover the code of the function "bodies_Compute_total_potential_energy()" has to be changed:
 * the sum must only add bodies' potentials, and NOT multiply them with their values 
 *
 * For comparison with the 'treecode': do no set  _USE_CONSTANT_INTERACTION_FACTOR_ 
 * (warning: the force vector 
 * contains the acceleration with /home/fortin/1..THESE/6..CODES_EXT/1..TREE_CODE/output_data/convert_TC_2_FMB ...).
 *
 *
 * For comparison with FMMPART3D: 
 * do not set _USE_CONSTANT_INTERACTION_FACTOR_ and  _USE_DSCAL_WITH_CONSTANT_INTERACTION_FACTOR_,
 * 
 */



/*********** Interaction factor ***********/
/* #define _USE_CONSTANT_INTERACTION_FACTOR_       */
/* #define _USE_DSCAL_WITH_CONSTANT_INTERACTION_FACTOR_ */


/*********** 'position_t':  ***********/
/* See: position.h */
#define _POSITION_T_STORED_WITH_ARRAY_



/*********** For computation in single precision: ***********/
#define _SINGLE_PREC_
/* Warning: direct computation is slower with single precision on IBM Power3 & IBM Power4 ! */




/*********** Bodies'data storing: ***********/
/* Storing the different data of a 'body' in the same array or in separate ones. */
#define _BODIES_SPLIT_DATA_



/*********************************************************************************************
**********************************************************************************************

   Debugging options:

**********************************************************************************************
*********************************************************************************************/


/* Trace memory consumption: */
/* #define _FMB_MEM_TRACE_ */

#endif /* #ifndef __CONFIG_H__ */





