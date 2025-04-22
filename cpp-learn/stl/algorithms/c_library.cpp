/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <cstdlib>
#include <assert.h>

/**
 * qsort
 * qsort_s
 * bsearch
 * bsearch_s
 */

int cmp(const void *a, const void *b);

bool arrays_are_equal(int *a, int *b, size_t size);

int main() {
    int numbers[] = {340, 2, 1, 6, -2, 89};
    size_t size = 6;

    // qsort
    qsort(numbers, size, sizeof(int), cmp);
    int sorted[] = {-2, 1, 2, 6, 89, 340};
    assert(arrays_are_equal(numbers, sorted, size));

    // qsort_s (C11)

    // bsearch
    int key = 89;
    int *item = (int *) bsearch(&key, numbers, size, sizeof(int), cmp);
    assert(item != nullptr);
    assert(*item == 89);

    // bsearch_s (C11)
}

int cmp(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

bool arrays_are_equal(int *a, int *b, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}
