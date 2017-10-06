#include "min_heap.h"
#include <QDebug>
min_heap::min_heap(int max_size) {
    last = -1;
    heap = std::vector<heap_element> (max_size);
    is_present = std::vector<bool> (max_size, false);
    element_index = std::vector<int> (max_size, -1);
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

void min_heap::push (heap_element element) {
    int key = element.key;
    //qDebug() << element.key;
    if (is_present[key] && heap[element_index[key]].distance > element.distance) {
        heap[element_index[key]] = element;
        update(element_index[key]);
    }
    else {
        is_present[key] = true;
        element_index[key] = ++last;
        heap[last] = element;
        swim(last);
    }
}

heap_element min_heap::top () {
    return heap[0];
}

void min_heap::show_heap () {
    for (int i = 0; i <= last; ++i)
        qDebug () << heap[i].key << heap[i].distance;
}

void min_heap::pop () {
    heap_element last_element = heap[last--];
    element_index[last_element.key] = 0;
    if (!empty()) {
        element_index[heap[0].key] = -1;
        is_present[heap[0].key] = false;
        heap[0] = last_element;
        dive (0);
    }
}

bool min_heap::empty() {
    return last < 0;
}

int min_heap::size() {
    return last+1;
}

void min_heap::swim (int index) {
    int parent = (index-1) >> 1;

    while (parent >= 0) {
        int child = (parent << 1) + 1;
        if (last > child && heap[child+1].distance < heap[child].distance)
            child++;
        if (heap[child].distance < heap[parent].distance) {
            heap_element temp = heap[child];
            element_index[heap[child].key] =  element_index[heap[parent].key];
            heap[child] = heap[parent];
            element_index[heap[parent].key] =  element_index[temp.key];
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
        if (last > child && heap[child+1].distance < heap[child].distance)
            child++;
        if (heap[child].distance < heap[parent].distance) {
            heap_element temp = heap[child];
            element_index[heap[child].key] =  element_index[heap[parent].key];
            heap[child] = heap[parent];
            element_index[heap[parent].key] =  element_index[temp.key];
            heap[parent] = temp;
            parent = child;
        }
        else break;
    }
}

void min_heap::update (int index) {
    int child = (index << 1) + 1;

    if (last > child && heap[child+1].distance < heap[child].distance)
        child++;

    if (heap[child].distance > heap[index].distance)
        swim (index);
    else if (heap[child].distance < heap[index].distance)
        dive (index);

}
