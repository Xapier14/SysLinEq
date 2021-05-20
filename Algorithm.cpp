#include <string>
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <wincon.h>

#include "Matrix.h"
#include "Algorithm.h"

//Color Codes

const int COLOR_REDUCTION = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
const int COLOR_PIVOT = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
const int COLOR_INPUT = BACKGROUND_GREEN | BACKGROUND_RED;
const int BLOCKWALL_LENGTH = 20;
bool AUTOSKIP = false;
bool CLEARSTEP = false;
int AUTOSKIP_DURATION = 500;

string Fit(string src, int length) {
	stringstream ss;
	for (int i = 0; i < src.length() && i < length; ++i) {
		ss << src[i];
	}
	if (src.length() < length) {
		ss << generate_repitition(length - src.length(), ' ');
	}
	return ss.str();
}

void Pause(string msg) {
	if (AUTOSKIP) {
		cout << endl;
		sleep(AUTOSKIP_DURATION);
		return;
	}
	string t;
	cout << msg << endl;
	getline(cin, t);
}

bool HasOtherLead(Matrix* mat, int pivotRow, int pivotCol) {
	for (int i = pivotRow+1; i < mat->GetRows(); ++i) {
		if (abs(mat->GetValue(i, pivotCol)) == 1) {
			return true;
		}
	}
	return false;
}

// An implementation of the gauss-jordan elimination algorithm
Matrix* Algorithm::Solve(Matrix* matrix, bool printStep) {
	// Get row and column counts.
	int targetRow = matrix->GetRows();
	int targetCol = matrix->GetColumns();
	 
	// Account for augmented matrix.
	if (matrix->IsAugmented) targetCol--;

	// Check for bad column count.
	if (targetCol < 1) throw new algo_exception(1, "Bad column count.");

	// The target must be the greater of the targetCol and targetRow.
	int target = targetRow;
	if (target < targetCol) target = targetCol;

	// Create a copy of the input matrix to work on.
	Matrix* copy = new Matrix(*matrix);

	// Init Step Counter
	int step = 1;

	// Loop through rows of target length.
	for (int pivotCol = 0, pivotRow = 0; pivotCol < target; ++pivotCol, ++pivotRow) {
		// Column Overshoot check
		if (pivotCol >= targetCol) continue;
		// Row Overshoot Check
		if (pivotRow >= targetRow) continue;

		// Check for invalid leading entry or has other leading entry.
		if (copy->GetValue(pivotRow, pivotCol) == 0 || HasOtherLead(copy, pivotRow, pivotCol)) {
			// Create a copy of the matrix if we need to rearrange.
			Matrix* oldCopy = new Matrix(*copy);

			// Check if we could not rearrange.
			if (!Rearrange(copy, pivotRow, pivotCol)) {
				// If we cannot, we move on to the next column.
				// Delete our old copy as we do not need it anymore.
				delete oldCopy;
				// Account for pivot row increment
				--pivotRow;
				continue;
			}
			else {
				// Using the oldCopy matrix, we print the new matrix with labels for which row was switched.
				if (printStep)
				{
					if (CLEARSTEP) {
						system("cls");
					}
					else {
						cout << generate_repitition(BLOCKWALL_LENGTH, '\n');
					}
					cout << step << ") ";
					step++;
					cout << "Swap row(s):" << endl;
					PrintSwapped(oldCopy, copy); //NOTE: might be replaced soon.
					Pause("Press enter to continue to the next step...");
				}
			}
			// Delete our old copy as we do not need it anymore.
			delete oldCopy;
		}

		// Check if the row is a pivot.
		if (!CheckPivot(copy, pivotRow, pivotCol) && pivotCol != copy->GetColumns() - 1) {
			// If it is, get the multiplicative inverse of the leading entry of the pivot.
			double scalar = 1 / copy->GetValue(pivotRow, pivotCol);
			// And multiply it to the row.
			copy->MultiplyByScalar(pivotRow, scalar);
			if (printStep)
			{
				if (CLEARSTEP) {
					system("cls");
				}
				else {
					cout << generate_repitition(BLOCKWALL_LENGTH, '\n');
				}
				cout << step << ") ";
				step++;
				cout << "Make pivot row:" << endl;
				copy->PrintMatrix(pivotRow, -1, COLOR_PIVOT);
				cout << "R" << to_string(pivotRow + 1) << " * (1/" << 1/scalar << ") -> R" << to_string(pivotRow + 1) << endl;
				Pause("Press enter to continue to the next step...");
			}
		}
		// Check if the other rows are reduced.
		// If not, reduce them.
		bool reduced = false;
		for (int r = 0; r < target; ++r) {
			// If current rows exceed target row, break as we can no longer reduce.
			if (r >= targetRow) break;
			// If the current row is not the pivot row
			if (r != pivotRow && pivotCol < target) {
				// We check if the row is reduced
				if (!CheckReduced(copy, r, pivotCol)) {
					// If so, we reduce it be getting our scalar
					double scalar = copy->GetValue(r, pivotCol);
					// And multiplying its inverse to the pivot row, and adding it to the current row.
					copy->AddRow(r, pivotRow, true, scalar);
					if (printStep)
					{
						if (CLEARSTEP) {
							system("cls");
						}
						else {
							cout << generate_repitition(BLOCKWALL_LENGTH, '\n');
						}
						cout << step << ") ";
						step++;
						cout << "Reduce row:" << endl;
						copy->PrintMatrix(r, -1, COLOR_REDUCTION, pivotRow, -1, COLOR_PIVOT);
						cout << "(R" << to_string(pivotRow + 1) << " * " << -scalar << ") + R" << to_string(r+1) << " -> R" << to_string(pivotRow + 1) << endl;
						Pause("Press enter to continue to the next step...");
					}
				}
			}
		}
	}
	return copy;
}

