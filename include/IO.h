#ifndef __FMB_IO_H__
#define __FMB_IO_H__


#include "body.h"
#include "bodies.h"

#ifndef _FMB_NO_ZLIB_
#include <zlib.h> /* see below for dedicated functions */
#else 
/*** We redefine all 'zlib' functions and types. ***/ 
#define Z_NULL  0
#define Z_ERRNO (-1)
/* Types: */
typedef void *gzFile;
/* Function declarations: */
int FMB_NO_ZLIB_error(); /* 'int' returned for compatibility with gzread and gzwrite */
gzFile FMB_NO_ZLIB_error_gzopen(); /* only for 'gzopen' */
#define gzclose(...)  FMB_NO_ZLIB_error()
#define gzopen(...)   FMB_NO_ZLIB_error_gzopen() 
#define gzgets(...)   FMB_NO_ZLIB_error() 
#define gzprintf(...) FMB_NO_ZLIB_error()		               
#define gzseek(...)   FMB_NO_ZLIB_error()	                       
#define gzread(...)   FMB_NO_ZLIB_error()
#define gzwrite(...)  FMB_NO_ZLIB_error()
#endif 











/*******************************************************************************
   Types
******************************************************************************/

/* Supported file formats: */
typedef enum FMB_IO_format_t {
  FMB_ASCII_format /* default */, 
  FMB_ASCII_human_format /*like 'FMB_ASCII_format' but "human readable",
			  * only for 'output_format' (not for 'input_format') */,  
  FMB_binary_format, 
  NEMO_format
} FMB_IO_format_t;
#define FMB_IO_FORMAT_PRINTF_TEST(x) ((x) == FMB_ASCII_format ? "FMB_ASCII_format" : 			\
				      ((x) == FMB_ASCII_human_format ? "FMB_ASCII_human_format" : 	\
				       ((x) == FMB_binary_format ? "FMB_binary_format" : 		\
					"NEMO_format")))

/* FMB binary format: 
 * - the first 4 bytes (i.e. 2 'unsigned long long') are the followings: 
 *   - the first 2 bytes are used as a "magic number" to check that the file corresponds to "FMB binary" format
 *   - the following 2 bytes are the "FMB binary format header" and contain additional informations 
 *     about the data types (float or double) and about which data are present in the file 
 *     (position, value, potential, force, speed vector) 
 * 
 * - the rest of the file contains, for each body, in this order:
 *     position (x,y,z)           (mandatory) 
 *     value                      (mandatory)
 *     potential                  (optional)
 *     force vector (x,y,z)       (optional) 
 *     speed vector (x,y,z)       (optional)
 *  
 * We use htonl/ntohl (as well as htonf/ntohf and htond/ntohd) functions for portability.
 */
#define FMB_BINARY_FORMAT_MAGIC_NUMBER                      0x77777777UL /* 2 bytes */
#define FMB_BINARY_FORMAT_HEADER_FLOAT_DATA_BIT             0x1UL 
#define FMB_BINARY_FORMAT_HEADER_POSITION_PRESENCE_BIT      0x2UL 
#define FMB_BINARY_FORMAT_HEADER_VALUE_PRESENCE_BIT         0x4UL 
#define FMB_BINARY_FORMAT_HEADER_POTENTIAL_PRESENCE_BIT     0x8UL 
#define FMB_BINARY_FORMAT_HEADER_FORCE_PRESENCE_BIT         0x10UL 
#define FMB_BINARY_FORMAT_HEADER_SPEED_PRESENCE_BIT         0x20UL 
#define FMB_BINARY_FORMAT_HEADER_IS_THE_BIT_SET(info, bit)  ((info) & (bit)) /* 'info' is an 'unsigned long' */

/* NEMO format: */
#define NEMO_SUFFIX_LENGTH (sizeof(".nemo")) /* ok for ".NEMO" also */ 

