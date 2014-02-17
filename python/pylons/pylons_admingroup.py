import logging;
import pymongo;

from pylons import request, response, session, tmpl_context as c, url;
from pylons.controllers.util import abort, redirect;

from bson.objectid import ObjectId;

from cocoamilk.model.group import Group;
from cocoamilk.model.user import User;
from cocoamilk.model.application import Application;

from cocoamilk.lib.base import BaseController, render;
import cocoamilk.lib.helpers as h;
import cocoamilk.lib.servicefilehandler as sfh;

import cocoamilk.lib.validator as Validator;

from formencode import htmlfill;
import formencode;

log = logging.getLogger(__name__)


# create a group
# list all the groups w/users assigned, w/applications assigned
# create a group
# assign/remove a user to/from a group
# assign/remove an application to/from a group
class AdmingroupController(BaseController):
    def index(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        h.logm("INF",5,"params "+ str(request.params));

        self.buildInformation();

        return render('/admingroup.mako');


    # request.params[]
    # add_user_group_id 
    # add_user_user_name
    def addUserToGroup(self):

        h.logm("DBG",5,"ACTION "+ h.getcalled());
        
        schema = Validator.FormAddUserValidator();
        try:
            form_result = schema.to_python(request.params);
            h.logm("DBG",5,"f r "+ str(request.params));
            h.logm("DBG",5,"TRY");

        except formencode.Invalid, error:
            h.logm("DBG",5,"EXCEPT");
            h.logm("ERR",5,"f r "+ str(request.params));
            c.message = h.literal(htmlfill.escapenl_formatter(error));
            session['message'] = c.message;
            session.save();


        else:

            h.logm("DBG",5,"OK");
            groupid = request.params['add_user_group_id'];

            db = self._py_object.app_globals.db;

            if 'add_user_user_name' in request.params and request.params['add_user_user_name'] != 'None':
                user = db.users.find_one({'name': request.params['add_user_user_name']});
                h.logm("INF",5,"found user: " + str(user));
                group = db.groups.find_one({'_id': ObjectId(groupid)});
                
                addUser = True;
                for userid in group['users']:
                    if userid == user['_id']:
                        session['message'] = 'Fail: user [' + user['name'] + '] already exist in group [' + group['name'] + '].';
                        session.save();
                        addUser = False;
                        break;

                if addUser:
                    db.groups.update({"_id" : ObjectId(groupid)}, {"$push" : {"users" : (user['_id'])}});
                    session['message_admingroup'] = 'Congratulations: ADDED user [' +  user['name'] + '] TO group [' + group['name'] + ']!';
                    session.save();

        self.buildInformation();
        return render('/admingroup.mako');



    # request.params
    # addApplication_group_id
    # addApplication_application_name
    def addApplication(self):

        h.logm("DBG",5,"ACTION "+ h.getcalled());
        
        db = self._py_object.app_globals.db;

        schema = Validator.FormAddApplicationValidator();
        try:
            form_result = schema.to_python(request.params);
            h.logm("DBG",5,"f r "+ str(request.params));
            h.logm("DBG",5,"TRY");

        except formencode.Invalid, error:
            h.logm("DBG",5,"EXCEPT");
            h.logm("ERR",5,"f r "+ str(request.params));
            c.message = h.literal(htmlfill.escapenl_formatter(error));
            session['message'] = c.message;
            session.save();

        else:

            h.logm("DBG",5,"OK");
            groupid = request.params['addApplication_group'];

            if 'addApplication_application_name' in request.params and request.params['addApplication_application_name'] != 'None':
                application = db.applications.find_one({'name': request.params['addApplication_application_name']});
                h.logm("INF",5,"found application: " + str(application));

                group = db.groups.find_one({'_id': ObjectId(groupid)});

                addApplication = True;
                for applicationid in group['applications']:
                    if applicationid == application['_id']:
                        session['message'] = 'Fail: application ' + application['name'] + ' already exist in group ' + group['name'] + '.';
                        session.save();
                        addApplication = False;
                        break;

                if addApplication:
                    session['message_admingroup'] = 'Congratulations: ADDED application [' +  application['name'] + '] TO group [' + group['name'] + ']!';
                    session.save();
                    db.groups.update({"_id" : ObjectId(groupid)}, {"$push" : {"applications" : (application['_id'])}});

        self.buildInformation();
        return render('/admingroup.mako');




    def submit(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        h.logm("DBG",5,"params "+ str(request.params));

        self.buildInformation();
        return render('/admingroup.mako');


    # receive request.params[]:
    #   remove_group_id
    def removeGroup(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        
        params = request.params;
        h.logm("DBG",5,"params "+ str(params));

        schema = Validator.FormRemoveGroupValidator();
        try:
            form_result = schema.to_python(request.params);

        except formencode.Invalid, error:
            c.message = h.literal(htmlfill.escapenl_formatter(error));
            session['message'] = c.message;
            session.save();

        else:
            h.logm("DBG",5,"OK");
            groupid = request.params['remove_group_id'];

            db = self._py_object.app_globals.db;
            group = db.groups.find_one({'_id': ObjectId(groupid)});
            if group:
                db.groups.remove({"_id" : ObjectId(groupid)});
            else:
                session['message'] = 'Cannot remove group from DB.';
                session.save();
                self.buildInformation();
                return render('/admingroup.mako');
            
            session['message_admingroup'] = 'Congratulations: REMOVED group [' +  group['name'] + ']!';
            session.save();

        self.buildInformation();
        return render('/admingroup.mako');


    # request.params[]:
    # remove_user_from_group_group_id
    # TODO: this is the only way I found to retrieve the associated user id to a button '-'
    # value = '-' ->  userid

    def removeUserFromGroup(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        h.logm("DBG",5,"params "+ str(request.params));
        
        schema = Validator.FormRemoveUserFromGroupValidator();
        try:
            form_result = schema.to_python(request.params);
            h.logm("DBG",5,"f r "+ str(request.params));
            h.logm("DBG",5,"TRY");

        except formencode.Invalid, error:
            h.logm("DBG",5,"EXCEPT");
            h.logm("ERR",5,"f r "+ str(request.params));
            c.message = h.literal(htmlfill.escapenl_formatter(error));
            session['message'] = c.message;
            session.save();

        else:

            h.logm("DBG",5,"OK");
            userid = 0;
            for (k, v) in request.params.iteritems():
                if v == '-':
                    userid = k;
                    break;
                
            groupid = request.params['remove_user_from_group_group_id'];
            h.logm("DBG",5,"group id" + groupid);
            
            h.logm("DBG",5,"user id" + userid);

            db = self._py_object.app_globals.db;
            db.groups.update({"_id" : ObjectId(groupid)}, {"$pull" : {"users" : (ObjectId(userid))}});

            group = db.groups.find_one({'_id': ObjectId(groupid)});
            user = db.users.find_one({'_id': ObjectId(userid)});
            session['message_admingroup'] = 'Congratulations: REMOVED user [' +  user['name'] + '] FROM group [' + group['name'] + ']!';
            session.save();


        self.buildInformation();
        return render('/admingroup.mako');




    # request.params[]:
    # removeApplication_application_id
    # TODO: this is the only way I found to retrieve the associated application id to a button '-'
    # value = '-' ->  applicationid

    def removeApplication(self):

        h.logm("DBG",5,"ACTION "+ h.getcalled());
        h.logm("DBG",5,"params "+ str(request.params));
        
        db = self._py_object.app_globals.db;

        schema = Validator.FormRemoveApplicationValidator();
        try:
            form_result = schema.to_python(request.params);
            h.logm("DBG",5,"f r "+ str(request.params));
            h.logm("DBG",5,"TRY");

        except formencode.Invalid, error:
            h.logm("DBG",5,"EXCEPT");
            h.logm("ERR",5,"f r "+ str(request.params));
            c.message = h.literal(htmlfill.escapenl_formatter(error));
            session['message'] = c.message;
            session.save();

        else:

            h.logm("DBG",5,"OK");
            applicationid = 0;
            for (k, v) in request.params.iteritems():
                if v == '-':
                    applicationid = k;
                    break;



            groupid = request.params['removeApplication_group'];
            group = db.groups.find_one({'_id': ObjectId(groupid)});

            application = db.applications.find_one({'_id': ObjectId(applicationid)});

            # removing application for group
            db.groups.update({"_id" : group['_id']}, {"$pull" : {"applications" : application['_id']}});

            session['message_admingroup'] = 'Congratulations: REMOVED application [' +  application['name'] + '] FROM group [' + group['name'] + ']!';
            session.save();

        self.buildInformation();
        return render('/admingroup.mako');




    # receive request.params[]:
    #   create_group_name
    #   create_group_user_name
    #   create_group_application_name

    def createGroup(self):
        db = self._py_object.app_globals.db;

        schema = Validator.CreateGroupValidator();
        try:
            form_result = schema.to_python(request.params);
            h.logm("DBG",5,"f r "+ str(request.params));

        except formencode.Invalid, error:
            h.logm("ERR",5,"f r "+ str(request.params));
            c.message = h.literal(htmlfill.escapenl_formatter(error));
            session['message'] = c.message;
            session.save();

        else:

            params = request.params;
            groupDict = {};
            groupDict['name'] = params['create_group_name'];
            # need to create the object group first
            group = Group(groupDict);

            if 'create_group_user_name' in params and params['create_group_user_name'] != 'None':
                user = db.users.find_one({'name': params['create_group_user_name']});
                group.users.append(user['_id']);

            if 'create_group_application_name' in params and params['create_group_application_name'] != 'None':
                application = db.applications.find_one({'name': params['create_group_application_name']});
                group.applications.append(application['_id']);

            if db.groups.find_one({'name': group.name}):
                h.logm("INF",5,"group already exist: " + str(group.encode()));
                c.message = 'group ' + group.name + ' already exist!';
                session['message'] = c.message;
                session.save();
                self.buildInformation();
                return render('/admingroup.mako');
            else:
                h.logm("INF",5,"create group: " + str(group.encode()));
                # now we save in DB
                # using groupid (ObjectID from DB) to create the bucket
                groupid = db.groups.save(group.encode()); 

            if groupid == 0:
                c.message = 'Cannot create group: ' + str(group);
                session['message'] = c.message;
                session.save();
                self.buildInformation();
                return render('/admingroup.mako');

            # if bucket not exist
            if not sfh.existbucket(groupid):
                h.logm("INF",5,"creating bucket for group: " + str(group.encode()));

                if not sfh.createbucket(groupid):
                    h.logm("INF",5,"cannot create bucket for group: " + str(group.encode()));
                    c.message = 'cannot create bucket for group: ' + str(group.encode());
                    session['message'] = c.message;
                    session.save();
                    self.buildInformation();
                    return render('/admingroup.mako');
            else:
                h.logm("INF",5,"bucket already exist for group: " + str(group.encode()));

            h.logm("INF",5,"complete Group: " + str(group.encode()));

            session['message_admingroup'] = 'Congratulations: CREATED group [' + group.name + ']!';
            session.save();

        self.buildInformation();
        return render('/admingroup.mako');


    def buildInformation(self):
        self.savesession();

        db = self._py_object.app_globals.db;
        c.allusers = db.users.find();

        c.groups = [];
        c.users = [None];
        for user in db.users.find({},{'name':1}):
            c.users.append(user['name']);
        c.applications = [None];
        for application in db.applications.find({},{'name':1}):
            c.applications.append(application['name']);

        for group in db.groups.find():
            # adding more information of the user using userid into group 
            if 'users' in group:
                group['usersMoreInfo'] = [];
                for userid in group['users']:
                    user = db.users.find_one({'_id': ObjectId(userid)},{'name':1});
                    if user:
                        group['usersMoreInfo'].append(user);

            # Creating a list of all users that are not in the actual group
            group['users2Add'] = [];
            for user in db.users.find({},{'name':1}):
                add = True;
                if 'usersMoreInfo' in group:
                    for groupUser in group['usersMoreInfo']:
                        if user['_id'] == groupUser['_id']:
                            add = False;
                            break;
                if add:
                    if not group['users2Add']:
                        group['users2Add'].append(None);
                    group['users2Add'].append(user['name']);

                
            if 'applications' in group:
                group['applicationsMoreInfo'] = [];
                for applicationid in group['applications']:
                    application = db.applications.find_one({'_id': ObjectId(applicationid)}, {'name':1});
                    if application:
                        group['applicationsMoreInfo'].append(application);


            group['applications2Add'] = [];
            for application in db.applications.find({},{'name':1}):
                add = True;
                if 'applicationsMoreInfo' in group:
                    for groupApplication in group['applicationsMoreInfo']:
                        if application['_id'] == groupApplication['_id']:
                            add = False;
                            break;
                if add:
                    if not group['applications2Add']:
                        group['applications2Add'].append(None);
                    group['applications2Add'].append(application['name']);
        
            c.groups.append(group);

            if not h.isAssigned(db, c.email):
                c.message = 'Warning: You are not assigned in a group';


    def savesession(self):

        if 'message_admingroup' in session and session['message_admingroup'] != '':
            c.message_admingroup = session['message_admingroup'];
            session['message_admingroup'] = '';
            session.save();
        else:
            c.message_admingroup = '';

        if 'message' in session and session['message'] != '':
            c.message = session['message'];
            session['message'] = '';
            session.save();
        else:
            c.message = '';

        if 'email' in session and session['email'] != '':
            c.email = session['email'];
            if c.email:
                c.logged = True;
        else:
            c.email = '';
            c.logged = False;
            c.message = 'Invalid email address. Please login.';
            c.groups = '';
            return render('/admingroup.mako');



    # request.params
    # TODO: this is the only way I found to retrieve the associated user id to a button '-'
    # value = '-' ->  userid

    def removeUser(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        h.logm("DBG",5,"params "+ str(request.params));
        
        db = self._py_object.app_globals.db;

        userid = 0;
        for (k, v) in request.params.iteritems():
            if v == '-':
                userid = k;
                break;
                
        db.groups.update({}, {"$pull" : {"users" : (ObjectId(userid))}});

        user = db.users.find_one({'_id': ObjectId(userid)});

        db.users.remove({'_id': ObjectId(userid)});

        session['message_admingroup'] = 'Congratulations: REMOVED user [' +  user['name'] + ']!';

        session.save();

        self.buildInformation();
        return render('/admingroup.mako');



    # request.params
    # TODO: this is the only way I found to retrieve the associated apple id to a button '-'
    # removeApple_userId
    # value = '-' ->  appleid

    def removeApple(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        h.logm("DBG",5,"params "+ str(request.params));
        
        db = self._py_object.app_globals.db;

        schema = Validator.FormRemoveUserAppleValidator();
        try:
            form_result = schema.to_python(request.params);
            h.logm("DBG",5,"f r "+ str(request.params));
            h.logm("DBG",5,"TRY");

        except formencode.Invalid, error:
            h.logm("DBG",5,"EXCEPT");
            h.logm("ERR",5,"f r "+ str(request.params));
            c.message = h.literal(htmlfill.escapenl_formatter(error));
            session['message'] = c.message;
            session.save();

        else:

            apple = 0;
            for (k, v) in request.params.iteritems():
                if v == '-':
                    apple = k;
                    break;
                
            h.logm("DBG",5,"OK");
            userid = request.params['removeApple_userId'];

            user = db.users.find_one({'_id': ObjectId(userid)});
            appleDataToRemove = None;
            for appleData in user['apples']:
                if appleData == apple:
                    appleDataToRemove = appleData;

            if not appleDataToRemove:
                c.message = 'cannot remove apple [' + apple + '] for user [' + user['name'] + '].';
                session['message'] = c.message;
                session.save();
                self.buildInformation();
                return render('/admingroup.mako');

            db.users.update({"_id" : user['_id']}, {"$pull" : {"apples" : appleDataToRemove}});

            session['message_admingroup'] = 'Congratulations: REMOVED apple [' + apple + '] from user [' +  user['name'] + ']!';

            session.save();

        self.buildInformation();
        return render('/admingroup.mako');


