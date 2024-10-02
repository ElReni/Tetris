# About
Classic Tetris game by using pure WinAPI. Project is made by Visual Studio 2019 and well commented. Memory leaks tested. Some points of implementation:
* Standard actions with figures (shape moving and rotating)
* Double buffering is used for drawing
* Adding additional score when more than one row is erased
* Using sounds and a couple of images for decoration
* Pausing and resuming the game

### Class diagram
![Alt text](ClassDiagram.png?raw=true "ClassDiagram")

TGame is a main class that controls the game. TMatrixMap is using to control playing field and displaying it. There are two matrix maps for direct game and for previewing a next figure. IMatrixFigure is a interface of shapes which is using in TGame so adding new figures is not a problem.

### Controls

* ←, →, ↓ for shape moving
* Space for shape rotating

### Game preview

![Alt text](preview1.png?raw=true "preview1")

![Alt text](preview2.png?raw=true "preview2")