typedef struct FMB_IO_Info_t {

  /*** 'info_display_level': ***/
  /* 0 : no information displayed
   * 1 : essential information displayed (i.e. no display of data, but display of the main steps, CPU times) 
   * 2 : more information displayed (octree structure, info about parallelism : these information cost CPU time to be displayed)
   * 3 : maximum information displayed */
  int info_display_level;

  /* input/output filenames: */
  char *input_data_filename;
  char *output_data_filename;

  /* input/output file formats: */
  FMB_IO_format_t input_format;
  FMB_IO_format_t output_format;
  bool input_format_from_cmd_line;
  bool output_format_from_cmd_line;
  
  /* input/output streams: */
  FILE *f_input_data;
  FILE *f_output_data;

  /* input/output streams: */
  bool input_data_currently_used;
  bool output_data_currently_used;

  /* data files compression: */
  bool are_input_data_bzipped2; 
  bool are_output_data_bzipped2; 
  bool are_input_data_gzipped; 
  bool are_output_data_gzipped; 

  /* input/output streams for gzipped files: */
  gzFile f_input_gz_data;
  gzFile f_output_gz_data;

  /* buffer for gzipped file reading: */
  char *gz_input_buf;
  long gz_input_buf_len;

  /* input/output secondary/tertiary streams for NEMO format: */
  FILE *f_input_data2;
  FILE *f_input_data3;
  FILE *f_output_data2;
  FILE *f_output_data3;
  bool use_PhaseSpace_in_input; /* if TRUE: we use 'PhaseSpace', 
				 * otherwise, we use 'Position' and 'Velocity'  
				 * (only for NEMO format) */
  bool use_PhaseSpace_in_output; /* same as 'use_PhaseSpace_in_output' but for writing */
  bool is_Velocity_present_in_input; /* only for NEMO format */
  bool is_Velocity_present_in_output; /* only for NEMO format */
  bool is_input_coord_float; /* if TRUE: we use 'float' (single precision) for 'PhaseSpace' (or 'Position' and 'Velocity), 
			      * otherwise, we use 'double' (double precision)   
			      * (only for NEMO format) */
  bool is_output_coord_float; /* same as 'is_input_coord_float' but for writing */
  FILE *f_input_data_forces;

  /* for FMB binary format: */
  unsigned long FMB_binary_format_header;
  

  /* with/without decomposed data files: */
  bool dump_proc_decomposition;
  bool use_decomposed_data_files; 
  bool proc_decomposition_with_empty_or_not; /* If FALSE we use "with_nb_particles". 
					      * Default is: "with_nb_particles". */

} FMB_IO_Info_t;







/*******************************************************************************
   Global variable declaration
******************************************************************************/

extern FILE *f_output; /* standard output stream: NOT related to "output" terms in 'FMB_IO_Info' */
extern FMB_IO_Info_t FMB_IO_Info;
#define INFO_DISPLAY(level) ( FMB_IO_Info.info_display_level >= (level) )








/*********************************************************************************************
   ZLIB
*********************************************************************************************/

/* If there is bug for gzgets with huge gzipped files, see inflate()/deflate() and
 * FAQ of zlib-1.2.3 => question #32: "Can zlib work with greater than 4 GB of data?" ... */  

/*** ASCII: ***/

/* 'nb_chars_to_read' should include the terminating NULL chararacter ('\0'). */
#define FMB_IO_FSCANF(nb_chars_to_read, fscanf_format, ...) {	                                                \
  if (FMB_IO_Info.are_input_data_gzipped){									\
    if (FMB_IO_Info.gz_input_buf_len < nb_chars_to_read){ 						        \
      FMB_IO_Info.gz_input_buf = FMB_realloc_with_check(FMB_IO_Info.gz_input_buf, nb_chars_to_read);	        \
      FMB_IO_Info.gz_input_buf_len = nb_chars_to_read;							        \
    }                                                                                                           \
    if (gzgets(FMB_IO_Info.f_input_gz_data, FMB_IO_Info.gz_input_buf, nb_chars_to_read) == Z_NULL){ FMB_ERROR_BRIEF(); }		\
    sscanf(FMB_IO_Info.gz_input_buf, fscanf_format, __VA_ARGS__);						\
  }														\
  else {													\
    if (fscanf(FMB_IO_Info.f_input_data, fscanf_format, __VA_ARGS__) == 0){ FMB_ERROR_BRIEF(); }                \
  }														\
}

#define FMB_IO_FPRINTF(...) {	                                                        \
  if (FMB_IO_Info.are_output_data_gzipped){						\
    gzprintf(FMB_IO_Info.f_output_gz_data, __VA_ARGS__);		                \
  }											\
  else {										\
    fprintf(FMB_IO_Info.f_output_data,  __VA_ARGS__);			                \
  }											\
}


