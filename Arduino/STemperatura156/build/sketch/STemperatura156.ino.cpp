#include <Arduino.h>
#line 1 "/run/media/jdemc/Nuevo vol/INBOX LINUX/Arduino/STemperatura156/STemperatura156.ino"
#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 28

float tempC;
//----temp----
int pinLM35 = 0;//pin datos 
//----Dist----
long tiempo;
int disparador = 22;   // triger
int entrada = 24;      // echo
float distancia;
//----humo----
int pin_mq = 32;

#line 16 "/run/media/jdemc/Nuevo vol/INBOX LINUX/Arduino/STemperatura156/STemperatura156.ino"
void setup();
#line 26 "/run/media/jdemc/Nuevo vol/INBOX LINUX/Arduino/STemperatura156/STemperatura156.ino"
void loop();
#line 16 "/run/media/jdemc/Nuevo vol/INBOX LINUX/Arduino/STemperatura156/STemperatura156.ino"
void setup()
{
  //--Dist--
  pinMode(disparador, OUTPUT);
  pinMode(entrada, INPUT);
  //--Humo-- 
  pinMode(pin_mq, INPUT);

  Serial.begin(9600);
}
void loop()
{

  tempC = analogRead(pinLM35);
  //calculamos la temperatura con la fórmula
  tempC = (5.0 * tempC * 100.0)/1024.0;
  //envia el dato al puerto serial
  Serial.print("Sensor T: ");
  Serial.print(tempC);
  Serial.print("\n");
  delay(1000);

  digitalWrite(disparador, HIGH);//pulso para activar sensor
  delayMicroseconds(10);
  digitalWrite(disparador, LOW);
  tiempo = (pulseIn(entrada, HIGH)/2);
  distancia = float(tiempo * 0.0343);
  Serial.print("Sensor D: ");
  Serial.println(distancia);
  Serial.print("\n");
  delay(1000);

  int chk = DHT11.read(DHT11PIN);
  Serial.print("Sensor H: ");
  Serial.print((float)DHT11.humidity, 2);
  Serial.print("\n");
  delay(1000);

  boolean mq_estado = digitalRead(pin_mq);//Leemos el sensor
  if(mq_estado) //si la salida del sensor es 1
  {
    Serial.print("Sensor G: ");
    Serial.println("Sin presencia de Gas(GLP/propano)");
  }
  else 
  {
    Serial.print("Sensor G: ");
    Serial.println("Gas detectado(GLP/propano)");
  }
  delay(1000); 
}

