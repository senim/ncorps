#ifndef __BODIES_H__
#define __BODIES_H__

#include "body.h"
#include "force_definition.h"
#include "memory_allocation.h"



#define BODIES_EXTENSION_FACTOR (6.0 / 5.0)


#define BODIES_P2P_BIT_NB 20 /* There will never be 2^20 bodies in one leaf ... */  
#define BODIES_GET_P2P_BIT_FIELD(p_b) ((p_b)->size_allocated) 



/* Nb direct interactions: */
#define NB_DIRECT_INT_MUTUAL(nb_bodies_tgt, nb_bodies_src) ((nb_bodies_tgt) * (nb_bodies_src) * 2)
#define NB_DIRECT_INT_NOMUTUAL(nb_bodies_tgt, nb_bodies_src) ((nb_bodies_tgt) * (nb_bodies_src))
/* With the mutual interaction principle, we compute sum_{i=0}^{n-2} sum_{j=i+1}^{n-1} 1 = n(n-1)/2 
 * interactions but this correspond to n(n-1) computations. */
#define NB_OWN_INT(nb_bodies) ((nb_bodies) * ((nb_bodies) - 1)) 
#define NB_OWN_INT_MAX(nb_bodies, nmax) (((nb_bodies) - 1) * (nmax))













/*********************************************************************************************
**********************************************************************************************
**********************************************************************************************

   body_array_t

**********************************************************************************************
**********************************************************************************************
**********************************************************************************************/
/* Size of 1 body = 88 bytes */
typedef struct {
  bodies_ind_t nb_bodies;
  bodies_ind_t size_allocated;  /* WARNING: we use here the bit #BODIES_P2P_BIT_NB 
				 * for P2P operations! */

/*   thread_ind_t thread_owner; /\* for debugging only *\/ */
  body_t *p_bodies;
} body_array_t;



FMB_INLINE bodies_ind_t body_array_Nb_bodies(body_array_t *p_b){
  return p_b->nb_bodies;
}


/* We must have: 0 <= body_number <= (p_b->nb_bodies - 1) */
FMB_INLINE VALUES_T body_array_Get_value(body_array_t *p_b, bodies_ind_t body_number){
  return body_Get_value(p_b->p_bodies + body_number);
}
FMB_INLINE void body_array_Set_value(body_array_t *p_b, bodies_ind_t body_number, VALUES_T value){
  body_Set_value(p_b->p_bodies + body_number, value);
}
FMB_INLINE VALUES_T *body_array_Get_p_value(body_array_t *p_b, bodies_ind_t body_number){
  return body_Get_p_value(p_b->p_bodies + body_number);
}
FMB_INLINE COORDINATES_T body_array_Get_pos_x(body_array_t *p_b, bodies_ind_t body_number){
  return position_Get_x(body_Get_p_position(p_b->p_bodies + body_number));
}
FMB_INLINE COORDINATES_T body_array_Get_pos_y(body_array_t *p_b, bodies_ind_t body_number){
  return position_Get_y(body_Get_p_position(p_b->p_bodies + body_number));
}
FMB_INLINE COORDINATES_T body_array_Get_pos_z(body_array_t *p_b, bodies_ind_t body_number){
  return position_Get_z(body_Get_p_position(p_b->p_bodies + body_number));
}
FMB_INLINE position_t *body_array_Get_p_position(body_array_t *p_b, bodies_ind_t body_number){
  return body_Get_p_position(p_b->p_bodies + body_number);
}
FMB_INLINE void body_array_Set_pos_x(body_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_pos_x){
  position_Set_x(body_Get_p_position(p_b->p_bodies + body_number), new_pos_x);
}
FMB_INLINE void body_array_Set_pos_y(body_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_pos_y){
  position_Set_y(body_Get_p_position(p_b->p_bodies + body_number), new_pos_y);
}
FMB_INLINE void body_array_Set_pos_z(body_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_pos_z){
  position_Set_z(body_Get_p_position(p_b->p_bodies + body_number), new_pos_z);
}
FMB_INLINE void body_array_Set_position(body_array_t *p_b, bodies_ind_t body_number, position_t *p_new_position){
  body_Set_position(p_b->p_bodies + body_number, p_new_position);
}
FMB_INLINE position_t *body_array_Get_p_speed_vector(body_array_t *p_b, bodies_ind_t body_number){
  return body_Get_p_speed_vector(p_b->p_bodies + body_number);
}
FMB_INLINE void body_array_Set_speed_vector(body_array_t *p_b, bodies_ind_t body_number, position_t *p_new_speed_vector){
  body_Set_speed_vector(p_b->p_bodies + body_number, p_new_speed_vector); 
}

FMB_INLINE COORDINATES_T body_array_Get_fx(body_array_t *p_b, bodies_ind_t body_number){
  return position_Get_x(body_Get_p_force_vector(p_b->p_bodies + body_number));
}
FMB_INLINE COORDINATES_T body_array_Get_fy(body_array_t *p_b, bodies_ind_t body_number){
  return position_Get_y(body_Get_p_force_vector(p_b->p_bodies + body_number));
}
FMB_INLINE COORDINATES_T body_array_Get_fz(body_array_t *p_b, bodies_ind_t body_number){
  return position_Get_z(body_Get_p_force_vector(p_b->p_bodies + body_number));
}
FMB_INLINE void body_array_Set_fx(body_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_fx){
  position_Set_x(body_Get_p_force_vector(p_b->p_bodies + body_number), new_fx);
}
FMB_INLINE void body_array_Set_fy(body_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_fy){
  position_Set_y(body_Get_p_force_vector(p_b->p_bodies + body_number), new_fy);
}
FMB_INLINE void body_array_Set_fz(body_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_fz){
  position_Set_z(body_Get_p_force_vector(p_b->p_bodies + body_number), new_fz);
}
FMB_INLINE position_t *body_array_Get_p_force_vector(body_array_t *p_b, bodies_ind_t body_number){
  return body_Get_p_force_vector(p_b->p_bodies + body_number);
}
FMB_INLINE void body_array_Set_force_vector(body_array_t *p_b, bodies_ind_t body_number, position_t *p_new_force_vector){ 
  body_Set_speed_vector(p_b->p_bodies + body_number, p_new_force_vector);
}




FMB_INLINE void body_array_Diplay_body(body_array_t *p_b,
				       bodies_ind_t body_number, 
				       FILE *f){

  display_body_fields(body_array_Get_pos_x(p_b, body_number),
		      body_array_Get_pos_y(p_b, body_number),
		      body_array_Get_pos_z(p_b, body_number),
		      body_array_Get_value(p_b, body_number),
		      body_array_Get_fx(p_b, body_number),
		      body_array_Get_fy(p_b, body_number),
		      body_array_Get_fz(p_b, body_number),
		      body_array_Get_p_speed_vector(p_b, body_number),
		      f);
}



void body_array_Initialize(body_array_t *p_b,
			   bodies_ind_t initial_size_allocated);
void body_array_Initialize_with_bodies(body_array_t *p_b,
				       body_t *p_bodies,
				       bodies_ind_t nb_bodies);

void body_array_Free(body_array_t *p_b);

void body_array_Realloc_arrays(body_array_t *p_b, bodies_ind_t new_size);

/* All the arrays are reallocated to a size of S. */
void body_array_Clear(body_array_t *p_b);

/* Fill the 'forces' and 'potential' vectors (when they exist) with 0.0 */
void body_array_ClearFP(body_array_t *p_b);

bool body_array_Is_full(body_array_t *p_b);