/* The following macro has to be used ONLY when 'f_gz_data' is NOT 'FMB_IO_Info.f_input_gz_data'. */
#define FMB_FSCANF_GZIP(f_data, f_gz_data, are_data_gzipped, gzgets_buffer_len, fscanf_format, ...) {	        \
  if (are_data_gzipped){											\
    char buf[gzgets_buffer_len];										\
    if (gzgets(f_gz_data, buf, gzgets_buffer_len) == Z_NULL){ FMB_ERROR_BRIEF(); }				\
    sscanf(buf, fscanf_format, __VA_ARGS__);									\
  }														\
  else {													\
    fscanf(f_data, fscanf_format, __VA_ARGS__);									\
  }														\
}

/* The following macro has to be used ONLY when 'f_gz' is NOT 'FMB_IO_Info.f_output_gz_data'. */
#define FMB_FPRINTF_GZIP(f, f_gz, are_data_gzipped, ...) {	                        \
  if (are_data_gzipped){								\
    gzprintf(f_gz, __VA_ARGS__);					                \
  }											\
  else {										\
    fprintf(f,  __VA_ARGS__);						                \
  }											\
}




#define FMB_IO_INPUT_FSEEK(offset, whence) {                                            \
  if (FMB_IO_Info.are_input_data_gzipped){						\
    gzseek(FMB_IO_Info.f_input_gz_data, offset, whence);	                        \
  }											\
  else {										\
    fseek(FMB_IO_Info.f_input_data, offset, whence);			                \
  }											\
}


/*** Binary: ***/

/* Reads a 'long' from input. */
FMB_INLINE long FMB_IO_Fread_long(){
  int length = sizeof(long);
  long res   = 0;
  if (FMB_IO_Info.are_input_data_gzipped){
    if (gzread(FMB_IO_Info.f_input_gz_data, &res, length) < length){ FMB_ERROR_BRIEF(); }
  }
  else {
    if (fread(&res, length /* size */, 1 /* number */, FMB_IO_Info.f_input_data) < 1){ FMB_ERROR_BRIEF(); }
  }
  return (long) ntohl((unsigned long) res);
}

/* Writes a 'long' to output. */
FMB_INLINE void FMB_IO_Fwrite_long(long l){
  int length = sizeof(long);
  long res   = (long) htonl((unsigned long) l);
  if (FMB_IO_Info.are_output_data_gzipped){
    if (gzwrite(FMB_IO_Info.f_output_gz_data, &res, length) < length){ FMB_ERROR_BRIEF();}
  }
  else {
    if (fwrite(&res, length /* size */, 1 /* number */, FMB_IO_Info.f_output_data) < 1){ FMB_ERROR_BRIEF(); }
  }
}

/* Reads an 'unsigned long' from input. */
FMB_INLINE unsigned long FMB_IO_Fread_ulong(){
  int length = sizeof(unsigned long);
  unsigned long res   = 0;
  if (FMB_IO_Info.are_input_data_gzipped){
    if (gzread(FMB_IO_Info.f_input_gz_data, &res, length) < length){ FMB_ERROR_BRIEF(); }
  }
  else {
    if (fread(&res, length /* size */, 1 /* number */, FMB_IO_Info.f_input_data) < 1){ FMB_ERROR_BRIEF(); }
  }
  return ntohl(res); 
}

/* Writes an 'unsigned long' to output. */
FMB_INLINE void FMB_IO_Fwrite_ulong(unsigned long l){
  int length = sizeof(unsigned long);
  unsigned long res   = htonl(l);
  if (FMB_IO_Info.are_output_data_gzipped){
    if (gzwrite(FMB_IO_Info.f_output_gz_data, &res, length) < length){ FMB_ERROR_BRIEF();}
  }
  else {
    if (fwrite(&res, length /* size */, 1 /* number */, FMB_IO_Info.f_output_data) < 1){ FMB_ERROR_BRIEF();}
  }
}




/* Reads a 'float' from input. */
FMB_INLINE float FMB_IO_Fread_float(){
  int length = sizeof(float);
  float res  = 0.0;
  if (FMB_IO_Info.are_input_data_gzipped){
    if (gzread(FMB_IO_Info.f_input_gz_data, &res, length) < length){ FMB_ERROR_BRIEF();}
  }
  else {
    if (fread(&res, length /* size */, 1 /* number */, FMB_IO_Info.f_input_data) < 1){ FMB_ERROR_BRIEF();}
  }
  return res;
}

