#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        13 

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 10 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 1000 // Time (in milliseconds) to pause between pixels


void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  int tomb[10] = {9, 4, 5, 3, 2, 1, 7, 8, 6, 12};
  //metódus hívás

}

void swap(unsigned char *x1, unsigned char *x2)
{
    unsigned char temp = *x1;
    *x1 = *x2;
    *x2 = temp;
}

void printArr(unsigned char *A, unsigned char *N)
{
    char *str = (char*) malloc(sizeof(char) * (*N) * 6);
    char currElement[6];
    unsigned char i;
    strcpy(str, "[");
    for (i = 0; i < *N - 1; i++)
    {
        sprintf(currElement, "%d, ", A[i]);
        strcat(str, currElement);
    }
    sprintf(currElement, "%d]\n", A[i]);
    strcat(str, currElement);
    printf("%s\n", str);
    free(str);
}

void minSelectSort(unsigned char *A, unsigned char *N)
{
    for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(131, 238, 255)); //light blue
    pixels.show();
    delay(DELAYVAL);
    }
    
    unsigned char *copy = malloc(sizeof(unsigned char) * (*N));
    memcpy(copy, A, *N);

    unsigned char min_i, i, j;
    for (i = 0; i < (*N) - 1; i++)
    {
        pixels.setPixelColor(i, pixels.Color(237, 28, 36)); // red
        pixels.show();
        min_i = i;
        for (j = (unsigned char)(i+1); j < *N; j++)
        {
            pixels.setPixelColor(j, pixels.Color(237, 28, 36)); // red
            pixels.show();
            if (A[j] < A[min_i])
                min_i = j;
            delay(DELAYVAL);
            pixels.setPixelColor(j, pixels.Color(131, 238, 255)); //light blue
            pixels.show();

        }

        if (min_i != i){
          pixels.setPixelColor(min_i, pixels.Color(0, 150, 0)); // green
          pixels.show();
          delay(DELAYVAL);
          swap(&A[i], &A[min_i]);
          pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // green 
          pixels.setPixelColor(min_i, pixels.Color(237, 28, 36)); // red
          pixels.show();
          delay(DELAYVAL);
          pixels.setPixelColor(min_i, pixels.Color(131, 238, 255)); // light blue
          pixels.show();
          delay(DELAYVAL);
        }
        else{
          pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // green
          pixels.show();
          delay(DELAYVAL);
        }
    }

    puts("Eredeti:");
    printArr(copy, N);
    puts("Rendezett:");
    printArr(A, N);
    free(copy);
}

void bubbleSort(unsigned char *A, unsigned char *N)
{
    for(int i=0; i<NUMPIXELS; i++) { 
      pixels.setPixelColor(i, pixels.Color(131, 238, 255));
      pixels.show();
      delay(DELAYVAL);
    }
    
    unsigned char *copy = malloc(sizeof(unsigned char) * (*N));
    memcpy(copy, A, *N);

    unsigned char i, j;
    for (i = 0; i < *N - 1; i++){
        for (j = 0; j < *N - i - 1; j++){
          pixels.setPixelColor(j, pixels.Color(237, 28, 36));
          pixels.setPixelColor(j+1, pixels.Color(237, 28, 36));
          pixels.show();
          delay(DELAYVAL);
            if (A[j] > A[j + 1]){
              swap(&A[j], &A[j + 1]);
              pixels.setPixelColor(j, pixels.Color(131, 238, 255));
              pixels.setPixelColor(j+1, pixels.Color(131, 238, 255));
              pixels.show();
              delay(DELAYVAL);
            }
            else{
              pixels.setPixelColor(j, pixels.Color(131, 238, 255));
              pixels.setPixelColor(j+1, pixels.Color(131, 238, 255));
              pixels.show();
              delay(DELAYVAL);
            }
        }
        pixels.setPixelColor(j+1, pixels.Color(0, 150, 0));
        pixels.show();
        delay(DELAYVAL);
    }
                

    puts("Eredeti:");
    printArr(copy, N);
    puts("Rendezett:");
    printArr(A, N);
    free(copy);
}

