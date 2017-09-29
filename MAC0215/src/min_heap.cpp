#include "min_heap.h"

template <class T>
void min_heap<T>::put_to_end (int index) {
    T temp = heap[index];
    heap[index] = heap[last];
    heap[last] = temp;

    order_heap ();
}

template <class T>
void min_heap<T>::order_heap () {
    for (int p = (last-1) >> 1; p >= 0; --p) {
        int parent = p;
        while (parent <= (last-1) >> 1) {
            int to_change = (parent << 1) + 1;
            if (heap[to_change+1] < heap[to_change])
                to_change++;
            if (heap[to_change] < heap[parent]) {
                T temp = heap[to_change];
                heap[to_change] = heap[parent];
                heap[parent] = temp;
                parent = to_change;
            }
            else break;
        }
    }
}

template <class T>
void min_heap<T>::insert (T element) {
    heap.push_back (element);
    last++;
    order_heap();
}

void min_heap<T>::update (T element) {
    int i = 0;
    while (i <= last) {
        if (equals (heap[i], element) && element < heap[i]) {
            heap[i] = element;
            order_heap ();
            break;
        }
        int next = i>>2;
        if (heap[next+1] < heap[next]) ++next;
        i = next;
    }
    if (i > last)
        insert(element);
}
