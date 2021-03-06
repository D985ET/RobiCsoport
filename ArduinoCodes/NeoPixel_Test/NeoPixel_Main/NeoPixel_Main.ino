#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SparkFun_APDS9960.h> // Gesture Sensor
#include <Adafruit_NeoPixel.h> //LED NeoPixel
#include <IRremote.h> //Remote Control + Receiver
#include <string.h>

LiquidCrystal_I2C lcd(0x27,20,4); 
const int ARRAY_SIZE = 5;
int tomb[ARRAY_SIZE] = {0,0,0,0,0};
int allapot = 1;
int rendezes = 0;
unsigned char* tombTemp;

SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

#define NEOPIXEL_PIN 4
#define APDS9960_INT    5
#define GREEN 0, 150, 0
#define RED 150, 0 , 0
#define BLUE 131, 238, 255

#define RECEIVER_PIN 9



Adafruit_NeoPixel pixels(ARRAY_SIZE, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 1000

void setup() {
  Serial.begin(9600);
  pinMode(APDS9960_INT, INPUT);
  attachInterrupt(0, interruptRoutine, FALLING);
  apds.init(); 
  apds.enableGestureSensor(true);
  IrReceiver.begin(RECEIVER_PIN, ENABLE_LED_FEEDBACK); //start the receiver
  // put your setup code here, to run once:
  tombTemp = (unsigned char *)malloc(sizeof(unsigned char) * ARRAY_SIZE);
  lcd.init();
  //lcd.init();
  lcd.backlight();
  pixels.begin();
  lcd.setCursor(0,0);
  lcd.print("Tombfeltoltes");
  delay(1000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(allapot == 1){
    receiverHandling2();
  }
  if(allapot == 2 )
  {
    /*
    if(isr_flag == 1)
    {
      
      detachInterrupt(0);
      handleGesture();
      isr_flag = 0;
      attachInterrupt(0, interruptRoutine, FALLING);
      
      
    }*/
    rendezes = 2;
    allapot = 3;
    delay(250);
  }
  if (allapot == 3)  
  {
    switch(rendezes) {
      case 1:
        bubbleSort();
        break;
      case 2:
        cocktailSort();
        break;
      case 3:
        minSelectSort();
        break;
      case 4:
        insertionSort();
        break;
      default:
        break;
    }
  }
  if(allapot == 4){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Program vege");
    Serial.println("Program vege");
    allapot=5;
    
  }
}

#pragma region GestureSensor
void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Valassz algoritmust!");
    if ( apds.isGestureAvailable() ) {
      Serial.println("elsoif");
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
        rendezes = 4;
        allapot = 3;
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        rendezes = 3;
        allapot = 3;
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        rendezes = 2;
        allapot = 3;
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        rendezes = 1;
        allapot = 3;
        break;
      default:
        Serial.println("NONE");
        
    }
   
  }
 
}
#pragma endregion GestureSensor

#pragma region Rendezes
// k??k sz??nnel felvillannak a t??mb elemei, rendez??algoritmus kezdete el??tt
void lightupArray()
{
  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    pixels.setPixelColor(i, pixels.Color(BLUE)); // light blue
    pixels.show();
    delay(DELAYVAL);
  }
}

void printArr(int tomb[], int len) {
  int i;
  for (i = 0; i < len; ++i) {
    lcd.print(String(String(tomb[i]) + ' '));
  }
}

