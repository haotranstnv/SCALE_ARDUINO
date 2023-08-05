#include <HX711.h>
#include <LiquidCrystal_I2C.h>
#define phaseA 2 //CLK RATORY ENCODER
#define phaseB 3 // DT RATORY ENCODER
// A4 --> SDA
// A5 --> SCL
#define led_red 6
#define button 7 // SW RATORY ENCODER
#define DOUT 12 // DOUT HX711
#define CLK 13 // CLK HX711
HX711 scale(DOUT, CLK);
float weight;
float calibration_factor = -109810; //HỆ SỐ CALIB, CẦN TÌM HỆ SỐ NÀY
LiquidCrystal_I2C lcd(0x27, 16, 2);
int count = 100, present, last, val_b;
void setup() 
{ 
   Serial.begin (9600);
   pinMode(phaseA,INPUT);
   pinMode(phaseB,INPUT);
   pinMode(button,INPUT);
   pinMode(led_red,OUTPUT);
   digitalWrite(led_red, LOW);
   last = digitalRead(phaseA);
   scale.set_scale();
   long zero_factor = scale.read_average();
   scale.tare();
   lcd.init();
   lcd.backlight();
   lcd.begin(16,2);
   lcd.setCursor(0,0);
   lcd.print("BTL DTUD");
   delay(3000);
   lcd.clear();
} 
void loop() 
{ 
   scale.set_scale(calibration_factor);
   val_b = digitalRead(button);
   while(val_b == LOW) 
   {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("INIT LIMIT: ");
      present = digitalRead(phaseA);  
      val_b = digitalRead(button);
      if (val_b = LOW) val_b = HIGH;
      if (present != last)
        {     
          if (digitalRead(phaseB) != present) 
            { 
              count ++;
	            if (count>100) count = 0;
            } 
          else 
            {
              count --;
	            if (count<0) count = 100;
            }
        } 
      last = present;
      lcd.setCursor(0, 1);
      lcd.print(count);   
   }
   weight = scale.get_units(5);
   if (weight>=count) digitalWrite(led_red, HIGH);
   else digitalWrite(led_red, LOW);
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Weight: ");
   lcd.setCursor(0, 1);
   lcd.print(weight);
   lcd.print(" KG");
}