void body_array_Adjust_memory(body_array_t *p_b);

/*! All the arrays are copied: \n
  see the use of this function in subdivide_leaf_octree() in the file "octree.c". */
void body_array_Affect(body_array_t *p_target, body_array_t *p_src);

/*! Do a copy of each field: */
void body_array_Add(body_array_t *p_b, 
		    body_t *p_body);

/* Remove the body #body_number: */
void body_array_Remove(body_array_t *p_b, bodies_ind_t body_number);

void body_array_Display(body_array_t *p_b, FILE *f);

long body_array_Get_mem_used(body_array_t *p_b);





		     
/*********************************************************************************************
**********************************************************************************************
**********************************************************************************************

   body_array_it_t

**********************************************************************************************
**********************************************************************************************
**********************************************************************************************/

typedef struct {
  bodies_ind_t counter; /* At the beginning counter equals nb_bodies-2, 
			 * and when counter equals -1 the iterator's current body 
			 * is the last body of the arrays. */
  body_array_t *p_corresponding_body_array; /* for 'body_array_it_Remove_current()' */
  body_t *p_current_body;
} body_array_it_t;


FMB_INLINE body_array_t *body_array_it_Get_body_array(body_array_it_t *p_i){
  return p_i->p_corresponding_body_array;
}

FMB_INLINE void body_array_it_Initialize(body_array_it_t *p_i, body_array_t *p_b){
  p_i->counter = body_array_Nb_bodies(p_b)-1; /* see 'body_array_it_Is_valid()' */
  p_i->p_corresponding_body_array = p_b;
  p_i->p_current_body = p_b->p_bodies;
}

FMB_INLINE bool body_array_it_Is_valid(body_array_it_t *p_i){
  return (p_i->counter >= 0); /* This is supposed to be the quickest test. */
}

/* Returns TRUE if *p_i_1 is strictly 'lower' than *p_i_2, 
 * i.e. there has been (strictly) more calls to 'body_array_it_Go2Next()'
 * for p_i_2 than for p_i_1. */
FMB_INLINE bool body_array_it_Is_strictly_lower(body_array_it_t *p_i_1, body_array_it_t *p_i_2){
#ifdef _DEBUG_
  if (p_i_1->p_corresponding_body_array != p_i_2->p_corresponding_body_array){
    FMB_error("Error in body_array_it_Is_strictly_lower(): the two iterators do not correspond to the same 'body_array_t' variable.");
  }
#endif /* #ifdef _DEBUG_ */
  return p_i_1->counter > p_i_2->counter;
}

FMB_INLINE void body_array_it_Go2Next(body_array_it_t *p_i){
  p_i->counter--;
  p_i->p_current_body++;
}

/* We define all the "body_array_it_Go2Next_No*()" functions as "body_array_it_Go2Next() 
 * since there is no difference here. */
#define body_array_it_Go2Next_NoS(p_i) body_array_it_Go2Next(p_i)
#define body_array_it_Go2Next_NoFS(p_i) body_array_it_Go2Next(p_i)
#define body_array_it_Go2Next_NoPVS(p_i) body_array_it_Go2Next(p_i)
#define body_array_it_Go2Next_NoPFS(p_i) body_array_it_Go2Next(p_i)
#define body_array_it_Go2Next_NoPVFS(p_i) body_array_it_Go2Next(p_i)
#define body_array_it_Go2Next_NoPPS(p_i) body_array_it_Go2Next(p_i)



/***** Elementary fonctions to access the fields of the current body of the iterator: *****/
FMB_INLINE VALUES_T body_array_it_Get_value(body_array_it_t *p_i){
  return body_Get_value(p_i->p_current_body);
}
FMB_INLINE void body_array_it_Set_value(body_array_it_t *p_i, VALUES_T value){
  body_Set_value(p_i->p_current_body, value);
}
FMB_INLINE VALUES_T *body_array_it_Get_p_value(body_array_it_t *p_i){
  return body_Get_p_value(p_i->p_current_body);
}
FMB_INLINE COORDINATES_T body_array_it_Get_pos_x(body_array_it_t *p_i){
  return position_Get_x(body_Get_p_position(p_i->p_current_body));
}
FMB_INLINE COORDINATES_T body_array_it_Get_pos_y(body_array_it_t *p_i){
  return position_Get_y(body_Get_p_position(p_i->p_current_body));
}
FMB_INLINE COORDINATES_T body_array_it_Get_pos_z(body_array_it_t *p_i){
  return position_Get_z(body_Get_p_position(p_i->p_current_body));
}
FMB_INLINE position_t *body_array_it_Get_p_position(body_array_it_t *p_i){
  return body_Get_p_position(p_i->p_current_body);
}
FMB_INLINE void body_array_it_Set_pos_x(body_array_it_t *p_i, COORDINATES_T pos_x){
  position_Set_x(body_Get_p_position(p_i->p_current_body), pos_x);
}
FMB_INLINE void body_array_it_Set_pos_y(body_array_it_t *p_i, COORDINATES_T pos_y){
  position_Set_y(body_Get_p_position(p_i->p_current_body), pos_y);
}
FMB_INLINE void body_array_it_Set_pos_z(body_array_it_t *p_i, COORDINATES_T pos_z){
  position_Set_z(body_Get_p_position(p_i->p_current_body), pos_z);
}
FMB_INLINE void body_array_it_Set_position(body_array_it_t *p_i, position_t *p_new_position){
  body_Set_position(p_i->p_current_body, p_new_position);
}
FMB_INLINE position_t *body_array_it_Get_p_speed_vector(body_array_it_t *p_i){
  return body_Get_p_speed_vector(p_i->p_current_body);
}
FMB_INLINE void body_array_it_Set_speed_vector(body_array_it_t *p_i, position_t *p_new_speed_vector){
  body_Set_speed_vector(p_i->p_current_body, p_new_speed_vector); 
}

FMB_INLINE COORDINATES_T body_array_it_Get_fx(body_array_it_t *p_i){
  return position_Get_x(body_Get_p_force_vector(p_i->p_current_body));
}
FMB_INLINE COORDINATES_T body_array_it_Get_fy(body_array_it_t *p_i){
  return position_Get_y(body_Get_p_force_vector(p_i->p_current_body));
}
FMB_INLINE COORDINATES_T body_array_it_Get_fz(body_array_it_t *p_i){
  return position_Get_z(body_Get_p_force_vector(p_i->p_current_body));
}
FMB_INLINE void body_array_it_Set_fx(body_array_it_t *p_i, COORDINATES_T new_fx){ 
  position_Set_x(body_Get_p_force_vector(p_i->p_current_body), new_fx);
}
FMB_INLINE void body_array_it_Set_fy(body_array_it_t *p_i, COORDINATES_T new_fy){ 
  position_Set_y(body_Get_p_force_vector(p_i->p_current_body), new_fy);
}
FMB_INLINE void body_array_it_Set_fz(body_array_it_t *p_i, COORDINATES_T new_fz){ 
  position_Set_z(body_Get_p_force_vector(p_i->p_current_body), new_fz);
}
FMB_INLINE position_t *body_array_it_Get_p_force_vector(body_array_it_t *p_i){
  return body_Get_p_force_vector(p_i->p_current_body);
}
FMB_INLINE void body_array_it_Set_force_vector(body_array_it_t *p_i, position_t *p_new_force_vector){ 
  body_Set_force_vector(p_i->p_current_body, p_new_force_vector);
}





