#include <stdlib.h>

#include "matrix.h"

int** create_matrix(int rows, int columns)
{
    int** arr = (int**)calloc(rows, sizeof(int*));

    for (int i = 0; i < rows; i++)
    {
        arr[i] = (int*)calloc(columns, sizeof(int));
    }

    return arr;
}

int** destroy_matrix(int** arr, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        free(arr[i]);
        arr[i] = NULL;
    }

    free(arr);
    arr = NULL;
    
    return arr;
}