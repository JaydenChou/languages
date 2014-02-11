#!/bin/bash
# 
#   buildme.sh
#   Executable and Library build
# 
#   Created by Javier Fuchs on 1/1/11.
#   Copyright (c) 2013 [Your company name here]. All rights reserved.
# 

#--------------------------------------------------------------------------------
# SETEOS DE VARIABLES DE AMBIENTE
#--------------------------------------------------------------------------------
. ./setenv.sh
GENERATOR=$0 
GENERATOR_ARGS=$@
DEBUG_CONFIGURATION=Debug
RELEASE_CONFIGURATION=Release
# build: "clean all", "all"
BUILD=${1:-"all"}
CONFIGURATION=${2:-${DEBUG_CONFIGURATION}}
XXX_SUBDIR=some
XXX_DIR=${XX_DIR}/${XXX_SUBDIR}
cd ${XXX_DIR}
. ${XXX_DIR}/setenv.sh
cd -
XX_YOU_DAEMON_NAME=${XXX_BIN_FILE}
mkdir -p ${XXX_INSTALL}
XX_INSTALL=${3:-${HOME}/${XX_YOU_DAEMON_NAME}}

#--------------------------------------------------------------------------------
# ULIMIT
#--------------------------------------------------------------------------------
ulimit -c unlimited


#--------------------------------------------------------------------------------
# CONSTRUYENDO ABOUT (version de repositorio, etc)
#--------------------------------------------------------------------------------
about()
{
    ABOUT=some/src/XXXAbout.h

	# GIT VERSION
	which git 1>/dev/null 2>&1
    if [ $? -eq 0 ]; then
        GIT_VERSION=$(git log -1 --oneline);
    else
        GIT_VERSION=1;
    fi
	echo "Regenerando ${ABOUT}"

    rm -f ${ABOUT}
    echo "
/* generated */
/* No editar este archivo. Es un archivo generado por el programa: ${GENERATOR} */
/* Ejecutar nuevamente: */
/* \"\$ ${GENERATOR} ${GENERATOR_ARGS} \" */
/* generated */
#ifndef __XXX_ABOUT_H__
#define __XXX_ABOUT_H__

/* SVN version */
#define XXX_ABOUT_GIT_VERSION \"${GIT_VERSION}\"

/* Instance ID */
#define XXX_ABOUT_INSTXXCE_ID \"${INSTXXCE_ID}\"

/* Copyright */
#define XXX_ABOUT_OWNER \"(c) You\"

/* En que plataforma y equipo se ejecuta esto */
#define XXX_ABOUT_UNAME \"`uname -a`\"

/* Dia y hora (UTC) */
#define XXX_ABOUT_BUILD_DATE_TIME \"`date -u`\"

/* Directorio donde se construye */
#define XXX_ABOUT_PWD \"${PWD}\"

/* Version compilador C++ */
#define XXX_ABOUT_CPLUSPLUS_VERSION \"`type g++`: `g++ -v 2>&1|tr '\n' ' - ' `\"

/* Version make */
#define XXX_ABOUT_MAKE_VERSION \"`type make`: `make -v | head -1`\"

/* Usuario que hizo login al equipo para construir el server */
#define XXX_ABOUT_USERNAME \"`echo ${USERNAME}`\"

/* Usuario que lanza el build */
#define XXX_ABOUT_ID \"`id`\"

/* Variable PATH en ese momento */
#define XXX_ABOUT_PATH \"`echo ${PATH}`\"

/* Version de ubuntu */
#define XXX_ABOUT_UBUNTU \"`cat /etc/debian_version`\"

/* Informacion del modelo de CPU */
#define XXX_ABOUT_CPUINFO \"`cat /proc/cpuinfo |grep \"model name\"|head -1|cut -d: -f2`\"

#endif /* __XXX_ABOUT_H__ */
" > ${ABOUT}
    echo ${ABOUT} done!
}

