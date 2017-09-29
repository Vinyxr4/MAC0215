#include "min_heap.h"

min_heap::min_heap() {
    last = 0;
}

void min_heap::order_heap () {
    for (int p = (last-1) >> 1; p >= 0; --p) {
        int parent = p;
        while (parent <= (last-1) >> 1) {
            int to_change = (parent << 1) + 1;
            if (heap[to_change+1].distance < heap[to_change].distance)
                to_change++;
            if (heap[to_change].distance < heap[parent].distance) {
                heap_element temp = heap[to_change];
                heap[to_change] = heap[parent];
                heap[parent] = temp;
                parent = to_change;
            }
            else break;
        }
    }
}

void min_heap::insert (heap_element element) {
    heap.push_back (element);
    last++;
    swim(last);
}

void min_heap::swim (int index) {
    int parent = (index-1) >> 1;

    while (1) {
        int child = (parent << 1) + 1;
        if (heap[child+1].distance < heap[child].distance)
            child++;
        if (heap[child].distance < heap[parent].distance) {
            heap_element temp = heap[child];
            heap[child] = heap[parent];
            heap[parent] = temp;
            if (parent - 1 < 0) break;
            parent = (parent-1) >> 1;
        }
        else break;
    }
}

void min_heap::dive (int index) {
    int parent = index;

    while (parent <= (last-1)>>1) {
        int child = (parent << 1) + 1;
        if (heap[child+1].distance < heap[child].distance)
            child++;
        if (heap[child].distance < heap[parent].distance) {
            heap_element temp = heap[child];
            heap[child] = heap[parent];
            heap[parent] = temp;
            parent = child;
        }
        else break;
    }
}

void min_heap::update (int index) {
    int child = (index << 1) + 1;
    if (heap[child+1].distance < heap[child].distance)
        child++;

    if (heap[child].distance >= heap[index].distance)
        swim (index);
    else dive (index);
}
