#include <iostream>
#include <string>
#include <Windows.h>
#include <wincon.h>

#include "Matrix.h"
#include "Algorithm.h"

using namespace std;

int main()
{
	srand(time(NULL));

	set_max_decimal(6);

	Matrix* mat = NULL;
	int tests = 0;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	while (true) {
		tests++;
		system("cls");
		int r = 3 + rand() % 2;
		int c = 4 + rand() % 2;
		mat = new Matrix(r, c, true);
		wstring s = L"Test #" + to_wstring(tests) + L"; Matrix Size: " + to_wstring(r) + L"x" + to_wstring(c);
		SetConsoleTitleW(s.c_str());

		for (int i = 0; i < r; ++i)
			for (int j = 0; j < c; ++j)
			{
				int m = -1;
				if (rand() % 2 == 0) {
					m = 1;
				}
				mat->SetValue(i, j, m * ((rand() % 10) + 1));
			}

		cout << "INPUT:\n";
		mat->PrintMatrix();
		sleep(2000);

		Matrix* solved = Algorithm::Solve(mat, true);

		cout << "INPUT:\n";
		mat->PrintMatrix();

		cout << "REDUCED ROW ECHELON FORM:\n";
		
		Matrix* altCopy = new Matrix(*solved);

		solved->PrintMatrix();
		sleep(2000);

		SolutionSet* set = SolutionSet::Parse(solved);
		set->Print();
		system("pause");

		delete mat;
		delete solved;
		delete altCopy;

	}
}