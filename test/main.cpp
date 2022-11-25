#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <ArduinoJson.h>
///////////////////////
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WifiManager.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>




/**
 * @author 腹黑小帅帅
 * @AUTStudentID: 18017970
 *
 */


//matrix setting 
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

//Network setting
/////////////////////////////////////////////////////////////////////////////////////


WiFiManager wifiManager;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"ntp1.aliyun.com",60*60*8,60000); //从阿里云时间服务器获取时间，东八时区
HTTPClient httpClient;
const char *SSID     = "SportPlanet-GAMEWALL";
const char *PSW = "123456abc";

Ticker ticker;

bool shouldSaveConfig = false;
char Port[5] = "7001"; // AWTRIX Host Port, default = 7001

/////////////////////////////////////////////////////////////////////////////////////
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };


void netInit(){

	wifiManager.resetSettings();
	wifiManager.setDebugOutput(true);
	// wifiManager.setHeadImgBase64(blueWifi);
	wifiManager.setMinimumSignalQuality(30);
	IPAddress _ip = IPAddress(192,168,4,25);
	IPAddress _gw = IPAddress(192,168,4,1);
	IPAddress _sn = IPAddress(255,255,255,0);
	wifiManager.setAPStaticIPConfig(_ip,_gw,_sn);

	wifiManager.setAPCallback(configModeCallback);
  // 设置点击保存的回调
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  // 设置 如果配置错误的ssid或者密码 退出配置模式
  wifiManager.setBreakAfterConfig(true);
  // 设置过滤重复的AP 默认可以不用调用 这里只是示范
  wifiManager.setRemoveDuplicateAPs(true);
  // 添加额外的参数 比如加入 mqtt 服务器地址 port 端口号  apikey 后面可以结合onenet使用
//   WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
//   WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
//   WiFiManagerParameter custom_apikey("apikey", "onenet apikey", api_key, 32);
//   wifiManager.addParameter(&custom_mqtt_server);
//   wifiManager.addParameter(&custom_mqtt_port);
//   wifiManager.addParameter(&custom_apikey);

/*** 尝试连接网络，失败去到配置页面 **/
  // ssid 命名为danpianjicainiao pwd是123456
  if (!wifiManager.autoConnect("danpianjicainiao","123456")) {
      Serial.println("failed to connect and hit timeout");
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(1000);
  }
  /*************************************/
  // 读取配置页面配置好的信息
//   strcpy(mqtt_server, custom_mqtt_server.getValue());
//   strcpy(mqtt_port, custom_mqtt_port.getValue());
//   strcpy(api_key, custom_apikey.getValue());

// 保存自定义信息
  if (shouldSaveConfig) {
      Serial.println("saving config");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
    //   json["mqtt_server"] = mqtt_server;
    //   json["mqtt_port"] = mqtt_port;
    //   json["api_key"] = api_key;
      json.printTo(Serial);
      //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}


void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}
/**
 * 功能描述：设置点击保存的回调
 */
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

/**
 * 功能描述：设置LED灯闪烁，提示用户进入配置模式
 */
void tick(){
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
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