FMB_INLINE void body_array_it_Display_body(body_array_it_t *p_i, FILE *f){
  display_body_fields(body_array_it_Get_pos_x(p_i),
		      body_array_it_Get_pos_y(p_i),
		      body_array_it_Get_pos_z(p_i),
		      body_array_it_Get_value(p_i),
		      body_array_it_Get_fx(p_i),
		      body_array_it_Get_fy(p_i),
		      body_array_it_Get_fz(p_i),
		      body_array_it_Get_p_speed_vector(p_i),
		      f);
}



/*! Returns all the fields currently pointed by the iterator \n
  in a 'body_t' variable (*p_body_out). */
FMB_INLINE void body_array_it_Get_body(body_array_it_t *p_i, body_t *p_body_out){
  position_Affect(body_Get_p_position(p_body_out), body_array_it_Get_p_position(p_i));
  body_Set_value(p_body_out, body_array_it_Get_value(p_i));
  position_Affect(body_Get_p_force_vector(p_body_out), body_array_it_Get_p_force_vector(p_i));
  position_Affect(body_Get_p_speed_vector(p_body_out), body_array_it_Get_p_speed_vector(p_i));
}




























/*********************************************************************************************
**********************************************************************************************
**********************************************************************************************

   body_split_array_t

**********************************************************************************************
**********************************************************************************************
**********************************************************************************************/





/*! This structure is used to store (retrieve, update ...) the fields of the bodies \n
  in each cell. Each field of a body is stored in a dedicated array. */
typedef struct {
  bodies_ind_t nb_bodies;
  bodies_ind_t size_allocated;  /* WARNING: we use here the bit #BODIES_P2P_BIT_NB 
				 * for P2P operations! */
  /* Positions: */
  COORDINATES_T *p_pos_x;
  COORDINATES_T *p_pos_y;
  COORDINATES_T *p_pos_z; 

  /* Values: */
  VALUES_T *p_values;

  /* Force vectors: */
  COORDINATES_T *p_fx;
  COORDINATES_T *p_fy;
  COORDINATES_T *p_fz;

  /* Speed vectors: */
  position_t *p_speed_vectors;

} body_split_array_t;




FMB_INLINE bodies_ind_t body_split_array_Nb_bodies(body_split_array_t *p_b){
  return p_b->nb_bodies;
}

/* We must have: 0 <= body_number <= (p_b->nb_bodies - 1) */
FMB_INLINE VALUES_T body_split_array_Get_value(body_split_array_t *p_b, bodies_ind_t body_number){
  return p_b->p_values[body_number];
}
FMB_INLINE void body_split_array_Set_value(body_split_array_t *p_b, bodies_ind_t body_number, VALUES_T value){
  p_b->p_values[body_number] = value;
}
FMB_INLINE VALUES_T *body_split_array_Get_p_value(body_split_array_t *p_b, bodies_ind_t body_number){
  return p_b->p_values + body_number;
}
FMB_INLINE COORDINATES_T body_split_array_Get_pos_x(body_split_array_t *p_b, bodies_ind_t body_number){
  return p_b->p_pos_x[body_number];
}
FMB_INLINE COORDINATES_T body_split_array_Get_pos_y(body_split_array_t *p_b, bodies_ind_t body_number){
  return p_b->p_pos_y[body_number];
}
FMB_INLINE COORDINATES_T body_split_array_Get_pos_z(body_split_array_t *p_b, bodies_ind_t body_number){
  return p_b->p_pos_z[body_number];
}
FMB_INLINE void body_split_array_Set_pos_x(body_split_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_pos_x){
  p_b->p_pos_x[body_number] = new_pos_x;
}
FMB_INLINE void body_split_array_Set_pos_y(body_split_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_pos_y){
  p_b->p_pos_y[body_number] = new_pos_y;
}
FMB_INLINE void body_split_array_Set_pos_z(body_split_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_pos_z){
  p_b->p_pos_z[body_number] = new_pos_z;
}
FMB_INLINE void body_split_array_Set_position(body_split_array_t *p_b, bodies_ind_t body_number, position_t *p_new_position){
  body_split_array_Set_pos_x(p_b, body_number, position_Get_x(p_new_position));
  body_split_array_Set_pos_y(p_b, body_number, position_Get_y(p_new_position));
  body_split_array_Set_pos_z(p_b, body_number, position_Get_z(p_new_position));
}
FMB_INLINE position_t *body_split_array_Get_p_speed_vector(body_split_array_t *p_b, bodies_ind_t body_number){
  return p_b->p_speed_vectors + body_number;
}
FMB_INLINE void body_split_array_Set_speed_vector(body_split_array_t *p_b, bodies_ind_t body_number, position_t *p_new_speed_vector){
  position_Affect(p_b->p_speed_vectors + body_number, p_new_speed_vector); 
}

FMB_INLINE COORDINATES_T body_split_array_Get_fx(body_split_array_t *p_b, bodies_ind_t body_number){
  return p_b->p_fx[body_number];
}
FMB_INLINE COORDINATES_T body_split_array_Get_fy(body_split_array_t *p_b, bodies_ind_t body_number){
  return p_b->p_fy[body_number];
}
FMB_INLINE COORDINATES_T body_split_array_Get_fz(body_split_array_t *p_b, bodies_ind_t body_number){
  return p_b->p_fz[body_number];
}
FMB_INLINE void body_split_array_Set_fx(body_split_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_fx){ 
  p_b->p_fx[body_number] = new_fx; 
}
FMB_INLINE void body_split_array_Set_fy(body_split_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_fy){ 
  p_b->p_fy[body_number] = new_fy; 
}
FMB_INLINE void body_split_array_Set_fz(body_split_array_t *p_b, bodies_ind_t body_number, COORDINATES_T new_fz){ 
  p_b->p_fz[body_number] = new_fz; 
}
FMB_INLINE void body_split_array_Set_force_vector(body_split_array_t *p_b, bodies_ind_t body_number, position_t *p_new_force_vector){ 
  body_split_array_Set_fx(p_b, body_number, position_Get_x(p_new_force_vector));
  body_split_array_Set_fy(p_b, body_number, position_Get_y(p_new_force_vector));
  body_split_array_Set_fz(p_b, body_number, position_Get_z(p_new_force_vector));
}






FMB_INLINE void body_split_array_Diplay_body(body_split_array_t *p_b,
					     bodies_ind_t body_number, 
					     FILE *f){

  display_body_fields(body_split_array_Get_pos_x(p_b, body_number),
		      body_split_array_Get_pos_y(p_b, body_number),
		      body_split_array_Get_pos_z(p_b, body_number),
		      body_split_array_Get_value(p_b, body_number),
		      body_split_array_Get_fx(p_b, body_number),
		      body_split_array_Get_fy(p_b, body_number),
		      body_split_array_Get_fz(p_b, body_number),
		      body_split_array_Get_p_speed_vector(p_b, body_number),
		      f);
}





void body_split_array_Initialize(body_split_array_t *p_b,
			   bodies_ind_t initial_size_allocated);

void body_split_array_Free(body_split_array_t *p_b);

void body_split_array_Realloc_arrays(body_split_array_t *p_b, bodies_ind_t new_size);

/* All the arrays are reallocated to a size of S. */
void body_split_array_Clear(body_split_array_t *p_b);

/* Fill the 'forces' and 'potential' vectors (when they exist) with 0.0 */
void body_split_array_ClearFP(body_split_array_t *p_b);

bool body_split_array_Is_full(body_split_array_t *p_b);

void body_split_array_Adjust_memory(body_split_array_t *p_b);

/*! All the arrays are copied: \n
  see the use of this function in subdivide_leaf_octree() in the file "octree.c". */
