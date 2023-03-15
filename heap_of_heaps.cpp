#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>


const int BASE = 100;


const int MAX_SIZE = 10000000;
const int START_SIZE = 1000;
const int STEP = 100000;
const int TESTS = 5;


typedef int data_t;


typedef struct {
    int parent;
    int keys[BASE];
} Node;


typedef struct {
    data_t *data;
    Node *nodes;
    int size;
    int capacity;
    int k;
} KHeap;


typedef struct {
    data_t *data;
    int data_size;
    int *keys;
    Node *nodes;
    int size;
    int capacity;
} BinHeap;


double test(data_t *arr, int size, int k);

int k_heap_top(KHeap *heap, data_t *value);

int k_heap_pop(KHeap *heap);

int k_heap_sift_down(KHeap *heap, int index);

int heap_sift_up(BinHeap *heap, int index);

int heap_sift_down(BinHeap *heap, int index);




int main() {
    srand(time(NULL));

    FILE *output = fopen("smart-heap.time", "a+");

    data_t *arr = (data_t *) calloc(MAX_SIZE, sizeof(data_t));

    fprintf(output, "%i-HEAP\n", BASE);

    for (int size = START_SIZE; size <= MAX_SIZE; size += STEP) {
        double avg_time = 0;

        for (int j = 1; j <= TESTS; j++) {
            for (int i = 0; i < size; i++) arr[i] = rand();
                
            avg_time += test(arr, size, BASE);
        }

        printf("Size: %i, Time: %.2f\n", size, avg_time / TESTS);

        fprintf(output, "%.2f\n", avg_time / TESTS);
    }

    fputc('\n', output);

    fclose(output);

    free(arr);

    return 0;
}




double test(data_t *arr, int size, int k) {
    clock_t t1 = clock();
    /*
    for (int i = 0; i < size; i++) printf("%i ", arr[i]);
    putchar('\n');
    */
    Node *nodes = (Node *) calloc(size, sizeof(Node));

    nodes[0].parent = -1;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < k; j++) {
            if (i * k + j + 1 < size) 
                nodes[i * k + j + 1].parent = j;
            
            nodes[i].keys[j] = i * k + j + 1;
        }

        BinHeap bin_heap = {arr, size, nodes[i].keys, nodes, k, k};

        for (int j = k / 2; j >= 0; j--)
            heap_sift_down(&bin_heap, j);
    }
    
    KHeap heap = {arr, nodes, size, size, k};

    for (int i = size / k; i >= 0; i--)
        k_heap_sift_down(&heap, i);

    for (int i = 0; i < size; i++) {
        data_t max = 0;
        k_heap_top(&heap, &max);
        k_heap_pop(&heap);
        arr[size - i - 1] = max;
    }

    free(nodes);
    /*
    for (int i = 0; i < size; i++) printf("%i ", arr[i]);
    putchar('\n');
    */
    clock_t t2 = clock();
    return 1000.0 * (t2 - t1) / CLOCKS_PER_SEC;
}


// K-HEAP


int k_heap_top(KHeap *heap, data_t *value) {
    *value = heap -> data[0];

    return 0;
}


int k_heap_pop(KHeap *heap) {
    (heap -> data)[0] = (heap -> data)[--heap -> size];
    (heap -> data)[heap -> size] = 0;

    if (heap -> size) {
        BinHeap parent_heap = {heap -> data, heap -> size, (heap -> nodes)[(heap -> size - 1) / heap -> k].keys, heap -> nodes, heap -> k, heap -> k};
        heap_sift_down(&parent_heap, (heap -> nodes)[heap -> size].parent);

        k_heap_sift_down(heap, 0);
    }

    return 0;
}


int k_heap_sift_down(KHeap *heap, int index) {
    while (index < heap -> size) {
        if (index * heap -> k + 1 >= heap -> size) break;

        BinHeap child_heap = {heap -> data, heap -> size, (heap -> nodes)[index].keys, heap -> nodes, heap -> k, heap -> k};
        BinHeap parent_heap = {heap -> data, heap -> size, (heap -> nodes)[(index - 1) / heap -> k].keys, heap -> nodes, heap -> k, heap -> k};

        int max = (heap -> nodes)[index].keys[0];

        max = ((heap -> data)[max] > (heap -> data)[index]) ? max : index; 

        if (max != index) {
            data_t swap = (heap -> data)[index];
            (heap -> data)[index] = (heap -> data)[max];
            (heap -> data)[max] = swap;

            heap_sift_up(&parent_heap, (heap -> nodes)[index].parent);
            heap_sift_down(&child_heap, 0);

            index = max;
        }
        else break;
    }

    return 0;
}


// BIN HEAP


data_t get_data(BinHeap *heap, int index) {
    return ((heap -> keys)[index] < heap -> data_size) ? (heap -> data)[(heap -> keys)[index]] : INT_MIN;
}


int heap_sift_up(BinHeap *heap, int index) {
    while (index) {
        int parent = (index - 1) / 2;

        if (get_data(heap, index) > get_data(heap, parent)) {
            (heap -> nodes)[(heap -> keys)[index]].parent = parent;
            (heap -> nodes)[(heap -> keys)[parent]].parent = index;

            int swap = (heap -> keys)[index];
            (heap -> keys)[index] = (heap -> keys)[parent];
            (heap -> keys)[parent] = swap;

            index = parent;
        }
        else break;
    }

    return 0;
}


int heap_sift_down(BinHeap *heap, int index) {
    while (index < heap -> size) {
        int left = 2 * index + 1, right = 2 * index + 2;

        int max = index;

        if (left < heap -> size)
            max = (get_data(heap, left) > get_data(heap, max)) ? left: max;
        
        if (right < heap -> size)
            max = (get_data(heap, right) > get_data(heap, max)) ? right : max;
        
        if (max == index) {
            return 0;
        }
        else {
            (heap -> nodes)[(heap -> keys)[index]].parent = max;
            (heap -> nodes)[(heap -> keys)[max]].parent = index;

            int swap = (heap -> keys)[index];
            (heap -> keys)[index] = (heap -> keys)[max];
            (heap -> keys)[max] = swap;

            index = max;
        }
    }

    return 0;
}
