#!/bin/ksh
# Program:    exportOracleScheme.sh
# Type:       Shell Script
# Arguments:  -
# Obs.:       Export oracle database using exp + synonyms and sequences.
#
# Aug/2008: Javier Fuchs                                    |\_/|   
#           Phone (+54 2944) 525173                        /     \  
#           http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                            \@/    

username=scott
password=tigger
name_of_file=$(uname -n).${username}.$(date +'%Y%m%d')

tables=$(sqlplus -S ${username}/${password} <<buffer
SET HEADING OFF;
SET LINE 256;
SET TAB OFF;
SET FEEDBACK OFF;
SET NEWPAGE NONE;
SELECT TABLE_NAME FROM USER_TABLES;
QUIT;
buffer
)
tables_good=$(echo ${tables}|tr ' ' ',')
exp ${username}/${password} FILE=${name_of_file}.dmp TABLES=${tables_good}
bzip2 ${name_of_file}.dmp

sqlplus -S ${username}/${password} <<buffer
SET HEADING OFF;
SET LINE 256;
SET TAB OFF;
SET FEEDBACK OFF;
SET NEWPAGE NONE;
SPOOL ${name_of_file}.sql
SELECT 'CREATE SYNONYM '|| SYNONYM_NAME || ' FOR ' || TABLE_NAME || ';' FROM USER_SYNONYMS;
SELECT 'CREATE SEQUENCE '|| SEQUENCE_NAME || ' START WITH ' || to_char(LAST_NUMBER) || ' MAXVALUE ' || to_char(MAX_VALUE) || ' MINVALUE ' || to_char(MIN_VALUE) || ' NOCYCLE NOCACHE NOORDER;' FROM USER_SEQUENCES;
QUIT;
buffer

echo "QUIT;" >> ${name_of_file}.sql
