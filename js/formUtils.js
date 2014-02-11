
// returns the selected value of the RADIO button object
// Usage :  document.searchInventories.myRadio
function getSelectedRadioValue(radioObj) {        
    var selectedRow="";
    // if there are more than 1 radio button on the form
    if(radioObj && radioObj.length >1){
        for(var i=0; i < radioObj.length; i++) {
            if(radioObj[i].checked) {
                selectedRow = radioObj[i].value;
            }
        }
        if(selectedRow != "") {
            return selectedRow;
        }
        return false;
    }
    else {
        if(radioObj.checked) {
            return(radioObj.value);
        }
        return false;            
    }
}



// returns the selected value of the SELECT (dropdown list) object
// Usage :  document.searchInventories.mySelect
function getSelectedSelectBoxValue(selectObj) {        
    return (selectObj.options[selectObj.selectedIndex].value);
}


// Select the passed string for select (dropdown) list.
// Note: This function will compare the text of dropdown.
function selectSelectBoxText(selectObj, textStr) {
	for(i=0; i<selectObj.options.length; i++) {
		if(selectObj.options[i].text == textStr) selectObj.options[i].selected = true;
	}
}


// Select the passed string for select (dropdown) list.
// Note: This function will compare the value of dropdown.
function selectSelectBoxValue(selectObj, valueStr) {
	for(i=0; i<selectObj.options.length; i++) {
		if(selectObj.options[i].value == valueStr) selectObj.options[i].selected = true;
	}
}



// Strip leading and trailing spaces
function trim(str) {
    return str.replace(/^\s+/g, '').replace(/\s+$/g, '');
}


function trimToUpper(str) {
    return (trim(str).toUpperCase());
}





// Invoke this function to get the lookup information relating to Lookup___
function getLookupInfo(entryObj, selectTableEnum){
	getLookupLocInfo(entryObj, selectTableEnum, "");      
}
// Invoke this function to get the lookup information relating to Lookup___
function getLookupLocInfo(entryObj, selectTableEnum, locationType){
    winName =  window.name + "lookupInfoWin";
    
	formLookupValue = entryObj;
	lookupurl = "soQueryServlet?ActionType=LOOKUP_INFOS&SELECT_TABLE_ENUM="+ selectTableEnum +"&LOCATION_TYPE="+ locationType;
    popUpWindow(lookupurl, winName, 650, 550, 400, 20);
}
// This function is being called by the popUp window above(lookupInfo.jsp) when a lookup code info is selected.            
function setLookupInfo(selectedCode) {
	var myArray = selectedCode.split('|');
			
	if(formLookupValue.type == 'text') {
		formLookupValue.value = myArray[0];
	}
	else{
		addOption(formLookupValue, myArray[0], myArray[0]);
	}
}                


            


// Invoke this function to get the lookup information relating to ComboToken
function getComboTokenInfo(entryObj, entryDesc, selectTableEnum){
	formEntryValue = entryObj;
	formEntryDesc  = entryDesc;
    winName =  window.name + "lookupInfoWin";
	lookupurl = "soQueryServlet?ActionType=INFO_LIST_SELECTION&SELECT_TABLE_ENUM="+ selectTableEnum;
    popUpWindow(lookupurl, winName, 650, 550, 400, 20);
}
		
// This function is being called by the popUp window above(comboTokenInfo.jsp) when a lookup code info is selected.            
function setComboTokenInfo(selectedCode) {
	var myArray = selectedCode.split('|');
			
	if(formEntryValue.type == 'text') {
		formEntryValue.value = myArray[0];
	}
	else{
		addOption(formEntryValue, myArray[0], myArray[0]);
	}
	if((formEntryDesc != null) && (formEntryDesc != ""))  {
		formEntryDesc.value = myArray[1];
	}
}                
		
	
		
		
// Check if the theText is already exist in the Select list
function isOptionExist(theSel, theText){            
    var found = false;
    if((theText.length >0) && (theSel.length >0)){
        for(var i=0; (i<theSel.length) && !found; i++) {
            var selVal = theSel.options[i].text;
            if(selVal.indexOf(theText) == 0) {
                return true;
            }
        }
    }
    return false;
}
		
