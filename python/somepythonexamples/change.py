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
		if os.path.isdir(afilename):
			continue;
		print afilename;
		anewfilename = afilename + '_new';
		fd = file(afilename, "rb")
		fdw = file(anewfilename, "wb")
		for buffer in fd.readlines():
			for s,r in changeDict.iteritems():
				buffer = buffer.replace(s,r);
			fdw.write(buffer);
		fd.close();
		fdw.close();
		os.rename(anewfilename, afilename);


