#!/bin/sh
#**************************************************************************
# File: configure.in
# 
# History:
#   Feb/2005: Javier Fuchs                                    |\_/|   
#             Phone (+54 2944) 525173                        /     \  
#             http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                              \@/    
#
sDirectory=novel
for sDirNovel in $(ls ${sDirectory}); do
	./buildBooksNovels ${sDirectory}/${sDirNovel} en > BookEn_${sDirectory}_${sDirNovel}.html
	./buildBooksNovels ${sDirectory}/${sDirNovel} sp > BookSp_${sDirectory}_${sDirNovel}.html
done
sDirectory=tech
for sDirTech in $(ls ${sDirectory}); do
	./buildBooksTechnical ${sDirectory}/${sDirTech} en > BookEn_${sDirectory}_${sDirTech}.html
	./buildBooksTechnical ${sDirectory}/${sDirTech} sp > BookSp_${sDirectory}_${sDirTech}.html
done
