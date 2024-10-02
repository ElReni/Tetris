#include "TMatrixMap.h"

//Colors for drawing matrix cells (matrix map)
#define CLR_YELLOW RGB(255, 232, 0)
#define CLR_RED RGB(255, 69, 0)
#define CLR_BLUE RGB(0, 191, 255)
#define CLR_GREEN RGB(0, 128, 0)
#define CLR_PURPLE RGB(123, 104, 238)
#define CLR_LIGHTGREEN RGB(173, 255, 47)
#define CLR_CYAN RGB(0, 255, 255)

//Constructor. Field sizes are specified in coordinates and number of rows\columns
TMatrixMap::TMatrixMap(int x, int y, int width, int height, int row, int column)
{
	int i, j;

	StartX = x;
	StartY = y;
	CellWidth = width / column; //Dimensions of one cell
	CellHeight = height / row;
	RowCount = row;
	ColumnCount = column;
	Matrix = (int**)calloc(row, sizeof(int*)); //Main game matrix for displaying game model
	TempMatrix = (int**)calloc(row, sizeof(int*)); //Additional temporary matrix for calculations
	for (i = 0; i < row; i++)
	{
		Matrix[i] = (int*)calloc(column, sizeof(int));
		TempMatrix[i] = (int*)calloc(column, sizeof(int));
		for (j = 0; j < column; j++)
		{
			Matrix[i][j] = 0;
			TempMatrix[i][j] = 0;
		}
	}
}

TMatrixMap::~TMatrixMap()
{
	int i;
	if (OldBrush != NULL)
	{
		(HBRUSH)SelectObject(hdc, OldBrush);
		DeleteObject(Brush);
	}
	for ( i = 0; i < RowCount; i++)
	{
		free(Matrix[i]);
		free(TempMatrix[i]);
	}
	free(Matrix);
	free(TempMatrix);
}

//Loading a brush to draw the figure
void TMatrixMap::BrushLoad(int brush_number)
{
	if (BrushNumber != brush_number)
	{
		if (Brush != NULL)
		{
			DeleteObject(Brush);
		}
		switch (brush_number)
		{
		case 0:
			//"Null" brush - emptiness of the playing field
			Brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
			break;
		case 1:
			Brush = CreateSolidBrush(CLR_YELLOW);
			break;
		case 2:
			Brush = CreateSolidBrush(CLR_RED);
			break;
		case 3:
			Brush = CreateSolidBrush(CLR_BLUE);
			break;
		case 4:
			Brush = CreateSolidBrush(CLR_GREEN);
			break;
		case 5:
			Brush = CreateSolidBrush(CLR_PURPLE);
			break;
		case 6:
			Brush = CreateSolidBrush(CLR_LIGHTGREEN);
			break;
		case 7:
			Brush = CreateSolidBrush(CLR_CYAN);
			break;
		}
		SelectObject(hdc, Brush);
	}	
}

//Drawing the main matrix
void TMatrixMap::DrawMatrix()
{
	int i, j;

	BrushLoad(0); //"Null" brush - emptiness of the playing field
	for (i = 0; i < RowCount; i++)
		for (j = 0; j < ColumnCount; j++)
			Rectangle(hdc, 
				StartX + CellWidth * j, 
				StartY + CellHeight * i, 
				StartX + CellWidth * (j + 1), 
				StartY + CellHeight * (i + 1));
}

//Update the main matrix from the temporary one. Each of the cells is compared and,
//if the cells are not equal, then the value is copied from the temporary
//to the main matrix and this cell is redrawn
void TMatrixMap::UpdateMatrix()
{
	int i, j;

	for (i = 0; i < RowCount; i++)
	{
		for (j = 0; j < ColumnCount; j++)
		{
			//If the cells are different
			if (Matrix[i][j] != TempMatrix[i][j])
			{				
				Matrix[i][j] = TempMatrix[i][j];
				//Selecting a brush for drawing based on a cell value
				switch (Matrix[i][j])
				{
				case 0:
					BrushLoad(0);
					break;
				case 1:
					BrushLoad(1);
					break;
				case 2:
					BrushLoad(2);
					break;
				case 3:
					BrushLoad(3);
					break;
				case 4:
					BrushLoad(4);
					break;
				case 5:
					BrushLoad(5);
					break;
				case 6:
					BrushLoad(6);
					break;
				case 7:
					BrushLoad(7);
					break;
				default:
					BrushLoad(1);
					break;
				}
				//Drawing the cell
				Rectangle(hdc,
					StartX + CellWidth * j,
					StartY + CellHeight * i,
					StartX + CellWidth * (j + 1),
					StartY + CellHeight * (i + 1));
			}
		}
	}
}

