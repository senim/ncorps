#include "IO.h"
#include "parameters.h"

#ifdef _FMB_NO_NEMO_ 

/*** We redefine all NEMO functions and types. ***/ 

/* Types: */
typedef FILE *stream;
typedef char *string;


/* Function declarations: */
void initparam(string *argv, string *defv){
  /* 'initparam()' has to be called before NEMO I/O functions... */
  FMB_error("You have compiled with \"FMB_NO_NEMO=y\": you can not use NEMO_format...\n");
}
stream stropen(string filename, string mode){return (stream) NULL;}
void strclose(stream s){};

int get_history(stream s){return 0;}
string *ask_history(void){return (string *) NULL;}

string get_type(stream str,     /* input stream obtained from stropen */
		string tag	/* tag of item being looked for */){return (string) NULL;}
bool get_tag_ok(stream str, string tag){ return FALSE;}
void get_data(stream str, string tag, string s, void *p, int i, ...){}
void get_data_coerced(stream str, string tag, string s, void *p, int i, ...){}
void get_set(stream str, string tag){}
void get_tes(stream str, string tag ){}

void put_data(stream str, string tag, string s, void *p, int i, ...){}
void put_set(stream str, string tag){}
void put_tes(stream str, string tag){}
void put_data_sub(stream str, string tag, string s, void *p, int *pi, unsigned char c/* for 'bool' NEMO type */){}

void get_data_set(stream str, string tag, string s, int i,  ...){}
void get_data_blocked(stream str, string tag, void *p, int i){}
void get_data_tes(stream str, string tag){}

void put_data_set(stream str, string tag, string s, int i, ...){}
void put_data_blocked(stream str, string tag, void *p, int i){}
void put_data_tes(stream str, string tag){}

#else /* #ifdef _FMB_NO_NEMO_ */ 

/* There is too much problems with inclusion of 'nemo.h' 
 * (partly since we do not use 'configure' (autoconf/automake) for FMB). */
// #include <nemo.h>

/*** So we declare the functions and types ourselves. ***/ 

/* Types: */
typedef FILE *stream;
typedef char *string;


/* Function declarations: */
void initparam(string *, string *);
stream stropen(string filename, string mode);
void strclose(stream);

int get_history(stream);
string *ask_history(void);

string get_type(stream str,     /* input stream obtained from stropen */
		string tag	/* tag of item being looked for */);
bool get_tag_ok(stream, string);
void get_data(stream, string, string, void *, int, ...);
void get_data_coerced(stream, string, string, void *, int, ...);
void get_set(stream str, string tag);
void get_tes(stream str, string tag );

void put_data(stream, string, string, void *, int, ...);
void put_set(stream str, string tag);
void put_tes(stream str, string tag);
void put_data_sub(stream, string, string, void *, int *, unsigned char /* for 'bool' NEMO type */);

void get_data_set(stream, string, string, int,  ...);
void get_data_blocked(stream, string, void *, int);
void get_data_tes(stream, string);

void put_data_set(stream, string, string, int, ...);
void put_data_blocked(stream, string, void *, int);
void put_data_tes(stream, string);


#endif /* #ifdef _FMB_NO_NEMO_ */


#ifdef _FMB_NO_ZLIB_
/* Replacement functions: */
int FMB_NO_ZLIB_error(){ /* 'int' returned for compatibility with gzread and gzwrite */
  FMB_error("FMB has been compiled without 'zlib' (see FMB_NO_ZLIB).");
  return 0;
}
gzFile FMB_NO_ZLIB_error_gzopen(){ /* only for 'gzopen' */
  FMB_error("FMB has been compiled without 'zlib' (see FMB_NO_ZLIB).");
  return Z_NULL;
}
#endif 



/*************** Here are the default values of the global variables: ***************/

FILE *f_output = NULL;
FMB_IO_Info_t FMB_IO_Info = {1, /* default value for 'FMB_IO_Info.info_display_level' */
			     0};








/*********************************************************************************************
   FMB_IO_InitI 
*********************************************************************************************/
/* Initialization for input ('I'): */
void FMB_IO_InitI(char *input_data_file){

  /* Initialization of 'gz_input_buf': */
  FMB_IO_Info.gz_input_buf = NULL;
  FMB_IO_Info.gz_input_buf_len = 0;

  /* Set 'input_data_currently_used': */
  if (FMB_IO_Info.input_data_currently_used){
    FMB_error("In FMB_IO_InitI(): the input data file is already opened.\n");
  }
  else {
    FMB_IO_Info.input_data_currently_used = TRUE;
  }

  /*** Save 'input_data_file': ***/
  if (FMB_IO_Info.input_data_filename != NULL){
    /* Check this is the same filename: */
    if (strcmp(FMB_IO_Info.input_data_filename, input_data_file) != 0){ 
      FMB_ERROR_BRIEF(); 
    }
  }
  else {
    FMB_IO_Info.input_data_filename = (char *) FMB_malloc_with_check((strlen(input_data_file)+1) * sizeof(char));
    strcpy(FMB_IO_Info.input_data_filename, input_data_file);
  }

  

  /*** Open 'FMB_IO_Info.f_input_data'. ***/

  /* Check if the input data file has NEMO file format: */
  if (((strstr(input_data_file, ".nemo") != NULL) || 
       (strstr(input_data_file, ".NEMO") != NULL) ||
       (FMB_IO_Info.input_format_from_cmd_line && (FMB_IO_Info.input_format == NEMO_format)))
      && (! (FMB_IO_Info.input_format_from_cmd_line && FMB_IO_Info.input_format != NEMO_format))){
    char *dummy_defv[]={ "", NULL }; /* dummy, just so that NEMO functions work... */
    char *dummy_argv[]={ "./bin/FMB", NULL }; /* dummy, just so that NEMO functions work... */

    if (FMB_IO_Info.use_decomposed_data_files){
      FMB_error("NEMO file format not implemented with \"FMB_IO_Info.use_decomposed_data_files == TRUE\" \n");
    }

    FMB_IO_Info.input_format = NEMO_format;
    initparam(dummy_argv, dummy_defv); /* 'initparam()' has to be called before NEMO I/O functions... */

    if ((FMB_IO_Info.f_input_data = stropen(FMB_IO_Info.input_data_filename, "r")) == NULL){
      FMB_error("In FMB_IO_InitI(): stropen(FMB_IO_Info.input_data_filename = %s, \"r\")",
		FMB_IO_Info.input_data_filename); 
    }

    if ((FMB_IO_Info.f_input_data2 = stropen(FMB_IO_Info.input_data_filename, "r")) == NULL){
      FMB_error("In FMB_IO_InitI(): stropen(FMB_IO_Info.input_data_filename = %s, \"r\")",
		FMB_IO_Info.input_data_filename); 
    } 
    
    FMB_IO_Info.f_input_data_forces    = NULL;
  }
  else { /* Not the NEMO format. */
    
    if ((FMB_IO_Info.f_input_data = fopen(FMB_IO_Info.input_data_filename, "r")) == NULL){
      /* The data file might be compressed: */
      /*** zlib/gzip: we read compressed data ***/
      char *gzip_filename = (char *) FMB_malloc_with_check(strlen(FMB_IO_Info.input_data_filename) + 1 + GZIP_DECOMPRESSOR_LENGTH);
      sprintf(gzip_filename, "%s%s", FMB_IO_Info.input_data_filename, GZIP_COMPRESSION_SUFFIX);

      if ((FMB_IO_Info.f_input_gz_data = gzopen(gzip_filename, "r")) == NULL){
	/* There is no "gzipped" file. */ 
	/*** We look for a ".bz2" file and decompress it.
	 * It may be better to use the "libbzip2" here, but there is no 'gets' or 'scanf' function is this lib ... */
	FILE *bz2_f = NULL;
	char *bz2_filename = (char *) FMB_malloc_with_check(strlen(FMB_IO_Info.input_data_filename) + 1 + strlen(BZIP2_COMPRESSION_SUFFIX));
	strcpy(bz2_filename, FMB_IO_Info.input_data_filename);
	strcat(bz2_filename, BZIP2_COMPRESSION_SUFFIX);
	if ((bz2_f = fopen(bz2_filename, "r")) != NULL){ /* just to test if the file exists */
	  fclose(bz2_f);
	  bunzip2_file(FMB_IO_Info.input_data_filename); /* decompress the file */
	  if ((FMB_IO_Info.f_input_data = fopen(FMB_IO_Info.input_data_filename, "r")) == NULL){
	    FMB_perror("fopen(%s, \"r\")", FMB_IO_Info.input_data_filename);
	  }
	  FMB_IO_Info.are_input_data_bzipped2 = TRUE;
	}
	else {
	  FMB_error("Can't open file: %s{,.gz,.bz2} ", FMB_IO_Info.input_data_filename);
	}
	FMB_free(bz2_filename);
      }
      else {      
	//	fprintf(f_output, "File opened: %s\n", gzip_filename);
	FMB_IO_Info.are_input_data_gzipped = TRUE;
      }
      FMB_free(gzip_filename);
    }
    
    /* Check whether this is binary or ASCII FMB format: */
    FMB_IO_Info.FMB_binary_format_header = FMB_IO_Fread_ulong();
    /*     fprintf(f_output, "\t magic number = %lx \n", FMB_IO_Info.FMB_binary_format_header);  */
    if (FMB_IO_Info.FMB_binary_format_header == FMB_BINARY_FORMAT_MAGIC_NUMBER){
      FMB_IO_Info.input_format = FMB_binary_format;
    }
    else {
      FMB_IO_Info.input_format = FMB_ASCII_format;
	/* in this case, we rewind to the beginning of the file: */
      FMB_IO_INPUT_FSEEK(0, SEEK_SET);
    }
    
  } /* Not the NEMO format. */

  
  if (INFO_DISPLAY(2)){
    fprintf(f_output, "FMB_IO_Info.input_data_filename: %s \n", FMB_IO_Info.input_data_filename); 
    fprintf(f_output, "FMB_IO_Info.input_format: %s \n", FMB_IO_FORMAT_PRINTF_TEST(FMB_IO_Info.input_format)); 
    fprintf(f_output, "FMB_IO_Info.are_input_data_gzipped = %s \n", (FMB_IO_Info.are_input_data_gzipped ? "TRUE" : "FALSE"));
    fprintf(f_output, "FMB_IO_Info.are_input_data_bzipped2 = %s \n", (FMB_IO_Info.are_input_data_bzipped2 ? "TRUE" : "FALSE"));
  }
}





