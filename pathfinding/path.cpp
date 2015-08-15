#include <iostream>
#include <iomanip>
#include <cmath>

#define N 0
#define NE 1
#define E 2
#define SE 3
#define S 4
#define SW 5
#define W 6
#define NW 7
#define NDIR 8
#define OPEN 1
#define BLOCKED 0
#define WALL -2
#define ME 2
#define TARGET 3

void printDirection(int);					//direction
void printGrid(int[][100], int, int);
void clearGrid(int[][100], int, int);
void blockBorders(int[][100], int, int);	//grid[][], maxX, maxY
void setWall(int[][100], int, int);
float getDistance(int, int, int, int);		//x1, y1, x2, y2
int calPath(int[][100], int, int, int&, int&, int, int);

static int PATH[1000];	//path to target
static int nSteps;		//step along path

int main(void)
{
	int maxX, maxY = 8;
	int grid[100][100];
	int atX = 0, atY = 0, targetX = 0, targetY = 0;
	float distanceTarget;
	int maxSteps = 24;

	std::cout << "Enter Grid Size [x] [y] " << std::endl;
	std::cin >> maxX >> maxY;

	maxX += 2;
	maxY += 2;

	while (atX <= 0 || atX > (maxX - 2) || atY <= 0 || atY > (maxY - 2))
	{
		std::cout << "Enter Your Coordinates [x] [y] " << std::endl;
		std::cin >> atX >> atY;
	}

	while (targetX <= 0 || targetX > (maxX - 2) || targetY <= 0 || targetY > (maxY - 2))
	{
		std::cout << "Enter Target Coordinates [x] [y] " << std::endl;
		std::cin >> targetX >> targetY;
	}

	distanceTarget = getDistance(atX, atY, targetX, targetY);
	std::cout << "Distance to target " << distanceTarget << std::endl << std::endl;
	clearGrid(grid, maxX, maxY);
	blockBorders(grid, maxX, maxY);

	grid[atX][atY] = ME;
	grid[targetX][targetY] = TARGET;
	printGrid(grid, maxX, maxY);

	setWall(grid, maxX, maxY);
	printGrid(grid, maxX, maxY);

	for (nSteps = 0; nSteps < maxSteps; nSteps++)
	{
		calPath(grid, maxX, maxY, atX, atY, targetX, targetY);
		if (atX == targetX && atY == targetY)
		{
			std::cout << "We HIT the Target by heading " << std::endl;
			break;
		}
	}

	for (int nS = 0; nS <= nSteps; nS++)
	{
		std::cout << std::setw(4) << nS;
		printDirection(PATH[nS]);
	}

	return 42;
}

void printDirection(int x)
{
	switch (x)
	{
		case N:
			std::cout << " N: ";
			break;
		case NE:
			std::cout << " NE: ";
			break;
		case E:
			std::cout << " E: ";
			break;
		case SE:
			std::cout << " SE: ";
			break;
		case S:
			std::cout << " S: ";
			break;
		case SW:
			std::cout << " SW: ";
			break;
		case W:
			std::cout << " W: ";
			break;
		case NW:
			std::cout << " NW: ";
			break;
		default:	
			std::cout << "Direction Error";
	}
	
	return;
}

void printGrid(int x[][100], int nC, int nR)
{
	std::cout << std::setw(4) << " r/c";

	for (int c = 0; c < nC; c++)
		std::cout << std::setw(4) << c;

	for (int r = 0; r < nR; r++)
	{
		std::cout << std::setw(4) << r;

		for (int c = 0; c < nC; c++)
			if (x[r][c] == BLOCKED)
				std::cout << std::setw(4) << " X";
			else if (x[r][c] == WALL)
				std::cout << std::setw(4) << " W";
			else if (x[r][c] == ME)
				std::cout << std::setw(4) << " ME";
			else if (x[r][c] == TARGET)
				std::cout << std::setw(4) << " TAR";
			else
				std::cout << std::setw(4) << x[r][c];

		std::cout << std::endl << std::endl << std::endl;
	}

	return;
}

void clearGrid(int x[][100], int nC, int nR)
{
	for (int r = 0; r < nR; r++)
		for (int c = 0; c < nC; c++)
			x[r][c] = OPEN;
	return;
}

float getDistance(int x1, int y1, int x2, int y2)
{
	return sqrt(static_cast<double>((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
}

void blockBorders(int x[][100], int nC, int nR)
{
	for (int c = 0; c < nC; c++)
	{
		x[0][c] = BLOCKED;
		x[nR - 1][c] = BLOCKED;
	}

	for (int r = 1; r < nR; r++)
	{
		x[r][0] = BLOCKED;
		x[r][nC - 1] = BLOCKED;
	}

	return;
}

void setWall(int x[][100], int nC, int nR)
{
	int r = 1, c = 1;

	while (r + c >= 0)
	{
		std::cout << "Enter Wall Coordinates [x] [y]  [-1 to quit] ";
		std::cin >> c >> r;
		x[r][c] = WALL;
		printGrid(x, nC, nR);
	}

	return;
}

int calPath(int x[][100], int nC, int nR, int &meX, int &meY, int tarX, int tarY)
{
	static int direction = -99;
	int testX, testY, minX, minY;
	float s = 100000.;
	float sTemp, sMin = 10000.;

	std::cout << std::fixed << std::showpoint << std::setprecision(2);
	std::cout << "Initail Distance to Target              " << getDistance(meX, meY, tarX, tarY) << std::setw(4) << x[meX][meY] << std::setw(4) << meX << std::setw(4) << meY << std::setw(4) << tarX << std::setw(4) << tarY << std::endl;

	testX = meX;
	testY = meY;

	for (int p = 0; p < NDIR; p++)
	{
		switch (p)
		{
			case N:
				testX = meX;
				testY = meY + 1;
				break;
			case NE:
				testX = meX + 1;
				testY = meY + 1;
				break;
			case E:
				testX = meX + 1;
				testY = meY;
				break;
			case SE:
				testX = meX + 1;
				testY = meY - 1;
				break;
			case S:
				testX = meX;
				testY = meY - 1;
				break;
			case SW:
				testX = meX - 1;
				testY = meY - 1;
				break;
			case W:
				testX = meX - 1;
				testY = meY;
				break;
			case NW:
				testX = meX - 1;
				testY = meY + 1;
				break;
		}

		sTemp = getDistance(testX, testY, tarX, tarY);

		if (sTemp < sMin && x[testX][testY]>0)
		{
			sMin = sTemp;
			direction = p;
			PATH[nSteps] = p;
			minX = testX;
			minY = testY;
		}

		printDirection(p);
		
		std::cout << "Initail Distance to Target              " << std::setw(8) << sMin << std::setw(8) << sTemp << std::setw(4) << x[testX][testY] << std::setw(4) << testX << std::setw(4) << testY << std::setw(4) << tarX << std::setw(4) << tarY << std::endl;
	}

	x[meX][meY] = -10 - nSteps;
	meX = minX;
	meY = minY;	
	x[meX][meY] = ME;

	std::cout << "\n\nHeaded "; 
	printDirection(direction); 
	std::cout << std::endl << std::endl;

	printGrid(x, nC, nR);

	return direction;
}