#include "bodies.h"
#include "IO.h"





/*********************************************************************************************
   bodies_Allocate
*********************************************************************************************/
bodies_t *bodies_Allocate(){
  return (bodies_t *) FMB_malloc_with_check(sizeof(bodies_t));
}






/*********************************************************************************************
**********************************************************************************************

   Elementary functions for 1 body:

**********************************************************************************************
*********************************************************************************************/

/*********************************************************************************************
   bodies_Initialize_body
*********************************************************************************************/
void bodies_Initialize_body(bodies_t *p_b, 
			    bodies_ind_t body_number){
  bodies_Set_pos_x(p_b, body_number, (COORDINATES_T) 0.0); 
  bodies_Set_pos_y(p_b, body_number, (COORDINATES_T) 0.0); 
  bodies_Set_pos_z(p_b, body_number, (COORDINATES_T) 0.0); 
  bodies_Set_value(p_b, body_number, 0.0); 
  position_Initialize(bodies_Get_p_speed_vector(p_b, body_number)); 
  bodies_Set_fx(p_b, body_number, (COORDINATES_T) 0.0); 
  bodies_Set_fy(p_b, body_number, (COORDINATES_T) 0.0); 
  bodies_Set_fz(p_b, body_number, (COORDINATES_T) 0.0); 
}






/*********************************************************************************************
   find_corresponding_child
*********************************************************************************************/
unsigned char find_corresponding_child(position_t *p_body_position,
				       position_t *p_center_position){
  if (position_Get_x(p_body_position) > position_Get_x(p_center_position))
    /* F.. */
    if (position_Get_y(p_body_position) > position_Get_y(p_center_position))
      /* F.R */
      if (position_Get_z(p_body_position) > position_Get_z(p_center_position))
	return FUR;
      else
	return FDR;
    else
      /* F.L */
      if (position_Get_z(p_body_position) > position_Get_z(p_center_position))
	return FUL;
      else
	return FDL;
  else
    /* B.. */
    if (position_Get_y(p_body_position) >	position_Get_y(p_center_position))
      /* B.R */
      if (position_Get_z(p_body_position) > position_Get_z(p_center_position))
	return BUR;
      else
	return BDR;
    else
      /* B.L */
      if (position_Get_z(p_body_position) > position_Get_z(p_center_position))
	return BUL;
      else
	return BDL;
}



















/*********************************************************************************************
**********************************************************************************************

   Elementary functions for a 'body_array_t' variable:

**********************************************************************************************
*********************************************************************************************/


/*********************************************************************************************
   body_array_Initialize
*********************************************************************************************/
void body_array_Initialize(body_array_t *p_b,
			   bodies_ind_t initial_size_allocated){
  p_b->nb_bodies = 0;

  p_b->p_bodies = 
    (body_t *) FMB_malloc_with_check(initial_size_allocated * sizeof(body_t));

  p_b->size_allocated = initial_size_allocated;

/*   p_b->thread_owner = -1; /\* for debugging only *\/ */
}


/*********************************************************************************************
   body_array_Initialize_with_bodies
*********************************************************************************************/
/* We assume the 'p_bodies' array has been allocated for 'nb_bodies' bodies. */
void body_array_Initialize_with_bodies(body_array_t *p_b,
				       body_t *p_bodies,
				       bodies_ind_t nb_bodies){
  p_b->nb_bodies = nb_bodies;

  p_b->p_bodies = p_bodies;

  p_b->size_allocated = nb_bodies;

/*   p_b->thread_owner = -1; /\* for debugging only *\/ */
}


/*********************************************************************************************
   body_array_Free
*********************************************************************************************/
void body_array_Free(body_array_t *p_b){
  p_b->nb_bodies = 0;
  p_b->size_allocated = 0;

  FMB_free(p_b->p_bodies);
}



/*********************************************************************************************
   body_array_Realloc_arrays
*********************************************************************************************/
/* Private function: */
/* WARNING: this function should not too much be called 
 * since it may not be efficient when the new_size is greater 
 * than the actual size allocated. 
 * Indeed the arrays might be allocated one after the other
 * in memory and they will all be copied somewhere else 
 * in the memory during the 'FMB_realloc_with_check()' call ... */
