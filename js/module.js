Parse.Cloud.define('timestamp', function(request, response) 
{
    var user = request.user;
    if (!user) 
    {
        response.error('Must be signed in to call this Cloud Function.')
        return;
    }

    // The rest of the function operates on the assumption that request.user is *authorized*
    Parse.Cloud.useMasterKey();

    var query = new Parse.Query(Parse.User);
    query.equalTo('objectId', user.id);

    // Get the first user which matches the above constraints.
    query.first ({
        success: function(aUser) 
        {
            response.success(new Date().getTime());
        },
        error: function(error) {
            response.error('Could not find user.');
        }
    });
});

