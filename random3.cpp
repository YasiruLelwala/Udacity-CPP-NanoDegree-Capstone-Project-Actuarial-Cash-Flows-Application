#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

// Class A is instantiated with an id
class A
{
public:
    A(int id) : _id(id) {std::cout << "Object of Class A created with id = " << _id << std::endl; };

    ~A() {std::cout << "Object of class A destroyed with id = " << _id << std::endl; };

    const int &getId() { return _id; };

private:
    int _id;
};

class B
{
public:
    B(std::unique_ptr<A> a) : _objectOfClassA(std::move(a)) {};
private:
    std::unique_ptr<A> _objectOfClassA;
    
};

class HoldingClass 
{
public:
    HoldingClass() {std::cout << "HoldingClass created" << std::endl; };

    void pushBackA(std::unique_ptr<A> a) // this function takes ownership of smart pointer and underlying object and then transfers this ownership to the holding class.
    {
        _objectsOfClassA.emplace_back(std::move(a)); 
    }

    void printSizeA() 
	{ 
		std::cout << "Number of A objects = " << _objectsOfClassA.size() << std::endl; 
	};

private:
    std::vector<std::unique_ptr<A>> _objectsOfClassA;
};

int main()
{
    std::shared_ptr<HoldingClass> holdingClass = std::make_shared<HoldingClass>();

    for (int i = 0; i < 2; i++)
    {
        std::unique_ptr<A> a = std::make_unique<A>(i);
        holdingClass->pushBackA(std::move(a)); // ownership of a is transferred to the function which then transfers it to the holding class

    }

    holdingClass->printSizeA();

    std::cout << "Finished" << std::endl;
}