/*********************************************************************************************
   FMB_IO_TerminateI 
*********************************************************************************************/
/* Termination for input ('I'): */
void FMB_IO_TerminateI(){

  switch(FMB_IO_Info.input_format){

  case FMB_ASCII_format:
  case FMB_binary_format:
    /* Close 'FMB_IO_Info.f_input_data'. */
    if (FMB_IO_Info.are_input_data_gzipped){
      if (gzclose(FMB_IO_Info.f_input_gz_data) == Z_ERRNO /* we test here only "system" errors (not "zlib" ones) */){
	FMB_perror("gzclose(FMB_IO_Info.f_input_gz_data=%s)", FMB_IO_Info.f_input_gz_data); 
      }
      FMB_IO_Info.f_input_gz_data = NULL;
    }
    else {
      if (fclose(FMB_IO_Info.f_input_data) == EOF){ 
	FMB_error("fclose(FMB_IO_Info.f_input_data=%s)", FMB_IO_Info.f_input_data); 
      }
      FMB_IO_Info.f_input_data = NULL;
    }
    
    /* Free the buffer used for gzipped file read: */  
    if (FMB_IO_Info.gz_input_buf != NULL){
      FMB_free(FMB_IO_Info.gz_input_buf);
    FMB_IO_Info.gz_input_buf = NULL;
    }
    
    break;
    
  case NEMO_format:

    /* Close 'FMB_IO_Info.f_input_data': */
    get_data_tes((stream) FMB_IO_Info.f_input_data, "Mass");
    get_tes((stream) FMB_IO_Info.f_input_data, "Particles"); 
    get_tes((stream) FMB_IO_Info.f_input_data, "SnapShot");
    strclose((stream) FMB_IO_Info.f_input_data);
    
    /* Close 'FMB_IO_Info.f_input_data2' (and 'FMB_IO_Info.f_input_data3' if necessary): */
    if (FMB_IO_Info.use_PhaseSpace_in_input){
      get_data_tes((stream) FMB_IO_Info.f_input_data2, "PhaseSpace");
    }
    else {
      get_data_tes((stream) FMB_IO_Info.f_input_data2, "Position");
      if (FMB_IO_Info.is_Velocity_present_in_input){
	get_data_tes((stream) FMB_IO_Info.f_input_data3, "Velocity");
      }
    }
    get_tes((stream) FMB_IO_Info.f_input_data2, "Particles"); 
    get_tes((stream) FMB_IO_Info.f_input_data2, "SnapShot");
    strclose((stream) FMB_IO_Info.f_input_data2);
    if ((! FMB_IO_Info.use_PhaseSpace_in_input) && (FMB_IO_Info.is_Velocity_present_in_input)){
      get_tes((stream) FMB_IO_Info.f_input_data3, "Particles"); 
      get_tes((stream) FMB_IO_Info.f_input_data3, "SnapShot");
      strclose((stream) FMB_IO_Info.f_input_data3);
    }

    /* Close 'FMB_IO_Info.f_input_data_forces' (if necessary): */
    if (FMB_IO_Info.f_input_data_forces != NULL){
      get_data_tes((stream) FMB_IO_Info.f_input_data_forces, "Acceleration");
      get_tes((stream) FMB_IO_Info.f_input_data_forces, "Particles"); 
      get_tes((stream) FMB_IO_Info.f_input_data_forces, "SnapShot");
      strclose((stream) FMB_IO_Info.f_input_data_forces);
    }

    break;
    
  default:
    FMB_error("In FMB_IO_TerminateI(): unknow file format.\n");
  }

  /* Release 'input_data_currently_used': */
  FMB_IO_Info.input_data_currently_used = FALSE;
}





/*********************************************************************************************
   FMB_IO_InitO 
*********************************************************************************************/
/* Initialization for output ('O'): */
void FMB_IO_InitO(char *output_data_file){

  /* Set 'output_data_currently_used': */
  if (FMB_IO_Info.output_data_currently_used){
    FMB_error("In FMB_IO_InitO(): the output data file is already opened.\n");
  }
  else {
    FMB_IO_Info.output_data_currently_used = TRUE;
  }

  /* We use the same as for input data files (unless the "--ot" option has been used): */ 
  if (! FMB_IO_Info.output_format_from_cmd_line){
    FMB_IO_Info.output_format                 = FMB_IO_Info.input_format ;
  }
  if ((FMB_IO_Info.output_format == FMB_ASCII_format) || 
      (FMB_IO_Info.output_format == FMB_ASCII_human_format)){
    FMB_IO_Info.are_output_data_gzipped       = TRUE; /* default with FMB_ASCII_format and FMB_ASCII_human_format */
  }

  if (FMB_IO_Info.output_format == NEMO_format){
    FMB_IO_Info.use_PhaseSpace_in_output      = FMB_IO_Info.use_PhaseSpace_in_input ;
    FMB_IO_Info.is_Velocity_present_in_output = FMB_IO_Info.is_Velocity_present_in_input ;
  }

  /* We use 'COORDINATES_T' for 'FMB_IO_Info.is_output_coord_float': */
  if (sizeof(COORDINATES_T) == sizeof(float)){
    FMB_IO_Info.is_output_coord_float = TRUE;
  }
  else {
    if (sizeof(COORDINATES_T) == sizeof(double)){
      FMB_IO_Info.is_output_coord_float = FALSE;
    }
    else {
      FMB_ERROR_BRIEF();
    }
  }

  FMB_IO_Info.output_data_filename = (char *) FMB_malloc_with_check((strlen(output_data_file) 
								     + 1 /* terminating '\0' */ 
								     + GZIP_COMPRESSION_SUFFIX_LENGTH /* only useful if (FMB_IO_Info.are_output_data_gzipped) */
								     + NEMO_SUFFIX_LENGTH /* only for NEMO format */) * sizeof(char));
  strcpy(FMB_IO_Info.output_data_filename, output_data_file);
  
  
  /* NEMO format: */
  if (FMB_IO_Info.output_format == NEMO_format){
    strcat(FMB_IO_Info.output_data_filename, ".nemo");

    if ((FMB_IO_Info.f_output_data = stropen(FMB_IO_Info.output_data_filename, "w+")) == NULL){
      FMB_error("In FMB_IO_InitO(): stropen(FMB_IO_Info.output_data_filename = %s, \"w+\")",
		FMB_IO_Info.output_data_filename); 
    } 
  }
  else { /* Not the NEMO format. */
  
    if (! FMB_IO_Info.are_output_data_gzipped){
      if ((FMB_IO_Info.f_output_data = fopen(FMB_IO_Info.output_data_filename, "w")) == NULL)
	FMB_perror("fopen(%s, \"w\")", FMB_IO_Info.output_data_filename); 
    }
    else {
      sprintf(FMB_IO_Info.output_data_filename, "%s%s", FMB_IO_Info.output_data_filename, GZIP_COMPRESSION_SUFFIX);
      if ((FMB_IO_Info.f_output_gz_data = gzopen(FMB_IO_Info.output_data_filename, "w")) == NULL){
	FMB_perror("gzopen(%s, \"w\")", FMB_IO_Info.output_data_filename); 
      }
    }

    
    /* Check for "FMB binary format" that COORDINATES_T and VALUES_T have the same type 
     * (either float or double): */
    if (FMB_IO_Info.output_format == FMB_binary_format){
      if (! ((sizeof(COORDINATES_T) == sizeof(float) && 
	     sizeof(VALUES_T)      == sizeof(float)) ||
	     (sizeof(COORDINATES_T) == sizeof(double) && 
	      sizeof(VALUES_T)      == sizeof(double)))){
	FMB_error("In FMB_IO_InitO(): in order to write output files with \"FMB binary format\", the types 'COORDINATES_T' and 'VALUES_T' should all be 'float' or all 'double'.\n");
      }
    }    
    
  } /* Not the NEMO format */
  
  if (INFO_DISPLAY(2)){
    fprintf(f_output, "FMB_IO_Info.output_data_filename: %s \n", FMB_IO_Info.output_data_filename); 
    fprintf(f_output, "FMB_IO_Info.output_format: %s \n", FMB_IO_FORMAT_PRINTF_TEST(FMB_IO_Info.output_format)); 
    fprintf(f_output, "FMB_IO_Info.are_output_data_gzipped = %s \n", (FMB_IO_Info.are_output_data_gzipped ? "TRUE" : "FALSE"));
    fprintf(f_output, "FMB_IO_Info.are_output_data_bzipped2 = %s \n", (FMB_IO_Info.are_output_data_bzipped2 ? "TRUE" : "FALSE"));
    
    if (FMB_IO_Info.output_format == NEMO_format){
      fprintf(f_output, "FMB_IO_Info.use_PhaseSpace_in_output = %s \n", (FMB_IO_Info.use_PhaseSpace_in_output ? "TRUE" : "FALSE"));
      if (! FMB_IO_Info.use_PhaseSpace_in_output){
	fprintf(f_output, "FMB_IO_Info.is_Velocity_present_in_output = %s \n", (FMB_IO_Info.is_Velocity_present_in_output ? "TRUE" : "FALSE"));
      }
      fprintf(f_output, "FMB_IO_Info.is_output_coord_float = %s \n", (FMB_IO_Info.is_output_coord_float ? "TRUE" : "FALSE"));
    }
  } /* if (INFO_DISPLAY(2)) */
}