/* Writes a 'float' to output. */
FMB_INLINE void FMB_IO_Fwrite_float(float f){
  int length = sizeof(float);
  if (FMB_IO_Info.are_output_data_gzipped){
    if (gzwrite(FMB_IO_Info.f_output_gz_data, &f, length) < length){ FMB_ERROR_BRIEF();}
  }
  else {
    if (fwrite(&f, length /* size */, 1 /* number */, FMB_IO_Info.f_output_data) < 1){ FMB_ERROR_BRIEF();}
  }
}




/* Reads a 'double' from input. */
FMB_INLINE double FMB_IO_Fread_double(){
  int length = sizeof(double);
  double res = 0.0;
  if (FMB_IO_Info.are_input_data_gzipped){
    if (gzread(FMB_IO_Info.f_input_gz_data, &res, length) < length){ FMB_ERROR_BRIEF();}
  }
  else {	
    if (fread(&res, length /* size */, 1 /* number */, FMB_IO_Info.f_input_data) < 1){ FMB_ERROR_BRIEF();}
  }
  return res;
}



/* Writes a 'double' to output. */
FMB_INLINE void FMB_IO_Fwrite_double(double d){
  int length = sizeof(double);
  if (FMB_IO_Info.are_output_data_gzipped){
    if (gzwrite(FMB_IO_Info.f_output_gz_data, &d, length) < length){ FMB_ERROR_BRIEF();}
  }
  else {
    if (fwrite(&d, length /* size */, 1 /* number */, FMB_IO_Info.f_output_data) < 1){ FMB_ERROR_BRIEF();}
  }
}





/***** Command line compression/decompression: *****/  

/* Compression type used: */
#define BZIP2_COMPRESSION_SUFFIX ".bz2"
#define BZIP2_COMPRESSOR "bzip2"
/* COMPRESSOR_LENGTH is the length of the compressor name + one white space. */
#define BZIP2_COMPRESSOR_LENGTH 15 
#define BZIP2_DECOMPRESSOR "bunzip2"
/* DECOMPRESSOR_LENGTH is the length of the decompressor name + one white space + the COMPRESSION_SUFFIX length. */
#define BZIP2_DECOMPRESSOR_LENGTH 15

#define GZIP_DECOMPRESSOR_LENGTH         15
#define GZIP_COMPRESSION_SUFFIX          ".gz"
#define GZIP_COMPRESSION_SUFFIX_LENGTH   (strlen(GZIP_COMPRESSION_SUFFIX))




void bunzip2_file(const char *filename);
void bzip2_file(const char *filename);









/*******************************************************************************
   Non-inlined functions
******************************************************************************/
void FMB_IO_InitI(char *data_file);
void FMB_IO_TerminateI();
void FMB_IO_InitO(char *data_file);
void FMB_IO_TerminateO();
void FMB_IO_Scan_header(bodies_ind_t *p_nb_bodies, position_t *p_center, COORDINATES_T *p_half_side);
void FMB_IO_Print_header(unsigned long step_counter /* only for FMB_ASCII_human_format, 
						     * but not very useful in fact... */,
			 bool only_position_and_value /* for FMB_binary_format header */,
			 bodies_ind_t nb_bodies,
			 position_t *p_center, 
			 COORDINATES_T half_side);

/* The following functions are only for input/output 
 * for input/output data files (i.e. not for standard output 
 * for example...). */
void FMB_IO_Print_position(position_t *FMB_RESTRICT p, precision_double_t prec);
void FMB_IO_Print_pos_xyz(COORDINATES_T pos_x,
			  COORDINATES_T pos_y,
			  COORDINATES_T pos_z,
			  precision_double_t prec);
int FMB_IO_Scan_body(body_t *p_body);
void FMB_IO_Print_body(body_t *p_body, bool only_position_and_value);
void FMB_IO_Print_body_from_bodies_it(bodies_it_t *p_i, bool only_position_and_value);
void FMB_IO_Print_all_bodies();
void FMB_IO_Print_all_bodies_from_bodies_t(bodies_t *p_bodies);





#endif /* #ifndef __FMB_IO_H__ */