//Attempt to insert coordinates into the matrix.
//If the matrix is ​​already filled at the location of the insertation,
//then returns FALSE and the insertion does not occur
bool TMatrixMap::Insert(_Coordinates *coordinates)
{
	int i;

	//Check if coordinates are outside the matrix.
	//There is a possibility to implement ignoring negative coordinates,
	//but it is not necessary for now
	for (i = 0; i < coordinates->Count; i++)
	{
		if (coordinates->Row[i] >= RowCount || 
			coordinates->Row[i] < 0 ||
			coordinates->Column[i] < 0 ||
			coordinates->Column[i] >= ColumnCount)
		{
			return FALSE;
		}
		//Check if the matrix is ​​filled at the given coordinates
		if (TempMatrix[coordinates->Row[i]][coordinates->Column[i]] != 0)
		{
			return FALSE;
		}
	}
	//Filling by coordinates
	for (i = 0; i < coordinates->Count; i++)
	{
		TempMatrix[coordinates->Row[i]][coordinates->Column[i]] = coordinates->Type;
	}
	return TRUE;
}

//Extracting (nulling) a matrix by given coordinates
void TMatrixMap::Extract(_Coordinates *coordinates)
{
	int i;

	for (i = 0; i < coordinates->Count; i++)
	{
		TempMatrix[coordinates->Row[i]][coordinates->Column[i]] = 0;
	}
}

//Extracting (nulling) of one row
void TMatrixMap::ExtractRow(int extract_row)
{
	int i;

	for (i = 0; i < ColumnCount; i++)
	{
		TempMatrix[extract_row][i] = 0;
	}
}

//Moving a group of coordinates.
//For the first, the cells by previous coordinates are exctracting,
//Then an attempt to fill cells by new coordinates
//If inserting new coordinates is unsuccessful - returning FALSE
bool TMatrixMap::Move(_Coordinates *prev_position, _Coordinates *next_position)
{
	Extract(prev_position); //Extracting cells from previous coordinates
	if (!Insert(next_position)) //Trying to insert new coordinates
	{
		Insert(prev_position); //Inserting failed, need to fill cells by previous coords back
		return FALSE;
	}
	return TRUE;
}

//Moving one row
bool TMatrixMap::MoveRow(int movable_row, int next_position)
{
	int i;
	for (i = 0; i < ColumnCount; i++)
	{		
		TempMatrix[next_position][i] = TempMatrix[movable_row][i];
	}
	return TRUE;
}

//Extract (nulling) the entire matrix
void TMatrixMap::ExtractMatrix()
{
	int i, j;

	for (i = 0; i < RowCount; i++)
	{
		for (j = 0; j < ColumnCount; j++)
		{
			TempMatrix[i][j] = 0;
		}
	}
	UpdateMatrix();
}

//Count and return the number of filled cells in a row
int TMatrixMap::RowFilled(int row_number)
{
	int i, filled = 0;

	for (i = 0; i < ColumnCount; i++)
	{
		if (TempMatrix[row_number][i] != 0)
		{
			filled++;
		}
	}
	return filled;
}

//============================== Setters and getters ==============================

RECT TMatrixMap::GetRect()
{
	RECT rect;
	rect.left = StartX;
	rect.top = StartY;
	rect.right = StartX + CellWidth * ColumnCount;
	rect.bottom = StartY + CellHeight * RowCount;
	return rect;
}
void TMatrixMap::SetDC(HDC work_hdc)
{
	hdc = work_hdc;
}

int TMatrixMap::GetColumnCount()
{
	return ColumnCount;
}

int TMatrixMap::GetRowCount()
{
	return RowCount;
}