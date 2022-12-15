// Code Create By Refael Ivgi 11.11.22

//Connections:
//joystick -> 3.3v , URy = A1
//Servo -> red = 5v, yellow = D7 , brown = GND
// Ultrasonic -> 5v , TRIGGER = D10 , ECHO = D11
//Reco Voice -> 5v , Tx Reco -> Rx arduino , Rx Reco -> Tx arduino


#include <Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN  10
#define ECHO_PIN     11
#define MAX_DISTANCE 20

Servo myservo;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//ULTRASONIC's variables
int distance;

//JOYSTICK's variables
int VRy = A1;
int mapY;
int yPosition = 0;

//SERVO's variables
int angle;

//RecoVoice's variable
byte com = 0;
int cnt=0;
int Reset = 4;

void setup() {
  
  myservo.attach(7);
  Serial.write(0xAA);
  Serial.write(0x37);
  delay(500);
  Serial.write(0xAA);
  Serial.write(0x21);
  digitalWrite(Reset, HIGH);
  delay(200); 
  pinMode(Reset, OUTPUT);
  Serial.begin(9600);
}

void loop() {

 //while (Serial.available()) {

    com = Serial.read();

    switch (com) {



      case 0x12:  //command for change to Manual Mode

        manualMode();
        Serial.print("Change to Manual Mode");


        break;

      case 0x13:  //command for change to Automatic Mode
        autoMode();


        break;

      case 0x14: // command for change to RESET MODE
              
        break;

    }
 // }





}


void manualMode() {
  Serial.print(com);

  while (com == 18) {
   
    yPosition = analogRead(VRy);
    mapY = map(yPosition, 0, 1023, 0, 180);

    //if you want to print the location of the y axis joystick .
    //Serial.print(mapY);



    //if you want to print the measured distance by cm .
    //Serial.print(sonar.ping_cm());

    Serial.print(mapY);
    if (mapY == 0 ) {
      for (angle = 0; angle <= 90; angle++)
      {
        com = Serial.read();
        yPosition = analogRead(VRy);
        mapY = map(yPosition, 0, 1023, 0, 180);
        
        distance = sonar.ping_cm();
        if (distance < 10 && distance != 0 )
        {
          Serial.print(distance);
          delay(50);
          Serial.print("CAMERA!");


        }
        if (mapY != 0 )
          break;

        myservo.write(angle);
        delay(80);

      }
    }
    if (mapY == 118) {

      for (angle = 0; angle <= 90; angle++)
      {
        com = Serial.read();
        yPosition = analogRead(VRy);
        mapY = map(yPosition, 0, 1023, 0, 180);
        distance = sonar.ping_cm();

        if (distance < 10 && distance != 0)
        {
          delay(50);
          Serial.print("CAMERA!");
        }

        if (mapY != 118)
          break;
        myservo.write(angle);
        delay(80);

      }
    }

    myservo.write(0);

  } 

  if (com == 0x13 ) {
    autoMode();
  }
}




void autoMode() {

  do {
    
    for (angle = 0; angle <= 90; angle++)
    {
      distance = sonar.ping_cm();
      com = Serial.read();
      if (com == 19 || com==20) {
        Serial.print(com);
        Serial.print("Change to Manual Mode");
        digitalWrite(Reset, LOW);
      }
      if (distance < 10 && distance != 0)
      {
        Serial.print("CAMERA!");
        Serial.print(com);
      }
      myservo.write(angle);
      delay(80);

    }
   

    // now scan back from 180 to 0 degrees
    for (angle = 90; angle > 0; angle--)
    {
      distance = sonar.ping_cm();
      com = Serial.read();
      if (com == 19 || com==20) {
        Serial.print(com);
        Serial.print("Change to Manual Mode");
        digitalWrite(Reset, LOW);
        
        
      }
      if (distance < 10 && distance != 0)
      {
        Serial.print("CAMERA!");
        Serial.print(com);
      }
      myservo.write(angle);
      delay(80);

    }
    
    myservo.write(0);

  } while (com == 255);

  if (com == 0x12) {
    Serial.print("Change to Manual Mode");
    manualMode ();
  }
}
