#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>     //LED DISPLAY
#include <Adafruit_NeoPixel.h> //LED NeoPixel

LiquidCrystal_I2C lcd(0x27,20,4); 
int tomb[] = {9, 7, 5, 3, 1};
#define NEOPIXEL_PIN 4

#define GREEN 0, 150, 0
#define RED 150, 0 , 0
#define BLUE 131, 238, 255

const int ARRAY_SIZE = 5;
Adafruit_NeoPixel pixels(ARRAY_SIZE, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 1000

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.init();
  lcd.backlight();
  pixels.begin();
  insertionSort();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

#pragma region Rendezes
// kék színnel felvillannak a tömb elemei, rendezőalgoritmus kezdete előtt
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
    pixels.setPixelColor(i, pixels.Color(RED)); // az aktuális kiválasztott elem, aminek a helyére megy a minimum
    pixels.show();
    min_i = i;
    for (j = (int)(i + 1); j < ARRAY_SIZE; j++)
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
}
void bubbleSort()
{
  lcd.setCursor(0, 0);
  lcd.print("E:");
  printArr(tomb, ARRAY_SIZE);
  lcd.setCursor(0, 1);
  lcd.print("Calculating...");
  /*unsigned char *copy = (unsigned char *)malloc(sizeof(unsigned char) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);*/
  lightupArray(); //felvillan minden kékkel
  delay(DELAYVAL);
  int i, j;
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
}    
   
void cocktailSort()
{
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
  }
}
void insertionSort()
{
  lcd.setCursor(0, 0);
  lcd.print("E:");
  printArr(tomb, ARRAY_SIZE);
  lcd.setCursor(0, 1);
  lcd.print("Calculating...");
  
  lightupArray(); //felvillantjuk a tömb elemeit kékkel
  delay(DELAYVAL);
  int i, elementToInsert, j;
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
      j = (int)(j - 1);
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
}
#pragma endregion Rendezes