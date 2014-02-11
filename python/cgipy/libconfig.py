#
#   Aug/2008: Javier Fuchs                                    |\_/|   
#             Phone (+54 2944) 525173                        /     \  
#             http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                              \@/    
menupage = {'path': "cgipy",
            'file':'menu.py',
            'background': '#000059',
            'frameborder': 1,
            'marginwidth': 0,
            'marginheight': 0,
            'noresize': True};

homepage = {'path':'cgipy',
            'file':'home.py',
            'background': '#000059',
            'frameborder': 1,
            'marginwidth': 10,
            'marginheight': 5,
            'noresize': True,
            'name': 'page',
            'id': 'page'};

hrefhome = { 'href':"cgipy/home.py", 'a':"home"};
hrefbio = { 'href':"cgipy/bio.py", 'a':"bio"};
hrefmyown = { 'href':"cgipy/myown.py", 'a':"myown"};
hrefc = { 'href':"cgipy/c.py", 'a':"C"};
hrefcpp = { 'href':"cgipy/cpp.py", 'a':"C++"};
hrefunix = { 'href':"cgipy/unix.py", 'a':"Unix/Linux"};
hreforacle = { 'href':"cgipy/oracle.py", 'a':"Oracle"};
hrefbooks = { 'href':"cgipy/books.py", 'a':"Books"};

hrefmeteored = {'title': "El Tiempo en Bariloche Aerodrome", 'href':"http://tiempo.meteored.com/prediccion_para-Bariloche Aerodrome-Bariloche Aerodrome-Argentina-America Sur-SAZS.html", 'target':'_blank', 'img':{'src':"http://tiempo.meteored.com/tablas/meteored.tab_Bariloche Aerodrome [Bariloche Aerodrome;Argentina].jpg", 'alt':"El Tiempo en Bariloche Aerodrome - Predicción a 7 días y condiciones actuales."}};

hrefwebcam = {'title': "Lago Nahuel Huapi", 'target':'_blank', 'href':"http://www.bariloche.com.ar/camaras/lago-nahuel-huapi.html", 'img':{'src':"http://www.bariloche.com.ar/camara/nahuelhuapi.jpg", 'alt':"Nahuel Huapi"}};

hrefmenu = { 'home': hrefhome, 'bio': hrefbio, 'myown': hrefmyown, 'c': hrefc, 'cpp': hrefcpp, 'unix': hrefunix, 'oracle': hreforacle, 'books': hrefbooks};

hrefall = {};
for k, v in hrefmenu.iteritems():
    hrefall[k] = v

hrefall['webcam'] = hrefwebcam;
hrefall['meteored'] = hrefmeteored;



page = {'menu': menupage,
        'home': homepage};

tmp = "/tmp";


def starthtmlhead(dtd = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN http://www.w3.org/TR/html4/strict.dtd">'):
    # Required header that tells the browser how to render the text.
    print 'Content-type: text/html\n\n';

    print dtd;
    print '<head>';
    print '<BASE href="http://zorrodelsur.servehttp.com/cgipy">';
    print '<link rel="shortcut icon" type="image/x-icon" href="img/python.gif">';
    print '<meta name="keywords" content="c++, c, linux, unix, oracle, javier, fuchs, javier fuchs, programming, telecomunicaciones">';
    print '<meta http-equiv="Content-Language" content="us">';
    print '<META http-equiv="Expires" content="Thu, 14 Apr 2005 11:59:56 UTC">';
    print '<meta name=author content="Javier Fuchs">';
    print '<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">';
    print '<META http-equiv="Content-Language" content="en-US">';

def mail():
    print '<B>javier.fuchs</B> at <B>gmail.com</B>';

def myaddress():
    print '  <ADDRESS>' + '    I can be reached by email: ';
    mail();
    print '  </ADDRESS>';

def copyright():
    print 'Copyright &#169; 2003/2008 Javier Fuchs';

def linegif():
    print '<img src="img/line.gif" width="152" height=1>';

def bottomline():
    print '  <P>';
    print '  </P>';
    print '';
    print '  <TABLE width="100%" cellpadding="0" cellspacing="0">';
    print '    <TR>';
    print '      <TD width="100%" valign="top" id="HEADLINE" align="right"><IMG src="img/line.gif" alt="-" longdesc="-" link="" width="100%" height="1"><SPAN class="c2">';
    copyright();
    print '</SPAN></TD>';
    print '    </TR>';
    print '  </TABLE>';


def printcss(what):
    print '  <LINK rel="stylesheet" href="../css/' + what + '.css" type="text/css">';

def pagecss():
    printcss('page');

def menucss():
    printcss('menu');


def printIMG(img):
        if img.has_key('border'):
            border = str(img['border']);
        else:
            border = '0';
        print '   <img border="'+ border +'" ';
        print '    src="' + img['src'] + '"';
        if img.has_key('width'):
            print '    width=' + str(img['width']);
        if img.has_key('height'):
            print '    height=' + str(img['height']);
        print '    alt="' + img['alt'] + '">';

def printHREF(options, separator = ''):
    for option in options:
        print '<A ';
        if option.has_key('title'):
            print 'title="' + option['title'] + '"';
        if option.has_key('type'):
            print '   type="' + option['type'] + '"';
        if option.has_key('rel'):
            print '   rel="' + option['rel'] +'"';

        if option.has_key('target'):
            target = option['target'];
            print '   target="' + option['target'] + '"';
        print '   href="' + option['href'] +'">';
        if option.has_key('a'):
            print '<b>' + option['a'] + '</b>';
        if option.has_key('img'):
            printIMG(option['img']);
        print '</a>';
        print separator;