function addOption(theSel, theText, theValue){
    // The (!(isOptionExist(theSel, theText))) check is to not allow adding
    // multiple of the same value to a select list.
    // But for FOC contact we don't anticiplate users will add miltiple of the 
    // same contact....  setContactInfo()
	if((theText != "") && (!(isOptionExist(theSel, theText)))){
		var newOpt = new Option(theText, theValue);
		var selLength = theSel.length;
		//alert("selLength="+selLength);
		if ((selLength ==1) && (theSel.options[0].value =="")){
			selLength=0;
		}
		theSel.options[selLength] = newOpt;
		theSel.options[selLength].selected = true;
	}
}
		
function deleteOption(entryObj){        
	var selLength = entryObj.length;
	if(selLength>0){
		if(entryObj.selectedIndex >=0) 
			entryObj.options[entryObj.selectedIndex] = null;
	}
	selLength = entryObj.length;
	if(selLength>0){
		entryObj.options[selLength-1].selected = true;
	}
}

function digitOnly(what, e) {
    var key = (navigator.appName == "Netscape") ? e.which : e.keyCode;
    // Nescape: If the Backspace='8' or Delete/Up/Down/Left/Right='0', then let it goes
    if (key == 0 || key == 8) {return true;}
         
    // If the key pressed is a digit from [0..9]
    if (key > 47 && key < 58) {
        return true;
    }else return false;
}


// Masking the NPA/NXX value as user is entering it
// Entering of the masking characters is optional
function maskNpaNxx(what, e) {
    var key = (navigator.appName == "Netscape") ? e.which : e.keyCode;
    var len = what.value.length;
    var badBrowser = ((navigator.org.indexOf("netscape") !=-1) && (navigator.version < 6));
        
    // Remove the default masking....
    if(what.value.indexOf("___/___") != -1) {
        what.value = "";
    }    
    // Nescape: If the Backspace='8' or Delete/Up/Down/Left/Right='0', then let it goes
    if (key == 0 || key == 8) {return true;}
         
    // If the key pressed was either a '/' or a digit from [0..9]
    if (key > 46 && key < 58) {
        switch (len){
            case 0:     
            case 1:     
            case 2:     
            case 4:     
            case 5:     
            case 6:     
                if(key == 47) return false;         // The '/' seperater is not at the correct place
                break;
            case 3:     
                // Don't even try giving the masking with Netscape 4.xx
                if (!badBrowser) {            
                    if(key != 47) what.value += "/";    // User didn't put a '/', let's give them one 
                }
                break;
        }
        return true;
    }else return false;
}

