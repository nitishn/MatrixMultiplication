//============================================================================
// Name        : matSeq.cpp
// Author      : Nitish Narala
// Version     :
// Copyright   : 
// Description : Sequential matrix multiplication
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
using namespace std;

// constants
int rowM1 = 0;
int colM1 = 0;
int colM2 = 0;
int **Mat1;
int **Mat2;
int **MatR;
timeval start, end;
double runTime = 0.0;

// function prototypes
void init();

void calc();

void print();

// main program
 int main( int argc, char **argv ) {

	 // check if arguments are correct and set arguments
		 if( argc < 3 )
			 return 0;
		 else {
			 rowM1 = atoi( argv[1] );
			 colM1 = atoi( argv[2] );
			 colM2 = atoi( argv[3] );

		 // dynamically allocate arrays and fill with random data
			 init();
		 }

	 // start timing
		 gettimeofday( &start, NULL );

	 // do the computation stuff
		 calc();

	 // stop timing
		gettimeofday( &end, NULL );

		// calc TIme
		runTime = ( end.tv_sec - start.tv_sec ) * 1000.0;
		runTime += ( end.tv_usec - start.tv_usec ) / 1000.0;
		cout << rowM1 << " " << colM1 << " " << colM2 << " took: " << runTime/1000.0 << " sec" << endl;

	 // print to screen or something
		 //print();

	 return 0;
 }

// function definitions
void init() {

	// allocate memory for three matrices
		Mat1 = new int* [rowM1];
		Mat2 = new int* [colM1];
		MatR = new int* [rowM1];
		for( int i = 0; i < rowM1; i++ )
			Mat1[i] = new int[colM1];
		for( int i = 0; i < colM1; i++ )
			Mat2[i] = new int[colM2];
		for( int i = 0; i < rowM1; i++ )
			MatR[i] = new int[colM2];

	// randomly generate numbers for input matrices
		srand( time(NULL) );

	// Mat1
		for( int i = 0; i < rowM1; i++ ) {
			for( int j = 0; j < colM1; j++ ) {
				Mat1[i][j] = rand() % 1000;
			}
		}

	// Mat2
		for( int i = 0; i < colM1; i++ ) {
			for( int j = 0; j < colM2; j++ ) {
				Mat2[i][j] = rand() % 1000;
			}
		}
}

void calc() {

	// MatR(i,j) = Mat1(i,k) * Mat2(k,j)
		for( int i = 0;  i < rowM1; i++ ){
			for( int j = 0; j < colM2; j++ ) {
				for( int k = 0; k < colM1; k++ ) {
					MatR[i][j] += Mat1[i][k] * Mat2[k][j];
				}
			}
		}
}

void print() {

	// Mat1
		for( int i = 0; i < rowM1; i++ ) {
			for( int j = 0; j < colM1; j++ ) {
				cout << Mat1[i][j] << " ";
			}
			cout << endl;
		}

	// Mat2
		for( int i = 0; i < colM1; i++ ) {
			for( int j = 0; j < colM2; j++ ) {
				cout << Mat2[i][j] << " " ;
			}
			cout << endl;
		}

	// Mat Result
		for( int i = 0; i < rowM1; i++ ) {
			for( int j = 0; j < colM2; j++ ) {
				cout << MatR[i][j] << " " ;
			}
			cout << endl;
		}
}