#--------------------------------------------------------------------------------
# COPIXXDO LIBRERIAS
#--------------------------------------------------------------------------------
copyLib()
{
    path=${1}
    libname=${2}
    SOURCE=${XX_DIR}/${path}/${CONFIGURATION}/lib${libname}.so
    LIB_INSTXXCE_ID_SO=lib${libname}.so.${INSTXXCE_ID}
    LIB_SO=lib${libname}.so
    cd ${XXX_INSTALL}
    cp ${SOURCE} ${LIB_INSTXXCE_ID_SO}
    ln -fs ${LIB_INSTXXCE_ID_SO} ${LIB_SO}

    MD5SUM=${LIB_INSTXXCE_ID_SO}.md5
    md5sum ${LIB_INSTXXCE_ID_SO} > ${MD5SUM}
    echo "done! -> md5sum = `cat ${MD5SUM}`"
    cd -
}

#--------------------------------------------------------------------------------
# CONSTRUYENDO LAS LIBRERIAS
#--------------------------------------------------------------------------------
buildLib()
{
    # LIBXXX_UTIL.SO
    path=${1}
    cd ${path} && make CFG=${CONFIGURATION} ${BUILD} && cd -
    error=${?}
    if [ ${error} -ne 0 ]; then
        echo "Error ${error} al construir libreria ${path}"
        exit
    fi
    echo ${path} done!
}

#--------------------------------------------------------------------------------
# COPIXXDO EL SERVIDOR
#--------------------------------------------------------------------------------
copyServer()
{
    TARGET=${XXX_INSTALL}/${XX_YOU_DAEMON_NAME}
    cp ${XXX_DIR}/${CONFIGURATION}/${XXX_BLD_FILE} ${TARGET}
	strip ${TARGET}

    cd ${XXX_INSTALL}
    MD5SUM=${XX_YOU_DAEMON_NAME}.md5
    md5sum ${XX_YOU_DAEMON_NAME} > ${MD5SUM}
    echo "${XX_YOU_DAEMON_NAME} done! -> md5sum = `cat ${MD5SUM}`"
    cd -
}
#--------------------------------------------------------------------------------
# CONSTRUYENDO EL SERVIDOR
#--------------------------------------------------------------------------------
buildServer()
{
	touch some/src/XXXMain.cpp 
    cd some && make CFG=${CONFIGURATION} ${BUILD} && cd -
    error=${?}
    if [ ${error} -ne 0 ]; then
        echo "Error ${error} al construir ${XX_YOU_DAEMON_NAME}"
        exit
    fi
    echo ${anBroker} done!
}

