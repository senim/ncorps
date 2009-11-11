#---------------------------------------------------------------------------------------#
#
#                                         MAKEFILE
#
#---------------------------------------------------------------------------------------#




################################### High-level options ##################################


#---------------------------------------------------------------------------------------#
# HOST_TARGET
#---------------------------------------------------------------------------------------#

HOST_TARGET=i686_32




#---------------------------------------------------------------------------------------#
# NEMO
#---------------------------------------------------------------------------------------#
# To use NEMO file format (for input/output data files): */
#NO_NEMO=y#n


#---------------------------------------------------------------------------------------#
# ZLIB
#---------------------------------------------------------------------------------------#
# To read/write directly in "*.gz" files (for input/output data files): */
#NO_ZLIB=y#n


#---------------------------------------------------------------------------------------#
# Debugging
#---------------------------------------------------------------------------------------#
#DEBUGGING=y


#---------------------------------------------------------------------------------------#
# Profiling
#---------------------------------------------------------------------------------------#
#PROFILING=y#n







################################ End of high-level options ############################## 







#---------------------------------------------------------------------------------------#
# Compiler options:
#---------------------------------------------------------------------------------------#
CC=gcc 


# For the compiler:
LIB_DIR=
INC_DIR= 
LIBRARIES=-lm 

WARNINGS=-Wall #-Winline
#WARNINGS = -w #for icc
#WARNINGS += -ansi #-pedantic # this is to ease the compilation on AIX (with xlc) 
			     # warning: should not be set when compiling generateur_XD_uniforme 
			     # (  because of srand48() et drand48() )

# Directories:
NBODY_DIRECT_SRC=src/
NBODY_DIRECT_INC=include/
INC_DIR += -I$(NBODY_DIRECT_INC)
BIN=bin/
OBJ=lib/

### Optimisation:
ifneq ($(PROFILING), y)
OPTIM_FLAGS = -O3 -funroll-loops -ftree-vectorize -msse2 -mtune=core2 -march=core2 -ffast-math
#OPTIM_FLAGS = -O2 -ffast-math -funroll-loops -fforce-addr -fomit-frame-pointer #-finline-limit=20000 
# -ffast-math => -fno-math-errno, -funsafe-math-optimizations, and -fno-trapping-math.
#
### flags  used by falcON: 
#OPTIM_FLAGS = -O2 -fPIC -ffast-math -funroll-loops -fforce-addr -fomit-frame-pointer -rdynamic    
# remark : -rdynamic => (with linux) ld -export-dynamic 
####
else
# "-fomit-frame-pointer" is incompatible with "-pg"
#OPTIM_FLAGS = -O2 -finline-limit=20000 -ffast-math -funroll-loops -fforce-addr #-fomit-frame-pointer
OPTIM_FLAGS = -03 -funroll-loops -ftree-vectorize -msse2 -mtune=core2 -march=core2 -ffast-math
endif

### GNU extensions (this is for sched_setaffinity()):
GNU_FLAGS += -D_GNU_SOURCE 








#---------------------------------------------------------------------------------------#
# Set parameters according to 'HOST_TARGET':
#---------------------------------------------------------------------------------------#
 LOCAL_PATH=$(HOME)/local/

 NEMO_INC=/users/Enseignants/fortin/Public/PPAR/Projet/NEMO/nemo_cvs/include/
 NEMO_LIB=/users/Enseignants/fortin/Public/PPAR/Projet/NEMO//nemo_cvs/lib/


### local path:
INC_DIR += -I$(LOCAL_PATH)include/
LIB_DIR += -L$(LOCAL_PATH)lib/


### NEMO:
ifneq ($(NO_NEMO), y)
 INC_DIR += -I$(NEMO_INC) -I$(NEMO_LIB) # we also include $(NEMO_LIB) for 'maxsizes.h' 
 LIB_DIR += -L$(NEMO_LIB)
 LIBRARIES+=-lnemo
else
 CODE_FLAGS+=-D_FMB_NO_NEMO_ 
endif 



### ZLIB:
ifneq ($(NO_ZLIB), y)
 LIBRARIES+= -lz
else
 CODE_FLAGS+=-D_FMB_NO_ZLIB_ 
endif 




CFLAGS = $(WARNINGS) \
	$(OPTIM_FLAGS) \
	$(GNU_FLAGS) 


### gprof:
GPROF_FLAG = -pg
ifeq ($(PROFILING), y) 
# CFLAGS = $(GPROF_FLAG) # (overwrite CFLAGS)
 CFLAGS += $(GPROF_FLAG) 
endif 





### Debugging:
ifeq ($(DEBUGGING), y)
 CFLAGS = -g -Wall -fno-inline -D_REENTRANT $(GNU_FLAGS) #-pg #-std=gnu99 	
