#pragma once

#include <Windows.h>
#include <stdlib.h>
#include "Coordinates.h"


#define CURRENT_FIGURE_ID 0
#define NEXT_FIGURE_ID 1

/*
Matrix map class module.
The map works with two matrices:
the main and the additional.
The main matrix stores the current state,
while the additional matrix
calculates the next state.
*/

class TMatrixMap
{
private:
	HDC hdc;	
	HBRUSH Brush = NULL, OldBrush = NULL;	
	int BrushNumber = -1;
	int **Matrix, **TempMatrix;
	int StartX, StartY, RowCount, ColumnCount; //Matrix parameters
	int CellWidth, CellHeight; //Dimensions of one cell

	void BrushLoad(int brush_number);

public:	
	
	void DrawMatrix();
	void UpdateMatrix();	
	bool Move(_Coordinates *prev_position, _Coordinates *next_position);
	bool MoveRow(int movable_row, int next_position);
	bool Insert(_Coordinates *coordinates);
	int RowFilled(int row_number);
	void Extract(_Coordinates *coordinates);
	void ExtractRow(int extract_row);
	void ExtractMatrix();
	~TMatrixMap();
	TMatrixMap(int x, int y, int width, int height, int row, int column);

	//Setters and getters
	void SetDC(HDC work_hdc);
	int GetRowCount();
	int GetColumnCount();
	RECT GetRect();
};




