#include <iostream>
#include <string>
#include <sstream>

#include "Matrix.h"
#include "Algorithm.h"

using namespace std;

// aux functions
string GetString() {
	string temp = "";
	getline(cin, temp);
	return temp;
}
char GetChar() {
	string temp = GetString();
	if (temp.length() > 0) {
		return temp[0];
	}
	else return ' ';
}
int GetInt() {
	string temp = GetString();
	int r = 0;
	if (temp.length() == 0) return 0;
	try {
		r = stoi(temp);
	}
	catch (exception ex) {}
	return r;
}
// splits a string into a vector list, with spaces as a seperator
vector<string>* Split(string text) {
	vector<string>* ret = new vector<string>();
	stringstream stringBuilder;
	for (int c = 0; c < text.length(); ++c) {
		if (stringBuilder.str() != "" && text[c] == ' ') {
			//new 
			ret->push_back(stringBuilder.str());
			stringBuilder.str(string()); //clear
		}
		else {
			stringBuilder << text[c];
		}
	}
	if (stringBuilder.str() != "") {
		//new word
		ret->push_back(stringBuilder.str());
		stringBuilder.str(string()); //clear
	}
	return ret;
}

// prints and formats a string into a fixed width paragraph.
void PrintParagraph(string text, int max_width) {
	vector<string>* spl = Split(text);
	for (int r = 0, w = 0; r < spl->size(); ++r) {
		if (w + (*spl)[r].length() >= max_width) {
			cout << endl;
			cout << (*spl)[r];
			w = (*spl)[r].length();
		}
		else {
			if (r == 0) {
				cout << (*spl)[r];
				w += (*spl)[r].length();
			}
			else {
				cout << " " << (*spl)[r];
				w += (*spl)[r].length() + 1;
			}
		}
	}
	delete spl;
}

// prints a string into a centered line.
void PrintCentered(string text, int lineWidth) {
	if (text.length() > lineWidth) {
		cout << text.substr(0, lineWidth);
		return;
	}
	int offset = (lineWidth / 2) - (text.length() / 2);
	cout << generate_repitition(offset, ' ');
	cout << text;
	cout << endl;
}

// pauses the console by requesting the user for input
void CPause(string text = "Press enter to continue.") {
	if (text != "") cout << text;
	GetString();
}

// prints the menu title
void PrintTitle() {
	
	cout << " __              _                                   __     __   _" << endl;
	cout << "/ _\\ _   _  ___ | |_   ___  _ __ ___   ___    ___   / _|   / /  (_) _ __    ___   __ _  _ __" << endl;
	cout << "\\ \\ | | | |/ __|| __| / _ \\| '_ ` _ \\ / __|  / _ \\ | |_   / /   | || '_ \\  / _ \\ / _` || '__|" << endl;
	cout << "_\\ \\| |_| |\\__ \\| |_ |  __/| | | | | |\\__ \\ | (_) ||  _| / /___ | || | | ||  __/| (_| || |" << endl;
	cout << "\\__/ \\__, ||___/ \\__| \\___||_| |_| |_||___/  \\___/ |_|   \\____/ |_||_| |_| \\___| \\__,_||_|" << endl;
	cout << "      |___/" << endl;
	cout << "\t\t    __                      _    _" << endl;
	cout << "\t\t   /__\\ __ _  _   _   __ _ | |_ (_)  ___   _ __   ___" << endl;
	cout << "\t\t  /_\\  / _` || | | | / _` || __|| | / _ \\ | '_ \\ / __|" << endl;
	cout << "\t\t //__ | (_| || |_| || (_| || |_ | || (_) || | | |\\__ \\" << endl;
	cout << "\t\t \\__/  \\__, | \\__,_| \\__,_| \\__||_| \\___/ |_| |_||___/" << endl;
	cout << "\t\t                 |_|" << endl;
	
}

// the main entry point of the application.
int main()
{
	// the paragraph length
	const int paraLength = 92;
	// current menu choice
	int choice;

	// the user-inputted matrix
	Matrix* mat = NULL;
	// the result matrix from the solving algorithm
	Matrix* result = NULL;
	// the solution set generated from the parser
	SolutionSet* set = NULL;
	// flag that specifies whether to print steps or not
	bool print = true;
	// the matrix size: n = rows, m = columns.
	int n = 0, m = 0;

	// function that sets the max decimals by the format_cell function
	set_max_decimal(6);

	cout << boolalpha;

	do
	{
		// display menu
		system("CLS");
		PrintTitle();
		cout << endl;
		PrintCentered("-[MENU]-", paraLength);
		cout << endl;
		cout << generate_repitition(paraLength, '-') << endl;
		cout << endl;
		PrintCentered("1. Solve System   2. Settings   3. About Algorithm   4. About Program", paraLength);
		cout << endl;
		cout << generate_repitition(paraLength, '-') << endl;
		cout << "Input: ";
		choice = GetInt();
		system("CLS");
		// consider user choice
		switch (choice)
		{
			case 1:
				while (true) {
					// input matrix size
					cout << "Input augmented matrix size:" << endl;
					cout << "Rows: ";
					n = GetInt();
					cout << "Columns: ";
					m = GetInt();
					if (n < 2 || m < 3) {
						system("cls");
						cout << "Invalid matrix size!" << endl;
						cout << "Matrix must atleast be 2x3." << endl;
						cout << endl;
					}
					else {
						break;
					}
				}
				system("cls");
				try {
					// populate matrix
					mat = Algorithm::InputMatrix(n, m, true);
					// solve system
					result = Algorithm::Solve(mat, print);
					// parse results
					set = SolutionSet::Parse(result);
					set->Print();
				}
				catch (algo_exception* aE) {
					cout << "ERROR! Exception thrown in solving algorithm." << endl;
					cout << "Error code: " << aE->ecode << endl;
					cout << "Error message: " << aE->msg;
				}
				cout << endl << endl;
				CPause();
				delete mat, result, set;
				continue;
			case 2:
				// change settings
				Algorithm::ChangeSettings(&print, paraLength);
				continue;
			case 3:
				// print about algo
				PrintCentered("About Algorithm", paraLength);
				cout << generate_repitition(paraLength, '-') << endl;
				PrintParagraph("Row reduction is the process of performing row operations to transform any matrix into reduced row echelon form. In reduced row echelon form, each successive row of the matrix has less dependencies than the previous, so solving systems of equations is a much easier task. The idea behind row reduction is to convert the matrix into an equivalent version in order to simplify certain matrix computations. Its two main purposes are to solve system of linear equations and calculate the inverse of a matrix.", paraLength);
				cout << endl << endl;
				CPause();
				continue;
			case 4:
				// print about program
				PrintCentered("About Program", paraLength);
				cout << generate_repitition(paraLength, '-') << endl;
				PrintParagraph("This program is a tool written to help visualize the process on using the Gauss-Jordan Algorithm to solve a linear system. It visualizes the step-by-step process on each row operation and shows how each row operation affects the linear system.", paraLength);
				cout << endl;
				cout << endl;
				cout << "Written By:\n\tCrisang, Lance Daniel\n\tGutierrez, Rommel\n\tFlorido, Noel\n\n";
				cout << "Version: 1.0 (Built: 5-27-2021)";
				cout << endl << endl;
				CPause();
				continue;
		}
	} while (true);
	return 0;

}