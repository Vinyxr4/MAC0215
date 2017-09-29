#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <vector>

template <class T>
class min_heap {
public:
    min_heap ();
    void insert (T element);
    T top ();
    void pop ();
    void update (T element);

private:
    std::vector<T> heap;
    void put_to_end (int index);
    void order_heap ();

    int last;
};

template <class T>
min_heap<T>::min_heap() {
    last = 0;
}

#endif // MIN_HEAP_H
