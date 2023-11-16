/*
------------------DR VASILIOS KELEFOURAS-----------------------------------------------------
------------------COMP1001 ------------------------------------------------------------------
------------------COMPUTER SYSTEMS MODULE-------------------------------------------------
------------------UNIVERSITY OF PLYMOUTH, SCHOOL OF ENGINEERING, COMPUTING AND MATHEMATICS---
*/

#include <Windows.h>
#include "array_constant_addition.h"

#define TIMES_TO_RUN 1 //how many times the function will run


int main() {

	unsigned short int output;
	int t;
	//the following command pins the current process to the 1st core
	//otherwise, the OS tongles this process between different cores
	BOOL success = SetProcessAffinityMask(GetCurrentProcess(), 1);
	if (success == 0) {
		//cout << "SetProcessAffinityMask failed" << endl;
		printf("\nSetProcessAffinityMask failed\n");
		system("pause");
		return -1;
	}

	//initialize the arrays
	initialization_ConstAdd();

	//define the timers measuring execution time
	clock_t start_1, end_1; //ignore this for  now
	start_1 = clock(); //start the timer (THIS IS NOT A VERY ACCURATE TIMER) - ignore this for now

	//auto start = std::chrono::high_resolution_clock::now(); //ACCURATE timer provided in C++ only

	//run this several times because this routine runs very fast 
	//The execution time needs to be at least some seconds in order to have a good measurement (why?) 
	//			because other processes run at the same time too, preempting our thread
	//---Appropriately MODIFY the 'TIMES_TO_RUN' and the input size (defined in the appropriate header file)---
	for (t = 0; t < TIMES_TO_RUN; t++) {

		//output = ConstAdd_default();
		//output = ConstAdd_SSE();
		output = ConstAdd_AVX();


	}



	//auto finish = std::chrono::high_resolution_clock::now(); 
	end_1 = clock(); //end the timer - ignore this for now


	if (Compare_ConstAdd() == 0)
		printf("\n\n\r ----- output is correct -----\n\r");
	else
		printf("\n\n\r -----output is INcorrect -----\n\r");

	printf(" clock() method: %ldms\n", (end_1 - start_1) / (CLOCKS_PER_SEC / 1000));
	//std::chrono::duration<double> elapsed = finish - start;
	//std::cout << "Elapsed time: " << elapsed.count() << " s\n";

	system("pause"); //this command does not let the output window to close
	return 0;
}