/// <summary>
/// Checks if the specified row is a pivot row.
/// </summary>
bool Algorithm::CheckPivot(Matrix* matrix, int rowIndex, int colIndex) {
	// A prerequisite of a pivot row is that is must be reduced.
	// Here, we check if it is not reduced.
	// If it is not, then it must not be a pivot row.
	//if (!CheckReduced(matrix, rowIndex, colIndex)) return false;

	// Now if it is, we return the result of "leading_entry == 1".
	// As a pivot row, it must have its leading entry equal to 1.
	return matrix->GetValue(rowIndex, colIndex) == 1;
}

/// <summary>
/// Checks if the specified row is reduced.
/// </summary>
bool Algorithm::CheckReduced(Matrix* matrix, int rowIndex, int colIndex) {
	// Get the row to check
	Row* r = matrix->GetRow(rowIndex);
	// Check if the index to check is within the matrix columns.
	if (colIndex >= matrix->GetColumns() || colIndex < 0) throw new algo_exception(3, "Invalid column index. Out-of-range.");
	/*
	// Check if the values to the left of the leading entry are zeros.
	// If found to have non-zeros, return false as the row is not reduced.
	for (int i = 0; i < colIndex; ++i) {
		if (matrix->GetValue(rowIndex, i) != 0) return false;
	}
	*/
	if (matrix->GetValue(rowIndex, colIndex) == 0) return true;
	return false;
}

/// <summary>
/// Prints the swapped rows from two given matrices, the before matrix and after matrix.
/// </summary>
void Algorithm::PrintSwapped(Matrix* before, Matrix* after) {
	// Matrix Size Check.
	if (before->GetRows() != after->GetRows() || before->GetColumns() != after->GetColumns())
		throw new algo_exception(5, "Matrix sizes should be equal.");
	// Define the length of a label.
	const int labelLength = 8;
	// Find row assignments.
	vector<RowAssignment> newAssign = FindChanged(before, after);
	// If we have row assignments.
	if (newAssign.size() > 0) {
		// Get a formatted vector list of string representation of the matrix.
		vector<string> formatted = after->FormatString();

		// For each line
		for (int r = 0; r < formatted.size(); ++r) {
			// Check if the line has a row assignment.
			bool hasAssign = false;
			RowAssignment* rA = NULL;
			// For each found row assignment.
			for (int a = 0; a < newAssign.size(); ++a) {
				// Check if it matches the current row to check.
				if (newAssign[a].newIndex == r - 1) {
					//If it does, store the address.
					rA = &(newAssign[a]);
					hasAssign = true;
					break;
				}
			}
			// If we have a row assignment on the row to print.
			if (hasAssign) {
				// Create the label.
				string label = "R" + to_string(rA->oldIndex + 1) + " -> R" + to_string(rA->newIndex + 1);
				// Fit it to size.
				label = Fit(label, labelLength);
				// Print the label
				cout << " " << label;
			}
			else {
				//If we do not, we print a blank label.
				cout << " " << generate_repitition(labelLength, ' ');
			}
			// We print the formatted line.
			cout << " " << formatted[r] << endl;;
		}
	}
}

