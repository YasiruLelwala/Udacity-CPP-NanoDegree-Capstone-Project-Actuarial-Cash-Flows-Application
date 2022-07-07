#include <iostream>
#include <vector>
#include <thread>
#include <mutex>


class MyClass
{
private:
    int _member;

public:
    MyClass(int val) : _member{val} { std::cout << "Object of MyClass created on the heap" << std::endl; }

    ~MyClass() { std::cout << "Object of MyClass destructed" << std::endl; }

    void printVal() { std::cout << ", managed object " << this << " with val = " << _member << std::endl; }
};

// void f(std::unique_ptr<MyClass> ptr)
// {
//     std::cout << "unique_ptr " << &ptr;
//     ptr->printVal();
// }

// int main()
// {
//     std::unique_ptr<MyClass> uniquePtr = std::make_unique<MyClass>(23);
//     std::cout << "unique_ptr " << &uniquePtr;
//     uniquePtr->printVal();

//     f(std::move(uniquePtr));

//     if (uniquePtr)
//         uniquePtr->printVal();

//     return 0;
// }

void f(std::shared_ptr<MyClass> ptr)
{
    std::cout << "shared_ptr (ref_cnt= " << ptr.use_count() << ") " << &ptr;
    ptr->printVal();
}

int main()
{
    std::shared_ptr<MyClass> sharedPtr = std::make_shared<MyClass>(23);
    std::cout << "shared_ptr (ref_cnt= " << sharedPtr.use_count() << ") " << &sharedPtr;
    sharedPtr->printVal();

    f(sharedPtr);

    std::cout << "shared_ptr (ref_cnt= " << sharedPtr.use_count() << ") " << &sharedPtr;
    sharedPtr->printVal();

    return 0;
}