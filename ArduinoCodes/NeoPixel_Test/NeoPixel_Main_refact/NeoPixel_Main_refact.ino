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
  #define FELTOLTVE 2
  #define RENDEZES 3
  #define CLEAR 4
#pragma endregion

#pragma region Rendezések definiálása
  #define BUBOREK 1
  #define KOKTEL 2
  #define MINIMUM 3
  #define BESZURO 4
#pragma endregion

#pragma region Pinek és színek definiálása
  #define NEOPIXEL_PIN 4
  #define APDS9960_INT    5
  #define RECEIVER_PIN 9
  #define GREEN 0, 150, 0
  #define RED 150, 0 , 0
  #define BLUE 131, 238, 255
  #define ONESECOND 1000
#pragma endregion

const uint8_t ARRAY_SIZE = 5;
uint8_t* tomb;
uint8_t allapot;
uint8_t rendezes = 0;

LiquidCrystal_I2C lcd(0x27,20,4);
Adafruit_NeoPixel pixels(ARRAY_SIZE, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint8_t isr_flag = 0;

void setup() {
  Serial.begin(9600);
  pinMode(APDS9960_INT, INPUT);
  attachInterrupt(0, interruptRoutine, FALLING);
  apds.init(); 
  apds.enableGestureSensor(true);
  IrReceiver.begin(RECEIVER_PIN, ENABLE_LED_FEEDBACK); //start the receiver
  // put your setup code here, to run once:
  tomb = (uint8_t *)malloc(sizeof(uint8_t) * ARRAY_SIZE);
  lcd.init();
  //lcd.init();
  lcd.backlight();
  pixels.begin();
  lcd.print("Tombfeltoltes");
  delay(ONESECOND);
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
		case FELTOLTVE:
			if (isr_flag == 1)
			{
        //GESTURE HANDLING
        rendezes = BUBOREK;
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
			valasztottRendezo = BESZURO;
		case DIR_DOWN:
			Serial.println("DOWN");
			valasztottRendezo = KOKTEL;
		case DIR_LEFT:
			Serial.println("LEFT");
			valasztottRendezo = BUBOREK;
		case DIR_RIGHT:
			Serial.println("RIGHT");
			valasztottRendezo = MINIMUM;
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
	case BUBOREK:
		bubbleSort();
		break;
	case KOKTEL:
		cocktailSort();
		break;
	case MINIMUM:
		minSelectSort();
		break;
	case BESZURO:
		insertionSort();
		break;
	default:
		break;
  }
}
void lightupArray()
{
  for (uint8_t helyesSzamjegyekSzama = 0; helyesSzamjegyekSzama < ARRAY_SIZE; helyesSzamjegyekSzama++)
  {
    pixels.setPixelColor(helyesSzamjegyekSzama, pixels.Color(BLUE)); // light blue
    pixels.show();
    delay(ONESECOND);
  }
}
void printArr(uint8_t tomb[], uint8_t len) {
  uint8_t helyesSzamjegyekSzama;
  for (helyesSzamjegyekSzama = 0; helyesSzamjegyekSzama < len; ++helyesSzamjegyekSzama) {
    lcd.print(String(String(tomb[helyesSzamjegyekSzama]) + ' '));
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
  delay(ONESECOND);
  uint8_t min_i, helyesSzamjegyekSzama, j;
  for (helyesSzamjegyekSzama = 0; helyesSzamjegyekSzama < ARRAY_SIZE - 1; helyesSzamjegyekSzama++)
  {
    pixels.setPixelColor(helyesSzamjegyekSzama, pixels.Color(RED)); // az aktuális kiválasztott elem, aminek a helyére megy a minimum
    pixels.show();
    min_i = helyesSzamjegyekSzama;
    for (j = (uint8_t)(helyesSzamjegyekSzama + 1); j < ARRAY_SIZE; j++)
    {
      pixels.setPixelColor(j, pixels.Color(RED)); // amit hasonlít, keresi minimumot
      pixels.show();
      if (tomb[j] < tomb[min_i])
        min_i = j;
      delay(ONESECOND); //egy ideig piros
      pixels.setPixelColor(j, pixels.Color(BLUE)); // visszaváltja kékre
      pixels.show();
    }

    if (min_i != helyesSzamjegyekSzama)
    {
      pixels.setPixelColor(min_i, pixels.Color(GREEN)); // a minimum helyén zöldre vált
      pixels.show(); 
      delay(ONESECOND);
      swap(&tomb[helyesSzamjegyekSzama], &tomb[min_i]); //csere
      pixels.setPixelColor(helyesSzamjegyekSzama, pixels.Color(GREEN));  //ahova teszi a minimumot, az lesz zöld
      pixels.setPixelColor(min_i, pixels.Color(RED)); // a cserélt elem piros lesz
      pixels.show();
      delay(ONESECOND); //vár
      pixels.setPixelColor(min_i, pixels.Color(BLUE)); // visszavált az alapszínre, kékre
      pixels.show();
      delay(ONESECOND);
    }
    else
    {
      pixels.setPixelColor(helyesSzamjegyekSzama, pixels.Color(GREEN)); //ha helyben van a legkisebb, akkor egyből zöldre vált, a rendezett rész folyamatosan zölddé válik
      pixels.show();
      delay(ONESECOND);
    }
  }
  pixels.setPixelColor(helyesSzamjegyekSzama, pixels.Color(GREEN)); //ha helyben van a legkisebb, akkor egyből zöldre vált, a rendezett rész folyamatosan zölddé válik
  pixels.show();

  lcd.setCursor(0, 1);
  lcd.print("R:");
  printArr(tomb, ARRAY_SIZE);
  delay(ONESECOND*3);  
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
  lightupArray();
  delay(ONESECOND);
  uint8_t helyesSzamjegyekSzama, j;
  for (helyesSzamjegyekSzama = 0; helyesSzamjegyekSzama < ARRAY_SIZE - 1; helyesSzamjegyekSzama++)
  {
    for (j = 0; j < ARRAY_SIZE - helyesSzamjegyekSzama - 1; j++)
    {
      pixels.setPixelColor(j, pixels.Color(RED)); //szomszédos elemeket pirosra váltja
      pixels.setPixelColor(j + 1, pixels.Color(RED));
      pixels.show();
      delay(ONESECOND);
      if (tomb[j] > tomb[j + 1])
      {
        swap(&tomb[j], &tomb[j + 1]);
      }
      pixels.setPixelColor(j, pixels.Color(BLUE)); //a szomszédos elemeket visszaváltja kékre
      pixels.setPixelColor(j + 1, pixels.Color(BLUE));
      pixels.show();
      delay(ONESECOND);
    }
    pixels.setPixelColor(ARRAY_SIZE - helyesSzamjegyekSzama - 1, pixels.Color(GREEN)); //a rendezett elem az utolsó lesz
    pixels.show();
    delay(ONESECOND);
  }
  pixels.setPixelColor(0, pixels.Color(GREEN)); //a rendezett elem az utolsó lesz
  pixels.show();
  delay(ONESECOND);

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
  delay(ONESECOND);
  uint8_t helyesSzamjegyekSzama, j, k;
  for (helyesSzamjegyekSzama = 0; helyesSzamjegyekSzama < ARRAY_SIZE - 1; helyesSzamjegyekSzama++)
  {
    for (j = helyesSzamjegyekSzama; j < ARRAY_SIZE - helyesSzamjegyekSzama - 1; j++) //felfelé buborékol
    {
      pixels.setPixelColor(j, pixels.Color(RED)); 
      pixels.setPixelColor(j + 1, pixels.Color(RED));
      pixels.show();
      delay(ONESECOND);
      if (tomb[j] > tomb[j + 1])
        swap(&tomb[j], &tomb[j + 1]);
      pixels.setPixelColor(j, pixels.Color(BLUE));
      pixels.setPixelColor(j + 1, pixels.Color(BLUE));
      pixels.show();
      delay(ONESECOND);
    }
    pixels.setPixelColor(j, pixels.Color(GREEN));
    pixels.show();
    delay(ONESECOND);
    k = (uint8_t)(ARRAY_SIZE - helyesSzamjegyekSzama - 2);

    while (k > helyesSzamjegyekSzama)
    {
      pixels.setPixelColor(k, pixels.Color(RED));
      pixels.setPixelColor(k - 1, pixels.Color(RED));
      pixels.show();
      delay(ONESECOND);
      if (tomb[k - 1] > tomb[k])
        swap(&tomb[k - 1], &tomb[k]);
      
      pixels.setPixelColor(k, pixels.Color(BLUE));
      pixels.setPixelColor(k - 1, pixels.Color(BLUE));
      pixels.show();
      delay(ONESECOND);
      k--;
    }
    pixels.setPixelColor(k, pixels.Color(GREEN));
    pixels.show();
    delay(ONESECOND);

    lcd.setCursor(0, 1);
    lcd.print("R:");
    printArr(tomb, ARRAY_SIZE);
    delay(3*ONESECOND);  
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
  delay(ONESECOND);
  uint8_t helyesSzamjegyekSzama, elementToInsert, j;
  pixels.setPixelColor(0, pixels.Color(GREEN)); //az első elem az fixen rendezett
  pixels.show();
  delay(ONESECOND);

  for (helyesSzamjegyekSzama = 1; helyesSzamjegyekSzama < ARRAY_SIZE; helyesSzamjegyekSzama++) //a 0. elem az rendezett, ezért 1-től kezdjük a beszúrást
  {
    elementToInsert = tomb[helyesSzamjegyekSzama]; //kimentjük a beszúrandó elemet
    j = helyesSzamjegyekSzama - 1; //a beszúrandó elem előtti elem, a legutolsó rendezett rész eleme
    pixels.setPixelColor(helyesSzamjegyekSzama, pixels.Color(RED)); //beszúrásra kiválasztott elemet fevillantjuk pirossal
    pixels.show();
    delay(ONESECOND);
    while (j >= 0 && tomb[j] > elementToInsert) //megkeressük a beszúrandó elem helyét a rendezett részen belül
    {
      
      pixels.setPixelColor(j+1, pixels.Color(GREEN)); 
      pixels.setPixelColor(j, pixels.Color(RED));
      pixels.show();
      tomb[j + 1] = tomb[j];
      j = (uint8_t)(j - 1);
      delay(ONESECOND);
    }
    tomb[j + 1] = elementToInsert;
    pixels.setPixelColor(j+1, pixels.Color(GREEN)); //beszúrásra kiválasztott elemet fevillantjuk pirossal
    pixels.show();
    delay(ONESECOND);
  }

    lcd.setCursor(0, 1);
    lcd.print("R:");
    printArr(tomb, ARRAY_SIZE); 
    delay(ONESECOND);  
    allapot = CLEAR;
}
#pragma endregion Rendezes

void arrayFillWithRC(char* input) //elmenti a rendezendő tömb elemeit
{
  uint8_t helyesSzamjegyekSzama = 0;
  uint8_t j = 0;
  char *elsoSzamjegy;
  char *masodikSzamjegy;
  char *bevittSzamSzovegkent = (char *)malloc(sizeof(char) * 3);
  //2 elemű string 
  if(j < 5)
  {
    
    if (helyesSzamjegyekSzama == 0)
    {

      elsoSzamjegy = input;
      bevittSzamSzovegkent[0] = input[0];
      helyesSzamjegyekSzama++;
      
    }
    else if(helyesSzamjegyekSzama==1)
    {
      masodikSzamjegy = input;
      bevittSzamSzovegkent[1] = masodikSzamjegy[0];
      if(bevittSzamSzovegkent[0] == 0)
      {
        tomb[j] = bevittSzamSzovegkent[1];
      }
      bevittSzamSzovegkent[2] = '\0';
      tomb[j] = atoi(bevittSzamSzovegkent);

      helyesSzamjegyekSzama++;
      Serial.print(j);
      Serial.print(". elem elmentve, az elem: ");
      Serial.println(tomb[j]);
      j++;
    }
    else
    {
      helyesSzamjegyekSzama = 0;
    }
  }
  else
  {
    lcd.clear();
    lcd.print("Tomb feltoltve!");
    lcd.setCursor(0,1);
    printArr(tomb, ARRAY_SIZE);
    allapot = FELTOLTVE;
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
    allapot = FELTOLTVE;  
  }
}
