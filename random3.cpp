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
private:
    
};

class HoldingClass 
{
public:
    HoldingClass() {std::cout << "HoldingClass created" << std::endl; };

    void pushBackA(std::unique_ptr<A> a)
    {
        _objectsOfClassA.emplace_back(std::move(a));
    }

    void printSizeA() 
	{ 
		//std::lock_guard<std::mutex> uLock(_mutex);

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
        holdingClass->pushBackA(std::move(a));

    }

    holdingClass->printSizeA();

    std::cout << "Finished" << std::endl;
}