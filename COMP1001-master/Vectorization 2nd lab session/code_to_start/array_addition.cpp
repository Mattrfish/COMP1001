#include "array_addition.h"

float  X1[M2], X2[M2], Y1[M2], test2[M2];

void initialization_Add() {

	float e = 0.1234f, p = 0.7264f, r = 0.11f;

	for (unsigned int j = 0; j != M2; j++) {
		Y1[j] = 0.0;
		test2[j] = 0.0;
		X1[j] = (j % 7) + r;
		X2[j] = (j % 13) + e;
	}
}


unsigned short int Add_default() {

	for (int j = 0; j < M2; j++) {
		Y1[j] = X1[j] + X2[j];
	}


	return 0;
}

unsigned short int Add_SSE() {

	__m128 num1, num2, num3;
	
	for (int i = 0; i < M2; i += 4) {
		num1 = _mm_loadu_ps(&X1[i]);
		num2 = _mm_loadu_ps(&X2[i]);
		num3 = _mm_add_ps(num1, num2);
		_mm_storeu_ps(&Y1[i], num3);
	}
	return 0;
}

unsigned short int Add_SSE_Any_Size() {

	__m128 num1, num2, num3;
    int i;

	for (i = 0; i < (M2/4)*4; i += 4) {
		num1 = _mm_loadu_ps(&X1[i]);
		num2 = _mm_loadu_ps(&X2[i]);
		num3 = _mm_add_ps(num1, num2);
		_mm_storeu_ps(&Y1[i], num3);
	}

	//padding
	for (i=((M2/4)*4); i < M2; i++) {
		Y1[i] = X1[i] + X2[i];
	}
	return 0;
}


unsigned short int Add_AVX() {

	__m256  ymm1, ymm2, ymm3;

	for (int i = 0; i < M2; i += 8) {
		ymm1 = _mm256_loadu_ps(&X1[i]);
		ymm2 = _mm256_loadu_ps(&X2[i]);
		ymm3 = _mm256_add_ps(ymm1, ymm2);
		_mm256_storeu_ps(&Y1[i], ymm3);


	}

	return 0;
}


unsigned short int Add_AVX_Any_Size() {

	__m256 ymm1, ymm2, ymm3;
	int i;

	for (i = 0; i < (M2 / 8) * 8; i += 8) {
		ymm1 = _mm256_loadu_ps(&X1[i]);
		ymm2 = _mm256_loadu_ps(&X2[i]);
		ymm3 = _mm256_add_ps(ymm1, ymm2);
		_mm256_storeu_ps(&Y1[i], ymm3);
	}
		//padding
		
	for (i = (M2 / 8) * 8; i < M2; i++) {
			Y1[i] = X1[i] + X2[i];
	}

	return 0;
}


unsigned short int Compare_Add() {


	for (int j = 0; j < M2; j++) {
		test2[j] = X1[j] + X2[j];
	}


	for (int j = 0; j < M2; j++)
		if (equal(Y1[j], test2[j]) == 1) {
			//printf("\n j=%d\n", j);
			return 1;
		}

	return 0;
}



