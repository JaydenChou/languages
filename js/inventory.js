function doQuery(thisForm, windowURL, windowName)
{
                if((this.window.name).indexOf("Inventory") != -1) 
                {
                        if((navigator.platform).indexOf("Win") != -1)
                        {
                                return window.open( windowURL, windowName, 'toolbar=0,location=0,directories=0,status=0,menuBar=0,scrollBars=1,resizable=1,top=50,left=152,height=470px,width=630px');                
                                window.focus();
                        }
                        else
                        {
                                return window.open( windowURL, windowName, 'toolbar=0,location=0,directories=0,status=0,menuBar=0,scrollBars=1,resizable=1,top=50,left=152,height=770px,width=980px');                
                                window.focus();
                        }                
                }
                else
                {
                        parent.navView.location.href = windowURL;
                }
}

function createTable(tableTitle, columnNamesArray, tableArray)
{
        var rows = tableArray.length;
        var cols = columnNamesArray.length +1;
        document.open();
        //creating table structure...
        document.write("<table align='center' cellpadding='2' cellspacing='2' width='100%'>");
        document.write("<tr nowrap>");        
        document.write("<td>");        
        document.write("<table align='center' cellpadding='0' cellspacing='2' width='100%'>");
        // writing caption...
        document.write("<caption align='top'><div class='PageTitle'>" + tableTitle + "</div></caption>");
        // writing rows ... (If it is vector, the row variable will be vector.size().
        for(var i = -1; i < rows; i++) 
        {        
                // Creating a row with Field Names...
                if( i == -1)
                {
                        document.write("<thead><tr bgcolor='#004080' nowrap>");
                        // create an empty column for the radio box
				if((tableTitle == "NNI Inventory")||(tableTitle.indexOf("Reserve DLCIs") != -1)) {
				}
				else {
	                        document.write("<th nowrap>" + " " + "</th>");
				}
                        
                        for(var j = 0;  j < (cols-1); j++)
                        {
                                document.write("<th nowrap>" + columnNamesArray[j] + "</th>");
                        }
                        document.write("</tr></thead>");
                }
                // Creating and filling a row...
                else
                {
                        //document.write("<tbody><tr nowrap>");
	                  document.write("<tbody>");
				if((i % 2) == 0)
				{
                  	      document.write("<tr nowrap bgcolor='#eeeeee'>");
				}
				else
				{
            	            document.write("<tr nowrap bgcolor='#ffffff'>");
				}
                        var choiceVal;
                        // creating a row
                        for(var j = 0;  j < cols; j++)
                        {
                                if(j==0)
                                {
						if((tableTitle == "NNI Inventory")||(tableTitle.indexOf("Reserve DLCIs") != -1)) {
							// document.write('<td nowrap><input type=radio name="RowNo" value="'+i+'"></td>');
						}
						else {
                                        document.write('<td nowrap><input type="radio" name="QueryString" value="' + tableArray[i][j] + '">' + '</td>');
						}
                                }
                                else if(j==1) {
						if(tableTitle == "NNI Inventory") {
                                           // document.write('<td nowrap><input type="radio" name="CircuitIdStringified" value="'+tableArray[i][j-1]+'">&nbsp;&nbsp;<a href="javascript:getSubscriptionDetail(\'' + tableArray[i][j-1] +'\')">' + tableArray[i][j] + '</a></td>');
					if(i == 0) {
                                           document.write('<td nowrap><input type="radio" checked name="QueryString" value="'+tableArray[i][j]+'">&nbsp;&nbsp;<a href="javascript:getSubscriptionDetail(\'' + i +'\')">' + tableArray[i][j] + '</a></td>');
					}
					else {
                                           document.write('<td nowrap><input type="radio" name="QueryString" value="'+tableArray[i][j]+'">&nbsp;&nbsp;<a href="javascript:getSubscriptionDetail(\'' + i +'\')">' + tableArray[i][j] + '</a></td>');
					}
                                    } 
						else if(tableTitle.indexOf("Reserve DLCIs") != -1) {
                                            document.write('<td nowrap><input type="radio" name="rowNo" value="'+ i +'">&nbsp;&nbsp;' + tableArray[i][j] + '</td>');
                                    } 
                                    else if (tableTitle == "Activity Inventory") {
                                            document.write('<td nowrap>' + tableArray[i][j] + '</td>');
                                    }
						else {
                                            document.write('<td nowrap><a href="javascript:getDetail(\'' + tableArray[i][j] +'\')">' + tableArray[i][j] + '</a>' + '</td>');                                    
						}
                                }
                                else 
                                {
                                        document.write("<td nowrap>" + tableArray[i][j] + "</td>");                                
                                }
                        }
                        document.write("</tr></tbody>");                
                }
        }
        document.write("</table>");
        document.write("</td>");        
        document.write("</tr>"); 
        document.write("<tr nowrap>");
        document.write("<td>");        
        document.write("<HR align='center' size='1'>");
        document.write("</td>");                        
        document.write("</tr>");
        document.write("<tr nowrap>");
        document.write("<td>");        
        document.write("<b>Total Rows : " + rows);
        document.write("</b></td>");                        
        document.write("</tr>"); 
        document.write("</table>");        
        document.close();
}

