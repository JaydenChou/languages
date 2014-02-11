#-------------------------------------------------------------------------------
# Nombre: makefile
#
# Descripcion: 
#
#-------------------------------------------------------------------------------

dirExe=./
dirObj=./
dirSrc=./
_C_link=gcc -o $@ -g -ggdb3 -fPIC -Wall
_C_compile=gcc -c -g -ggdb3 -Wunused -o $@
#-------------------------------------------------------------------------------
#-- Etiquetas

EXES=$(dirExe)srv $(dirExe)cli

all     : $(EXES)
	@ echo "Constuido..."

#-------------------------------------------------------------------------------
#-- EXE 

$(dirExe)srv : $(dirObj)errores.o $(dirObj)srv.o $(dirObj)clisrvnet.o
	$(_C_link) $(dirObj)errores.o $(dirObj)srv.o $(dirObj)clisrvnet.o

$(dirExe)cli : $(dirObj)errores.o $(dirObj)cli.o $(dirObj)clisrvnet.o
	$(_C_link) $(dirObj)errores.o $(dirObj)cli.o $(dirObj)clisrvnet.o


#------- OBJ -------------------------------------------------------------
#--
OBJS=$(dirObj)srv.o $(dirObj)cli.o $(dirObj)errores.o

$(dirObj)srv.o : $(dirSrc)srv.c $(dirSrc)errores.h $(dirSrc)clisrvnet.h
	$(_C_compile) $(dirSrc)srv.c

$(dirObj)cli.o : $(dirSrc)cli.c $(dirSrc)errores.h $(dirSrc)clisrvnet.h
	$(_C_compile) $(dirSrc)cli.c

$(dirObj)clisrvnet.o : $(dirSrc)clisrvnet.c $(dirSrc)clisrvnet.h
	$(_C_compile) $(dirSrc)clisrvnet.c

$(dirObj)errores.o : $(dirSrc)errores.c $(dirSrc)errores.h
	$(_C_compile) $(dirSrc)errores.c

#------ Clean -----------------------------------------------------------------
#--
clean   :
	rm -f $(EXES) $(OBJS)
