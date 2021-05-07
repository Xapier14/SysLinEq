#include <string>
#include <iostream>
#include <vector>
#include "Matrix.h"

// The amount of decimals present in formatted cells.
int MAX_DECIMALS = 2;

/// <summary>
/// Sets the decimal count shown in formatted cells.
/// </summary>
void set_max_decimal(int maxDecimal) {
	if (maxDecimal < 1) throw new exception("Max decimal must be greater than 0.");
	if (maxDecimal > 10) throw new exception("Max decimal must not be greater than 10.");
	MAX_DECIMALS = maxDecimal;
}

/// <summary>
/// Creates a string representation of a double. Removes trailing zeros.
/// </summary>
string to_string_ex(double val) {
	// Create our initial string representation.
	string temp = to_string(val);

	// Find the position of the decimal point.
	int dec = temp.find_first_of('.');

	// If it does not exist, it means we have a whole number.
	if (dec == string::npos)
	{
		//We add ".0" to the end for style and formatting consistency.
		temp = temp + ".0";
	}
	else {
		// If the number is a fraction.
		// Get the number of decimal points via the difference of the position of the point and the length.
		dec = temp.length() - dec - 1;

		// Check if the number of points is greater than the allowed max.
		if (dec > MAX_DECIMALS) {
			// If so, delete the excess points.
			temp.erase(temp.length() - (dec - MAX_DECIMALS));
		}

		// Remove trailing zeros.
		temp.erase(temp.find_last_not_of('0') + 1, string::npos);
	}

	// Last check for orphaned decimal points, add zero if needed.
	if (temp[temp.length() - 1] == '.') temp = temp + '0';
	return temp;
}

/// <summary>
/// Formats the double into a fixed-width cell.
/// </summary>
string format_cell(double val, int spaceLeft) {
	string result = ""; //this our final result
	string data = to_string_ex(val); //let's get a string representation of the double first.

	int dPos = data.find_first_of('.'); //this gives us the position of the decimal point, as well as the amount of whole numbers
	int dCount = data.length() - (dPos + 1); //number of decimal points

	//we check if there is too much data to the left of the string representation
	if (dPos > spaceLeft) {
		//if it is, we simply clip it
		result = "~" + data.substr(dPos - spaceLeft + 1, data.length() - (dPos - spaceLeft + 1));
	}
	else {
		//if it is not, we pad it to match the allotted space
		for (int i = 0; i < spaceLeft - dPos; ++i) result += " ";
		result = result + data;
	}

	//we check if the number of decimals are lesser than the max
	if (dCount < MAX_DECIMALS) {
		//if it does, we pad the right side of the string to meet the desired cell length
		for (int i = 0; i < MAX_DECIMALS - dCount; ++i) result += "0";
	}

	//return our final result
	return result;
}

/// <summary>
/// Generates a string of repeated characters.
/// </summary>
string generate_repitition(int rep, char c) {
	string ret = ""; //result

	//creates a repetition of the specified character c
	for (int i = 0; i < rep; ++i) ret += c;

	//returns the repetition
	return ret;
}


// Matrix methods


Cell::Cell() {
	Value = 0;
	Parent = NULL;
}

void Cell::Init(double val, Matrix* parent) {
	//initializes the value of the cell, as well as its parent matrix
	Value = val;
	Parent = parent;
}


void Row::Init(int length, Matrix* parent) {
	//we check if the row_cell length is shorter than the allowed minimum, if it does: we throw an exception
	if (length < MIN_MATRIX_R) throw new exception(("Row length must not be less than " + to_string(MIN_MATRIX_R) + ".").c_str());

	//initializes & creates the vector list that we store addresses
	Data = vector<Cell*>();
	Length = length;

	//for each uncreated cell, we create an instance of the cell class and store its pointer
	for (int i = 0; i < length; ++i) {
		Cell* d = new Cell();
		//we initialize this by setting the parent.
		d->Init(0, parent); //however, the initial parent is usually just NULL
		//we then add it to the vector list
		Data.push_back(d);
	}

	//sets the parent matrix to the specified address
	Parent = parent; //again, this is usually just NULL
}

void Row::Repoint() {
	//for each cell that is apart of the row, we repoint their parent matrix to the parent matrix of the row
	for (int i = 0; i < Length; ++i) {
		Data[i]->Parent = Parent;
	}
}

void Column::Init(int length, Matrix* parent, vector<Row*> referenceRows, int referenceColumn) {
	//we check if the column_cell length is shorter than the allowed minimum, if it does: we throw an exception
	if (length < MIN_MATRIX_C) throw new exception(("Column length must not be less than " + to_string(MIN_MATRIX_C) + ".").c_str());

	//initializes & creates the vector list that we store addresses
	Data = vector<Cell*>();
	Length = length;

	//for each uncreated cell, we get the address of the correct cell by referencing the already made rows.
	//by doing so, rows and columns reference the same cells
	for (int r = 0; r < referenceRows.size(); ++r) { //for each row
		Data.push_back(referenceRows[r]->Data[referenceColumn]); //we get the address of the cell and store it in the vector list
	}
}

