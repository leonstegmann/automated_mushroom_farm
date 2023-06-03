//include Libraries for OLED Display 
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

//  Include libraries for Humidity Sensor AM2023
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>

//include Wire Library for I2C-Bus
#include <Wire.h>

//OLED
#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

//Humidity Sensor
Adafruit_AM2320 AM2320 = Adafruit_AM2320();

void OLED_refresh();



void setup() {

  //OPEN SERIAL COMMUNICATION
  Serial.begin(9600);

  //CAN_BUS SETUP USRP, Arduino as MASTER
  Wire.begin();//(SDA 4, SCL 5) //for Slave would be zb. Wire.begin(0x8)

  // Motor 
  pinMode(3,"OUTPUT");

  //OLED_DISPLAY Setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // mit I2C-Adresse 0x3c initialisiere

  // Humidity Sensor
  AM2320.begin();

}

static int16_t fan_speed = 0;
static unsigned int fan_speed_percent = 0;
static int tempC;
static unsigned int humidity;
static int counter = 0;
static unsigned int timer = 0;

//MAIN
void loop() {
  
  tempC = round(AM2320.readTemperature());
  humidity = round(AM2320.readHumidity());

  counter-=5; 
  if (counter % 10 == 0){
    if (counter <=100) {
      counter = 255;
    }
    fan_speed = counter; 
  }
  analogWrite(3,fan_speed);
  fan_speed_percent = (unsigned int) round( (float) fan_speed * 100/255);
  
  timer++;
  if (timer % 4 == 0 ){
    Serialmyprint();
    OLED_refresh();
  }

  delay(500);
}


void OLED_refresh(){

  //Display Setup
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  //FAN Speed  
  display.setCursor(0,0); // (Spalte | Zeile)
  display.println("FAN");
  display.setCursor(33,0);
  display.println( fan_speed_percent );  
  display.setCursor(50,0);
  display.println("%");  

  //Temp
  display.setCursor(0,10);
  display.println("T");
  display.setCursor(33,10);
  display.println(tempC);
  display.setCursor(50,10);
  display.println("C");

  //Humidity
  display.setCursor(0,20);
  display.println("H");
  display.setCursor(33,20);
  display.println(humidity);
  display.setCursor(50,20);
  display.println("%");
 
 //Display aktualisieren
  display.display(); 

}

void Serialmyprint(){
  
  //Print Fan Speed
  Serial.print("Fan Speed: ");
  Serial.print(fan_speed_percent);
  Serial.println(" %");
  
  // Print the temperature and humidity in the Serial Monitor:
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.println("C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");


  Serial.println();
}