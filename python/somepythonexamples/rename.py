#!/usr/bin/env python

import sys;
import os;

if __name__ == "__main__": 
	fd = file(sys.argv[1], "rb")
    	changeDict = dict()
	for buffer in fd.readlines():
		changeDict[buffer.split()[0]] = buffer.split()[1];
	fd.close();
	print changeDict;

    	for afilename in sys.argv[2:]:
		anewfilename = afilename;
		for s,r in changeDict.iteritems():
			anewfilename = afilename.replace(s,r);
		if anewfilename != afilename:
			print 'renaming: ' + afilename + ' to ' + anewfilename;
			os.rename(afilename, anewfilename);

