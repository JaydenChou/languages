#!/usr/bin/env python
# Program:    createmenufromgnometofluxbox.py
# Type:       Python Script
# Obs.:       creates a fluxbox menu searching recursively every program in the menu of gnome. This menu is printed in the standard output.
# Arguments:  1
#             Path where the files are
# Return:     -
#   Feb/2005: Javier Fuchs                                    |\_/|   
#             Phone (+54 2944) 525173                        /     \  
#             http://southfox.servehttp.com                 /_.~ ~,_\ 
#                                                              \@/    
import sys
import os


def find_icon(path, name):
    #print "searching for", name, " in ", path
    for root, dirs, files in os.walk(path+'/'):
        #print files
        for file in files:
            if name in file:
                #print '[' + path + '/' + file + ']'
                return path + '/' + file
        for dir in dirs:
            rta = find_icon(root + dir, name) 
            if rta != None:
                return rta


def grep_the_file(the_file, cat):
    fd = file(the_file, "rb")
    big_buffer = ""
    app = dict()
    for buffer in fd.readlines():
        if "Categories" in buffer or "Exec" in buffer or "Icon=" in buffer or "Name=" in buffer and "GenericName" not in buffer:
            b = buffer.split('=')
            app[b[0].strip()] = b[1].strip()
        if "Categories" in buffer:
            if app.has_key('Name'):
                cat[app['Name']] = buffer.split('=')[1].strip().split(';')
    fd.close()

    return app


if __name__ == "__main__": 
    cat = dict()
    menu = dict()
    for the_file in sys.argv[1:]:
        menu[the_file] = grep_the_file(the_file, cat)

    print "[begin] (SubMenu)"
    print "	[submenu] (Others)"
    listofcat = dict()
    for app in cat:
        for nameofcat in cat[app]:
            if nameofcat.strip() != "":
                if nameofcat not in listofcat.values():
                    listofcat[nameofcat] = 1
    keys = listofcat.keys();
    keys.sort()
    for nameofcat in keys:
        if nameofcat not in ["Utility","Game","Network","System","Development","Game","CardGame","Settings","BoardGame","Graphics","AudioVideo","LogicGame","Office"]: continue
        print "		[submenu] (" + nameofcat + ")"
        for k in menu.keys():
            if menu[k].has_key('Categories'):
                if nameofcat in menu[k]['Categories']:
                    buffer = "			[exec] (" + menu[k]['Name'].strip() + ") {" + menu[k]['Exec'].strip() + "}"
                    if menu[k].has_key('Icon'):
                        pathIcon = nameIcon = menu[k]['Icon'].strip()
                        if not os.path.isfile(pathIcon):
                            pathIcon = '/usr/share/icons/hicolor/48x48/apps/' + nameIcon
                            if not os.path.isfile(pathIcon):
                                pathIcon = find_icon('/usr/share/icons', nameIcon)
                                if pathIcon != None:
                                    nameIcon = pathIcon
                        buffer += " <" + nameIcon + ">"
                    print buffer
        print "		[end]"
    print """
	[submenu] (Fluxbox Menu)
		[config] (Configure)
		[submenu] (System Styles) {Choose a style...}
			[stylesdir] (/usr/share/fluxbox/styles)
			[stylesdir] (/usr/share/commonbox/styles/)
		[end]
		[submenu] (User Styles) {Choose a style...}
			[stylesdir] (~/.fluxbox/styles)
		[end]
		[workspaces]   (Workspace List)
		[submenu] (Tools)
			[exec] (Window name) {xprop WM_CLASS|cut -d " -f 2|xmessage -file - -center}
			[exec] (Screenshot - JPG) {import screenshot.jpg && display -resize 50% screenshot.jpg}
			[exec] (Screenshot - PNG) {import screenshot.png && display -resize 50% screenshot.png}
			[exec] (Run) {fbrun }
			[exec] (Regen Menu) { createmenufromgnometofluxbox.py /usr/share/applications/*.desktop > ~/.fluxbox/my-others}

		[end]
		[exec] (Lock screen) {xscreensaver-command -lock}
		[commanddialog] (Fluxbox Command)
		[reconfig] (Reload config)
		[restart] (Restart)
		[separator]
		[exit] (Exit)
	[end]
    """
    print "	[end]"
    print "[end]"