void swap(int *x1, int *x2)
{
  int temp = *x1;
  *x1 = *x2;
  *x2 = temp;
}
void minSelectSort()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("E:");
  printArr(tomb, ARRAY_SIZE);
  lcd.setCursor(0, 1);
  lcd.print("Calculating...");
  lightupArray();
  delay(DELAYVAL);
  int min_i, i, j;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    pixels.setPixelColor(i, pixels.Color(RED)); // az aktu??lis kiv??lasztott elem, aminek a hely??re megy a minimum
    pixels.show();
    min_i = i;
    for (j = (int)(i + 1); j < ARRAY_SIZE; j++)
    {
      pixels.setPixelColor(j, pixels.Color(RED)); // amit hasonl??t, keresi minimumot
      pixels.show();
      if (tomb[j] < tomb[min_i])
        min_i = j;
      delay(DELAYVAL); //egy ideig piros
      pixels.setPixelColor(j, pixels.Color(BLUE)); // visszav??ltja k??kre
      pixels.show();
    }

    if (min_i != i)
    {
      pixels.setPixelColor(min_i, pixels.Color(GREEN)); // a minimum hely??n z??ldre v??lt
      pixels.show(); 
      delay(DELAYVAL);
      swap(&tomb[i], &tomb[min_i]); //csere
      pixels.setPixelColor(i, pixels.Color(GREEN));  //ahova teszi a minimumot, az lesz z??ld
      pixels.setPixelColor(min_i, pixels.Color(RED)); // a cser??lt elem piros lesz
      pixels.show();
      delay(DELAYVAL); //v??r
      pixels.setPixelColor(min_i, pixels.Color(BLUE)); // visszav??lt az alapsz??nre, k??kre
      pixels.show();
      delay(DELAYVAL);
    }
    else
    {
      pixels.setPixelColor(i, pixels.Color(GREEN)); //ha helyben van a legkisebb, akkor egyb??l z??ldre v??lt, a rendezett r??sz folyamatosan z??ldd?? v??lik
      pixels.show();
      delay(DELAYVAL);
    }
  }
  pixels.setPixelColor(i, pixels.Color(GREEN)); //ha helyben van a legkisebb, akkor egyb??l z??ldre v??lt, a rendezett r??sz folyamatosan z??ldd?? v??lik
  pixels.show();

  lcd.setCursor(0, 1);
  lcd.print("R:");
  printArr(tomb, ARRAY_SIZE);
  delay(3000);  
  allapot = 4;
}
void bubbleSort()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("E:");
  printArr(tomb, ARRAY_SIZE);
  lcd.setCursor(0, 1);
  lcd.print("Calculating...");
  /*unsigned char *copy = (unsigned char *)malloc(sizeof(unsigned char) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);*/
  lightupArray(); //felvillan minden k??kkel
  delay(DELAYVAL);
  int i, j;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    for (j = 0; j < ARRAY_SIZE - i - 1; j++)
    {
      pixels.setPixelColor(j, pixels.Color(RED)); //szomsz??dos elemeket pirosra v??ltja
      pixels.setPixelColor(j + 1, pixels.Color(RED));
      pixels.show();
      delay(DELAYVAL);
      if (tomb[j] > tomb[j + 1])
      {
        swap(&tomb[j], &tomb[j + 1]);
      }
      pixels.setPixelColor(j, pixels.Color(BLUE)); //a szomsz??dos elemeket visszav??ltja k??kre
      pixels.setPixelColor(j + 1, pixels.Color(BLUE));
      pixels.show();
      delay(DELAYVAL);
    }
    pixels.setPixelColor(ARRAY_SIZE - i - 1, pixels.Color(GREEN)); //a rendezett elem az utols?? lesz
    pixels.show();
    delay(DELAYVAL);
  }
  pixels.setPixelColor(0, pixels.Color(GREEN)); //a rendezett elem az utols?? lesz
  pixels.show();
  delay(DELAYVAL);

  lcd.setCursor(0, 1);
  lcd.print("R:");
  printArr(tomb, ARRAY_SIZE);
  delay(3000);  
  allapot = 4;
}    
   
void cocktailSort()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("E:");
  printArr(tomb, ARRAY_SIZE);
  lcd.setCursor(0, 1);
  lcd.print("Calculating...");
  lightupArray();
  delay(DELAYVAL);
  int i, j, k;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    for (j = i; j < ARRAY_SIZE - i - 1; j++) //felfel?? bubor??kol
    {
      pixels.setPixelColor(j, pixels.Color(RED)); 
      pixels.setPixelColor(j + 1, pixels.Color(RED));
      pixels.show();
      delay(DELAYVAL);
      if (tomb[j] > tomb[j + 1])
        swap(&tomb[j], &tomb[j + 1]);
      pixels.setPixelColor(j, pixels.Color(BLUE));
      pixels.setPixelColor(j + 1, pixels.Color(BLUE));
      pixels.show();
      delay(DELAYVAL);
    }
    pixels.setPixelColor(j, pixels.Color(GREEN));
    pixels.show();
    delay(DELAYVAL);
    k = (int)(ARRAY_SIZE - i - 2);

    while (k > i)
    {
      pixels.setPixelColor(k, pixels.Color(RED));
      pixels.setPixelColor(k - 1, pixels.Color(RED));
      pixels.show();
      delay(DELAYVAL);
      if (tomb[k - 1] > tomb[k])
        swap(&tomb[k - 1], &tomb[k]);
      
      pixels.setPixelColor(k, pixels.Color(BLUE));
      pixels.setPixelColor(k - 1, pixels.Color(BLUE));
      pixels.show();
      delay(DELAYVAL);
      k--;
    }
    pixels.setPixelColor(k, pixels.Color(GREEN));
    pixels.show();
    delay(DELAYVAL);

    lcd.setCursor(0, 1);
    lcd.print("R:");
    printArr(tomb, ARRAY_SIZE);
    delay(3000);  
    allapot = 4;  
  }
}
void insertionSort()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("E:");
  printArr(tomb, ARRAY_SIZE);
  lcd.setCursor(0, 1);
  lcd.print("Calculating...");
  
  lightupArray(); //felvillantjuk a t??mb elemeit k??kkel
  delay(DELAYVAL);
  int i, elementToInsert, j;
  pixels.setPixelColor(0, pixels.Color(GREEN)); //az els?? elem az fixen rendezett
  pixels.show();
  delay(DELAYVAL);

  for (i = 1; i < ARRAY_SIZE; i++) //a 0. elem az rendezett, ez??rt 1-t??l kezdj??k a besz??r??st
  {
    elementToInsert = tomb[i]; //kimentj??k a besz??rand?? elemet
    j = i - 1; //a besz??rand?? elem el??tti elem, a legutols?? rendezett r??sz eleme
    pixels.setPixelColor(i, pixels.Color(RED)); //besz??r??sra kiv??lasztott elemet fevillantjuk pirossal
    pixels.show();
    delay(DELAYVAL);
    while (j >= 0 && tomb[j] > elementToInsert) //megkeress??k a besz??rand?? elem hely??t a rendezett r??szen bel??l
    {
      
      pixels.setPixelColor(j+1, pixels.Color(GREEN)); 
      pixels.setPixelColor(j, pixels.Color(RED));
      pixels.show();
      tomb[j + 1] = tomb[j];
      j = (int)(j - 1);
      delay(DELAYVAL);
    }
    tomb[j + 1] = elementToInsert;
    pixels.setPixelColor(j+1, pixels.Color(GREEN)); //besz??r??sra kiv??lasztott elemet fevillantjuk pirossal
    pixels.show();
    delay(DELAYVAL);
  }

    lcd.setCursor(0, 1);
    lcd.print("R:");
    printArr(tomb, ARRAY_SIZE); 
    delay(3000);  
    allapot = 4;
}
#pragma endregion Rendezes

