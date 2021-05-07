#include <string>
#include <iostream>

#include "Matrix.h"
#include "Algorithm.h"

// An implementation of the gauss-jordan elimination algorithm
void Algorithm::Solve(Matrix* matrix, Matrix* outResult, bool printStep) {
	// Get row and column counts.
	int targetRow = matrix->GetRows();
	int targetCol = matrix->GetColumns();

	// Account for augmented matrix.
	if (matrix->IsAugmented) targetCol--;

	// Check for bad column count.
	if (targetCol < 1) throw new exception("Bad column count.");

	// The target must be the lesser of the targetCol and targetRow.
	int target = targetRow;
	if (target < targetCol) target = targetCol;

	// Create a copy of the input matrix to work on.
	Matrix* copy = new Matrix(*matrix);

	// Loop through rows of target length.
	for (int pivot = 0; pivot < target; ++pivot) {
		// Check for invalid leading entry.
		if (matrix->GetValue(pivot, pivot) == 0) {
			// Check if we could not rearrange.
			if (!Rearrange(matrix, pivot, pivot)) {
				// If so, we move on to the next column.
				continue;
			}
		}

		// Check if the row is a pivot.
		if (!CheckPivot(copy, pivot, pivot)) {
			// If it is not, make it a pivot row.
		}

		// Check if the other rows are reduced.
		// If not, reduce them.
	}

	// If the result matrix is not NULL, delete it.
	if (outResult != NULL) {
		delete outResult;
	}

	// Set the address of the result matrix to the address of our copied work matrix.
	outResult = copy;
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
	if (colIndex >= matrix->GetColumns() || colIndex < 0) throw new exception("Invalid column index. Out-of-range.");
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
		throw new exception("Matrix sizes should be equal.");
	// Do comparison checks
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
		throw new exception("Row1 index is out-of-range.");
	if (r2 < 0 || r2 >= m2->GetRows())
		throw new exception("Row2 index is out-of-range.");

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

bool Algorithm::Rearrange(Matrix* matrix, int column, int row) {
	// Matrix Size Check.
	if (column < 0 || column >= matrix->GetColumns())
		throw new exception("Column index is out-of-range.");

	// Swapping algo.
	// Conditions for swapping (in order of priority):
	//		1) existance of a leading entry on another row. (firstPass = false)
	//		2) current row has a leading entry of zero. (firstPass = true)

	bool firstPass = false;
	bool changed = false;
	bool force = false;

	// We check if we have a leading entry in another row.
	// If so, we force the algo even if our working row is valid.
	for (int r = 0; r < matrix->GetRows(); ++r) {
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
		for (int r = 0; r < matrix->GetRows(); ++r) {
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