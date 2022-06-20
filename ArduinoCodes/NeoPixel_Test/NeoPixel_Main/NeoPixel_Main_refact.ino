#pragma region Könyvtárak beimportálása
#include <LiquidCrystal_I2C.h>
#include <SparkFun_APDS9960.h> // Gesture Sensor
#include <Adafruit_NeoPixel.h> //LED NeoPixel
#include <IRremote.h> //Remote Control + Receiver
#include <string.h>
#include <stdint.h>
#pragma endregion

#pragma region Állapotok definiálása
#define TOMBFELTOLTES 1
#define TOMBFELTOLTVE 2
#define RENDEZES 3
#define CLEAR 4
#pragma endregion

LiquidCrystal_I2C lcd(0x27,20,4);
Adafruit_NeoPixel pixels(ARRAY_SIZE, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const uint8_t ARRAY_SIZE = 5;
uint8_t tomb[ARRAY_SIZE] = new uint8_t[5];
uint8_t allapot;
uint8_t rendezes = 0;
uint8_t* tombTemp;

SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint8_t isr_flag = 0;

#define NEOPIXEL_PIN 4
#define APDS9960_INT    5
#define RECEIVER_PIN 9
#define GREEN 0, 150, 0
#define RED 150, 0 , 0
#define BLUE 131, 238, 255
#define DELAYVAL 1000

void setup() {
  Serial.begin(9600);
  pinMode(APDS9960_INT, INPUT);
  attachInterrupt(0, interruptRoutine, FALLING);
  apds.init(); 
  apds.enableGestureSensor(true);
  IrReceiver.begin(RECEIVER_PIN, ENABLE_LED_FEEDBACK); //start the receiver
  // put your setup code here, to run once:
  tombTemp = (uint8_t *)malloc(sizeof(uint8_t) * ARRAY_SIZE);
  lcd.init();
  //lcd.init();
  lcd.backlight();
  pixels.begin();
  lcd.print("Tombfeltoltes");
  delay(1000);
  lcd.clear();
  allapot = TOMBFELTOLTES;
}

void loop() {
  // put your main code here, to run repeatedly:
	switch (allapot)
	{
		case TOMBFELTOLTES:
			receiverHandling2();
			break;
		case TOMBFELTOLTVE:
			if (isr_flag == 1)
			{
			detachInterrupt(0);
			rendezes = handleGesture();
			isr_flag = 0;
			attachInterrupt(0, interruptRoutine, FALLING);
			}
			delay(250);
		case RENDEZES:
			rendez(); 
			break;
		case CLEAR:
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Program vege");
			break;
		default:
			break;
	}
}

#pragma region GestureSensor
void interruptRoutine() {
  isr_flag = 1;
}

uint8_t handleGesture() {
    uint8_t valasztottRendezo;
	if ( apds.isGestureAvailable() ) {
		Serial.println("elsoif");
		switch ( apds.readGesture() ) {
		case DIR_UP:
			Serial.println("UP");
			valasztottRendezo = 4;
		case DIR_DOWN:
			Serial.println("DOWN");
			valasztottRendezo = 3;
		case DIR_LEFT:
			Serial.println("LEFT");
			valasztottRendezo = 2;
		case DIR_RIGHT:
			Serial.println("RIGHT");
			valasztottRendezo = 1;
		default:
			valasztottRendezo = 0;
			Serial.println("NONE");
			
		}
		allapot = RENDEZES;
		return valasztottRendezo;
  }
 
}
#pragma endregion GestureSensor

#pragma region Rendezes
// kék színnel felvillannak a tömb elemei valamely rendezőalgoritmus kezdete előtt
void rendez(){
  switch (rendezes)
  {
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
void lightupArray()
{
  for (uint8_t i = 0; i < ARRAY_SIZE; i++)
  {
    pixels.setPixelColor(i, pixels.Color(BLUE)); // light blue
    pixels.show();
    delay(DELAYVAL);
  }
}
void printArr(uint8_t tomb[], uint8_t len) {
  uint8_t i;
  for (i = 0; i < len; ++i) {
    lcd.print(String(String(tomb[i]) + ' '));
  }
}
void swap(uint8_t *x1, uint8_t *x2)
{
  uint8_t temp = *x1;
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
  uint8_t min_i, i, j;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    pixels.setPixelColor(i, pixels.Color(RED)); // az aktuális kiválasztott elem, aminek a helyére megy a minimum
    pixels.show();
    min_i = i;
    for (j = (uint8_t)(i + 1); j < ARRAY_SIZE; j++)
    {
      pixels.setPixelColor(j, pixels.Color(RED)); // amit hasonlít, keresi minimumot
      pixels.show();
      if (tomb[j] < tomb[min_i])
        min_i = j;
      delay(DELAYVAL); //egy ideig piros
      pixels.setPixelColor(j, pixels.Color(BLUE)); // visszaváltja kékre
      pixels.show();
    }

    if (min_i != i)
    {
      pixels.setPixelColor(min_i, pixels.Color(GREEN)); // a minimum helyén zöldre vált
      pixels.show(); 
      delay(DELAYVAL);
      swap(&tomb[i], &tomb[min_i]); //csere
      pixels.setPixelColor(i, pixels.Color(GREEN));  //ahova teszi a minimumot, az lesz zöld
      pixels.setPixelColor(min_i, pixels.Color(RED)); // a cserélt elem piros lesz
      pixels.show();
      delay(DELAYVAL); //vár
      pixels.setPixelColor(min_i, pixels.Color(BLUE)); // visszavált az alapszínre, kékre
      pixels.show();
      delay(DELAYVAL);
    }
    else
    {
      pixels.setPixelColor(i, pixels.Color(GREEN)); //ha helyben van a legkisebb, akkor egyből zöldre vált, a rendezett rész folyamatosan zölddé válik
      pixels.show();
      delay(DELAYVAL);
    }
  }
  pixels.setPixelColor(i, pixels.Color(GREEN)); //ha helyben van a legkisebb, akkor egyből zöldre vált, a rendezett rész folyamatosan zölddé válik
  pixels.show();

  lcd.setCursor(0, 1);
  lcd.print("R:");
  printArr(tomb, ARRAY_SIZE);
  delay(3000);  
  allapot = CLEAR;
}
void bubbleSort()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("E:");
  printArr(tomb, ARRAY_SIZE);
  lcd.setCursor(0, 1);
  lcd.print("Calculating...");
  /*uint8_t *copy = (uint8_t *)malloc(sizeof(uint8_t) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);*/
  lightupArray(); //felvillan minden kékkel
  delay(DELAYVAL);
  uint8_t i, j;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    for (j = 0; j < ARRAY_SIZE - i - 1; j++)
    {
      pixels.setPixelColor(j, pixels.Color(RED)); //szomszédos elemeket pirosra váltja
      pixels.setPixelColor(j + 1, pixels.Color(RED));
      pixels.show();
      delay(DELAYVAL);
      if (tomb[j] > tomb[j + 1])
      {
        swap(&tomb[j], &tomb[j + 1]);
      }
      pixels.setPixelColor(j, pixels.Color(BLUE)); //a szomszédos elemeket visszaváltja kékre
      pixels.setPixelColor(j + 1, pixels.Color(BLUE));
      pixels.show();
      delay(DELAYVAL);
    }
    pixels.setPixelColor(ARRAY_SIZE - i - 1, pixels.Color(GREEN)); //a rendezett elem az utolsó lesz
    pixels.show();
    delay(DELAYVAL);
  }
  pixels.setPixelColor(0, pixels.Color(GREEN)); //a rendezett elem az utolsó lesz
  pixels.show();
  delay(DELAYVAL);

  lcd.setCursor(0, 1);
  lcd.print("R:");
  printArr(tomb, ARRAY_SIZE);
  delay(3000);  
  allapot = CLEAR;
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
  uint8_t i, j, k;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    for (j = i; j < ARRAY_SIZE - i - 1; j++) //felfelé buborékol
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
    k = (uint8_t)(ARRAY_SIZE - i - 2);

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
    allapot = CLEAR;  
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
  
  lightupArray(); //felvillantjuk a tömb elemeit kékkel
  delay(DELAYVAL);
  uint8_t i, elementToInsert, j;
  pixels.setPixelColor(0, pixels.Color(GREEN)); //az első elem az fixen rendezett
  pixels.show();
  delay(DELAYVAL);

  for (i = 1; i < ARRAY_SIZE; i++) //a 0. elem az rendezett, ezért 1-től kezdjük a beszúrást
  {
    elementToInsert = tomb[i]; //kimentjük a beszúrandó elemet
    j = i - 1; //a beszúrandó elem előtti elem, a legutolsó rendezett rész eleme
    pixels.setPixelColor(i, pixels.Color(RED)); //beszúrásra kiválasztott elemet fevillantjuk pirossal
    pixels.show();
    delay(DELAYVAL);
    while (j >= 0 && tomb[j] > elementToInsert) //megkeressük a beszúrandó elem helyét a rendezett részen belül
    {
      
      pixels.setPixelColor(j+1, pixels.Color(GREEN)); 
      pixels.setPixelColor(j, pixels.Color(RED));
      pixels.show();
      tomb[j + 1] = tomb[j];
      j = (uint8_t)(j - 1);
      delay(DELAYVAL);
    }
    tomb[j + 1] = elementToInsert;
    pixels.setPixelColor(j+1, pixels.Color(GREEN)); //beszúrásra kiválasztott elemet fevillantjuk pirossal
    pixels.show();
    delay(DELAYVAL);
  }

    lcd.setCursor(0, 1);
    lcd.print("R:");
    printArr(tomb, ARRAY_SIZE); 
    delay(3000);  
    allapot = CLEAR;
}
#pragma endregion Rendezes

char i = 0;
char j = 0;
char* elso;
char* ketto;
char* temp;
char* temptomb = (char *)malloc(sizeof(char) * 3);

void arrayFillWithRC(char* input) //elmenti a rendezendő tömb elemeit
{
  //2 elemű string 
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
    ConvertArray();
    lcd.setCursor(0,1);
    printArr(tomb, ARRAY_SIZE);
    allapot = TOMBFELTOLTVE;
  } 
}

void receiverHandling2(){ //returns string "0"..."9", "OK"
  //allapot = TOMBFELTOLTES; - Felesleges szerintem
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
    allapot = TOMBFELTOLTVE;  
  }
}

//Kell ez még???
void ConvertArray(){
  	for (uint8_t i=0; i < ARRAY_SIZE; i++)
	{
		tomb[i] = tombTemp[i];  
	
	}
}
