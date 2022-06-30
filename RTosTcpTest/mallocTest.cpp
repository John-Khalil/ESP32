// double colum.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

class classTest  {
public:
	void sayHello(void);
};

void classTest::sayHello(void) {
	cout << "this is test \n";
}


classTest ffs;




unsigned short stringCounter(unsigned char *counted) {
	unsigned short counter = 0;
	while (*counted) {
		counter++;
		counted++;
	}
	return counter;
}

typedef unsigned long loopVar;

loopVar _LOOP_COUNTER_ = 0;
loopVar *_NESTED_LOOPS_= (loopVar*)malloc(1*sizeof(loopVar));
unsigned char _NESTED_LOOPS_COUNTER_ = 0;
#define loop(_LOOP_ITERATIONS_,_LOOP_BODY_) _NESTED_LOOPS_[_NESTED_LOOPS_COUNTER_++]=_LOOP_COUNTER_; _NESTED_LOOPS_=(loopVar *)realloc(_NESTED_LOOPS_,(_NESTED_LOOPS_COUNTER_+1) *sizeof(loopVar));   _LOOP_COUNTER_=_LOOP_ITERATIONS_;while(_LOOP_COUNTER_--) _LOOP_BODY_; _LOOP_COUNTER_=_NESTED_LOOPS_[--_NESTED_LOOPS_COUNTER_];  _NESTED_LOOPS_=(loopVar *)realloc(_NESTED_LOOPS_, (_NESTED_LOOPS_COUNTER_+1) * sizeof(loopVar));

#define arrayCounter stringCounter 

loopVar _FOR_EACH_LOOP_COUNTER = 0;
#define forEach(_ARRAY_LIKE_STRUCTURE,_SINGLE_ELEMENT_,_FOREACH_LOOP_BODY_) _FOR_EACH_LOOP_COUNTER = 0; loop(arrayCounter(_ARRAY_LIKE_STRUCTURE),{_SINGLE_ELEMENT_=_ARRAY_LIKE_STRUCTURE[_FOR_EACH_LOOP_COUNTER++];_FOREACH_LOOP_BODY_;});

//#define _ARRAY_LIKE_STRUCTURE.forEach((_SINGLE_ELEMENT_){_FOREACH_LOOP_BODY_}) _FOR_EACH_LOOP_COUNTER = 0; loop({_SINGLE_ELEMENT_=_ARRAY_LIKE_STRUCTURE[_FOR_EACH_LOOP_COUNTER++],_FOREACH_LOOP_BODY_;},arrayCounter(_ARRAY_LIKE_STRUCTURE))

int main()
{/**
	unsigned char memoryAllocationCounter = 0;
	unsigned short *p = NULL;
	p = (unsigned short *)malloc((++memoryAllocationCounter) *sizeof(unsigned short));
	*p = 20;
	
	cout << *p << endl;

	 (unsigned short *)realloc(p, (++memoryAllocationCounter) * sizeof(unsigned short));

	cout << *p << endl;

	unsigned short *testPtr = p + 1;
	
	*testPtr = 5;

	cout<< "- "<< p[0] << endl;
	cout << "-+ " << p[1] << endl;
	*/
	
	int x = 0;
	loop(3, {
		loop(3, {
			loop(3, {
				loop(3, {
					loop(3, {
						cout << ++x << endl;
					});
				});
			});
		});
	});

	/**
	unsigned char *testPtr = NULL;
	testPtr = new unsigned char[18];




	cout << " test to if mem allocation worked >>>>          "<< sizeof(testPtr) << endl;

	unsigned char testArray[10] = "hello!";

	unsigned char *testTestPtr = testPtr + 3;

	free(testTestPtr);
	
	memcpy(testPtr, testArray,sizeof(testArray));


	unsigned char variable;
	forEach(testPtr, variable, {


		cout<< "- -" << variable <<endl;
	});
	
	//ffs.sayHello();

    cout << "\nHello World!\n";

	*/
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
