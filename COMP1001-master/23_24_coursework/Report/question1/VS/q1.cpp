/*
------------------DR VASILIOS KELEFOURAS-----------------------------------------------------
------------------COMP1001 ------------------------------------------------------------------
------------------COMPUTER SYSTEMS MODULE-------------------------------------------------
------------------UNIVERSITY OF PLYMOUTH, SCHOOL OF ENGINEERING, COMPUTING AND MATHEMATICS---
*/


#include <stdio.h>
#include <time.h>
#include <pmmintrin.h>
#include <process.h>
#include <chrono>
#include <iostream>
#include <immintrin.h>
#include <omp.h>

#define M 1024*512
#define ARITHMETIC_OPERATIONS1 3*M
#define TIMES1 10

#define N 8192
#define ARITHMETIC_OPERATIONS2 4*N*N
#define TIMES2 10


//function declaration
void initialize();
void routine1(float alpha, float beta);
void routine2(float alpha, float beta);
void routine1_vec(float alpha, float beta);
void routine2_vec(float alpha, float beta);

__declspec(align(64)) float  y[M], z[M] ;
__declspec(align(64)) float A[N][N], x[N], w[N];


int main() {

    float alpha = 0.023f, beta = 0.045f;
    double run_time, start_time;
    unsigned int t;

    initialize();

    //routine1
    printf("\nRoutine1:");
    start_time = omp_get_wtime(); //start timer

    for (t = 0; t < TIMES1; t++)
        routine1(alpha, beta);
    run_time = omp_get_wtime() - start_time; //end timer
    printf("\n Time elapsed is %f secs \n %e FLOPs achieved\n", run_time, (double)(ARITHMETIC_OPERATIONS1) / ((double)run_time / TIMES1));


    //routine2
    printf("\nRoutine2:");
    start_time = omp_get_wtime(); //start timer

    for (t = 0; t < TIMES2; t++)
        routine2(alpha, beta);

    run_time = omp_get_wtime() - start_time; //end timer
    printf("\n Time elapsed is %f secs \n %e FLOPs achieved\n", run_time, (double)(ARITHMETIC_OPERATIONS2) / ((double)run_time / TIMES2));


    //routine1_vec
    printf("\nRoutine1_vec:");
    start_time = omp_get_wtime(); //start timer

    for (t = 0; t < TIMES1; t++)
        routine1_vec(alpha, beta);

    run_time = omp_get_wtime() - start_time; //end timer
    printf("\n Time elapsed is %f secs \n %e FLOPs achieved\n", run_time, (double)(ARITHMETIC_OPERATIONS1) / ((double)run_time / TIMES1));


    //routine2_vec
    printf("\nRoutine2_vec:");
    start_time = omp_get_wtime(); //start timer

    for (t = 0; t < TIMES2; t++)
        routine2_vec(alpha, beta);

    run_time = omp_get_wtime() - start_time; //end timer
    printf("\n Time elapsed is %f secs \n %e FLOPs achieved\n", run_time, (double)(ARITHMETIC_OPERATIONS2) / ((double)run_time / TIMES2));

    
    return 0;
}

void initialize() {

    unsigned int i, j;

    //initialize routine2 arrays
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            A[i][j] = (i % 99) + (j % 14) + 0.013f;
        }

    //initialize routine1 arrays
    for (i = 0; i < N; i++) {
        x[i] = (i % 19) - 0.01f;
        w[i] = (i % 5) - 0.002f;
    }

    //initialize routine1 arrays
    for (i = 0; i < M; i++) {
        z[i] = (i % 9) - 0.08f;
        y[i] = (i % 19) + 0.07f;
    }


}



void routine1(float alpha, float beta) {

    unsigned int i;


    for (i = 0; i < M; i++)
        y[i] = alpha * y[i] + beta * z[i];

}

void routine2(float alpha, float beta) {

    unsigned int i, j;


    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            w[i] = w[i] - beta + alpha * A[i][j] * x[j];


}

void routine1_vec(float alpha, float beta) {

    __m128 num1, num2, num3, num4, num5;
    unsigned int i;

    for (i = 0; i < (M / 4) * 4; i += 4) {
        num1 = _mm_set1_ps(alpha);
        num2 = _mm_set1_ps(beta);
        num3 = _mm_loadu_ps(&z[i]);
        num4 = _mm_loadu_ps(&y[i]);

        num5 = _mm_add_ps(_mm_mul_ps(num1, num4), _mm_mul_ps(num2, num3));
        
        _mm_storeu_ps(&y[i], num5);

        //padding
        for (; i < M; i++) {
            y[i] = alpha * y[i] + beta * z[i];
        }

    }

}


void routine2_vec(float alpha, float beta) {
    __m128 num1, num2, num3, num4, num5, num6, num7, num8, num9;
    unsigned int i, j;

    for (i = 0; i < (N / 4) * 4; i += 4) {
        for (j = 0; j < (N / 4) * 4; j += 4) {
            num1 = _mm_loadu_ps(&A[i][j]);
            num2 = _mm_loadu_ps(&x[j]);
            num3 = _mm_loadu_ps(&w[i]);

            num4 = _mm_set1_ps(alpha);
            num5 = _mm_set1_ps(beta);
            

            num6 = _mm_mul_ps(num4, num1);
            num7 = _mm_mul_ps(num6, num2);
            num8 = _mm_sub_ps(num3, num5);
            num9 = _mm_add_ps(num8, num7);
            _mm_storeu_ps(&w[i], num9);
        }

    }
    
    //padding
    for (; i < M; i++) {
        for (; j < N; j++) {
            w[i] = w[i] - beta + alpha * A[i][j] * x[j];
        }
    }



}



