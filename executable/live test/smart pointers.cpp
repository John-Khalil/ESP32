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


struct FREE_DELETE{
	void operator()(void* x) { free(x); }
};

std::shared_ptr<unsigned char>smartCache(unsigned char*cachedData){
    std::shared_ptr<unsigned char>sharedSmartPointer((unsigned char*)(calloc(stringCounter(cachedData)+1,sizeof(unsigned char))),FREE_DELETE());
    _CS(sharedSmartPointer.get(),cachedData);
    return sharedSmartPointer;
}



#define var_AUTO(z) void* g_iLine##z##var
// #define decl_AUTO(x) var_AUTO(x)
#define INLINE_VARIABLE        var_AUTO(__LINE__)

#define SMART_CACHE(cachedData) std::shared_ptr<unsigned char>SMART_CACHE##__LINE__##VAR;cachedData=(SMART_CACHE##__LINE__##VAR=smartCache(cachedData)).get();



int main(){
    unsigned char sampleText[]="some random test";
    unsigned char *samplePointer=NULL;
    {
        samplePointer=sampleText;
        // std::shared_ptr<unsigned char>secondSharedPointer;
       
        // secondSharedPointer=smartCache(samplePointer);
        // samplePointer=secondSharedPointer.get();
        SMART_CACHE(samplePointer);
        cout<<"samplePointer >> "<<samplePointer<<endl;

        cout<<"address >> "<<(uint64_t)samplePointer<<"\t address >> "<<(uint64_t)sampleText<<endl;
        
    }

    cout<<"samplePointer >> "<<samplePointer<<endl;     // this should print garbage
}



// int main()
// {
// 	//std::shared_ptr<entity>testObject = testFunction();
// 	//cout << "check order" << endl;
// 	unsigned char *testRawPointer = NULL;

// 	std::shared_ptr<unsigned char>secondSharedPointer;

// 	{

// 		unsigned char txt[200] = "this is test";
// 		std::shared_ptr<unsigned char>callocMemory((unsigned char*)calloc(100, sizeof(unsigned char)), FREE_DELETE());
		
// 		_CS((testRawPointer=callocMemory.get()), txt);
// 		cout << testRawPointer << endl;
// 		secondSharedPointer = callocMemory;
// 		cout << "first scope @ use_count >> " << callocMemory.use_count() <<"\t"<< secondSharedPointer.use_count() << endl;
// 	}

// 	cout << testRawPointer << endl;
// 	cout << "second scope @ use_count >> "  << secondSharedPointer.use_count() << endl;

// }

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
