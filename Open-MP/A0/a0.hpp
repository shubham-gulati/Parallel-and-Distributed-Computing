//File: a0.hpp
// Shubham	
// Gulati
// I AFFIRM THAT WHEN WORKING ON THIS ASSIGNMENT
// I WILL FOLLOW UB STUDENT CODE OF CONDUCT, AND
// I WILL NOT VIOLATE ACADEMIC INTEGRITY RULES
#ifndef A0_HPP
#define A0_HPP

#include <vector>
#include <iostream>
#include <list>
#include <omp.h>
// IMPLEMENT ME!

template <typename T, typename Pred>
void omp_extract_if(const std::vector<T>& in, std::vector<T>& out, Pred pred) {


int out_size = 0;
int s = in.size();
int size = 0;
std::vector<std::vector<int>> vec_private;
int prefix_val = 0;
int num_threads = omp_get_max_threads();

int partitions = s/num_threads;
vec_private.resize(num_threads);

#pragma omp parallel for
for (int i=0; i<num_threads; i++) {
	for (int j= i*partitions; j<(i+1)*partitions && j < in.size(); j++) {
		if (pred(in[j])) {
			vec_private[i].push_back(in[j]);
		}
	}
}

for (int i=0;i<vec_private.size();i++) {
	size += vec_private[i].size();
	out.resize(size);
}

#pragma omp parallel for
for (int i=0;i<vec_private.size();i++) {
	for (int j=0;j<vec_private[i].size();j++) {
		out[j+prefix_val] = vec_private[i][j]; 
	}
	prefix_val = vec_private[i].size();
}

} // omp_extract_if

#endif // A0_HPP
