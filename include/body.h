#ifndef __BODY_H__
#define __BODY_H__

#include "position.h"
#include "force_definition.h"


/**********************************************************************************************
***********************************************************************************************

   BODY_T

***********************************************************************************************
**********************************************************************************************/

typedef struct {

  position_t position;

  VALUES_T v; /* mass or charge of the body */

  position_t force_vector;

  position_t speed_vector;

} body_t;










/**********************************************************************************************
***********************************************************************************************

   Elementary functions

***********************************************************************************************
**********************************************************************************************/

FMB_INLINE VALUES_T body_Get_value(FMB_CONST body_t *FMB_RESTRICT p_body){
  return p_body->v;
}
FMB_INLINE void body_Set_value(body_t *FMB_RESTRICT p_body, VALUES_T value){
  p_body->v = value;
}
FMB_INLINE VALUES_T *body_Get_p_value(body_t *FMB_RESTRICT p_body){
  return &(p_body->v);
}
FMB_INLINE position_t *body_Get_p_position(body_t *FMB_RESTRICT p_body){
  return &(p_body->position);
}
FMB_INLINE void body_Set_position(body_t *FMB_RESTRICT p_body, position_t *FMB_RESTRICT p_new_position){
  position_Affect(&(p_body->position), p_new_position);
}
FMB_INLINE position_t *body_Get_p_speed_vector(body_t *FMB_RESTRICT p_body){
  return &(p_body->speed_vector);
}
FMB_INLINE void body_Set_speed_vector(body_t *FMB_RESTRICT p_body, position_t *FMB_RESTRICT p_new_speed_vector){
  position_Affect(&(p_body->speed_vector), p_new_speed_vector); 
}

FMB_INLINE position_t *body_Get_p_force_vector(body_t *FMB_RESTRICT p_body){
  return &(p_body->force_vector);
}
FMB_INLINE void body_Set_force_vector(body_t *FMB_RESTRICT p_body, position_t *FMB_RESTRICT p_new_force_vector){ 
  position_Affect(&(p_body->force_vector), p_new_force_vector);
}










/**********************************************************************************************
***********************************************************************************************

   Miscellaneous functions

***********************************************************************************************
**********************************************************************************************/


FMB_INLINE void body_Initialize(body_t *FMB_RESTRICT p_body){
  position_Initialize(body_Get_p_position(p_body)); 
  body_Set_value(p_body, 0.0); 
  position_Initialize(body_Get_p_force_vector(p_body)); 
  position_Initialize(body_Get_p_speed_vector(p_body)); 
}


#define display_body_fields(pos_x, pos_y, pos_z, value, fx, fy, fz, p_speed_vector, f) \
  display_body_fields_noSpeedVector(pos_x, pos_y, pos_z, value, fx, fy, fz, f) 


/* Too big for inlining (and no need). */
static void display_body_fields_noSpeedVector(COORDINATES_T pos_x,
					      COORDINATES_T pos_y,
					      COORDINATES_T pos_z,
					      VALUES_T value,
					      COORDINATES_T fx,
					      COORDINATES_T fy,
					      COORDINATES_T fz,
					      FILE *f){
  fprintf(f, "*** pos= ");
  pos_xyz_Display(pos_x, pos_y, pos_z, f, low);
  /* Warning: when displaying here the speed vector the SORT_COMMANDE_FORMAT must be modified! */
  /*  fprintf(f, "\tspeed=");
      position_Display(p_speed_vector, f, high); */
  fprintf(f, "\tv= ");
  fprintf(f, LOW_PRECISION_DOUBLE_FPRINTF, value);
  fprintf(f, "\t\t\t\t\t\tforce= ");
  pos_xyz_Display(fx, fy, fz, f, high);
  fprintf(f, "\t\t\t\t\t\t\t\t\t\t\n");
}




FMB_INLINE void body_Display(body_t *FMB_RESTRICT p_body, FILE *f){
  display_body_fields(position_Get_x(body_Get_p_position(p_body)),
		      position_Get_y(body_Get_p_position(p_body)),
		      position_Get_z(body_Get_p_position(p_body)),
		      body_Get_value(p_body),
		      position_Get_x(body_Get_p_force_vector(p_body)),
		      position_Get_y(body_Get_p_force_vector(p_body)),
		      position_Get_z(body_Get_p_force_vector(p_body)),
		      body_Get_p_speed_vector(p_body),
		      f);
}



#endif
