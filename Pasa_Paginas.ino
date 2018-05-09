/* Pasapáginas automático controlado con 4 servomotores y 1 servomotor de rotación contínua desde el driver PCA9685 y Arduino.
   Autor: Javier Vargas. El Hormiguero 30/04/2018
   https://creativecommons.org/licenses/by/4.0/
*/

//Pines
#define SFrontal 0
#define SDerecha 1
#define SIzquierda 2
#define SPasador 3
#define SRueda 4
#define BotonDer 5
#define BotonIzq 7

//Calibración del angulo 0 (Servos verticales y horizontales)
//#define DebugOffset
#define SFrontal0 90
#define SDerecha0 85
#define SIzquierda0 105
#define SPasador0 97

//Amplitud de movimiento servos (Movimiento total el doble del indicado)
#define SFrontal1 90 //Rango de apoyo de la rueda
#define SFrontal2 40 //Rango de preparación
#define SDerecha1 70 //Rango de sujeccin de la pagina derecha
#define SIzquierda1 70 //Rango de sujeccion de la pagina izquierda
#define SPasador1 80 //Rango del pasador 
#define VelocidadRueda 20 //Velocidad entre 0 y 100 de la rueda
#define TiempoRuedaDER 1350
#define TiempoRuedaIZQ 950

//Ancho de señal PWM para controlar servos
//Servo angulo
#define SERVOMIN  160 // Anchura minima de señal PWM
#define SERVOMAX  550 // Anchura maxima de señal PWM
//Servo rotacion continua
#define SERVOCW  200 // velocidad maxima sentido horario
#define SERVOCCW 550 // velocidad maxima antihoraria
#define SERVOCERO 331 //Parado

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //Direccion 0x40

void setup() {
  //Serial.begin(9600);
  pinMode(BotonDer, INPUT_PULLUP);
  pinMode(BotonIzq, INPUT_PULLUP);

  pwm.begin();
  pwm.setPWMFreq(60);  //Frecuencia de 60Hz en servos analógicos
  delay(10);
  StopDER();
}

///////////////////////////////////
///////////////LOOP//////////////
///////////////////////////////////

void loop() {
  
  //MODO DEBUG
#ifdef DebugOffset
  SetServo(SDerecha, SDerecha0);
  SetServo(SIzquierda, SIzquierda0);
  SetServo(SPasador, SPasador0);
  SetServo(SFrontal, SFrontal0);
  SetVelocidad(SRueda, 0);
#else

  //PROGRAMA PRINCIPAL
  if (digitalRead(BotonIzq) == 0) {
    MovIzq();
  }
  if (digitalRead(BotonDer) == 0) {
    MovDer();
  }
#endif

}
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////

void MovIzq() {
  StopIZQ();
  delay(1000);
  SetServo(SFrontal, SFrontal0 + SFrontal1);
  delay(400);
  SetVelocidad(SRueda, -VelocidadRueda);
  delay(TiempoRuedaIZQ);
  SetVelocidad(SRueda, 0);
  SetServo(SPasador, SPasador0 - SPasador1);
  delay(100);
  SetServo(SFrontal, SFrontal0);
  delay(1000);
  SetServo(SDerecha, SDerecha0 + SDerecha1);
  delay(1000);
  StopIZQ();
  delay(200);
  SetServo(SDerecha, 0);
  delay(500);
  StopIZQ();
}

void MovDer() {
  StopDER();
  delay(1000);
  SetServo(SFrontal, SFrontal0 - SFrontal1);
  delay(400);
  SetVelocidad(SRueda, VelocidadRueda);
  delay(TiempoRuedaDER);
  SetVelocidad(SRueda, 0);
  SetServo(SPasador, SPasador0 + SPasador1);
  delay(400);
  SetServo(SFrontal, SFrontal0);
  delay(1000);
  SetServo(SIzquierda, SIzquierda0 - SIzquierda1);
  delay(1000);
  StopDER();
  delay(200);
  SetServo(SDerecha, 0);
  delay(500);
  StopDER();
}

void StopDER() {
  SetServo(SDerecha, SDerecha0 - SDerecha1 + 20);
  SetServo(SIzquierda, SIzquierda0 + SIzquierda1 - 20);
  SetServo(SPasador, SPasador0 - SPasador1);
  SetServo(SFrontal, SFrontal0 - SFrontal2);
  SetVelocidad(SRueda, 0);
}

void StopIZQ() {
  SetServo(SDerecha, SDerecha0 - SDerecha1 + 20);
  SetServo(SIzquierda, SIzquierda0 + SIzquierda1 - 20);
  SetServo(SPasador, SPasador0 + SPasador1);
  SetServo(SFrontal, SFrontal0 + SFrontal2);
  SetVelocidad(SRueda, 0);
}

//Servo 0 - 180º
void SetServo(byte servo, int angulo) {
  angulo = constrain(angulo, 0, 180);
  pwm.setPWM(servo, 0, map(angulo, 0, 180, SERVOMIN, SERVOMAX));
}

//Servo de rotación continua
void SetVelocidad(byte servo, int velocidad) {
  velocidad = constrain(velocidad, -100, 100);
  if (velocidad < 0) pwm.setPWM(servo, 0, map(velocidad, -100, 0, SERVOCCW, SERVOCERO));
  else pwm.setPWM(servo, 0, map(velocidad, 0, 100, SERVOCERO, SERVOCW));
}

