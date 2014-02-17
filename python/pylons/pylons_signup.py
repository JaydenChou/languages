import logging

from pylons import request, response, session, tmpl_context as c, url
from pylons.controllers.util import abort, redirect
import cocoamilk.lib.helpers as h;
import formencode
from formencode import htmlfill;

from cocoamilk.lib.base import BaseController, render

from cocoamilk.model.user import User;
from cocoamilk.model.group import Group;

log = logging.getLogger(__name__)

class SignupValidator(formencode.Schema):
    allow_extra_fields = True;
    filter_extra_fields = True;
    name = formencode.validators.String(not_empty=True);
    email = formencode.validators.Email(not_empty=True)

class SignupController(BaseController):

    def index(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());

        db = self._py_object.app_globals.db;
        c.users = db.users.find();

        c.message = '';
        c.email = '';
        c.logged = True;
        return render('/signup.mako')

    def submit(self):
        h.logm("DBG",5,"ACTION "+ h.getcalled());

        db = self._py_object.app_globals.db;
        c.users = db.users.find();
        c.email = '';
        c.logged = True;
        c.message = '';
        schema = SignupValidator();
        try:
            form_result = schema.to_python(request.params);
            h.logm("DBG",5,"f r "+ str(request.params));

            
        except formencode.Invalid, error:
            h.logm("ERR",5,"f r "+ str(request.params));

            c.message = h.literal(htmlfill.escapenl_formatter(error));
            return render('/signup.mako');

        else:

            user = User(request.params);

            if db.users.find({'email': user.email}).count():
                c.message = ("email " + request.params['email'] + " already exist");
                return render('/signup.mako');
            elif db.users.find({'name': user.name}).count():
                c.message = ("name " + request.params['name'] + " already exist");
                return render('/signup.mako');
            else:
                h.logm("INF",5,"user "+ str(user));
                encoded = user.encode();
                h.logm("INF",5,"user encoded "+ str(encoded));

                db.users.save(user.encode()); 
                h.redirect(url(controller='home', action='index'));

                

