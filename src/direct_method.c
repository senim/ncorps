#include "direct_method.h"
#include "IO.h" 



/* Here are the initialization of the global variables: */
bodies_t bodies;
char *Direct_data_file;
bool Direct_are_data_bzipped2 = FALSE; 
position_t center;
COORDINATES_T half_side;

FMB_Info_t FMB_Info;


/* See definition in 'FMB.c'. */
void bunzip2_file(const char *filename);
void bzip2_file(const char *filename);












/*********************************************************************************************
**********************************************************************************************

   Direct_method_Init

**********************************************************************************************
*********************************************************************************************/
void Direct_method_Init(){

  /* Checking: */
  if (f_output == NULL){
    FMB_error("'f_output' must be set.\n");
  }


  /************************************ eps_soft_square: **********************************************/
  fprintf(f_output, "Softening parameter: %.1e\n", FMB_Info.eps_soft); 
  FMB_Info.eps_soft_square = FMB_Info.eps_soft * FMB_Info.eps_soft;

  /* Clear 'center' and 'half_side': */
  position_Initialize(&center);
  half_side = (COORDINATES_T) 0.0;

}






/*********************************************************************************************
**********************************************************************************************

   Direct_method_Data

**********************************************************************************************
*********************************************************************************************/
void Direct_method_Data(char *data_file){
  bodies_ind_t k;
  bodies_ind_t nb_bodies; 

  if (INFO_DISPLAY(2)){
    fprintf(f_output, "Opening data file \'%s\' for direct computation... \n", data_file); 
  }

  /* Initialize Input operations: */    
  FMB_IO_InitI(data_file);
  
  FMB_IO_Scan_header(&nb_bodies, &center, &half_side);

  if (INFO_DISPLAY(1)){
    fprintf(f_output, "Bodies number: ");
    fprintf(f_output, FORMAT_BODIES_IND_T, nb_bodies);
    fprintf(f_output, "\n"); 
    fflush(f_output);
  }


  bodies_Initialize(&bodies, nb_bodies);

  for (k=0; k<nb_bodies ;++k){
    /* We have to use 'bodies_Add()'! */
    body_t body_tmp;
    body_Initialize(&body_tmp);

    if (FMB_IO_Scan_body(&body_tmp) != 1){
      FMB_error("In Direct_method_Data(): FMB_IO_Scan_body() failed for body #%i\n", k);
    }

/*     if (k<100){ body_Display(&body_tmp, f_output); }  */
    
    bodies_Add(&bodies, &body_tmp);
  }

  bodies_ClearFP(&bodies);

  /* Terminate Input operations: */
  FMB_IO_TerminateI();

}




/*********************************************************************************************
 ********************************************************************************************
**********************************************************************************************

 Direct_method_Data_bodies

**********************************************************************************************
*********************************************************************************************/
 /* Same as Direct_method_Data() but we use the position and values
  * of all bodies stored in 'p_b' (instead of the bodies stored
  * in the file "data_file" in Direct_method_Data()). */
void Direct_method_Data_bodies(bodies_t *p_b){
  
  bodies_it_t it;

  bodies_Initialize(&bodies, bodies_Nb_bodies(p_b));

  for (bodies_it_Initialize(&it, p_b);
       bodies_it_Is_valid(&it);
       bodies_it_Go2Next(&it)){
    /* We have to use 'bodies_Add()'! */
    body_t body_tmp;
    bodies_it_Get_body(&it, &body_tmp);
    bodies_Add(&bodies, &body_tmp);
  }

  bodies_ClearFP(&bodies);

}





/*********************************************************************************************
**********************************************************************************************

   Direct_method_Compute

**********************************************************************************************
*********************************************************************************************/
void Direct_method_Compute(){

    /********************* Without reciprocity: *******************************************/
    /* bodies_Compute_own_interaction_no_mutual() is not implemented ... */

    /********************* With reciprocity: **********************************************/
    /* Compute the force and the potential: */
    bodies_Compute_own_interaction(&bodies);        


    /**************** Possible scaling with CONSTANT_INTERACTION_FACTOR: ********************/
    /* We can also use CONSTANT_INTERACTION_FACTOR only for the total potential energy ... */
#ifdef _USE_CONSTANT_INTERACTION_FACTOR_
    bodies_Scale_with_CONSTANT_INTERACTION_FACTOR(&bodies);
#endif /* #ifdef _USE_CONSTANT_INTERACTION_FACTOR_ */


}