void body_array_Realloc_arrays(body_array_t *p_b, bodies_ind_t new_size){

  p_b->p_bodies = 
    (body_t *) FMB_realloc_with_check(p_b->p_bodies,
				  new_size * sizeof(body_t));
  p_b->size_allocated = new_size;
}




/*********************************************************************************************
   body_array_ClearFP
*********************************************************************************************/
void body_array_ClearFP(body_array_t *p_b){
  body_array_it_t it;
  /* Can be accelerated with 'memset'. */
  for (body_array_it_Initialize(&it, p_b);
       body_array_it_Is_valid(&it);
       body_array_it_Go2Next_NoPVS(&it)){
    position_Initialize(body_array_it_Get_p_force_vector(&it));
  }
}


/*********************************************************************************************
   body_array_Is_full
*********************************************************************************************/
bool body_array_Is_full(body_array_t *p_b){
  return (p_b->nb_bodies == p_b->size_allocated);
}


/*********************************************************************************************
   body_array_Adjust_memory
*********************************************************************************************/
void body_array_Adjust_memory(body_array_t *p_b){
  if (p_b->nb_bodies < p_b->size_allocated){
    body_array_Realloc_arrays(p_b, p_b->nb_bodies);
  }
}


/*********************************************************************************************
   body_array_Affect
*********************************************************************************************/
void body_array_Affect(body_array_t *p_target, body_array_t *p_src){
  if (p_target->size_allocated != p_src->size_allocated){
    body_array_Realloc_arrays(p_target, p_src->size_allocated);
  }

  memcpy(p_target->p_bodies, p_src->p_bodies, 
	 p_target->size_allocated * sizeof(body_t));

  p_target->nb_bodies = p_src->nb_bodies;
}





/*********************************************************************************************
   body_array_Add
*********************************************************************************************/
void body_array_Add(body_array_t *p_b, 
		    body_t *p_body){
  if (body_array_Is_full(p_b)){
    /* We use +1 for the small values of p_b->size_allocated: */
    body_array_Realloc_arrays(p_b, (p_b->size_allocated + 1) * BODIES_EXTENSION_FACTOR);
  }

  Copy_body2body_array(p_b, 
		       p_b->nb_bodies,
		       p_body);
  p_b->nb_bodies++;
  
}


/*********************************************************************************************
   body_array_Remove
*********************************************************************************************/
void body_array_Remove(body_array_t *p_b, bodies_ind_t body_number){
  bodies_ind_t k;
  
  for (k=body_number; k<(p_b->nb_bodies - 1) ;++k){
    Copy_body_array2body_array(p_b, k, p_b, k+1);
  }
  p_b->nb_bodies--;
}


/*********************************************************************************************
   body_array_Display
*********************************************************************************************/
void body_array_Display(body_array_t *p_b, FILE *f){
  body_array_it_t it;
  for (body_array_it_Initialize(&it, p_b);
       body_array_it_Is_valid(&it);
       body_array_it_Go2Next(&it)){
    body_array_it_Display_body(&it, f);
  }
}





/*********************************************************************************************
   body_array_Get_mem_used
*********************************************************************************************/
long body_array_Get_mem_used(body_array_t *p_b){
  long res = sizeof(body_array_t);
  res += p_b->size_allocated * sizeof(body_t);
  return res;
}
















/*********************************************************************************************
**********************************************************************************************

   Elementary functions for a 'body_split_array_t' variable:

**********************************************************************************************
*********************************************************************************************/



/*********************************************************************************************
   body_split_array_Initialize
*********************************************************************************************/
void body_split_array_Initialize(body_split_array_t *p_b,
				 bodies_ind_t initial_size_allocated){
  p_b->nb_bodies = 0;

  p_b->p_pos_x = (COORDINATES_T *) 
    FMB_malloc_with_check(initial_size_allocated * sizeof(COORDINATES_T));
  p_b->p_pos_y = (COORDINATES_T *) 
    FMB_malloc_with_check(initial_size_allocated * sizeof(COORDINATES_T));
  p_b->p_pos_z = (COORDINATES_T *) 
    FMB_malloc_with_check(initial_size_allocated * sizeof(COORDINATES_T));

  p_b->p_values = (VALUES_T *) 
    FMB_malloc_with_check(initial_size_allocated * sizeof(VALUES_T));

  p_b->p_fx = (COORDINATES_T *) 
    FMB_malloc_with_check(initial_size_allocated * sizeof(COORDINATES_T));
  p_b->p_fy = (COORDINATES_T *) 
    FMB_malloc_with_check(initial_size_allocated * sizeof(COORDINATES_T));
  p_b->p_fz = (COORDINATES_T *) 
    FMB_malloc_with_check(initial_size_allocated * sizeof(COORDINATES_T));

  p_b->p_speed_vectors = (position_t *) 
    FMB_malloc_with_check(initial_size_allocated * sizeof(position_t));

  p_b->size_allocated = initial_size_allocated;
}


