var screenWidth = 800;
var screenHeight = 600;
if(screen){
	screenWidth = screen.width;
	screenHeight = screen.height;
}
var resolution = screenWidth + " / " + screenHeight;
var docFontSize = "8pt";
var docFontSizeSmall = "7pt";
if(resolution == "1024 / 768"){
    docFontSize = "9pt";
    docFontSizeSmall = "8pt";
}else if((resolution == "1152 / 900") || (resolution == "1152 / 864")){
    docFontSize = "11pt";
    docFontSizeSmall = "10pt";
}else if(resolution == "1280 / 1024"){
    docFontSize = "12pt";
    docFontSizeSmall = "11pt";
}else{
    docFontSize = "8pt";
    docFontSizeSmall = "7pt";
}

    // obj       = the <input="button" id="LookupBtn">
    // isDisable = the boolean value=true to disable, false to enable
    function disableLookupBtn(obj, isDisable){
        obj.disabled = isDisable;
        if(isDisable) {
            obj.style.background="url('images/binocularDsbl2.gif')"
        }else {
            obj.style.background="url('images/binocular2.gif')"
        }
    }
    
    // All JSP & HTML Using the style id="", must have this function 
    // invoked on the OnLoad() event like: <Body OnLoad="initLookupBtn(document.myFormName)">
    function initLookupBtn(thisForm) {
	    items = thisForm.elements.length;
	    for(i = 0; i < items; i++) {
		    if(thisForm.elements[i].id == "LookupBtn") {
			    if(thisForm.elements[i].disabled == true) {
			        disableLookupBtn(thisForm.elements[i], true);
			    }
		    }
	    }
    }

function changeLookupView(thisObj)
{
	if(thisObj.disabled == true)
	{
	 	thisObj.style.backgroundColor = "transparent";
	}
	else 
	{
		thisObj.style.backgroundColor = "silver";
	}
	return true;
}


document.writeln('<META http-equiv="Content-Style-Type" content="text/css">');
document.writeln('<style type="text/css">');

	document.writeln('body');
	document.writeln('{');
	document.writeln('background-color : White;');
	document.writeln('font-size: '+docFontSize+ ';');
	document.writeln('font-family: Arial,Helvetica');
	document.writeln('}');

	document.writeln('table.noBorder');
	document.writeln('{');
	document.writeln('border: 0;');
	document.writeln('}');

	document.writeln('table.blackBorder');
	document.writeln('{');
	document.writeln('border-color: #000000;');
	document.writeln('border-style: solid;');
	document.writeln('border-width: 1px;');
	document.writeln('}');

	document.writeln('th.shaded');
	document.writeln('{');
	document.writeln('background-color: #EBC79E;');
	document.writeln('}');

	document.writeln('tr.alternateRow');
	document.writeln('{');
	document.writeln('background-color: #EEEEEE;');
	document.writeln('}');

	document.writeln('tr.headerRow');
	document.writeln('{');
	document.writeln('background-color: #EBC79E;');
	document.writeln('font-weight: bold;');
	document.writeln('vertical-align: BOTTOM;');
	document.writeln('}');

	document.writeln('tr.headerRowSmall');
	document.writeln('{');
	document.writeln('background-color: #EBC79E;');
	document.writeln('font-size: '+docFontSizeSmall+ ';');
	document.writeln('font-weight: bold;');
	document.writeln('vertical-align: BOTTOM;');
	document.writeln('}');

	document.writeln('tr.footerRow');
	document.writeln('{');
	document.writeln('background-color: #EBC79E;');
	document.writeln('font-weight: bold;');
	document.writeln('}');

	document.writeln('td.label');
	document.writeln('{');
	document.writeln('text-align : right;');
	document.writeln('font-size: '+docFontSize+ ';');
	document.writeln('font-weight: bold;');
	document.writeln('font-style: bold;');
	document.writeln('white-space: nowrap;');
	document.writeln('font-family: Arial,Helvetica');
	document.writeln('}');
     	

    document.writeln('td.bold');
	document.writeln('{');
	//document.writeln('text-align : right;');
	document.writeln('font-size: '+docFontSize+ ';');
	document.writeln('font-weight: bold;');
	document.writeln('font-style: bold;');
	document.writeln('font-family: Arial,Helvetica');
	document.writeln('}');

	document.writeln('td');
	document.writeln('{');
	//document.writeln('text-align : left;');
	document.writeln('font-size: '+docFontSize+ ';');
	document.writeln('font-family: Arial,Helvetica');
	document.writeln('}');
      
	document.writeln('td.small');
	document.writeln('{');
	document.writeln('font-size: '+docFontSizeSmall+ ';');
	document.writeln('font-family: Arial,Helvetica');
	document.writeln('}');

	document.writeln('th');
	document.writeln('{');
	document.writeln('text-align : left;');
	document.writeln('font-size: '+docFontSize+ ';');
	document.writeln('font-family: Arial,Helvetica');
	document.writeln('}');

	document.writeln('span.small');
	document.writeln('{');
	document.writeln('font-size: '+docFontSizeSmall+ ';');
	document.writeln('font-family: Arial,Helvetica');
	document.writeln('}');

	document.writeln('.monospace');
	document.writeln('{');
	document.writeln('font-family:Courier,monospace;');
	document.writeln('}');

	document.writeln('.PageTitle {font-family: Arial,Helvetica; font-size: '+docFontSize+'; color: #C0C0C0; text-align: center; text-decoration: none; }');
    document.writeln('#RemoveBtn, #ClearBtn, #AddToQueryBtn, #QueryrBtn, #HelpBtn {  font-family: monospace; }');

    document.writeln('#HiddenField {  font-family: Times; font-size :'+docFontSizeSmall+'; color: navy; border: 0; background-color:#EEEEEE;}');
    document.writeln('#Btn {  font-family: monospace; font-size:'+docFontSizeSmall+';}');
    document.writeln('#LookupBtn {  font-family: monospace; background-color: #EEEEEE; background-image:url("images/binocular2.gif"); background-repeat: no-repeat; }');
    document.writeln('</style>');
