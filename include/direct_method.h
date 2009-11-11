#ifndef __DIRECT_METHOD_H__
#define __DIRECT_METHOD_H__


#include "bodies.h"
#include "memory_allocation.h"



/***************************** Global variables: *******************************/
/* We recall here the declarations of some useful global variables. */

extern bodies_t bodies;
extern FMB_Info_t FMB_Info;







/**************************** Direct Method: ************************************/
void Direct_method_Init();

void Direct_method_Data_bodies(bodies_t *p_b);
void Direct_method_Data(char *data_file);

void Direct_method_Compute();

void Direct_method_Move();
void KnD_Direct_method_Move(REAL_T dt ) ; 
void K_Direct_method_Move(REAL_T dt ) ; 

void Direct_method_Terminate();




/**************************** Auxiliary functions: ******************************/

void Direct_method_Sum(char *results_file,
		       unsigned long step_number_value,
		       bodies_t *p_bodies, 
		       VALUES_T total_potential_energy);

void Direct_method_Dump_bodies(char *results_file,
			       unsigned long step_number_value,
			       bodies_t *p_bodies);



#endif /* #ifndef __DIRECT_METHOD_H__ */






