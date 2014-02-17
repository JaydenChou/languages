import sys
import os
import re
import shutil;
import mimetypes;
from paste.fileapp import FileApp;

from pylons import config;
import cocoamilk.lib.helpers as h;


def createfile(filepointer, appid, breadid = None):
    h.logm("DBG",5,"ACTION "+ h.getcalled());
    
    filepath = buildfilepath(appid, breadid);
    h.logm("DBG",5,"filepath "+ filepath);

    permanent_file = open(filepath, 'wb');
    filepointer.file.seek(0);
    shutil.copyfileobj(filepointer.file, permanent_file);
    filepointer.file.close();
    permanent_file.close();

    return (filepath, os.path.getsize(filepath));


def createfilename(filename, appid, breadid = None):
    h.logm("DBG",5,"ACTION "+ h.getcalled());
    
    filepath = buildfilepath(appid, breadid);
    size = os.path.getsize(filename)

    permanent_file = open(filepath, 'wb');
    fileptr = open(filename, 'rb');
    if not fileptr:
        h.logm("DBG",5, 'cannot read file');
        return None;

    shutil.copyfileobj(fileptr, permanent_file);
    fileptr.close()
    permanent_file.close()
    return (filepath, size);



def createdir(groupid):
    h.logm("DBG",5,"ACTION "+ h.getcalled());
    assert config['app_conf']['permanent_store'];
    dirpath = os.path.join(config['app_conf']['permanent_store'], groupid);
    if not os.path.exists(filepath):
        os.mkdir(dirpath);
    return dirpath;
    

def existdir(groupid):
    h.logm("DBG",5,"ACTION "+ h.getcalled());
    assert config['app_conf']['permanent_store'];
    dirpath = os.path.join(config['app_conf']['permanent_store'], groupid);
    return os.path.exists(dirpath);
    

def buildfilename(appid, breadid=None):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    if breadid:
        filename = appid + "_" + breadid;
    else:
        filename = appid;

    return filename;



def buildfilepath(appid, breadid=None):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    assert config['app_conf']['permanent_store'];
    filepath = os.path.join(config['app_conf']['permanent_store'], buildfilename(appid, breadid));

    return filepath;


def getfilepath(appid, breadid=None):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    filepath = buildfilepath(appid, breadid);

    if not os.path.exists(filepath):
        h.logm("DBG",5, 'file ' + filepath + ' does not exist');
        return None;

    return filepath;


def deletefile(appid, breadid=None):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    filename = getfilepath(appid, breadid);

    os.unlink(filename);



def getfile(appid, breadid=None):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    filename = getfilepath(appid, breadid);

    content_type = mimetypes.guess_type(filename);

    fileptr = open(filename, 'rb');
    if not fileptr:
        h.logm("DBG",5, 'cannot read file');
        return None;

    data = fileptr.read()
    h.logm("INF",5, 'filename %s data len %d' % (filename, len(data)));

    fileptr.close()

    return data;


def getfile_(appid, breadid=None):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    filename = getfilepath(appid, breadid);

    h.logm("INF",5, 'filename %s' % filename);
    headers = [('Content-Disposition', 'attachment; filename=\"' + filename + '\"'),
        ('Content-Type', 'application/octet-stream'),
        ('Content-Length', str(os.path.getsize(filename)))];

    h.logm("INF",5, 'headers ' + str(headers));
#    return FileApp(filename, headers=headers);
    return FileApp(filename, headers=headers)



def getfileUsingResponse_(appid, breadid, response):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    filename = getfilepath(appid, breadid);

    with open(filename, 'rb') as fileptr:
        shutil.copyfileobj(fileptr, response)

    h.logm("INF",5, 'filename %s' % filename);
    fileptr.close()



def getfileUsingResponse(appid, breadid, response):
    h.logm("DBG",5,"ACTION "+ h.getcalled());

    filename = getfilepath(appid, breadid);

    with open(filename, 'rb') as fileptr:
        shutil.copyfileobj(fileptr, response)

    h.logm("INF",5, 'filename %s' % filename);
    fileptr.close()



#return (data, content_type);


