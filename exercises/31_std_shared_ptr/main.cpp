#include "../exercise.h"
#include <iostream>
#include <memory>
#include <ostream>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10);
    std::shared_ptr<int> ptrs[]{shared, shared, shared};

    std::weak_ptr<int> observer = shared;
    std::cout << observer.use_count() << std::endl;
    ASSERT(observer.use_count() == 4, "");

    ptrs[0].reset();
    std::cout << observer.use_count() << std::endl;
    ASSERT(observer.use_count() == 3, "");

    ptrs[1] = nullptr;
    std::cout << observer.use_count() << std::endl;
    ASSERT(observer.use_count() == 2, "");

    ptrs[2] = std::make_shared<int>(*shared);//这时候ptr[2]指向一个新的资源,但是资源上的值还是*shared=10
    std::cout << observer.use_count() << std::endl;
    ASSERT(observer.use_count() == 1, "");

    ptrs[0] = shared;
    ptrs[1] = shared;
    ptrs[2] = std::move(shared);//删除一个又指向另一个,所以不变
    std::cout << observer.use_count() << std::endl;
    ASSERT(observer.use_count() == 3, "");

    std::ignore = std::move(ptrs[0]);//不会-1,ptrs0依然持有原来的对象
    ptrs[1] = std::move(ptrs[1]);    //
    ptrs[1] = std::move(ptrs[2]);    //-1
    std::cout << "?? " << *ptrs[0] << std::endl;
    ASSERT(observer.use_count() == 2, "");//为什么不是1?

    shared = observer.lock();
    std::cout << observer.use_count() << std::endl;
    ASSERT(observer.use_count() == 3, "");

    shared = nullptr;
    for (auto &ptr : ptrs) ptr = nullptr;
    std::cout << observer.use_count() << std::endl;
    ASSERT(observer.use_count() == 0, "");

    shared = observer.lock();//观察对象已经被销毁,所以依然只获得一个指向为空的shared_ptr
    std::cout << observer.use_count() << std::endl;
    ASSERT(observer.use_count() == 0, "");

    return 0;
}