void cocktailSort(unsigned char *A, unsigned char *N)
{

  for(int i=0; i<NUMPIXELS; i++) { 
      pixels.setPixelColor(i, pixels.Color(131, 238, 255));
      pixels.show();
      delay(DELAYVAL);
    }
    
    unsigned char *copy = malloc(sizeof(unsigned char) * (*N));
    memcpy(copy, A, *N);
    
    unsigned char k;
    for (unsigned char i = 0; i < *N - 1; i++)
    {
        for (unsigned char j = 0; j < *N - i - 1; j++)
        {
            pixels.setPixelColor(j, pixels.Color(237, 28, 36));
            pixels.setPixelColor(j+1, pixels.Color(237, 28, 36));
            pixels.show();
            delay(DELAYVAL);
            if (A[j] > A[j + 1])
                swap(&A[j], &A[j + 1]);
            pixels.setPixelColor(j, pixels.Color(131, 238, 255));
            pixels.setPixelColor(j+1, pixels.Color(131, 238, 255));
            pixels.show();
            delay(DELAYVAL);
        }
        pixels.setPixelColor(j+1, pixels.Color(0, 150, 0));
        pixels.show();
        delay(DELAYVAL);
        k = (unsigned char)(*N - i - 1);

        while (k > 1 + i)
        {
            pixels.setPixelColor(k, pixels.Color(237, 28, 36));
            pixels.setPixelColor(k-1, pixels.Color(237, 28, 36));
            pixels.show();
            delay(DELAYVAL);
            if (A[k - 1] > A[k])
                swap(&A[k - 1], &A[k]);
            k--;
            pixels.setPixelColor(k, pixels.Color(131, 238, 255));
            pixels.setPixelColor(k-1, pixels.Color(131, 238, 255));
            pixels.show();
            delay(DELAYVAL);
        }
        pixels.setPixelColor(k-1, pixels.Color(0, 150, 0));
        pixels.show();
        delay(DELAYVAL);
    }

    puts("Eredeti:");
    printArr(copy, N);
    puts("Rendezett:");
    printArr(A, N);
    free(copy);
}

void insertionSort(unsigned char *A, unsigned char *N)
{
    for(int i=0; i<NUMPIXELS; i++) { 
      pixels.setPixelColor(i, pixels.Color(131, 238, 255));
      pixels.show();
      delay(DELAYVAL);
    }
    unsigned char *copy = malloc(sizeof(unsigned char) * (*N));
    memcpy(copy, A, *N);

    unsigned char i, elementToInsert, j;
    for (i = 1; i < *N; i++)
    {
        elementToInsert = A[i];
        j = i - 1;
        pixels.setPixelColor(elementToInsert, pixels.Color(237, 28, 36));
        pixels.show();
        delay(DELAYVAL);
        while (j >= 0 && A[j] > elementToInsert)
        {
          //TODO
            A[j + 1] = A[j];
            j = (unsigned char) (j - 1);
        }
        A[j + 1] = elementToInsert;
    }

    puts("Eredeti:");
    printArr(copy, N);
    puts("Rendezett:");
    printArr(A, N);
    free(copy);
}

void shellSort(unsigned char *A, unsigned char *N)
{
  //TODO
    for(int i=0; i<NUMPIXELS; i++) { 
      pixels.setPixelColor(i, pixels.Color(131, 238, 255));
      pixels.show();
      delay(DELAYVAL);
    }
    unsigned char *copy = malloc(sizeof(unsigned char) * (*N));
    memcpy(copy, A, *N);

    for (unsigned char gap = *N / 2; gap > 0; gap /= 2)
    {
        for (unsigned char i = gap; i < *N; i += 1)
        {
            unsigned char temp = A[i];

            unsigned char j;
            for (j = i; j >= gap && A[j - gap] > temp; j -= gap)
                A[j] = A[j - gap];

            A[j] = temp;
        }
    }

    puts("Eredeti:");
    printArr(copy, N);
    puts("Rendezett:");
    printArr(A, N);
    free(copy);
}
