#include <WiFi.h>
#include <Wire.h>
#include "PubSubClient.h"
#include "Seeed_BME280.h"
#include"melody.h"
const int pin = 26;
const int ch = 1;
//音階の指定→　melodyAry[]
int melodyAry[] =   {DO_N,RE_N,MI_N,FA_N,SO_N,RA_N,SI_N,DO_U};    
//音の長さの指定→　melotimeAry[]  
int melotimeAry[] = {300,300,300,300,300,300,300,300,300};
BME280 bme280;
// BMX055　加速度センサのI2Cアドレス  
#define Addr_Accl 0x19  // (JP1,JP2,JP3 = Openの時)
// BMX055　ジャイロセンサのI2Cアドレス
#define Addr_Gyro 0x69  // (JP1,JP2,JP3 = Openの時)
// BMX055　磁気センサのI2Cアドレス
#define Addr_Mag 0x13   // (JP1,JP2,JP3 = Openの時)
#define HR_PIN 34  // 心拍センサーの信号線をGPIO34につなぐ
//タクトスイッチを接続するピン
#define switch_Pin 33
#define cds_pin 32

const char* ssid = "ssid";
const char* password = "password";
const char* mqttServer = "hoge.cloudmqtt.com";
const char* mqttDeviceId = "esp32-dev";
const char* mqttUser = "mqttuser";
const char* mqttPassword = "mqttpassword";
const char* mqttTopic = "MENTAL";
const int mqtt_port = portno;

// センサーの値を保存するグローバル関数
float xAccl = 0.00;
float yAccl = 0.00;
float zAccl = 0.00;
float xGyro = 0.00;
float yGyro = 0.00;
float zGyro = 0.00;
int   xMag  = 0;
int   yMag  = 0;
int   zMag  = 0;
int MENTAL =0;
int HERAT  =0;
int LIGHT = 0;
float pressure=0.0;
float Temp = 0.0;
float Humid = 0.0;
WiFiClient espClient;
PubSubClient client(espClient);
void setup()
{
  Serial.begin(115200);
  // Wire(Arduino-I2C)の初期化
  Wire.begin(); //SCL SDA
  if(!bme280.init()){
    Serial.println("Device error!");
  }
  pinMode(HR_PIN, INPUT);
  pinMode(switch_Pin,INPUT);
  //BMX055 初期化
  BMX055_Init();
  setup_wifi();
  client.setServer(mqttServer,mqtt_port);

  ledcSetup(ch,12000,8); //ch basefreq dutybit
  ledcAttachPin(pin,ch); //pin ch
  for(int i=0;i<sizeof melotimeAry / sizeof(melotimeAry[0]);i++){
        musicFunc(ch,melodyAry[i],melotimeAry[i]);
   }
  ledcSetup(ch,12000,8); //ch basefreq dutybit
}
void musicFunc(int ch,int meloFreq,int meloTime){
  ledcWriteTone(ch,meloFreq); //ch freq
  delay(meloTime);
}
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

