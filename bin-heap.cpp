#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>


const int MAX_SIZE = 10000000;
const int START_SIZE = 1000;
const int STEP = 100000;


typedef int data_t;

typedef struct {
    data_t *data;
    int size;
    int capacity;
} Heap;


double test(data_t *arr, int size);

int heap_top(Heap *heap, data_t *value);

int heap_pop(Heap *heap);

int heap_sift_up(Heap *heap, int index);

int heap_sift_down(Heap *heap, int index);




int main() {
    srand(time(NULL));

    FILE *output = fopen("bin-heap.time", "w");

    data_t *arr = (data_t *) calloc(MAX_SIZE, sizeof(data_t));

    for (int size = START_SIZE; size <= MAX_SIZE; size += STEP) {
        double avg_time = 0;

        for (int j = 1; j <= 5; j++) {
            for (int i = 0; i < size; i++) arr[i] = rand();
            
            avg_time += test(arr, size);
        }

        fprintf(output, "%.2f\n", avg_time / 5);
        fflush(output);
    }

    free(arr);

    fclose(output);

    return 0;
}




double test(data_t *arr, int size) {
    clock_t t1 = clock();
    /*
    for (int i = 0; i < size; i++) printf("%i ", arr[i]);
    putchar('\n');
    */   
    Heap heap = {arr, size, size};

    for (int i = size / 2; i >= 0; i--)
        heap_sift_down(&heap, i);

    for (int i = 0; i < size; i++) {
        data_t max = 0;
        heap_top(&heap, &max);
        heap_pop(&heap);
        arr[size - i - 1] = max;
    }
    /*
    for (int i = 0; i < size; i++) printf("%i ", arr[i]);
    putchar('\n');
    */
    clock_t t2 = clock();
    return 1000.0 * (t2 - t1) / CLOCKS_PER_SEC;
}


int heap_top(Heap *heap, data_t *value) {
    *value = heap -> data[0];

    return 0;
}


int heap_pop(Heap *heap) {
    (heap -> data)[0] = (heap -> data)[--heap -> size];
    (heap -> data)[heap -> size] = 0;

    if (heap -> size) heap_sift_down(heap, 0);

    return 0;
}


int heap_sift_down(Heap *heap, int index) {
    while (index < heap -> size) {
        int left = 2 * index + 1, right = 2 * index + 2;

        int max = index;

        if (left < heap -> size)
            max = ((heap -> data)[left] > (heap -> data)[max]) ? left: max;
        
        if (right < heap -> size)
            max = ((heap -> data)[right] > (heap -> data)[max]) ? right : max;
        
        if (max == index) {
            return 0;
        }
        else {
            data_t swap = (heap -> data)[index];
            (heap -> data)[index] = (heap -> data)[max];
            (heap -> data)[max] = swap;

            index = max;
        } 
    }

    return 0;
}