/*********************************************************************************************
   FMB_IO_TerminateO
*********************************************************************************************/
/* Termination for output ('O'): */
void FMB_IO_TerminateO(){

  if (FMB_IO_Info.output_format != NEMO_format){
    if (! FMB_IO_Info.are_output_data_gzipped){
      if (fclose(FMB_IO_Info.f_output_data) == EOF){ FMB_perror("fclose(FMB_IO_Info.f_output_data)"); }

      if (FMB_IO_Info.output_format == FMB_ASCII_human_format){
	/***** Sort the "results" file: *****/
	{
	  char * commande = FMB_malloc_with_check( (2*strlen(FMB_IO_Info.output_data_filename) + 30) * sizeof(char));
	  sprintf(commande, SORT_COMMANDE_FORMAT, FMB_IO_Info.output_data_filename, FMB_IO_Info.output_data_filename);
	  if (INFO_DISPLAY(1)){
	    fprintf(f_output, "Sorting the \"results file\" ... "); 
	    fflush(f_output);
	  }
	  if (system(commande) == -1)
	    FMB_perror("system(sort ...)");
	  if (INFO_DISPLAY(1)){
	    fprintf(f_output, "done.\n");
	  }
	  FMB_free(commande);
	}
      } /* if (FMB_IO_Info.output_format == FMB_ASCII_human_format) */
    } /* if (! FMB_IO_Info.are_data_gzipped) */
    else {
      if (gzclose(FMB_IO_Info.f_output_gz_data) == Z_ERRNO /* we test here only "system" errors (not "zlib" ones) */){ FMB_perror("gzclose(FMB_IO_Info.f_output_gz_data=%s)", FMB_IO_Info.f_output_gz_data); }
      /* no sorting of compressed "results" file ... */
    } /* else (! FMB_IO_Info.are_data_gzipped) */ 
  } /* if (FMB_IO_Info.output_format != NEMO_format) */

  /* Release 'output_data_currently_used': */
  FMB_IO_Info.output_data_currently_used = FALSE;

  /***** Free memory: *****/
  FMB_free(FMB_IO_Info.output_data_filename);
  FMB_IO_Info.output_data_filename = NULL;
}








/*********************************************************************************************
   FMB_IO_Set_NEMO_coord_float
*********************************************************************************************/
/* This auxiliary function is used only for NEMO snapshots in order to detect 
 * whether the NEMO file contains 'float' or 'double' numbers. */
void FMB_IO_Set_NEMO_coord_float(FILE *str /* stream */, 
				 char *tag, 
				 bool *p_coord_float /* output parameter */){
  
  char *tmp_string = get_type((stream) str, tag);
  if (strcmp("f", tmp_string) == 0){
    *p_coord_float = TRUE;
  }
  else {
    if (strcmp("d", tmp_string) == 0){
      *p_coord_float = FALSE;
    }
    else {
      FMB_error("In FMB_IO_Set_NEMO_coord_float(): unknow type (neither \"f\" nor \"d\") for stream=%s and tag=%s \n",
		str, tag);
    }
  }
  free(tmp_string); /* not allocated by 'FMB_malloc()' so I prefer to use 'free()' instead of 'FMB_free()' */
}








/*********************************************************************************************
   FMB_IO_Scan_header
*********************************************************************************************/
/* This functions sets 'FMB_Indo.nb_bodies', '*p_center' and '*p_half_side'. 
 * If 'p_center' or 'p_half_side' are NULL, we do not compute them for NEMO format 
 * (but we still have to read them from the file for FMB format 
 * (otherwise the stream is not correctly set afterwards)). */
void FMB_IO_Scan_header(bodies_ind_t *p_nb_bodies,
			position_t *p_center, 
			COORDINATES_T *p_half_side){
  bodies_ind_t nb_bodies;
  position_t center;
  COORDINATES_T half_side;

  if (! FMB_IO_Info.input_data_currently_used){
    FMB_error("In FMB_IO_Header(): FMB_IO_InitI() has not been called first...\n");
  }

  switch (FMB_IO_Info.input_format){
    
  case FMB_ASCII_format:
    
    FMB_IO_FSCANF(100 /* for safety */, "%li\n", &nb_bodies);

    FMB_IO_FSCANF(NB_BYTES__FOR__NO_PRECISION_DOUBLE_FSCANF_X4, NO_PRECISION_DOUBLE_FSCANF_X4, 
		  &half_side,
		  position_Get_p_x(&center), 
		  position_Get_p_y(&center),
		  position_Get_p_z(&center));


    /*** We now use the lowest power of 2 that encloses all particles: ***/
    half_side = FMB_POW(2.0, FMB_CEIL(FMB_LOG2(half_side))); 

    break;

  case FMB_binary_format:
    
    /* Retrieve the "FMB binary format header": */
    FMB_IO_Info.FMB_binary_format_header = FMB_IO_Fread_ulong();
    if (INFO_DISPLAY(2)){
      fprintf(f_output, "FMB binary format header info: header = %lx\n", FMB_IO_Info.FMB_binary_format_header);
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_FLOAT_DATA_BIT)){
	fprintf(f_output, "\t - data are 'float'\n");  
      }
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_POSITION_PRESENCE_BIT)){
	fprintf(f_output, "\t - 'position' is present\n");  
      }
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_VALUE_PRESENCE_BIT)){
	fprintf(f_output, "\t - 'value' is present\n");  
      }
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_POTENTIAL_PRESENCE_BIT)){
	fprintf(f_output, "\t - 'potential' is present\n");  
      }
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_FORCE_PRESENCE_BIT)){
	fprintf(f_output, "\t - 'force' is present\n");  
      }
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_SPEED_PRESENCE_BIT)){
	fprintf(f_output, "\t - 'speed' is present\n");  
      }
    }
      
    nb_bodies = FMB_IO_FREAD_BODIES_IND();

    half_side = FMB_IO_FREAD_COORDINATE(); 
    position_Set_x(&center, FMB_IO_FREAD_COORDINATE());
    position_Set_y(&center, FMB_IO_FREAD_COORDINATE());
    position_Set_z(&center, FMB_IO_FREAD_COORDINATE());


    /*** We now use the lowest power of 2 that encloses all particles: ***/
    half_side = FMB_POW(2.0, FMB_CEIL(FMB_LOG2(half_side))); 

    break;

  case NEMO_format:{
    
    /* We have here to browse the whole file to determine the position and coordinates 
     * of the computational box => this may be quite slow ...
     * Remark: in the 'treecode' for example, they do the same, but all particles are 
     * already stored in a single array (and this is therefore much faster than browsing the 
     * file).
     * For FMB, I prefer longer 'octree construction' time than greater memory requirements. */
    
    /*     double unused; */
    int nobj=0, n=0;
    COORDINATES_T x_min=0.0, y_min=0.0, z_min=0.0;
    COORDINATES_T x_max=0.0, y_max=0.0, z_max=0.0;
    COORDINATES_T x_side=0.0, y_side=0.0, z_side=0.0, side=0.0;
    
    get_history((stream) FMB_IO_Info.f_input_data);
    ask_history();
	
    get_set((stream) FMB_IO_Info.f_input_data, "SnapShot");

    get_set((stream) FMB_IO_Info.f_input_data, "Parameters");
    /*     get_data_coerced((stream) FMB_IO_Info.f_input_data, "Time", "d" /\* DoubleType *\/, &unused, 0); */
    get_data((stream) FMB_IO_Info.f_input_data, "Nobj", "i" /* IntType */, &nobj, 0);
    nb_bodies = (bodies_ind_t) nobj;
    get_tes((stream) FMB_IO_Info.f_input_data,"Parameters");



    get_set((stream) FMB_IO_Info.f_input_data, "Particles"); 
    if (get_tag_ok((stream) FMB_IO_Info.f_input_data, "PhaseSpace")){
      FMB_IO_Info.use_PhaseSpace_in_input = TRUE;
      FMB_IO_Set_NEMO_coord_float(FMB_IO_Info.f_input_data, "PhaseSpace", &FMB_IO_Info.is_input_coord_float); 
      get_data_set((stream) FMB_IO_Info.f_input_data, "PhaseSpace", (FMB_IO_Info.is_input_coord_float ? "f" : "d"), nb_bodies, 2, 3, 0); 
    }
    else {
      FMB_IO_Info.use_PhaseSpace_in_input = FALSE;
      
      /* Set 'FMB_IO_Info.f_input_data' to 'Position': */
      if (get_tag_ok((stream) FMB_IO_Info.f_input_data, "Position")){
	FMB_IO_Set_NEMO_coord_float(FMB_IO_Info.f_input_data, "Position", &FMB_IO_Info.is_input_coord_float);
	get_data_set((stream) FMB_IO_Info.f_input_data, "Position", (FMB_IO_Info.is_input_coord_float ? "f" : "d"), nb_bodies, 1, 3, 0); 
      }
      else {
	FMB_error("In FMB_IO_Header(): item \'Position\' not found.\n");
      }

      /* Set 'FMB_IO_Info.f_input_data2' to 'Velocity' if necessary: */
      if (get_tag_ok((stream) FMB_IO_Info.f_input_data2, "Velocity")){
	bool is_Velocity_float = FALSE;
	
	FMB_IO_Info.is_Velocity_present_in_input = TRUE;
	get_history((stream) FMB_IO_Info.f_input_data2);
	ask_history();
	get_set((stream) FMB_IO_Info.f_input_data2, "SnapShot");
	get_set((stream) FMB_IO_Info.f_input_data2, "Parameters");
	/* 	get_data_coerced((stream) FMB_IO_Info.f_input_data2, "Time", "d" /\* DoubleType *\/, &unused, 0); */
	get_data((stream) FMB_IO_Info.f_input_data2, "Nobj", "i" /* IntType */, &nobj, 0);
	get_tes((stream) FMB_IO_Info.f_input_data2,"Parameters");
	get_set((stream) FMB_IO_Info.f_input_data2, "Particles"); 
	
	FMB_IO_Set_NEMO_coord_float(FMB_IO_Info.f_input_data2, "Velocity", &is_Velocity_float);
	if (is_Velocity_float != FMB_IO_Info.is_input_coord_float){
	  FMB_ERROR_BRIEF(); /* for now, I assume that "Position" and "Velocity" have the same precision (float or double), 
			      * but this can be changed */
	}
	get_data_set((stream) FMB_IO_Info.f_input_data2, "Velocity", (FMB_IO_Info.is_input_coord_float ? "f" : "d"), nb_bodies, 1, 3, 0);       
      }
      else {
	FMB_IO_Info.is_Velocity_present_in_input = FALSE;
      }
    }

    if (INFO_DISPLAY(2)){
      fprintf(f_output, "FMB_IO_Info.use_PhaseSpace_in_input = %s \n", (FMB_IO_Info.use_PhaseSpace_in_input ? "TRUE" : "FALSE"));
      if (! FMB_IO_Info.use_PhaseSpace_in_input){
	fprintf(f_output, "FMB_IO_Info.is_Velocity_present_in_input = %s \n", (FMB_IO_Info.is_Velocity_present_in_input ? "TRUE" : "FALSE"));
      }
      fprintf(f_output, "FMB_IO_Info.is_input_coord_float = %s \n", (FMB_IO_Info.is_input_coord_float ? "TRUE" : "FALSE"));
    }

    
    /* Check: */
    if (sizeof(position_t) != 3 * sizeof(COORDINATES_T)){ FMB_ERROR_BRIEF(); }
      
    if ((p_center != NULL) || (p_half_side != NULL)){
      for (n=0; n<nb_bodies; ++n){ 
	float  xf = 0.0, yf = 0.0, zf = 0.0;
	double xd = 0.0, yd = 0.0, zd = 0.0;
	float  xuf = 0.0, yuf = 0.0, zuf = 0.0; /* unused (velocity) */
	double xud = 0.0, yud = 0.0, zud = 0.0; /* unused (velocity) */
	position_t pos_tmp;
	
	if (FMB_IO_Info.use_PhaseSpace_in_input){
	  if (FMB_IO_Info.is_input_coord_float){
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &xf, 1); 
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &yf, 1); 
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &zf, 1); 
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &xuf, 1);
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &yuf, 1);
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &zuf, 1);
	  }
	  else {
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &xd, 1); 
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &yd, 1); 
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &zd, 1); 
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &xud, 1);
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &yud, 1);
	    get_data_blocked((stream) FMB_IO_Info.f_input_data, "PhaseSpace", &zud, 1);
	  }
	}
	else {
	  if (FMB_IO_Info.is_input_coord_float){
	    get_data_blocked((stream) FMB_IO_Info.f_input_data,  "Position", &xf, 1); 
	    get_data_blocked((stream) FMB_IO_Info.f_input_data,  "Position", &yf, 1); 
	    get_data_blocked((stream) FMB_IO_Info.f_input_data,  "Position", &zf, 1); 
	    if (FMB_IO_Info.is_Velocity_present_in_input){
	      get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Velocity", &xuf, 1); 
	      get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Velocity", &yuf, 1); 
	      get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Velocity", &zuf, 1); 
	    }
	  }
	  else {
	    get_data_blocked((stream) FMB_IO_Info.f_input_data,  "Position", &xd, 1); 
	    get_data_blocked((stream) FMB_IO_Info.f_input_data,  "Position", &yd, 1); 
	    get_data_blocked((stream) FMB_IO_Info.f_input_data,  "Position", &zd, 1); 
	    if (FMB_IO_Info.is_Velocity_present_in_input){
	      get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Velocity", &xud, 1); 
	      get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Velocity", &yud, 1); 
	      get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Velocity", &zud, 1); 
	    }
	  }
	}

	/* Convert (if necessary) float/double to COORDINATES_T: */
	if (FMB_IO_Info.is_input_coord_float){
	  position_Set_x(&pos_tmp, (COORDINATES_T) xf);
	  position_Set_y(&pos_tmp, (COORDINATES_T) yf);
	  position_Set_z(&pos_tmp, (COORDINATES_T) zf);	  
	}
	else {
	  position_Set_x(&pos_tmp, (COORDINATES_T) xd);
	  position_Set_y(&pos_tmp, (COORDINATES_T) yd);
	  position_Set_z(&pos_tmp, (COORDINATES_T) zd);	  
	}
	

	if (n == 0){
	  x_min = position_Get_x(&pos_tmp); 
	  y_min = position_Get_y(&pos_tmp);  
	  z_min = position_Get_z(&pos_tmp); 	
	  
	  x_max = position_Get_x(&pos_tmp); 
	  y_max = position_Get_y(&pos_tmp); 
	  z_max = position_Get_z(&pos_tmp); 
	}
	else { /* if (n == 0) */ 
	  if (position_Get_x(&pos_tmp) < x_min){ x_min = position_Get_x(&pos_tmp); } 
	  if (position_Get_y(&pos_tmp) < y_min){ y_min = position_Get_y(&pos_tmp); } 
	  if (position_Get_z(&pos_tmp) < z_min){ z_min = position_Get_z(&pos_tmp); } 
	  
	  if (position_Get_x(&pos_tmp) > x_max){ x_max = position_Get_x(&pos_tmp); } 
	  if (position_Get_y(&pos_tmp) > y_max){ y_max = position_Get_y(&pos_tmp); } 
	  if (position_Get_z(&pos_tmp) > z_max){ z_max = position_Get_z(&pos_tmp); } 
	} /* else (n == 0) */
	
	
	/*** For debugging: ***/
