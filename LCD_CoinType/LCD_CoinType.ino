
#include <LiquidCrystal.h>
#include "HX711.h"

#define DOUT  9
#define CLK  8
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2, button1Pin{10}, button2Pin{6};
int button1State{1}, button2State{1};
int counter{1};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
HX711 scale;
float calibration_factor = 13130;

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
  pinMode(button1Pin, INPUT);
  pinMode (button2Pin, INPUT);
  lcd.createChar(0, customChar);
  lcd.begin(16, 2);
  lcd.print("Press the button");

  //calibrating the scale
  Serial.begin(9600);
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  scale.tare(); //Reset the scale to 0
  

  while (true){
      button1State = digitalRead(button1Pin);
      if (button1State == HIGH){
        lcd.clear();
        return;

      }
    }
}
void loop(){
  firstCheck();
  int coin_type{selectCoin()};
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(calculations(coin_type, scaleWeight()));
  check_reloop();
	
 
 
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }

}
// Checks for initial button press and asks for the coin type
void firstCheck(){
  lcd.clear();
  lcd.print("Select coin type:");
  delay(300);
  return;
  }
//Toggles coin type and selects after a 3 second gap
int selectCoin(){
  while(true){
    lcd.setCursor(0,1);
    switch(counter){
      case 1:
        lcd.print("Nickel ");
     	  lcd.write((byte)0);
      	lcd.print("5   ");
        calib_type = 5;
        break;
      case 2:
        lcd.print("Dime ");
      	lcd.write((byte)0);
      	lcd.print("10   ");
        calib_type = 10;
        break;
      case 3:
        lcd.print("Quarter ");
      	lcd.write((byte)0);
      	lcd.print("25");
        calib_type = 25;
        break;
      case 4:
        lcd.print("Loonie $1       ");
      	lcd.write((byte)0);
      	lcd.print("1");
        calib_type = 100;
        break;
      case 5:
        lcd.print("Toonie $2       ");
        calib_type = 200;
        break;
        case 6:
        //Scale mode is calib_type set to 500
          lcd.print("Scale Mode   ");
          calib_type = 500;
          break;
      default:
        lcd.print("Mistake");
        break;
    }
    while(true){
      button1State = digitalRead(button1Pin);
      button2State = digitalRead(button2Pin);
      if (button1State == HIGH){
        counter = (counter % 6) + 1;
        delay(300);
        break;
      }
      if (button2State == HIGH){
        delay(300);
		return(calib_type);
      }
    }
  }
}
double scaleWeight(){
  //measures the total weight on the scale
  //Includes initial calibration and subtracts
  //the weight of the container
  //returns the weight
  scale.set_scale(calibration_factor);
  double WEIGHT=(scale.get_units());
  Serial.print(WEIGHT);
  return(WEIGHT);
  //above line is a value so the code runs
}
int calculations(int coin_type, double weight){
  //divides by the weight of each coin 
  double coin_weight{0};
  switch (coin_type){
    
    case 1:
    coin_weight = 2.35;
    break;
    
    case 5:
    coin_weight = 3.95;
    break;
    
    case 10:
    coin_weight = 1.75;
    break;
    
    case 25:
    coin_weight = 4.40;
    break;

    case 100:
    	coin_weight = 6.27;
    	break;
      
  case 200:
    coin_weight = 6.92;
    break;
  // case 500:
  //   return scaleMode();
  }
  	lcd.clear();
  	return (int(round(weight / coin_weight)));
}
void check_reloop(){
lcd.setCursor(0,1);
lcd.print("Press B1 to redo");
while(true){
  button1State = digitalRead(button1Pin);
  if (button1State == HIGH){
    delay(300);
    return;
    
  }
  }
}

// double scaleMode(){
//   //Insert code for scale mode here

//   scale.set_scale(calibration_factor); //Adjust to this calibration factor
//   Serial.print("Reading: ");
//   Serial.print(scale.get_units(10),2);
//   lcd.print("Not Ready yet   ");
//   return (500);
//}

