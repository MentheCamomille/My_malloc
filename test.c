#include <stdio.h>
#include "my_malloc.h"

void test_simple_allocation() {
    int* arr = (int*)my_malloc(10 * sizeof(int));
    if (arr != NULL) {
        printf("Test d'allocation simple réussi\n");
        for (int i = 0; i < 10; i++) {
            arr[i] = i * i;
            printf("arr[%d] = %d\n", i, arr[i]);
        }
    } else {
        printf("Test d'allocation simple échoué\n");
    }
    my_free(arr);
}

void test_realloc() {
    int* arr = (int*)my_malloc(10 * sizeof(int));
    if (arr != NULL) {
        for (int i = 0; i < 10; i++) {
            arr[i] = i * i;
        }

        arr = (int*)my_realloc(arr, 20 * sizeof(int));
        if (arr != NULL) {
            printf("Test realloc réussi\n");
            for (int i = 10; i < 20; i++) {
                arr[i] = i * i;
                printf("arr[%d] = %d\n", i, arr[i]);
            }
        } else {
            printf("Test realloc échoué\n");
        }
    } else {
        printf("Allocation initiale échouée\n");
    }
    my_free(arr);
}

void test_best_fit() {
    int* arr1 = (int*)my_malloc_best_fit(10 * sizeof(int));
    int* arr2 = (int*)my_malloc_best_fit(20 * sizeof(int));
    int* arr3 = (int*)my_malloc_best_fit(30 * sizeof(int));

    if (arr1 != NULL && arr2 != NULL && arr3 != NULL) {
        printf("Test malloc best-fit réussi\n");
    } else {
        printf("Test malloc best-fit échoué\n");
    }

    my_free(arr1);
    my_free(arr2);
    my_free(arr3);
}

int main() {
    initialize_heap();
    test_simple_allocation();
    test_realloc();
    test_best_fit();
    return 0;
}
