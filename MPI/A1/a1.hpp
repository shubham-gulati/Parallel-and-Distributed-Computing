// File: a1.hpp
// Shubham	
// Gulati
// I AFFIRM THAT WHEN WORKING ON THIS ASSIGNMENT
// I WILL FOLLOW UB STUDENT CODE OF CONDUCT, AND
// I WILL NOT VIOLATE ACADEMIC INTEGRITY RULES
#ifndef A1_HPP
#define A1_HPP

#include <vector>
#include <mpi.h>
#include <cmath>
#include <iostream>

// IMPLEMENT ME!
template <typename T, typename Pred>
void mpi_extract_if(MPI_Comm comm, const std::vector<T>& in, std::vector<T>& out, Pred pred) {
	int size, rank;
	MPI_Comm_size(comm, &size);

	MPI_Datatype MPI_X;
	MPI_Type_contiguous(sizeof(T), MPI_BYTE, &MPI_X);
	MPI_Type_commit(&MPI_X);
	
	std::vector<T> primes;
	std::vector<T> unqeual;
 
	for (int j= 0; j < in.size(); j++) {
		if (pred(in[j])) {
			primes.push_back(in[j]);		
		}
	}

	//using all gather first to send the sizes of data in all processes to calculate the final recv count.
	int *recvSizes = new int[size];
	int *sendVal = new int[1];
	sendVal[0] = primes.size();
	MPI_Allgather(&sendVal[0], 1, MPI_INT, &recvSizes[0], 1, MPI_INT, comm);

	int *sendCounts = new int[size];
	int *recvCounts = new int[size];
	int *sendDisp = new int[size];
	int *recvDisp = new int[size];

	int sum = 0;
	int send_sum = 0;
	for (int i=0;i<size;i++) {
		sendCounts[i] = primes.size()/size;
		send_sum+=sendCounts[i];
	}

	for (int i=0;i<size;i++) {
		recvCounts[i] = recvSizes[i]/size;
		sum+=recvCounts[i];
	}

	out.resize(sum);
	int s = primes.size() - send_sum;
	while (s > 0) {
		unqeual.push_back(primes.back());
		primes.pop_back();
		s--;
	}

	sendDisp[0]= 0;
	recvDisp[0] = 0;
	
	for (int i=1;i<size;i++) {
		sendDisp[i] = sendDisp[i-1] + sendCounts[i-1];
	}

	for (int i=1;i<size;i++) {
		recvDisp[i] = recvDisp[i-1] + recvCounts[i-1];
	}


	//implement AlltoAll to know every processor about the total number of items. Those items needs to be distributed equally among the processors.
	MPI_Alltoallv(&primes[0], &sendCounts[0], &sendDisp[0], MPI_X, &out[0], &recvCounts[0], &recvDisp[0], MPI_X, comm);

	if (unqeual.size() > 0) {
		out.insert(out.end(), unqeual.begin(), unqeual.end());
	}

	MPI_Type_free(&MPI_X);

} // mpi_extract_if

#endif // A1_HPP

