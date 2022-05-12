void setup() {
  
}

void loop() {
  
}

/***
 * @TODO read the data from remote
 * Checks for input memory addresses 
 * from the IR Remote
 */
void recieveHandling() {
  if(IrReceiver.decode()){
    switch(IrReceiver.decodedIRData.decodedRawData)
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
        case 0xFD02BF00: Serial.println("STOP"); break;
        default: Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); break;
    }
    IrReceiver.resume();
  }
}

/***
 * Print the array to the LCD1602 as string
 * @param A is the array
 * @param N is the size
 */
void printArr(unsigned char *A, unsigned char *N)
{
    char* str = (char) malloc(sizeof(char) * (*N) * 6);
    char* currElement[6];
    unsigned char i;
    strcpy(str, "[");
    for (i = 0; i < *N - 1; i++)
    {
        sprintf(currElement, "%d, ", A[i]);
        strcat(str, currElement);
    }
    sprintf(currElement, "%d]\n", A[i]);
    strcat(str, currElement);
    lcd.print("%s\n", str);
    free(str);
}
