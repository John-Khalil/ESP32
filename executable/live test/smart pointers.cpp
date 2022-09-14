// smart pointers test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>
using namespace std;


unsigned short stringCounter(unsigned char *counted) {
	unsigned short counter = 0;
	while (*counted) {
		counter++;
		counted++;
	}
	return counter;
}

unsigned char *_CS(unsigned char *bigString, unsigned char *smallString) {
	unsigned char *smallStringLocation = bigString + stringCounter(bigString);		// lucky for us c/c++ support pointer arthematic
	while (*smallString) {
		*smallStringLocation = *smallString;
		smallString++;
		smallStringLocation++;
	}
	return bigString;
}

class entity {
public:
	entity() {
		cout << "object is now created" << endl;
	}
	~entity() {
		cout << "object is destroyed" << endl;
	}
};


std::shared_ptr<entity>testFunction(void) {
	return std::shared_ptr<entity>(new entity());
}


struct free_delete
{
	void operator()(void* x) { free(x); }
};

int main()
{
	//std::shared_ptr<entity>testObject = testFunction();
	//cout << "check order" << endl;
	unsigned char *testRawPointer = NULL;

	std::shared_ptr<unsigned char>secondSharedPointer;

	{

		unsigned char txt[200] = "this is test";
		std::shared_ptr<unsigned char>callocMemory((unsigned char*)calloc(100, sizeof(unsigned char)), free_delete());
		
		_CS((testRawPointer=callocMemory.get()), txt);
		cout << testRawPointer << endl;
		secondSharedPointer = callocMemory;
		cout << "first scope @ use_count >> " << callocMemory.use_count() <<"\t"<< secondSharedPointer.use_count() << endl;
	}

	cout << testRawPointer << endl;
	cout << "second scope @ use_count >> "  << secondSharedPointer.use_count() << endl;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
