#pragma once

#include <Windows.h>
#include "TMatrixMap.h"
#include "TMatrixFigure.h"
#include <stdlib.h>
#include "TRandomGenerator.h"

#define BACKGROUND_COLOR RGB(255, 255, 255)

/*
Game class module.
Implements the entire game model plus
does some rendering.
*/

//Possible game states
enum GameStatus
{
	INITIALIZATION,
	MENU,
	PLAY,
	PAUSE,
	GAME_OVER
};

//Game class
class TGame
{
private:
	//Window identifiers
	HINSTANCE hInst;
	HWND hwnd = NULL;
	//Context for rendering
	HDC hdc;	
	//Two matrix maps - one for the playing field, the other for previewing the next figure
	TMatrixMap Map = TMatrixMap(0, 0, 350, 450, 16, 12);
	TMatrixMap Preview = TMatrixMap(400, 50, 100, 50, 2, 4);
	//Two figures - current for moving and next for preview
	IMatrixFigure* CurrentFigure = NULL;
	IMatrixFigure* NextFigure = NULL;
	//Random Number Generator
	TRandomGenerator Generator;
	//Timer ID
	int GameTimerId = 1;
	//Game parameters
	int TimerElapse = 1000, Score = 0, RowsToSpeedUp = 10;
	float ScoreMultiplier = 1;
	GameStatus Status = INITIALIZATION;
	RECT ScoreRect, PauseIconRect, GameOverIconRect;
	HBRUSH BackgrBrush;

	void GameOver(); //Game over
	IMatrixFigure* GenerateFigure(); //Random shape generation
	void ChangeFigure(); //Change the current figure to the next one
	void NewNextFigure(); //Create a new next figure
	void NewCurrentFigure(); //Create a new current figure
	int EraseRows(); //Erase fully filled rows
	void UpdateScore(int score_increase); //Update the score
	//Drawing functions
	void DrawScore(); //Score drawing
	void DrawMenu(); //Entire menu drawing
	void DrawBitmap(RECT* coordinates, int resource_id); //Draw image from resources
	void DrawWorkspace(); //Draw entire workspace
public:
	TGame(HINSTANCE work_hinstance, HWND work_hwnd, HDC work_hdc);
	~TGame();
	void OnTimer(); //Timer processing
	void OnKeyPress(int key_id); //Key pressing processing
	void NewGame(); //New game
	void Pause(); //Pause
};