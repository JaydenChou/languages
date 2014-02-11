#!/bin/sh
#*******************************************************************************
#   File: runme.sh
#   
#   Comments: shell script to run the tests
#   
#   Compiler: tested in linux only
#      gcc/g++ (GCC) 3.2.2 20030222 - Red Hat 9 (i386) - kernel 2.4
#      gcc/g++ 3.3.3 20040412 - Fedora Core 2 (i686) - kernel 2.6.10
#      gcc/g++ 3.4.3 20050227 - Fedora Core 3 (i386) - kernel 2.6.12
#      gcc/g++ 4.0.0 20050519 - Fedora Core 4 (i686) - kernel 2.6.11
#      gcc/g++ 4.0.1 20050727 - Fedora Core 4 (x86_64) - kernel 2.6.12
#   
#   History:
#      Oct-20-2005: Javier Fuchs                                   |\_/|   
#                   Phone (+54 2944) 525173                       /     \  
#                   http://southfox.servehttp.com                /_.~ ~,_\ 
#                                                                   \@/    
#*******************************************************************************

./TupleTestDriver
rta=$?
echo "error code = ${rta}"
if [ ${rta} -eq 0 ]; then
   echo "    +------------------------+"
   echo "    | Tuple test... OK.      |"
   echo "    +------------------------+"
else
   echo "    +------------------------+"
   echo "    | Tuple test... ERROR!!! |"
   echo "    +------------------------+"
fi

echo "Testing Set:"
./SetTestDriver
rta=$?
echo "error code = ${rta}"
if [ ${rta} -eq 0 ]; then
   echo "    +------------------------+"
   echo "    | Set test... OK.        |"
   echo "    +------------------------+"
else
   echo "    +------------------------+"
   echo "    | Set test... ERROR!!!   |"
   echo "    +------------------------+"
fi
