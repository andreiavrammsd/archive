/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <numeric>
#include <functional>
#include <cassert>
#include <valarray>

struct Record {
    int unit_price;
    int number_of_units;
};

long price(long val, const Record &r);

struct Matrix {
    size_t dim2() const { return 0; }

    std::vector<double> row(size_t) const { return {}; }
};

std::valarray<double> operator*(const Matrix &m, std::valarray<double> &v);

int main() {
    // x=accumulate(b,e,i)			        x is the sum of i and the elements of [b:e)
    // x=accumulate(b,e,i,f)		        accumulate using f instead of +
    // x=inner_product(b,e,b2,i)		    x is the inner product of [b:e) and [b2:b2+(e-b)),
    // 					                    that is, the sum of i and (*p1)*(*p2) for each p1 in [b:e)
    // 					                    and the corresponding p2 in [b2:b2+(e-b))
    // x=inner_product(b,e,b2,i,f,f2)		inner_product using f and f2 instead of + and *
    // p=partial_sum(b,e,out)			    Element i of [out:p) is the sum of elements [b:b+i]
    // p=partial_sum(b,e,out,f)		        partial_sum using f instead of +
    // p=adjacent_difference(b,e,out)		Element i of [out:p) is (*b+i)-*(b+i-1) for i>0 ;
    // 					                    if e-b>0, then *out is *b
    // p=adjacent_difference(b,e,out,f)	    adjacent_difference using f instead of -
    // iota(b,e,v)				            For each element in [b:e) assign ++v ;
    // 					                    thus the sequence becomes v+1, v+2, ...

    /**
     * accumulate
     */
    std::vector<int> ints{1, 2, 3};

    assert(accumulate(ints.begin(), ints.end(), 0) == 6);
    assert(accumulate(ints.begin(), ints.end(), 10, std::multiplies<>{}) == 60);

    std::vector<Record> records{
            {15, 2},
            {10, 3},
            {9,  0},
    };

    assert(accumulate(records.begin(), records.end(), 0, price) == 60);

    /**
     * inner_product
     *
     * The key operation in multiplying a Matrix by a valarray is an inner_product (see struct Matrix)
     */
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{4, 5, 0};

    assert(inner_product(a.begin(), a.end(), b.begin(), 0) == 14);
    assert(inner_product(a.begin(), a.end(), a.begin(), -2) == 12);

    /**
     * partial_sum() and adjacent_difference()
     *
     * The partial_sum() and adjacent_difference() algorithms are inverses of each other and deal with the
     * notion of incremental change.
     */

    std::vector<double> temps = {17, 19, 20, 20, 17};

    // Given a sequence a, b, c, d, etc., adjacent_difference() produces a, b-a, c-b, d-c, etc.
    adjacent_difference(temps.begin(), temps.end(), temps.begin());
    assert(temps == (std::vector<double>{17, 2, 1, 0, -3}));

    // Given a sequence a, b, c, d, etc., partial_sum() produces a, a+b, a+b+c, a+b+c+d, etc.
    partial_sum(temps.begin(), temps.end(), temps.begin());
    assert(temps == (std::vector<double>{17, 19, 20, 20, 17}));

    /**
     * iota
     */
    std::vector<int> v(5);

    iota(v.begin(), v.end(), 50);
    assert(v == (std::vector<int>{50, 51, 52, 53, 54}));
}

long price(long val, const Record &r) {
    return val + r.unit_price * r.number_of_units;
}

std::valarray<double> operator*(const Matrix &m, std::valarray<double> &v) {
    std::valarray<double> res(m.dim2());

    for (size_t i = 0; i < m.dim2(); ++i) {
        auto ri = m.row(i);
        res[i] = inner_product(ri.begin(), ri.end(), &v[0], 0.0);
    }

    return res;
}