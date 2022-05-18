#include "Adafruit_NeoPixel.h"
#include <ctype.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?

#pragma region SETTINGS
//globális változók
unsigned char* tomb; //tömböt kívül deklarálom, hogy ne kelljen minden metódusnak átadni paraméterként (egyszerűbb kód)

//pinkiosztások
#define PIN  13 

//konstansok
#define ARRAY_SIZE 10 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(ARRAY_SIZE, PIN, NEO_GRB + NEO_KHZ800);

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

void swap(unsigned char *x1, unsigned char *x2)
{
    unsigned char temp = *x1;
    *x1 = *x2;
    *x2 = temp;
}

void printArr(unsigned char* array, unsigned char* len) //tetszőleges tömb kiíratása
{
    //kétjegyű szám: 3 helyet foglal el a kijelzőn (stringen) - 5*3 = 15 helyet foglal maximum a kijelző 1.sorában
    char *str = (char*) malloc(sizeof(char) * (*len) * 3);
    char currElement[3]; //egy elem mindig max. 3 karakter lesz
    unsigned char i;
    for (i = 0; i < ARRAY_SIZE - 1; i++){
    sprintf(currElement, "%d, ", array[i]);
    strcat(str, currElement);
    }
  sprintf(currElement, "%d\0", array[i]); // utolsó szám + \0 karakter
  strcat(str, currElement);

    //kijelző: string kiírása a kijelzőre
    printf("%s\n", str);
    free(str);
}

//kék színnel felvillannak a tömb elemei, rendezőalgoritmus kezdete előtt
void lightupArray()
{
  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    pixels.setPixelColor(i, pixels.Color(237, 28, 36)); // light blue
    pixels.show();
    delay(DELAYVAL);
  }
}

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

bool arrayFillWithRC() //elmenti egy ciklusban a rendezendő tömb elemeit
{
  for (unsigned char i = 0; i < ARRAY_SIZE; i++)
    tomb[i] = inputElement();
  return true;
  //mikor lesz hamis a tömbfeltöltés?
}

//RENDEZŐALGORITMUSOK
void minSelectSort()
{
  unsigned char *copy = (unsigned char *)malloc(sizeof(unsigned char) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);

  unsigned char min_i, i, j;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    pixels.setPixelColor(i, pixels.Color(237, 28, 36)); // red
    pixels.show();
    min_i = i;
    for (j = (unsigned char)(i + 1); j < ARRAY_SIZE; j++)
    {
      pixels.setPixelColor(j, pixels.Color(237, 28, 36)); // red
      pixels.show();
      if (tomb[j] < tomb[min_i])
        min_i = j;
      delay(DELAYVAL);
      pixels.setPixelColor(j, pixels.Color(131, 238, 255)); // light blue
      pixels.show();
    }

    if (min_i != i)
    {
      pixels.setPixelColor(min_i, pixels.Color(0, 150, 0)); // green
      pixels.show();
      delay(DELAYVAL);
      swap(&tomb[i], &tomb[min_i]);
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));  // green
      pixels.setPixelColor(min_i, pixels.Color(237, 28, 36)); // red
      pixels.show();
      delay(DELAYVAL);
      pixels.setPixelColor(min_i, pixels.Color(131, 238, 255)); // light blue
      pixels.show();
      delay(DELAYVAL);
    }
    else
    {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // green
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

  unsigned char i, j;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    for (j = 0; j < ARRAY_SIZE - i - 1; j++)
    {
      pixels.setPixelColor(j, pixels.Color(237, 28, 36));
      pixels.setPixelColor(j + 1, pixels.Color(237, 28, 36));
      pixels.show();
      delay(DELAYVAL);
      if (tomb[j] > tomb[j + 1])
      {
        swap(&tomb[j], &tomb[j + 1]);
        pixels.setPixelColor(j, pixels.Color(131, 238, 255));
        pixels.setPixelColor(j + 1, pixels.Color(131, 238, 255));
        pixels.show();
        delay(DELAYVAL);
      }
      else
      {
        pixels.setPixelColor(j, pixels.Color(131, 238, 255));
        pixels.setPixelColor(j + 1, pixels.Color(131, 238, 255));
        pixels.show();
        delay(DELAYVAL);
      }
    }
    pixels.setPixelColor(j + 1, pixels.Color(0, 150, 0));
    pixels.show();
    delay(DELAYVAL);
  }

  puts("Eredeti:");
  printArr(copy, ARRAY_SIZE);
  puts("Rendezett:");
  printArr(tomb, ARRAY_SIZE);
  free(copy);
}
void cocktailSort()
{
  unsigned char *copy = malloc(sizeof(unsigned char) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);

  unsigned char i, j, k;
  for (i = 0; i < ARRAY_SIZE - 1; i++)
  {
    for (j = 0; j < ARRAY_SIZE - i - 1; j++)
    {
      pixels.setPixelColor(j, pixels.Color(237, 28, 36));
      pixels.setPixelColor(j + 1, pixels.Color(237, 28, 36));
      pixels.show();
      delay(DELAYVAL);
      if (tomb[j] > tomb[j + 1])
        swap(&tomb[j], &tomb[j + 1]);
      pixels.setPixelColor(j, pixels.Color(131, 238, 255));
      pixels.setPixelColor(j + 1, pixels.Color(131, 238, 255));
      pixels.show();
      delay(DELAYVAL);
    }
    pixels.setPixelColor(j + 1, pixels.Color(0, 150, 0));
    pixels.show();
    delay(DELAYVAL);
    k = (unsigned char)(ARRAY_SIZE - i - 1);

    while (k > 1 + i)
    {
      pixels.setPixelColor(k, pixels.Color(237, 28, 36));
      pixels.setPixelColor(k - 1, pixels.Color(237, 28, 36));
      pixels.show();
      delay(DELAYVAL);
      if (tomb[k - 1] > tomb[k])
        swap(&tomb[k - 1], &tomb[k]);
      k--;
      pixels.setPixelColor(k, pixels.Color(131, 238, 255));
      pixels.setPixelColor(k - 1, pixels.Color(131, 238, 255));
      pixels.show();
      delay(DELAYVAL);
    }
    pixels.setPixelColor(k - 1, pixels.Color(0, 150, 0));
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
  unsigned char *copy = malloc(sizeof(unsigned char) * ARRAY_SIZE);
  memcpy(copy, tomb, ARRAY_SIZE);

  unsigned char i, elementToInsert, j;
  for (i = 1; i < ARRAY_SIZE; i++)
  {
    elementToInsert = tomb[i];
    j = i - 1;
    pixels.setPixelColor(elementToInsert, pixels.Color(237, 28, 36));
    pixels.show();
    delay(DELAYVAL);
    while (j >= 0 && tomb[j] > elementToInsert)
    {
      //TODO
      tomb[j + 1] = tomb[j];
      j = (unsigned char)(j - 1);
    }
    tomb[j + 1] = elementToInsert;
  }

  puts("Eredeti:");
  printArr(copy, ARRAY_SIZE);
  puts("Rendezett:");
  printArr(tomb, ARRAY_SIZE);
  free(copy);
}
