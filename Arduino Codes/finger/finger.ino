// import files 
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
String color;
LiquidCrystal_I2C lcd(0X3f,2 ,1 ,0 ,4 , 5, 6, 7, 3, POSITIVE);

// initialize arduino serial
SoftwareSerial espSerial(6,7);

// initialize fingerprint serial
SoftwareSerial mySerial(12,13);

// initialize ffingerprin
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// initialize interrupt pin
const int interruptPin = 2;

//initialize button state
volatile int  buttonState = LOW;

// initializer fingerprint id
uint8_t id;

//String state;
void setup(){
  
  //state="nothing";
  //  pinMode(8,INPUT_PULLUP);
  //  pinMode(10,INPUT_PULLUP);

  pinMode(interruptPin, INPUT_PULLUP);
  
  //lcd.begin(16,2);
  //lcd.setBacklight(HIGH);
  // set baud rate for serial monitor
  Serial.begin(9600);

  // set baud rate for arduino
  
  espSerial.begin(115200);

  // Make sure serial is available
  while (!Serial);  

  // Wait a moment
  delay(100);
  Serial.println("\n\nClassroom Attendance System");
  lcd.clear();
  lcd.print("Attendance Ready");

  
  finger.begin(57600); 
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    lcd.clear();
    lcd.print("Found Sensor");
    
  } else {
      Serial.println("Did not find fingerprint sensor :(");
      lcd.clear();
      lcd.print("Sensor Error");
      while (1) { delay(1); }
      }
      
      finger.getTemplateCount();
      id=finger.templateCount;
      attachInterrupt(digitalPinToInterrupt(interruptPin), pin_ISR,RISING);
 }

void loop(){
//if (digitalRead(10)=="LOW"){
//  state="enroll";
//  }
// if (digitalRead(8)=="LOW"){
//  state="verify";
//  }
//  if (state=="verify"){
//    verify();
//    }
//  if (state=="enroll"){
//    enroll();
//    }


//if (digitalRead(10)==  LOW){
//        Serial.println("UP PRESSED");
//        state="enroll";
//        delay(1000);
//      }
//    if (digitalRead(8)==  LOW){
//        Serial.println("DOWN PRESSED");
//        state="verify";
//        delay(1000);
//      }
//     if (state=="enroll"){
//      Serial.println("UP HAS BEEN PRESSED");
//      enroll();
//      }
//      if (state=="verify"){
//        Serial.println("DOWN HAS BEEN PRESSED");
//        verify();
//        }

if(!buttonState){
  enroll();
}else {
  verify();
    }
}

void pin_ISR(){
  buttonState = !buttonState;
}


// declare function to enroll and return fingerprint id 
uint8_t enroll() {
  id++;
  Serial.println(id);
  Serial.println("Ready to enroll a fingerprint!");  
  lcd.clear();
  lcd.print("Ready to Enroll");
  if (id == 0) {
  // ID #0 not allowed, try again!
     return 0;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);

  int p = -1;
    //*******************************************
  int currentState = buttonState;
    //*******************************************

  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
    lcd.clear();
    lcd.print("Waiting for valid finger");
  
  while (p!=FINGERPRINT_OK){
     p = finger.getImage();
     if (p==FINGERPRINT_OK){
      Serial.println("Image Taken");
      }
      //*******************************************
    if(currentState != buttonState)
      break;
    //*******************************************
    }
        if(currentState != buttonState)
      return;
    //*******************************************
    

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
   lcd.clear();
   lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); 
//  lcd.clear();
//  lcd.print("ID"+String(id));
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
//  lcd.clear();
//  lcd.print("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.print("Image Taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  
  Serial.println(id);
//  lcd.clear();
//  lcd.print("Creating model for #"+String(id));

  
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
      //id++;
    lcd.clear();
    lcd.print("Prints matched");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
      lcd.clear();
//    lcd.print("Fingerprint did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); 
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
//    lcd.clear();
//    lcd.print("Stored");
    //    giveToEsp(id);
    // *****************************GIVEN TO ESP HERE***************************************
    espSerial.print(id);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}



//declare function to verify and return fingerprint id
uint8_t verifyy() {
  Serial.println("Place finger");
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); 
  Serial.println(finger.confidence);
  if (finger.fingerID!=-1){
    espSerial.print(finger.fingerID);
    Serial.print("givin to esp function");
  }  
}

int verifyyy() {
  Serial.println("Waiting for valid finger...");
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}




uint8_t verify() {
  int p=-1;
   //******************************************
   int currentState = buttonState;
   //******************************************
  Serial.println("Waiting for finger");
  while (p!=FINGERPRINT_OK){
     p = finger.getImage();
     if (p==FINGERPRINT_OK){
      Serial.println("Image Taken");
      }
          //******************************************
    if(currentState != buttonState)
      break;
    //******************************************
    }
    //******************************************
      if(currentState != buttonState)
      return;
    //******************************************

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }     
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); 
  Serial.println(finger.confidence); 
  // *****************************GIVEN TO ESP HERE***************************************
       espSerial.print(finger.fingerID);
       return finger.fingerID;
       }



