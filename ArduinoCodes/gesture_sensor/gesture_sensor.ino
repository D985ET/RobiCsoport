#include <Wire.h>
#include <SparkFun_APDS9960.h>

// pinek, makrók
#define APDS9960_INTERRUPT 2 // megszakító/interrupt pinnek kell lennie

// globális változók
SparkFun_APDS9960 gestSensor = SparkFun_APDS9960();
int isr_flag = 0;

void setup() {
  Serial.begin(9600);
  //interrupt pin-t inputnak állítjuk be
  pinMode(APDS9960_INTERRUPT, INPUT);
  
  // Interrupt Service Routine beállítása
  attachInterrupt(digitalPinToInterrupt(APDS9960_INTERRUPT), interruptRoutine, FALLING);
  //1. paraméter: alapértelmezetten 0 szerepelt, de szerintem a APDS9960_INTERRUPT 2-es pin is megfelel, mivel ugyanaz
  //2. paraméter: az interrupt után lefutó metódus, IRS
  //3. paraméter: mikor legyen interrupt? - amikor lefutó ágon van

  if ( gestSensor.init() ) {
    Serial.println("APDS-9960 inicializálása sikerrel járt.");
  } else {
    Serial.println("Hiba az APDS-9960 inicializálásakor.");
  }
  
  if ( gestSensor.enableGestureSensor(true) ) {
    Serial.println("Gesztusérzékelő működésbe lépett.");
  } else {
    Serial.println("Hiba a gesztusérzékelő indításakor");
  }
}

void loop() {
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    gesztusKezeles();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
}

void interruptRoutine() {
  isr_flag = 1;
}

void gesztusKezeles() {
    if ( gestSensor.isGestureAvailable() ) {
    switch ( gestSensor.readGesture() ) {
      case DIR_UP:
        Serial.println("FEL - Beszúrásos rendezés");
        break;
      case DIR_DOWN:
        Serial.println("LE - Minimumkiválasztásos rendezés");
        break;
      case DIR_LEFT:
        Serial.println("BALRA - Koktélrendezés");
        break;
      case DIR_RIGHT:
        Serial.println("JOBBRA - Buborékrendezés");
        break;
      case DIR_NEAR:
        Serial.println("KÖZEL - Shell-rendezés");
        break;
      default:
        Serial.println("NONE");
    }
  }
}