// Validating and reformating the NPA/NXX value
// Reset and Return the value for the NPA/NXX field with the formated value    
function validateNpaNxx(npaNxx) {
    var stripped;
    var formated ="";
        
    stripped = npaNxx.value.replace(/[\/]/g, '');
    if(stripped.length == 6) {
        formated = stripped.substring(0,3) +"/"+ stripped.substring(3,stripped.length);
        npaNxx.value = formated;
    }
    else {
        alert("Invalid NPA/NXX - "+  npaNxx.value +".  Six digit NPA/NXX expected.");
        npaNxx.select();
        npaNxx.focus();
        return false;
    }
    return formated;
}
 
 
function setTxtMxLen(field, len){
    field.maxLength = len;   
}
function maskPhone(what, e, countryCode) {
    var key = (navigator.appName == "Netscape") ? e.which : e.keyCode;
    var badBrowser = ((navigator.org.indexOf("netscape") !=-1) && (navigator.version < 6));
    // Remove the default masking....
    if(what.value.indexOf("___") != -1) {
        what.select();
    }    
    var len = what.value.length;

    // Nescape: If the Backspace='8' or Delete/Up/Down/Left/Right='0', then let it goes
    if (key == 0 || key == 8) {return true;}
    if((countryCode.toUpperCase() == "US") || (countryCode.toUpperCase() == "CA")) {
        // If the key pressed was either a '(', ')', '-', 'x' or a digit from [0..9]
        if ((key > 47 && key < 58) || key == 40 || key == 41 || key == 45 || key == 120 || key == 88) {
            switch (len){                
                case 2:     
                case 3:     
                case 5:                 
                case 6:     
                case 7:     
                case 9:     
                case 10:     
                case 11:     
                case 12:     
                case 14:     
                case 15:     
                case 16:     
                case 17:                 
                    if(key == 40 || key == 41 || key == 45 || key == 120 || key == 88) return false;         // The seperater is not at the correct place
                    break;
                case 0: // Don't even try giving the masking with Netscape 4.xx
                    if(key == 41 || key == 45 || key == 120 || key == 88) return false; // Wrong seperater            
                    if (!badBrowser) {            
                        if(key != 40) what.value += "(";    // User didn't put a '(', let's give them one 
                    }
                    break;
                case 1: // incase we missed the '(' from case 0:
                    if(key == 40 || key == 41 || key == 45 || key == 120 || key == 88) return false;  
                    if (!badBrowser) {            
                        if(what.value.indexOf("(") == -1) what.value = "(" + what.value;    // If the Phone number had a value and it was highlighted, we need this
                    }                    
                    // The seperater is not at the correct place
                    break;                    
                case 4: // Don't even try giving the masking with Netscape 4.xx
                    if(key == 40 || key == 45 || key == 120 || key == 88) return false; // Wrong seperater                        
                    if (!badBrowser) {            
                        if(key != 41) what.value += ")";    // User didn't put a ')', let's give them one 
                    }
                    break;           
                case 8: // Don't even try giving the masking with Netscape 4.xx
                    if(key == 40 || key == 41 || key == 120 || key == 88) return false; // Wrong seperater                        
                    if (!badBrowser) {            
                        if(key != 45) what.value += "-";    // User didn't put a '-', let's give them one 
                    }
                    break;
                case 13:// Don't even try giving the masking with Netscape 4.xx
                    if(key == 40 || key == 41 || key == 45) return false; // Wrong seperater            
                    if (!badBrowser) {            
                        if((key != 120) &&(key != 88)) what.value += "x";    // User didn't put a 'x', let's give them one 
                    }
                    break;
            }
            return true;
        }else {
            return false;
        }
    }    
    else{// Free format international phone number
        switch (len){
            case 0: 
                // Except only numbers or '+' as the first entry
                if (!(key == 43 || (key > 47 && key < 58))) return false;
                if (!badBrowser) { // Don't even try giving the masking with Netscape 4.xx
                    if(key != 43) what.value += "+";    // User didn't put a '+', let's give them one 
                }
                break;
            case 1: // Except only numbers for the first digit after the '+' sign
                if((key < 47 || key > 58)) return false;
                if (!badBrowser) {            
                    if(what.value.indexOf("+") == -1) what.value = "+" + what.value;    // If the Phone number had a value and it was highlighted, we need this
                }                                    
                break;
        }
        return true;
    }
}
function maskZip(what, e, countryCode) {
    var key = (navigator.appName == "Netscape") ? e.which : e.keyCode;
    var badBrowser = ((navigator.org.indexOf("netscape") !=-1) && (navigator.version < 6));
    // Remove the default masking....
    if(what.value.indexOf("___") != -1) {
        what.select();
    }    
    var len = what.value.length;

    // Nescape: If the Backspace='8' or Delete/Up/Down/Left/Right='0', then let it goes
    if (key == 0 || key == 8) {return true;}
    if((countryCode.toUpperCase() == "US") || (countryCode.toUpperCase() == "CA")) {
        // If the key pressed was either a '-', or a digit from [0..9]
        if ((key > 47 && key < 58) || key == 45) {
            switch (len){                
                case 5: // Don't even try giving the masking with Netscape 4.xx
                    if (!badBrowser) {            
                        if(key != 45) what.value += "-";    // User didn't put a ')', let's give them one 
                    }
                    break;           
            }
            return true;
        }else return false;
    }    
    else{// Free format PC
        return true;
    }
}

