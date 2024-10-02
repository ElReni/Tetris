#pragma once

#include "TGame.h"
#include "Resource.h"

TGame::TGame(HINSTANCE work_hinstance, HWND work_hwnd, HDC work_hdc)
{
	//Window parameters saving
	hInst = work_hinstance;
	hwnd = work_hwnd;
	hdc = work_hdc;
	//Draw the workspace
	DrawWorkspace();	
}

TGame::~TGame()
{
	KillTimer(hwnd, GameTimerId);
	if (CurrentFigure != NULL)
	{
		delete CurrentFigure;
	}
	if (NextFigure != NULL)
	{
		delete NextFigure;
	}
	DeleteObject(BackgrBrush);
}

//Random figure generation
IMatrixFigure* TGame::GenerateFigure()
{
	//Number generaion
	int type = Generator.GetRandom(1, 7); 
	IMatrixFigure* NewFigure;
	//Figure creation
	switch (type)
	{
	case 1:
		NewFigure = new TMatrixI();
		NewFigure->SetType(type);
		return NewFigure;
		break;
	case 2:
		NewFigure = new TMatrixS();
		NewFigure->SetType(type);
		return NewFigure;
		break;
	case 3:
		NewFigure = new TMatrixZ();
		NewFigure->SetType(type);
		return NewFigure;
		break;
	case 4:
		NewFigure = new TMatrixL();
		NewFigure->SetType(type);
		return NewFigure;
		break;
	case 5:
		NewFigure = new TMatrixJ();
		NewFigure->SetType(type);
		return NewFigure;
		break;
	case 6:
		NewFigure = new TMatrixT();
		NewFigure->SetType(type);
		return NewFigure;
		break;
	case 7:
		NewFigure = new TMatrixO();
		NewFigure->SetType(type);
		return NewFigure;
		break;
	default:
		return NULL;
		break;
	}
	return NewFigure;
}

//Create new figure and set it for preview
void TGame::ChangeFigure()
{
	delete CurrentFigure;
	Preview.ExtractMatrix();
	CurrentFigure = NextFigure; //Set next figure to current
	NextFigure = NULL;	
	NewNextFigure(); //Create new next figure
	Preview.Insert(NextFigure->GetCoordinates());
	Preview.UpdateMatrix();
	CurrentFigure->MoveCentralTo(0, 5); //Move new figure to center
	CurrentFigure->ApplyTempCoordinates();
}

//Destroys completely filled rows on map.
//Erases all filled rows, and 
//moves higher rows down.
//Returns the number of rows erased.
int TGame::EraseRows()
{
	int i, n = 0, filled = 0;

	//Rows checking starts from the bottom of the map
	for (i = Map.GetRowCount() - 1; i > -1; i--)
	{
		//Number of filled cells
		if ((filled = Map.RowFilled(i)) > 0) 
		{
			if (filled == Map.GetColumnCount()) //If entire row is filled
			{
				Map.ExtractRow(i);
				n++; //Number of deleted rows
			}
			else
			{
				if (n > 0)
				{
					Map.MoveRow(i, i + n);
					Map.ExtractRow(i);
				}
				
			}				
		}
		else //If no one cell in the row is filled then ending of check
		{			
			return n;
		}
	}	
	return n;
}

//Updating the score and redrawing it
void TGame::UpdateScore(int rows_erased)
{
	int scoreForErase;
	//If erased more than one row then adding bonus scores
	switch (rows_erased)
	{
	case 1:
		scoreForErase = Map.GetColumnCount();
		break;
	case 2:
		scoreForErase = Map.GetColumnCount() + 25;
		break;
	case 3:
		scoreForErase = Map.GetColumnCount() + 80;
		break;
	case 4:
		scoreForErase = Map.GetColumnCount() + 200;
		break;
	default:
		scoreForErase = (Map.GetColumnCount() * rows_erased) + (100 * rows_erased);
		break;
	}
	Score += scoreForErase * ScoreMultiplier;
	DrawScore(); //Redraw score
}

//Draw the score
void TGame::DrawScore()
{
	char temp[10];
	_itoa_s(Score, temp, 10, 10);
	FillRect(hdc, &ScoreRect, BackgrBrush);
	DrawTextA(hdc, temp, -1, &ScoreRect, DT_CENTER);	
	InvalidateRect(hwnd, &ScoreRect, FALSE);
}

