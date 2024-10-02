#include "TMatrixFigure.h"

TMatrixFigure::~TMatrixFigure()
{
		free(Coordinates.Row);
		free(Coordinates.Column);
		free(CentralCoordinate.Row);
		free(CentralCoordinate.Column);
		free(TempCoordinates.Row);
		free(TempCoordinates.Column);
}

//Move the figure.
//Moving occurs relative to the first coordinate in the array (0 index) 
//to the specified row and column 
//and returning the result
struct _Coordinates * TMatrixFigure::MoveTo(int row, int column)
{
	int i, offset_row, offset_column;

	//Calculating offsets
	offset_row = row - Coordinates.Row[0];
	offset_column = column - Coordinates.Column[0];
	for (i = 0; i < Coordinates.Count; i++)
	{
		TempCoordinates.Row[i] = Coordinates.Row[i] + offset_row;
		TempCoordinates.Column[i] = Coordinates.Column[i] + offset_column;
	}
	return &TempCoordinates;
}

//Move the figure.
//Moving occurs relative to the central coordinate by the specified
//row and column and returning the result
struct _Coordinates * TMatrixFigure::MoveCentralTo(int row, int column)
{
	int i, offset_row, offset_column;

	GetCentralCoordinate();
	//Calculating offsets
	offset_row = row - CentralCoordinate.Row[0];
	offset_column = column - CentralCoordinate.Column[0];
	for (i = 0; i < Coordinates.Count; i++)
	{
		TempCoordinates.Row[i] = Coordinates.Row[i] + offset_row;
		TempCoordinates.Column[i] = Coordinates.Column[i] + offset_column;
	}
	return &TempCoordinates;
}

//Move the figure.
//Move the entire figure vertically
//by a given amount and return the result
struct _Coordinates * TMatrixFigure::MoveVertical(int move_value)
{
	int i;

	for (i = 0; i < Coordinates.Count; i++)
	{
		TempCoordinates.Row[i] = Coordinates.Row[i] + move_value;
		TempCoordinates.Column[i] = Coordinates.Column[i];
	}
	return &TempCoordinates;
}

//Move the figure.
//Move the entire figure horizontally
//by a given amount and return the result
struct _Coordinates * TMatrixFigure::MoveHorizontal(int move_value)
{
	int i;

	for (i = 0; i < Coordinates.Count; i++)
	{
		TempCoordinates.Row[i] = Coordinates.Row[i];
		TempCoordinates.Column[i] = Coordinates.Column[i] + move_value;
	}
	return &TempCoordinates;
}

//Rotate the figure.
//Rotate is performed around the central coordinate,
//which always remains in the same place
struct _Coordinates * TMatrixFigure::Rotate()
{
	int i;

	GetCentralCoordinate();
	for (i = 0; i < Coordinates.Count; i++)
	{
		//y2 = x1 + py - px
		//x2 = px + py - y1
		//py, px - central point coordinates
		//x1, y1 - previous coordinates
		//x2, y2 - next coordinates
		TempCoordinates.Row[i] = Coordinates.Column[i] + CentralCoordinate.Row[0] - CentralCoordinate.Column[0];
		TempCoordinates.Column[i] = CentralCoordinate.Column[0] + CentralCoordinate.Row[0] - Coordinates.Row[i];
	}
	return &TempCoordinates;
}

//Moving coordinates from temporary storage to current
void TMatrixFigure::ApplyTempCoordinates()
{
	int i;

	for (i = 0; i < Coordinates.Count; i++)
	{
		Coordinates.Row[i] = TempCoordinates.Row[i];
		Coordinates.Column[i] = TempCoordinates.Column[i];
	}
}

//Initialization of the coordinate structure for the specified 
//number of points
void TMatrixFigure::CoordinatesInitialize(int coord_count)
{
	Coordinates.Row = (int*)malloc(coord_count* sizeof(int));
	Coordinates.Column = (int*)malloc(coord_count* sizeof(int));
	Coordinates.Count = coord_count;
	TempCoordinates.Row = (int*)malloc(coord_count* sizeof(int));
	TempCoordinates.Column = (int*)malloc(coord_count* sizeof(int));
	TempCoordinates.Count = coord_count;
	CentralCoordinate.Row = (int*)malloc(1* sizeof(int));
	CentralCoordinate.Column = (int*)malloc(1* sizeof(int));
	CentralCoordinate.Count = 1;
}

//==================== Setters and getters ====================

bool TMatrixFigure::SetCoordinates(struct _Coordinates* coordinates)
{
	int i;

	if (Coordinates.Count == coordinates->Count)
	{
		for (i = 0; i < Coordinates.Count; i++)
		{
			Coordinates.Row[i] = coordinates->Row[i];
			Coordinates.Column[i] = coordinates->Column[i];

		}
		return true;
	}
	return false;
}

void TMatrixFigure::SetType(int type)
{
	TempCoordinates.Type = type;
	Coordinates.Type = type;
	CentralCoordinate.Type = type;
}

int TMatrixFigure::GetType()
{
	return Coordinates.Type;
}