// tableTile        the caption for the table
// culumnNamesArray the column-names, put a '-' or '' for the first element when creating the table 
//                        with a radio button.  Otherwise, give the column-name
// dataArray        the 2-dimentional array of data for the table.   
//                        If creating a radio on the 1st column, give the radio's value on the first column.
//                        Otherwise, give the column data.
// linkArray        the array with the same number of columns as the dataArray.
//                        Give a value of 'radio' to create the radio-button on the 1st column.
//                        Otherwise a 0 for No Hyperlink, and a 1 to Hyperlink.
function radioLinkTable(tableTitle, columnNamesArray, dataArray, linkArray)
{
    radioLinkSelectTable(tableTitle, columnNamesArray, dataArray, linkArray, '');
}

// tableTile        the caption for the table
// culumnNamesArray the column-names, put a '-' or '' for the first element when creating the table 
//                        with a radio button.  Otherwise, give the column-name
// dataArray        the 2-dimentional array of data for the table.   
//                        If creating a radio on the 1st column, give the radio's value on the first column.
//                        Otherwise, give the column data.
// linkArray        the array with the same number of columns as the dataArray.
//                        Give a value of 'radio' to create the radio-button on the 1st column.
//                        Otherwise a 0 for No Hyperlink, and a 1 to Hyperlink.
// radioSelectedEvent the JavaScript Action-Event and the action or function name to carry out when 
//                        the event occurs.  (i.e..  'onClick="checkSelected()"' ).  See order\orderRequestInfo.jsp for usage.
function radioLinkSelectTable(tableTitle, columnNamesArray, dataArray, linkArray, radioSelectedEvent)
{
	var rows = dataArray.length;
	var cols = columnNamesArray.length ;
	document.open();
	//creating table structure...
	document.write("<table align='center' cellpadding='0' cellspacing='0' width='100%'>");
	document.write("<tr nowrap>");
	document.write("<td>");	
	document.write("<table align='center' cellpadding='0' cellspacing='0' width='100%'>");
	// writing caption...
	document.write("<caption align='top'><div class='PageTitle'>" + tableTitle + "</div></caption>");
	// writing rows ... (If it is vector, the row variable will be vector.size().
	for(var i = -1; i < rows; i++) {	
		// Creating a row with Field Names...
		if( i == -1){
			document.write("<thead><tr bgcolor='#004080' nowrap>");
			for(var j = 0;  j < cols; j++){
			    if(j==0){
			        // If the 1st linkArray column says 'radio', then don't print its column name
			        // we are appending it to the seceond column any way...
			        if(linkArray[j] == 'radio'){
			            // Don't print this's column's name
			        }
			        else {
				        document.write("<th nowrap>" + columnNamesArray[j] + "</th><td bgcolor='white'>&nbsp;</td>");
			        }
			    }
			    else {
				    document.write("<th nowrap>" + columnNamesArray[j] + "</th><td bgcolor='white'>&nbsp;</td>");
				}
			}
			document.write("</tr></thead>");
		}
		// Creating and filling a row...
		else{
		    if (i%2 == 1) {
			    document.write("<tbody><tr nowrap bgcolor='EEEEEE'>");
			} else {
			    document.write("<tbody><tr nowrap bgcolor='FFFFFF'>");
			}
			var choiceVal;
			
			// creating a row
			// Note that we are starting from index of '1' and NOT from '0'
			for(var j = 1;  j < cols; j++){
			    // if we are at the 2nd column & the PREVIOUS column in the linkArray says 'radio', then create the radio, Otherwise
			    // treat it as any other column.
				if(j==1){
				    // if PREVIOUS column was a 'radio' button.  Append the PREVIOUS and the CURRENT column to the same table <td> tag
				    if(linkArray[j-1] == 'radio'){
				        if (i==0) {
					        document.write('<td nowrap><input type="radio" checked name="QueryString" value="' + dataArray[i][j-1] + '" ' + radioSelectedEvent + '>&nbsp;');					        
					    }
					    else{				        
					        document.write('<td nowrap><input type="radio" name="QueryString" value="' + dataArray[i][j-1] + '" ' + radioSelectedEvent + '>&nbsp;');
					    }
				        if(linkArray[j] ==1) {
					        document.write('<a href="javascript:getDetail' +j+ '(\'' + dataArray[i][j] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
				        }
				        else {
					        document.write(dataArray[i][j] + "</td><td>&nbsp;</td>");				
				        }					    
					}
					// if PREVIOUS column is NOT a radio button
					else {
					    // if PREVIOUS column was a Hyperlink
				        if(linkArray[j-1] ==1) {
					        document.write('<td nowrap><a href="javascript:getDetail' + 0 + '(\'' + dataArray[i][j-1] +'\')">' + dataArray[i][j-1] + '</a>' + '</td><td>&nbsp;</td>');
				        }
				        else {
					        document.write("<td nowrap>" + dataArray[i][j-1] + "</td><td>&nbsp;</td>");				
				        }
				        
				        // handles the CURRENT column
				        if(linkArray[j] ==1) {
					        document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][j] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
				        }
				        else {
					        document.write("<td nowrap>" + dataArray[i][j] + "</td><td>&nbsp;</td>");				
				        }
				    }
				}
				else if(linkArray[j] ==1) {
//					document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][j] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
                                         if (tableTitle=='Interface Table' && linkArray[3] ==1 && i==0 && j==3 && columnNamesArray[j]=='Port Speed')
                                         {
					    document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][2] +'\',\'' + dataArray[i][12] +'\',\'' + dataArray[i][13] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
                                         }
                                         else if (tableTitle=='Interface Table' && linkArray[3] ==1 && (i== eval( rows-1 )) && j==3 && columnNamesArray[j]=='Port Speed')
                                         {
					    document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][2] +'\',\'' + dataArray[i][14] +'\',\'' + dataArray[i][15] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
                                         }
                                         else if (tableTitle=='Interface Table' && linkArray[3] ==1 && ( i!=0 || i!= eval( rows-1 )) && j==3 && columnNamesArray[j]=='Port Speed')
                                         {
					    document.write('<td nowrap>' + dataArray[i][j] + '</td><td>&nbsp;</td>');
                                         }
                                         else if (tableTitle=='PVC Inventory' && linkArray[3] ==1 && j==3 && ( dataArray[i][3] =='NEW' || dataArray[i][3] =='PRV' || dataArray[i][3] =='INS' || dataArray[i][3]=='DIS' || dataArray[i][3]=='CAN'))
                                         {
					    document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][1] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
                                         }
                                         else if (tableTitle=='Interface Table' && i==(eval(dataArray.length-1)) && linkArray[4] ==1 && j==4 )
                                         {
					    document.write('<td nowrap>' + dataArray[i][j] + '</td><td>&nbsp;</td>');
                                         }
                                         else if (tableTitle=='Interface Table' && linkArray[4] ==1 && j==4 && ( dataArray[i][4] =='NEW' || dataArray[i][4] =='PRV' || dataArray[i][4] =='INS' || dataArray[i][4]=='DIS' || dataArray[i][4]=='CAN'))
                                         {
					    document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][4] +'\',\'' + dataArray[i][11] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
                                         }
                                         else
                                            document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][j] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
				}
				else {
					document.write("<td nowrap>" + dataArray[i][j] + "</td><td>&nbsp;</td>");				
				}
			}
			document.write("</tr></tbody>");		
		}
	}
	document.write("</table>");
	document.write("</td>");	
	document.write("</tr>");
	document.write("<tr nowrap>");
	document.write("<td>");	
	document.write("<HR align='center' size='1'>");
	document.write("</td>");			
	document.write("</tr>");
	//document.write("<tr nowrap>");
	//document.write("<td>");	
	//document.write("<b>Total Rows : " + rows);
	//document.write("</b></td>");			
	//document.write("</tr>");
	document.write("</table>");	
	document.close();
}

