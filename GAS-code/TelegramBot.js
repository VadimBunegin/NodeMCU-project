function doPost(e) {
  var update = JSON.parse(e.postData.contents);
  
  if (update.hasOwnProperty('message')) {
    var msg = update.message;
    var chatId = msg.chat.id;

    if (msg.hasOwnProperty('entities') && msg.entities[0].type == 'bot_command') {
      
      if (msg.text == '/lastpost') {
        
        var sheet =  SpreadsheetApp.openById('1_X5-g5yJAPMT6iduGbY5v-jFLKKUVpjZrYqUwnkgruQ').getSheets()[0]
        
        var message = ''
        for(var i = 0; i <= 10; i++){
          message += '\n_________________________________________\n' + ' <strong>' + i + '</strong> | ' + sheet.getRange(sheet.getLastRow()-(10-i), 1, 1,  3).getValues()[0][0] + ' | <strong>' + sheet.getRange(sheet.getLastRow()-(10-i), 1, 1,  3).getValues()[0][1] + '</strong>';
        }
        
        var payload = {
          'method': 'sendMessage',
          'chat_id': String(chatId),
          'text': message,
          'parse_mode': 'HTML'
        }     
        var data = {
          "method": "post",
          "payload": payload
        }
        
        // и отправляем его боту (замените API на свой)
        var API_TOKEN = '6141234459:AAHWnUUv2TJpUV8Bgu-2UOXAePSVc62XbsI'
        UrlFetchApp.fetch('https://api.telegram.org/bot' + API_TOKEN + '/', data);
      }
    }
  }
}