// Validating and reformating the phone number for US and CA, plus the International
// Reset and Return the value for the phone field with the formated value    
function validateZip(zipCode, countryCode) {
    var stripped;
    var formated ="";
    if((countryCode.toUpperCase() == "US") || (countryCode.toUpperCase() == "CA")) {
        stripped = zipCode.value.replace('-', '');
        
        if(stripped.length == 5) {
            formated = stripped;
        }else if((stripped.length > 5) && (stripped.length < 10)) {
            formated = stripped.substring(0,5) +"-"+ stripped.substring(5, stripped.length);            
        }else if(stripped.length > 9) {
            formated = stripped.substring(0,5) +"-"+ stripped.substring(5, 9);            
        }else {
            zipCode.value = "";
            zipCode.focus();
            zipCode.select();
        }
        if(formated !="") {
            zipCode.value = formated;
        }        
    }
    else { // Freeformat International zipCode number
        if(zipCode.value.indexOf("____") != -1) {
            formated = "";
        }else {
            formated = zipCode.value;
        }
    }    
    return formated;
}


function setTxtMxLenDom(phone, zipPc, countryCode){
    if((countryCode.toUpperCase() == "US") || (countryCode.toUpperCase() == "CA")) {
        phone.maxLength = "18";
        phone.value     = "(___) ___-____ x____";
        zipPc.value     = "_____-____";
    }else {
        phone.maxLength = "25";        
        phone.value     = "+________________________";
        zipPc.value     = "__________";
    }
}
    
    
// Validating and reformating the phone number for US and CA, plus the International
// Reset and Return the value for the phone field with the formated value    
function validatePhone(phone, countryCode) {
    var stripped;
    var formated ="";
    if((countryCode.toUpperCase() == "US") || (countryCode.toUpperCase() == "CA")) {
        stripped = phone.value.replace(/[\(\)\.\-\x\_\ ]/g, '');
        
        if(stripped.length == 10) {
            formated = "("+ stripped.substring(0,3) +")"+ stripped.substring(3,6) +"-"+ stripped.substring(6,10);
        }else if((stripped.length > 10) && (stripped.length < 15)){
            formated = "("+ stripped.substring(0,3) +")"+ stripped.substring(3,6) +"-"+ stripped.substring(6,10) +"x"+ stripped.substring(10, stripped.length);            
        }else if(stripped.length > 14){
            formated = "("+ stripped.substring(0,3) +")"+ stripped.substring(3,6) +"-"+ stripped.substring(6,10) +"x"+ stripped.substring(10, 14);            
        }
        else {
            phone.value = "(___) ___-____ x____";
            phone.focus();
            phone.select();
        }
        if(formated !="") {
            phone.value = formated;
        }        
    }
    else { // Freeformat International phone number
        if(phone.value.indexOf("+___") != -1) {
            formated = "";
        }else {
            stripped = phone.value.replace(/[\+\ ]/g, '');
            if(stripped.length < 25) {
                formated = "+"+ stripped;
            }else {
                formated = "+"+ stripped.substring(0,24);
            }
        }
    }    
    return formated;
}

    
// Masking the Date value as user is entering it
// Entering of the masking characters is optional
function maskDate(what, e) {
    var key = (navigator.appName == "Netscape") ? e.which : e.keyCode;
    var badBrowser = ((navigator.org.indexOf("netscape") !=-1) && (navigator.version < 6));        
    // Remove the default masking....
    if(what.value.indexOf("MM/DD/") != -1) {what.value = ""; }    
    var len = what.value.length;
    var now = new Date();
    var year = now.getFullYear();
    
    // Nescape: If the Backspace='8' or Delete/Up/Down/Left/Right='0', then let it goes
    if (key == 0 || key == 8) {return true;}
    // Don't let user enter any more after we have enough of 10 charaters
    // if ((len > 9) && (what.value.indexOf("MM/DD/") == -1)) {return false;}
         
    // If the key pressed was either a '/' or a digit from [0..9]
    if (key > 46 && key < 58) {
        switch (len){
            case 0:     
            case 1:     
            case 3:     
            case 4:     
            case 6:     
            case 7:     
            case 8:     
            case 9:     
                if(key == 47) return false;         // The '/' seperater is not at the correct place
                break;
            case 2:     
                // Don't even try giving the masking with Netscape 4.xx
                if (!badBrowser) {            
                    if(key != 47) what.value += "/";    // User didn't put a '/', let's give them one 
                }
                break;
            case 5:     
                // Don't even try giving the masking with Netscape 4.xx
                if (!badBrowser) {            
                    what.value += "/"+ year;
                    //if(key != 47) what.value += "/"+ year;    // User didn't put a '/', let's give them one                     
                    //if(key == 47) what.value += year;    // User didn't put a '/', let's give them one                     
                }
                break;
        }
        return true;
    }else return false;
}

