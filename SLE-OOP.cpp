#include <iostream>
#include <string>

#include "Matrix.h"
#include "Algorithm.h"

using namespace std;

int main()
{
	//Matrix* mat = Matrix::CreateMatrix(3, 3, true);

	//Matrix* mat = new Matrix(3, 4, true);

	set_max_decimal(3);

	/*	Sample Input:
	
		for (int row = 0; row < rowSize; ++row)
			for (int col = 0; col < columnSize; ++col)
			{
				string t = "";
				getline(cin, t);
				mat->SetValue(row, col, stod(t));
			}
	*/

	/*
	mat->SetValue(0, 0, 0);
	mat->SetValue(0, 1, 6);
	mat->SetValue(0, 2, 3);
	mat->SetValue(0, 3, 7);
	mat->SetValue(1, 0, 1);
	mat->SetValue(1, 1, 2);
	mat->SetValue(1, 2, 5);
	mat->SetValue(1, 3, 10);
	mat->SetValue(2, 0, 1);
	mat->SetValue(2, 1, 8);
	mat->SetValue(2, 2, 9);
	mat->SetValue(2, 3, 12);
	*/

	Matrix* mat = Algorithm::InputMatrix(6, 4, true);

	cout << "Input:" << endl;

	//mat->PrintMatrix();
	mat->PrintMatrix(-1, -1, -1);

	Matrix* result = Algorithm::Solve(mat, true);

	cout << "Reduced Row Echelon Form:" << endl;

	result->PrintMatrix();

	cout << "";

	char c;
	cin >> c;
	delete mat;
}