struct _Coordinates* TMatrixFigure::GetCentralCoordinate()
{
	CentralCoordinate.Row[0] = Coordinates.Row[Central];
	CentralCoordinate.Column[0] = Coordinates.Column[Central];
	return &CentralCoordinate;
}

struct _Coordinates* TMatrixFigure::GetCoordinates()
{
	return &Coordinates;
}

bool TMatrixFigure::SetCentralCoordinate(int coordinate_number)
{
	if (coordinate_number > Coordinates.Count || coordinate_number < 0)
	{
		return false;
	}
	CentralCoordinate.Row[0] = Coordinates.Row[coordinate_number];
	CentralCoordinate.Column[0] = Coordinates.Column[coordinate_number];
	Central = coordinate_number;
}

//==================== Implementation and constructors of specific figures ====================

TMatrixI::TMatrixI()
{
	int i;

	CoordinatesInitialize(4);
	Central = 1;
	for (i = 0; i < Coordinates.Count; i++)
	{
		Coordinates.Row[i] = 0;
		Coordinates.Column[i] = i;
		TempCoordinates.Row[i] = 0;
		TempCoordinates.Column[i] = i;
	}
}

struct _Coordinates* TMatrixI::Rotate()
{
	struct _Coordinates* temp = TMatrixFigure::Rotate();
	if (Central == 1)
	{
		Central = 2;
	}
	else
	{
		Central = 1;
	}
	return temp;
}

TMatrixS::TMatrixS()
{
	int i;

	CoordinatesInitialize(4);
	Central = 2;
	for (i = 0; i < 2; i++)
	{
		Coordinates.Row[i] = 1;
		Coordinates.Column[i] = i;
		TempCoordinates.Row[i] = 1;
		TempCoordinates.Column[i] = i;
	}
	for (i = 2; i < 4; i++)
	{
		Coordinates.Row[i] = 0;
		Coordinates.Column[i] = i - 1;
		TempCoordinates.Row[i] = 0;
		TempCoordinates.Column[i] = i - 1;
	}
}

TMatrixZ::TMatrixZ()
{
	int i;

	CoordinatesInitialize(4);
	Central = 1;
	for (i = 0; i < 2; i++)
	{
		Coordinates.Row[i] = 0;
		Coordinates.Column[i] = i;
		TempCoordinates.Row[i] = 0;
		TempCoordinates.Column[i] = i;
	}
	for (i = 2; i < 4; i++)
	{
		Coordinates.Row[i] = 1;
		Coordinates.Column[i] = i - 1;
		TempCoordinates.Row[i] = 1;
		TempCoordinates.Column[i] = i - 1;
	}
}

TMatrixL::TMatrixL()
{
	{
		int i;

		CoordinatesInitialize(4);
		Central = 2;
		Coordinates.Row[0] = 1;
		Coordinates.Column[0] = 0;
		TempCoordinates.Row[0] = 1;
		TempCoordinates.Column[0] = 0;
		for (i = 1; i < 4; i++)
		{
			Coordinates.Row[i] = 0;
			Coordinates.Column[i] = i - 1;
			TempCoordinates.Row[i] = 0;
			TempCoordinates.Column[i] = i - 1;
		}
	}
}

TMatrixJ::TMatrixJ()
{
	{
		int i;

		CoordinatesInitialize(4);
		Central = 1;
		for (i = 0; i < 3; i++)
		{
			Coordinates.Row[i] = 0;
			Coordinates.Column[i] = i;
			TempCoordinates.Row[i] = 0;
			TempCoordinates.Column[i] = i;
		}
		Coordinates.Row[i] = 1;
		Coordinates.Column[i] = 2;
		TempCoordinates.Row[i] = 1;
		TempCoordinates.Column[i] = 2;		
	}
}

TMatrixT::TMatrixT()
{
	{
		int i;

		CoordinatesInitialize(4);
		Central = 1;
		for (i = 0; i < 3; i++)
		{
			Coordinates.Row[i] = 0;
			Coordinates.Column[i] = i;
			TempCoordinates.Row[i] = 0;
			TempCoordinates.Column[i] = i;
		}
		Coordinates.Row[i] = 1;
		Coordinates.Column[i] = 1;
		TempCoordinates.Row[i] = 1;
		TempCoordinates.Column[i] = 1;
	}
}

TMatrixO::TMatrixO()
{
	{
		int i;

		CoordinatesInitialize(4);
		Central = 0;
		for (i = 0; i < 2; i++)
		{
			Coordinates.Row[i] = 0;
			Coordinates.Column[i] = i;
			TempCoordinates.Row[i] = 0;
			TempCoordinates.Column[i] = i;
		}
		for (i = 2; i < 4; i++)
		{
			Coordinates.Row[i] = 1;
			Coordinates.Column[i] = i - 2;
			TempCoordinates.Row[i] = 1;
			TempCoordinates.Column[i] = i - 2;
		}
	}
}

struct _Coordinates* TMatrixO::Rotate()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		TempCoordinates.Row[i] = Coordinates.Row[i];
		TempCoordinates.Column[i] = Coordinates.Column[i];
	}
	return &TempCoordinates;
}