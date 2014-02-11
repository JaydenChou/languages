
// This is used for opening the Query pages
// If the cookie for "xpoe.preference.multiple_page" is set to "YES" then
// it will get and return the long value of time to generate a uniqe string 
// so that a different window name can be generated.   Otherwise, return the empty string
function getWindowSuffixName() {
    
	var multiple_page = getCookie("xpoe.preference.multiple_page");
	if(multiple_page=="YES") {
	    var date = new Date();
        return(date.getTime());
	}
	return "";
}

// This is used for popup windows from the inventory pages....
// If the cookie for "xpoe.preference.multiple_page" is set to "YES" then
// get and return the current window name.   Otherwise, return empty string
function getCurrentWindowName() {
	var multiple_page = getCookie("xpoe.preference.multiple_page");
	if(multiple_page=="YES") {    
        return window.name;
    }
    return "";
}

function getThisWindowName() {
    return window.name;
}

// This is used for popup windows from the Add PVC....
// If the cookie for "xpoe.preference.multiple_page" is set to "YES" then
// get and return the current top window's name.   Otherwise, return empty string
function getTopWindowName() {
	var multiple_page = getCookie("xpoe.preference.multiple_page");
	if(multiple_page=="YES") {    
        return top.window.name;
    }
    return "";
}


// keeping an arry of all the child windows opened,
// and close them all upon itself is closed,
// if the closeChildWindows is invoked on the OnUnLoad event
var windowArray = new Array();
var currIndex   = 0;



// All popup windows should use this function
function popUpWindow(winUrl, winName, winHeight, winWidth, winLeft, winDown) {    
    found = false;
    for(i=0; i<windowArray.length; i++) {
        if (windowArray[i] && (!windowArray[i].closed) && (windowArray[i].name == winName)){
            windowArray[i].focus();
            if(winUrl.length>5)
                windowArray[i].location.href=winUrl;
            found = true;
        }
    }    
    if(!found) {
	    windowArray[currIndex] = window.open(winUrl, winName, 'toolbar=0,location=0,directories=0,status=1,menuBar=0,scrollBars=1,resizable=1,height=' +winHeight+ ',width=' +winWidth+ ',screenX=' + winLeft + ',screenY=' + winDown + ',left=' + winLeft + ',top=' + winDown +'');
	    windowArray[currIndex].focus();
	    currIndex++;
	}
}
// Close all children windows once the parent is closed.
// Sometimes if the window.opener need to refresh the data. 
// That time we have to take care to not close the child windows.
// to do this we have to check a flag, that will tell to this method not to close child windows.
// Note: If any document need to stop this auto close command, it needs to set this value to false
//       after this js-lib include.
var closeWinOnCommand = true;
function closeChildWindows() {
    if(closeWinOnCommand){
        for(i=0; i<windowArray.length; i++) {
            if (windowArray[i] && !windowArray[i].closed){
                if(windowArray[i].closeChildWindows) {
                    windowArray[i].closeChildWindows();
                }
                windowArray[i].close();
            }
        }
    }
}

// For S/O's Cancel buttons, or window close only....
// close all the popup windows for lookup information
function closeLookupInfoWin() {
    if(closeWinOnCommand){
        // Make sure to grab the handle to the popup windows
        prefix = getThisWindowName();
        popUpWindow("", prefix+"contactListWin", 1, 1, 1, 1);
        popUpWindow("", prefix+"lookupInfoWin", 1, 1, 1, 1);
        popUpWindow("", prefix+"contactListWincontactInfoWin", 1, 1, 1, 1);
        
        // Call this function to close all these popups.
        closeChildWindows();
    }    
}