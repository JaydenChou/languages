#!/usr/bin/env python
# -*- coding: iso-8859-15 -*-

import sys;
import os;

changeDict = dict();

def change(afilename):
	anewfilename = afilename + '_new';
	fd = file(afilename, "rb");
	fdw = file(anewfilename, "wb");
	for buffer in fd.readlines():
		for s,r in changeDict.iteritems():
			buffer = buffer.replace(s,r);
		fdw.write(buffer);
	fd.close();
	fdw.close();
	os.rename(anewfilename, afilename);

#import os
#for root, dirs, files in os.walk(top, topdown=False):
#    for name in files:
#        os.remove(os.path.join(root, name))
#    for name in dirs:
#        os.rmdir(os.path.join(root, name))
#

def find_file(path):
	for root, dirs, files in os.walk(path+'/', topdown=False):
		if ".svn" in root:
			continue;
        	for name in files:
                		print os.path.join(root, name);
				change(os.path.join(root, name));

if __name__ == "__main__": 
	changeDict = dict();
	changeDict['Lib'] = 'Lib';



	print changeDict;
	find_file(sys.argv[1]);
	