/// <summary>
/// Checks if the specified rows are equal.
/// </summary>
bool Algorithm::IsRowEqual(Matrix* m1, Matrix* m2, int r1, int r2) {
	// Check if the matrix column counts are not equal.
	// If so, return false as the rows will not be equal.
	if (m1->GetColumns() != m2->GetColumns())
		return false;

	// Check index range.
	if (r1 < 0 || r1 >= m1->GetRows())
		throw new algo_exception(4, "Row1 index is out-of-range.");
	if (r2 < 0 || r2 >= m2->GetRows())
		throw new algo_exception(4, "Row2 index is out-of-range.");

	// Fetch rows.
	Row* base = m1->GetRow(r1);
	Row* check = m2->GetRow(r2);

	// Compare each values.
	// If a pair is not equal, return false as the rows are not equal.
	for (int i = 0; i < base->Length; ++i) {
		if (base->Data[i] != check->Data[i])
			return false;
	}
	// Return true as both rows are equal.
	return true;
}

/// <summary>
/// Checks if the specified rows are equal.
/// </summary>
bool Algorithm::IsRowEqual(Row* r1, Row* r2) {
	// Length check
	if (r1->Length != r2->Length) return false;

	//For each element of row
	for (int c = 0; c < r1->Length; ++c) {
		// Check if one of them is not equal to the other.
		if (r1->Data[c]->Value != r2->Data[c]->Value)
			// If so, the rows should not be equal.
			return false;
	}

	// If we have passed the for loop, it rows should be equal.
	return true;
}

bool Algorithm::Rearrange(Matrix* matrix, int row, int column) {
	// Matrix Size Check.
	if (column < 0 || column >= matrix->GetColumns())
		throw new algo_exception(3, "Column index is out-of-range.");

	// Swapping algo.
	// Conditions for swapping (in order of priority):
	//		1) existance of a leading entry on another row. (firstPass = false)
	//		2) current row has a leading entry of zero. (firstPass = true)

	bool firstPass = false;
	bool changed = false;
	bool force = false;
	bool positiveForce = false;

	// We check if we have a leading entry in another row.
	// If so, we force the algo even if our working row is valid.
	for (int r = row; r < matrix->GetRows(); ++r) {
		if (abs(matrix->GetValue(r, column)) == 1)
		{
			force = true;
			if (matrix->GetValue(r, column) == 1) {
				positiveForce = true;
			}
		}
	}

	while (true) {
		// We check if we need to rearrange.
		// If not, we return.
		if (matrix->GetValue(row, column) != 0 && !force) {
			break;
		}

		// For each row, compare with our priority.
		for (int r = row+1; r < matrix->GetRows(); ++r) {
			if (matrix->GetValue(r, column) == matrix->GetValue(row, column)) continue;
			if (!firstPass) {
				// This is the first pass, meaning: we are checking for our first priority.
				if (abs(matrix->GetValue(r, column)) == 1) {
					// Our row is a pivot, we can now swap this with the row we are working on.
					
					// We check if the positiveForce flag is raised, meaning we need to swap with a positive leading entry.
					// This swaps either if:
					// Positive force true,
					//		leading entry is a positive.
					// Positive force false,
					//		leading entry can be either positive or negative.
					if (!positiveForce || matrix->GetValue(r, column) != -1)
					{
						matrix->SwapRow(row, r);
						changed = true;
						break;
					}
				}
			}
			else {
				// Second pass.
				if (matrix->GetValue(r, column) != 0) {
					// Our row has a non-zero leading entry, we can now swap this with the row we are working on.
					matrix->SwapRow(row, r);
					changed = true;
					break;
				}
			}
		}
		// Since we completed our pass for our first priority, we can un-force the algo now.
		if (force) force = false;
		if (firstPass) {
			if (!changed)
				// If we have completed our second pass and that nothing has changed, we break out.
				break;
		}
		else {
			// Moving on to our second priority.
			firstPass = true;
		}
	}

	// Return if the algo changed/swapped any rows.
	return changed;
}

