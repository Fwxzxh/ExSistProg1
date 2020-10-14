
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
//int pin_mq = 32;
#define         MQ1                   (1)     //define la entrada analogica para el sensor
#define         RL_VALOR              (5)     //define el valor de la resistencia mde carga en kilo ohms
#define         RAL                   (9.83)  // resistencia del sensor en el aire limpio / RO, que se deriva de la datasheet                                         tabla de la hoja de datos
#define         GAS_LP                (0)
#define         GAS_CO                (1)
float           LPCurve[3]  =  {2.3, 0.21, -0.47};//curva que identifca al Gas LP
float           COCurve[3]  =  {2.3, 0.72, -0.34};//Curva que identifica al Gas CO
float           Ro           =  10;
int LP;
int CO;
//---humedad---
float humedad;
//----fan----
int mssg = 0;
int FAN = 40;//pin Ventilador


void setup()
{
  //--Dist--
  pinMode(disparador, OUTPUT);
  pinMode(entrada, INPUT);
  //--Humo---------------------------
  //Calibrando el sensor.
  Ro = Calibracion(MQ1);
  //--fan--
  pinMode(FAN, OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  
  LP = porcentaje_gas(lecturaMQ(MQ1) / Ro, GAS_LP);//Calculo el % de gas y lo mando
  CO = porcentaje_gas(lecturaMQ(MQ1) / Ro, GAS_LP);//Calculo el % de gas y lo mando
  //delay(500);

  tempC = analogRead(pinLM35);
  //calculamos la temperatura con la fórmula
  tempC = (5.0 * tempC * 100.0) / 1024.0;


  digitalWrite(disparador, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  digitalWrite(disparador, HIGH);//pulso para activar sensor
  delayMicroseconds(10);
  digitalWrite(disparador, LOW);
  //  tiempo = (pulseIn(entrada, HIGH)/2);
  //  distancia = float(tiempo * 0.0343);
  tiempo = pulseIn(entrada, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
  distancia = tiempo * 10 / 292 / 2;  //convertimos a distancia, en cm


  int chk = DHT11.read(DHT11PIN);//Leo Pin DHT11
  humedad = (DHT11.humidity);
  //delay(1000);

  Serial.print("LP: ");
  Serial.print(LP);
  Serial.print(" ");
  Serial.print("CO: ");
  Serial.print(CO);
  Serial.print(" ");
  Serial.print("Sen T: ");
  Serial.print(tempC);
  Serial.print(" ");
  Serial.print("Sen D: ");
  Serial.println(distancia);
  Serial.print(" ");
  Serial.print(" Sen H: ");
  Serial.print(humedad);
  Serial.print(" "); //
  delay(1000);
  

  mssg = Serial.read();//leo serial, si recibo H, enciene el Ventilador
  if (mssg == 'H')
  {
    digitalWrite(FAN, HIGH);
  }
  else if (mssg == 'L')
  {
    digitalWrite(FAN, LOW);
  }
  
}


float calc_res(int raw_adc)//Calcula el valor de la resistencia
{
  return ( ((float)RL_VALOR * (1023 - raw_adc) / raw_adc));
}

float Calibracion(float mq_pin) {//Calibra la resistencia con valores actuales
  int i;
  float val = 0;
  for (i = 0; i < 50; i++) {                                                                         //tomar múltiples muestras
    val += calc_res(analogRead(mq_pin));
    Serial.print("Calibrando");
    Serial.print(i);
    delay(50);
  }
  val = val / 50;                                                                                       //calcular el valor medio
  val = val / RAL;
  return val;
}

float lecturaMQ(int mq_pin) {// Lee del sensor analogico el valor de la resistencia
  int i;
  float rs = 0;
  for (i = 0; i < 5; i++) {
    rs += calc_res(analogRead(mq_pin));
    delay(50);
  }
  rs = rs / 5;
  return rs;
}

int porcentaje_gas(float rs_ro_ratio, int gas_id) {//Calcula el porcentaje de gas con la curva
  if ( gas_id == GAS_LP ) {
    return porcentaje_gas(rs_ro_ratio, LPCurve);
  } else if ( gas_id == GAS_CO ) {
    return porcentaje_gas(rs_ro_ratio, COCurve);
  }
  return 0;
}

int porcentaje_gas(float rs_ro_ratio, float *pcurve) {
  return (pow(10, (((log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0])));
}
