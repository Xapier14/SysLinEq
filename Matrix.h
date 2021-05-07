#pragma once
#include <string>
#include <vector>

using namespace std;
// Minimum matrix row count.
const int MIN_MATRIX_R = 2;
// Minimum matrix column count for non-augmented.
const int MIN_MATRIX_C = 2;
// Minimum matrix coumn count for augmented.
const int MIN_MATRIXAUG_C = 3;

// Converts a double into a string with applied formatting.
string to_string_ex(double val);
string format_cell(double val, int spaceLeft);
string generate_repitition(int rep, char c);
void set_max_decimal(int maxDecimal);

class Matrix;

class Cell {
public:
	double Value;
	Matrix* Parent;
	// Default constructor.
	Cell();
	// Initializes class values.
	void Init(double val, Matrix* parent);
};

class Row {
public:
	Matrix* Parent;
	vector<Cell*> Data;
	int Length;
	// Initializes class values.
	void Init(int length, Matrix* parent);
	// Repoints parent object.
	void Repoint();
};

class Column {
public:
	Matrix* Parent;
	vector<Cell*> Data;
	int Length;
	// Initializes class values.
	void Init(int length, Matrix* parent, vector<Row*> referenceRows, int referenceColumn);
	// Repoints parent object.
	void Repoint();
};

class Matrix {
private:
	int _rows;
	int _columns;
	vector<Row*> _dataRows;
	vector<Column*> _dataColumns;
public:
	bool IsAugmented;
	// Default constructor.
	Matrix(int rows, int columns, bool isAugmented);
	// Destructor.
	~Matrix();
	// Copy Constructor.
	Matrix(const Matrix& src);
	// Repoints data member parents to matrix parent object.
	void Repoint();
	// Get number of rows.
	int GetRows();
	// Get number of columns.
	int GetColumns();
	// Get row object using specified rowIndex.
	Row* GetRow(int rowNumber);
	// Get column object using specified rowIndex.
	Column* GetColumn(int columnIndex);
	// Sets value for a cell with the specified rowIndex and columnIndex.
	void SetValue(int rowIndex, int columnIndex, double val);
	// Gets a value from a cell  with the specified rowIndex and columnIndex.
	double GetValue(int rowIndex, int columnIndex);
	// Formats the matrix to a string array.
	vector<string> FormatString();
	// Prints the matrix to the screen.
	void PrintMatrix();

	// Elementary Row Operations.
	
	// Swaps two rows, these are 0-based indexes.
	void SwapRow(int row1, int row2);
	// Multiplies a row by a non-zero scalar.
	void MultiplyByScalar(int rowIndex, double scalar);
	// Adds a scalar-multiple of a row onto another row.
	void AddInverse(int originRow, int multipleRow, double scalar);

	// Static functions.
	
	// Creates a matrix and repoints its data mambers (kinda un-needed, might remove sometime soon (TM)).
	static Matrix* CreateMatrix(int rows, int columns, bool isAugmented);
};