#include <IRremote.hpp> //InfraRed Remote Controller
#include <LiquidCrystal.h>     //LED DISPLAY
#include <Adafruit_NeoPixel.h> //LED NeoPixel
#include <ctype.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#pragma region SETTINGS
//globális változók
unsigned char* tomb; //tömböt kívül deklarálom, hogy ne kelljen minden metódusnak átadni paraméterként (egyszerűbb kód)

//pinkiosztások
#define NEOPIXEL_PIN  13
#define RECEIVER_PIN 9
//LED-display-pins
#define en 2
#define rs 3
#define d4 7
#define d5 6
#define d6 5
#define d7 4
#define GREEN 0, 150, 0
#define RED 150, 0 , 0
#define BLUE 131, 238, 255
//konstansok
const unsigned char ARRAY_SIZE = 5; // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(ARRAY_SIZE, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define DELAYVAL 1000 // Time (in milliseconds) to pause between pixels

#pragma endregion

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  tomb = (unsigned char *)malloc(sizeof(unsigned char) * ARRAY_SIZE);
  // 5 elemnek lefoglalunk 5 'unsigned char'-nyi (ARRAY_SIZE * 1 Byte) memóriát, 'tomb' változó ugyanúgy indexelhető
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
}

void printArr(unsigned char *array, unsigned char *len) // tetszőleges tömb kiíratása
{
  // kétjegyű szám: 3 helyet foglal el a kijelzőn (stringen) - 5*3 = 15 helyet foglal maximum a kijelző 1.sorában
  char *str = (char *)malloc(sizeof(char) * (*len) * 3);
  char currElement[3]; // egy elem mindig max. 3 karakter lesz
  unsigned char i;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    sprintf(currElement, "%d, ", array[i]);
    strcat(str, currElement);
  }
  sprintf(currElement, "%d\0", array[i]); // utolsó szám + \0 karakter
  strcat(str, currElement);

  // kijelző: string kiírása a kijelzőre
  printf("%s\n", str);
  free(str);
}

#pragma region TombFeltoltes

unsigned char inputElement() // egyetlen kétjegyű szám bekérése, visszatér egy leendő tömbelemmel
{
  unsigned char accepted = 0;
  char first[2];
  first[1] = '\0';

  char *element = (char *)malloc(sizeof(char) * 3);
  char remoteValue[2]; // távirányító dekódolt értékei lehetnek: "1" ... "9", "OK", "ND" (OK: elfogad, ND: not defined)
  do           // amíg a helyes számjegyek el nem érik a kettőt
  {
    printf("%d. számjegy: ", accepted + 1); // kijelzőre
    scanf("%s", remoteValue);
    first[0] = remoteValue[0];
    if (isdigit(first[0]))
    {
      accepted += 1;
      strcat(element, first);
      printf("%s\n", element); // kijelzőre
    }
    else if (strcmp(remoteValue, "OK") == 0) // ha OK-gombot nyom
    {
      if (accepted == 0) // ha nincs helyes számjegy, és OK gombot nyom -> nem adott meg számot -> folytassa a bekérést
      {
        puts("Nem adott meg számot");
        continue; // folytassa a ciklust
      }
      else if (accepted == 1) // ha 1 helyes számjegy már van, és OK gombot nyom -> egyjegyű
        break;
    }
    else
    {
      puts("Ez nem számjegy!");
      continue;
    }
  } while (accepted < 2);
  unsigned char arrNumber = atoi(element);
  free(element);
  return arrNumber; // egésszé alakított tömbelem
}