//Draw the menu.
//Fill certain structures for storing as well
void TGame::DrawMenu()
{
	RECT temp;
	temp.left = 375;
	temp.top = 25;	
	temp.right = 525;
	temp.bottom = 50;
	DrawText(hdc, L"Следующая фигура", 16, &temp, DT_CENTER);

	temp.left = 375;
	temp.top = 150;
	temp.right = 525;
	temp.bottom = 175;
	DrawText(hdc, L"Счет", 4, &temp, DT_CENTER);

	ScoreRect.left = 375;
	ScoreRect.top = 175;
	ScoreRect.right = 525;
	ScoreRect.bottom = 200;	
	DrawScore();
}

//Draw the workspace.
//Also here is filling of some 
//structures for images drawing
void TGame::DrawWorkspace()
{
	RECT rect;
	int tempInt;

	SetBkMode(hdc, TRANSPARENT);
	Map.SetDC(hdc);
	Preview.SetDC(hdc);
	GetClientRect(hwnd, &rect);
	BackgrBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	FillRect(hdc, &rect, BackgrBrush);
	DeleteObject(BackgrBrush);
	Map.DrawMatrix();
	Preview.DrawMatrix();
	DrawMenu();	
	InvalidateRect(hwnd, &rect, FALSE);

	//Position of image for pause
	PauseIconRect.left = 425;
	PauseIconRect.top = 250;
	PauseIconRect.right = 475;
	PauseIconRect.bottom = 300;

	//Position of image for game over.
	//Occupies 2/5 of the vertical component of the playing field and the entire horizontal one
	GameOverIconRect = Map.GetRect();
	tempInt = (GameOverIconRect.bottom * 2) / 5;
	GameOverIconRect.bottom = GameOverIconRect.top + (GameOverIconRect.bottom + tempInt) / 2;
	GameOverIconRect.top = GameOverIconRect.bottom - tempInt;
}

//Next figure creating
void TGame::NewNextFigure()
{
	if (NextFigure != NULL)
	{
		delete NextFigure;
	}
	NextFigure = GenerateFigure();
}

//Current figure creating
void TGame::NewCurrentFigure()
{
	if (CurrentFigure != NULL)
	{
		delete CurrentFigure;
	}
	CurrentFigure = GenerateFigure();
	//Moving to center
	CurrentFigure->MoveCentralTo(0, 5);
	CurrentFigure->ApplyTempCoordinates();
}

//New game starting
void TGame::NewGame()
{
	if (Status == PAUSE)
	{
		Pause();
	}
	Status = INITIALIZATION;
	//Timer recreating
	KillTimer(hwnd, GameTimerId);
	TimerElapse = 500;
	//New figures creating
	NewCurrentFigure();
	NewNextFigure();
	//Maps clearing
	Map.ExtractMatrix();
	Map.DrawMatrix();
	Preview.ExtractMatrix();
	Preview.DrawMatrix();
	//Figures insering and maps updating
	Preview.Insert(NextFigure->GetCoordinates());
	Map.Insert(CurrentFigure->GetCoordinates());
	Preview.UpdateMatrix();
	Map.UpdateMatrix();
	//Nulling the score
	Score = 0;
	DrawScore();	
	//Redrawing
	InvalidateRect(hwnd, &Map.GetRect(), TRUE);
	InvalidateRect(hwnd, &Preview.GetRect(), TRUE);
	Status = PLAY;
	SetTimer(hwnd, GameTimerId, TimerElapse, NULL);		
}

//Game over
void TGame::GameOver()
{
	Status = GAME_OVER;
	//Game over image drawing
	DrawBitmap(&GameOverIconRect, IDB_BITMAP2);
	PlaySound(MAKEINTRESOURCE(IDR_WAVE2), hInst, SND_ASYNC | SND_RESOURCE);
}