void body_split_array_Affect(body_split_array_t *p_target, body_split_array_t *p_src);

/*! Do a copy of each field: */
void body_split_array_Add(body_split_array_t *p_b, 
			  body_t *p_body);

/* Remove the body #body_number: */
void body_split_array_Remove(body_split_array_t *p_b, bodies_ind_t body_number);

void body_split_array_Display(body_split_array_t *p_b, FILE *f);

long body_split_array_Get_mem_used(body_split_array_t *p_b);









		     
/*********************************************************************************************
**********************************************************************************************
**********************************************************************************************

   body_split_array_it_t

**********************************************************************************************
**********************************************************************************************
**********************************************************************************************/

typedef struct {
  bodies_ind_t counter; /* At the beginning counter equals nb_bodies-2, 
			 * and when counter equals -1 the iterator's current body 
			 * is the last body of the arrays. */
  body_split_array_t *p_corresponding_body_split_array; /* for 'body_split_array_it_Remove_current()' */
  /* A pointer on the current position in each array 
   * (i.e. a pointer for each field of the current body): */
  COORDINATES_T *p_current_pos_x;
  COORDINATES_T *p_current_pos_y;
  COORDINATES_T *p_current_pos_z;

  VALUES_T *p_current_value;
  COORDINATES_T *p_current_fx;
  COORDINATES_T *p_current_fy;
  COORDINATES_T *p_current_fz;
  position_t *p_current_speed_vector;
} body_split_array_it_t;



FMB_INLINE body_split_array_t *body_split_array_it_Get_body_split_array(body_split_array_it_t *p_i){
  return p_i->p_corresponding_body_split_array;
}

FMB_INLINE void body_split_array_it_Initialize(body_split_array_it_t *p_i, body_split_array_t *p_b){
  p_i->counter = body_split_array_Nb_bodies(p_b)-1; /* see 'body_split_array_it_Is_valid()' */
  p_i->p_corresponding_body_split_array = p_b;
  p_i->p_current_pos_x = p_b->p_pos_x;
  p_i->p_current_pos_y = p_b->p_pos_y;
  p_i->p_current_pos_z = p_b->p_pos_z;
  p_i->p_current_value = body_split_array_Get_p_value(p_b, 0);
  p_i->p_current_fx = p_b->p_fx;
  p_i->p_current_fy = p_b->p_fy;
  p_i->p_current_fz = p_b->p_fz;
  /* This might be useless if there is no access to *p_current_speed_vector
   * afterwards, but it is tedious to write different iterators 
   * (the best will be a C++ template class). */
  p_i->p_current_speed_vector = body_split_array_Get_p_speed_vector(p_b, 0);
}

FMB_INLINE bool body_split_array_it_Is_valid(body_split_array_it_t *p_i){
  return (p_i->counter >= 0); /* This is supposed to be the quickest test. */
}

/* Returns TRUE if *p_i_1 is strictly 'lower' than *p_i_2, 
 * i.e. there has been (strictly) more calls to 'body_split_array_it_Go2Next()'
 * for p_i_2 than for p_i_1. */
FMB_INLINE bool body_split_array_it_Is_strictly_lower(body_split_array_it_t *p_i_1, body_split_array_it_t *p_i_2){
#ifdef _DEBUG_
  if (p_i_1->p_corresponding_body_split_array != p_i_2->p_corresponding_body_split_array){
    FMB_error("Error in body_split_array_it_Is_strictly_lower(): the two iterators do not correspond to the same 'body_split_array_t' variable.");
  }
#endif /* #ifdef _DEBUG_ */
  return p_i_1->counter > p_i_2->counter;
}

FMB_INLINE void body_split_array_it_Go2Next(body_split_array_it_t *p_i){
  p_i->counter--;
  p_i->p_current_pos_x++;
  p_i->p_current_pos_y++;
  p_i->p_current_pos_z++;
  p_i->p_current_value++;
  p_i->p_current_fx++;
  p_i->p_current_fy++;
  p_i->p_current_fz++;
  p_i->p_current_speed_vector++;
}

/* The p_current_speed_vector is not incremented. 
 * It is up to the user to ensure that no access 
 * to *p_current_speed_vector will be done afterwards! */
FMB_INLINE void body_split_array_it_Go2Next_NoS(body_split_array_it_t *p_i){
  p_i->counter--;
  p_i->p_current_pos_x++;
  p_i->p_current_pos_y++;
  p_i->p_current_pos_z++;
  p_i->p_current_value++;
  p_i->p_current_fx++;
  p_i->p_current_fy++;
  p_i->p_current_fz++;
}
/* p_current_force_vector and p_current_speed_vector are not incremented. */
FMB_INLINE void body_split_array_it_Go2Next_NoFS(body_split_array_it_t *p_i){
  p_i->counter--;
  p_i->p_current_pos_x++;
  p_i->p_current_pos_y++;
  p_i->p_current_pos_z++;
  p_i->p_current_value++;
}
/* p_current_pos_{x,y,z}, p_current_value and p_current_speed_vector are not incremented. */
FMB_INLINE void body_split_array_it_Go2Next_NoPVS(body_split_array_it_t *p_i){
  p_i->counter--;
  p_i->p_current_fx++;
  p_i->p_current_fy++;
  p_i->p_current_fz++;
}
/* p_current_pos_{x,y,z}, p_current_f{x,y,z}_vector and p_current_speed_vector are not incremented. */
FMB_INLINE void body_split_array_it_Go2Next_NoPFS(body_split_array_it_t *p_i){
  p_i->counter--;
  p_i->p_current_value++;
}
/* p_current_pos_{x,y,z}, p_current_value, p_current_f{x,y,z}_vector and p_current_speed_vector are not incremented. */
FMB_INLINE void body_split_array_it_Go2Next_NoPVFS(body_split_array_it_t *p_i){
  p_i->counter--;
}
/* p_current_pos_{x,y,z}, p_current_potential and p_current_speed_vector are not incremented. */
FMB_INLINE void body_split_array_it_Go2Next_NoPPS(body_split_array_it_t *p_i){
  p_i->counter--;
  p_i->p_current_value++;
  p_i->p_current_fx++;
  p_i->p_current_fy++;
  p_i->p_current_fz++;
}


/***** Elementary fonctions to access the fields of the current body of the iterator: *****/
FMB_INLINE VALUES_T body_split_array_it_Get_value(body_split_array_it_t *p_i){
  return *(p_i->p_current_value);
}
FMB_INLINE void body_split_array_it_Set_value(body_split_array_it_t *p_i, VALUES_T value){
  *(p_i->p_current_value) = value;
}
FMB_INLINE VALUES_T *body_split_array_it_Get_p_value(body_split_array_it_t *p_i){
  return p_i->p_current_value;
}
FMB_INLINE COORDINATES_T body_split_array_it_Get_pos_x(body_split_array_it_t *p_i){
  return *(p_i->p_current_pos_x);
}
FMB_INLINE COORDINATES_T body_split_array_it_Get_pos_y(body_split_array_it_t *p_i){
  return *(p_i->p_current_pos_y);
}
FMB_INLINE COORDINATES_T body_split_array_it_Get_pos_z(body_split_array_it_t *p_i){
  return *(p_i->p_current_pos_z);
}
FMB_INLINE void body_split_array_it_Set_pos_x(body_split_array_it_t *p_i, COORDINATES_T new_pos_x){
  *(p_i->p_current_pos_x) = new_pos_x;
}
FMB_INLINE void body_split_array_it_Set_pos_y(body_split_array_it_t *p_i, COORDINATES_T new_pos_y){
  *(p_i->p_current_pos_y) = new_pos_y;
}
FMB_INLINE void body_split_array_it_Set_pos_z(body_split_array_it_t *p_i, COORDINATES_T new_pos_z){
  *(p_i->p_current_pos_z) = new_pos_z;
}
FMB_INLINE void body_split_array_it_Set_position(body_split_array_it_t *p_i, position_t *p_new_position){
  body_split_array_it_Set_pos_x(p_i, position_Get_x(p_new_position));
  body_split_array_it_Set_pos_y(p_i, position_Get_y(p_new_position));
  body_split_array_it_Set_pos_z(p_i, position_Get_z(p_new_position));
}
FMB_INLINE position_t *body_split_array_it_Get_p_speed_vector(body_split_array_it_t *p_i){
  return p_i->p_current_speed_vector;
}
FMB_INLINE void body_split_array_it_Set_speed_vector(body_split_array_it_t *p_i, position_t *p_new_speed_vector){
  position_Affect(p_i->p_current_speed_vector, p_new_speed_vector); 
}

