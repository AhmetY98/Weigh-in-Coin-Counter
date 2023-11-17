#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2, buttonPin{10};
int buttonState{1};
int counter{1};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int calib_type{0};
byte customChar[8] = {
  B00100,
  B01110,
  B10101,
  B10100,
  B10101,
  B01110,
  B00100,
  B00000
};


void setup() {
  pinMode(buttonPin, INPUT);
  lcd.createChar(0, customChar);
  lcd.begin(16, 2);
  lcd.print("Press the button");
}
// Checks for initial button press and asks for the coin type
void firstCheck(){
  while (true){
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH){
      lcd.clear();
      lcd.print("Select coin type:");
      delay(300);
      return;
    }
  }
}
//Toggles coin type and selects after a 3 second gap
void selectCoin(){
  while(true){
    lcd.setCursor(0,1);
    switch(counter){
      case 1:
        lcd.print("Penny ");
      	lcd.write((byte)0);
      	lcd.print("1    ");
        calib_type = 1;
        break;
      case 2:
        lcd.print("Nickel ");
     	lcd.write((byte)0);
      	lcd.print("5   ");
        calib_type = 5;
        break;
      case 3:
        lcd.print("Dime ");
      	lcd.write((byte)0);
      	lcd.print("10   ");
        calib_type = 10;
        break;
      case 4:
        lcd.print("Quarter ");
      	lcd.write((byte)0);
      	lcd.print("25");
        calib_type = 25;
        break;
      case 5:
        lcd.print("Loonie $1       ");
      	lcd.write((byte)0);
      	lcd.print("1");
        calib_type = 100;
        break;
      case 6:
        lcd.print("Toonie $2       ");
        calib_type = 200;
        break;
      case 7:
        lcd.print("Test finished");
        exit(0);
      default:
        lcd.print("Mistake");
        break;
    }
    while(true){
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH){
        counter = (counter % 7) + 1;
        delay(300);
        break;
      }
    }
  }
}
void loop(){
  firstCheck();
  selectCoin();
  return;
}