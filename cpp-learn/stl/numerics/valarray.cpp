/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <iostream>
#include <sstream>
#include <valarray>

template <typename T>
bool equals(const std::valarray<T> &a, const std::valarray<T> &b);

size_t gslice_index(const std::gslice &s, size_t i, size_t j);  // max(i,j) to their corresponding index

int main()
{
    // A Numerical Array: valarray
    // Much numeric work relies on relatively simple single-dimensional vectors of floating-point values_.
    // In particular, such vectors are well supported by high-performance machine architectures, libraries
    // relying on such vectors are in wide use, and very aggressive optimization of code using such vectors
    // is considered essential in many fields. The valarray from <valarray> is a single-dimensional
    // numerical array. It provides the usual numeric vector arithmetic operations for an array type plus
    // support for slices and strides:

    // Numerical Array Classes
    // valarray<T>		    A numerical array of type T
    // slice		        A BLAS-like slice (start, length, and stride); §40.5.4
    // slice_array<T>	    A subarray identified by a slice; §40.5.5
    // gslice		        A slice generalized to describe a matrix
    // gslice_array<T>	    A submatrix identified by a generalized slice; §40.5.6
    // mask_array<T>	    A subset of an array identified by a mask; §40.5.2
    // indirect_array<T>	A subset of an array identified by a list of indices; §40.5.2

    /**
     * Constructors and Assignments
     */

    // Constructors

    // valarray va {};		valarray with no elements
    // valarray va {n};	    valarray of n elements with value T{} ; explicit
    // valarray va {t,n};	valarray of n elements with value t
    // valarray va {p,n};	valarray of n elements with values_ copied from [p:p+n)
    // valarray va {v2};	Move and copy constructor
    // valarray va {a};	    Construct va with elements from a;
    // 			            a can be a slice_array, gslice_array, mask_array, or indirect_array;
    //             			the number of elements is the number of elements in a
    // valarray va {args};	Construct from the initializer_list {args};
    //             			the number of elements is the number of elements in {args}
    // va.~valarray()		Destructor

    std::valarray<double> varr0;         // placeholder, we can assign to v0 later
    std::valarray<float> varr1(1000);    // 1000 elements with value float()==0.0F
    std::valarray<int> varr2(-1, 2000);  // 2000 elements with value -1
    // std::valarray<double> varr3(100, 9.8064);// bad mistake: floating-point valarray size
    std::valarray<int> varr4(varr2);     // v4 has v3.size() elements
    std::valarray<int> varr5{-1, 2000};  // two elements

    varr5[0] = 1;
    assert(varr5.min() == 1);

    // Assignments

    // va2=va		    Copy assignment: va2.size() becomes va.size()
    // va2=move(va)	    Move assignment: va becomes empty
    // va=t			    Scalar assignment: each element of va is a copy of t
    // va={args}		Assignment from the initializer_list {args} ;
    //                  the number of elements of va becomes {args}.size()
    //                  Assignment from a; a.size() must equal va.size()
    // va=a			    Assignment from a; a.size() must equal va.size() ;
    //                  a can be a slice_array, gslice_array, mask_array, or indirect_array
    // va@=va2		    v[i]@=va2[i] for each element of va; @ can be /, %, +, -, ^, &, |, <<, or >>
    // va@=t		    v[i]@=t for each element of va; @ can be /, %, +, _, ^, &, |, <<, or >>

    /**
     * Subscripting
     */

    // t=va[i]      Subscripting: t is a reference to the i th element of va; no range checking
    // a2=va[x]     Subset: x is a slice, a gslice, valarray<bool>, or a valarray<size_t>
    // Each operator[] returns a subset of the elements from a valarray.

    // A slice of a const valarray:
    // valarray<T> operator[](slice) const; // copy of elements
    const std::valarray<char> v0{"abcdefghijklmnop", 16};
    assert(v0.size() == 16);
    assert(v0[0] == 'a');
    assert(v0[v0.size() - 1] == 'p');

    std::valarray<char> v1{v0[std::slice(2, 5, 3)]};

    assert(v1.size() == 5);
    assert(equals(v1, std::valarray<char>{"cfilo", 5}));

    // A slice of a non-const valarray:
    // slice_array<T> operator[](slice); // references to elements
    std::valarray<char> v0b{"abcdefghijklmnop", 16};
    std::valarray<char> v2{"ABCDE", 5};
    v0b[std::slice(2, 5, 3)] = v2;

    assert(equals(v0b, std::valarray<char>{"abAdeBghCjkDmnEp", 16}));

    // A gslice of a const valarray:
    // valarray<T> operator[](const gslice&) const; // copies of elements
    const std::valarray<char> v0c{"abcdefghijklmnop", 16};
    const std::valarray<size_t> len{2, 3};
    const std::valarray<size_t> str{7, 2};
    std::valarray<char> v3{v0c[std::gslice(3, len, str)]};

    assert(equals(v3, std::valarray<char>{"dfhkmo", 6}));

    // A gslice of a non-const valarray:
    // gslice_array<T> operator[](const gslice&); // references to elements
    std::valarray<char> v0d{"abcdefghijklmnop", 16};
    std::valarray<char> v4{"ABCDE", 5};
    const std::valarray<size_t> len2{2, 3};
    const std::valarray<size_t> str2{7, 2};
    v0d[std::gslice(3, len2, str2)] = v4;

    assert(equals(v0d, std::valarray<char>{"abcAeBgCijDlEnop", 16}));

    // A valarray<bool> (a mask) of a const valarray:
    // valarray<T> operator[](const valarray<bool>&) const; // copies of elements

    // A valarray<bool> (a mask) of a non-const valarray:
    // mask_array<T> operator[](const valarray<bool>&); // references to elements

    // A valarray<size_t> (a set of indices) of a const valarray:
    // valarray<T> operator[](const valarray<size_t>&) const; // references to elements

    // A valarray<size_t> (a set of indices) of a non-const valarray:
    // valarray<T> operator[](const valarray<size_t>&) const; // references to elements

    // A valarray<size_t> (a set of indices) of a non-const valarray:
    // indirect_array<T> operator[](const valarray<size_t>&); // references to elements

    /**
     * Operations
     */

    // Member Operations
    // va.swap(va2)
    // n=va.siz e()
    // t=va.sum()
    // t=va.min()
    // t=va.max()
    // va2=va.shift(n)
    // va2=va.cshift(n)
    // va2=va.apply(f)
    // va.resiz e(n,t)
    // va.resiz e(n)

    // Operations
    // swap(va,va2)
    // va3=va@va2	    @ can be +, -, *, /, %, &, |, ^, <<, >>, &&, ||
    // vb=v@v2		    @ can be ==, !=, <, <=, >, >=
    // v2=@(v)	        @ can be abs, acos, asin, atan, cos, cosh, exp, log, log10
    // v3=atan2(v,v2)
    // v3=pow(v,v2)
    // p=begin(v)
    // p=end(v)

    int alpha[] = {1, 2, 3, 4, 5, 6, 7, 8};

    std::valarray<int> a(alpha, 8);
    assert(equals(a, std::valarray<int>{1, 2, 3, 4, 5, 6, 7, 8}));

    std::valarray<int> a2 = a.shift(2);
    assert(equals(a2, std::valarray<int>{3, 4, 5, 6, 7, 8, 0, 0}));

    std::valarray<int> a3 = a << 2;
    assert(equals(a3, std::valarray<int>{4, 8, 12, 16, 20, 24, 28, 32}));

    std::valarray<int> a4 = a.shift(-2);
    assert(equals(a4, std::valarray<int>{0, 0, 1, 2, 3, 4, 5, 6}));

    std::valarray<int> a5 = a >> 2;
    assert(equals(a5, std::valarray<int>{0, 0, 0, 1, 1, 1, 1, 2}));

    std::valarray<int> a6 = a.cshift(2);
    assert(equals(a6, std::valarray<int>{3, 4, 5, 6, 7, 8, 1, 2}));

    std::valarray<int> a7 = a.cshift(-2);
    assert(equals(a7, std::valarray<int>{7, 8, 1, 2, 3, 4, 5, 6}));

    /**
     * Slices
     *
     * A slice is an abstraction that allows us to manipulate a one-dimensional array (e.g., a built-in array,
     * a vector, or a valarray ) efficiently as a matrix of arbitrary dimension.
     */

    // 3-by-4 matrix
    size_t rows = 3;
    size_t columns = 4;
    std::valarray<int> v{
        00, 01, 02, 03, 12, 11, 12, 13, 20, 21, 22, 23,
    };

    // Row x is described by slice(x*4,4,1)
    size_t x = 0;
    std::valarray<int> row0 = v[std::slice(x * columns, columns, 1)];
    assert(equals(row0, std::valarray<int>{00, 01, 02, 03}));

    // Column y is described by slice(y,3,4)
    size_t y = 2;
    std::valarray<int> col2 = v[std::slice(y, rows, columns)];
    assert(equals(col2, std::valarray<int>{02, 12, 22}));

    // slice_array
    // From a valarray and a slice, we can build something that looks and feels like a valarray but is really
    // simply a way of referring to the subset of the array described by the slice.
    std::valarray<double> d = {1, 2, 3, 4};

    std::slice_array<double> v_even = d[std::slice(1, d.size() / 2, 2)];
    assert(equals(std::valarray<double>(v_even), std::valarray<double>{2, 4}));

    std::slice_array<double> v_odd = d[std::slice(0, d.size() / 2 + d.size() % 2, 2)];
    assert(equals(std::valarray<double>(v_odd), std::valarray<double>{1, 3}));

    v_even *= v_odd;  // multiply element pairs and store results in even elements
    assert(equals(std::valarray<double>(v_even), std::valarray<double>{2, 12}));

    v_odd = 0;  // assign 0 to every odd element of d
    assert(equals(std::valarray<double>(v_odd), std::valarray<double>{0, 0}));

    /**
     * Generalized Slices
     *
     * A slice (§29.2.2, §40.5.4) can describe a row or a column of an n-dimensional array. However,
     * sometimes we need to extract a subarray that is not a row or a column. For example, we might
     * want to extract the 3-by-2 matrix from the top-left corner of a 4-by-3 matrix:
     */

    // 4-by-3 matrix
    std::valarray<int> matrix{
        00, 01, 02, 10, 11, 12, 20, 21, 22, 30, 31, 32,
    };

    // 2 elements in the first dimension
    // 3 elements in the second dimension
    std::valarray<size_t> lengths{2, 3};

    // 3 is the stride for the first index
    // 1 is the stride for the second index
    std::valarray<size_t> strides{3, 1};

    std::gslice s(0, lengths, strides);
    std::ostringstream out;

    for (int i = 0; i < 3; ++i) {      // for each row
        for (int j = 0; j < 2; ++j) {  // for each element in row
            out << "(" << i << "," << j << ")->" << gslice_index(s, i, j) << "; ";
        }
    }

    assert(out.str() == "(0,0)->0; (0,1)->1; (1,0)->3; (1,1)->4; (2,0)->6; (2,1)->7; ");
}

template <typename T>
bool equals(const std::valarray<T> &a, const std::valarray<T> &b)
{
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

size_t gslice_index(const std::gslice &s, size_t i, size_t j)
{
    return s.start() + i * s.stride()[0] + j * s.stride()[1];
}