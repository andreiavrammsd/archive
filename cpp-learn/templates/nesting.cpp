/*
 The C++ Programming Language, 4th Edition
 */

#include <memory>
#include <cassert>

// Formally, a member of a template depends on all of a template's arguments.
// That can have unfortunate side effects in cases where the behavior of the
// member does not in fact use every template argument.

template <typename T, typename Allocator>
class List {
   private:
    struct Link {
        T val;
        Link *succ;
        Link *prev;
    };
};

// Here, Link is an implementation detail of List. Thus, it seems a perfect example of a type best
// defined in the scope of List and even kept private. This has been a popular design and generally
// works very well. But surprisingly, it can imply performance cost compared to using a nonlocal Link type.
// [...] This leads us to consider a design where Link isn't a member:

template <typename T, typename Allocator>
class LList;

template <typename T>
class Link {
    template <typename U, typename A>
    friend class LList;

    T val;
    Link *succ;
    Link *prev;
};

template <typename T, typename Allocator>
class LList {
   public:
    void push(T val)
    {
        Allocator alloc;
        Link<T> *node = alloc.allocate(1);

        node->val = val;
        node->succ = head;

        if (head) {
            head->prev = node;
        }

        head = node;
    }

    T pop()
    {
        auto val = head->val;

        auto h = head;
        head = head->succ;
        delete h;

        return val;
    }

   private:
    Link<T> *head{};
};

int main()
{
    LList<int, std::allocator<Link<int>>> list;

    list.push(1);
    list.push(2);
    list.push(3);

    assert(list.pop() == 3);
    assert(list.pop() == 2);
    assert(list.pop() == 1);
}
