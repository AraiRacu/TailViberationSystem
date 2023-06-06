#include <ArduinoOSCWiFi.h>
#include <M5Core2.h>
#include <AXP192.h>

// arp -aでIPアドレスをすべて取得
// ipconfigで自身のIPアドレスを取得

const char* ssid = "SSID"; //WiFIのSSIDを入力
const char* pass = "Pass"; // WiFiのパスワードを入力

const IPAddress ip(192, 168, XX, XX); //自身のIPアドレス
const IPAddress gateway(192, 168, XX, XX); //デフォルトゲートウェイ
const IPAddress subnet(255, 255, XX, XX); //サブネットマスク

const char *pc_addr = "192.168.XX.XX";  //送信元のPCのIPアドレス
const int pc_port = XXXX; //受信先のポート

const int vibeStrength = 10000 //バイブレーションの強さ

int i; float f; String s;
bool isVibe = false;

void setup(){
  M5.begin(); //Init M5Core.
  //M5.Power.begin();
  M5.IMU.Init();  //Init IMU sensor.
  M5.Lcd.fillScreen(BLACK); //Set the screen background color to black.
  M5.Lcd.setTextColor(GREEN , BLACK); //Sets the foreground color and background color of the displayed text.
  M5.Lcd.setTextSize(2);  //Set the font size.
  M5.Axp.SetLDOVoltage(3, vibeStrength);

  //Wifi Setup
  #ifdef ESP_PLATFORM
    WiFi.disconnect(true, true);  // disable wifi, erase ap info
    delay(1000);
    WiFi.mode(WIFI_STA);
  #endif
  WiFi.begin(ssid, pass);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf(".");
    delay(500);
  }
  M5.Lcd.setCursor(20, 10);
  M5.Lcd.printf("?");
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.println(WiFi.localIP());

  OscWiFi.subscribe(pc_port, "/filter", onOscReceived);
}

void loop() {
  OscWiFi.parse();
  //if(isVibe) vibration()
  delay(10);  // Delay 10ms.
}

void vibration() {
  M5.Axp.SetLDOEnable(3, true);  // 3番をtrueにしてバイブレーション開始
  delay(10);                   // バイブレーションの長さ（ms）はお好みで調整
  M5.Axp.SetLDOEnable(3, false); // 3番をfalseにしてバイブレーション修了
}

void onOscReceived(const OscMessage& m){
  M5.Lcd.setCursor(0, 90);
  M5.Lcd.printf("Tail_IsGrabbed %d",m.arg<int>(0));

  if(m.arg<int>(0) == 1){
    M5.Axp.SetLDOEnable(3, true);
  }else{
    M5.Axp.SetLDOEnable(3, false);
  }
}