function appendYear(what, e) {
    var len = what.value.length;
    var now = new Date();
    var year = now.getFullYear();
    var key = (navigator.appName == "Netscape") ? e.which : e.keyCode;
    
    // Nescape: If the Backspace='8' or Delete/Up/Down/Left/Right='0', then let it goes
    if (key == 0 || key == 8) {return true;}
    // Don't let user enter any more after we have enough of 10 charaters
    // if ((len > 9) && (what.value.indexOf("MM/DD/") == -1)) {return false;}
         
    // If the key pressed was either a '/' or a digit from [0..9]
    if (len == 5) {
        what.value += "/"+ year;   
        return true;
    }
    return false;
}

// Validating and reformating the Date value and checks the validity of the date
// Reset and Return the value for the phone field with the formated value    
function validateDate(myDate) {
    var stripped;
    var formated ="";
        
    stripped = myDate.value.replace(/[\-\/\ ]/g, '');
    if(stripped.length == 8) {
        formated = stripped.substring(0,2) +"/"+ stripped.substring(2,4) +"/"+ stripped.substring(4,8);
        myDate.value = formated;
    }
    var rt = isValidDate(myDate, "/");
    return formated;
}

// This function accepts 10 digit string with 2nd argument that specifies 
// the deliminator and checks for the validity of the date.
function isValidDate(myDateObj, sep) {
    myDate = myDateObj.value;
    // checks if date passed is in valid mm/dd/yyyy format
    if (myDate.length == 10) {
        if (myDate.substring(2,3) == sep && myDate.substring(5,6) == sep) {
	        var month  = myDate.substring(0,2);
	        var date = myDate.substring(3,5);
	        var year  = myDate.substring(6,10);

	        var test = new Date(year, month-1, date);

	        if (year == y2k(test.getYear()) && (month-1 == test.getMonth()) && (date == test.getDate())) {
	            return true;
	        }
	        else {
	            alert( "The request date has an invalid date!  Enter a valid date and try again." );
	            myDateObj.select();
	            myDateObj.focus();
	            return false;
	        }
	    }
	    else {
	        alert( "The request date must be in a format exactly like MM/DD/YYYY with / as the delimiter." );
	        myDateObj.select();
	        myDateObj.focus();
	        return false;
	    }
    }
    else {
	    alert( "The request date must be in a format of MM/DD/YYYY with 2 digits for month and date and 4 digits for year." );
	    myDateObj.select();
	    myDateObj.focus();
	    return false;
	}
}
    
    
// Helper function for isValidDate function.
function y2k(number) { 
    return (number < 1000) ? (number + 1900) : number; 
}


// Change the selection box's selected item base on the given text value
function changeSelection(listObj, selectText) {       
    var found      = false;
            
    if((selectText.length >0) && (listObj.length >0)){
        for(var i=0; (i<listObj.length) && !found; i++) {
            var selVal = listObj.options[i].text;
            if(selVal.indexOf(selectText) == 0) {
                found = true;
                listObj.options[i].selected = true;
            }
        }
        if(!found) {
            listObj.options[0].selected = true;
        }                
    }
}

