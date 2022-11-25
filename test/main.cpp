#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
///////////////////////
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WifiManager.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>




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

/////////////////////////////////////////////////////////////////////////////////////

const char *SSID     = "SportPlanet-GAMEWALL";
const char *PSW = "123456abc";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"ntp1.aliyun.com",60*60*8,60000); //从阿里云时间服务器获取时间，东八时区

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

/////////////////////////////////////////////////////////////////////////////////////

WiFiManager wifiManager;

void wifiManagment(){
  Serial.begin(9600);
  wifiManager.autoConnect(SSID, PSW);

}

// 发送HTTP请求并且将服务器响应通过串口输出
void httpClientRequest(String URL){
 
  //重点1 创建 HTTPClient 对象
  HTTPClient httpClient;
 
  //重点2 通过begin函数配置请求地址。此处也可以不使用端口号和PATH而单纯的
  httpClient.begin(URL); 
  Serial.print("URL: "); Serial.println(URL);
 
  //重点3 通过GET函数启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);
  
  //重点4. 如果服务器响应HTTP_CODE_OK(200)则从服务器获取响应体信息并通过串口输出
  //如果服务器不响应HTTP_CODE_OK(200)则将服务器响应状态码通过串口输出
  if (httpCode == HTTP_CODE_OK) {
    // 使用getString函数获取服务器响应体内容
    String responsePayload = httpClient.getString();
    Serial.println("Server Response Payload: ");
    Serial.println(responsePayload);
  } else {
    Serial.println("Server Respose Code：");
    Serial.println(httpCode);
  }
 
  //重点5. 关闭ESP8266与服务器连接
  httpClient.end();
}

void timeInit(){
  Serial.begin(115200);
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