/*********************************************************************************************
   body_split_array_Free
*********************************************************************************************/
void body_split_array_Free(body_split_array_t *p_b){
  p_b->nb_bodies = 0;
  p_b->size_allocated = 0;

  FMB_free(p_b->p_pos_x);
  FMB_free(p_b->p_pos_y);
  FMB_free(p_b->p_pos_z);

  FMB_free(p_b->p_values);

  FMB_free(p_b->p_fx);
  FMB_free(p_b->p_fy);
  FMB_free(p_b->p_fz);

  FMB_free(p_b->p_speed_vectors);

}


/*********************************************************************************************
   body_split_array_Realloc_arrays
*********************************************************************************************/
/* Private function: */
/* WARNING: this function should not too much be called 
 * since it may not be efficient when the new_size is greater 
 * than the actual size allocated. 
 * Indeed the arrays might be allocated one after the other
 * in memory and they will all be copied somewhere else 
 * in the memory during the 'FMB_realloc_with_check()' call ... */
void body_split_array_Realloc_arrays(body_split_array_t *p_b, bodies_ind_t new_size){

  p_b->p_pos_x = (COORDINATES_T *) 
    FMB_realloc_with_check(p_b->p_pos_x,
				      new_size * sizeof(COORDINATES_T));
  p_b->p_pos_y = (COORDINATES_T *) 
    FMB_realloc_with_check(p_b->p_pos_y,
				      new_size * sizeof(COORDINATES_T));
  p_b->p_pos_z = (COORDINATES_T *) 
    FMB_realloc_with_check(p_b->p_pos_z,
				      new_size * sizeof(COORDINATES_T));

  p_b->p_values = (VALUES_T *) 
    FMB_realloc_with_check(p_b->p_values,
				      new_size * sizeof(VALUES_T));

  p_b->p_fx = (COORDINATES_T *) 
    FMB_realloc_with_check(p_b->p_fx,
				      new_size * sizeof(COORDINATES_T));
  p_b->p_fy = (COORDINATES_T *) 
    FMB_realloc_with_check(p_b->p_fy,
				      new_size * sizeof(COORDINATES_T));
  p_b->p_fz = (COORDINATES_T *) 
    FMB_realloc_with_check(p_b->p_fz,
				      new_size * sizeof(COORDINATES_T));

  p_b->p_speed_vectors = (position_t *) 
    FMB_realloc_with_check(p_b->p_speed_vectors, new_size * sizeof(position_t));
  
  p_b->size_allocated = new_size;
}




/*********************************************************************************************
   body_split_array_ClearFP
*********************************************************************************************/
void body_split_array_ClearFP(body_split_array_t *p_b){
  body_split_array_it_t it;
  /* Can be accelerated with 'memset'. */
  for (body_split_array_it_Initialize(&it, p_b);
       body_split_array_it_Is_valid(&it);
       body_split_array_it_Go2Next_NoPVS(&it)){

    body_split_array_it_Set_fx(&it, (COORDINATES_T) 0.0);
    body_split_array_it_Set_fy(&it, (COORDINATES_T) 0.0);
    body_split_array_it_Set_fz(&it, (COORDINATES_T) 0.0);

  }
}


/*********************************************************************************************
   body_split_array_Is_full
*********************************************************************************************/
bool body_split_array_Is_full(body_split_array_t *p_b){
  return (p_b->nb_bodies == p_b->size_allocated);
}


/*********************************************************************************************
   body_split_array_Adjust_memory
*********************************************************************************************/
void body_split_array_Adjust_memory(body_split_array_t *p_b){
  if (p_b->nb_bodies < p_b->size_allocated){
    body_split_array_Realloc_arrays(p_b, p_b->nb_bodies);
  }
}


