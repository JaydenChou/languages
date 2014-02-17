import sys
import os
import re
import shutil;
import mimetypes;
import zipfile;
import biplist;
import imghdr;


from tempfile import NamedTemporaryFile;
from pylons import config;

import cocoamilk.lib.helpers as h;
import cocoamilk.lib.sgmlparser as sgml;
from cocoamilk.model.breadEntry import BreadEntry;



def getInfo(filepointer):
    h.logm("DBG",5,"ACTION "+ h.getcalled());
    
    # Validate BREAD extension
    if not 'bread' in filepointer.filename:
        return (False, None, "bread extension not valid");

    assert config['app_conf']['permanent_store'];
    tmpfile = NamedTemporaryFile(delete=False, dir=config['app_conf']['permanent_store']);
    shutil.copyfileobj(filepointer.file, tmpfile);
    tmpfile.close();

    nodesToRemove = [];
    nodesToRemove.append(tmpfile.name);

    h.logm("DBG",5,"open "+ tmpfile.name);
    zfile = zipfile.ZipFile(tmpfile.name, "r");

    ret = False;

    breadDict = {};

    message = "";

    while 1:

        # Validate it's a ZIP file type
        if not zipfile.is_zipfile(tmpfile.name):
            message = "not BREAD valid format (zip)";
            break;

        # Contains Payload on root folder
        if 'Payload/' not in zfile.namelist():
            message = "Payload does not exist";
            break;


        # Validate that contains a unique folder called XXX.app
        dirApp = None;
        for name in zfile.namelist():
            if '.app' in name:
                dirApp = name;
                break;
        
        if dirApp is None:
            message = "dirApp not exist";
            break;

        m = re.search('Payload/(.+?)(\.[^.]*$|$)',  dirApp)
        if m.group(0) is None:
            message = "bad regex 0";
            break;
        if m.group(1) is None:
            message = "bad regex 1";
            break;
        if m.group(2) is None:
            message = "bad regex 2";
            break;
        if m.group(2) != '.app/':
            message = "bad regex 3, not a valid group [" + m.group(2) + "]";
            break;

        # Validate that contains Info.plist file
        infoplist = dirApp + 'Info.plist'
        if infoplist not in zfile.namelist():
            message = "Info.plist = [" + infoplist + " not in zipfile";
            break;

        # extract info plist in temporary path inside pylons
        path = tmpfile.name + '.d';
        os.mkdir(path);
        if not os.path.exists(path):
            message = "cannot create internal path: " + path;
            break;
        nodesToRemove.append(path);

        zfile.extract(infoplist, path=path);

        infoplist = os.path.join(path, infoplist);
        if not os.path.exists(infoplist):
            message = "Info.plist = [" + infoplist + " not in path: " + path;
            break;


        # Validate Info.plist is in XML format

        try:
            plist = biplist.readPlist(infoplist)
        except (biplist.InvalidPlistException, biplist.NotBinaryPlistException), e:
            message = "not a plist = [" + e;
            break;

        # Extract the following data from Info.plist:

        # commodity ID
        commodityCode = plist['CFCommodityCode'];
        if not commodityCode:
            message = "commodityCode does not exist in plist";
            break;

        # version
        commodityVersion = plist['CFCommodityVersion'];
        if not commodityVersion:
            h.logm("WRN",5,"CFCommodityVersion does not exist in plist");
            # if not version, try with short
            commodityVersion = plist['CFCommodityShortVersionString'];
            if not commodityVersion:
                message = "CFCommodityShortVersionString does not exist in plist";
                break;

        if 'CFCommodityShortVersionString' in plist:
            commodityShortVersion = plist['CFCommodityShortVersionString'];
        else:
            commodityShortVersion = '';

        # applications display name
        commodityDisplayName = plist['CFCommodityDisplayName'];
        if not commodityDisplayName:
            message = "CFCommodityDisplayName does not exist in plist";
            break;


        # check if exist iTunesArtwork file, if not look for an icon into plist
        commodityIcon = None;
        artwork = dirApp + 'iTunesArtwork'
        if artwork in zfile.namelist():
            h.logm("WRN",5,"iTunesArtwork = " + artwork + " in zipfile");
            zfile.extract(artwork, path=path);
            commodityIcon = os.path.join(path, dirApp, 'iTunesArtwork');
            if not os.path.exists(commodityIcon):
                commodityIcon = None;

        if not commodityIcon:
            commodityIcon = getIconFile(dirApp, zfile, path, plist);

        if not commodityIcon:
            message = "Not icon in BREAD";
            break;


        extension = imghdr.what(commodityIcon);
        if extension:
            shutil.move(commodityIcon, commodityIcon + "." + extension);
            commodityIcon = commodityIcon + "." + extension;
            if not os.path.exists(commodityIcon):
                message = "rename fail on icon " + commodityIcon + "for bread";
                commodityIcon = None;

        if not commodityIcon:
            break;

        # Validate that contains embedded.mobileprovision file
        mobileprovisionfile = None;
        mobileprovision = dirApp + 'embedded.mobileprovision'
        if mobileprovision in zfile.namelist():
            h.logm("WRN",5,"mobileprovisionfile = [" + mobileprovision + " in BREAD");
            zfile.extract(mobileprovision, path=path);
            mobileprovisionfile = os.path.join(path, mobileprovision);
            if not os.path.exists(mobileprovisionfile):
                mobileprovisionfile = None;

        if not mobileprovisionfile:
            message = "embedded.mobileprovision does not exist in bread";
            break;

        # Validate embedded.mobileprovision is a SGML file
        # Validate that contains plist inside in XML format
