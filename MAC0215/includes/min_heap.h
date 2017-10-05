#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <vector>
#include <queue>

struct heap_element {
    int x_pos, y_pos;
    float distance;
    int key;

    heap_element (int x=0, int y=0, float dist=-1.0, int k = -1):
        x_pos(x), y_pos (y), distance(dist), key(k) {}
};

struct compare_distances {
    bool operator () (const heap_element &l, const heap_element &r) {
        return l.distance > r.distance;
    }
};
typedef std::priority_queue<heap_element, std::vector<heap_element>, compare_distances> priority_queue;

class min_heap {
public:
    min_heap (int max_size);
    void push (heap_element element);
    void pop ();
    int size ();
    bool empty ();
    heap_element top ();
    void show_heap ();


private:
    std::vector<heap_element> heap;
    std::vector<bool> is_present;
    std::vector<int> element_index;
    void update (int index);
    void order_heap ();
    void swim (int index);
    void dive (int index);

    int last;
};

#endif // MIN_HEAP_H