/* 	fprintf(f_output, " pos_tmp = "); */
/* 	position_Display(&pos_tmp, f_output, high); */
/* 	fprintf(f_output, " \n"); */
/* 	fprintf(f_output, "x_min = %f, y_min = %f, z_min = %f\n", x_min, y_min, z_min); */
/* 	fprintf(f_output, "x_max = %f, y_max = %f, z_max = %f\n", x_max, y_max, z_max); */
      } /* for n */
    } /* if ((p_center != NULL) || (p_half_side != NULL)) */

    if (FMB_IO_Info.use_PhaseSpace_in_input){
      get_data_tes((stream) FMB_IO_Info.f_input_data, "PhaseSpace");
    } /* if (FMB_IO_Info.use_PhaseSpace_in_input) */
    else {
      get_data_tes((stream) FMB_IO_Info.f_input_data, "Position");
      if (FMB_IO_Info.is_Velocity_present_in_input){
	get_data_tes((stream) FMB_IO_Info.f_input_data2, "Velocity");
      }
    } /* else (FMB_IO_Info.use_PhaseSpace_in_input) */

    /* Close 'FMB_IO_Info.f_input_data': */
    get_tes((stream) FMB_IO_Info.f_input_data, "Particles"); 
    get_tes((stream) FMB_IO_Info.f_input_data, "SnapShot");
    strclose((stream) FMB_IO_Info.f_input_data);
    
    /* We reopen the "stream" for future calls to FMB_IO_Scan_body(): */
    if ((FMB_IO_Info.f_input_data = stropen(FMB_IO_Info.input_data_filename, "r")) == NULL){
      FMB_error("In FMB_IO_Scan_header(): stropen(FMB_IO_Info.input_data_filename = %s, \"r\")",
		FMB_IO_Info.input_data_filename); 
    }  

    /* Close and reopen 'FMB_IO_Info.f_input_data2' if necessary: */
    if ((! FMB_IO_Info.use_PhaseSpace_in_input) && (FMB_IO_Info.is_Velocity_present_in_input)){
      get_tes((stream) FMB_IO_Info.f_input_data2, "Particles"); 
      get_tes((stream) FMB_IO_Info.f_input_data2, "SnapShot");
      strclose((stream) FMB_IO_Info.f_input_data2);
    
      /* We reopen the "stream" for future calls to FMB_IO_Scan_body(): */
      if ((FMB_IO_Info.f_input_data2 = stropen(FMB_IO_Info.input_data_filename, "r")) == NULL){
	FMB_error("In FMB_IO_Scan_header(): stropen(FMB_IO_Info.input_data_filename = %s, \"r\")",
		  FMB_IO_Info.input_data_filename); 
      }  
    }


    if ((p_center != NULL) || (p_half_side != NULL)){
      /* We try first to round the 'x_min' and 'x_max' values only if 
       * the difference to the floor/ceil is below 1%:  */
      COORDINATES_T x_min_floor = FMB_FLOOR(x_min);
      COORDINATES_T y_min_floor = FMB_FLOOR(y_min);
      COORDINATES_T z_min_floor = FMB_FLOOR(z_min);
      COORDINATES_T x_max_ceil  = FMB_CEIL(x_max);
      COORDINATES_T y_max_ceil  = FMB_CEIL(y_max);
      COORDINATES_T z_max_ceil  = FMB_CEIL(z_max);
      if (FMB_FABS((x_min - x_min_floor) / x_min_floor) < (COORDINATES_T) 0.01){ x_min = x_min_floor ; }
      if (FMB_FABS((y_min - y_min_floor) / y_min_floor) < (COORDINATES_T) 0.01){ y_min = y_min_floor ; }
      if (FMB_FABS((z_min - z_min_floor) / z_min_floor) < (COORDINATES_T) 0.01){ z_min = z_min_floor ; }
      if (FMB_FABS((x_max_ceil - x_max) / x_max_ceil) < (COORDINATES_T) 0.01){   x_max = x_max_ceil  ; }
      if (FMB_FABS((y_max_ceil - y_max) / y_max_ceil) < (COORDINATES_T) 0.01){   y_max = y_max_ceil  ; }
      if (FMB_FABS((z_max_ceil - z_max) / z_max_ceil) < (COORDINATES_T) 0.01){   z_max = z_max_ceil  ; }

      /* Compute the side of the computational box: */
      /*       fprintf(f_output, "x_min = %f, y_min = %f, z_min = %f \n", x_min, y_min, z_min); */
      /*       fprintf(f_output, "x_max = %f, y_max = %f, z_max = %f \n", x_max, y_max, z_max); */
      x_side = x_max - x_min;
      y_side = y_max - y_min;
      z_side = z_max - z_min;
      side = MAX(y_side, z_side);
      side = MAX(side, x_side);
      /*       fprintf(f_output, "side = %f \n", side); */
      side = FMB_POW(2.0, FMB_CEIL(FMB_LOG2(side))); /* we use the lowest power of 2 that encloses all particles  */
      /*       fprintf(f_output, "side = %f \n", side); */

      /* Compute half side: */
      half_side = side * 0.5;
      
      /* Compute center coordinates: we use the average position between (x_min, y_min, z_min) and (x_max, y_max, z_max). */
      position_Set_x(&center, (x_max + x_min) / 2);
      position_Set_y(&center, (y_max + y_min) / 2);
      position_Set_z(&center, (z_max + z_min) / 2);
    }

    
    /*** We set the two (or three) streams for future calls to FMB_IO_Scan_body(): ***/
    
    /* Set stream #1 for mass: */
    get_history((stream) FMB_IO_Info.f_input_data);
    ask_history();
    get_set((stream) FMB_IO_Info.f_input_data, "SnapShot");
    get_set((stream) FMB_IO_Info.f_input_data, "Parameters");
    /*     get_data_coerced((stream) FMB_IO_Info.f_input_data, "Time", "d" /\* DoubleType *\/, &unused, 0); */
    get_data((stream) FMB_IO_Info.f_input_data, "Nobj", "i" /* IntType */, &nobj, 0);
    get_tes((stream) FMB_IO_Info.f_input_data,"Parameters");
    get_set((stream) FMB_IO_Info.f_input_data, "Particles"); 
    { /* Check for precision used for "Mass": */
      bool is_Mass_float = FALSE;
      FMB_IO_Set_NEMO_coord_float(FMB_IO_Info.f_input_data, "Mass", &is_Mass_float);
      if (is_Mass_float != FMB_IO_Info.is_input_coord_float){
	FMB_ERROR_BRIEF(); /* for now, I assume that "Mass" has the same precision (float or double) 
			    * as "PhaseSpace" (or "Position" and "Velocity"), but this can be changed */
      }
    }
    get_data_set((stream) FMB_IO_Info.f_input_data, "Mass", (FMB_IO_Info.is_input_coord_float ? "f" : "d"), nb_bodies, 0);

    
    /* Set stream #2 for PhaseSpace (or stream #2 and #3 for Position and Velocity): */
    get_history((stream) FMB_IO_Info.f_input_data2);
    ask_history();
    get_set((stream) FMB_IO_Info.f_input_data2, "SnapShot");
    get_set((stream) FMB_IO_Info.f_input_data2, "Parameters");
    /*     get_data_coerced((stream) FMB_IO_Info.f_input_data2, "Time", "d" /\* DoubleType *\/, &unused, 0); */
    get_data((stream) FMB_IO_Info.f_input_data2, "Nobj", "i" /* IntType */, &nobj, 0);
    get_tes((stream) FMB_IO_Info.f_input_data2,"Parameters");
    get_set((stream) FMB_IO_Info.f_input_data2, "Particles"); 
    
    if (FMB_IO_Info.use_PhaseSpace_in_input){
      get_data_set((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", (FMB_IO_Info.is_input_coord_float ? "f" : "d"), nb_bodies, 2, 3, 0); 
    }
    else {
      /* Set 'FMB_IO_Info.f_input_data2' to 'Position': */
      get_data_set((stream) FMB_IO_Info.f_input_data2, "Position", (FMB_IO_Info.is_input_coord_float ? "f" : "d"), nb_bodies, 1, 3, 0); 
      
      /* Open and set 'FMB_IO_Info.f_input_data3' to 'Velocity': */
      if (FMB_IO_Info.is_Velocity_present_in_input){
	if ((FMB_IO_Info.f_input_data3 = stropen(FMB_IO_Info.input_data_filename, "r")) == NULL){
	  FMB_error("In FMB_IO_Scan_header(): stropen(FMB_IO_Info.input_data_filename = %s, \"r\")",
		    FMB_IO_Info.input_data_filename); 
	}        
	get_history((stream) FMB_IO_Info.f_input_data3);
	ask_history();
	get_set((stream) FMB_IO_Info.f_input_data3, "SnapShot");
	get_set((stream) FMB_IO_Info.f_input_data3, "Parameters");
	/* 	get_data_coerced((stream) FMB_IO_Info.f_input_data3, "Time", "d" /\* DoubleType *\/, &unused, 0); */
	get_data((stream) FMB_IO_Info.f_input_data3, "Nobj", "i" /* IntType */, &nobj, 0);
	get_tes((stream) FMB_IO_Info.f_input_data3,"Parameters");
	get_set((stream) FMB_IO_Info.f_input_data3, "Particles"); 
	get_data_set((stream) FMB_IO_Info.f_input_data3, "Velocity", (FMB_IO_Info.is_input_coord_float ? "f" : "d"), nb_bodies, 1, 3, 0);       
      }
    }

#if 0 
    /* Set stream for forces (accelerations) (if they are present): */
    if ((FMB_IO_Info.f_input_data_forces = stropen(FMB_IO_Info.input_data_filename, "r")) == NULL){
      FMB_error("In FMB_IO_Scan_header(): stropen(FMB_IO_Info.input_data_filename = %s, \"r\")",
		FMB_IO_Info.input_data_filename); 
    }  
    get_history((stream) FMB_IO_Info.f_input_data_forces);
    ask_history();
    get_set((stream) FMB_IO_Info.f_input_data_forces, "SnapShot");
    get_set((stream) FMB_IO_Info.f_input_data_forces, "Particles"); 
    if (get_tag_ok((stream) FMB_IO_Info.f_input_data_forces, "Acceleration")){
      get_data_set((stream) FMB_IO_Info.f_input_data_forces, "Acceleration", (FMB_IO_Info.is_input_coord_float ? "f" : "d"), nb_bodies, 1, 3, 0);       
      if (INFO_DISPLAY(2)){
	fprintf(f_output, " 'Acceleration' found in snapshot.\n");
      }
    } 
    else {
      get_tes((stream) FMB_IO_Info.f_input_data_forces, "Particles"); 
      get_tes((stream) FMB_IO_Info.f_input_data_forces, "SnapShot");
      strclose((stream) FMB_IO_Info.f_input_data_forces);
      FMB_IO_Info.f_input_data_forces = NULL;
    }
#endif 

    break;
  }
  default:
    FMB_error("In FMB_IO_Header(): unknow file format.\n");
  }

  if (INFO_DISPLAY(2)){
    fprintf(f_output, " FMB_IO_Header(): half_side = %e \n", half_side);     
    fprintf(f_output, " FMB_IO_Header(): center = ");
    position_Display(&center, f_output, high);
    fprintf(f_output, " \n");
  }

  /* Set 'out' parameters: */
  *p_nb_bodies = nb_bodies;
  if (p_half_side != NULL){
    *p_half_side = half_side;
  }
  if (p_center != NULL){
    position_Set_x(p_center, position_Get_x(&center));
    position_Set_y(p_center, position_Get_y(&center));
    position_Set_z(p_center, position_Get_z(&center));
  }
}