#something change with iOS6, this is not valid
#if not sgml.wellFormed(mobileprovisionfile):
#            message = "embedded.mobileprovision is not a SGML well formed";
#            break;

        # Extract the following data from the plist:	
        # Extract ProvisionedApples (it's an array) from the plist part ->	apple's list
        provisionedApples = sgml.filter(mobileprovisionfile);

        breadDict['commodityCode'] = commodityCode;
        breadDict['commodityVersion'] = commodityVersion;
        breadDict['commodityShortVersion'] = commodityShortVersion;
        breadDict['name'] = commodityDisplayName;
        breadDict['iconFile'] = commodityIcon;
        breadDict['iconContentType'] = mimetypes.guess_type(commodityIcon)[0];
        breadDict['apples'] = provisionedApples;
        h.logm("INF",5,"APPLES = [" + str(provisionedApples) + "]");

        h.logm("INF",5,"return breadDict = [" + str(breadDict) + "]");

        message = "Valid BREAD";

        ret = True;
        break;
        

    h.logm("DBG",5,"MESSAGE "+ message);

    return (ret, breadDict, message, nodesToRemove);


def removeNodes(nodesToRemove):
    # remove tmpfile
    for nodeToRemove in nodesToRemove:
        if os.path.exists(nodeToRemove):
            if os.path.isdir(nodeToRemove):
                shutil.rmtree(nodeToRemove);
            else:
                os.unlink(nodeToRemove);


def getIconFile(dirApp, zfile, path, plist):
    h.logm("DBG",5,"ACTION "+ h.getcalled());
    iconFile = getIconFileIOS4(dirApp, zfile, path, plist);
    if iconFile:
        return iconFile;

    iconFile = getIconFileIOS3(dirApp, zfile, path, plist);
    if iconFile:
        return iconFile;

    iconFile = getIconFileIOS5(dirApp, zfile, path, plist);
    if iconFile:
        return iconFile;

    h.logm("ERR",5,"no IconFile in bread");
    return None;




# icon (choose one) ->  (it's an array of icons, iOS 4.x and so on)
def getIconFileIOS4(dirApp, zfile, path, plist):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    if not 'CFCommodityIconFiles' in plist:
        h.logm("ERR",5,"no CFCommodityIconFiles in plist");
        return None;

    commodityIconFiles = plist['CFCommodityIconFiles'];

    for commodityIconName in commodityIconFiles:
        iconapp = dirApp + commodityIconName
        if iconapp in zfile.namelist():
            zfile.extract(iconapp, path=path);

            commodityIconFile = os.path.join(path, dirApp, commodityIconName);
            if os.path.exists(commodityIconFile):
                break;
            else:
                h.logm("ERR",5,"commodityIconFile "+ commodityIconFile + " does not exist in BREAD");
                commodityIconFile = None;

    if not commodityIconFile:
        h.logm("ERR",5,"commodityIconFile does not exist in BREAD");
    else:
        h.logm("INF",5,"commodityIconFile in BREAD!, using " + commodityIconFile);

    return commodityIconFile;


# icon ios3 (legacy iOS 3.x)
def getIconFileIOS3(dirApp, zfile, path, plist):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    if not 'CFCommodityIconFile' in plist:
        return None;

    commodityIconName = plist['CFCommodityIconFile'];

    iconapp = dirApp + commodityIconName
    if iconapp in zfile.namelist():
        zfile.extract(iconapp, path=path);

        commodityIconFile = os.path.join(path, dirApp, commodityIconName);
        if os.path.exists(commodityIconFile):
            return commodityIconFile;
        
    h.logm("ERR",5,"commodityIconFile (IOS3)"+ commodityIconFile + " does not exist in BREAD");

    return None;



# CFCommodityIcons (it's a dictionary iOS 5.x & 6.x)
def getIconFileIOS5(dirApp, zfile, path, plist):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    if not 'CFCommodityIcons' in plist:
        h.logm("ERR",5,"no CFCommodityIcons in plist");
        return None;

    commodityIconFileDict = plist['CFCommodityIcons'];

    if 'CFCommodityPrimaryIcon' in commodityIconFileDict:
        commodityIconName = commodityIconFileDict['CFCommodityPrimaryIcon']

        iconapp = dirApp + commodityIconName
        if iconapp in zfile.namelist():
            zfile.extract(iconapp, path=path);

            commodityIconFile = os.path.join(path, dirApp, commodityIconName);

            if os.path.exists(commodityIconFile):
                return commodityIconFile;
            else:
                commodityIconFile = None;
    else:
        h.logm("ERR",5,"no CFCommodityPrimaryIcon in plist");

    if 'UINewsstandIcon' in commodityIconFileDict:
        commodityIconName = commodityIconFileDict['UINewsstandIcon']

        iconapp = dirApp + commodityIconName
        if iconapp in zfile.namelist():
            zfile.extract(iconapp, path=path);

            commodityIconFile = os.path.join(path, commodityIconName);
            if os.path.exists(commodityIconFile):
                return commodityIconFile;
            else:
                commodityIconFile = None;
    else:
        h.logm("ERR",5,"no UINewsstandIcon in plist");

    if not commodityIconFile:
        h.logm("ERR",5,"commodityIconFile (IOS5)"+ commodityIconFile + " does not exist in BREAD");

    return None;