//Pops up a new window to show the help for the current screen
function popUpHelp(page){
    if(!window.helpWindow) {
	    helpWindow = window.open(page, "helpWindow", 'toolbar=0,location=0,directories=0,status=0,menuBar=0,scrollBars=1,resizable=1,height=600px,width=600px,screenX=100,screenY=100,top=1,left=100');
    }            
    else {               
        // still open
        if(!helpWindow.closed) {                    
	        helpWindow.close; 
	        helpWindow = window.open(page, "helpWindow", 'toolbar=0,location=0,directories=0,status=0,menuBar=0,scrollBars=1,resizable=1,height=600px,width=600px,screenX=100,screenY=100,top=1,left=100');	                
        }
        else {
	        helpWindow = window.open(page, "helpWindow", 'toolbar=0,location=0,directories=0,status=0,menuBar=0,scrollBars=1,resizable=1,height=600px,width=600px,screenX=100,screenY=100,top=1,left=100');
        }
    }
    helpWindow.focus();            
}	

function arrayToString(tableArray, rows, cols, i)
{
    var str = "";
    
        for(var j = 0;  j < cols; j++) {
                str = str + tableArray[i][j] + "|";
        }
        return (str.substring(0, (str.length-1)));
}


function createSimpleTable(tableTitle, columnNamesArray, tableArray)
{
        var rows = tableArray.length;
        var cols = columnNamesArray.length;
        document.open();
        document.write("<table align='center' cellpadding='2' cellspacing='2' width='100%'>");
        document.write("<tr nowrap>");  
        document.write("<td>");        
        document.write("<table align='center' cellpadding='2' cellspacing='2' width='100%'>");
        document.write("<caption align='top'><div class='PageTitle'>" + tableTitle + "</div></caption>");
        for(var i = -1; i < rows; i++)
        {        
                // Creating a row with Field Names...
                if( i == -1)
                {
                        document.write("<thead><tr bgcolor='#004080' nowrap>");
                        for(var j = 0;  j < cols; j++)
                        {
                                document.write("<th nowrap>" + columnNamesArray[j] + "</th>");
                        }
                        document.write("</tr></thead>");
                }
                // Creating and filling a row...
                else
                {
		                if (i%2 == 1) {
			                document.write("<tbody><tr nowrap bgcolor='EEEEEE'>");
		                } else {
			                document.write("<tbody><tr nowrap bgcolor='FFFFFF'>");
		                }
                        for(var j = 0;  j < cols; j++)
                        {
                                document.write("<td nowrap>" + tableArray[i][j] + "</td>");
                        }
                        document.write("</tr></tbody>");                
                }
        }
        document.write("</table>");
        document.write("</td>");        
        document.write("</tr>");
        document.write("<tr nowrap>");
        document.write("<td>");        
        document.write("<HR align='center' size='1'>");
        document.write("</td>");                        
        document.write("</tr>");
        //document.write("<tr nowrap>");
        //document.write("<td>");        
        //document.write("<b>Total Rows : " + rows);
        //document.write("</b></td>");                        
        //document.write("</tr>"); 
        document.write("</table>");        
        document.close();
}