char i = 0;
char j = 0;
char* elso;
char* ketto;
char* temp;
char* temptomb = (char *)malloc(sizeof(char) * 3);

void arrayFillWithRC(char* input) //elmenti egy ciklusban a rendezend?? t??mb elemeit
{
  //2 elem?? string 
  if(j < 5)
  {
    
    if (i == 0)
    {

      elso = input;
      temptomb[0] = input[0];
      i++;
      
    }
    else if(i==1)
    {
      ketto = input;
      temptomb[1] = ketto[0];
      if(temptomb[0] == 0)
      {
        tomb[j] = temptomb[1];
      }
      temptomb[2] = '\0';
      tombTemp[j] = atoi(temptomb);

      i++;
      Serial.print(j);
      Serial.print(". elem elmentve, az elem: ");
      Serial.println(tombTemp[j]);
      j++;
    }
    else
    {
      i = 0;
    }
  }
  else
  {
    lcd.clear();
    lcd.print("Tomb feltoltve!");
    Serial.println("Tomb feltoltve!");
    ConvertArray();
    lcd.setCursor(0,1);
    printArr(tomb, ARRAY_SIZE);
    allapot = 2; // 2 = tomb feltoltve
  } 
}

void receiverHandling2(){ //returns string "0"..."9", "ND", "OK"
  allapot = 1; //1 = tombfelt??lt??s kezd??dik
  if (IrReceiver.decode()){
    switch (IrReceiver.decodedIRData.decodedRawData)
    {
      case 2907897600: lcd.print("0"); arrayFillWithRC("0"); break; // Button 0
      case 3910598400: lcd.print("1"); arrayFillWithRC("1"); break;// Button 1
      case 3860463360: lcd.print("2"); arrayFillWithRC("2"); break;// Button 2
      case 4061003520: lcd.print("3"); arrayFillWithRC("3"); break;// Button 3
      case 4077715200: lcd.print("4"); arrayFillWithRC("4"); break;// Button 4
      case 3877175040: lcd.print("5"); arrayFillWithRC("5"); break;// Button 5
      case 2707357440: lcd.print("6"); arrayFillWithRC("6"); break;// Button 6
      case 4144561920: lcd.print("7"); arrayFillWithRC("7"); break;// Button 7
      case 3810328320: lcd.print("8"); arrayFillWithRC("8"); break;// Button 8
      case 2774204160: lcd.print("9"); arrayFillWithRC("9"); break;// Button 9
      case 3208707840: lcd.print("OK"); arrayFillWithRC("OK"); lcd.clear();break;
      default: //lcd.print("Nem szamjegy"); 
        break; //Not Defined
    }
    lcd.print(" ");
    IrReceiver.resume();
  }
  else if(tomb[0] != 0)
  {
    
    allapot = 2;  
  }
}

void ConvertArray(){
  
for (int i=0; i < ARRAY_SIZE; i++)
{
    tomb[i] = tombTemp[i];  
  
}
}