/*********************************************************************************************
   body_split_array_Affect
*********************************************************************************************/
void body_split_array_Affect(body_split_array_t *p_target,body_split_array_t *p_src){
  body_split_array_Realloc_arrays(p_target, p_src->size_allocated);

  memcpy(p_target->p_pos_x, p_src->p_pos_x, 
	 p_target->size_allocated * sizeof(COORDINATES_T));
  memcpy(p_target->p_pos_y, p_src->p_pos_y, 
	 p_target->size_allocated * sizeof(COORDINATES_T));
  memcpy(p_target->p_pos_z, p_src->p_pos_z, 
	 p_target->size_allocated * sizeof(COORDINATES_T));

  memcpy(p_target->p_values, p_src->p_values, 
	 p_target->size_allocated * sizeof(VALUES_T));

  memcpy(p_target->p_fx, p_src->p_fx, 
	 p_target->size_allocated * sizeof(COORDINATES_T));
  memcpy(p_target->p_fy, p_src->p_fy, 
	 p_target->size_allocated * sizeof(COORDINATES_T));
  memcpy(p_target->p_fz, p_src->p_fz, 
	 p_target->size_allocated * sizeof(COORDINATES_T));

  memcpy(p_target->p_speed_vectors, p_src->p_speed_vectors, 
	 p_target->size_allocated * sizeof(position_t));

  p_target->nb_bodies = p_src->nb_bodies;
}



/*********************************************************************************************
   body_split_array_Add
*********************************************************************************************/
void body_split_array_Add(body_split_array_t *p_b, 
			  body_t *p_body){
  if (body_split_array_Is_full(p_b)){
    /* We use +1 for the small values of p_b->size_allocated: */
    body_split_array_Realloc_arrays(p_b, (p_b->size_allocated + 1) * BODIES_EXTENSION_FACTOR);
  }

  Copy_body2body_split_array(p_b, 
			     p_b->nb_bodies,
			     p_body);
  p_b->nb_bodies++;
  
}


/*********************************************************************************************
   body_split_array_Remove
*********************************************************************************************/
void body_split_array_Remove(body_split_array_t *p_b, bodies_ind_t body_number){
  bodies_ind_t k;
  
  for (k=body_number; k<(p_b->nb_bodies - 1) ;++k){
    Copy_body_split_array2body_split_array(p_b, k, p_b, k+1);
  }
  p_b->nb_bodies--;
}


/*********************************************************************************************
   body_split_array_Display
*********************************************************************************************/
void body_split_array_Display(body_split_array_t *p_b, FILE *f){
  body_split_array_it_t it;
  for (body_split_array_it_Initialize(&it, p_b);
       body_split_array_it_Is_valid(&it);
       body_split_array_it_Go2Next(&it)){
    body_split_array_it_Display_body(&it, f);
  }
}




/*********************************************************************************************
   body_split_array_Get_mem_used
*********************************************************************************************/
long body_split_array_Get_mem_used(body_split_array_t *p_b){
  long res = sizeof(body_split_array_t);
  bodies_ind_t n = p_b->size_allocated;

  res += n * sizeof(position_t);
  res += n * sizeof(VALUES_T);
  res += n * sizeof(position_t);
  res += n * sizeof(position_t);

  return res;
}






















/*********************************************************************************************
   bodies_Add_pot_and_forces
*********************************************************************************************/
void bodies_Add_pot_and_forces(bodies_t *p_tgt_bodies, 
			       bodies_t *p_src_bodies){
  bodies_it_t it_src, it_tgt;

  bodies_it_Initialize(&it_tgt, p_tgt_bodies); 
  bodies_it_Initialize(&it_src, p_src_bodies);

  for (; 
       bodies_it_Is_valid(&it_tgt);
       bodies_it_Go2Next_NoPVS(&it_tgt), bodies_it_Go2Next_NoPVS(&it_src)){
    
    bodies_it_Set_fx(&it_tgt, bodies_it_Get_fx(&it_tgt) + bodies_it_Get_fx(&it_src));
    bodies_it_Set_fy(&it_tgt, bodies_it_Get_fy(&it_tgt) + bodies_it_Get_fy(&it_src));
    bodies_it_Set_fz(&it_tgt, bodies_it_Get_fz(&it_tgt) + bodies_it_Get_fz(&it_src));
  } /* for */
}