function createPagedTable(tableTitle, columnNamesArray, dataArray, linkArray)
{
    createEventPagedTable(tableTitle, columnNamesArray, dataArray, linkArray, '');
}

function createEventPagedTable(tableTitle, columnNamesArray, dataArray, linkArray, radioSelectedEvent)
{
	var rows = dataArray.length;
	var cols = columnNamesArray.length;
	document.open();
	//creating table structure...
	document.write("<table align='center' cellpadding='0' cellspacing='0' width='100%'>");
	document.write("<tr nowrap>");
	document.write("<td>");	
	document.write("<table align='center' cellpadding='0' cellspacing='0' width='100%'>");
	// writing caption...
	if (tableTitle != "Added PVCs") {
	    document.write("<caption align='top'><div class='PageTitle'>" + tableTitle + "</div></caption>");
	}
	// writing rows ... (If it is vector, the row variable will be vector.size().
	for(var i = -1; i < rows; i++) {	
		// Creating a row with Field Names...
		if( i == -1){
			document.write("<thead><tr bgcolor='#004080' nowrap>");
			for(var j = 1;  j < cols; j++){
				document.write("<th nowrap>" + columnNamesArray[j] + "</th>" + "<td bgcolor='white'>&nbsp;</td>");
			}
			document.write("</tr></thead>");
		}
		// Creating and filling a row...
		else{
		    if (i%2 == 1) {
			    document.write("<tbody><tr nowrap align=center bgcolor='EEEEEE'>");
			    // document.write("<tbody><tr nowrap align=center bgcolor='E7E7E7'>");
			} else {
			    document.write("<tbody><tr nowrap align=center bgcolor='FFFFFF'>");
			}
			var choiceVal;
			// creating a row
			for(var j = 1;  j < cols; j++){
			    // if its the 1st column & the 1st linkArray says 'radio', then create the radio, Otherwise
			    // treat it as any other column.
				if((j==1) && (linkArray[0] == 'radio')){
					    document.write('<td nowrap><input type="radio" name="BeanString" value="' + dataArray[i][j-1] + '" tabIndex=6 ' + radioSelectedEvent + '>&nbsp;');
				        if(linkArray[j] ==1) {
					        document.write('<a href="javascript:getDetail' +j+ '(\'' + dataArray[i][0] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
				        }
				        else {
					        document.write(dataArray[i][j] + "</td><td>&nbsp;</td>");				
				        }					    
				}
				else if(linkArray[j] ==1) {
				    if (tableTitle == "Activity Inventory") {
				        if (j==4) {
					        document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][1] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
				        } else {
					        document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][0] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
				        }
				    } else if (tableTitle == "Added PVCs") {
					    document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][1] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
				    } else {
					    document.write('<td nowrap><a href="javascript:getDetail' +j+ '(\'' + dataArray[i][j] +'\')">' + dataArray[i][j] + '</a>' + '</td><td>&nbsp;</td>');
				    }
				}
				else {
					document.write("<td nowrap>" + dataArray[i][j] + "</td><td>&nbsp;</td>");				
				}
			}
			document.write("</tr></tbody>");		
		}
	}
	document.write("</table>");
	document.write("</td>");	
	document.write("</tr>");
	document.write("<tr nowrap>");
	document.write("<td>");	
	document.write("<HR align='center' size='1'>");
	document.write("</td>");			
	document.write("</tr>");
	document.write("</table>");	
	document.close();
        
}

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
	windowArray[currIndex] = window.open(winUrl, winName, 'toolbar=0,location=0,directories=0,status=1,menuBar=0,scrollBars=1,resizable=1,height=' +winHeight+ ',width=' +winWidth+ ',screenX=' + winLeft + ',screenY=' + winDown + ',left=' + winLeft + ',top=' + winDown +'');
	windowArray[currIndex].focus();
	tempHdl = windowArray[currIndex];
	currIndex++;
	// Included return statement to return window handle.
      return tempHdl;
}
// Close all children windows once the parent is closed.
// Sometimes if the window.opener need to refresh the data. 
// That time we have to take care to not close the child windows.
// to do this we have to check a flag, that will tell to this method not to close child windows.
// Note: If any document need to stop this auto close command, it needs to set this value to false
//       after this js-lib include.
var closeWinOnCommand = true;
function closeChildWindows() {
  if(closeWinOnCommand)
  {
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

// Strip leading and trailing spaces
/*function stripSpaces(input) {
    var ret = (input.replace(/^\W+/,'')).replace(/\W+$/,'');
    ret = ret.toUpperCase();
    
    return ret;
}*/

function stripSpaces(str) {
    tmpStr = str.replace(/^\s+/g, '').replace(/\s+$/g, '');
    tmpStr = tmpStr.toUpperCase();
    // Missing return statement in previous version...
    return tmpStr;
}

        
// Form the PVC id by padding the value with zeros and append "PVC" at the front
function formPvcId(pvc_value) {
    var padding = "0000000000";
    // Updated this function to fix the PR # 578 (The problem is with forming the PVC ID string).
    // var pvc_id  = "PVC" + padding.substring(0, (padding.length - pvc_value.length)) + pvc_value;
    var pvc_id  = "PVC" + padding.substring(0, (padding.length - pvc_value.length)+1) + pvc_value;
    return pvc_id;
}
      
// used for exportingthe different sets of data for all inventory pages to file       
// theSelectionList  the select list name.
// 
function exportToFile(queryFor, qualifierQueryString) {
    LoadingNextPage = true;
            
    //theSelectionList.disabled=true;            
    document.inventoryListGetNet.QueryType.value  = "EXPORT_TO_FILE";    
    document.inventoryListGetNet.QueryFor.value   = queryFor;    
    document.inventoryListGetNet.QueryString.value = qualifierQueryString;
    document.inventoryListGetNet.submit();
    return true;
}

// This is not being used at this time, since we export every rows of the query
/*function exportToFile(theSelectionList, queryFor, qualifierQueryString) {
    LoadingNextPage = true;
            
	var selectedRange      = theSelectionList.options[theSelectionList.selectedIndex].value;
	var v                  = selectedRange.split('|');
    	    
	var startRow = v[0];
	var endRow   = v[1];
    //theSelectionList.disabled=true;            
    document.inventoryListGetNet.QueryType.value  = "EXPORT_TO_FILE";    
    document.inventoryListGetNet.QueryFor.value   = queryFor;    
    document.inventoryListGetNet.QueryString.value = qualifierQueryString;
    document.inventoryListGetNet.minRow.value = startRow;           
    document.inventoryListGetNet.maxRow.value = endRow;           
    document.inventoryListGetNet.submit();
    return true;
}
*/        
// used for getting the different sets of data all inventory pages        
// theSelectionList  the select list name.
// 
function getNextSet(theSelectionList, queryType, qualifierQueryString) {
    LoadingNextPage = true;
            
	var selectedRange      = theSelectionList.options[theSelectionList.selectedIndex].value;
	var v                  = selectedRange.split('|');
    	    
	var startRow = v[0];
	var endRow   = v[1];
    theSelectionList.disabled=true;            
    document.inventoryListGetNet.QueryType.value   = queryType;    
    document.inventoryListGetNet.QueryString.value = qualifierQueryString;
    document.inventoryListGetNet.minRow.value = startRow;           
    document.inventoryListGetNet.maxRow.value = endRow;           
    document.inventoryListGetNet.submit();
    return true;
}

function preSelectRange(theRange, theSelectionList){
// Change the selection box's selected item base on the given text value
    var found      = false;
   // var theSelectionList   = document.searchInventories.range;
            
    // pre-select the agent country list
    if((theRange.length >0) && (theSelectionList.length >0)){
        for(var i=0; (i<theSelectionList.length) && !found; i++) {
            var selVal = theSelectionList.options[i].text;
            // alert("selVal="+selVal+" theRange="+theRange);
            if(selVal.indexOf(theRange) == 0) {
                found = true;
                theSelectionList.options[i].selected = true;
            }
        }
        if(!found) {
            theSelectionList.options[0].selected = true;
        }                
    }            
}
