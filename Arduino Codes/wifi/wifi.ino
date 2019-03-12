#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);  
  WiFi.begin("Myown", "12345678"); 
  //WiFi.begin("Snowden", "12345678"); 
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);
    Serial.println("Waiting for connection");
  }  
}


void loop() {     
 sendRequest("killme");
//  if (Serial.available()>0){
//        String response="";
//    while (Serial.available()){
//      String input = Serial.readString();
//      response+=input;
//      }
//      sendRequest(response);
//    }

}

void sendRequest(String i){
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   HTTPClient http;    //Declare object of class HTTPClient
  // http.begin("http://knusts.herokuapp.com:80/dump");      //Specify request destination
   http.begin("http://192.168.43.215:8000/dump");      //Specify request destination
   //http.begin("http://192.168.43.250:8000/dump");      //Specify request destination   
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header
   int httpCode=http.POST("title="+String(i));
   String payload = http.getString();                  //Get the response payload
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
   http.end();  //Close connection
 }else{
    Serial.println("Error in WiFi connection");   
 }
  delay(3000);  //Send a request every 30 seconds
  }
