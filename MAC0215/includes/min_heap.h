#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <vector>
#include <queue>

struct heap_element {
    int x_pos, y_pos;
    float distance;

    heap_element (int x=0, int y=0, float dist=0.0):
        x_pos(x), y_pos (y), distance(dist){}
};

struct compare_distances {
    bool operator () (const heap_element &l, const heap_element &r) {
        return l.distance > r.distance;
    }
};
typedef std::priority_queue<heap_element, std::vector<heap_element>, compare_distances> priority_queue;

class min_heap {
public:
    min_heap ();
    void insert (heap_element element);
    heap_element pop ();
    void update (int index);

private:
    std::vector<heap_element> heap;
    void order_heap ();
    void swim (int index);
    void dive (int index);

    int last;
};

#endif // MIN_HEAP_H
