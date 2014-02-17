"""
    Root REST Services URL: adm
    controller: adm
    actions:
        Groups
            add group (POST)
            remove group (DELETE)
            update group (PUT)
            info about groups (GET)
        Groups/Users
            add user to a group (POST)
            remove user from a group (DELETE)
            Groups/Applications
            add application to a group (POST)
            remove application from a group (DELETE)
        Users
            add user (POST)
            remove user (DELETE)
            update user (PUT)
            info about users (GET)

        Applications
            remove application (DELETE)
            info about one application (GET)
            info about applications (GET)


"""
import logging
import sys

from pylons import request, response, session, tmpl_context as c, url
from pylons.controllers.util import abort, redirect

from bson.objectid import ObjectId;
from bson import json_util
import json    


from cocoamilk.model.group import Group;
from cocoamilk.model.user import User;
from cocoamilk.model.application import Application;

import cocoamilk.lib.helpers as h;
import cocoamilk.lib.servicefilehandler as sfh;

import cocoamilk.lib.validator as Validator;
from cocoamilk.lib.base import BaseController, render

log = logging.getLogger(__name__)
            

class AdmController(BaseController):


    """
    * JSON error format

    Attribute       Description                                 Mandatory   Type 
    error_code	    Numeric code identifying the error type	    YES         Integer
    error_message	Simple description of the error	            YES         String


    * Common error codes
        Code    Description
        403	    Forbidden: the user's caller has no access to do the request
        404	    Not found: the BE cannot found a instance of an object
        405	    Already Exists: the caller tries to insert an instance that already exists
        407	    Invalid Arguments: The caller does not send mandatory parameters or a parameter has wrong values
        500	    Other Error: Used to wrap up unhandled errors on the BE
    """

    def jsonErrorResponse(self, code, message):
        responseDict = { "error_code" : code,
                         "error_message" :  message };

        return self.jsonResponse(responseDict);

    def jsonResponse(self, responseObject):
        return h.jsonResponse(responseObject);



    """
    actions:
        Groups
            add group (POST)
            remove group (DELETE)
            update group (PUT)
            info about groups (GET)
    """

    def group(self, id=None, action2=None, id2=None):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method == 'POST':
            return self.groupPost();
        elif request.method == 'PUT':
            return self.groupPut(id);
        elif request.method == 'DELETE':
            return self.groupDelete(id);
        else:
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/group action does only allow POST/PUT/DELETE request methods");


    """
    Method: POST
    Action: Adds a new group
    URL: /adm/group
        Body Parameters: A JSON containing the following attributes:
        name. Mandatory. Group's name.
        users. Optional. An array of OIDs of users
        applications. Optional. An array of OIDs of applications
    Return:
        If ok returns a JSON object containing the following attributes:
        oid. Mandatory. Group's OID.


    """        


    def groupPost(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'POST':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/group action does only allow POST request method");

        h.logm("DBG",5,"request.POST body: " + request.body);

        groupSchema = json.loads(request.body);
        group = Group(groupSchema);

        db = self._py_object.app_globals.db;
        if db.groups.find_one({'name': group.name}):
            h.logm("INF",5,"group already exist: " + str(group.encode()));
            return self.jsonErrorResponse(405, "group already exist: " + str(group.encode()));


        h.logm("INF",5,"trying to create group: " + str(group.encode()));

        if 'users' in groupSchema:
            h.logm("INF",5,"Looking for users: " + str(groupSchema['users']));
            for userid in groupSchema['users']:
                try:
                    user = db.users.find_one({'_id': ObjectId(userid)});
                except:
                    return self.jsonErrorResponse(404, "Invalid Mongo DB User Id {'_id' : " + userid + '}');
                else:
                    if not user:
                        return self.jsonErrorResponse(404, "Invalid user id " + userid);

                    h.logm("INF",5,"found user: " + str(user));
        if 'applications' in groupSchema:
            h.logm("INF",5,"Looking for applications: " + str(groupSchema['applications']));
            for applicationid in groupSchema['applications']:
                try:
                    application = db.applications.find_one({'_id': ObjectId(applicationid)});
                except:
                    return self.jsonErrorResponse(404, "Invalid Mongo DB Application Id {'_id' : " + applicationid + '}');
                else:
                    if not application:
                        return self.jsonErrorResponse(404, "Invalid application id " + applicationid);

                h.logm("INF",5,"found application: " + str(application));

        groupid = db.groups.save(group.encode()); 

        if groupid == 0:
            return self.jsonErrorResponse(500, "Cannot create group: " + str(group.encode()));

        # if bucket not exist
        if not sfh.existbucket(groupid):
            h.logm("INF",5,"creating bucket for group: " + str(group.encode()));

            if not sfh.createbucket(groupid):
                return self.jsonErrorResponse(500, "Cannot create bucket" + str(group.encode()));

        else:
            h.logm("INF",5,"bucket already exist for group: " + str(group.encode()));

        responseDict = { "oid" : str(groupid) };

        return self.jsonResponse(responseDict);


    def groupUser(self, id, id2):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method == 'POST':
            return self.groupAddUser(id,id2);
        elif request.method == 'DELETE':
            return self.groupDeleteUser(id,id2);
        else:
            return self.jsonErrorResponse(500, "adm/groupUser/{id}/{id2} action only allows POST/DELETE request method. Request: " + request.method);

    """
    Method: POST
    Action: Adds a user to a group
    URL: /adm/groupUser/{groupOid}/{userOid}
        URL Placeholders: The URL contains the following placeholders:
        groupOid. Mandatory. Group's OID.
        userOid. Mandatory. User's OID.
    Return:
        If ok returns a JSON object containing the following attributes:
        groupOid. Mandatory. Group's OID.
        userOid. Mandatory. User's OID.
        If not ok returns an error in JSON format.
    
    """        
    def groupAddUser(self, id, id2):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'POST':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/groupUser/{id}/{id2} action does only allow POST request method. Request: " + request.method);

        if not id:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/group/id/user/id2, missing parameter id for group.");
        
        groupid = str(id);

        if not id2:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/group/id/user/id2, missing parameter id2 for user.");
        
        userid = str(id2);

        db = self._py_object.app_globals.db;
        try:
            group = db.groups.find_one({'_id': ObjectId(groupid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB Group Id {'_id' : " + groupid + '}');
        else:
            if not group:
                return self.jsonErrorResponse(404, "Invalid group id " + groupid);

            h.logm("INF",5,"found group: " + str(group));


        if userid in str(group['users']):
            return self.jsonErrorResponse(405, "userid " + userid + " already exist in group: " + str(group));

        try:
            user = db.users.find_one({'_id': ObjectId(userid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB User Id {'_id' : " + userid + '}');
        else:
            if not user:
                return self.jsonErrorResponse(404, "Invalid user id " + userid);

            h.logm("INF",5,"found user: " + str(user));


        db.groups.update({"_id" : group['_id']}, {"$push" : {"users" : ObjectId(userid)}});

        responseObject = {};
        responseObject["groupOid"] = groupid;
        responseObject["userOid"] = userid;

        return self.jsonResponse(responseObject);


    """
    Method: DELETE
    Action: Removes a user from a group
    URL: /adm/groupUser/{groupOid}/{userOid}
        URL Placeholders: The URL contains the following placeholders:
        groupOid. Mandatory. Group's OID.
        userOid. Mandatory. User's OID.
    Return:
        If ok returns a JSON object containing the following attributes:
        groupOid. Mandatory. Group's OID.
        userOid. Mandatory. User's OID.
        If not ok returns an error in JSON format.
    
    """        
    def groupDeleteUser(self, id, id2):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'DELETE':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/groupUser/{id}/{id2} action does only allow DELETE request method. Request: " + request.method);

        if not id:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/group/id/user/id2, missing parameter id for group.");
        
        groupid = str(id);

        if not id2:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/group/id/user/id2, missing parameter id2 for user.");
        
        userid = str(id2);

        db = self._py_object.app_globals.db;
        try:
            group = db.groups.find_one({'_id': ObjectId(groupid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB Group Id {'_id' : " + groupid + '}');
        else:
            if not group:
                return self.jsonErrorResponse(404, "Invalid group id " + groupid);

            h.logm("INF",5,"found group: " + str(group));


        if userid not in str(group['users']):
            return self.jsonErrorResponse(405, "userid " + userid + " does not exist in group: " + str(group));

        try:
            user = db.users.find_one({'_id': ObjectId(userid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB User Id {'_id' : " + userid + '}');
        else:
            if not user:
                return self.jsonErrorResponse(404, "Invalid user id " + userid);

            h.logm("INF",5,"found user: " + str(user));


        db.groups.update({"_id" : group['_id']}, {"$pull" : {"users" : ObjectId(userid)}});

        responseObject = {};
        responseObject["groupOid"] = groupid;
        responseObject["userOid"] = userid;

        return self.jsonResponse(responseObject);


    def groupApplication(self, id, id2):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method == 'POST':
            return self.groupAddApplication(id,id2);
        elif request.method == 'DELETE':
            return self.groupDeleteApplication(id,id2);
        else:
            return self.jsonErrorResponse(500, "adm/groupApplication/{id}/{id2} action only allows POST/DELETE request method. Request: " + request.method);

    """
    Method: POST
    Action: Adds a application to a group
    URL: /adm/groupApplication/{groupOid}/{applicationOid}
        URL Placeholders: The URL contains the following placeholders:
        groupOid. Mandatory. Group's OID.
        applicationOid. Mandatory. Application's OID.
    Return:
        If ok returns a JSON object containing the following attributes:
        groupOid. Mandatory. Group's OID.
        applicationOid. Mandatory. Application's OID.
        If not ok returns an error in JSON format.
    
    """        
    def groupAddApplication(self, id, id2):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'POST':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/groupApplication/{id}/{id2} action does only allow POST request method. Request: " + request.method);

        if not id:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/group/id/application/id2, missing parameter id for group.");
        
        groupid = str(id);

        if not id2:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/group/id/application/id2, missing parameter id2 for application.");
        
        applicationid = str(id2);

        db = self._py_object.app_globals.db;
        try:
            group = db.groups.find_one({'_id': ObjectId(groupid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB Group Id {'_id' : " + groupid + '}');
        else:
            if not group:
                return self.jsonErrorResponse(404, "Invalid group id " + groupid);

            h.logm("INF",5,"found group: " + str(group));


        if applicationid in str(group['applications']):
            return self.jsonErrorResponse(405, "applicationid " + applicationid + " already exist in group: " + str(group));

        try:
            application = db.applications.find_one({'_id': ObjectId(applicationid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB Application Id {'_id' : " + applicationid + '}');
        else:
            if not application:
                return self.jsonErrorResponse(404, "Invalid application id " + applicationid);

            h.logm("INF",5,"found application: " + str(application));


        db.groups.update({"_id" : group['_id']}, {"$push" : {"applications" : ObjectId(applicationid)}});

        responseObject = {};
        responseObject["groupOid"] = groupid;
        responseObject["applicationOid"] = applicationid;

        return self.jsonResponse(responseObject);


    """
    Method: DELETE
    Action: Removes a application from a group
    URL: /adm/groupApplication/{groupOid}/{applicationOid}
        URL Placeholders: The URL contains the following placeholders:
        groupOid. Mandatory. Group's OID.
        applicationOid. Mandatory. Application's OID.
    Return:
        If ok returns a JSON object containing the following attributes:
        groupOid. Mandatory. Group's OID.
        applicationOid. Mandatory. Application's OID.
        If not ok returns an error in JSON format.
    
    """        
    def groupDeleteApplication(self, id, id2):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'DELETE':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/groupApplication/{id}/{id2} action does only allow DELETE request method. Request: " + request.method);

        if not id:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/group/id/application/id2, missing parameter id for group.");
        
        groupid = str(id);

        if not id2:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/group/id/application/id2, missing parameter id2 for application.");
        
        applicationid = str(id2);

        db = self._py_object.app_globals.db;
        try:
            group = db.groups.find_one({'_id': ObjectId(groupid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB Group Id {'_id' : " + groupid + '}');
        else:
            if not group:
                return self.jsonErrorResponse(404, "Invalid group id " + groupid);

            h.logm("INF",5,"found group: " + str(group));


        if applicationid not in str(group['applications']):
            return self.jsonErrorResponse(405, "applicationid " + applicationid + " does not exist in group: " + str(group));

        try:
            application = db.applications.find_one({'_id': ObjectId(applicationid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB Application Id {'_id' : " + applicationid + '}');
        else:
            if not application:
                return self.jsonErrorResponse(404, "Invalid application id " + applicationid);

            h.logm("INF",5,"found application: " + str(application));


        db.groups.update({"_id" : group['_id']}, {"$pull" : {"applications" : ObjectId(applicationid)}});

        responseObject = {};
        responseObject["groupOid"] = groupid;
        responseObject["applicationOid"] = applicationid;

        return self.jsonResponse(responseObject);


    """
    Method: PUT
    Action: Updates a group
    URL: /adm/group/{oid}
        URL Placeholders: The URL contains the following placeholders:
        oid. Mandatory. Group's OID.
        Body Parameters: A JSON containing the following attributes:
        name. Optional. Group's name.
        users. Optional. An array of OIDs of users
        applications. Optional. An array of OIDs of applications
    Return:
        If ok returns a JSON object containing the following attributes:
        oid. Mandatory. Group's OID.
        If not ok returns an error in JSON format.
        Attention: If the body specifies users array and/or applications array  these arrays overrides the existing ones.
    """

    def groupPut(self, id=None):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'PUT':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/group action does only allow PUT request method");
        if not id:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/group/id, missing parameter id");

        groupid = str(id);
        h.logm("DBG",5,"request.POST body: " + request.body + ", groupid=" + groupid);

        groupSchema = json.loads(request.body);

        db = self._py_object.app_globals.db;

        group = None;

        try:
            group = db.groups.find_one({'_id': ObjectId(groupid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB Group Id {'_id' : " + groupid + '}');
        else:
            if not group:
                return self.jsonErrorResponse(404, "Invalid group id " + groupid);

            h.logm("INF",5,"found group: " + str(group));

        h.logm("INF",5,"group schema: " + str(groupSchema));
        if 'name' in groupSchema:
            h.logm("INF",5,"group schema name: " + groupSchema['name']);
            groupWithName = db.groups.find_one({'name': groupSchema['name']});
            if groupWithName and str(groupWithName['_id']) != groupid:
                h.logm("INF",5,"group name is (unique) and already exist: " + str(group));
                return self.jsonErrorResponse(405, "group name is (unique) and already exist w/another object id: " + str(group));

        h.logm("INF",5,"trying to update group: " + str(group));

        if 'users' in groupSchema:
            h.logm("INF",5,"Looking for users: " + str(groupSchema['users']));


            uniqueList=[];
            for userid in groupSchema['users']:
                try:
                    user = db.users.find_one({'_id': ObjectId(userid)});
                except:
                    return self.jsonErrorResponse(404, "Invalid Mongo DB User Id {'_id' : " + userid + '}');
                else:
                    if not user:
                        return self.jsonErrorResponse(404, "Invalid user id " + userid);

                    if userid in uniqueList:
                        return self.jsonErrorResponse(500, "One of the users id is repetead: {'_id' : " + userid + '}');
                        
                    uniqueList.append(userid);

                    h.logm("INF",5,"found user: " + str(user));

                    
        if 'applications' in groupSchema:
            h.logm("INF",5,"Looking for applications: " + str(groupSchema['applications']));
            uniqueList=[];
            for applicationid in groupSchema['applications']:
                try:
                    application = db.applications.find_one({'_id': ObjectId(applicationid)});
                except:
                    return self.jsonErrorResponse(404, "Invalid Mongo DB Application Id {'_id' : " + applicationid + '}');
                else:
                    if not application:
                        return self.jsonErrorResponse(404, "Invalid application id " + applicationid);

                    if applicationid in uniqueList:
                        return self.jsonErrorResponse(500, "One of the applications id is repetead: {'_id' : " + applicationid + '}');
                        
                    uniqueList.append(applicationid);

                h.logm("INF",5,"found application: " + str(application));

        # Attention: If the body specifies users array and/or applications array  these arrays overrides the existing ones.

        if 'name' in groupSchema:
            db.groups.update({"_id" : group['_id']}, {"$set" : {"name" : groupSchema['name']}});


        if 'users' in groupSchema:
            for userid in group['users']:
                db.groups.update({"_id" : group['_id']}, {"$pull" : {"users" : ObjectId(userid)}});
            for userid in groupSchema['users']:
                db.groups.update({"_id" : group['_id']}, {"$push" : {"users" : (ObjectId(userid))}});

        if 'applications' in groupSchema:
            for applicationid in group['applications']:
                db.groups.update({"_id" : group['_id']}, {"$pull" : {"applications" : ObjectId(applicationid)}});
            for applicationid in groupSchema['applications']:
                db.groups.update({"_id" : group['_id']}, {"$push" : {"applications" : (ObjectId(applicationid))}});

        responseDict = { "oid" : str(group['_id']) };

        return self.jsonResponse(responseDict);


    """
    Method: DELETE
    Action: Removes the given group
    URL: /adm/group/{oid}
        URL Placeholders: The URL contains the following placeholders:
        oid. Mandatory. Group's OID.
    Return:
        If ok returns a JSON object containing the following attributes:
        oid. Mandatory. Group's removed OID.
        If not ok returns an error in JSON format.
    """

    def groupDelete(self, id=None):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'DELETE':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/group action does only allow DELETE request method");
        if not id:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/group/id, missing parameter id");

        groupid = str(id);
        h.logm("DBG",5,"request w/ groupid=" + groupid);

        db = self._py_object.app_globals.db;

        group = None;

        try:
            group = db.groups.find_one({'_id': ObjectId(groupid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB Group Id {'_id' : " + groupid + '}');
        else:
            if not group:
                return self.jsonErrorResponse(404, "Invalid group id " + groupid);

            h.logm("INF",5,"found group: " + str(group));

        db.groups.remove({'_id': ObjectId(groupid)});

        responseDict = { "oid" : str(group['_id']) };

        return self.jsonResponse(responseDict);


    """
    Method: GET
    Action: Return information about all groups
    URL: /adm/groups
    Return:
    If ok returns a JSON array containing JSON objects that represents groups. Each group object contains the following attributes:
    oid. Mandatory. Group's OID.
    name. Mandatory. Group's name.
    applications. Optional. An array of OIDs of applications.
    users. Optional. An array of OIDs of users.
    If not ok returns an error in JSON format.
    """        

    def groups(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'GET':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/groups does only allow GET request method");

        db = self._py_object.app_globals.db;
        groups = db.groups.find();
        responseArray = [];
        for group in groups:
            responseArray.append(Group(group).encode());

        return self.jsonResponse(responseArray);


    """
    Users
        add user (POST)
        remove user (DELETE)
        update user (PUT)
        info about users (GET)
    """

    def user(self, id=None):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method == 'POST':
            return self.userPost();
        elif request.method == 'PUT':
            return self.userPut(id);
        elif request.method == 'DELETE':
            return self.userDelete(id);
        else:
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/user action does only allow POST/PUT/DELETE request methods");


    """
    Method: POST
    Action: Adds a new user
    URL: /adm/user
        Body Parameters: A JSON containing the following attributes:
        name. Mandatory. User's name.
        email. Mandatory. User's email.
        apples. Optional. An array of JSON object that represents Apples. Each apple object contains the following attributes:
        aioi. Mandatory. Apple's AIOI.
        model. Mandatory. Apple's model (like: iPhone3,1).
        version. Mandatory. Apple's operating system version.
    Return:
        If ok returns a JSON object containing the following attributes:
        oid. Mandatory. User's OID.
        If not ok returns an error in JSON format.
    """        


    def userPost(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'POST':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/user action does only allow POST request method");

        h.logm("DBG",5,"request.POST body: " + request.body);

        userSchema = json.loads(request.body);
        user = User(userSchema);

        db = self._py_object.app_globals.db;
        if db.users.find_one({'name': user.name}):
            h.logm("INF",5,"user already exist: " + str(user.encode()));
            return self.jsonErrorResponse(405, "user already exist with this name: " + str(user.encode()));

        db = self._py_object.app_globals.db;
        if db.users.find_one({'email': user.email}):
            h.logm("INF",5,"user already exist: " + str(user.encode()));
            return self.jsonErrorResponse(405, "user already exist with this email: " + str(user.encode()));

        h.logm("INF",5,"trying to create user: " + str(user.encode()));

        userid = db.users.save(user.encode()); 

        if userid == 0:
            return self.jsonErrorResponse(500, "Cannot create user: " + str(user.encode()));

        responseDict = { "oid" : str(userid) };

        return self.jsonResponse(responseDict);


    """
    Method: DELETE
    Action: Removes the given user
    URL: /adm/user/{oid}
        URL Placeholders: The URL contains the following placeholders:
        oid. Mandatory. User's OID.
    Return:
        If ok returns a JSON object containing the following attributes:
        oid. Mandatory. User's removed OID.
        If not ok returns an error in JSON format.
    """

    def userDelete(self, id=None):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'DELETE':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/user action does only allow DELETE request method");
        if not id:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/user/id, missing parameter id");

        userid = str(id);
        h.logm("DBG",5,"request w/ userid=" + userid);

        db = self._py_object.app_globals.db;

        user = None;

        try:
            user = db.users.find_one({'_id': ObjectId(userid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB User Id {'_id' : " + userid + '}');
        else:
            if not user:
                return self.jsonErrorResponse(404, "Invalid user id " + userid);

            h.logm("INF",5,"found user: " + str(user));

        db.groups.update({}, {"$pull" : {"users" : (ObjectId(userid))}});
        db.users.remove({'_id': ObjectId(userid)});

        responseDict = { "oid" : str(user['_id']) };

        return self.jsonResponse(responseDict);


    """
    Method: PUT
    Action: Updates a user
    URL: /adm/user/{oid}
        URL Placeholders: The URL contains the following placeholders:
        oid. Mandatory. User's OID.
    Body Parameters: A JSON containing the following attributes:
        name. Mandatory. User's name.
        email. Mandatory. User's email.
        apples. Optional. An array of JSON object that represents Apples. Each apple object contains the following attributes:
        aioi. Mandatory. Apple's AIOI.
        model. Mandatory. Apple's model (like: iPhone3,1).
        version. Mandatory. Apple's operating system version.
    Return:
        If ok returns a JSON object containing the following attributes:
        oid. Mandatory. Group's OID.
        If not ok returns an error in JSON format. 

    Attention: If the body specifies apples array these arrays overrides the existing ones.
    """

    def userPut(self, id=None):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'PUT':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/user action does only allow PUT request method");
        if not id:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/user/id, missing parameter id");

        userid = str(id);
        h.logm("DBG",5,"request.POST body: " + str(request.body) + ", userid=" + userid);

        userSchema = json.loads(request.body);

        db = self._py_object.app_globals.db;

        user = None;

        try:
            user = db.users.find_one({'_id': ObjectId(userid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB User Id {'_id' : " + userid + '}');
        else:
            if not user:
                return self.jsonErrorResponse(404, "Invalid user id " + userid);

            h.logm("INF",5,"found user: " + str(user));

        if 'name' in userSchema:
            userWithName = db.users.find_one({'name': userSchema['name']});
            if userWithName and str(userWithName['_id']) != userid:
                h.logm("INF",5,"user name is (unique) and already exist: " + str(userWithName));
                return self.jsonErrorResponse(405, "user name is (unique) and already exist w/another object id: " + str(userWithName));

        if 'email' in userSchema:
            userWithEmail = db.users.find_one({'email': userSchema['email']});
            if userWithEmail and str(userWithEmail['_id']) != userid:
                h.logm("INF",5,"user email is (unique) and already exist: " + str(userWithEmail));
                return self.jsonErrorResponse(405, "user email is (unique) and already exist w/another object id: " + str(userWithEmail));

        h.logm("INF",5,"trying to update user: " + str(user));

        # Attention: If the body specifies users array and/or applications array  these arrays overrides the existing ones.

        if 'name' in userSchema:
            db.users.update({"_id" : user['_id']}, {"$set" : {"name" : userSchema['name']}});

        if 'email' in userSchema:
            db.users.update({"_id" : user['_id']}, {"$set" : {"email" : userSchema['email']}});

        if 'apples' in userSchema:
            for apple in user['apples']:
                db.users.update({"_id" : user['_id']}, {"$pull" : {"apples" : apple}});
            for apple in userSchema['apples']:
                db.users.update({"_id" : user['_id']}, {"$push" : {"apples" : str(apple)}});

        responseDict = { "oid" : str(user['_id']) };

        return self.jsonResponse(responseDict);



    """
    Method: GET
    Action: Return information about all users
    URL: /adm/users
    Return:
        If ok returns a JSON array containing JSON objects that represents users. Each user object contains the following attributes:
        oid. Mandatory. User's OID.
        name. Mandatory. User's name.
        email. Mandatory. User's email.
        apples. Optional. An array of JSON object that represents Apples. Each apple object contains the following attributes:
        aioi. Mandatory. Apple's AIOI.
        model. Mandatory. Apple's model (like: iPhone3,1).
        version. Mandatory. Apple's operating system version.
        If not ok returns an error in JSON format.
    
    """        

    def users(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'GET':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/users does only allow GET request method");

        db = self._py_object.app_globals.db;
        users = db.users.find();
        responseArray = [];
        for user in users:
            responseArray.append(User(user).encode());

        return self.jsonResponse(responseArray);




    """
    Applications
        remove application (DELETE)
        info about one application (GET)
        info about applications (GET)
    """

    def application(self, id=None):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method == 'GET':
            return self.applicationGet(id);
        elif request.method == 'DELETE':
            return self.applicationDelete(id);
        else:
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/application action only allows GET/DELETE request method");



    """
    Method: DELETE
    Action: Removes the given application
    URL: /adm/application/{oid}
        URL Placeholders: The URL contains the following placeholders:
        oid. Mandatory. Application's OID.
    Return:
        If ok returns a JSON object containing the following attributes:
        oid. Mandatory. Application's removed OID.
        If not ok returns an error in JSON format.
    """

    def applicationDelete(self, id=None):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'DELETE':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/application action does only allow DELETE request method");
        if not id:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/application/id, missing parameter id");

        applicationid = str(id);
        h.logm("DBG",5,"request w/ applicationid=" + applicationid);

        db = self._py_object.app_globals.db;

        application = None;

        try:
            application = db.applications.find_one({'_id': ObjectId(applicationid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB Application Id {'_id' : " + applicationid + '}');
        else:
            if not application:
                return self.jsonErrorResponse(404, "Invalid application id " + applicationid);

            h.logm("INF",5,"found application: " + str(application));

        if 'breadEntries' in application:
            for breadEntry in application['breadEntries']:
                sfh.deletebread(str(applicationid), str(breadEntry['id']));

        db.groups.update({}, {"$pull" : {"applications" : (ObjectId(applicationid))}});
        db.applications.remove({'_id': ObjectId(applicationid)});

        responseDict = { "oid" : str(application['_id']) };

        return self.jsonResponse(responseDict);



    """
    Method: GET
    Action: Return information about the given application
    URL: /adm/application/{oid}
        URL Placeholders: The URL contains the following placeholders:
        oid. Mandatory. Application's OID.
    Return:
        If ok returns a JSON object that represents an application. The following are the definition of these object:
        oid. Mandatory. Application's OID.
        name. Mandatory. Application's name.
        commodityCode. Mandatory. Application's name.
        createdDate. Mandatory. Application's creation date.
        accesssCount. Mandatory. Application's accesss count. Counts all the accesss of all BREAD files of these application.
        breadEntries. Mandatory. An array containing JSON objects that represents breadEntries. Each breadEntry object contains the following attributes:
        oid. Mandatory. ipEntry's OID.
        releaseNotes. Mandatory. ipEntry's release (build) notes.
        accesssCount. Mandatory. ipEntry's accesss count.
        commodityVersion. Mandatory. ipEntry's commodity version.
        fileSize. Mandatory. ipEntry's file size.
        apples. Mandatory. An array of AIOIs of apples.
    """        

    def applicationGet(self, id=None):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'GET':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/applications does only allow GET request method");
        if not id:
            h.logm("ERR",5,"EXCEPTION, parameter missing" + request.method);
            return self.jsonErrorResponse(407, "adm/application/id missing parameter id");

        applicationid = str(id);

        db = self._py_object.app_globals.db;
        try:
            application = db.applications.find_one({'_id': ObjectId(applicationid)});
        except:
            return self.jsonErrorResponse(404, "Invalid Mongo DB Application Id {'_id' : " + applicationid + '}');
        else:
            if not application:
                return self.jsonErrorResponse(404, "Invalid application id " + applicationid);

        return self.jsonResponse(Application(application).encodeWithBreadEntries());


    """
    Method: GET
    Action: Return information about all applications
    URL: /adm/applications
    Return:
        If ok returns a JSON array containing JSON objects that represents applications. Each application object contains the following attributes:
        oid. Mandatory. Application's OID.
        name. Mandatory. Application's name.
        commodityCode. Mandatory. Application's name.
        createdDate. Mandatory. Application's creation date.
        accesssCount. Mandatory. Application's accesss count. Counts all the accesss of all BREAD files of these application.
        breadEntries. Mandatory. An array containing JSON objects that represents breadEntries. Each breadEntry object contains the following attributes:
        oid. Mandatory. ipEntry's OID.
        releaseNotes. Mandatory. ipEntry's release (build) notes.
        accesssCount. Mandatory. ipEntry's accesss count.
        commodityVersion. Mandatory. ipEntry's commodity version.
        fileSize. Mandatory. ipEntry's file size.
        apples. Mandatory. An array of AIOIs of apples.
    """        

    def applications(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());
        if request.method != 'GET':
            h.logm("ERR",5,"EXCEPTION, method is " + request.method);
            return self.jsonErrorResponse(500, "adm/applications does only allow GET request method");

        db = self._py_object.app_globals.db;
        applications = db.applications.find();
        responseArray = [];
        for application in applications:
            responseArray.append(Application(application).encodeWithBreadEntries());

        return self.jsonResponse(responseArray);