//Timer event processing
void TGame::OnTimer()
{
	switch (Status)
	{
	case PLAY: //Game status
		//If current figure can't be moved down
		if (!Map.Move(CurrentFigure->GetCoordinates(), CurrentFigure->MoveVertical(1)))
		{
			//Checking for filled rows
			int n = EraseRows();
			if (n > 0) //If filled rows are presented
			{
				//Score updating
				UpdateScore(n);
				//Count erased rows to speed up the game
				RowsToSpeedUp -= n;
				if (RowsToSpeedUp <= 0)
				{
					//With game speed up the score multiplier increases as well
					ScoreMultiplier += 0.2;
					if (TimerElapse - 100 >= 100)
					{
						//Timer speed up
						KillTimer(hwnd, GameTimerId);						
						TimerElapse -= 100;						
						SetTimer(hwnd, GameTimerId, TimerElapse, NULL);
					}
					RowsToSpeedUp += 10;
				}
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), hInst, SND_ASYNC | SND_RESOURCE);
			}	
			//Figure changing
			ChangeFigure();
			//If can not insert new figure - game over
			if (!Map.Insert(CurrentFigure->GetCoordinates()))
			{
				GameOver();
			}
			Map.UpdateMatrix();			
		}
		else //If current figure can be moved down
		{			
			CurrentFigure->ApplyTempCoordinates();
			Map.UpdateMatrix();			
		}
		InvalidateRect(hwnd, &Map.GetRect(), TRUE);
		InvalidateRect(hwnd, &Preview.GetRect(), TRUE);
		break;
	}
}

//Processing key press event 
void TGame::OnKeyPress(int key_id)
{
	switch (Status)
	{
		case PLAY:
		{
			HINSTANCE hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
			switch (key_id)
			{
				//Space - figure rotating
			case VK_SPACE:
				if (Map.Move(CurrentFigure->GetCoordinates(), CurrentFigure->Rotate()))
				{
					CurrentFigure->ApplyTempCoordinates();	
					Map.UpdateMatrix();
					InvalidateRect(hwnd, &Map.GetRect(), TRUE);
					PlaySound(MAKEINTRESOURCE(IDR_FIGURE_MOVE), hInst, SND_ASYNC | SND_RESOURCE);
				}				
				break;
				//Moving to left
			case VK_LEFT:
				if (Map.Move(CurrentFigure->GetCoordinates(), CurrentFigure->MoveHorizontal(-1)))
				{
					CurrentFigure->ApplyTempCoordinates();
					Map.UpdateMatrix();
					InvalidateRect(hwnd, &Map.GetRect(), TRUE);
				}
				break;
				//Moving to right
			case VK_RIGHT:
				if (Map.Move(CurrentFigure->GetCoordinates(), CurrentFigure->MoveHorizontal(1)))
				{
					CurrentFigure->ApplyTempCoordinates();
					Map.UpdateMatrix();
					InvalidateRect(hwnd, &Map.GetRect(), TRUE);
				}
				break;
				//Moving to down
			case VK_DOWN:
				if (Map.Move(CurrentFigure->GetCoordinates(), CurrentFigure->MoveVertical(1)))
				{
					CurrentFigure->ApplyTempCoordinates();	
					Map.UpdateMatrix();
					InvalidateRect(hwnd, &Map.GetRect(), TRUE);					
				}				
				break;
			}
		}
	}	
}

//Drawing image from the resources
void TGame::DrawBitmap(RECT *coordinates, int resource_id)
{
	HBITMAP tempBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(resource_id)), prevBitmap;
	HDC tempDC;
	BITMAP bmp;
	POINT  pictureSize;

	//Creating memory instance
	tempDC = CreateCompatibleDC(hdc);
	prevBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);
	if (prevBitmap)
	{
		//The context memory is set to the same 
		//display mode that is used in 
		//context display
		SetMapMode(tempDC, GetMapMode(hdc));
		//Determine the image dimensions
		GetObject(tempBitmap, sizeof(BITMAP), (LPSTR)& bmp);
		pictureSize.x = bmp.bmWidth;
		pictureSize.y = bmp.bmHeight;
		//Draw the bitmap image
		StretchBlt(hdc, coordinates->left, coordinates->top, coordinates->right - coordinates->left, coordinates->bottom - coordinates->top,
			tempDC, 0, 0, pictureSize.x, pictureSize.y, SRCCOPY);
		//Restore memory context
		SelectObject(tempDC, prevBitmap);
		//Repaint the area
		InvalidateRect(hwnd, coordinates, FALSE);
	}
	//Delete the memory context
	DeleteDC(tempDC);
	DeleteObject(tempBitmap);
}

//Game pause
void TGame::Pause()
{
	switch (Status)
	{
	case PLAY:
	{
		Status = PAUSE;
		DrawBitmap(&PauseIconRect, IDB_BITMAP1);		
		KillTimer(hwnd, GameTimerId); //Timer deleting
		break;
	}
	case PAUSE:
		FillRect(hdc, &PauseIconRect, BackgrBrush);
		InvalidateRect(hwnd, &PauseIconRect, FALSE);
		Status = PLAY;
		SetTimer(hwnd, GameTimerId, TimerElapse, NULL);
		break;
	}
}