/*********************************************************************************************
**********************************************************************************************

   Scaling of the forces and the potentials with CONSTANT_INTERACTION_FACTOR

**********************************************************************************************
*********************************************************************************************/

/*********************************************************************************************
   bodies_Scale_with_CONSTANT_INTERACTION_FACTOR
*********************************************************************************************/
#ifdef _USE_CONSTANT_INTERACTION_FACTOR_
/* We can also use CONSTANT_INTERACTION_FACTOR only for the total potential energy ... */
void bodies_Scale_with_CONSTANT_INTERACTION_FACTOR(bodies_t *p_b){
#ifdef _USE_DSCAL_WITH_CONSTANT_INTERACTION_FACTOR_
#ifndef _BODIES_SPLIT_DATA_
  dscal(3 * bodies_Nb_bodies(p_b),
	CONSTANT_INTERACTION_FACTOR,
	position_Get_p_x(bodies_Get_p_force_vector(p_b, 0)),
	1);
#else /* #ifndef _BODIES_SPLIT_DATA_ */
  dscal(bodies_Nb_bodies(p_b),
	CONSTANT_INTERACTION_FACTOR,
	p_b->p_fx,
	1);
  dscal(bodies_Nb_bodies(p_b),
	CONSTANT_INTERACTION_FACTOR,
	p_b->p_fy,
	1);
  dscal(bodies_Nb_bodies(p_b),
	CONSTANT_INTERACTION_FACTOR,
	p_b->p_fz,
	1);
#endif /* #ifndef _BODIES_SPLIT_DATA_ */
#else /* #ifdef _USE_DSCAL_WITH_CONSTANT_INTERACTION_FACTOR_ */
  bodies_it_t it;
  for (bodies_it_Initialize(&it, p_b);
       bodies_it_Is_valid(&it); 
       bodies_it_Go2Next_NoPVS(&it)){

    bodies_it_Set_fx(&it, bodies_it_Get_fx(&it) * CONSTANT_INTERACTION_FACTOR);
    bodies_it_Set_fy(&it, bodies_it_Get_fy(&it) * CONSTANT_INTERACTION_FACTOR);
    bodies_it_Set_fz(&it, bodies_it_Get_fz(&it) * CONSTANT_INTERACTION_FACTOR);
  }
#endif /* #ifdef _USE_DSCAL_WITH_CONSTANT_INTERACTION_FACTOR_ */
}
#endif /* #ifdef _USE_CONSTANT_INTERACTION_FACTOR_ */




















/*********************************************************************************************
**********************************************************************************************

   Move of the bodies

**********************************************************************************************
*********************************************************************************************/


/*********************************************************************************************
   bodies_Move
*********************************************************************************************/

/* Kick Move : V+= a * dt/2  */ 
void bodies_Kick_Move(bodies_it_t * it,REAL_T dt){
  position_t force_vector ;
  VALUES_T m = bodies_it_Get_value(it);
 
  position_Set_x(&force_vector, bodies_it_Get_fx(it));
  position_Set_y(&force_vector, bodies_it_Get_fy(it));
  position_Set_z(&force_vector, bodies_it_Get_fz(it));
				
  position_Mult_by_real(&force_vector ,1/m );
				    
  position_Mult_by_real(&force_vector ,dt/2 );

  position_Add(bodies_it_Get_p_speed_vector(it),&force_vector);
}

/* Drift  Move:  X +=  V * dt */
void bodies_Drift_Move(bodies_it_t * it,REAL_T dt){
  position_t speed_vector ;

  position_Affect(&speed_vector,bodies_it_Get_p_speed_vector(it));
			       				    
  position_Mult_by_real(&speed_vector ,dt );

  bodies_it_Set_pos_x(it, bodies_it_Get_pos_x(it) + position_Get_x(&speed_vector));
  bodies_it_Set_pos_y(it, bodies_it_Get_pos_y(it) + position_Get_y(&speed_vector));
  bodies_it_Set_pos_z(it, bodies_it_Get_pos_z(it) + position_Get_z(&speed_vector));
}  












/*********************************************************************************************
**********************************************************************************************

   Direct computation

**********************************************************************************************
*********************************************************************************************/

#include "direct_computation.c"





