FMB_INLINE COORDINATES_T body_split_array_it_Get_fx(body_split_array_it_t *p_i){
  return *(p_i->p_current_fx);
}
FMB_INLINE COORDINATES_T body_split_array_it_Get_fy(body_split_array_it_t *p_i){
  return *(p_i->p_current_fy);
}
FMB_INLINE COORDINATES_T body_split_array_it_Get_fz(body_split_array_it_t *p_i){
  return *(p_i->p_current_fz);
}
FMB_INLINE void body_split_array_it_Set_fx(body_split_array_it_t *p_i, COORDINATES_T new_fx){ 
  *(p_i->p_current_fx) = new_fx;
}
FMB_INLINE void body_split_array_it_Set_fy(body_split_array_it_t *p_i, COORDINATES_T new_fy){ 
  *(p_i->p_current_fy) = new_fy;
}
FMB_INLINE void body_split_array_it_Set_fz(body_split_array_it_t *p_i, COORDINATES_T new_fz){ 
  *(p_i->p_current_fz) = new_fz;
}
FMB_INLINE void body_split_array_it_Set_force_vector(body_split_array_it_t *p_i, position_t *p_new_force_vector){ 
  body_split_array_it_Set_fx(p_i, position_Get_x(p_new_force_vector));
  body_split_array_it_Set_fy(p_i, position_Get_y(p_new_force_vector));
  body_split_array_it_Set_fz(p_i, position_Get_z(p_new_force_vector));
}





FMB_INLINE void body_split_array_it_Display_body(body_split_array_it_t *p_i, FILE *f){

  display_body_fields(body_split_array_it_Get_pos_x(p_i),
		      body_split_array_it_Get_pos_y(p_i),
		      body_split_array_it_Get_pos_z(p_i),
		      body_split_array_it_Get_value(p_i),
		      body_split_array_it_Get_fx(p_i),
		      body_split_array_it_Get_fy(p_i),
		      body_split_array_it_Get_fz(p_i),
		      body_split_array_it_Get_p_speed_vector(p_i),
		      f);
}





/*! Returns all the fields currently pointed by the iterator \n
  in a 'body_t' variable (*p_body_out). */
FMB_INLINE void body_split_array_it_Get_body(body_split_array_it_t *p_i, body_t *p_body_out){

  position_Set_x(body_Get_p_position(p_body_out), body_split_array_it_Get_pos_x(p_i));
  position_Set_y(body_Get_p_position(p_body_out), body_split_array_it_Get_pos_y(p_i));
  position_Set_z(body_Get_p_position(p_body_out), body_split_array_it_Get_pos_z(p_i));

  body_Set_value(p_body_out, body_split_array_it_Get_value(p_i));
  position_Set_x(body_Get_p_force_vector(p_body_out), body_split_array_it_Get_fx(p_i));
  position_Set_y(body_Get_p_force_vector(p_body_out), body_split_array_it_Get_fy(p_i));
  position_Set_z(body_Get_p_force_vector(p_body_out), body_split_array_it_Get_fz(p_i));
  position_Affect(body_Get_p_speed_vector(p_body_out), body_split_array_it_Get_p_speed_vector(p_i));
}







































#ifdef _BODIES_SPLIT_DATA_

/*********************************************************************************************
**********************************************************************************************
**********************************************************************************************

   BODIES WITH SPLIT DATA

**********************************************************************************************
**********************************************************************************************
*********************************************************************************************/



/*********************************************************************************************
**********************************************************************************************

   bodies_t

**********************************************************************************************
*********************************************************************************************/

typedef body_split_array_t bodies_t;



#define bodies_Nb_bodies(p_b) body_split_array_Nb_bodies(p_b)
#define bodies_Get_value(p_b, body_number) body_split_array_Get_value(p_b, body_number)
#define bodies_Set_value(p_b, body_number, value) body_split_array_Set_value(p_b, body_number, value)
#define bodies_Get_p_value(p_b, body_number) body_split_array_Get_p_value(p_b, body_number)
#define bodies_Get_pos_x(p_b, body_number) body_split_array_Get_pos_x(p_b, body_number)
#define bodies_Get_pos_y(p_b, body_number) body_split_array_Get_pos_y(p_b, body_number)
#define bodies_Get_pos_z(p_b, body_number) body_split_array_Get_pos_z(p_b, body_number)
#define bodies_Set_pos_x(p_b, body_number, new_pos_x) body_split_array_Set_pos_x(p_b, body_number, new_pos_x)
#define bodies_Set_pos_y(p_b, body_number, new_pos_y) body_split_array_Set_pos_y(p_b, body_number, new_pos_y)
#define bodies_Set_pos_z(p_b, body_number, new_pos_z) body_split_array_Set_pos_z(p_b, body_number, new_pos_z)
#define bodies_Get_p_speed_vector(p_b, body_number) body_split_array_Get_p_speed_vector(p_b, body_number)
#define bodies_Set_position(p_b, body_number, p_new_position) body_array_Set_position(p_b, body_number, p_new_position)
#define bodies_Set_speed_vector(p_b, body_number, p_new_speed_vector) body_split_array_Set_speed_vector(p_b, body_number, p_new_speed_vector)
#define bodies_Get_fx(p_b, body_number) body_split_array_Get_fx(p_b, body_number)
#define bodies_Get_fy(p_b, body_number) body_split_array_Get_fy(p_b, body_number)
#define bodies_Get_fz(p_b, body_number) body_split_array_Get_fz(p_b, body_number)
#define bodies_Set_fx(p_b, body_number, new_fx) body_split_array_Set_fx(p_b, body_number, new_fx)
#define bodies_Set_fy(p_b, body_number, new_fy) body_split_array_Set_fy(p_b, body_number, new_fy)
#define bodies_Set_fz(p_b, body_number, new_fz) body_split_array_Set_fz(p_b, body_number, new_fz)
#define bodies_Set_force_vector(p_b, body_number, p_new_force_vector) body_split_array_Set_force_vector(p_b, body_number, p_new_force_vector)