void Column::Repoint() {
	//for each cell that is apart of the column, we repoint their parent matrix to the parent matrix of the column
	for (int i = 0; i < Length; ++i) {
		Data[i]->Parent = Parent;
	}
}

Matrix::Matrix(int rows, int columns, bool isAugmented) {
	//matrix size constraint check
	if (isAugmented) {
		//if it is an augmented matrix and that the column count is less than the minimum
		//we throw an exception
		if (columns < MIN_MATRIXAUG_C) throw new exception(("Number of columns must not be less than " + to_string(MIN_MATRIXAUG_C) + ".").c_str());
	}
	else {
		//if it is NOT an augmented matrix and that the column count is less than the minimum
		//we throw an exception
		if (columns < MIN_MATRIX_C) throw new exception(("Number of columns must not be less than " + to_string(MIN_MATRIX_C) + ".").c_str());
	}
	//if the row count is less than the minimum
	//we throw an exception
	if (rows < MIN_MATRIX_R) throw new exception(("Number of rows must not be less than " + to_string(MIN_MATRIX_R) + ".").c_str());

	//we set our private data members to the specified counts of rows and columns
	_rows = rows;
	_columns = columns;

	//we create vector lists for our row and column addresses
	_dataRows = vector<Row*>();
	_dataColumns = vector<Column*>();

	//init rows
	for (int i = 0; i < _rows; ++i) {
		//create a new instance of our row class
		Row* rw = new Row();

		//we initialize it by specifying the amount of cells which is the same as
		//the amount of columns
		rw->Init(columns, this);

		//we add the newly initialized row to our list of row addresses.
		_dataRows.push_back(rw);
	}

	//init columns
	for (int i = 0; i < _rows; ++i) {
		//create a new instance of our column class
		Column* cl = new Column();

		//we initialize by specifying the amount of cells which is the same as the number of rows
		//only this time, we give it our list of rows
		//we do this so that we can references the cells made by the rows instead of creating new cells

		//doing it this way gives us more flexibility as cells modified from
		//a row object reflects onto a column with the same cell and vise-versa
		cl->Init(rows, this, _dataRows, i);

		//we add the initialized column to our list of column addresses.
		_dataColumns.push_back(cl);
	}
	//Note: cells are created from the row init() code,
	//		rows get the same cells via the same pointer.

	IsAugmented = isAugmented;
}

Matrix::~Matrix() {
	//for each row
	for (int r = 0; r < _rows; ++r) {
		//and for each cell in the row
		for (int c = 0; c < _columns; ++c) {
			//we delete it
			delete _dataRows[r]->Data[c];
		}
		//after deleting the row's cells
		//we then delete the row.
		delete _dataRows[r];
	}
	//we do the same for the columns
	//but as we have already deleted all of the cells
	//we do not need to do it again
	for (int c = 0; c < _columns; ++c) {
		delete _dataColumns[c];
	}
}

Matrix::Matrix(const Matrix& src) {

	// NOTE: the code below was copy-pasted from the constructor above (excluding size checks).
	// There might have been a better way to do this instead of creating redundant code.
	
	//we set our private data members to the specified counts of rows and columns
	_rows = src._rows;
	_columns = src._columns;

	//we create vector lists for our row and column addresses
	_dataRows = vector<Row*>();
	_dataColumns = vector<Column*>();

	//init rows
	for (int i = 0; i < _rows; ++i) {
		//create a new instance of our row class
		Row* rw = new Row();

		//we initialize it by specifying the amount of cells which is the same as
		//the amount of columns
		rw->Init(src._columns, this);

		//we add the newly initialized row to our list of row addresses.
		_dataRows.push_back(rw);
	}

	//init columns
	for (int i = 0; i < _rows; ++i) {
		//create a new instance of our column class
		Column* cl = new Column();

		//we initialize by specifying the amount of cells which is the same as the number of rows
		//only this time, we give it our list of rows
		//we do this so that we can references the cells made by the rows instead of creating new cells

		//doing it this way gives us more flexibility as cells modified from
		//a row object reflects onto a column with the same cell and vise-versa
		cl->Init(src._rows, this, _dataRows, i);

		//we add the initialized column to our list of column addresses.
		_dataColumns.push_back(cl);
	}
	//Note: cells are created from the row init() code,
	//		rows get the same cells via the same pointer.

	IsAugmented = src.IsAugmented;

	// Now that we have created a matrix that has exactly the same properties as our source matrix.
	// We now need to copy the values from the source matrix to our new matrix.

	for (int r = 0; r < _rows; ++r) {
		for (int c = 0; c < _columns; ++c) {
			// The value is fetched via the private member collection '_dataRows'.
			// Which is then used to get 'r' that has a cell collection that is used to get the cell for column 'c'.
			SetValue(r, c, src._dataRows[r]->Data[c]->Value);
		}
	}

	// We should now have a 1:1 copy of the source matrix.
}