# CFLAGS := -g $(CFLAGS) 	
endif











#---------------------------------------------------------------------------------------#
# General options:
#---------------------------------------------------------------------------------------#
CODE_FLAGS += -DCODE_DIRECTORY=\"$(PWD)/\"      # Initialization  

# OS
OS_FLAGS = -D__LINUX__
CODE_FLAGS += $(OS_FLAGS)

#CODE_FLAGS += -D_USE_CONSTANT_INTERACTION_FACTOR_ 







#---------------------------------------------------------------------------------------#
# Objects files:
#---------------------------------------------------------------------------------------#
OBJECTS = $(OBJ)math_implementation.o \
	$(OBJ)bodies.o \
	$(OBJ)memory_allocation.o \
	$(OBJ)IO.o \
	$(OBJ)direct_method.o

SRCS = $(subst $(OBJ),$(NBODY_DIRECT_SRC),$(OBJECTS:.o=.c))






#---------------------------------------------------------------------------------------#
#--------------------------------------- Rules: ----------------------------------------#
#---------------------------------------------------------------------------------------#

all: init .remove.o $(OBJECTS) direct 
	@echo ""
	@echo "     *************** Compilation is over! ***************"
	@echo ""

# What must be done before everything:
init:
	@echo ""
	@echo "Compiling $(HOST_TARGET) with $(CC)= `which $(CC)` ..."
	@echo ""
ifeq ($(MAKEFILE_LIST),) #see .remove.o
	@echo "Error: MAKEFILE_LIST is undefined by make/gmake. Check that make/gmake version is >= 3.80, otherwise install it!"
	@exit 1
endif

direct: $(BIN)NBODY_direct

doc: 
	doxygen ./doc/Doxyfile 

# For .remove.o, we require the MAKEFILE_LIST feature that was added in GNU make 3.80.  
# Earlier versions do not have it.	
# MAKEFILE_LIST contains all Makefiles used/included.
#
# To force 'all' target when $(MAKEFILE_LIST) is modified: 
MY_MAKEFILE_LIST:=$(MAKEFILE_LIST) # To avoid .depend (included below)
.remove.o: $(MY_MAKEFILE_LIST)
#	@echo "MY_MAKEFILE_LIST: " $(MY_MAKEFILE_LIST)
#	@echo "first word in MY_MAKEFILE_LIST: " $(firstword $(MY_MAKEFILE_LIST))
	$(MAKE) -f $(firstword $(MY_MAKEFILE_LIST)) net.o 
	touch $@





#---------------------------------------------------------------------------------------#
#---------------------------------------- Bin: -----------------------------------------#
#---------------------------------------------------------------------------------------#

$(BIN)NBODY_direct: $(NBODY_DIRECT_SRC)main_direct_method.c $(OBJECTS)
	$(CC) $(CFLAGS) $(CODE_FLAGS) $(INC_DIR) $(LIB_DIR)  -o $@ $+ $(LIBRARIES) 







#---------------------------------------------------------------------------------------#
#-------------------------------------- .o files: --------------------------------------#
#---------------------------------------------------------------------------------------#

$(OBJ)%.o: $(NBODY_DIRECT_SRC)%.c $(NBODY_DIRECT_INC)%.h
	$(CC) -c $(CFLAGS) $(CODE_FLAGS) $(INC_DIR) -o $@ $<

.depend: $(SRCS) 
	rm -f .depend 
	for i in $^ ; do \
		OBJ=$$( echo $$i | sed 's?$(NBODY_DIRECT_SRC)?$(OBJ)?;s?.c$$?.o?' ) ; \
		gcc  $(CFLAGS) $(CODE_FLAGS) $(INC_DIR) -MM -MT "$${OBJ}" $$i >> .depend 2>/dev/null ; \
	done 

-include .depend





#---------------------------------------------------------------------------------------#
#----------------------------------- Auxiliary targets: --------------------------------#
#---------------------------------------------------------------------------------------#

net.o: 
	@echo ""
	@echo "*** Remove object files: ***"
	rm -f ${OBJ}*.o ${OBJ}*.a ${OBJ}*.gcda ${OBJ}*.gcno *.o *.gcda *.gcno
	@echo ""

net.bin:
	@echo ""
	@echo "*** Remove binary files: ***"
	rm -f ${BIN}DIRECT_METHOD* 
	@echo ""

clean: 
	@echo "" 
	@echo "*** Clean: ***"
	rm -f core *~ ${NBODY_DIRECT_SRC}*~ ${NBODY_DIRECT_INC}*~ 
	@echo ""

vclean: net.o clean 
	rm -f .remove.o .depend 


.PHONY: all init direct doc net.o net.bin clean vclean 






