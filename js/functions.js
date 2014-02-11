//
// common functions
//

// adding parse specialization

var _debug = false;
var _moreDebug = false;
var _development = false;

//
// Function: debugWindow(msg)
//
function debugWindow(msg) {
    if (_debug) {
        if (_moreDebug)
        {
            window.alert(msg);
        }
        console.log(msg);
    }
}


// 02/22/2014       // YYYY -> range (6,10)          // MM   -> range (0,2)      // DD   -> range(3, 5)
// MM/DD/YYYY       // 6789                          // 12                       // 45
// 0123456789       //    4                          //  2                       //  2
                    //  0-4

function stringsToDateTime(stringMMDDYYYY, stringHHMM)
{
    var month = stringMMDDYYYY.substring(0,2) - 1;
    var day = stringMMDDYYYY.substring(3,5);
    var year = stringMMDDYYYY.substring(6,10);
    var hour = stringHHMM.substring(0,2);
    var minute = stringHHMM.substring(3,5);
    var gameDate = new Date(year, month, day, hour, minute, 0);
    return gameDate;
}


// convert a string date from YYYY-MM-DD to a js-date()
// 2014-02-22       // YYYY -> range (0,4)          // MM   -> range (5,7)      // DD   -> range(8, 10)
// YYYY-MM-DD       // 0123                         // 45                       // 67
// 0123456789       //    4                         //  2                       //  2
                    //  0-4

function stringsToDateTimeYYYYMMDDHHMM(stringYYYYMMDD, stringHHMM)
{
    var year = stringYYYYMMDD.substring(0,4);
    var month = stringYYYYMMDD.substring(5,7) - 1;
    var day = stringYYYYMMDD.substring(8,10);
    var hour = stringHHMM.substring(0,2);
    var minute = stringHHMM.substring(3,5);
    var gameDate = new Date(year, month, day, hour, minute, 0);
    return gameDate;
}


function commonDateUTC(date) {
    //var datetime = new Date(date.getTime());
    var datetime = ParseClass.dateFromServer();
    datetime.setUTCHours(datetime.getHours());
    datetime.setUTCMinutes(datetime.getMinutes());

    return date;   
}


// return date as string (without time)
function commonJsonDate(date)
{
    if (date)
    {
        var json = null;
        if (typeof(date.toJSON) == "function")
        {
            json = date.toJSON();
        }
        else if (typeof(date.iso) != "undefined")
        {
            json = date.iso;
        }
        else if (typeof(date) == "string")
        {
            json = date;
        }
        else
        {
            return null;
        }
        
        if (json.length > 10)
        {
            var newJsonDate = json.substring(5,7) + '/' + json.substring(8,10) + '/' + json.substring(0,4);
            return newJsonDate;
        }
        else
        {
            return null;
        }
    }
    else
    {
        return null;
    }
}


// return time as string (without date)
function commonJsonTime(date)
{
    if (date)
    {

        var string = null;
        if (typeof(date.toString) == "function")
        {
            string = date.toString();
        }
        else if (typeof(date) == "string")
        {
            string = date;
        }
        else
        {
            return null;
        }

        if (string.length >= (26))
        {
            return string.substring(16,21);
        }
        else
            {
            return null;
        }

    }
    else
    {
        return null;
    }

}
        

//
// Function: handleCommonErrors(dataSourceName)
// Handles common errors with a feed data source.  Will display an error in these cases.
//
// dataSourceName: name of the main feed data source
//
//
function handleCommonErrors(dataSourceName, errorHandler)
{
    var feedDataSource = dashcode.getDataSource(dataSourceName);
    
    // Watch the errorMessage property of the data source, if it changes, check the error
    // and display something.
    feedDataSource.addObserverForKeyPath({}, function(changeNotification, keyPath){
        var errorStatus = feedDataSource.valueForKey('statusCode');
        var errorMessage = feedDataSource.valueForKey('errorMessage');
        
        if (errorMessage) {
            if (errorStatus < 0) { 
                errorHandler("Server information not available: " + errorMessage);
            } else {
                errorHandler("Error status " + errorStatus);
            }
        }
    }, "errorMessage", null);

    // Check to make sure this is RSS or ATOM and that there is at least one article
    feedDataSource.addObserverForKeyPath({}, function(changeNotification, keyPath){
        var entries = feedDataSource.valueForKeyPath('content');
        var foundCount = 0;
        
        if (entries && (DC.typeOf(entries) == 'array')) {
            foundCount = entries.length;
        } else {
            errorHandler(feedDataSource.fullURL() + ' does not appear to be a valid parse object');
            return;
        }
        
        if (!foundCount) {
            debugWindow("Not found any records.");
        }
        
    }, "content", null);    
}


//
// show/hide elements from the screen
//

function hideNode(nodeName)
{
    var node = document.getElementById(nodeName);
    node.style.display = "none";
    node.style.visibility = 'hidden';
}

function showNode(nodeName)
{
    var node = document.getElementById(nodeName);
    node.style.display = "block";
    node.style.visibility = 'visible';
}

function hideButton(buttonName)
{
    var button = document.getElementById(buttonName);
    button.style.display = "none";
    button.style.visibility = 'hidden';
}

function showButton(buttonName)
{
    var button = document.getElementById(buttonName);
    button.style.display = "block";
    button.style.visibility = 'visible';
}

//
// Spinner functions
//

function startSpinner(spinnerName)
{
    var loadingActivityIndicator = document.getElementById(spinnerName);
    if (loadingActivityIndicator)
    {
        loadingActivityIndicator.style.visibility = 'visible';
        loadingActivityIndicator.object.startAnimation();
    }
}

function stopSpinner(spinnerName)
{
    var loadingActivityIndicator = document.getElementById(spinnerName);
    if (loadingActivityIndicator)
    {
        loadingActivityIndicator.style.visibility = 'hidden';
        loadingActivityIndicator.object.stopAnimation();
    }
}