void Matrix::Repoint() {
	for (int i = 0; i < _rows; ++i) {
		_dataRows[i]->Parent = this;
		_dataRows[i]->Repoint();
	}
	for (int i = 0; i < _columns; ++i) {
		_dataColumns[i]->Parent = this;
		_dataColumns[i]->Repoint();
	}
}

int Matrix::GetRows() {
	return _rows;
}
int Matrix::GetColumns() {
	return _columns;
}

Row* Matrix::GetRow(int rowIndex) {
	if (rowIndex < 0 || rowIndex >= _rows) throw new exception("Row index out-of-range.");
	return _dataRows[rowIndex];
}

Column* Matrix::GetColumn(int columnIndex) {
	if (columnIndex < 0 || columnIndex >= _rows) throw new exception("Column index out-of-range.");
	return _dataColumns[columnIndex];
}
void Matrix::SetValue(int rowIndex, int columnIndex, double val) {
	if (columnIndex < 0 || columnIndex >= _rows || rowIndex < 0 || rowIndex >= _rows) throw new exception("Index out-of-range.");
	_dataRows[rowIndex]->Data[columnIndex]->Value = val;
}
double Matrix::GetValue(int rowIndex, int columnIndex) {
	if (columnIndex < 0 || columnIndex >= _rows || rowIndex < 0 || rowIndex >= _rows) throw new exception("Index out-of-range.");
	return _dataRows[rowIndex]->Data[columnIndex]->Value;
}
vector<string> Matrix::FormatString() {
	vector<string> ret = vector<string>();
	//char styling
	//ul = ┌, dl = └, ur = ┐, dr = ┘, as = │
	char ul = 218, dl = 192, ur = 191, dr = 217, as = 179;
	char cs = ' ', ds = 179;

	//get com cell width
	int w = 0;
	for (int r = 0; r < _rows; ++r) {
		for (int c = 0; c < _columns; ++c) {
			string temp = to_string_ex(GetValue(r, c));
			int size = temp.find_first_of('.');
			if (size != string::npos) {
				if (size > w) w = size;
			}
		}
	}
	//create string representation
	for (int r = 0; r < _rows; ++r) {
		string lineTemp = "";
		lineTemp = lineTemp + as;
		lineTemp = lineTemp + " ";
		for (int c = 0; c < _columns - 1; ++c) {
			lineTemp = lineTemp + format_cell(GetValue(r, c), w);
			if (c != _columns - 2) {
				lineTemp = lineTemp + cs + " ";
			}
			else {
				if (IsAugmented)
				{
					lineTemp = lineTemp + " " + ds + " ";
				}
				else {
					lineTemp = lineTemp + cs + " ";
				}
			}
		}
		lineTemp = lineTemp + format_cell(GetValue(r, _columns - 1), w);
		lineTemp = lineTemp + " ";
		lineTemp = lineTemp + as;
		ret.push_back(lineTemp);
	}
	int udLen = ret[0].length() - 2;
	string u = ul + generate_repitition(udLen, ' ') + ur;
	string d = dl + generate_repitition(udLen, ' ') + dr;
	ret.insert(ret.begin(), 1, u);
	ret.push_back(d);
	return ret;
}

void Matrix::PrintMatrix() {
	vector<string> r = FormatString();
	for (int i = 0; i < r.size(); ++i) cout << r[i] << endl;
}

Matrix* Matrix::CreateMatrix(int rows, int columns, bool isAugmented) {
	Matrix* ret = new Matrix(rows, columns, isAugmented);
	ret->Repoint();

	return ret;
}

void Matrix::SwapRow(int row1, int row2) {
	// Range checks.
	if (row1 < 0 || row1 >= _rows) throw new exception("Row 1 is out-of-range.");
	if (row2 < 0 || row2 >= _rows) throw new exception("Row 2 is out-of-range.");

	// Get rows.
	Row* r1 = GetRow(row1);
	Row* r2 = GetRow(row2);

	// Store row 2 on temporary var.
	vector<double> temp = vector<double>();
	for (int i = 0; i < r2->Length; ++i) {
		temp.push_back(r2->Data[i]->Value);
	}

	// Swap values between row1 and temp data.
	for (int i = 0; i < temp.size(); ++i) {
		// Swap r2 first as r1 will have its data changed later.
		r2->Data[i]->Value = r1->Data[i]->Value;
		// Swap r1 with temp.
		r1->Data[i]->Value = temp[i];
	}
}