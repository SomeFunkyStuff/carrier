#include <SPI.h>
#include <SD.h>
#include <Keyboard.h>

int DEBUG = 0;
int SD_PIN = 10;

void setup() {
  Keyboard.begin();
  
  if (DEBUG==1){
  initDebug();
  }

 
File file = loadFile("script.txt");
 if (!file) {
  fail();
  return;
 }
  while ( file.available()) {
    String line = file.readStringUntil('\n');
    int length = line.length() + 1; 
    char buffer[length];
    line.toCharArray(buffer, length);
    String cmd = line.substring(0,4);
    debug(buffer);  
    if (cmd=="PUSH"){
      handlePush(line.substring(5));
    }else
    if (cmd=="TYPE"){
      handleType(line.substring(5));
    }
     if (cmd=="WAIT"){
      handleWait(line.substring(5));
    }

      
    else{
      debug("Unknown command "+cmd);
    }
    
    
  }
  file.close();
  Keyboard.end();
  success();
}

void handlePush(String msg){
  push( msg);
  delay(100);
  Keyboard.releaseAll();
}

void handleWait(String msg){
  delay(msg.toInt());
 
}

bool isDigit(char c){
  return c>='0' && c<= '9';
}

void push(String s){
  char tmp[s.length()] ;
  s.toCharArray(tmp,s.length());
  bool isNum = false;
  int buf=0;
  for(int i=0;i<s.length();i++){
    char c = tmp[i];
    bool digit = isDigit(c);
    if(isNum && !digit){
      isNum = false;
      Keyboard.press(buf);
    } else
    if(!isNum && digit){
      buf = 0;
      isNum = true;
   } 
  
   if(isNum && digit){
      buf=buf*10+(c-'0');
    }
  }
}


void handleType(String msg){
debug("Type "+msg);
Keyboard.println(msg);

}
 void success(){
  //FAIL
 }

 void fail(){
  //FAIL
 }
  
void initDebug(){
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
 }

 
File loadFile(char filename[]){
  File file;
  
  if (!SD.begin(SD_PIN)) {
    debug("initialization failed!");
    return file;
  }
  
  debug("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
 return  SD.open(filename);
}

 void debug(char message[]){
    if (DEBUG == 1){
    Serial.println(message);
     }
  }
 
 
 void debug(String message){
    int length = message.length() + 1; 
    char buffer[length];
    message.toCharArray(buffer, length);
    debug(buffer);
  }

void loop() {
  // nothing happens after setup
}


