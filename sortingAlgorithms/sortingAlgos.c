#include "sortingAlgos.h"

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
    unsigned char *copy = malloc(sizeof(unsigned char) * (*N));
    memcpy(copy, A, *N);

    unsigned char min_i, i, j;
    for (i = 0; i < (*N) - 1; i++)
    {
        min_i = i;
        for (j = (unsigned char)(i+1); j < *N; j++)
        {
            if (A[j] < A[min_i])
                min_i = j;
        }

        if (min_i != i)
            swap(&A[i], &A[min_i]);
    }

    puts("Eredeti:");
    printArr(copy, N);
    puts("Rendezett:");
    printArr(A, N);
    free(copy);
}

void bubbleSort(unsigned char *A, unsigned char *N)
{
    unsigned char *copy = malloc(sizeof(unsigned char) * (*N));
    memcpy(copy, A, *N);

    unsigned char i, j;
    for (i = 0; i < *N - 1; i++)
        for (j = 0; j < *N - i - 1; j++)
            if (A[j] > A[j + 1])
                swap(&A[j], &A[j + 1]);

    puts("Eredeti:");
    printArr(copy, N);
    puts("Rendezett:");
    printArr(A, N);
    free(copy);
}

void cocktailSort(unsigned char *A, unsigned char *N)
{
    unsigned char *copy = malloc(sizeof(unsigned char) * (*N));
    memcpy(copy, A, *N);
    
    unsigned char k;
    for (unsigned char i = 0; i < *N - 1; i++)
    {
        for (unsigned char j = 0; j < *N - i - 1; j++)
        {
            if (A[j] > A[j + 1])
                swap(&A[j], &A[j + 1]);
        }
        k = (unsigned char)(*N - i - 1);

        while (k > 1 + i)
        {
            if (A[k - 1] > A[k])
                swap(&A[k - 1], &A[k]);
            k--;
        }
    }

    puts("Eredeti:");
    printArr(copy, N);
    puts("Rendezett:");
    printArr(A, N);
    free(copy);
}

void insertionSort(unsigned char *A, unsigned char *N)
{
    unsigned char *copy = malloc(sizeof(unsigned char) * (*N));
    memcpy(copy, A, *N);

    unsigned char i, elementToInsert, j;
    for (i = 1; i < *N; i++)
    {
        elementToInsert = A[i];
        j = i - 1;

        while (j >= 0 && A[j] > elementToInsert)
        {
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