#define bodies_Initialize(p_b, initial_size_allocated) body_split_array_Initialize(p_b, initial_size_allocated)
#define bodies_Free(p_b)  body_split_array_Free(p_b)
#define bodies_Realloc_arrays(p_b, new_size) body_split_array_Realloc_arrays(p_b, new_size)
#define bodies_Clear(p_b)  body_split_array_Clear(p_b)
#define bodies_ClearFP(p_b) body_split_array_ClearFP(p_b)
#define bodies_Is_full(p_b) body_split_array_Is_full(p_b)
#define bodies_Add(p_b, p_body) body_split_array_Add(p_b, p_body)
#define bodies_Adjust_memory(p_b) body_split_array_Adjust_memory(p_b)
#define bodies_Affect(p_target, p_src) body_split_array_Affect(p_target, p_src)
#define bodies_Remove(p_b, body_number) body_split_array_Remove(p_b, body_number)
#define bodies_Display(p_b, f) body_split_array_Display(p_b, f)
#define bodies_Get_mem_used(p_b) body_split_array_Get_mem_used(p_b)








#else /* #ifdef _BODIES_SPLIT_DATA_ */

/*********************************************************************************************
**********************************************************************************************
**********************************************************************************************

   BODIES WITHOUT SPLIT DATA (i.e. WITH BODY_T)

**********************************************************************************************
**********************************************************************************************
*********************************************************************************************/



/*********************************************************************************************
**********************************************************************************************

   bodies_t

**********************************************************************************************
*********************************************************************************************/

/*! This structure is used to store (retrieve, update ...) the fields of the bodies \n
  in each cell.  */
typedef body_array_t bodies_t;

#define bodies_Nb_bodies(p_b) body_array_Nb_bodies(p_b)
#define bodies_Get_value(p_b, body_number) body_array_Get_value(p_b, body_number)
#define bodies_Set_value(p_b, body_number, value) body_array_Set_value(p_b, body_number, value)
#define bodies_Get_p_value(p_b, body_number) body_array_Get_p_value(p_b, body_number)
#define bodies_Get_pos_x(p_b, body_number) body_array_Get_pos_x(p_b, body_number)
#define bodies_Get_pos_y(p_b, body_number) body_array_Get_pos_y(p_b, body_number)
#define bodies_Get_pos_z(p_b, body_number) body_array_Get_pos_z(p_b, body_number)
#define bodies_Set_pos_x(p_b, body_number, new_pos_x) body_array_Set_pos_x(p_b, body_number, new_pos_x)
#define bodies_Set_pos_y(p_b, body_number, new_pos_y) body_array_Set_pos_y(p_b, body_number, new_pos_y)
#define bodies_Set_pos_z(p_b, body_number, new_pos_z) body_array_Set_pos_z(p_b, body_number, new_pos_z)
#define bodies_Set_position(p_b, body_number, p_new_position) body_array_Set_position(p_b, body_number, p_new_position)
#define bodies_Get_p_speed_vector(p_b, body_number) body_array_Get_p_speed_vector(p_b, body_number) /* only defined for body_array_t */
#define bodies_Set_speed_vector(p_b, body_number, p_new_speed_vector) body_array_Set_speed_vector(p_b, body_number, p_new_speed_vector)
#define bodies_Get_fx(p_b, body_number) body_array_Get_fx(p_b, body_number) 
#define bodies_Get_fy(p_b, body_number) body_array_Get_fy(p_b, body_number) 
#define bodies_Get_fz(p_b, body_number) body_array_Get_fz(p_b, body_number) 
#define bodies_Set_fx(p_b, body_number, new_fx) body_array_Set_fx(p_b, body_number, new_fx) 
#define bodies_Set_fy(p_b, body_number, new_fy) body_array_Set_fy(p_b, body_number, new_fy) 
#define bodies_Set_fz(p_b, body_number, new_fz) body_array_Set_fz(p_b, body_number, new_fz) 
#define bodies_Set_force_vector(p_b, body_number, p_new_force_vector) body_array_Set_force_vector(p_b, body_number, p_new_force_vector)




#define bodies_Initialize(p_b, initial_size_allocated) body_array_Initialize(p_b, initial_size_allocated)
#define bodies_Initialize_with_bodies(p_b, p_bodies, nb_bodies) body_array_Initialize_with_bodies(p_b, p_bodies, nb_bodies)
#define bodies_Free(p_b)  body_array_Free(p_b)
#define bodies_Realloc_arrays(p_b, new_size) body_array_Realloc_arrays(p_b, new_size)
#define bodies_Clear(p_b)  body_array_Clear(p_b)
#define bodies_ClearFP(p_b) body_array_ClearFP(p_b)
#define bodies_Is_full(p_b) body_array_Is_full(p_b)
#define bodies_Add(p_b, p_body) body_array_Add(p_b, p_body)
#define bodies_Adjust_memory(p_b) body_array_Adjust_memory(p_b)
#define bodies_Affect(p_target, p_src) body_array_Affect(p_target, p_src)
#define bodies_Remove(p_b, body_number) body_array_Remove(p_b, body_number)
#define bodies_Display(p_b, f) body_array_Display(p_b, f)
#define bodies_Get_mem_used(p_b) body_array_Get_mem_used(p_b)





#endif /* #ifdef _BODIES_SPLIT_DATA_ */





/* This function is apart ... */
bodies_t *bodies_Allocate();







/*********************************************************************************************
**********************************************************************************************

   Elementary functions for 1 body (non-inlined ones):

**********************************************************************************************
*********************************************************************************************/


void bodies_Initialize_body(bodies_t *p_b, bodies_ind_t body_number);


void bodies_Display_body(bodies_t *p_b, bodies_ind_t body_number, FILE *f);

/* This could be a function of the 'position' module, but since there is no
   'position.c' file, I put it there. */
unsigned char find_corresponding_child(position_t *p_body_position,
				       position_t *p_center_position);















/*********************************************************************************************
**********************************************************************************************

   To copy 2 bodies (we provide various kinds):

**********************************************************************************************
*********************************************************************************************/


FMB_INLINE void copy_all_body_fields(COORDINATES_T *p_body_target_pos_x,
				     COORDINATES_T body_src_pos_x,
				     COORDINATES_T *p_body_target_pos_y,
				     COORDINATES_T body_src_pos_y,
				     COORDINATES_T *p_body_target_pos_z,
				     COORDINATES_T body_src_pos_z,
				     VALUES_T *p_body_target_value,
				     VALUES_T body_src_value,
				     COORDINATES_T *p_body_target_fx,
				     COORDINATES_T body_src_fx,
				     COORDINATES_T *p_body_target_fy,
				     COORDINATES_T body_src_fy,
				     COORDINATES_T *p_body_target_fz,
				     COORDINATES_T body_src_fz,
				     position_t *p_body_target_speed_vector,
				     position_t *p_body_src_speed_vector){
  /* Copy of each field: */
  *p_body_target_pos_x = body_src_pos_x;
  *p_body_target_pos_y = body_src_pos_y;
  *p_body_target_pos_z = body_src_pos_z;
  *p_body_target_value = body_src_value;
  *p_body_target_fx = body_src_fx;
  *p_body_target_fy = body_src_fy;
  *p_body_target_fz = body_src_fz;
  position_Affect(p_body_target_speed_vector, p_body_src_speed_vector);
}

