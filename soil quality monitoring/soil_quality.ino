#include <ThingSpeak.h> 
#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>  // http web access library
#include <ArduinoJson.h>        // JSON decoding library
// Libraries for SSD1306 OLED display
#include <Wire.h>              // include wire library (for I2C devices such as the SSD1306 display)
#include <Adafruit_GFX.h>      // include Adafruit graphics library
#include <Adafruit_SSD1306.h>  // include Adafruit SSD1306 OLED display driver
 
//#define OLED_RESET   5     // define SSD1306 OLED reset at ESP8266 GPIO5 (NodeMCU D1)
//Adafruit_SSD1306 display(OLED_RESET);


unsigned long myChannelNumber = 766770;  
const char * myWriteAPIKey = "W5DLAMO1SQQ4JBJK";  



 
// set Wi-Fi SSID and password
const char *ssid     = "vivo 1606";
const char *password = "amanchoudhary";
 
// set location and API key
String Location = " Gandhinagar, IN";
String API_Key  = "f45f5d522f8b588f3d98cf75809cf9d9";



// declaring pins of color sensor

const int S0 = D4;  
const int S1 = D5;  
const int S2 = D6;  
const int S3 = D7;  
const int sensorOut = D8; 

int redFrequency = 0;  
int greenFrequency = 0;  
int blueFrequency = 0;  

int frequency = 0;


WiFiClient client;
 
void setup(void)
{
  Serial.begin(9600);
  delay(1000);
 
 // Wire.begin(4, 0);           // set I2C pins [SDA = GPIO4 (D2), SCL = GPIO0 (D3)], default clock is 100kHz
 
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
 
  //Wire.setClock(400000L);   // set I2C clock to 400kHz
 
 // display.clearDisplay();  // clear the display buffer
  //display.setTextColor(WHITE, BLACK);
  //display.setTextSize(1);
  //display.setCursor(0, 0);
  //display.println("  Internet Weather");
  //display.print("  Station - Jaipur");
  //display.display();
 
  WiFi.begin(ssid, password);
 
  Serial.print("Connecting.");
  //display.setCursor(0, 24);
  //display.println("Connecting...");
  //display.display();
  while ( WiFi.status() != WL_CONNECTED )
  {
    delay(500);
    Serial.print(".");
  }
   ThingSpeak.begin(client);
  Serial.println("connected");
  //display.print("connected");
  //display.display();
  delay(1000);

  
  


  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);





 
}
 
void loop()
{
  if (WiFi.status() == WL_CONNECTED)  //Check WiFi connection status
  {
    HTTPClient http;  //Declare an object of class HTTPClient
 
    // specify request destination
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=" + API_Key);  // !!
 
    int httpCode = http.GET();  // send the request
 
    if (httpCode > 0)  // check the returning code
    {
      String payload = http.getString();   //Get the request response payload
 
      DynamicJsonBuffer jsonBuffer(512);
 
      // Parse JSON object
      JsonObject& root = jsonBuffer.parseObject(payload);
      if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        return;
      }
 
      float temp = (float)(root["main"]["temp"]) - 273.15;        // get temperature in °C
      int   humidity = root["main"]["humidity"];                  // get humidity in %
      float pressure = (float)(root["main"]["pressure"]) / 1000;  // get pressure in bar
      float wind_speed = root["wind"]["speed"];                   // get wind speed in m/s
      int  wind_degree = root["wind"]["deg"];                     // get wind degree in °
 
      // print data
      Serial.printf("Temperature = %.2f°C\r\n", temp);
       // Firebase.setFloat("Temp",temp);
        //delay(200);
 
      Serial.printf("Humidity    = %d %%\r\n", humidity);
      Serial.printf("Pressure    = %.3f bar\r\n", pressure);
      Serial.printf("Wind speed  = %.1f m/s\r\n", wind_speed);
      Serial.printf("Wind degree = %d°\r\n\r\n", wind_degree);

 ThingSpeak.writeField(myChannelNumber,1,temp,myWriteAPIKey);
 ThingSpeak.writeField(myChannelNumber,6,humidity,myWriteAPIKey);
      
 
    //  display.setCursor(0, 24);
      //display.printf("Temperature: %5.2f C\r\n", temp);
      //display.printf("Humidity   : %d %%\r\n", humidity);
      //display.printf("Pressure   : %.3fbar\r\n", pressure);
      //display.printf("Wind speed : %.1f m/s\r\n", wind_speed);
      //display.printf("Wind degree: %d", wind_degree);
      //display.drawRect(109, 24, 3, 3, WHITE);     // put degree symbol ( ° )
      //display.drawRect(97, 56, 3, 3, WHITE);
      //display.display();
      
    }
    

 
    http.end();   //Close connection
 
  }
 
 
  delay(100);   // wait 1 minute


// Setting upp code for Color sensor
// Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
   // redFrequency = map(redFrequency, 138,145,255,0);
   // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redFrequency);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.println(blueFrequency);
  delay(100);


float ph;
float nitrogen;
float phosphorus;
float potassium;

if(redFrequency >= 679 && redFrequency <= 686 && greenFrequency >= 844 && greenFrequency <= 849 && blueFrequency >= 703 && blueFrequency <= 712)
{
  ph=5.5;
  Serial.print("ph is slightly acidic = 5.5");
 
}

else if(redFrequency >= 800 && redFrequency <= 850 && greenFrequency >=1000 && greenFrequency <= 1080 && blueFrequency >= 700 && blueFrequency <= 900   )  
{
  nitrogen = 30;
  Serial.print("Nitrogen content : 30kg/acre");

}

else if(redFrequency >= 580 && redFrequency <= 600 && greenFrequency >=665 && greenFrequency <= 695 && blueFrequency >= 570 && blueFrequency <= 590   )  
{
  potassium=50;
  Serial.print("potassium content : 50kg/acre");
 
}


else if(redFrequency >= 865 && redFrequency <= 895 && greenFrequency >=900 && greenFrequency <= 930 && blueFrequency >= 685 && blueFrequency <= 710   )  
{
  phosphorus=30;
  Serial.print("phosphorus content : 30kg/acre");
 
}

else if(redFrequency >= 750 && redFrequency <= 785 && greenFrequency >=910 && greenFrequency <= 940 && blueFrequency >= 700 && blueFrequency <= 725   )  
{
  nitrogen=20;
  Serial.print("nitrogen content : 20kg/acre");
 
}
else if(redFrequency >= 830 && redFrequency <= 860 && greenFrequency >=865 && greenFrequency <= 900 && blueFrequency >= 660 && blueFrequency <= 690   )  
{
  phosphorus=15;
  Serial.print("phosphorus content : 15kg/acre");
 
}
else if(redFrequency >= 1000 && redFrequency <= 1050 && greenFrequency >=1030 && greenFrequency <= 1117 && blueFrequency >= 700 && blueFrequency <= 840  )  
{
  phosphorus=25;
  Serial.print("phosphorus content : 25kg/acre");
 
}
  else
  {
   /* nitrogen=0;
    phosphorus=0;
    potassium=0;
    ph=5;*/
    //Serial.println("not any color");
    }

   ThingSpeak.writeField(myChannelNumber,2,nitrogen,myWriteAPIKey);
   ThingSpeak.writeField(myChannelNumber,3,phosphorus,myWriteAPIKey);
   ThingSpeak.writeField(myChannelNumber,4,potassium,myWriteAPIKey);
   ThingSpeak.writeField(myChannelNumber,5,ph,myWriteAPIKey);
  

  

 
}
// End of code.
