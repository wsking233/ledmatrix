#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
///////////////////////
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WifiManager.h>
#include <SPI.h>


/**
 * @author 腹黑小帅帅
 *
 *
 *
 */


//////////////////////////////////////////////////////////
#define PIN D7
#define NUM_LED 644
#define MATRIX_WIDTH 46
#define MATRIX_HEIGHT 14

// #define NUM_LED 256
// #define MATRIX_WIDTH 32
// #define MATRIX_HEIGHT 8

#define NUM_MATRIX (MATRIX_WIDTH * MATRIX_HEIGHT)

Adafruit_NeoPixel pixels(NUM_MATRIX, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, PIN, 
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);


bool TestMatrix = false;
////////////////////////////////////////////////////////

const char *ssid     = "NUC-WILLIAM-1430";
const char *password = "123456abc";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"ntp1.aliyun.com",60*60*8,60000); //从阿里云时间服务器获取时间，东八时区

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

////////////////////////////////////////////////////////

void wifiManagment(){
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
}


void timeInit(){
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay (500);
    Serial.print(".");
  }
  timeClient.begin();
  timeClient.update();
}

void setup()
{

  // matrixInit();
  timeInit();

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(60);
  matrix.print("BOOT");
  matrix.setTextColor(matrix.Color(204,0,204));
}


void printText(String text){

   	for (int x = 32; x >= -100; x--)
		{
      // timeClient.update();
			matrix.clear();
      matrix.setCursor(x, 2);
      matrix.print(text);
			matrix.setTextColor(matrix.Color(0, 255, 50));
			matrix.show();
			delay(200);
		}
}

void showCurrentTime(){
  	for (int x = 32; x >= -100; x--)
		{ 
      timeClient.update();
			matrix.clear();
      matrix.setCursor(x, 2);
      matrix.printf("%02d:%02d", timeClient.getHours(), timeClient.getMinutes());
			matrix.setTextColor(matrix.Color(0, 0, 250));
			matrix.show();
			delay(200);
		}
}

void loop()
{

  printText("Hello, World!");
  delay(200);
  showCurrentTime();
  delay(200);

}

