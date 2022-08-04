#include<ThingerESP8266.h>
#include<ESP8266WiFi.h>
#include "DHT.h" //library sensor yang telah diimportkan
#define DHTPIN D1     //Pin apa yang digunakan
#define DHTTYPE DHT11   // DHT 11
#define kipas D4
#define pompa D6
//#define termoelektrik D7

DHT dht(DHTPIN, DHTTYPE);
int sensorPinSoil = A0; 
float humidity_1,celcius_1,kelembabanTanah;

//konfigurasi ThingerIO

#define USERNAME "Aminahnurrahmawati"
#define DEVICE_ID "rblelektronikaindustri"
#define DEVICE_CREDENTIAL "iW+STmXXPdgI0R1i"
//#define LED_Pin 4

//variabel untuk thinger.io
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

//konfigurasi wifi 
const char* SSID= "KOSANSTONEATAS" ;
const char* SSID_PASSWORD = "123lulusitb";

void setup() {
  Serial.begin(9600);
  thing.add_wifi(SSID, SSID_PASSWORD);
  dht.begin();

  pinMode(kipas, OUTPUT);
  pinMode(pompa, OUTPUT);
//  pinMode(termoelektrik, OUTPUT);

 thing["informasi_greenhouse"] >> [](pson & out) {
  out["kelembaban udara"] = humidity_1;
  out["Temperatur"] = celcius_1;
  out["Kelembaban Tanah"] = kelembabanTanah;
 };
}
 void loop() {
  thing.handle();

   //Pembacaan untuk data kelembaban
  humidity_1 = dht.readHumidity();
  //Pembacaan dalam format celcius (c)
  celcius_1 = dht.readTemperature();
  //kelembaban tanah
  int hasilPembacaan = analogRead(sensorPinSoil);
  kelembabanTanah = (100 - ((hasilPembacaan/1023.00)*100));
  Serial.print("Kelembaban Tanah");Serial.print(kelembabanTanah); 
  Serial.println("Temperatur");Serial.print(celcius_1);
  Serial.println("Kelembaban Udara");Serial.print(humidity_1);
  delay(3000);

if (dht.readTemperature() > 20) {
    digitalWrite(kipas, LOW);
  } else if (dht.readTemperature() <= 20) {
    digitalWrite(kipas, HIGH);
  }
  //if (dht.readTemperature() < 20) {
    //digitalWrite(termoelektrik, LOW);
  //} else if (dht.readTemperature() <= 20) {
   // digitalWrite(termoelektrik, HIGH);
 // }

  if (kelembabanTanah < 35) {
    digitalWrite(pompa, HIGH);
  } else {
    digitalWrite(pompa, LOW);
  }
  
  
 }
