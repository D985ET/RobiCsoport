#include "sortingAlgos.h"

int main()
{
    unsigned char a0[] = {1, 9, 7, 3, 5, 3, 5, 3, 2, 7};
    unsigned char len = sizeof(a0) / sizeof(a0[0]);
    unsigned char a1[10], a2[10], a3[10], a4[10];
    memcpy(a1, a0, len);
    memcpy(a2, a0, len);
    memcpy(a3, a0, len);
    memcpy(a4, a0, len);
    
    puts("\n-------Minimumkiválasztásos rendezés:-------\n");
    minSelectSort(a0, &len);

    puts("\n-------Buborékrendezés:-------\n");
    bubbleSort(a1, &len);

    puts("\n-------Koktélrendezés:-------\n");
    cocktailSort(a2, &len);

    puts("\n-------Beszúró rendezés:-------\n");
    insertionSort(a3, &len);

    puts("\n-------Shell-rendezés:-------\n");
    shellSort(a4, &len);
    
    return 0;
}
