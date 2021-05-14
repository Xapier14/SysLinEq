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
	string t;
	cout << msg << endl;
	getline(cin, t);
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

		// Check for invalid leading entry.
		if (copy->GetValue(pivotRow, pivotCol) == 0) {
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
				cout << step << ") ";
				step++;
				cout << "Make pivot row:" << endl;
				copy->PrintMatrix(pivotRow, -1, COLOR_PIVOT);
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
				if (!CheckReduced(copy, r, pivotCol + 1)) {
					// If so, we reduce it be getting our scalar
					double scalar = copy->GetValue(r, pivotCol);
					// And multiplying its inverse to the pivot row, and adding it to the current row.
					copy->AddRow(r, pivotRow, true, scalar);
					if (printStep)
					{
						cout << step << ") ";
						step++;
						cout << "Reduce row:" << endl;
						copy->PrintMatrix(r, -1, COLOR_REDUCTION);
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
	if (!CheckReduced(matrix, rowIndex, colIndex)) return false;

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
	// Check if the values to the left of the leading entry are zeros.
	// If found to have non-zeros, return false as the row is not reduced.
	for (int i = 0; i < colIndex; ++i) {
		if (matrix->GetValue(rowIndex, i) != 0) return false;
	}
	// Return true as the row has zero values to its left.
	return true;
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

	// We check if we have a leading entry in another row.
	// If so, we force the algo even if our working row is valid.
	for (int r = row; r < matrix->GetRows(); ++r) {
		if (CheckPivot(matrix, r, column))
			force = true;
	}

	while (true) {
		// We check if we need to rearrange.
		// If not, we return.
		if (matrix->GetValue(row, column) != 0 && !force) {
			break;
		}

		// For each row, compare with our priority.
		for (int r = row; r < matrix->GetRows(); ++r) {
			if (!firstPass) {
				// This is the first pass, meaning: we are checking for our first priority.
				if (CheckPivot(matrix, r, column)) {
					// Our row is a pivot, we can now swap this with the row we are working on.
					matrix->SwapRow(row, r);
					changed = true;
					break;
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
				cout << "\n\n\n\n\n\n\n";
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