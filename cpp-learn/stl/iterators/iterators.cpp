/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>
#include <forward_list>
#include <list>

/**
 * Input iterator
 * Output iterator
 * Forward iterator
 * Bidirectional iterator
 * Random-access iterator
 */

int main() {
    /**
     * Input iterator
     *
     * We can iterate forward using ++ and read each element (repeatedly) using *.
     * We can compare input iterators using == and !=.
     *
     * This is the kind of iterator that istream offers; see §38.5.
     */

    std::vector<int> v{1, 2, 3};
    std::vector<int>::iterator itr0, itr1;

    itr0 = v.begin();
    itr1 = v.begin() + 1;
    assert(itr0 != itr1);

    itr1 = itr1 - 1;
    assert(itr0 == itr1);

    assert(*itr1 == 1);

    /**
     * Output iterator
     *
     * We can iterate forward using ++ and write an element once only using *.
     *
     * This is the kind of iterator that ostream offers; see §38.5.
     */
    std::stringstream s;

    std::ostream_iterator<int> out(s, ",");
    *out = 5;
    copy(v.begin(), v.end(), out);
    assert(s.str() == "5,1,2,3,");

    std::forward_list<int> flist;
    copy(v.begin(), v.end(), front_inserter(flist));
    s.str("");
    while (!flist.empty()) {
        s << flist.front();
        flist.pop_front();
    }
    assert(s.str() == "321");

    /**
     * Forward iterator
     *
     * We can iterate forward repeatedly using ++ and read and write (unless the
     * elements are const) elements repeatedly using *. If a forward iterator points to a class
     * object, we can use -> to refer to a member. We can compare forward iterators using == and
     * !=.
     *
     * This is the kind of iterator forward_list offers (§31.4).
     */
    flist.push_front(1);
    flist.push_front(2);

    std::forward_list<int>::iterator flitbeg, flitend;
    flitbeg = flist.begin();
    flitend = flist.end();

    assert(flitbeg != flitend);
    assert(*flitbeg == 2);

    ++flitbeg;
    assert(flitbeg != flitend);
    assert(*flitbeg == 1);

    ++flitbeg;
    assert(flitbeg == flitend);

    /**
     * Bidirectional iterator
     *
     * We can iterate forward (using ++) and backward (using --) and read and write
     * (unless the elements are const) elements repeatedly using *. If a bidirectional
     * iterator points to a class object, we can use -> to refer to a member. We can
     * compare bidirectional iterators using == and !=.
     *
     * This is the kind of iterator that list , map , and set offer (§31.4).
     */
    std::vector<int>::const_reverse_iterator ritr;
    s.str("");

    for (ritr = v.crbegin(); ritr != v.crend(); ++ritr) {
        s << *ritr;
    }

    assert(s.str() == "321");
    assert(ritr == v.crend());

    --ritr;
    assert(ritr == v.crend() - 1);
    assert(*ritr == 1);

    ++ritr;
    assert(ritr == v.crend());

    std::list<int> list;
    list.push_back(2);
    list.push_front(1);

    std::list<int>::iterator litbeg, litend;
    litbeg = list.begin();
    litend = list.end();

    assert(litbeg != litend);
    --litend;
    --litend;
    assert(litbeg == litend);
    assert(*litend == 1);
    assert(*litbeg == 1);

    /**
     * Random-access iterator
     *
     * We can iterate forward (using ++ or +=) and backward (using - or -=) and read and write
     * (unless the elements are const) elements repeatedly using * or []. If a random-access iterator
     * points to a class object, we can use -> to refer to a member. We can subscript a random-access
     * iterator using [], add an integer using +, and subtract an integer using -. We can find the distance
     * between two random-access iterators to the same sequence by subtracting one from the other.
     * We can compare random-access iterators using ==, !=, <, <=, >, and >=.
     *
     * This is the kind of iterator that vector offers (§31.4).
     */
    assert(*itr0 == 1);
    assert(itr0 == v.begin());

    ++itr0;
    assert(*itr0 == 2);

    --itr0;
    assert(*itr0 == 1);

    assert(itr0[0] == 1);
}
