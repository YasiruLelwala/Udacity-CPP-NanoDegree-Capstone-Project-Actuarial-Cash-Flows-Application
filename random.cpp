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
    B(A &a) : _objectOfClassA(a) {};

    A &getA() { return _objectOfClassA; };

    const int &getIdMultipliedByTen() { return _idMultipliedByTen; };

    void run()
    {
        this->_idMultipliedByTen = multiplyIdByTen();
    };

    int multiplyIdByTen() 
    {
        int idMultipliedByTen = _objectOfClassA.getId() * 10;
        return idMultipliedByTen;
    };

private:
    A &_objectOfClassA; // holds a reference to an object of class A

    int _idMultipliedByTen;
};

class HoldingClass
{
public:
    HoldingClass() {std::cout << "HoldingClass created" << std::endl; };

    void pushBackA(A &&a)
	{
        //std::lock_guard<std::mutex> uLock(_mutex);

		//std::cout << "Object of Class A, id: " << a.getId() << std::endl; 

		_objectsOfClassA.emplace_back(std::move(a));
	};

    void pushBackB(B &&b)
	{
        std::lock_guard<std::mutex> uLock(_mutex);

		//std::cout << "Object of Class B, id: " << b.getId() << std::endl; 

		_objectsOfClassB.emplace_back(std::move(b));
	};

    void printSizeA() 
	{ 
		std::lock_guard<std::mutex> uLock(_mutex);

		std::cout << "Number of A objects = " << _objectsOfClassA.size() << std::endl; 

	};

private:
    std::mutex _mutex;

    std::vector<A> _objectsOfClassA;
    std::vector<B> _objectsOfClassB;
};

int main()
{
    std::shared_ptr<HoldingClass> holdingClass = std::make_shared<HoldingClass>();

    for (int i = 0; i < 2; i++)
    {
        std::unique_ptr<A> a = std::make_unique<A>(i);
        holdingClass->pushBackA(std::move(*a));

    }

    holdingClass->printSizeA();

    std::cout << "Finished" << std::endl;

    
}

