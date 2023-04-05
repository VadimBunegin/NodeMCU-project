function doGet(e) { 
  Logger.log( JSON.stringify(e) );
  var result = 'Ok';
  if (e.parameter == 'undefined') {
    result = 'No Parameters';
  }
  else {
    
    

    var sheet_id = '1_D2hvjHOGG9Gi1ZxsClz12WPox_ixWU-7EeWy-JZS6M'; 	// Spreadsheet ID
    var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();
    var newRow = sheet.getLastRow()+1;						
    var rowData = [];
    var Curr_Date = new Date();
    rowData[0] = Curr_Date; 
    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]);
      Logger.log(param + ':' + e.parameter[param]);
      if(param == "value"){
        
        
       var cell = sheet.getRange("B"+newRow);
       cell.setValue(value);

      }
      
    
  }
   var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
   Utilities.sleep(10000);

   newRange.setValues([rowData]); 
   
   
  }
  return ContentService.createTextOutput(result);
}



function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}
