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
    libconfig.pagecss();
    print '  <META name="DESCRIPTION" content="This is the homepage of Javier Fuchs">';
    print '';
    print '  <TITLE>Javier Fuchs Homepage</TITLE>';
    print '';
    print '<STYLE type="text/css">';
    print ' span.c2 {font-size: 80%}';
    print ' div.c1 {text-align: center}';
    print '</STYLE>';
    print '<style>';
    print 'div.camera {';
    print 'padding:0.5em;';
    print 'border-style:solid;';
    print 'border-width:2px;';
    print 'float:right';
    print '}';
    print '</style>';
    print '</HEAD>';
    print '';
    print '<BODY>';
    print '  <DIV class="c1">';
    print '    <H1>Welcome to Javier Fuchs homepage!</H1>';
    print '  </DIV>';
    print '';
    print '  <p>';
    print '  <div class=camera>';
    print '<!-- www.meteored.com --><a ';
    print '    target="_blank"';
    print '    ';
    # www.meteored.com 
    libconfig.printHREF([\
            {'title': "El Tiempo en Bariloche Aerodrome", 'href':"http://tiempo.meteored.com/prediccion_para-Bariloche Aerodrome-Bariloche Aerodrome-Argentina-America Sur-SAZS.html", 'target':'_blank', 'img':{'src':"http://tiempo.meteored.com/tablas/meteored.tab_Bariloche Aerodrome [Bariloche Aerodrome;Argentina].jpg", 'alt':"El Tiempo en Bariloche Aerodrome - Predicción a 7 días y condiciones actuales."}}, \
            {'title': "Lago Nahuel Huapi", 'target':'_blank', 'href':"http://www.bariloche.com.ar/camaras/lago-nahuel-huapi.html", 'img':{'src':"http://www.bariloche.com.ar/camara/nahuelhuapi.jpg", 'alt':"Nahuel Huapi"}}]);
    print '  </div>';
    print '  <p>';
    print '';
    print '  <P>I work in programming and administration.</P>';
    print '';
    print '  <P>My skill portfolio includes the following:</P>';
    print '';
    print '  <UL>';
    print '    <LI>Databases: ';
    libconfig.printHREF([libconfig.hrefall['oracle']]);
    print ' 7.x 8i 9, PostgreSQL, and MySQL</LI>';
    print '';
    print '    <LI>PL/SQL and SQL</LI>';
    print '';
    print '    <LI>PRO*C</LI>';
    print '';
    print '    <LI>Scripting: shell (Bash, Korn), enough Python, and a little of Perl.</LI>';
    print '';
    print '    <LI>Unix: AIX, Solaris 7 8 10, SCO, HP-UX, Tru64, Unisys</LI>';
    print '';
    print '    <LI>Linux: RedHat 6.2, 7.x, 8.0, 9.0, Suse 7.x, Fedora Core 2</LI>';
    print '';
    print '    <LI>';
    libconfig.printHREF([libconfig.hrefall['c']]);
    print 'and ';
    libconfig.printHREF([libconfig.hrefall['cpp']]);
    print '(under ';
    libconfig.printHREF([libconfig.hrefall['unix']]);
    print ', DOS, Windows, VMS, Tandem, AS/400)</LI>';

    print '';
    print '    <LI>Java: JSP, Servlets, Struts, J2EE (EJB BMPs and CMPs, Session Beans)</LI>';
    print '';
    print '    <LI>RPG/COBOL</LI>';
    print '';
    print '    <LI>Web: HTTP (Apache), JSP (Tomcat), J2EE (JBoss, Weblogic, Websphere)</LI>';
    print '';
    print '    <LI>IDEs: Visual Studio 5/6, JBuilder, Eclipse, IntelliJIdea.</LI>';
    print '';
    print '    <LI>Python, Perl, PHP.</LI>';
    print '  </UL>';
    print '';
    print '  <P>Over the years, I have worked in computers for several companies: ';
    libconfig.printHREF([libconfig.hrefall['bio']]);
    print '.</P>';
    print '';
    print '  <P>What I like better than anything else is code, write efficient and reusable software components, in every platform, working with abstraction and flexibility.</P>';
    print '';
    print '  <P>These pages are permanently under construction.<BR>';
    print '  Constructive comments are most welcome.</P>';
    print '';
    libconfig.myaddress();
    print '';
    libconfig.bottomline();
    print '</BODY>';
    print '</HTML>';

main();
