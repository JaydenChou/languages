#!/usr/bin/python
#
#   Aug/2008: Javier Fuchs                                    |\_/|   
#             Phone (+54 2944) 525173                        /     \  
#             http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                              \@/    
import cgi;
import os;
import sys;
import cgitb; cgitb.enable();

import libconfig;

def main():
    libconfig.starthtmlhead();

    print '<meta name="CHARSET" content="ISO-8859-1">'
    print '<link rel="stylesheet" href="css/menu.css" type="text/css">'
    print '<base target=page>'
    print '</head>'
    print '<HTML>';
    print '<FRAMESET background="#000059" cols="200,*">';
    for pagename, pagedict in libconfig.page.iteritems():
        aux = '  <FRAME src="' + pagedict['path'] + os.sep + pagedict['file'] + '" ';
        aux += 'background="' + pagedict['background'] + '" ';
        aux += 'frameborder="' + str(pagedict['frameborder']) + '" ';
        aux += 'marginwidth="' + str(pagedict['marginwidth']) + '" ';
        aux += 'marginheight="' + str(pagedict['marginheight']) + '" ';
        if pagedict.has_key('name'):
            aux += 'name="' + pagedict['name'] + '" ';
        if pagedict.has_key('id'):
            aux += 'id="' + pagedict['id'] + '" ';
        if pagedict['noresize']:
            aux += 'noresize ';
        aux += '>';
        print aux;
    print '';
    print '  <NOFRAMES>';
    print '    <BODY>';
    print '      I"m sorry your navigator not support frames.';
    print '    </BODY>';
    print '  </NOFRAMES>';
    print '</FRAMESET>';
    print '</HTML>';

main();
