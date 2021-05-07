#pragma once
#include <string>

#include "Matrix.h"

using namespace std;

class Algorithm {
public:
	// An implementation of the Gauss-Jordan Elimination Algorithm
	static void Solve(Matrix* matrix, Matrix* outResult, bool printStep);
	static bool CheckPivot(Matrix* matrix, int rowIndex, int colIndex);
	static bool CheckReduced(Matrix* matrix, int rowIndex, int colIndex);
	static void PrintSwapped(Matrix* before, Matrix* after);
	static bool IsRowEqual(Matrix* m1, Matrix* m2, int r1, int r2);
	static bool Rearrange(Matrix* matrix, int column, int row);
};