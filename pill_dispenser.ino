#include <Servo.h>  
#include <LiquidCrystal.h>
#include <RTClib.h>

RTC_DS3231 RTC;
Servo servo; 

const int servoPin = 6;
const int buttonPin = 9;
const int ledPin = 7;
const int buzzerPin = 8;

int angle = 0;
int angleIncrement = 45;  
int newAngle;
int reverseNewAngle; 

int buttonState;
int movementDelay = 50;
int debounceDelay = 1000;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup(){
  pinMode(buttonPin, INPUT); 
  pinMode(ledPin,OUTPUT);    
  pinMode(buzzerPin,OUTPUT); 
  digitalWrite(ledPin,LOW);  // Turn off LED light
  digitalWrite(buzzerPin,LOW);  // Turn off buzzer 
  servo.attach(servoPin);    // Attach servo object to servoPin
  servo.write(angle);        
  Serial.begin(9600);        // useful for debugging: initialize serial 

  // Initialize the amount of columns and rows in the LCD 
  lcd.begin(16, 2);
}

void loop(){  
  buttonState = digitalRead(buttonPin); 
  Serial.println(angle);                
  LCD_countdown(); 

  newAngle = angle + angleIncrement; 
  if (newAngle <= 180) {          
    while(angle < newAngle){    
      angle = angle + 1;        
      servo.write(angle);       
      Serial.println(angle);    
      delay(movementDelay);     // Add delay to slow movement down 
    }  

    flashLED(4,150);
  }
  else {
    while (angle > 0) {
      reverseNewAngle = angle - angleIncrement; 
      while(angle > reverseNewAngle){         
        angle = angle - 1;   
        servo.write(angle);   
        Serial.println(angle);
        delay(movementDelay); 
      }
      LCD_countdown(); 
      flashLED(4,150);
    }
  }
}

void flashLED(int numFlashes, int flashDelay){  
  lcd.clear();              
  lcd.setCursor(0, 0);   // Cursor set to column 0, line 0 
  lcd.print("Medication time!");  

  for (int i = 0; i < numFlashes; i++){ 
    digitalWrite(ledPin,HIGH);       
    digitalWrite(buzzerPin,HIGH);    
    delay(flashDelay);                
    digitalWrite(ledPin,LOW);         
    digitalWrite(buzzerPin,LOW);       
    delay(flashDelay);                 
  }

  // The code will not move on (will be stuck in loop) until the button is pressed. 
  while(digitalRead(buttonPin) == LOW){}; 

  delay(1000);    // Delay 1 second before clearing the screen
  lcd.clear();   
  lcd.print("Done!");
  lcd.setCursor(0,1);
  lcd.print(":)"); 
  delay(1500);    // Delay 1.5 second before clearing the screen
  lcd.clear();    
}

void LCD_countdown() {
  int seconds = 5;
  while (seconds > 0) {
     lcd.clear();              
    lcd.setCursor(0, 0);       // Set cursor to top left
    lcd.print("Take medication");  
    lcd.setCursor(0, 1);
    lcd.print("in ");
    lcd.print(seconds);
    lcd.print(" seconds");
    delay(1000);
    seconds -= 1; 
  }
}