void loop()
{
  
  // connect to mqtt if not already connected
  if (!client.connect(mqttDeviceId, mqttUser, mqttPassword)) {
    
     //BMX055 加速度の読み取り
    BMX055_Accl();
    //BMX055 ジャイロの読み取り
    BMX055_Gyro();
    //BMX055 磁気の読み取り
    BMX055_Mag();
    //気圧の読み取り
   // pressure = bme280.getPressure();
    //温度の読み取り
    Temp = bme280.getTemperature();
    //湿度の読み取り
    Humid = bme280.getHumidity();
    //HERAT  = analogRead(HR_PIN);
    MENTAL = digitalRead(switch_Pin);
    LIGHT  = analogRead(cds_pin);
        //String payload = "{\"myName\":";
         String payload = "{";
            //payload += "\"";
           // payload += mqttDeviceId;
           // payload += "\"";
           // payload += ",";
            payload += "\"xAccl\":";
            payload += xAccl;
            payload += ",";
            payload += "\"yAccl\":";
            payload += yAccl;
            payload += ",";
            payload += "\"zAccl\":";
            payload += zAccl;
            payload += ",";
            /*payload += "\",\"xGyro\":";
            payload += xGyro;
            payload += "\",\"xGyro\":";
            payload += xGyro;
            payload += "\",\"xGyro\":";
            payload += xGyro;
            payload += "\",\"xMag\":";
            payload += xMag;
            payload += "\",\"yMag\":";
            payload += yMag;
            payload += "\",\"zMag\":";
            payload += zMag;*/
            payload += "\"LIGHT\":";
            payload += LIGHT;
            payload += ",";
            payload += "\"menheraflg\":";
            payload += MENTAL;
            payload += ",";
            payload += "\"heart\":";
            payload += 0;
            payload += ",";
            payload += "\"Temp\":";
            payload += Temp;
            payload += ",";
            payload += "\"Humid\":";
            payload += Humid;
           // payload += ",";
          //  payload += "\"pressure\":";
          //  payload += pressure;
            payload += "}";
     Serial.print("Publish message: ");
     Serial.println(payload);
     client.publish(mqttTopic,(char*) payload.c_str());
    //client.subscribe();
  }
  // if we're connected, then publish our message
  if (client.connect(mqttDeviceId, mqttUser, mqttPassword)) {
    
     //BMX055 加速度の読み取り
    BMX055_Accl();
    //BMX055 ジャイロの読み取り
    BMX055_Gyro();
    //BMX055 磁気の読み取り
    BMX055_Mag();
    //気圧の読み取り
   // pressure = bme280.getPressure();
    //温度の読み取り
    Temp = bme280.getTemperature();
    //湿度の読み取り
    Humid = bme280.getHumidity();
    HERAT  = analogRead(HR_PIN);
    MENTAL = digitalRead(switch_Pin);
    LIGHT  = analogRead(cds_pin);
  //String payload = "{\"myName\":";
         String payload = "{";
            //payload += "\"";
           // payload += mqttDeviceId;
           // payload += "\"";
           // payload += ",";
            payload += "\"xAccl\":";
            payload += xAccl;
            payload += ",";
            payload += "\"yAccl\":";
            payload += yAccl;
            payload += ",";
            payload += "\"zAccl\":";
            payload += zAccl;
            payload += ",";
            /*payload += "\",\"xGyro\":";
            payload += xGyro;
            payload += "\",\"xGyro\":";
            payload += xGyro;
            payload += "\",\"xGyro\":";
            payload += xGyro;
            payload += "\",\"xMag\":";
            payload += xMag;
            payload += "\",\"yMag\":";
            payload += yMag;
            payload += "\",\"zMag\":";
            payload += zMag;*/
            payload += "\"LIGHT\":";
            payload += LIGHT;
            payload += ",";
            payload += "\"menheraflg\":";
            payload += MENTAL;
            payload += ",";
            payload += "\"heart\":";
            payload += HERAT;
            payload += ",";
            payload += "\"Temp\":";
            payload += Temp;
            payload += ",";
            payload += "\"Humid\":";
            payload += Humid;
           // payload += ",";
          //  payload += "\"pressure\":";
          //  payload += pressure;
            payload += "}";
   Serial.print("Publish message: ");
   Serial.println(payload);
   client.publish(mqttTopic, (char*) payload.c_str());
  }
  // and now wait for 1s, regularly calling the mqtt loop function
  for (int i=0; i<1000; i++) {
    client.loop();
    delay(1);
  }
}
void BMX055_Init()
{
  //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Accl);
  Wire.write(0x0F); // Select PMU_Range register
  Wire.write(0x03);   // Range = +/- 2g
  Wire.endTransmission();
  delay(100);
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Accl);
  Wire.write(0x10);  // Select PMU_BW register
  Wire.write(0x08);  // Bandwidth = 7.81 Hz
  Wire.endTransmission();
  delay(100);
  //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Accl);
  Wire.write(0x11);  // Select PMU_LPW register
  Wire.write(0x00);  // Normal mode, Sleep duration = 0.5ms
  Wire.endTransmission();
  delay(100);
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Gyro);
  Wire.write(0x0F);  // Select Range register
  Wire.write(0x04);  // Full scale = +/- 125 degree/s
  Wire.endTransmission();
  delay(100);
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Gyro);
  Wire.write(0x10);  // Select Bandwidth register
  Wire.write(0x07);  // ODR = 100 Hz
  Wire.endTransmission();
  delay(100);
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Gyro);
  Wire.write(0x11);  // Select LPM1 register
  Wire.write(0x00);  // Normal mode, Sleep duration = 2ms
  Wire.endTransmission();
  delay(100);
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4B);  // Select Mag register
  Wire.write(0x83);  // Soft reset
  Wire.endTransmission();
  delay(100);
  //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4B);  // Select Mag register
  Wire.write(0x01);  // Soft reset
  Wire.endTransmission();
  delay(100);
  //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4C);  // Select Mag register
  Wire.write(0x00);  // Normal Mode, ODR = 10 Hz
  Wire.endTransmission();
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4E);  // Select Mag register
  Wire.write(0x84);  // X, Y, Z-Axis enabled
  Wire.endTransmission();
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x51);  // Select Mag register
  Wire.write(0x04);  // No. of Repetitions for X-Y Axis = 9
  Wire.endTransmission();
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x52);  // Select Mag register
  Wire.write(0x16);  // No. of Repetitions for Z-Axis = 15
  Wire.endTransmission();
}
//=====================================================================================//
void BMX055_Accl()
{
  int data[6];
  for (int i = 0; i < 6; i++)
  {
    Wire.beginTransmission(Addr_Accl);
    Wire.write((2 + i));// Select data register
    Wire.endTransmission();
    Wire.requestFrom(Addr_Accl, 1);// Request 1 byte of data
    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    if (Wire.available() == 1)
      data[i] = Wire.read();
  }
  // Convert the data to 12-bits
  xAccl = ((data[1] * 256) + (data[0] & 0xF0)) / 16;
  if (xAccl > 2047)  xAccl -= 4096;
  yAccl = ((data[3] * 256) + (data[2] & 0xF0)) / 16;
  if (yAccl > 2047)  yAccl -= 4096;
  zAccl = ((data[5] * 256) + (data[4] & 0xF0)) / 16;
  if (zAccl > 2047)  zAccl -= 4096;
  xAccl = xAccl * 0.0098; // renge +-2g
  yAccl = yAccl * 0.0098; // renge +-2g
  zAccl = zAccl * 0.0098; // renge +-2g
}
//=====================================================================================//
void BMX055_Gyro()
{
  int data[6];
  for (int i = 0; i < 6; i++)
  {
    Wire.beginTransmission(Addr_Gyro);
    Wire.write((2 + i));    // Select data register
    Wire.endTransmission();
    Wire.requestFrom(Addr_Gyro, 1);    // Request 1 byte of data
    // Read 6 bytes of data
    // xGyro lsb, xGyro msb, yGyro lsb, yGyro msb, zGyro lsb, zGyro msb
    if (Wire.available() == 1)
      data[i] = Wire.read();
  }
  // Convert the data
  xGyro = (data[1] * 256) + data[0];
  if (xGyro > 32767)  xGyro -= 65536;
  yGyro = (data[3] * 256) + data[2];
  if (yGyro > 32767)  yGyro -= 65536;
  zGyro = (data[5] * 256) + data[4];
  if (zGyro > 32767)  zGyro -= 65536;

  xGyro = xGyro * 0.0038; //  Full scale = +/- 125 degree/s
  yGyro = yGyro * 0.0038; //  Full scale = +/- 125 degree/s
  zGyro = zGyro * 0.0038; //  Full scale = +/- 125 degree/s
}
//=====================================================================================//
void BMX055_Mag()
{
  int data[8];
  for (int i = 0; i < 8; i++)
  {
    Wire.beginTransmission(Addr_Mag);
    Wire.write((0x42 + i));    // Select data register
    Wire.endTransmission();
    Wire.requestFrom(Addr_Mag, 1);    // Request 1 byte of data
    // Read 6 bytes of data
    // xMag lsb, xMag msb, yMag lsb, yMag msb, zMag lsb, zMag msb
    if (Wire.available() == 1)
      data[i] = Wire.read();
  }
  // Convert the data
  xMag = ((data[1] <<8) | (data[0]>>3));
  if (xMag > 4095)  xMag -= 8192;
  yMag = ((data[3] <<8) | (data[2]>>3));
  if (yMag > 4095)  yMag -= 8192;
  zMag = ((data[5] <<8) | (data[4]>>3));
  if (zMag > 16383)  zMag -= 32768;
}