vector<RowAssignment> Algorithm::FindChanged(Matrix* oldMatrix, Matrix* newMatrix) {
	// Create our return container.
	vector<RowAssignment> ret;

	// For each row from the oldMatrix
	for (int r = 0; r < oldMatrix->GetRows(); ++r) {
		// Get the row.
		Row* ref = oldMatrix->GetRow(r);
		
		// Find its position in the newMatrix.
		int newR = FindRow(newMatrix, ref);

		// If the row is found, and that it was assigned to a new index.
		if (newR != -1 && newR != r) {
			// Create a RowAssignment and put it in our container.
			ret.push_back(RowAssignment(r, newR));
		}
	}

	// Return our results.
	return ret;
}

int Algorithm::FindRow(Matrix* matrix, Row* ref) {
	// Check length.
	if (matrix->GetColumns() != ref->Length)
		throw new algo_exception(7, "Row size should be equal.");

	// For each row of the matrix.
	for (int r = 0; r < matrix->GetRows(); ++r) {
		// We get the row to check.
		Row* chk = matrix->GetRow(r);

		// Then we compare it if it is equal to our reference row.
		if (IsRowEqual(chk, ref)) {
			// If it does, then its row index is what we need to return.
			return r;
		}
	}
	
	// Retrun -1 if the row cannot be found.
	return -1;
}

double GetDouble() {
	string t = "";
	double r = 0;
	getline(cin, t);
	r = stod(t);
	return r;
}

Matrix* Algorithm::InputMatrix(int rowSize, int columnSize, bool augmented) {
	Matrix* mat = new Matrix(rowSize, columnSize, augmented);
	for (int r = 0; r < rowSize; ++r) {
		for (int c = 0; c < columnSize; ++c) {
			string eMsg = "";
			while (true) {
				cout << generate_repitition(40, '\n');
				if (eMsg != "") {
					cout << "ERROR: " << eMsg << endl;
				}
				cout << "Input Matrix:" << endl;
				mat->PrintMatrix(r, c, COLOR_INPUT);
				try {
					cout << "Input Value for M(" << r + 1 << ", " << c + 1 << "): ";
					mat->SetValue(r, c, GetDouble());
					break;
				}
				catch (exception e){
					eMsg = "Invalid input.";
					continue;
				}
			}
		}
	}
	return mat;
}

bool IsNegative(double num) {
	return num < 0;
}