/*********************************************************************************************
   FMB_IO_Print_header
*********************************************************************************************/
void FMB_IO_Print_header(unsigned long step_counter /* only for FMB_ASCII_human_format, 
						     * but not very useful in fact... */,
			 bool only_position_and_value /* for FMB_binary_format header */,
			 bodies_ind_t nb_bodies,
			 position_t *p_center, 
			 COORDINATES_T half_side){
  
  if (! FMB_IO_Info.output_data_currently_used){
    FMB_error("In FMB_IO_Print_position(): FMB_IO_InitO() has not been called first.\n");
  }
  
  switch (FMB_IO_Info.output_format){
    
  case FMB_ASCII_format:
    /* Write 'nb_bodies': */
    FMB_IO_FPRINTF("%li\n", nb_bodies); 
    
    /* Write 'half_side' and octree center position: */
    FMB_IO_FPRINTF(LOW_PRECISION_DOUBLE_FPRINTF, half_side);
    FMB_IO_FPRINTF("\t");
    FMB_IO_Print_position(p_center, low);    
    FMB_IO_FPRINTF("\n");
    break;

  case FMB_ASCII_human_format:
    FMB_IO_FPRINTF("Step #%lu\n", step_counter);

    /* Write 'nb_bodies': */
    FMB_IO_FPRINTF("Nb bodies: %li\n", nb_bodies); 

    /* Write 'half_side' and octree center position: */
    FMB_IO_FPRINTF("Octree half side: ");
    FMB_IO_FPRINTF(LOW_PRECISION_DOUBLE_FPRINTF, half_side);
    FMB_IO_FPRINTF("\n");
    FMB_IO_FPRINTF("Octree center: ");
    FMB_IO_Print_position(p_center, low);    
    FMB_IO_FPRINTF("\n");
    break;
    
  case FMB_binary_format: {
    unsigned long header = 0;
    unsigned long magic_number = FMB_BINARY_FORMAT_MAGIC_NUMBER;

    /* Write the magic number: */
    /*     fprintf(f_output, "\t magic number = %lx \n", magic_number); */
    FMB_IO_Fwrite_ulong(magic_number);
    
    /* Set and write the header: */
    if (sizeof(COORDINATES_T) == sizeof(float)){
      header |= FMB_BINARY_FORMAT_HEADER_FLOAT_DATA_BIT;
    }
    if (only_position_and_value){
	header |= FMB_BINARY_FORMAT_HEADER_VALUE_PRESENCE_BIT;
	header |= FMB_BINARY_FORMAT_HEADER_POSITION_PRESENCE_BIT;
	FMB_IO_Info.FMB_binary_format_header = header;
    }
    else {
      if (FMB_IO_Info.input_format == FMB_binary_format){
	/* By default, we use the same "FMB binary format header" than 
	 * in the input file (if the input file is in FMB binary format of course...). */
	header = FMB_IO_Info.FMB_binary_format_header;
      }
      else {
	/* Else, we write all fields except the "speed vector". */
	header |= FMB_BINARY_FORMAT_HEADER_VALUE_PRESENCE_BIT;
	header |= FMB_BINARY_FORMAT_HEADER_POSITION_PRESENCE_BIT;
	header |= FMB_BINARY_FORMAT_HEADER_FORCE_PRESENCE_BIT;
	/*    header |= FMB_BINARY_FORMAT_HEADER_SPEED_PRESENCE_BIT; */
	FMB_IO_Info.FMB_binary_format_header = header;
      }
    }
    //    fprintf(f_output, "FMB binary format header = %lx\n", header);
    FMB_IO_Fwrite_ulong(header);
    
    /* Write 'nb_bodies': */
    FMB_IO_FWRITE_BODIES_IND(nb_bodies); 

    /* Write 'half_side' and octree center position: */
    FMB_IO_FWRITE_COORDINATE(half_side);
    FMB_IO_FWRITE_COORDINATE(position_Get_x(p_center));
    FMB_IO_FWRITE_COORDINATE(position_Get_y(p_center));
    FMB_IO_FWRITE_COORDINATE(position_Get_z(p_center));
    break;
  }
    
  case NEMO_format:
    FMB_error("FMB_IO_Print_header() not implemented for NEMO format.\n");
    break;
 
  default:
    FMB_error("In FMB_IO_Print_header(): unknow file format.\n");
  }  

}