FMB_INLINE void Copy_body2body_array(body_array_t *p_b_target,
				     bodies_ind_t body_target_number,
				     body_t *p_body){
  copy_all_body_fields(position_Get_p_x(body_array_Get_p_position(p_b_target, body_target_number)),
		       position_Get_x(body_Get_p_position(p_body)),
		       position_Get_p_y(body_array_Get_p_position(p_b_target, body_target_number)),
		       position_Get_y(body_Get_p_position(p_body)),
		       position_Get_p_z(body_array_Get_p_position(p_b_target, body_target_number)),
		       position_Get_z(body_Get_p_position(p_body)),
		       body_array_Get_p_value(p_b_target, body_target_number),
		       body_Get_value(p_body),
		       position_Get_p_x(body_array_Get_p_force_vector(p_b_target, body_target_number)),
		       position_Get_x(body_Get_p_force_vector(p_body)),
		       position_Get_p_y(body_array_Get_p_force_vector(p_b_target, body_target_number)),
		       position_Get_y(body_Get_p_force_vector(p_body)),
		       position_Get_p_z(body_array_Get_p_force_vector(p_b_target, body_target_number)),
		       position_Get_z(body_Get_p_force_vector(p_body)),
		       body_array_Get_p_speed_vector(p_b_target, body_target_number),
		       body_Get_p_speed_vector(p_body));
}

FMB_INLINE void Copy_body2body_split_array(body_split_array_t *p_b_target,
					   bodies_ind_t body_target_number,
					   body_t *p_body){
  copy_all_body_fields(p_b_target->p_pos_x + body_target_number,
		       position_Get_x(body_Get_p_position(p_body)),
		       p_b_target->p_pos_y + body_target_number,
		       position_Get_y(body_Get_p_position(p_body)),
		       p_b_target->p_pos_z + body_target_number,
		       position_Get_z(body_Get_p_position(p_body)),
		       body_split_array_Get_p_value(p_b_target, body_target_number),
		       body_Get_value(p_body),
		       p_b_target->p_fx + body_target_number,
		       position_Get_x(body_Get_p_force_vector(p_body)),
		       p_b_target->p_fy + body_target_number,
		       position_Get_y(body_Get_p_force_vector(p_body)),
		       p_b_target->p_fz + body_target_number,
		       position_Get_z(body_Get_p_force_vector(p_body)),
		       body_split_array_Get_p_speed_vector(p_b_target, body_target_number),
		       body_Get_p_speed_vector(p_body));
}


FMB_INLINE void Copy_body_array2body_array(body_array_t *p_b_target,
					   bodies_ind_t body_target_number,
					   body_array_t *p_b_src,
					   bodies_ind_t body_src_number){
  copy_all_body_fields(position_Get_p_x(body_array_Get_p_position(p_b_target, body_target_number)),
		       position_Get_x(body_array_Get_p_position(p_b_src, body_src_number)),
		       position_Get_p_y(body_array_Get_p_position(p_b_target, body_target_number)),
		       position_Get_y(body_array_Get_p_position(p_b_src, body_src_number)),
		       position_Get_p_z(body_array_Get_p_position(p_b_target, body_target_number)),
		       position_Get_z(body_array_Get_p_position(p_b_src, body_src_number)),
		       body_array_Get_p_value(p_b_target, body_target_number),
		       body_array_Get_value(p_b_src, body_src_number),
		       position_Get_p_x(body_array_Get_p_force_vector(p_b_target, body_target_number)),
		       position_Get_x(body_array_Get_p_force_vector(p_b_src, body_src_number)),
		       position_Get_p_y(body_array_Get_p_force_vector(p_b_target, body_target_number)),
		       position_Get_y(body_array_Get_p_force_vector(p_b_src, body_src_number)),
		       position_Get_p_z(body_array_Get_p_force_vector(p_b_target, body_target_number)),
		       position_Get_z(body_array_Get_p_force_vector(p_b_src, body_src_number)),
		       body_array_Get_p_speed_vector(p_b_target, body_target_number),
		       body_array_Get_p_speed_vector(p_b_src, body_src_number));
}

FMB_INLINE void Copy_body_split_array2body_split_array(body_split_array_t *p_b_target,
						       bodies_ind_t body_target_number,
						       body_split_array_t *p_b_src,
						       bodies_ind_t body_src_number){
  copy_all_body_fields(p_b_target->p_pos_x + body_target_number,
		       body_split_array_Get_pos_x(p_b_src, body_src_number),
		       p_b_target->p_pos_y + body_target_number,
		       body_split_array_Get_pos_y(p_b_src, body_src_number),
		       p_b_target->p_pos_z + body_target_number,
		       body_split_array_Get_pos_z(p_b_src, body_src_number),
		       body_split_array_Get_p_value(p_b_target, body_target_number),
		       body_split_array_Get_value(p_b_src, body_src_number),
		       p_b_target->p_fx + body_target_number,
		       body_split_array_Get_fx(p_b_src, body_src_number),
		       p_b_target->p_fy + body_target_number,
		       body_split_array_Get_fy(p_b_src, body_src_number),
		       p_b_target->p_fz + body_target_number,
		       body_split_array_Get_fz(p_b_src, body_src_number),
		       body_split_array_Get_p_speed_vector(p_b_target, body_target_number),
		       body_split_array_Get_p_speed_vector(p_b_src, body_src_number));
}


#ifdef _BODIES_SPLIT_DATA_
#define Copy_body2bodies(p_b_target, body_target_number, p_body) Copy_body2body_split_array(p_b_target, body_target_number, p_body)
#define Copy_bodies2bodies(p_b_target, body_target_number,  p_b_src, body_src_number) Copy_body_split_array2body_split_array(p_b_target, body_target_number,  p_b_src, body_src_number)
#else /* #ifdef _BODIES_SPLIT_DATA_ */
#define Copy_body2bodies(p_b_target, body_target_number, p_body) Copy_body2body_array(p_b_target, body_target_number, p_body)
#define Copy_bodies2bodies(p_b_target, body_target_number,  p_b_src, body_src_number) Copy_body_array2body_array(p_b_target, body_target_number,  p_b_src, body_src_number)
#endif /* #ifdef _BODIES_SPLIT_DATA_ */








































/*********************************************************************************************
**********************************************************************************************
**********************************************************************************************

   bodies_it_t (iterator for bodies_t)

**********************************************************************************************
**********************************************************************************************
*********************************************************************************************/


#ifdef _BODIES_SPLIT_DATA_ 

typedef body_split_array_it_t bodies_it_t;