#--------------------------------------------------------------------------------
# HACE PREGUNTAS (S/N) (S) es el default
#--------------------------------------------------------------------------------
getAnswer()
{
    prompt=${1}
    read -p "${prompt}? (S/N) [S] " -t 10 -n 1 doit
    if [ ${#doit} -eq 0 ]; then
        doit='S'
    else
        doit="`echo ${doit} | tr '[:lower:]' '[:upper:]'`"
    fi
    echo " "
    if [ $doit == S ]; then
        return 1
    fi
    return 0
}

#--------------------------------------------------------------------------------
# HACE UNA PREGUNTA C/UN VALOR X DEFAULT
#--------------------------------------------------------------------------------
getValue()
{
    prompt=${1}
    actual=${2}
    newvalue=""
    read -p "${prompt}? [actual = ${actual}] " -t 30 newvalue
    if [ ${#newvalue} -eq 0 ]; then
        echo "${actual}" | sed "s.\/.\\\/.g" | sed "s/\./\\\./g"
    fi
    echo "${newvalue}" | sed "s.\/.\\\/.g" | sed "s/\./\\\./g";
}


#--------------------------------------------------------------------------------
# CONSTRUIR EL SERVICIO PARA LXXZAR UN DAEMON
#--------------------------------------------------------------------------------
buildDaemonLinux()
{
	#
	# CONSTRUIR XXXD.IN
	# 

    CONFIG_PROG=${XX_YOU_DAEMON_NAME}
    CONFIG_PROG_ARGS=""
    CONFIG_XXX_INSTALL=${XXX_INSTALL}

    CONFIG_PROG=`getValue "Nombre del servidor" "${CONFIG_PROG}"`
    ${XXX_DIR}/${CONFIGURATION}/${XXX_BLD_FILE} -h
    CONFIG_PROG_ARGS=`getValue "Opciones o argumentos de linea de comando para ${CONFIG_PROG}" "${CONFIG_PROG_ARGS}"`

    echo "#"
    echo "#${CONFIG_PROG} Variables de Ambiente."
    echo "#"
    echo "# PATH:"
    CONFIG_XXX_INSTALL=`getValue "Directorio donde va a correr el programa ${CONFIG_PROG}" "${CONFIG_XXX_INSTALL}"`
    XXX_INSTALL=$(echo ${CONFIG_XXX_INSTALL} | sed "s.\\\/.\/.g" | sed "s/\\\./\./g")
    export XXX_INSTALL
    if [ ! -d ${XXX_INSTALL} ]; then
        mkdir -p ${XXX_INSTALL}
    fi

    getAnswer "Copiar ejecutables y librerias a ${XXX_INSTALL}"
    if [ $? -eq 1 ]; then
        copyLib util anutil
        copyLib db anotlmysql
        copyServer
    fi
    CONFIG_LD_LIBRARY_PATH=${CONFIG_XXX_INSTALL}:$(echo "/usr/lib/odbc" | sed "s.\/.\\\/.g" | sed "s/\./\\\./g")
    echo "# MYSQL:"
    CONFIG_XXX_USERNAME=${XXX_USERNAME}
    CONFIG_XXX_PASSWORD=${XXX_PASSWORD}
    CONFIG_XXX_DATABASE=${XXX_DATABASE}

    CONFIG_XXX_USERNAME=`getValue "Usuario MySql" "${CONFIG_XXX_USERNAME}"`
    CONFIG_XXX_PASSWORD=`getValue "Password MySql" "${CONFIG_XXX_PASSWORD}"`
    CONFIG_XXX_DATABASE=`getValue "Servidor MySql" "${CONFIG_XXX_DATABASE}"`
    CONFIG_PID_FILE=${XXX_INSTALL}/${CONFIG_PROG}d.pid
    CONFIG_PID_FILE=`getValue "pid (process id) file" "${CONFIG_PID_FILE}"`
    CONFIG_LOCKFILE=${XXX_INSTALL}/${CONFIG_PROG}d.lock
    CONFIG_LOCKFILE=`getValue "lock file" "${CONFIG_LOCKFILE}"`
    CONFIG_LOG_DIR=${XXX_INSTALL}/log
    CONFIG_LOG_DIR=`getValue "directorio para archivos de log" "${CONFIG_LOG_DIR}"`
    CONFIG_LOG_FILE=${XX_YOU_DAEMON_NAME}.log
    CONFIG_LOG_FILE=`getValue "nombre de log" "${CONFIG_LOG_FILE}"`
    CONFIG_INSTXXCE_ID=${INSTXXCE_ID}
    CONFIG_INSTXXCE_ID=`getValue "instance id" "${CONFIG_INSTXXCE_ID}"`
    CONFIG_XXX_SLEEP_TIME=${XXX_SLEEP_TIME}
    CONFIG_XXX_SLEEP_TIME=`getValue "sleep time (Segundos)" "${CONFIG_XXX_SLEEP_TIME}"`
    CONFIG_MSG="# AUTOGENERADO: Este archivo fue generado por otro proceso (buildme.sh), \\n# y es preferible volverlo a generar y no modificarlo."

	#
	# CONSTRUIR SETENV
	# 
    SETENV_SH_IN=${XXX_DIR}/setenv.in
    SETENV_SH_OUT=${XXX_INSTALL}/setenv.sh
    rm -f ${SETENV_SH_OUT}
    cat ${SETENV_SH_IN} \
        | sed "s/@ESTA_ES_UNA_PLXXTILLA@/\"${CONFIG_MSG}\"/g" \
        | sed "s/\"# AUTOGENERADO/\# AUTOGENERADO/g" \
        | sed "s/@CONFIG_LD_LIBRARY_PATH@/"${CONFIG_LD_LIBRARY_PATH}"/g" \
        | sed "s/@CONFIG_XXX_INSTALL@/"${CONFIG_XXX_INSTALL}"/g" \
        | sed "s/@CONFIG_PID_FILE@/"${CONFIG_PID_FILE}"/g" \
        | sed "s/@CONFIG_LOG_DIR@/"${CONFIG_LOG_DIR}"/g" \
        | sed "s/@CONFIG_LOG_FILE@/"${CONFIG_LOG_FILE}"/g" \
        | sed "s/@CONFIG_XXX_USERNAME@/"${CONFIG_XXX_USERNAME}"/g" \
        | sed "s/@CONFIG_XXX_PASSWORD@/"${CONFIG_XXX_PASSWORD}"/g" \
        | sed "s/@CONFIG_XXX_DATABASE@/"${CONFIG_XXX_DATABASE}"/g" \
        | sed "s/@CONFIG_INSTXXCE_ID@/"${CONFIG_INSTXXCE_ID}"/g" \
        | sed "s/@CONFIG_XXX_SLEEP_TIME@/"${CONFIG_XXX_SLEEP_TIME}"/g" \
        > ${SETENV_SH_OUT}
    if [ $? -eq 0 ]; then
        echo "${SETENV_SH_OUT} generado correctamente"
        chmod +x ${SETENV_SH_OUT}
    else
        echo "${SETENV_SH_OUT} con errores"
    fi

    XXXD_IN=${XXX_DIR}/anBrokerd.in
    XXXD_OUT=${XXX_INSTALL}/anBroker${INSTXXCE_ID}d
    rm -f ${XXXD_OUT}
    cat ${XXXD_IN} \
        | sed "s/@ESTA_ES_UNA_PLXXTILLA@/\"${CONFIG_MSG}\"/g" \
        | sed "s/\"# AUTOGENERADO/\# AUTOGENERADO/g" \
        | sed "s/@CONFIG_PROG@/"${CONFIG_PROG}"/g" \
        | sed "s/@CONFIG_PROG_ARGS@/\"${CONFIG_PROG_ARGS}\"/g" \
        | sed "s/@CONFIG_XXX_INSTALL@/"${CONFIG_XXX_INSTALL}"/g" \
        | sed "s/@CONFIG_LOCKFILE@/"${CONFIG_LOCKFILE}"/g" \
        | sed "s/@CONFIG_INSTXXCE_ID@/"${CONFIG_INSTXXCE_ID}"/g" \
        > ${XXXD_OUT}
    if [ $? -eq 0 ]; then
        echo "${XXXD_OUT} generado correctamente"
        chmod +x ${XXXD_OUT}
    else
        echo "${XXXD_OUT} con errores"
    fi
}

#--------------------------------------------------------------------------------
# MAIN
#--------------------------------------------------------------------------------
main()
{
    echo "CONFIGURATION=${CONFIGURATION} en ${XXX_INSTALL}..."
    mkdir -p ${XXX_INSTALL}
    about
    getAnswer "with clean"
    if [ $? -eq 1 ]; then
        BUILD="clean all"
    fi
    getAnswer "Build util"
    if [ $? -eq 1 ]; then
        buildLib util 
    fi
    getAnswer "Build otl"
    if [ $? -eq 1 ]; then
        buildLib db 
    fi

    getAnswer "Build server"
    if [ $? -eq 1 ]; then
        buildServer
    fi
    getAnswer "Construir script del server"
    if [ $? -eq 1 ]; then
        buildDaemonLinux
    fi
}

main

#--------------------------------------------------------------------------------
# LISTO
#--------------------------------------------------------------------------------
echo "CONFIGURATION=${CONFIGURATION} en ${XXX_INSTALL} - done!"

