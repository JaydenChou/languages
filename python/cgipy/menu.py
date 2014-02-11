#!/usr/bin/python
#
#   Aug/2008: Javier Fuchs                                    |\_/|   
#             Phone (+54 2944) 525173                        /     \  
#             http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                              \@/    
import cgi;
import os;
import sys;
import time;
import cgitb; cgitb.enable();

import libconfig;


def main():
    libconfig.starthtmlhead();
    libconfig.menucss();
    print '<base target=page>';
    print '</head>'
    print '<HTML>';
    print '<body bgcolor="#000059" link="#404040" vlink="#404040" alink="#404040">';
    print '<table border=0 cellpadding=0 cellspacing=0 align=center width=200 frame=void>';
    print '<tr><td bgcolor="#000059" height="100%" width=18>&nbsp;</td><td bgcolor="#000059" width=152 valign=top>';
    print '<center>';
    print '<br>';
    print 'Welcome!!!!!';
    print '<br>';

    gifhtml = { 'src':"img/html.gif",
                'width':30,
                'height':30,
                'alt':"using html" }

    gifpy = { 'src':"img/python.gif",
              'width':30,
              'height':30,
              'alt':"using python cgi" }

    optionhtml = { 'title':"using html",
                'type':"text/html",
                'rel':"alternate",
                'target':"_top",
                'href':"http://zorrodelsur.servehttp.com/html",
                'img':gifhtml};

    optionpy = { 'title':"using python cgi",
                'type':"text/html",
                'rel':"alternate",
                'target':"_top",
                'href':"http://zorrodelsur.servehttp.com/cgipy/index.py",
                'img':gifpy};

    libconfig.printHREF([optionpy, optionhtml]);

    print '<br>';
    print '</center>';
    
    print '<font face="verdana,sans-serif" size=2 >';

    libconfig.linegif();
    print '<br>';

    libconfig.printHREF(libconfig.hrefmenu.values(), '<br>');

    print '</font>';

    libconfig.linegif();
    print '<font face="verdana,sans-serif" size=1 >';

    print '<address>';
    libconfig.copyright();
    print '</font>';
    print '<br>';
    libconfig.mail();
    print '</address>';

    print '<p>';
    print '<font face="verdana,sans-serif" size=1>';
    lastUpdate = time.localtime(os.stat('menu.py').st_ctime);
    print 'Last update: ' + str(lastUpdate[0]) + '/' + str(lastUpdate[1]) + '/' + str(lastUpdate[2]) \
        + ' ' + str(lastUpdate[3]) + ':' + str(lastUpdate[4]) + ':' + str(lastUpdate[5]);
    print '</font>';
    print '<p>';
    print '<center>';
    print 'Powered by:';
    print '<br>';
    print '<font face="verdana,sans-serif" size=2 >';

    gifVi = { 'src':"http://www.vim.org/images/vim_edited_animated_love.gif",
              'alt':"Vi IMproved version 6.1.320" };

    poweredVi = { 'title':"Vi IMproved version 6.1.320", 
            'target':"_blank",
            'href':"http://www.vim.org",
            'img':gifVi};

    gifFedora = { 'src':"img/powered_by_fedora.png",
              'alt':"Fedora Core 2" };

    poweredFedora = { 'title':"Fedora Core 2", 
            'target':"_blank",
            'href':"http://fedora.redhat.com",
            'img':gifFedora};

    gifHtmlValid = { 'src':"http://www.w3.org/Icons/valid-html401", 'target':"_blank",
              'height':31,
              'width':88,
              'alt':"Valid HTML 4.01!" };
    poweredHtmlValid = { 'title':"Valid HTML 4.01!", 
            'target':"_blank",
            'href':"http://validator.w3.org/check?uri=http://zorrodelsur.servehttp.com",
            'img':gifHtmlValid};

    gifLUser = { 'src':"img/lxuser.jpg",
              'alt':"Linux User #326182" };
    poweredLUser = { 'title':"Linux User #326182", 
            'target':"_blank",
            'href':"http://counter.li.org",
            'img':gifLUser};

    gifViewable = { 'src':"http://www.anybrowser.org/campaign/bvgraphics/logoab8.gif",
              'alt':"Viewable With Any Browser" };
    poweredViewable = { 'title':"Viewable With Any Browser", 
            'target':"_blank",
            'href':"http://www.anybrowser.org/campaign/",
            'img':gifViewable};

    gifApache = { 'src':"img/apache.gif",
              'alt':"Apache HTTP Server" };
    poweredApache = { 'title':"Apache HTTP Server", 
            'target':"_blank",
            'href':"http://www.apache.org",
            'img':gifApache};

    gifSeti = { 'src':"http://setiathome.berkeley.edu/user_images/brentcox.gif",
              'alt':"Supporting Seti@home" };
    poweredSeti = { 'title':"Supporting Seti@home", 
            'target':"_blank",
            'href':"http://setiathome.berkeley.edu/view_profile.php?userid=294019",
            'img':gifSeti};

    libconfig.printHREF([poweredApache, poweredViewable, poweredLUser, poweredHtmlValid, poweredFedora, poweredVi], '<br>');
    print 'Supporting:';
    libconfig.printHREF([poweredSeti]);

    print '</font>';
    print '</center>';
    print '</td></tr>';
    print '</TABLE>';
    print '<br>';
    libconfig.linegif();
    print '<br>';
    print '<br>';
    print '<font size=+1>';
    print '</font>';
    print '';
    print '</body>';
    print '</html>';


main();
