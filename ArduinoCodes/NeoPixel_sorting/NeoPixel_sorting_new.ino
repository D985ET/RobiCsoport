#include <stdint.h>
#include <ctype.h>

#include <IRremote.hpp> //InfraRed Remote Controller
#include <LiquidCrystal.h>     //LED DISPLAY
#include <Adafruit_NeoPixel.h> //LED NeoPixel

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

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
#define GREEN 0x009600
#define RED 0x960000
#define BLUE 0x83EEFF


#pragma region SETTINGS
// globális változók
uint8_t *tomb; // tömböt kívül deklarálom, hogy ne kelljen minden metódusnak átadni paraméterként (egyszerűbb kód)


//konstansok
const uint8_t ARRAY_SIZE = 5; // Popular NeoPixel ring size

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
  tomb = (uint8_t *)malloc(sizeof(uint8_t) * ARRAY_SIZE);
  // 5 elemnek lefoglalunk 5 'uint8_t'-nyi (ARRAY_SIZE * 1 Byte) memóriát, 'tomb' változó ugyanúgy indexelhető
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
}

void printArr(uint8_t *array, uint8_t *len) // tetszőleges tömb kiíratása
{
  // kétjegyű szám: 3 helyet foglal el a kijelzőn (stringen) - 5*3 = 15 helyet foglal maximum a kijelző 1.sorában
  char *str = (char *)malloc(sizeof(char) * (*len) * 3);
  char currElement[3]; // egy elem mindig max. 3 karakter lesz
  uint8_t i;
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

uint8_t inputElement() // egyetlen kétjegyű szám bekérése, visszatér egy leendő tömbelemmel
{
  uint8_t accepted = 0;
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
  uint8_t arrNumber = atoi(element);
  free(element);
  return arrNumber; // egésszé alakított tömbelem
}

/* INFRARED REMOTE CONTROLLER 
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
*/

bool arrayFillWithRC() //elmenti egy ciklusban a rendezendő tömb elemeit
{
  for (uint8_t i = 0; i < ARRAY_SIZE; i++)
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

//A megadott indexű pixelt felvillantja a megfelelő rgb értékkel
void lightupPixel(uint8_t index, uint32_t rgb)
{
  pixels.setPixelColor(index, rgb); // az aktuális kiválasztott elem, aminek a helyére megy a minimum
  pixels.show();
}

// kék színnel felvillannak a tömb elemei, rendezőalgoritmus kezdete előtt
void lightupArray()
{
  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    lightupPixel(i, BLUE);
    delay(DELAYVAL);
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
  uint8_t *copy = (uint8_t *)malloc(sizeof(uint8_t) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);
  lightupArray();
  delay(DELAYVAL);
  uint8_t min_i, i, j;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    min_i = i;
    for (j = (uint8_t)(i + 1); j < ARRAY_SIZE; j++)
    {
      lightupPixel(j, RED); // az az elem, amivel aktuálisan hasonlít, keresi minimumot
      if (tomb[j] < tomb[min_i])
        min_i = j;
      delay(DELAYVAL); //egy ideig még piros
      lightupPixel(j, BLUE); // visszaváltja kékre
    }

    if (min_i != i)
    {
      lightupPixel(min_i, GREEN); // a minimum helyén zöldre vált
      delay(DELAYVAL);
      
      swap(&tomb[i], &tomb[min_i]); //csere
      
      pixels.setPixelColor(i, GREEN);  //ahova teszi a minimumot, az lesz zöld
      
      lightupPixel(min_i, RED); // a cserélt elem piros lesz
      delay(DELAYVAL);
      
      lightupPixel(min_i, BLUE); // visszavált az alapszínre, kékre
    }
    else
    {
      lightupPixel(i, GREEN); // ha helyben van a legkisebb, akkor egyből zöldre vált, a rendezett rész folyamatosan zölddé válik
    }
    delay(DELAYVAL);
  }

  lightupPixel(i, GREEN); // ha helyben van a legkisebb, akkor egyből zöldre vált, a rendezett rész folyamatosan zölddé válik

  // kijelző: 1.sor: rendezett tömb, 2.sor: az eredeti tömb másolata
  puts("Eredeti:");
  printArr(copy, ARRAY_SIZE);
  puts("Rendezett:");
  printArr(tomb, ARRAY_SIZE);
  free(copy);
}
void bubbleSort()
{
  uint8_t *copy = (uint8_t *)malloc(sizeof(uint8_t) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);
  lightupArray(); //felvillan minden kékkel
  delay(DELAYVAL);
  uint8_t i, j;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    for (j = 0; j < ARRAY_SIZE - i - 1; j++)
    {
      pixels.setPixelColor(j, RED); //szomszédos elemeket pirosra váltja
      lightupPixel(j+1, RED);
      delay(DELAYVAL);
      if (tomb[j] > tomb[j + 1])
      {
        swap(&tomb[j], &tomb[j + 1]);
      }
      pixels.setPixelColor(j, BLUE); //a szomszédos elemeket visszaváltja kékre
      lightupPixel(j + 1, BLUE);
      delay(DELAYVAL);
    }
    lightupPixel(j, GREEN); // a legnagyobb elem felbuborékolt, az már a helyén van (rendezett)
    delay(DELAYVAL);
  }
  lightupPixel(0, GREEN); //a 0. elemet már nem kell vizsgálni, az a végére automatikusan a helyére kerül, nyugodtan állíthatunk neki zöldet
  delay(DELAYVAL);
}
    
   
void cocktailSort()
{
  uint8_t *copy = (uint8_t *) malloc(sizeof(uint8_t) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);
  lightupArray();
  delay(DELAYVAL);
  uint8_t i, j, k;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    for (j = i; j < ARRAY_SIZE - i - 1; j++) //felfelé buborékol
    {
      pixels.setPixelColor(j, RED); // szomszédos elemeket pirosra váltja
      lightupPixel(j+1, RED);

      delay(DELAYVAL);
      if (tomb[j] > tomb[j + 1])
        swap(&tomb[j], &tomb[j + 1]);
      pixels.setPixelColor(j, BLUE); // szomszédos elemeket visszaváltja kékre
      lightupPixel(j + 1, BLUE); 
      delay(DELAYVAL);
    }
    lightupPixel(j, GREEN);
    delay(DELAYVAL);

    //ARRAY_SIZE - i - 2 miért? -> utolsó elemet levizsgáltuk (ARRAY_SIZE-i-1), nem zargatjuk
    for(k = ARRAY_SIZE - i - 2; k > i; k--) //visszafordul, de csak i-ig megy: sorozat elemei elölről is rendezetté válnak, nincs értelme mindig 0-ig vizsgálni
    {
      pixels.setPixelColor(k, RED);
      lightupPixel(k-1, RED);
      delay(DELAYVAL);
      if (tomb[k - 1] > tomb[k])
        swap(&tomb[k - 1], &tomb[k]);
      
      pixels.setPixelColor(k, BLUE);
      lightupPixel(k-1, BLUE);
      delay(DELAYVAL);
      k--;
    }
    lightupPixel(k, GREEN);
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
  uint8_t *copy = (uint8_t *) malloc(sizeof(uint8_t) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);
  lightupArray(); //felvillantjuk a tömb elemeit kékkel
  delay(DELAYVAL);
  uint8_t i, elementToInsert, j;
  lightupPixel(0, GREEN); // az első elem fixen rendezett
  delay(DELAYVAL);

  for (i = 1; i < ARRAY_SIZE; i++) //a 0. elem az rendezett, ezért 1-től kezdjük a beszúrást
  {
    elementToInsert = tomb[i]; //kimentjük a beszúrandó elemet
    j = i - 1; //a beszúrandó elem előtti elem, a legutolsó rendezett rész eleme
    lightupPixel(i, RED); // beszúrásra kiválasztott elemet fevillantjuk pirossal
    delay(DELAYVAL);
    while (j >= 0 && tomb[j] > elementToInsert) //megkeressük a beszúrandó elem helyét a rendezett részen belül
    {
      
      pixels.setPixelColor(j+1, GREEN); 
      lightupPixel(j, RED);

      tomb[j + 1] = tomb[j];
      j--;

      delay(DELAYVAL);
    }
    tomb[j + 1] = elementToInsert;
    lightupPixel(j + 1, GREEN); // beszúrásra kiválasztott elemet fevillantjuk pirossal
    delay(DELAYVAL);
  }

  puts("Eredeti:");
  printArr(copy, ARRAY_SIZE);
  puts("Rendezett:");
  printArr(tomb, ARRAY_SIZE);
  free(copy);
}
#pragma endregion Rendezes
