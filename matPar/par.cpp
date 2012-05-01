#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <mpi.h>

using namespace std;


// constants
#define MaxSize 3000
// tags for sends/receives
#define DATA_TAG 1
#define TERMINATE_TAG 0
#define RESULT_TAG 2
 
int rank;
int size;
int MatSize;
int MatA[MaxSize][MaxSize];
int MatB[MaxSize][MaxSize];
int MatR[MaxSize][MaxSize];
int sendBuffer[MaxSize+1], recvBuffer[MaxSize+1];
double startTime, endTime;

// function prototypes
void master();
void slave();
void init( int[][MaxSize], int, int );

// main magic
int main( int argc, char *argv[] ) {

	// init MPI stuff
		MPI_Init( &argc, &argv );
		MPI_Comm_rank( MPI_COMM_WORLD, &rank );
		MPI_Comm_size( MPI_COMM_WORLD, &size );

		MatSize = atoi( argv[1] );	

	// master process
		if( rank == 0 ) {
			master();			
		}

	// slave process
		else {
			slave();	
		}

	
	MPI_Finalize();
	return 0;
}


// function definitions

// master distributes work and collects results
void master() {

	// local variables
		int rowCounter = 0;
		int doneSlave = 0, doneRow = 0;
		int currentRow = 0;
		MPI_Status status;

	// init matrix B
		init( MatA, 0, MatSize );
		init( MatB, 0, MatSize );

	// start timing
		startTime = MPI_Wtime();

	// broadcast matrix B
		MPI_Bcast( &MatB, MatSize*MatSize, MPI_INT, 0, MPI_COMM_WORLD );

	// seed rows to all available slaves
		for( int i = 1; i < size; i++ ) {

			// fill send buffer
				for( int j = 0; j < MatSize; j++ ) {
					sendBuffer[j] = MatA[i-1][j];
				}
	
			// append row to last 
				sendBuffer[MatSize] = i-1;			

			// send off row	
				MPI_Send( &sendBuffer, MatSize+1, MPI_INT, i, DATA_TAG, MPI_COMM_WORLD );
				currentRow++;
		}

	// receive results from slaves
		while( rowCounter < MatSize ) {
		
		// receive finished rows				
			MPI_Recv( &recvBuffer, MatSize+1, MPI_INT, MPI_ANY_SOURCE, RESULT_TAG, MPI_COMM_WORLD, &status );
			doneSlave = status.MPI_SOURCE;
			doneRow = recvBuffer[MatSize];

		// populate results matrix in correct rows
			for( int i = 0; i < MatSize; i++ ) {
				MatR[doneRow][i] = recvBuffer[i];
			}

		// send new rows back to slave which just finished
			if( currentRow < MatSize ) {

				for( int i = 0; i < MatSize; i++ ) {
					sendBuffer[i] = MatA[currentRow][i];
				}

				MPI_Send( &sendBuffer, MatSize+1, MPI_INT, doneSlave, DATA_TAG, MPI_COMM_WORLD );
			}
	
		// update num rows finished and which row of Matrix A we're on
			rowCounter++;

		// update current row
			sendBuffer[MatSize] = currentRow;
			currentRow++;
		}

     	// all done with rows so send term tag
			for( int i = 1; i < size; i++ ) {
   			MPI_Send( 0, 0, MPI_INT, i, TERMINATE_TAG, MPI_COMM_WORLD );
			}

	for( int i = 0; i < MatSize; i++ ){
		for( int j = 0; j < MatSize; j++ ){
		//	cout << MatR[i][j] << " ";
		}
	//	cout << endl;
	}


	// stop timing
		endTime = MPI_Wtime();
		cout << MatSize << " " << endTime - startTime << endl;
}


// slave does computation work
void slave() {

	// local variables
		MPI_Status status;
		int column;

	// receive matrix B
		MPI_Bcast( &MatB, MatSize*MatSize, MPI_INT, 0, MPI_COMM_WORLD );

	// received seeded row
		MPI_Recv( &recvBuffer, MatSize+1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status );

	// while there's work to do (tag is not term tag)
		while( status.MPI_TAG == DATA_TAG ) {
			
			// grab column 
				column = recvBuffer[MatSize];
	
			// else compute partial result 
				for( int i = 0; i < MatSize; i++  ) {					
					for( int j = 0; j < MatSize; j++ ) {
						sendBuffer[i] += recvBuffer[j] * MatB[j][column];
					}
				}

			
			sendBuffer[MatSize] = column;

			// send results back to master
				MPI_Send( &sendBuffer, MatSize+1, MPI_INT, 0, RESULT_TAG, MPI_COMM_WORLD );

			for(int i = 0; i < MatSize+1; i++)
				sendBuffer[i] = 0;

			// recv next row
				MPI_Recv( &recvBuffer, MatSize+1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
		}
			// exit if term tag
			//	if( status.MPI_TAG == TERMINATE_TAG ) {
			//		exit(0);
			//	}
}

// init function inits strip of matrix A
void init( int mat[][MaxSize], int startRow, int endRow ) {

	// loop over row and col to init matrix
		for( int i = startRow; i < endRow; i++ ) {
			for( int j = 0; j < MatSize; j++ ){
				mat[i][j] = 1;
			}
		}
}
