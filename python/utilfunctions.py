# Program:    utilfunctions.py
# Type:       Python Script

#   Feb/2005: Javier Fuchs                                    |\_/|   
#             Phone (+54 2944) 525173                        /     \  
#             http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                              \@/    

#
# Util Functions
# 

import sys
import os
import string
import time
import logging
import re

#
# GLOBAL vars
# 
RunDict= dict({ 'FUCHS_DIR': os.getenv('FUCHS_DIR') + '/', 'FUCHS_OUT_DIR' :os.getenv('FUCHS_OUT_DIR') + '/' , 'FUCHS_LOG_DIR': os.getenv('FUCHS_LOG_DIR') + '/'})

LogLevelMax=9
LogLevelConsoleMax=4


#
# Get environment
#
def getenv(aKey, aDefault=None):
	return os.environ.get(aKey,aDefault)

#  SYNOPSYS: return aString with all dangerous values escaped for inclusionbetween " "
def escapeStr(aString):
	# XXX: write real implementation
	return str(aString)

#
#  NAME: getcalled
#  SYNOPSIS: Getting a function name from within itself
#            from http://mail.python.org/pipermail/tutor/2001-June/006440.html
#  RETURN VALUE: string containing name of a python function
#
def getcalled():
	try: raise SystemError
	except SystemError:
		exc, val, tb = sys.exc_info()
		# from the traceback, get the second frame back (first is getcalled)
		caller = tb.tb_frame.f_back
		# delete the traceback object to free memory
		del exc, val, tb
		return caller.f_code.co_name

#
#  NAME: basename
#  SYNOPSIS: strip directory and suffix from filenames or pathnames
#  RETURN VALUE: string with a directory name
#  EXAMPLES: 
#		str=basename("a/b/c.txt");
#		# str will contain "c.txt"
#
def basename(aPath):
	m = re.compile("([^\/]+)$").search(aPath)
	return m.group(0)


def logm(aType, aLevel, aMsg):
	if aLevel<LogLevelMax:
		logging.info("L=" + str(aLevel) + ":T=" + aType + ":" + aMsg)
		if aLevel<LogLevelConsoleMax:
			print ":T=" + aType + ":" + aMsg
			sys.stdout.flush()

#
#  NAME: setVar
#  SYNOPSIS: get var using 'key' from global dictionary 
#  RETURN VALUE: result with 'val'
#  EXAMPLES:
#  EXCEPTIONS: -
#
def setVar(aDict):
	logm("DBG",5,"ACTION " + getcalled()+" "+ str(aDict))
	global RunDict
	msg=""
	if os.environ.has_key(aDict["key"]) and aDict.get("override",0)!="true": 
		msg="NOT (environ)"
	else:
		RunDict[aDict["key"]]= expandVars(aDict["val"])
	logm("DBG",5,"ENV " + getcalled() + " "+msg+" SET key='" + aDict["key"] + "' val='" + aDict["val"] +"' Run="+str(RunDict)) 



#
# UTILITY FUNCTIONS
#

def getVar(key,dflt,overrideEnv,allowUndefined):
	"DESC: gets 'val' @ 'key' from test environment or os env, using 'dflt' if not present. os env takes precedence unless 'overrideEnv' is defined. Raises an exception if undefined and 'allowUndefined' is not set"
	global RunDict
	if not (allowUndefined or RunDict.has_key(key) or os.environ.has_key(key)):
		raise TestSpecException, "var not defined KEY='" + str(key) +"' and allowUndefined is false"

	result=""
	if overrideEnv!='':
		result= RunDict.get(key,getenv(key,dflt))
	else:
		result= getenv(key,RunDict.get(key,dflt))
	RunDict[key]= result #save, ej. for dumps, etc.
	return result

def expandVars(val,defaultValue ='',overrideEnv='',allowUndefined=''):
	"""DESC: expand all the vars in 'string' looking in the OS environment and then in the global dictionary
		If some values are not defined and 'allowUndefined' is false raises an exception, else defaultValue is used.
		OS environment values take precedence unless 'overrideEnv' is specified
	EXAMPLE: 
		expandVars("hello, $Who logged in as $UID") -> hello, Mauricio logged in as 1000
		expandVars("${LParen}hello$RParen") -> (hello)"""
	se= val # default, no vars, preserve type

	string= str(val)
	if r'$' in string:
		#XXX: FIXME, may have $ but still no variables to expand
		global RunDict
		m= re.match(r'^\$([\w\d]+)$|\$\{([\w\d]+)\}$', string)
		if m:
			#A: 'exact' variable expansion, try to keep var type
			se= getVar(m.group(1) or m.group(2),defaultValue,overrideEnv,allowUndefined)
		else:
			#A: vars mixed with text, all converted to string
			def repl(match,defaultValue=defaultValue,overrideEnv=overrideEnv,allowUndefined=allowUndefined): 
					return str(getVar(match.group(1) or match.group(2),defaultValue,overrideEnv,allowUndefined))
			se= re.sub(r'\$([\w\d]+)|\$\{([\w\d]+)\}',repl, string)

	return se