char* receiverHandling(){ //returns string "0"..."9", "ND", "OK"
    //TODO: determine, which HEX-code belongs to which value
    if (IrReceiver.decode())
    {
      switch (IrReceiver.decodedIRData.decodedRawData)
      {
      case 0xF30CBF00:
        Serial.println("0");
        break; // Button 0
      case 0xEF10BF00:
        Serial.println("1");
        break; // Button 1
      case 0xEE11BF00:
        Serial.println("2");
        break; // Button 2
      case 0xED12BF00:
        Serial.println("3");
        break; // Button 3
      case 0xEB14BF00:
        Serial.println("4");
        break; // Button 4
      case 0xEA15BF00:
        Serial.println("5");
        break; // Button 5
      case 0xE916BF00:
        Serial.println("6");
        break; // Button 6
      case 0xE718BF00:
        Serial.println("7");
        break; // Button 7
      case 0xE619BF00:
        Serial.println("8");
        break; // Button 8
      case 0xE51ABF00:
        Serial.println("9");
        break; // Button 9
      case 0xFD02BF00:
        return "OK";
        break;
      default:
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        break;
      }
      IrReceiver.resume();
    }
}

bool arrayFillWithRC() //elmenti egy ciklusban a rendezendő tömb elemeit
{
  for (unsigned char i = 0; i < ARRAY_SIZE; i++)
    tomb[i] = inputElement();
  return true;
  //mikor lesz hamis a tömbfeltöltés?
}

#pragma endregion

#pragma region Kivalaszt1

void welcome()
{
  Serial.begin(9600);
  IrReceiver.begin(RECEIVER_PIN, ENABLE_LED_FEEDBACK); // start the receiver
  lcd.begin(16, 2);

  lcd.setCursor(0, 0); // 1.sorba ír
  lcd.print("Rendezes");
  delay(1000);
  lcd.setCursor(0, 1); // 2.sorba ír
  delay(1000);
  lcd.print("Tombfeltoltes");
}

#pragma endregion

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

void swap(unsigned char *x1, unsigned char *x2)
{
  unsigned char temp = *x1;
  *x1 = *x2;
  *x2 = temp;
}
void minSelectSort()
{
  unsigned char *copy = (unsigned char *)malloc(sizeof(unsigned char) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);
  lightupArray();
  delay(DELAYVAL);
  unsigned char min_i, i, j;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    pixels.setPixelColor(i, pixels.Color(RED)); // az aktuális kiválasztott elem, aminek a helyére megy a minimum
    pixels.show();
    min_i = i;
    for (j = (unsigned char)(i + 1); j < ARRAY_SIZE; j++)
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

  // kijelző: 1.sor: rendezett tömb, 2.sor: az eredeti tömb másolata
  puts("Eredeti:");
  printArr(copy, ARRAY_SIZE);
  puts("Rendezett:");
  printArr(tomb, ARRAY_SIZE);
  free(copy);
}
void bubbleSort()
{
  unsigned char *copy = (unsigned char *)malloc(sizeof(unsigned char) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);
  lightupArray(); //felvillan minden kékkel
  delay(DELAYVAL);
  unsigned char i, j;
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
}    
   
void cocktailSort()
{
  unsigned char *copy = (unsigned char *) malloc(sizeof(unsigned char) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);
  lightupArray();
  delay(DELAYVAL);
  unsigned char i, j, k;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    for (j = 0; j < ARRAY_SIZE - i - 1; j++) //felfelé buborékol
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
    pixels.setPixelColor(j + 1, pixels.Color(GREEN));
    pixels.show();
    delay(DELAYVAL);
    k = (unsigned char)(ARRAY_SIZE - i - 1);

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
  }

  puts("Eredeti:");
  printArr(copy, ARRAY_SIZE);
  puts("Rendezett:");
  printArr(tomb, ARRAY_SIZE);
  free(copy);
}
void insertionSort()
{
  unsigned char *copy = (unsigned char *) malloc(sizeof(unsigned char) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);
  lightupArray(); //felvillantjuk a tömb elemeit kékkel
  delay(DELAYVAL);
  unsigned char i, elementToInsert, j;
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
      j = (unsigned char)(j - 1);
      delay(DELAYVAL);
    }
    tomb[j + 1] = elementToInsert;
    pixels.setPixelColor(j+1, pixels.Color(GREEN)); //beszúrásra kiválasztott elemet fevillantjuk pirossal
    pixels.show();
    delay(DELAYVAL);
  }

  puts("Eredeti:");
  printArr(copy, ARRAY_SIZE);
  puts("Rendezett:");
  printArr(tomb, ARRAY_SIZE);
  free(copy);
}
#pragma endregion Rendezes
