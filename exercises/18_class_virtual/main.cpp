#include "../exercise.h"
#include <iostream>
#include <ostream>

// READ: 虚函数 <https://zh.cppreference.com/w/cpp/language/virtual>

struct A {
    virtual char virtual_name() const {
        return 'A';
    }
    char direct_name() const {
        return 'A';
    }
};

/**
由于这个时候，A 和 B 中没有任何字段，因此它们实际上的大小应该是 0，但是 C++ 禁止 0 长度的结构体存在，没有任何字段的结构体长度会设置为 1 字节，以免为它们分配空间时产生不正常的指针。
但是一旦我把 A::name 方法声明为虚，就会发现结构体的长度变为 8，这就是因为结构体里多了一个虚表指针。每个对象的虚函数实现的函数指针会存放在虚表指针中，这样实现了通过对象调用方法时，可以调用到具体实现类的对应方法。
这就是所谓虚函数重写的意思，就是说对于这个类型的对象，类型提供了另一个函数指针，替代继承自基类的虚表中同名同参的函数指针。
 */
struct B : public A {
    // READ: override <https://zh.cppreference.com/w/cpp/language/override>
    char virtual_name() const override {
        return 'B';
    }
    char direct_name() const {
        return 'B';
    }
};
struct C : public B {
    // READ: final <https://zh.cppreference.com/w/cpp/language/final>
    char virtual_name() const final {
        return 'C';
    }
    char direct_name() const {
        return 'C';
    }
};
struct D : public C {
    char direct_name() const {
        return 'D';
    }
};

int main(int argc, char **argv) {
    constexpr auto MSG = "Replace '?' with its correct name.";
    std::cout << "Size of A:" << sizeof(A) << std::endl;
    std::cout << "Size of B:" << sizeof(B) << std::endl;

    A a;
    B b;
    C c;
    D d;

    ASSERT(a.virtual_name() == 'A', MSG);
    ASSERT(b.virtual_name() == 'B', MSG);
    ASSERT(c.virtual_name() == 'C', MSG);

    //找最近的父类的方法
    ASSERT(d.virtual_name() == 'C', MSG);

    ASSERT(a.direct_name() == 'A', MSG);
    ASSERT(b.direct_name() == 'B', MSG);
    ASSERT(c.direct_name() == 'C', MSG);
    ASSERT(d.direct_name() == 'D', MSG);

    A &rab = b;
    B &rbc = c;
    C &rcd = d;

    ASSERT(rab.virtual_name() == 'B', MSG);
    ASSERT(rbc.virtual_name() == 'C', MSG);
    ASSERT(rcd.virtual_name() == 'C', MSG);

    //没有声明为虚的同名函数只能用引用类型的
    ASSERT(rab.direct_name() == 'A', MSG);
    ASSERT(rbc.direct_name() == 'B', MSG);
    ASSERT(rcd.direct_name() == 'C', MSG);

    A &rac = c;
    B &rbd = d;

    ASSERT(rac.virtual_name() == 'C', MSG);

    //同样的，找继承链上的最近父类
    ASSERT(rbd.virtual_name() == 'C', MSG);

    //没有声明为虚的同名函数只能用y引用的同名函数，不是最近的父类的
    ASSERT(rac.direct_name() == 'A', MSG);
    ASSERT(rbd.direct_name() == 'B', MSG);

    A &rad = d;

    //同样的，找继承链上的最近父类
    ASSERT(rad.virtual_name() == 'C', MSG);
    //没有声明为虚的同名函数只能用y引用的同名函数，不是最近的父类的
    ASSERT(rad.direct_name() == 'A', MSG);

    return 0;
}

// READ: 扩展阅读-纯虚、抽象 <https://zh.cppreference.com/w/cpp/language/abstract_class>
// READ: 扩展阅读-虚继承 <https://zh.cppreference.com/w/cpp/language/derived_class>
