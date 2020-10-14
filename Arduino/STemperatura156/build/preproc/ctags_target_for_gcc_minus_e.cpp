# 1 "/run/media/jdemc/Nuevo vol/INBOX LINUX/Arduino/STemperatura156/STemperatura156.ino"
# 2 "/run/media/jdemc/Nuevo vol/INBOX LINUX/Arduino/STemperatura156/STemperatura156.ino" 2
dht11 DHT11;


float tempC;
//----temp----
int pinLM35 = 0;//pin datos 
//----Dist----
long tiempo;
int disparador = 22; // triger
int entrada = 24; // echo
float distancia;
//----humo----
int pin_mq = 32;

void setup()
{
  //--Dist--
  pinMode(disparador, 0x1);
  pinMode(entrada, 0x0);
  //--Humo-- 
  pinMode(pin_mq, 0x0);

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

  digitalWrite(disparador, 0x1);//pulso para activar sensor
  delayMicroseconds(10);
  digitalWrite(disparador, 0x0);
  tiempo = (pulseIn(entrada, 0x1)/2);
  distancia = float(tiempo * 0.0343);
  Serial.print("Sensor D: ");
  Serial.println(distancia);
  Serial.print("\n");
  delay(1000);

  int chk = DHT11.read(28);
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