/*********************************************************************************************
   FMB_IO_Scan_body
*********************************************************************************************/
/* Get next body from the input data file and store it in '*p_body'.
 * We return '1' in case of success, 'O' if there was no more body to read (=> not done yet...). */ 
int FMB_IO_Scan_body(body_t *p_body){

  if (! FMB_IO_Info.input_data_currently_used){
    FMB_error("In FMB_IO_Scan_body(): FMB_IO_InitI() has not been called first.\n");
  }

  switch (FMB_IO_Info.input_format){
    
  case FMB_ASCII_format:

      FMB_IO_FSCANF(NB_BYTES__FOR__NO_PRECISION_DOUBLE_FSCANF_X4, NO_PRECISION_DOUBLE_FSCANF_X4, 
		      position_Get_p_x(body_Get_p_position(p_body)), 
		      position_Get_p_y(body_Get_p_position(p_body)), 
		      position_Get_p_z(body_Get_p_position(p_body)), 
		      body_Get_p_value(p_body));
      break;

  case FMB_binary_format:

    if ((! FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, 
						   FMB_BINARY_FORMAT_HEADER_POSITION_PRESENCE_BIT))
	|| (! FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, 
						      FMB_BINARY_FORMAT_HEADER_VALUE_PRESENCE_BIT))){
      FMB_error("In FMB_IO_Scan_body(): 'position' or 'value' is not present...");
    }

	
    if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_FLOAT_DATA_BIT)){
      /*** The data in the input file are 'float'. ***/
      float  massf = 0.0; /* mass */
      float  xf = 0.0, yf = 0.0, zf = 0.0;    /* position */
      float unused;

      /* IMPORTANT: for now, we only read 'position' and 'value' from the input data file. */
      xf = FMB_IO_Fread_float(); 
      yf = FMB_IO_Fread_float();
      zf = FMB_IO_Fread_float();
      massf = FMB_IO_Fread_float();
      
      /* Skip the other field, if required: */
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, 
						  FMB_BINARY_FORMAT_HEADER_POTENTIAL_PRESENCE_BIT)){
	unused = FMB_IO_Fread_float(); /* skip potential */
      }
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, 
						  FMB_BINARY_FORMAT_HEADER_FORCE_PRESENCE_BIT)){
	unused = FMB_IO_Fread_float(); /* skip force vector */
	unused = FMB_IO_Fread_float();
	unused = FMB_IO_Fread_float();
      }
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, 
						  FMB_BINARY_FORMAT_HEADER_SPEED_PRESENCE_BIT)){
	unused = FMB_IO_Fread_float(); /* skip speed vector */
	unused = FMB_IO_Fread_float();
	unused = FMB_IO_Fread_float();
      }

    /* Convert (if necessary) float/double to COORDINATES_T: */
      body_Set_value(p_body, (VALUES_T) massf);      
      position_Set_x(body_Get_p_position(p_body), (COORDINATES_T) xf);	
      position_Set_y(body_Get_p_position(p_body), (COORDINATES_T) yf);
      position_Set_z(body_Get_p_position(p_body), (COORDINATES_T) zf);
    }
    else {
      /*** The data in the input file are 'double'. ***/
      double massd = 0.0; /* mass */
      double xd = 0.0, yd = 0.0, zd = 0.0;    /* position */
      double unused;

      /* IMPORTANT: for now, we only read 'position' and 'value' from the input data file. */
      xd = FMB_IO_Fread_double(); 
      yd = FMB_IO_Fread_double();
      zd = FMB_IO_Fread_double();
      massd = FMB_IO_Fread_double();
      
      /* Skip the other field, if required: */
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, 
						  FMB_BINARY_FORMAT_HEADER_POTENTIAL_PRESENCE_BIT)){
	unused = FMB_IO_Fread_double(); /* skip potential */
      }
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, 
						  FMB_BINARY_FORMAT_HEADER_FORCE_PRESENCE_BIT)){
	unused = FMB_IO_Fread_double(); /* skip force vector */
	unused = FMB_IO_Fread_double();
	unused = FMB_IO_Fread_double();
      }
      if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, 
						  FMB_BINARY_FORMAT_HEADER_SPEED_PRESENCE_BIT)){
	unused = FMB_IO_Fread_double(); /* skip speed vector */
	unused = FMB_IO_Fread_double();
	unused = FMB_IO_Fread_double();
      }
      
    /* Convert (if necessary) float/double to COORDINATES_T: */
      body_Set_value(p_body, (VALUES_T) massd);      
      position_Set_x(body_Get_p_position(p_body), (COORDINATES_T) xd);	
      position_Set_y(body_Get_p_position(p_body), (COORDINATES_T) yd);
      position_Set_z(body_Get_p_position(p_body), (COORDINATES_T) zd);
    }
    
    break;

  case NEMO_format: {
    float  massf = 0.0; /* mass */
    double massd = 0.0; /* mass */
    float  xf = 0.0, yf = 0.0, zf = 0.0;    /* position */
    double xd = 0.0, yd = 0.0, zd = 0.0;    /* position */
    float  vxf = 0.0, vyf = 0.0, vzf = 0.0; /* velocity */
    double vxd = 0.0, vyd = 0.0, vzd = 0.0; /* velocity */

    /* Retrieve mass: */
    if (FMB_IO_Info.is_input_coord_float){
      get_data_blocked((stream) FMB_IO_Info.f_input_data, "Mass", &massf, 1); 
      body_Set_value(p_body, (VALUES_T) massf);      
    }
    else {
      get_data_blocked((stream) FMB_IO_Info.f_input_data, "Mass", &massd, 1); 
      body_Set_value(p_body, (VALUES_T) massd);      
    }

    /* Retrieve PhaseSpace (or position and velocity): */
    if (FMB_IO_Info.use_PhaseSpace_in_input){
      if (FMB_IO_Info.is_input_coord_float){
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &xf, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &yf, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &zf, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &vxf, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &vyf, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &vzf, 1); 
      }
      else {
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &xd, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &yd, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &zd, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &vxd, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &vyd, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "PhaseSpace", &vzd, 1); 
      }
    }
    else {
      if (FMB_IO_Info.is_input_coord_float){
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Position", &xf, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Position", &yf, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Position", &zf, 1); 
	if (FMB_IO_Info.is_Velocity_present_in_input){
	  get_data_blocked((stream) FMB_IO_Info.f_input_data3, "Velocity", &vxf, 1); 
	  get_data_blocked((stream) FMB_IO_Info.f_input_data3, "Velocity", &vyf, 1); 
	  get_data_blocked((stream) FMB_IO_Info.f_input_data3, "Velocity", &vzf, 1); 
	}
      }
      else {
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Position", &xd, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Position", &yd, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data2, "Position", &zd, 1); 
	if (FMB_IO_Info.is_Velocity_present_in_input){
	  get_data_blocked((stream) FMB_IO_Info.f_input_data3, "Velocity", &vxd, 1); 
	  get_data_blocked((stream) FMB_IO_Info.f_input_data3, "Velocity", &vyd, 1); 
	  get_data_blocked((stream) FMB_IO_Info.f_input_data3, "Velocity", &vzd, 1); 
	}
      }
    }
    
    /* Convert (if necessary) float/double to COORDINATES_T: */
    if (FMB_IO_Info.is_input_coord_float){
      position_Set_x(body_Get_p_position(p_body), (COORDINATES_T) xf);	
      position_Set_y(body_Get_p_position(p_body), (COORDINATES_T) yf);
      position_Set_z(body_Get_p_position(p_body), (COORDINATES_T) zf);
      position_Set_x(body_Get_p_speed_vector(p_body), (COORDINATES_T) vxf);	
      position_Set_y(body_Get_p_speed_vector(p_body), (COORDINATES_T) vyf);
      position_Set_z(body_Get_p_speed_vector(p_body), (COORDINATES_T) vzf);
    }
    else {
      position_Set_x(body_Get_p_position(p_body), (COORDINATES_T) xd);	
      position_Set_y(body_Get_p_position(p_body), (COORDINATES_T) yd);
      position_Set_z(body_Get_p_position(p_body), (COORDINATES_T) zd);
      position_Set_x(body_Get_p_speed_vector(p_body), (COORDINATES_T) vxd);	
      position_Set_y(body_Get_p_speed_vector(p_body), (COORDINATES_T) vyd);
      position_Set_z(body_Get_p_speed_vector(p_body), (COORDINATES_T) vzd);
    }
    
    /* Retrieve Forces (if they are present): */
    if (FMB_IO_Info.f_input_data_forces != NULL){
      float  xf = 0.0, yf = 0.0, zf = 0.0;    
      double xd = 0.0, yd = 0.0, zd = 0.0;    

      if (FMB_IO_Info.is_input_coord_float){
	get_data_blocked((stream) FMB_IO_Info.f_input_data_forces, "Acceleration", &xf, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data_forces, "Acceleration", &yf, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data_forces, "Acceleration", &zf, 1); 

	/* Convert (if necessary) float/double to COORDINATES_T: */
	position_Set_x(body_Get_p_force_vector(p_body), (COORDINATES_T) xf * body_Get_value(p_body));	
	position_Set_y(body_Get_p_force_vector(p_body), (COORDINATES_T) yf * body_Get_value(p_body));
	position_Set_z(body_Get_p_force_vector(p_body), (COORDINATES_T) zf * body_Get_value(p_body));
      }
      else {
	get_data_blocked((stream) FMB_IO_Info.f_input_data_forces, "Acceleration", &xd, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data_forces, "Acceleration", &yd, 1); 
	get_data_blocked((stream) FMB_IO_Info.f_input_data_forces, "Acceleration", &zd, 1); 
	
	/* Convert (if necessary) float/double to COORDINATES_T: */
	position_Set_x(body_Get_p_force_vector(p_body), (COORDINATES_T) xd * body_Get_value(p_body));	
	position_Set_y(body_Get_p_force_vector(p_body), (COORDINATES_T) yd * body_Get_value(p_body));
	position_Set_z(body_Get_p_force_vector(p_body), (COORDINATES_T) zd * body_Get_value(p_body));
      }
    }

    break;
  }
  default:
    FMB_error("In FMB_IO_Scan_body(): unknow file format.\n");
  }

  return 1; 
}






