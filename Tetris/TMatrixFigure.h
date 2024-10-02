#pragma once

#include <stdlib.h>
#include "Coordinates.h"

/*
Module of figure classes.
Works with the _Coordinates coordinate structure.
All methods that change the coordinates of a figure
are places the results of calculations in a buffer
(temporary array of coordinates)
and returns them, after which the coordinates
can be checked, for example, for
collision.
To apply the obtained result
of working with coordinates, you need to call the
function ApplyTempCoordinates()
or work through a setter.
*/

//Интерфейс фигур
class IMatrixFigure
{
public:
	virtual struct _Coordinates * MoveTo(int row, int column) = 0; //Move figure to
	virtual struct _Coordinates * MoveCentralTo(int row, int column) = 0; //Move figure by central cell to
	virtual struct _Coordinates * MoveVertical(int move_value) = 0; //Move vertically by value
	virtual struct _Coordinates * MoveHorizontal(int move_value) = 0; //Move horizontally by value
	virtual struct _Coordinates * Rotate() = 0; //Figure rotating
	virtual void ApplyTempCoordinates() = 0; //Apply new coordinates
	virtual ~IMatrixFigure() = 0 {};

	//Setters and getters
	virtual bool SetCoordinates(struct _Coordinates *coordinates) = 0;
	virtual void SetType(int type) = 0;
	virtual int GetType() = 0;	
	virtual struct _Coordinates * GetCentralCoordinate() = 0;
	virtual struct _Coordinates * GetCoordinates() = 0;	
};

/*
Base class of shapes.
Implementation of all methods in general.
Implements the functionality of shapes, but
cannot generate them, for this
you need to use its descendants.
*/
class TMatrixFigure: public IMatrixFigure
{
protected:
	struct _Coordinates Coordinates, CentralCoordinate, TempCoordinates;
	int Central;

	void CoordinatesInitialize(int coord_count); //Initialize the number of points
public:
	~TMatrixFigure();
	void ApplyTempCoordinates();
	struct _Coordinates * MoveTo(int row, int column);
	struct _Coordinates * MoveCentralTo(int row, int column);
	struct _Coordinates * MoveVertical(int move_value);
	struct _Coordinates * MoveHorizontal(int move_value);
	struct _Coordinates * Rotate();	
	
	void SetType(int type);
	int GetType();
	bool SetCentralCoordinate(int coordinate_number);
	struct _Coordinates * GetCentralCoordinate();
	bool SetCoordinates(struct _Coordinates* coordinates);
	struct _Coordinates * GetCoordinates();
};

//==================== Classes of concrete figures ====================
//
class TMatrixI : public TMatrixFigure
{
public:
	TMatrixI();
	virtual struct _Coordinates* Rotate();
};

class TMatrixS : public TMatrixFigure
{
public:
	TMatrixS();
};

class TMatrixZ : public TMatrixFigure
{
public:
	TMatrixZ();
};

class TMatrixL : public TMatrixFigure
{
public:
	TMatrixL();
};

class TMatrixJ : public TMatrixFigure
{
public:
	TMatrixJ();
};

class TMatrixT : public TMatrixFigure
{
public:
	TMatrixT();
};

class TMatrixO : public TMatrixFigure
{
public:
	TMatrixO();
	virtual struct _Coordinates* Rotate();
};