function getParm(string,parm) {
    // returns value of parm from string
    var startPos = string.indexOf(parm + "=");
    if (startPos > -1) {
        startPos = startPos + parm.length + 1;
        var endPos = string.indexOf("&",startPos);
        if (endPos == -1)
            endPos = string.length;
        return unescape(string.substring(startPos,endPos));
    }
    return '';
}
//var passed = location.search.substring(1);
//document.formname.myfield.value = getParm(passed,'AAO');
//document.formname.otherfield.value = getParm(passed,'OCD');

// Masking the input fields to allow numaric inputs.
function maskAsNumaric(what, e) {
    var key = (navigator.appName == "Netscape") ? e.which : e.keyCode;
    var len = what.value.length;
    var badBrowser = ((navigator.org.indexOf("netscape") !=-1) && (navigator.version < 6));
        
    // Nescape: If the Backspace='8' or Delete/Up/Down/Left/Right='0', then let it goes
    if (key == 0 || key == 8) {return true;}
         
    // If the key pressed was either a '/' or a digit from [0..9]
    if (key > 46 && key < 58) {
        switch (len){
            case 0:     
            case 1:     
            case 2:  
            case 3:       
            case 4:     
            case 5:     
            case 6:     
                if(key == 47) return false;         // The '/' seperater is not at the correct place
                break;
        }
        return true;
    }else return false;
}

// Masking the Date value as user is entering it
// Entering of the masking characters is optional
// USAGE: <input type="text name="fieldName" value"MM/DD/YYYY" size="12" maxlength="10" onKeyPress="return dateMask(this, event);" onKeyUp="return appendYear(this, event);"/>
function dateMask(what, e) {
    var key = (navigator.appName == "Netscape") ? e.which : e.keyCode;
    var badBrowser = (document.layers);        
    // Remove the default masking....
    if(what.value.indexOf("MM/DD/") != -1) {what.value = ""; }    
    var len = what.value.length;
    var now = new Date();
    var year = now.getFullYear();
    
    // Nescape: If the Backspace='8' or Delete/Up/Down/Left/Right='0', then let it goes
    if (key == 0 || key == 8) {return true;}
    // Don't let user enter any more after we have enough of 10 charaters
    // if ((len > 9) && (what.value.indexOf("MM/DD/") == -1)) {return false;}
         
    // If the key pressed was either a '/' or a digit from [0..9]
    if (key > 46 && key < 58) {
        switch (len){
            case 0:     
            case 1:     
            case 3:     
            case 4:     
            case 6:     
            case 7:     
            case 8:     
            case 9:     
                if(key == 47) return false;         // The '/' seperater is not at the correct place
                break;
            case 2:     
                // Don't even try giving the masking with Netscape 4.xx
                if (!badBrowser) {            
                    if(key != 47) what.value += "/";    // User didn't put a '/', let's give them one 
                }
                break;
            case 5:     
                // Don't even try giving the masking with Netscape 4.xx
                if (!badBrowser) {            
                    what.value += "/"+ year;
                    //if(key != 47) what.value += "/"+ year;    // User didn't put a '/', let's give them one                     
                    //if(key == 47) what.value += year;    // User didn't put a '/', let's give them one                     
                }
                break;
        }
        return true;
    }else return false;
}
function appendYear(what, e) {
    var len = what.value.length;
    var now = new Date();
    var year = now.getFullYear();
    var key = (navigator.appName == "Netscape") ? e.which : e.keyCode;
    
    // Nescape: If the Backspace='8' or Delete/Up/Down/Left/Right='0', then let it goes
    if (key == 0 || key == 8) {return true;}
    // Don't let user enter any more after we have enough of 10 charaters
    // if ((len > 9) && (what.value.indexOf("MM/DD/") == -1)) {return false;}
         
    // If the key pressed was either a '/' or a digit from [0..9]
    if (len == 5) {
        what.value += "/"+ year;   
        return true;
    }
    return false;
}