/*********************************************************************************************
   FMB_IO_Print_position
*********************************************************************************************/
/* Write the position in the output data file. */ 
void FMB_IO_Print_position(position_t *FMB_RESTRICT p,  
			   precision_double_t prec /* only for 'FMB_ASCII_format'
						    * and 'FMB_ASCII_human_format' */){
  FMB_IO_Print_pos_xyz(position_Get_x(p),
		       position_Get_y(p),
		       position_Get_z(p),
		       prec);

}




/*********************************************************************************************
   FMB_IO_Print_pos_xyz
*********************************************************************************************/
/* Write the position in the output data file. */ 
void FMB_IO_Print_pos_xyz(COORDINATES_T pos_x,
			  COORDINATES_T pos_y,
			  COORDINATES_T pos_z,
			  precision_double_t prec /* only for 'FMB_ASCII_format'
						    * and 'FMB_ASCII_human_format' */){

  if (! FMB_IO_Info.output_data_currently_used){
    FMB_error("In FMB_IO_Print_position(): FMB_IO_InitO() has not been called first.\n");
  }

  switch (FMB_IO_Info.output_format){
    
  case FMB_ASCII_format: 
    
    if (prec == low){
      FMB_IO_FPRINTF(LOW_PRECISION_DOUBLE_FPRINTF, pos_x);
      FMB_IO_FPRINTF("\t");
      FMB_IO_FPRINTF(LOW_PRECISION_DOUBLE_FPRINTF, pos_y);
      FMB_IO_FPRINTF("\t");
      FMB_IO_FPRINTF(LOW_PRECISION_DOUBLE_FPRINTF, pos_z);
    }
    else {
      FMB_IO_FPRINTF(HIGH_PRECISION_DOUBLE_FPRINTF, pos_x);
      FMB_IO_FPRINTF("\t");
      FMB_IO_FPRINTF(HIGH_PRECISION_DOUBLE_FPRINTF, pos_y);
      FMB_IO_FPRINTF("\t");
      FMB_IO_FPRINTF(HIGH_PRECISION_DOUBLE_FPRINTF, pos_z);
    }
  
    break;

  case FMB_ASCII_human_format: 
    FMB_IO_FPRINTF("(");
    
    if (prec == low){
      FMB_IO_FPRINTF(LOW_PRECISION_DOUBLE_FPRINTF, pos_x);
      FMB_IO_FPRINTF(", ");
      FMB_IO_FPRINTF(LOW_PRECISION_DOUBLE_FPRINTF, pos_y);
      FMB_IO_FPRINTF(", ");
      FMB_IO_FPRINTF(LOW_PRECISION_DOUBLE_FPRINTF, pos_z);
    }
    else {
      FMB_IO_FPRINTF(HIGH_PRECISION_DOUBLE_FPRINTF, pos_x);
      FMB_IO_FPRINTF(", ");
      FMB_IO_FPRINTF(HIGH_PRECISION_DOUBLE_FPRINTF, pos_y);
      FMB_IO_FPRINTF(", ");
      FMB_IO_FPRINTF(HIGH_PRECISION_DOUBLE_FPRINTF, pos_z);
    }
  
    FMB_IO_FPRINTF(")");
    break;
    
  case FMB_binary_format: 
    FMB_IO_FWRITE_COORDINATE(pos_x);
    FMB_IO_FWRITE_COORDINATE(pos_y);
    FMB_IO_FWRITE_COORDINATE(pos_z);
    break;

  case NEMO_format:
    FMB_error("FMB_IO_Print_position() not implemented for NEMO format.\n");

  default:
    FMB_error("In FMB_IO_Print_position(): unknow file format.\n");
  }
}








/*********************************************************************************************
   FMB_IO_Print_body_fields
*********************************************************************************************/
/* Internal function. */ 
void FMB_IO_Print_body_fields(COORDINATES_T pos_x,
			      COORDINATES_T pos_y,
			      COORDINATES_T pos_z,
			      VALUES_T value,
			      COORDINATES_T fx, 
			      COORDINATES_T fy, 
			      COORDINATES_T fz,
			      position_t *p_speed_vector,
			      bool only_position_and_value){
  
  if (! FMB_IO_Info.output_data_currently_used){
    FMB_error("In FMB_IO_Print_body_fields(): FMB_IO_InitO() has not been called first.\n");
  }

  switch (FMB_IO_Info.output_format){
    
  case FMB_ASCII_format: 
    if (only_position_and_value){
    /* WARNING: we only print here the "position" and "value" of the body,
     * so that we can use this in 'FMB_proc_Dump_proc_decomposition()'. */
    /* WARNING #2: we use '%e' (and not '%f') for cosmological distributions! */
    FMB_IO_FPRINTF("%e\t%e\t%e\t%e\n", 
		   pos_x,
		   pos_y,
		   pos_z,
		   value);
    }
    else {
      FMB_IO_Print_pos_xyz(pos_x, pos_y, pos_z, low);
      FMB_IO_FPRINTF("\t");
      FMB_IO_FPRINTF(LOW_PRECISION_DOUBLE_FPRINTF, value);
      FMB_IO_FPRINTF("\t");
      FMB_IO_Print_pos_xyz(fx, fy, fz, high);
      /*  FMB_IO_FPRINTF("\t"); */
      /*  FMB_IO_Print_position(p_speed_vector, high); */
      FMB_IO_FPRINTF("\n");
    }
    break;

  case FMB_ASCII_human_format: 
    FMB_IO_FPRINTF("*** pos= ");
    FMB_IO_Print_pos_xyz(pos_x, pos_y, pos_z, low);
    FMB_IO_FPRINTF("\tv= ");
    FMB_IO_FPRINTF(LOW_PRECISION_DOUBLE_FPRINTF, value);
    if (! only_position_and_value){
      FMB_IO_FPRINTF("\t\t");
      FMB_IO_FPRINTF("\t\t\t\t\t\tforce= ");
      FMB_IO_Print_pos_xyz(fx, fy, fz, high);
      /*  FMB_IO_FPRINTF("\tspeed="); */
      /*  FMB_IO_Print_position(p_speed_vector, high); */
    }
    FMB_IO_FPRINTF("\t\t\t\t\t\t\t\t\t\t\n");
    break;

  case FMB_binary_format: 
    /* 'only_position_and_value' should have already been used in 'FMB_IO_Print_header()'. */
    if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_POSITION_PRESENCE_BIT)){
      FMB_IO_Print_pos_xyz(pos_x, pos_y, pos_z, low /* unused with FMB_binary_format */);
    }
    if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_VALUE_PRESENCE_BIT)){
      FMB_IO_FWRITE_VALUE(value);
    }
    if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_FORCE_PRESENCE_BIT)){
      FMB_IO_Print_pos_xyz(fx, fy, fz, high /* unused with FMB_binary_format */);
    }
    if (FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(FMB_IO_Info.FMB_binary_format_header, FMB_BINARY_FORMAT_HEADER_SPEED_PRESENCE_BIT)){
      FMB_IO_Print_position(p_speed_vector, high /* unused with FMB_binary_format */);
    }
    break;
    
  case NEMO_format:
    FMB_error("FMB_IO_Print_body_fields() not implemented for NEMO format.\n");
    
  default:
    FMB_error("In FMB_IO_Print_body_fields(): unknow file format.\n");
  }
}






/*********************************************************************************************
   FMB_IO_Print_body
*********************************************************************************************/
/* Write the body in the output data file. */ 
void FMB_IO_Print_body(body_t *p_body,
		       bool only_position_and_value){

  if (! FMB_IO_Info.output_data_currently_used){
    FMB_error("In FMB_IO_Print_body(): FMB_IO_InitO() has not been called first.\n");
  }

  switch (FMB_IO_Info.output_format){
    
  case FMB_ASCII_format: 
  case FMB_ASCII_human_format: 
  case FMB_binary_format:
    FMB_IO_Print_body_fields(position_Get_x(body_Get_p_position(p_body)),
			     position_Get_y(body_Get_p_position(p_body)),
			     position_Get_z(body_Get_p_position(p_body)),
			     body_Get_value(p_body),
			     position_Get_x(body_Get_p_force_vector(p_body)),
			     position_Get_y(body_Get_p_force_vector(p_body)),
			     position_Get_z(body_Get_p_force_vector(p_body)),
			     body_Get_p_speed_vector(p_body),
			     only_position_and_value);
    break;
    
  case NEMO_format:
    FMB_error("FMB_IO_Print_body() not implemented for NEMO format.\n");

  default:
    FMB_error("In FMB_IO_Print_body(): unknow file format.\n");
  }
}








