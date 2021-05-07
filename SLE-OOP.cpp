#include <iostream>
#include <string>

#include "Matrix.h"
#include "Algorithm.h"

using namespace std;

int main()
{
	//Matrix* mat = Matrix::CreateMatrix(3, 3, true);

	Matrix* mat = new Matrix(3, 4, true);

	set_max_decimal(3);

	/*
		Visualize:
				| 0 | 1 | 2 | c
			| 0	| 6 | 6 | 3 |
			| 1 | 5 | 2 | 5 |
			| 2 | 1 | 8 | 9 |
			  r
	*/


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
	
	mat->PrintMatrix();

	/*
	vector<string> matrixResult = mat->FormatString();
	for (int i = 0; i < matrixResult.size(); ++i) {
		string label = "This is row #" + to_string(i) + " -> ";
		if (i > 0 && i < matrixResult.size() - 1) {
			cout << label;
		}
		else {
			cout << generate_repitition(label.length(), ' ');
		}
		cout << matrixResult[i] << endl;
	}
	*/

	Matrix* result = Algorithm::Solve(mat, false);

	result->PrintMatrix();

	cout << "";

	delete mat;

	char c;
	cin >> c;
}