/*********************************************************************************************
**********************************************************************************************
************************* Move of the bodies: ************************************************

   Direct_method_Move : Leapfrog integrator ( Kick Drift Kick )  

**********************************************************************************************
*********************************************************************************************/

void KnD_Direct_method_Move(REAL_T dt ){
  /**** Kick N Drift ***/
	bodies_it_t it;
 for (bodies_it_Initialize(&it, &bodies);
      bodies_it_Is_valid(&it);
      bodies_it_Go2Next(&it)){
   bodies_Kick_Move(&it,dt);
   bodies_Drift_Move(&it,dt); 
 }
}

void K_Direct_method_Move(REAL_T dt ){
  /************************* Move of the bodies: ******************************************/
  bodies_it_t it;
  for (bodies_it_Initialize(&it, &bodies);
       bodies_it_Is_valid(&it);
       bodies_it_Go2Next(&it)){
    bodies_Kick_Move(&it,dt);
  }
}










/*********************************************************************************************
**********************************************************************************************

   Direct_method_Terminate

**********************************************************************************************
*********************************************************************************************/
void Direct_method_Terminate(){

  bodies_Free(&bodies);

  if (Direct_are_data_bzipped2){
    /* We recompress the data file: */
    bzip2_file(Direct_data_file);
  }
  FMB_free(Direct_data_file);

}



























/*********************************************************************************************
**********************************************************************************************

   sum

**********************************************************************************************
*********************************************************************************************/
void Direct_method_Sum(char *results_file,
		       unsigned long step_number_value,
		       bodies_t *p_bodies, 
		       VALUES_T total_potential_energy){

  FILE *f_results;
  position_t force_sum;
  bodies_it_t it;

  f_results = f_output;

  position_Initialize(&force_sum);
  for (bodies_it_Initialize(&it, p_bodies);
       bodies_it_Is_valid(&it);
       bodies_it_Go2Next(&it)){ 
    position_Set_x(&force_sum, position_Get_x(&force_sum) + bodies_it_Get_fx(&it));
    position_Set_y(&force_sum, position_Get_y(&force_sum) + bodies_it_Get_fy(&it));
    position_Set_z(&force_sum, position_Get_z(&force_sum) + bodies_it_Get_fz(&it));
  }
  fprintf(f_results, "Sum (force): ");
  position_Display(&force_sum, f_results, high);

  fprintf(f_results, "\n");

}








/*********************************************************************************************
**********************************************************************************************

   save 

**********************************************************************************************
*********************************************************************************************/
void Direct_method_Dump_bodies(char *results_filename,
			       unsigned long step_number_value,
			       bodies_t *p_bodies){
  bodies_it_t it;

  /* Initialize Ouput operations: */    
  FMB_IO_InitO(results_filename);
  
  if (FMB_IO_Info.output_format != NEMO_format){
    
    /********** FMB file format: **********/
    if (FMB_IO_Info.output_format == FMB_binary_format){
      FMB_error("Unable to write the 'header' for FMB_binary_format in Direct_method_Dump_bodies(). \n");
    }
    FMB_IO_Print_header(step_number_value, FALSE /* only_position_and_value */,
			bodies_Nb_bodies(p_bodies), &center, half_side);
    
    for (bodies_it_Initialize(&it, p_bodies);
	 bodies_it_Is_valid(&it);
	 bodies_it_Go2Next(&it)){ 
      
      FMB_IO_Print_body_from_bodies_it(&it, FALSE /* only_position_and_value */);
    } /* for */
    
  } /* if (FMB_IO_Info.output_format != NEMO_format) */
  else {
    /********** NEMO file format: **********/
    FMB_IO_Print_all_bodies_from_bodies_t(p_bodies);
  } /* else (FMB_IO_Info.output_format != NEMO_format) */
  
  /* Terminate Output operations: */    
  FMB_IO_TerminateO();

}














































