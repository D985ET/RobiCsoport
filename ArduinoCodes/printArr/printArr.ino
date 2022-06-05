#include <LiquidCrystal_I2C.h>
#include <ctype.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Tombfeltoltes");
}

void loop() {
}
/***
 * Print the array to the LCD1602 as string
 * @param A is the array
 * @param N is the size
 */
void printArr(uint8_t *A, uint8_t *N)
{
    char* str = (char*) malloc(sizeof(char) * (*N) * 3);
    char* currElement = (char*) malloc(sizeof(char) * 3);
    uint8_t i;
    strcpy(str, "[");
    for (i = 0; i < *N - 1; i++)
    {
        sprintf(currElement, "%d, ", A[i]);
        strcat(str, currElement);
    }
    sprintf(currElement, "%d]", A[i]);
    strcat(str, currElement);
    lcd.print(str);
    free(str);
}

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

char* receiverHandling(){ //returns string "0"..."9", "ND", "OK"
  //TODO: determine, which HEX-code belongs to which value
  if (IrReceiver.decode())
  {
    switch (IrReceiver.decodedIRData.decodedRawData)
    {
    case 0xF30CBF00: Serial.println("0"); break; // Button 0
    case 0xEF10BF00: Serial.println("1"); break; // Button 1
    case 0xEE11BF00: Serial.println("2"); break; // Button 2
    case 0xED12BF00: Serial.println("3"); break; // Button 3
    case 0xEB14BF00: Serial.println("4"); break; // Button 4
    case 0xEA15BF00: Serial.println("5"); break; // Button 5
    case 0xE916BF00: Serial.println("6"); break; // Button 6
    case 0xE718BF00: Serial.println("7"); break; // Button 7
    case 0xE619BF00: Serial.println("8"); break; // Button 8
    case 0xE51ABF00: Serial.println("9"); break; // Button 9
    case 0xFD02BF00: return "OK"; break;
    default: Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); break;
    }
    IrReceiver.resume();
  }
}

bool arrayFillWithRC() //elmenti egy ciklusban a rendezendő tömb elemeit
{
  for (uint8_t i = 0; i < ARRAY_SIZE; i++)
    tomb[i] = inputElement();
  return true;
  //mikor lesz hamis a tömbfeltöltés?
}