#define bodies_it_Get_bodies(p_i) body_split_array_it_Get_body_split_array(p_i)
#define bodies_it_Initialize(p_i, p_b) body_split_array_it_Initialize(p_i, p_b)
#define bodies_it_Is_valid(p_i) body_split_array_it_Is_valid(p_i)
#define bodies_it_Is_strictly_lower(p_i_1, p_i_2) body_split_array_it_Is_strictly_lower(p_i_1, p_i_2)
#define bodies_it_Go2Next(p_i) body_split_array_it_Go2Next(p_i)
#define bodies_it_Go2Next_NoS(p_i) body_split_array_it_Go2Next_NoS(p_i) 
#define bodies_it_Go2Next_NoFS(p_i) body_split_array_it_Go2Next_NoFS(p_i) 
#define bodies_it_Go2Next_NoPVS(p_i) body_split_array_it_Go2Next_NoPVS(p_i) 
#define bodies_it_Go2Next_NoPFS(p_i) body_split_array_it_Go2Next_NoPFS(p_i) 
#define bodies_it_Go2Next_NoPVFS(p_i) body_split_array_it_Go2Next_NoPVFS(p_i) 
#define bodies_it_Go2Next_NoPPS(p_i) body_split_array_it_Go2Next_NoPPS(p_i) 
#define bodies_it_Get_value(p_i) body_split_array_it_Get_value(p_i)
#define bodies_it_Set_value(p_i, value) body_split_array_it_Set_value(p_i, value)
#define bodies_it_Get_p_value(p_i) body_split_array_it_Get_p_value(p_i)
#define bodies_it_Get_pos_x(p_i) body_split_array_it_Get_pos_x(p_i)
#define bodies_it_Get_pos_y(p_i) body_split_array_it_Get_pos_y(p_i)
#define bodies_it_Get_pos_z(p_i) body_split_array_it_Get_pos_z(p_i)
#define bodies_it_Set_pos_x(p_i, new_pos_x)  body_split_array_it_Set_pos_x(p_i, new_pos_x)
#define bodies_it_Set_pos_y(p_i, new_pos_y)  body_split_array_it_Set_pos_y(p_i, new_pos_y)
#define bodies_it_Set_pos_z(p_i, new_pos_z)  body_split_array_it_Set_pos_z(p_i, new_pos_z)
#define bodies_it_Set_position(p_i, p_new_position)  body_split_array_it_Set_position(p_i, p_new_position)
#define bodies_it_Get_p_speed_vector(p_i) body_split_array_it_Get_p_speed_vector(p_i)
#define bodies_it_Set_speed_vector(p_i, p_new_speed_vector) body_split_array_it_Set_speed_vector(p_i, p_new_speed_vector)
#define bodies_it_Get_fx(p_i) body_split_array_it_Get_fx(p_i)
#define bodies_it_Get_fy(p_i) body_split_array_it_Get_fy(p_i)
#define bodies_it_Get_fz(p_i) body_split_array_it_Get_fz(p_i)
#define bodies_it_Set_fx(p_i, new_fx) body_split_array_it_Set_fx(p_i, new_fx)
#define bodies_it_Set_fy(p_i, new_fy) body_split_array_it_Set_fy(p_i, new_fy)
#define bodies_it_Set_fz(p_i, new_fz) body_split_array_it_Set_fz(p_i, new_fz)
#define bodies_it_Set_force_vector(p_i, p_new_force_vector) body_split_array_it_Set_force_vector(p_i, p_new_force_vector)
#define bodies_it_Display_body(p_i, f) body_split_array_it_Display_body(p_i, f)
#define bodies_it_Get_body(p_i, p_body_out) body_split_array_it_Get_body(p_i, p_body_out)



#else /* #ifdef _BODIES_SPLIT_DATA_ */

typedef body_array_it_t bodies_it_t;

#define bodies_it_Get_bodies(p_i) body_array_it_Get_body_array(p_i)
#define bodies_it_Initialize(p_i, p_b) body_array_it_Initialize(p_i, p_b)
#define bodies_it_Is_valid(p_i) body_array_it_Is_valid(p_i)
#define bodies_it_Is_strictly_lower(p_i_1, p_i_2) body_array_it_Is_strictly_lower(p_i_1, p_i_2)
#define bodies_it_Go2Next(p_i) body_array_it_Go2Next(p_i)
#define bodies_it_Go2Next_NoS(p_i) body_array_it_Go2Next_NoS(p_i) 
#define bodies_it_Go2Next_NoFS(p_i) body_array_it_Go2Next_NoFS(p_i) 
#define bodies_it_Go2Next_NoPVS(p_i) body_array_it_Go2Next_NoPVS(p_i) 
#define bodies_it_Go2Next_NoPFS(p_i) body_array_it_Go2Next_NoPFS(p_i) 
#define bodies_it_Go2Next_NoPVFS(p_i) body_array_it_Go2Next_NoPVFS(p_i) 
#define bodies_it_Go2Next_NoPPS(p_i) body_array_it_Go2Next_NoPPS(p_i) 
#define bodies_it_Get_value(p_i) body_array_it_Get_value(p_i)
#define bodies_it_Set_value(p_i, value) body_array_it_Set_value(p_i, value)
#define bodies_it_Get_p_value(p_i) body_array_it_Get_p_value(p_i)
#define bodies_it_Get_pos_x(p_i) body_array_it_Get_pos_x(p_i)
#define bodies_it_Get_pos_y(p_i) body_array_it_Get_pos_y(p_i)
#define bodies_it_Get_pos_z(p_i) body_array_it_Get_pos_z(p_i)
#define bodies_it_Set_pos_x(p_i, new_pos_x)  body_array_it_Set_pos_x(p_i, new_pos_x)
#define bodies_it_Set_pos_y(p_i, new_pos_y)  body_array_it_Set_pos_y(p_i, new_pos_y)
#define bodies_it_Set_pos_z(p_i, new_pos_z)  body_array_it_Set_pos_z(p_i, new_pos_z)
#define bodies_it_Set_position(p_i, p_new_position)  body_array_it_Set_position(p_i, p_new_position)
#define bodies_it_Get_p_speed_vector(p_i) body_array_it_Get_p_speed_vector(p_i)
#define bodies_it_Set_speed_vector(p_i, p_new_speed_vector) body_array_it_Set_speed_vector(p_i, p_new_speed_vector)
#define bodies_it_Get_fx(p_i) body_array_it_Get_fx(p_i)
#define bodies_it_Get_fy(p_i) body_array_it_Get_fy(p_i)
#define bodies_it_Get_fz(p_i) body_array_it_Get_fz(p_i)
#define bodies_it_Set_fx(p_i, new_fx) body_array_it_Set_fx(p_i, new_fx)
#define bodies_it_Set_fy(p_i, new_fy) body_array_it_Set_fy(p_i, new_fy)
#define bodies_it_Set_fz(p_i, new_fz) body_array_it_Set_fz(p_i, new_fz)
#define bodies_it_Set_force_vector(p_i, p_new_force_vector) body_array_it_Set_force_vector(p_i, p_new_force_vector)
#define bodies_it_Display_body(p_i, f) body_array_it_Display_body(p_i, f)
#define bodies_it_Get_body(p_i, p_body_out) body_array_it_Get_body(p_i, p_body_out)


#endif /* #ifdef _BODIES_SPLIT_DATA_ */












/*! Warning: a call to this function causes the iterator to 'points' on the next body. */
FMB_INLINE void bodies_it_Remove_current(bodies_it_t *p_i){
  /* We need to access *p_corresponding_bodies
   * to update p_corresponding_bodies->nb_bodies */
  bodies_Remove(bodies_it_Get_bodies(p_i), 
		bodies_Nb_bodies(bodies_it_Get_bodies(p_i)) - 2 - p_i->counter);
  --p_i->counter;
}













/*********************************************************************************************
**********************************************************************************************

   bodies_Add_pot_and_forces

**********************************************************************************************
*********************************************************************************************/

void bodies_Add_pot_and_forces(bodies_t *p_tgt_bodies, 
			       bodies_t *p_src_bodies);





/*********************************************************************************************
**********************************************************************************************

   Scaling of the forces and the potentials with CONSTANT_INTERACTION_FACTOR

**********************************************************************************************
*********************************************************************************************/

#ifdef _USE_CONSTANT_INTERACTION_FACTOR_
/* We can also use CONSTANT_INTERACTION_FACTOR only for the total potential energy ... */
void bodies_Scale_with_CONSTANT_INTERACTION_FACTOR(bodies_t *p_b);
#endif /* #ifdef _USE_CONSTANT_INTERACTION_FACTOR_ */







/*********************************************************************************************
**********************************************************************************************

   Move of the bodies

**********************************************************************************************
*********************************************************************************************/

void bodies_Move(bodies_t *p_b);
void bodies_Kick_Move(bodies_it_t * it,REAL_T dt);
void bodies_Drift_Move(bodies_it_t * it,REAL_T dt);  











/*********************************************************************************************
**********************************************************************************************

   Direct computation

**********************************************************************************************
*********************************************************************************************/

#include "direct_computation.h"








#endif 
