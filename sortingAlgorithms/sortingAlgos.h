#if !defined(SORTING_ALGOS_H)
#define SORTING_ALGOS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void swap(unsigned char *x1, unsigned char *x2);
void minSelectSort(unsigned char *A, unsigned char *length);
void bubbleSort(unsigned char *A, unsigned char *length);
void cocktailSort(unsigned char *A, unsigned char *length);
void insertionSort(unsigned char *A, unsigned char *length);
void shellSort(unsigned char *A, unsigned char *length);

#endif // SORTING_ALGOS_H

