#pragma once
#include <string>

#include "Matrix.h"

using namespace std;

struct RowAssignment;

class Algorithm {
public:
	// An implementation of the Gauss-Jordan Elimination Algorithm
	static Matrix* Solve(Matrix* matrix, bool printStep);
	static bool CheckPivot(Matrix* matrix, int rowIndex, int colIndex);
	static bool CheckReduced(Matrix* matrix, int rowIndex, int colIndex);
	static void PrintSwapped(Matrix* before, Matrix* after);
	static bool IsRowEqual(Matrix* m1, Matrix* m2, int r1, int r2);
	static bool IsRowEqual(Row* r1, Row* r2);
	static bool Rearrange(Matrix* matrix, int row, int column);
	static vector<RowAssignment> FindChanged(Matrix* oldMatrix, Matrix* newMatrix);
	static int FindRow(Matrix* matrix, Row* ref);
	static Matrix* InputMatrix(int rowSize, int columnSize, bool augmented);
};

class algo_exception : public exception {
public:
	int ecode;
	string msg;
	algo_exception(int code) : exception() {
		ecode = code;
		msg = "";
	}
	algo_exception(int code, string message) : exception(message.c_str()) {
		ecode = code;
		msg = message;
	}
};

struct RowAssignment {
	int oldIndex;
	int newIndex;
	RowAssignment() {
		oldIndex = 0;
		newIndex = 0;
	}
	RowAssignment(int o, int n) {
		oldIndex = o;
		newIndex = n;
	}
};

enum class SolutionType {
	NoSolution,
	UniqueSolution,
	ManySolution
};

class SolutionSet {
private:
	vector<string> _vars;
	SolutionType _sType;
public:
	static SolutionSet* Parse(Matrix* srcMatrix);
	void Print();
};