SolutionSet* SolutionSet::Parse(Matrix* srcMatrix) {
	if (!srcMatrix->IsAugmented) throw new algo_exception(8, "Matrix is not augmented.");
	SolutionSet* ret = new SolutionSet();
	ret->_sType = SolutionType::UniqueSolution;
	// Do stuff.
	for (int r = 0; r < srcMatrix->GetRows(); ++r) {
		int definition = -1;
		bool hasNonConst = false;
		bool nonZeroConst = srcMatrix->GetValue(r, srcMatrix->GetColumns() - 1) != 0;
		bool allZero = true;
		bool initial = true;
		stringstream ss;
		for (int c = 0; c < srcMatrix->GetColumns() - 1; ++c) {
			if (srcMatrix->GetValue(r, c) != 0)
			{
				allZero = false;
				// we dont have a definition yet,
				if (definition < 0) {
					//this will be our indep var
					definition = c;
					ss << "X" << c + 1 << " =";
				}
				else {
					hasNonConst = true;
					//this is a dep var
					if (initial) {
						initial = false;
						ss << " " << -srcMatrix->GetValue(r, c) << "(X" << c + 1 << ")";
					}
					else
					{
						if (IsNegative(srcMatrix->GetValue(r, c))) {
							// write addition
							ss << " + " << abs(srcMatrix->GetValue(r, c)) << "(X" << c + 1 << ")";
						}
						else {
							// write subtraction
							ss << " - " << abs(srcMatrix->GetValue(r, c)) << "(X" << c + 1 << ")";
						}
					}
				}
			}
		}
		if (nonZeroConst && allZero) {
			ret->_sType = SolutionType::NoSolution;
			ss.clear();
			ret->_vars.clear();
			break;
		}
		if (definition > -1)
		{
			if (hasNonConst) {
				ret->_sType = SolutionType::ManySolution;
				if (srcMatrix->GetValue(r, srcMatrix->GetColumns() - 1) != 0) {
					if (IsNegative(srcMatrix->GetValue(r, srcMatrix->GetColumns() - 1))) {
						// write subtraction
						ss << " - " << abs(srcMatrix->GetValue(r, srcMatrix->GetColumns() - 1));
					}
					else {
						// write addition
						ss << " + " << abs(srcMatrix->GetValue(r, srcMatrix->GetColumns() - 1));
					}
				}
			}
			else {
				ss << " " << srcMatrix->GetValue(r, srcMatrix->GetColumns() - 1);
			}
			if (ss.str().length() > 0) ret->_vars.push_back(ss.str());
		}
	}
	return ret;
}
void SolutionSet::Print() {
	switch (_sType) {
		case SolutionType::NoSolution:
			cout << "Solution Type:\n\t-NO SOLUTION-\n" << endl;
			cout << "Remarks:\n\tThe system is inconsistent." << endl;
			return;
		case SolutionType::UniqueSolution:
			cout << "Solution Type:\n\t-UNIQUE SOLUTION-\n" << endl;
			cout << "Solution Set:" << endl;
			break;
		case SolutionType::ManySolution:
			cout << "Solution Type:\n\t-MANY/INFINITE SOLUTIONS-\n" << endl;
			cout << "Solution Set:" << endl;
			break;
	}
	for (int e = 0; e < _vars.size(); ++e) {
		cout << "\t" << _vars[e] << endl;
	}
}

void PrintCenteredex(string text, int lineWidth) {
	if (text.length() > lineWidth) {
		cout << text.substr(0, lineWidth);
		return;
	}
	int offset = (lineWidth / 2) - (text.length() / 2);
	cout << generate_repitition(offset, ' ');
	cout << text;
	cout << endl;
}
string GetStringex() {
	string temp = "";
	getline(cin, temp);
	return temp;
}
int GetIntex() {
	string temp = GetStringex();
	int r = 0;
	if (temp.length() == 0) return 0;
	try {
		r = stoi(temp);
	}
	catch (exception ex) {}
	return r;
}
void Algorithm::ChangeSettings(bool* pS, int pL) {
	bool retu;
	int choice;
	string eM = "";
	while (true)
	{
		retu = false;
		if (eM != "") cout << eM;
		eM = "";
		cout << endl;
		PrintCenteredex("-[SETTINGS]-", pL);
		cout << endl;
		cout << generate_repitition(pL, '-') << endl;
		cout << endl;
		cout << " 1. Print Step = " << *pS << endl;
		cout << " 2. Clear After Step = " << CLEARSTEP << endl;
		cout << " 3. Autoplay Step = " << AUTOSKIP << endl;
		cout << " 4. Autoplay Step Duration = " << AUTOSKIP_DURATION << "ms" << endl;
		cout << " 5. Return to menu" << endl;
		cout << endl;
		cout << generate_repitition(pL, '-') << endl;
		cout << "Input: ";
		choice = GetIntex();
		switch (choice) {
		case 1:
			*pS = !*pS;
			break;
		case 2:
			CLEARSTEP = !CLEARSTEP;
			break;
		case 3:
			AUTOSKIP = !AUTOSKIP;
			break;
		case 4:
			cout << "Enter new value for duration (in ms): ";
			choice = GetIntex();
			if (choice >= 200 && choice <= 5000) {
				AUTOSKIP_DURATION = choice;
			}
			else {
				eM = "Value out-of-range. Range: 200-5000.\n";
			}
			break;
		case 5:
			retu = true;
			break;
		}
		system("cls");
		if (retu) break;
	}
}