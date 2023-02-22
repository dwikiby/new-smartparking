#include <Servo.h> //includes the servo library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

//#include <SoftwareSerial.h> // setting pengiriman ke ESP
//SoftwareSerial Slot(2, 3);// RX, TX

byte terisi[] = { //simbol terisi
  0b00000,
  0b00001,
  0b00010,
  0b10100,
  0b01000,
  0b00000,
  0b00000,
  0b00000
};

byte kosong[] = {
  0b00000,
  0b10001,
  0b01010,
  0b00100,
  0b01010,
  0b10001,
  0b00000,
  0b00000
};

Servo myservo;


#define ir_enter 4
#define ir_back 5

//int S1=0, S2=0;
int flag1=0, flag2=0;  
int slot = 4; 

void setup(){
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(115200);
//  Slot.begin(115200); //pengiriman slot

//  lcd.createChar(0, terisi);
//  lcd.createChar(1, kosong);

pinMode(ir_car1, INPUT);
pinMode(ir_car2, INPUT);
//pinMode(ir_car3, INPUT);
//pinMode(ir_car4, INPUT);
//pinMode(ir_car5, INPUT);
//pinMode(ir_car6, INPUT);

//pinMode(ir_enter, INPUT);
//pinMode(ir_back, INPUT);
//  
//myservo.attach(6);
//myservo.write(90);

lcd.begin(16, 2);  
lcd.backlight();
lcd.setCursor (0,0);
lcd.print("     ARDUINO    ");
lcd.setCursor (0,1);
lcd.print(" PARKING SYSTEM ");
delay (2000);
lcd.clear();

//Read_Sensor();

}

void loop(){

//Read_Sensor();

lcd.setCursor (0,0);
lcd.print("   Have Slot: "); 
lcd.print(slot);
Serial.println(slot);
//Slot.println(slot);
lcd.print("    ");  

lcd.setCursor (0,1); // informasi slot parkir 1
if(S1==1 ){
  lcd.print("P1:");
  lcd.setCursor(3,1);
  lcd.write(byte(0));
  slot = 3;}

     else{
      lcd.setCursor (0,1);
      lcd.print("P1:");
      lcd.setCursor(3,1);
      lcd.write(byte(1));
      slot = 4;}

lcd.setCursor (5,1); // informasi slot parkir 2 
if(S2==1){
  lcd.print("P2:");
  lcd.setCursor(8,1);
  lcd.write(byte(0));
  slot = 3;}
  
     else{lcd.print("P2:");
     lcd.setCursor(8,1);
     lcd.write(byte(1));
     slot = 4;}

lcd.setCursor (10,1); // informasi slot parkir 3
if(S3==1){
  lcd.print("P3:");
  lcd.setCursor(12,1);
  lcd.write(byte(0));
  slot = 3;}
  
     else{lcd.print("P3:");
     lcd.setCursor(12,1);
     lcd.write(byte(1));
     slot = 4;}

lcd.setCursor (14,1); // informasi slot parkir 4
if(S4==1){
  lcd.print("P4:");
  lcd.setCursor(16,1);
  lcd.write(byte(0));
  slot = 3;}
  
     else{lcd.print("P4:");
     lcd.setCursor(16,1);
     lcd.write(byte(1));
     slot = 4;}

//lcd.setCursor (0,2);
//if(S3==1){lcd.print("S3:Fill ");}
//     else{lcd.print("S3:Empty");}
//
//lcd.setCursor (10,2);
//if(S4==1){lcd.print("S4:Fill ");}
//     else{lcd.print("S4:Empty");}
//
// lcd.setCursor (0,3);
//if(S5==1){lcd.print("S5:Fill ");}
//     else{lcd.print("S5:Empty");}
//
//lcd.setCursor (10,3);
//if(S6==1){lcd.print("S6:Fill ");}
//     else{lcd.print("S6:Empty");}    

if(digitalRead (ir_enter) == 0 && flag1==0){
if(slot>0){flag1=1;
if(flag2==0){myservo.write(180); slot = slot-1;}
}else{
lcd.setCursor (0,0);
lcd.print(" Sorry Parking Full ");  
delay(1500);
}   
}

if(digitalRead (ir_back) == 0 && flag2==0){flag2=1;
if(flag1==0){myservo.write(180); slot = slot+1;}
}

if(flag1==1 && flag2==1){
delay (1000);
myservo.write(90);
flag1=0, flag2=0;
}

delay(500);
}
