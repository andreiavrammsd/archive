/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <iostream>
#include <sstream>

int main()
{
    // Conceptually, an output stream puts characters into a buffer. Sometime later, the characters are
    // then written to ("flushed to") wherever they are supposed to go. Such a buffer is called a stream-buf.
    // Its definition is found in <streambuf>. Different types of streambufs implement different
    // buffering strategies. Typically, the streambuf stores characters in an array until an overflow forces it
    // to write the characters to their real destination.

    // The set of template arguments for an ostream and its streambuf must be the same, and they determine
    // the type of character used in the character buffer.

    // As usual, a couple of aliases are provided for the (supposedly) most common cases:
    // using streambuf = basic_streambuf<char>;
    // using wstreambuf = basic_streambuf<wchar_t>;

    // public basic_streambuf<C,Tr> Operations

    // sb.~basic_streambuf()
    // loc=sb.g etloc()
    // loc2=sb.pubimbue(loc)
    // psb=sb.pubsetbuf(s,n)
    // pos=sb.pubseekoff(n,w,m)
    // pos=sb.pubseekoff(n,w)
    // pos=sb.pubseekpos(n,m)
    // pos=sb.pubseekpos(n)
    // sb.pubsync()

    // All constructors are protected because basic_streambuf is designed as a base class.

    // protected basic_streambuf<C,Tr> Operations

    // basic_streambuf sb {};
    // basic_streambuf sb {sb2};
    // sb=sb2
    // sb.swap(sb2)
    // sb.imbue(loc)
    // psb=sb.setbuf(s,n)
    // pos=sb.seekoff(n,w,m)
    // pos=sb.seekoff(n,w)
    // pos=sb.seekpos(n,m)
    // n=sb.sync()

    // The put-and-get interface is separated into a public and a protected one:

    // public Put and Get basic_streambuf<C,Tr> Operations

    // n=sb.in_avail()
    // c=sb.snextc()
    // n=sb.sbumpc()
    // c=sb.sgetc()
    // n=sb.sgetn(p,n)
    // n=sb.sputbackc(c)
    // n=sb.sungetc()
    // n=sb.sputc(c)
    // n=sb.sputn(s,n)

    // protected Put and Get basic_streambuf<C,Tr> Operations

    // sb.setg(b,n,end)
    // pc=sb.eback()
    // pc=sb.gptr()
    // pc=sb.egptr()
    // sb.gbump(n)
    // n=sb.showmanyc()
    // n=sb.underflow()
    // n=sb.uflow()
    // n=sb.pbackfail(c)
    // n=sb.pbackfail()
    // sb.setp(b,e)
    // pc=sb.pbase()
    // pc=sb.pptr()
    // pc=sb.epptr()
    // sb.pbump(n)
    // n2=sb.xsgetn(s,n)
    // n2=sb.xsputn(s,n)
    // n=sb.overflow(c)
    // n=sb.overflow()

    /**
     * Output Streams and Buffers
     *
     * An ostream provides operations for converting values_ of various types into character sequences
     * according to conventions (§38.4.2) and explicit formatting directives (§38.4.5). In addition, an
     * ostream provides operations that deal directly with its streambuf
     */

    std::ostringstream out;

    out << "0123456789";
    out.seekp(8);                       // 8 from beginning
    out << '#';                         // ad '#' and move position (+1)
    out.seekp(-4, std::ios_base::cur);  // 4 backward
    out << '*';                         // add '*' and move position (+1)

    assert(out.str() == "01234*67#9");

    // The flush() operation allows the user to empty the buffer without waiting for an overflow.
    // It is possible to use << to write a streambuf directly into an ostream. This is primarily handy for
    // implementers of I/O mechanisms.

    /**
     * Input Streams and Buffers
     *
     * An istream provides operations for reading characters and converting them into values_ of various
     * types (§38.4.1). In addition, an istream provides operations that deal directly with its streambuf
     */

    std::istringstream in{"abc"};

    assert(in.get() == 'a');
    assert(in.peek() == 'b');
    in.putback('a');
    assert(in.peek() == 'a');

    /**
     * Buffer Iterators
     */

    // istreambuf_iterator
    // An istreambuf_iterator reads a stream of characters from an istream_buffer
    // If you use an istreambuf_iterator as an input iterator, its effect is like that of
    // other input iterators: a stream of characters can be read from input using c=*p++:

    // istreambuf_iterator p {};
    // istreambuf_iterator p {p2};
    // istreambuf_iterator p {is};
    // istreambuf_iterator p {psb};
    // istreambuf_iterator p {nullptr};
    // istreambuf_iterator p {prox};
    // p.~istreambuf_iterator()
    // c=*p
    // p−>m
    // p=++p
    // prox=p++
    // p.equal(p2)
    // p==p2
    // p!=p2

    // ostreambuf_iterator
    // An ostreambuf_iterator writes a stream of characters to an ostream_buffer :

    // ostreambuf_iterator p {os};
    // ostreambuf_iterator p {psb};
    // p=c
    // *p
    // ++p
    // p++
    // p.failed()

    std::istringstream sin{"abc"};
    sin.seekg(1);
    std::ostringstream sout;

    auto buf_in = std::istreambuf_iterator<char>{sin};
    auto buf_in_end = std::istreambuf_iterator<char>{};
    auto buf_out = std::ostreambuf_iterator<char>{sout};

    copy(buf_in, buf_in_end, buf_out);

    assert(sout.str() == "bc");
    assert(!buf_out.failed());
}
