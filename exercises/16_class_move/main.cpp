#include "../exercise.h"
#include <cstddef>
#include <cstring>
#include <iostream>

// READ: 左值右值（概念）<https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170>
// READ: 左值右值（细节）<https://zh.cppreference.com/w/cpp/language/value_category>
// READ: 关于移动语义 <https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#move-semantics>
// READ: 如果实现移动构造 <https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170>

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

class DynFibonacci {
    size_t *cache;
    int cached;

public:
    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int capacity) : cache(new size_t[capacity]{0, 1}), cached(2) { std::cout << "Constructor called\n"; }

    // TODO: 实现移动构造器
    DynFibonacci(DynFibonacci &&source) noexcept {
        std::cout << "Move constructor called\n";
        //初始化自身对象
        cache = source.cache;
        cached = source.cached;
        //将被移动对象的资源设置为一个合法的状态，一般指针为空其他值自定
        source.cache = nullptr;
        source.cached = 0;
    }

    // TODO: 实现移动赋值
    // NOTICE: ⚠ 注意移动到自身问题 ⚠
    DynFibonacci &operator=(DynFibonacci &&source) noexcept {
        if (this != &source) {
            std::cout << "Move assignment called\n";
            //！！释放原本持有的资源！！
            delete[] cache;
            //获取source资源
            cache = source.cache;
            cached = source.cached;
            //设置source资源为合法状态
            source.cache = nullptr;
            source.cached = 0;
        } else {
            //警报：移动到自身，但什么也不做直接返回自身
            std::cerr << "self-assignment detected\n";
        }
        //返回自身
        return *this;
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci() {
        std::cout << "Destructor called\n";
        delete[] cache;
    }
    DynFibonacci(DynFibonacci const &others)
        : cache(new size_t[others.cached]),
          cached(others.cached) {
        std::cout << "Copy constructor called" << std::endl;
        std::memcpy(cache, others.cache, cached * sizeof(size_t));
    }
    // TODO: 实现正确的缓存优化斐波那契计算
    size_t operator[](int i) {
        for (; cached < i + 1; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    size_t operator[](int i) const {
        ASSERT(i <= cached, "i out of range");
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    bool is_alive() const {
        return cache;
    }
};

int main(int argc, char **argv) {

    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    //move constructor
    DynFibonacci const fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}