/*********************************************************************************************
   FMB_IO_Print_body_from_bodies_it
*********************************************************************************************/
/* Write the next body into 'p_i' in the output data file. */ 
void FMB_IO_Print_body_from_bodies_it(bodies_it_t *p_i,
				      bool only_position_and_value){

  if (! FMB_IO_Info.output_data_currently_used){
    FMB_error("In FMB_IO_Print_body_from_bodies_it(): FMB_IO_InitO() has not been called first.\n");
  }

  switch (FMB_IO_Info.output_format){
    
  case FMB_ASCII_format: 
  case FMB_ASCII_human_format: 
  case FMB_binary_format:
 FMB_IO_Print_body_fields(bodies_it_Get_pos_x(p_i),
			   bodies_it_Get_pos_y(p_i),
			   bodies_it_Get_pos_z(p_i),
			   bodies_it_Get_value(p_i),
			   bodies_it_Get_fx(p_i),
			   bodies_it_Get_fy(p_i),
			   bodies_it_Get_fz(p_i),
			   bodies_it_Get_p_speed_vector(p_i), 
			   only_position_and_value);
    break;
    
  case NEMO_format:
    FMB_error("FMB_IO_Print_body_from_bodies_it() not implemented for NEMO format.\n");

  default:
    FMB_error("In FMB_IO_Print_body_from_bodies_it(): unknow file format.\n");
  }
}










/*********************************************************************************************
   FMB_IO_Print_all_bodies_from_bodies_t
*********************************************************************************************/
/* Write all the bodies of the 'bodies_t' variable in the output data file. 
 * Only for NEMO format. */ 
void FMB_IO_Print_all_bodies_from_bodies_t(bodies_t *p_bodies){

  if (! FMB_IO_Info.output_data_currently_used){
    FMB_error("In FMB_IO_Print_all_bodies_from_bodies_t(): FMB_IO_InitO() has not been called first.\n");
  }

  switch (FMB_IO_Info.output_format){
    
  case FMB_ASCII_format: 
  case FMB_ASCII_human_format: 
  case FMB_binary_format: 
    FMB_error("FMB_IO_Print_all_bodies_from_bodies_t() not implemented for the FMB formats.\n");
    break;

    
  case NEMO_format: {
    bodies_it_t it;
    bodies_ind_t nb_bodies = bodies_Nb_bodies(p_bodies);

#define MaxVecDim 9 /* value of 'MaxVecDim' in nemo */
    int i=0, nobj = 0, dims[MaxVecDim];
    double time = 0.0;
/*     int coordsystem = 66306; /\* dummy value... *\/ */

    /* Set dims[] to 0: */
    for (i=0; i<MaxVecDim; i++){  dims[i] = 0; }

    /*** Set snapshot: ***/
    put_set((stream) FMB_IO_Info.f_output_data, "SnapShot");
    
    /*** Set header: ***/
    put_set((stream) FMB_IO_Info.f_output_data, "Parameters");      
    put_data_sub((stream) FMB_IO_Info.f_output_data, "Time", (FMB_IO_Info.is_output_coord_float ? "f" : "d"), (void *)&time, dims, 0);
    nobj = (int) nb_bodies;
    put_data_sub((stream) FMB_IO_Info.f_output_data, "Nobj", "i", (void *)&nobj, dims, 0);
    put_tes((stream) FMB_IO_Info.f_output_data, "Parameters");      
    
    /*** Set data: *** 
     * we write here one body at a time thanks to 'put_data_blocked()' 
     * (we write first all masses, then all Position, etc., which implies several passes on the octree,
     * but I don't think it is possible to have just one pass (with several streams): it was possible on reading,
     * but much harder on writing (I would need to know exactly how much to skip with lseek() to correctly set 
     * the different streams...)) */
    put_set((stream) FMB_IO_Info.f_output_data, "Particles");      
/*     put_data_sub((stream) FMB_IO_Info.f_output_data, "CoordSystem", "i", (void *)&coordsystem, dims, 0); */
      

    /*** Mass ***/
    put_data_set((stream) FMB_IO_Info.f_output_data, "Mass", (FMB_IO_Info.is_output_coord_float ? "f" : "d"), nb_bodies, 0);       
    for (bodies_it_Initialize(&it, p_bodies);
	 bodies_it_Is_valid(&it);
	 bodies_it_Go2Next(&it)){
      put_data_blocked((stream) FMB_IO_Info.f_output_data, "Mass", (void *) bodies_it_Get_p_value(&it), 1);	  
    }
    put_data_tes((stream) FMB_IO_Info.f_output_data, "Mass");
    

    if (FMB_IO_Info.use_PhaseSpace_in_output){

      /*** PhaseSpace ***/
      put_data_set((stream) FMB_IO_Info.f_output_data, "PhaseSpace", (FMB_IO_Info.is_output_coord_float ? "f" : "d"), nb_bodies, 2, 3, 0);
      for (bodies_it_Initialize(&it, p_bodies);
	   bodies_it_Is_valid(&it);
	   bodies_it_Go2Next(&it)){
	position_t pos;
	position_Set_x(&pos, bodies_it_Get_pos_x(&it));
	position_Set_y(&pos, bodies_it_Get_pos_y(&it));
	position_Set_z(&pos, bodies_it_Get_pos_z(&it));

	put_data_blocked((stream) FMB_IO_Info.f_output_data, "PhaseSpace", (void *)&pos, 3);	  
	put_data_blocked((stream) FMB_IO_Info.f_output_data, "PhaseSpace", (void *)bodies_it_Get_p_speed_vector(&it), 3);	  
      }
      put_data_tes((stream) FMB_IO_Info.f_output_data, "PhaseSpace");

    } /* if (FMB_IO_Info.use_PhaseSpace_in_output) */
    else {

      /*** Position ***/
      put_data_set((stream) FMB_IO_Info.f_output_data, "Position", (FMB_IO_Info.is_output_coord_float ? "f" : "d"), nb_bodies, 1, 3, 0);
      for (bodies_it_Initialize(&it, p_bodies);
	   bodies_it_Is_valid(&it);
	   bodies_it_Go2Next(&it)){
	    position_t pos;
	    position_Set_x(&pos, bodies_it_Get_pos_x(&it));
	    position_Set_y(&pos, bodies_it_Get_pos_y(&it));
	    position_Set_z(&pos, bodies_it_Get_pos_z(&it));

	    put_data_blocked((stream) FMB_IO_Info.f_output_data, "Position", (void *)&pos, 3);	  
      }
      put_data_tes((stream) FMB_IO_Info.f_output_data, "Position");
      
      /*** Velocity ***/
      put_data_set((stream) FMB_IO_Info.f_output_data, "Velocity", (FMB_IO_Info.is_output_coord_float ? "f" : "d"), nb_bodies, 1, 3, 0);
      for (bodies_it_Initialize(&it, p_bodies);
	   bodies_it_Is_valid(&it);
	   bodies_it_Go2Next(&it)){
	put_data_blocked((stream) FMB_IO_Info.f_output_data, "Velocity", (void *)bodies_it_Get_p_speed_vector(&it), 3);	  
      }
      put_data_tes((stream) FMB_IO_Info.f_output_data, "Velocity");
      
    } /* else (FMB_IO_Info.use_PhaseSpace_in_output) */

    

    /*** Accelerations ***/
    /* I assume here: accelerations = forces / mass */
    put_data_set((stream) FMB_IO_Info.f_output_data, "Acceleration", (FMB_IO_Info.is_output_coord_float ? "f" : "d"), nb_bodies, 1, 3, 0);       
    for (bodies_it_Initialize(&it, p_bodies);
	 bodies_it_Is_valid(&it);
	 bodies_it_Go2Next(&it)){
      position_t acc;
      position_Set_x(&acc, bodies_it_Get_fx(&it));
      position_Set_y(&acc, bodies_it_Get_fy(&it));
      position_Set_z(&acc, bodies_it_Get_fz(&it));
      position_Mult_by_real(&acc, (REAL_T) 1/bodies_it_Get_value(&it));
      put_data_blocked((stream) FMB_IO_Info.f_output_data, "Acceleration", (void *)&acc, 3);	  
    }
    put_data_tes((stream) FMB_IO_Info.f_output_data, "Acceleration");
    
    /* tes (Particles): */
    put_tes((stream) FMB_IO_Info.f_output_data, "Particles");      
    
    /* tes (Snapshot): */
    put_tes((stream) FMB_IO_Info.f_output_data, "SnapShot");
    
#undef MaxVecDim
    break;
  }
    
  default:
    FMB_error("In FMB_IO_Print_all_bodies_from_bodies_t(): unknow file format.\n");
  }
}










/*********************************************************************************************
**********************************************************************************************

   b[un]zip2_file 

**********************************************************************************************
*********************************************************************************************/

void bunzip2_file(const char *filename){
  /* bunzip2_file() is used for "data_file": */
  char *decompression_command = (char *) FMB_malloc_with_check(strlen(filename) + 1 + BZIP2_DECOMPRESSOR_LENGTH);
  strcpy(decompression_command, BZIP2_DECOMPRESSOR);
  strcat(decompression_command, " ");
  strcat(decompression_command, filename);
  strcat(decompression_command, BZIP2_COMPRESSION_SUFFIX);
  if (system(decompression_command) == -1){ FMB_ERROR_BRIEF(); }
  FMB_free(decompression_command);
}


void bzip2_file(const char *filename){
  /* bzip2_file() is used for "data_file": */
  char *compression_command = (char *) FMB_malloc_with_check(strlen(filename) + 1 + BZIP2_COMPRESSOR_LENGTH);
  strcpy(compression_command, BZIP2_COMPRESSOR);
  strcat(compression_command, " ");
  strcat(compression_command, filename);
  if (system(compression_command) == -1){ FMB_ERROR_BRIEF(); }
  FMB_free(compression_command);
}



















