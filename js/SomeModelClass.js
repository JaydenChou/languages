//
// Class: SomeModelClass
// model class to allocate kvo from some
//
// 
SomeModelClass = Class.create(DC.KVO, {
    constructor: function(parseObject) {
        this.parseObject = parseObject;
        if (!parseObject.isNew())
        {
            this.name = parseObject.get("name");
            this.objectId = parseObject.id;
            this.someUrl = parseObject.get("someUrl");
        }
    }
});



//
// static public methods
// @scope SomeModelClass
//
Object.extend(SomeModelClass, {

    //
    // Function: query
    //
    query: function(finishBlock) 
    {

        debugWindow(arguments.callee.name);
        var loadingActionTextLabel = document.getElementById('loadingActionTextLabel');
        loadingActionTextLabel.innerHTML = "Loading some...";    
        var ds = dashcode.getDataSource('some');
        var query = new Parse.Query(Parse.Object.extend("xxx_some"));
        query.find({
            success: function(results) {
            debugWindow("Successfully retrieved " + results.length + " somes.");

            var inventory = ds.valueForKeyPath('content');
            inventory.removeAllObjects();
            if (results.length > 0)
            {
                var parseObject = results[0]; // only one some
                var newObject = new SomeModelClass(parseObject);
                inventory.addObject(newObject);
            }

            finishBlock();
          },
          error: function(error) {

            showErrorView('Error when expecting COSAS: ' + error.message, {
                close: function()
                {
                    finishBlock();
                }
            });
          }
        });
    }
    
});






