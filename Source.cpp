#include <SFML/Graphics.hpp>
#include<Windows.h>
#include<iostream>
#include <thread>
#include<fstream>
#include <iostream>
#include <future>

using namespace sf;
using namespace std;

Font font;
//int retclick = 0;
class  Button {
public:
	int xLeftUp, yLeftup, xrightdown, yrightdown;
    RenderWindow *renderwindow;
	string Text11;
	Color col;
	void (*Function)();
	Button(RenderWindow *rendW,string Text1 ="", Color setcol = Color::White,int x1 = 0,int y1 = 0,int x2 = 0,int y2 = 0,void (*f)() = NULL) {
		xLeftUp = x1;
		xrightdown = x2;
		yLeftup = y1;
		yrightdown = y2;
		Function = f;
		renderwindow = rendW;
		col = setcol;
		Text11 = Text1;
	}
	void Render(int posxmouse,int posymouse,int *click) {
		if (((posxmouse<xLeftUp)||(posymouse<yLeftup))||((posxmouse>(xLeftUp+xrightdown))||(posymouse>(yLeftup+yrightdown)))) {
			RectangleShape quad = RectangleShape(Vector2f(xLeftUp, yLeftup));
			quad.setSize(Vector2f(xrightdown, yrightdown));
			quad.setPosition(Vector2f(xLeftUp, yLeftup));
			quad.setFillColor(col);
			quad.setOutlineThickness(2);
			quad.setOutlineColor(Color::Black);
			Text text(Text11, font, 40);
			text.setPosition(xLeftUp+10,yLeftup);
			text.setString(Text11);
			text.setFillColor(Color::Black);
			renderwindow->draw(quad);
			renderwindow->draw(text);
			//retclick = 0;
		}
		else
		{
			if (*click == 0) {
				RectangleShape quad = RectangleShape(Vector2f(xLeftUp, yLeftup));
				quad.setSize(Vector2f(xrightdown, yrightdown));
				quad.setPosition(Vector2f(xLeftUp, yLeftup));
				quad.setFillColor(col+Color(30,30,30));
				quad.setOutlineThickness(2);
				quad.setOutlineColor(Color::Black);
				Text text(Text11, font, 40);
				text.setPosition(xLeftUp+10, yLeftup);
				text.setString(Text11);
				text.setFillColor(Color::Black);
				renderwindow->draw(quad);
				renderwindow->draw(text);
			}
			else if(*click == 1)
			{
				*click = 0;
				RectangleShape quad = RectangleShape(Vector2f(xLeftUp, yLeftup));
				quad.setSize(Vector2f(xrightdown, yrightdown));
				quad.setPosition(Vector2f(xLeftUp, yLeftup));
				quad.setFillColor(col + Color(50, 50, 50));
				quad.setOutlineThickness(2);
				quad.setOutlineColor(Color::Black);
				Text text(Text11, font, 40);
				text.setPosition(xLeftUp + 10, yLeftup);
				text.setString(Text11);
				text.setFillColor(Color::Black);
				renderwindow->draw(quad);
				renderwindow->draw(text);
				Function();
			}
		}			
	}
};

int chet = 0;
int level = 2;
bool iswrite = false;
char CheckersPos[8][8];
int mas[16][2];
bool oldcheck1 = !true;
bool realoldcheck = false;
bool masHighLight[8][8];
bool isPlay = !true;
int isclick = 0;
int masdelcheck[48][48];
int xold, yold;
int empityX, empityY;
int NumberStep;
bool endcalculate = true;
bool stepcompl = true;
string oldCheckersPos = "";

int WhiteChecks(char* CheckersPos1) {
	int countwhitecheckers = 0;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (CheckersPos1[8 * i + j] == 'W') {
				countwhitecheckers++;
			}
			if (CheckersPos1[8 * i + j] == 'K') {
				countwhitecheckers += 2;
			}
		}
	}
	return countwhitecheckers;
}
int BlackChecks(char* CheckersPos1) {
	int countblackcheckers = 0;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {

			if (CheckersPos1[8 * i + j] == 'B') {
				countblackcheckers++;
			}
			if (CheckersPos1[8 * i + j] == 'Q') {
				countblackcheckers += 2;
			}
		}
	}
	return countblackcheckers;
}
void draw(RenderWindow *Rw) {
	RectangleShape quad = RectangleShape(Vector2f(80, 80));
	quad.setSize(Vector2f(5,650));
	quad.setFillColor(Color::Black);
	quad.setPosition(15,75);
	Rw->draw(quad);
	quad.setPosition(660, 75);
	Rw->draw(quad);
	quad.setSize(Vector2f(650, 5));
	quad.setFillColor(Color::Black);
	quad.setPosition(15, 75);
	Rw->draw(quad);
	quad.setPosition(15,720);
	Rw->draw(quad);
	quad.setSize(Vector2f(80, 80));
	for (int i = 0;i < 8; i++) {
		for (int j = 0;j < 8; j++) {
			if (masHighLight[i][j]&&endcalculate) {
				quad.setOutlineThickness(5);
				quad.setOutlineColor(Color::Green);
				quad.setSize(Vector2f(75, 75));
			}
			else
			{
				quad.setSize(Vector2f(80, 80));
				quad.setOutlineThickness(0);
			}
			quad.setPosition(i*80+20,j*80+80);
			if ((i + j) % 2 != 0) {
				quad.setFillColor(Color(150,42,42));
			}				
			else 
			{
				quad.setFillColor(Color::White);
			}
			Rw->draw(quad);
		}
	}
}

void deletemas();
void NextStep(int x, int y, char CheckersPos1[8][8], int xold1, int yold1,bool correct);
void drawnextstep(int x, int y, RenderWindow* RW, char CheckersPos1[8][8],bool oldcheck2);

void LouseorWin() {
	bool BW = false, WW = false;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (CheckersPos[i][j] == 'W' || CheckersPos[i][j] == 'K') {
				drawnextstep(i, j, NULL, CheckersPos, false);
				if (mas[0][0] != -1)WW = true;
			}
			if (CheckersPos[i][j] == 'B' || CheckersPos[i][j] == 'Q') {
				drawnextstep(i, j, NULL, CheckersPos, true);
				if (mas[0][0] != -1)BW = true;
			}
			deletemas();
		}
	}
	if (!BW)isPlay = false;
	if (!WW) isPlay = false;
}

void mas22(char a[8][8],char b[8][8]) {
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j<8;j++)
		{
			a[i][j] = b[i][j];
		}
	}
 }

void mas3(int a[16][2], int b[16][2]) {
	for (int i = 0;i < 16;i++) {
		for (int j = 0;j < 2;j++)
		{
			a[i][j] = b[i][j];
		}
	}
}
void mas4(int a[48][48], int b[48][48]) {
	for (int i = 0;i < 48;i++) {
		for (int j = 0;j < 48;j++)
		{
			a[i][j] = b[i][j];
		}
	}
}

int thebeststep(bool  colorStep,char Pos[8][8],int countstep,bool firststep) {
	int num;
	int deltaK = 0;
	int empityX1 = 0, empityY1 = 0, NumberStep1 = 0;
	if (countstep == 0) 
	{
		if (colorStep) {
			return BlackChecks(*Pos) - WhiteChecks(*Pos);
		}
		else
		{
			return WhiteChecks(*Pos) - BlackChecks(*Pos);
		}
	}
	else
	{
		char newpos[8][8],PosCopy[8][8];
		mas22(PosCopy, Pos);
		mas22(newpos, PosCopy);
		RenderWindow rwww;
		bool startx = false;
		for (int i = 0;i < 8;i++) 
		{
			for (int j = 0;j < 8;j++) 
			{
				mas22(newpos, PosCopy);
				drawnextstep(i, j,&rwww , newpos,colorStep);
				int dopmas[16][2];
				int dopdelmas[48][48];
				mas3(dopmas, mas);
				mas4(dopdelmas, masdelcheck);
				int k = 0;				
				while (dopmas[k][0]>-1)
				{					
					mas3(mas, dopmas);
					mas4(dopdelmas, masdelcheck);
					mas22(newpos, PosCopy);
					NextStep(dopmas[k][0],dopmas[k][1],newpos,i,j,false);
					k++;
					int nextpoint = thebeststep((!colorStep), newpos, (countstep-1),false);		
					
					if (countstep%2 ==1) {
						if (!startx) {
							deltaK = nextpoint;
							startx = true;
							empityX1 = i;
							empityY1 = j;
							NumberStep1 = k-1;
						}
						else
						{
							if (nextpoint < deltaK) {
								deltaK = nextpoint;
								empityX1 = i;
								empityY1 = j;
								NumberStep1 = k - 1;
							}
							if (nextpoint == deltaK) {
								int random = rand() % 2;
								if (random == 1) {
									empityX1 = i;
									empityY1 = j;
									NumberStep1 = k - 1;
								}
							}
						}
					}
					else
					{
						if (!startx) {
							deltaK = nextpoint;
							startx = true;
							empityX1 = i;
							empityY1 = j;
							NumberStep = k - 1;
						}
						else
						{
							if (nextpoint > deltaK) {
								deltaK = nextpoint;
								empityX1 = i;
								empityY1 = j;
								NumberStep1 = k - 1;
							}
							if (nextpoint == deltaK) {
								int random = rand() % 2;
								if (random == 1) {
									empityX1 = i;
									empityY1 = j;
									NumberStep1 = k - 1;
								}
							}
						}
					}
				}
				deletemas();
			}
		}
		if (!startx && countstep%2 == 1&&countstep!=1) {
			deltaK = 1000;
		}
		else if (!startx && countstep % 2 == 0 && countstep != 1)
		{
			deltaK = -1000;
		}
	}
	empityX = empityX1;
	empityY = empityY1;
	NumberStep = NumberStep1;
	if (countstep == level) {
		endcalculate = true;
	}
	return deltaK;
}

void DrawCheckers(RenderWindow *Rw) {
	CircleShape circle = CircleShape(10);
	circle.setScale(Vector2f(3,3));
	circle.setOutlineThickness(1);
	circle.setOutlineColor(Color::Magenta);
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if (CheckersPos[i][j] != ' ') 
			{
				if (CheckersPos[i][j] =='W') {
					circle.setFillColor(Color::White);
					circle.setPosition(i*80+28.5,j*80+88.5);
					Rw->draw(circle);
				}
				else if(CheckersPos[i][j] == 'B')
				{
					circle.setFillColor(Color::Black);
					circle.setPosition(i * 80 + 28.5, j * 80 + 88.5);
					Rw->draw(circle);
				}
				else if (CheckersPos[i][j] == 'Q')
				{
					circle.setOutlineThickness(1.5);
					circle.setOutlineColor(Color::Yellow);
					circle.setFillColor(Color::Black);
					circle.setPosition(i * 80 + 28.5, j * 80 + 88.5);
					Rw->draw(circle);
					circle.setOutlineThickness(1);
					circle.setOutlineColor(Color::Magenta);
				}
				else if (CheckersPos[i][j] == 'K')
				{
					circle.setOutlineThickness(1.5);
					circle.setOutlineColor(Color::Yellow);
					circle.setFillColor(Color::White);
					circle.setPosition(i * 80 + 28.5, j * 80 + 88.5);
					Rw->draw(circle);
					circle.setOutlineThickness(1);
					circle.setOutlineColor(Color::Magenta);
				}
			}
		}
	}
}

int cutting(char thischeck, int x, int y, int count,int oldx,int oldy,int countdel);
int cuttingQueen(char thischeck, int x, int y, int count1,int olddirectionmove, int countdel,int*deletemassive,int countiteration,char CheckersPos1[8][8]);
void isQueen(char CheckersPos1[8][8]);

void deletemas() {
	for (int i = 0;i < 16;i++) {
		for (int j = 0;j < 2;j++) {
			mas[i][j] = -1;
		}
	}
	for (int i = 0;i < 48;i++) {
		for (int j = 0;j < 48;j++) {
			masdelcheck[i][j] = -(1);
		}
	}
}
bool PossibilityNextMove(char CheckersPos1[8][8], bool oldcheck3)
{
	for (int i = 0;i < 8;i++)
		for (int j = 0;j < 8;j++)
			masHighLight[i][j] = false;
	bool ret = false;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
				int* a = new int[0];				
				if (CheckersPos1[i][j] == 'W'&&!oldcheck3) {
					if (cuttingQueen(CheckersPos1[i][j], i, j, 0, 0, 0, a, 2, CheckersPos1)>0) {
						masHighLight[i][j] = true;
						ret =  true;
					}
				}
				if (CheckersPos1[i][j] == 'B' && oldcheck3) {
					if (cuttingQueen(CheckersPos1[i][j], i, j, 0, 0, 0, a, 2, CheckersPos1) > 0) {
						masHighLight[i][j] = true;
						ret = true;
					}
				}
				if (CheckersPos1[i][j] == 'Q' && oldcheck3) {
					if (cuttingQueen(CheckersPos1[i][j], i, j, 0, 0, 0, a, 8, CheckersPos1)>0) {
						masHighLight[i][j] = true;
						ret =  true;
					}
				}
				if (CheckersPos1[i][j] == 'K'&&!oldcheck3) {
					if (cuttingQueen(CheckersPos1[i][j], i, j, 0, 0, 0, a, 8, CheckersPos1) > 0) {
						masHighLight[i][j] = true;
						ret = true;
					}
				}
				delete []a ;
		}
	}
	deletemas();
	return ret;
}

void drawnextstep(int x, int y, RenderWindow* RW,char CheckersPos1[8][8],bool oldcheck2) {
	PossibilityNextMove(CheckersPos1,oldcheck2);
	int count = 0;
	CircleShape circle = CircleShape(10);
	circle.setScale(Vector2f(2, 2));
	circle.setFillColor(Color::Green);
	xold = x;
	yold = y;
	int countplace;
	int* a = new int[0];
	if (((CheckersPos1[x][y] == 'W'|| CheckersPos1[x][y] == 'K') && !oldcheck2) || ((CheckersPos1[x][y] == 'Q' || CheckersPos1[x][y] == 'B') && oldcheck2))
	{
	if (CheckersPos1[x][y] != ' ') {
		bool iscut = PossibilityNextMove(CheckersPos1,oldcheck2);
		if (CheckersPos1[x][y] == 'W' || CheckersPos1[x][y] == 'B') {
			if ((cuttingQueen(CheckersPos1[x][y], x, y, 0, -1, 0, a, 2, CheckersPos1) == 0) && (!iscut))
			{
				if (CheckersPos1[x+1][y+1] == ' ' && CheckersPos1[x][y] == 'W') {
					circle.setPosition((x + 1) * 80 + 35, (y + 1) * 80 + 95);
					mas[count][0] = x + 1;
					mas[count][1] = y + 1;
					count++;
				}
				if (CheckersPos1[x+1][y-1] == ' ' && CheckersPos1[x][y] == 'B') {
					circle.setPosition((x + 1) * 80 + 35, (y - 1) * 80 + 95);
					mas[count][0] = x + 1;
					mas[count][1] = y - 1;
					count++;
				}
				if (CheckersPos1[x - 1][y + 1] == ' ' && CheckersPos1[x][y] == 'W') {
					circle.setPosition((x - 1) * 80 + 35, (y + 1) * 80 + 95);
					mas[count][0] = x - 1;
					mas[count][1] = y + 1;
					count++;
				}
				if (CheckersPos1[x - 1][y - 1] == ' ' && CheckersPos1[x][y] == 'B') {
					circle.setPosition((x - 1) * 80 + 35, (y - 1) * 80 + 95);
					mas[count][0] = x - 1;
					mas[count][1] = y - 1;
					count++;
				}
			}
		}
		if (CheckersPos1[x][y] == 'Q' || CheckersPos1[x][y] == 'K') {
			if ((cuttingQueen(CheckersPos1[x][y], x, y, 0, -1, 0, a, 8,CheckersPos1) == 0) && (!iscut))
			{
				bool upright, upleft, downright, downleft;
				upleft = true;
				upright = true;
				downleft = true;
				downright = true;
				for (int xi = 1;xi < 8;xi++) {
					if (CheckersPos1[x + xi][y + xi] == ' ' && upright) {
						circle.setPosition((x + xi) * 80 + 35, (y + xi) * 80 + 95);
						
						if (x + xi < 8 && y + xi < 8)
						{
							mas[count][0] = x + xi;
							mas[count][1] = y + xi;
							count++;
						}
					}
					else if (CheckersPos1[x + xi][y + xi] != ' ') {
						upright = false;
					}
					if (CheckersPos1[x + xi][y - xi] == ' ' && upleft) {
						circle.setPosition((x + xi) * 80 + 35, (y - xi) * 80 + 95);
						if (x + xi < 8 && y - xi >= 0)
						{
							mas[count][0] = x + xi;
							mas[count][1] = y - xi;
							count++;
						}
						
					}
					else if (CheckersPos1[x + xi][y - xi] != ' ') {
						upleft = false;
					}
					if (CheckersPos1[x - xi][y + xi] == ' ' && downright) {
						circle.setPosition((x - xi) * 80 + 35, (y + xi) * 80 + 95);
						if (x - xi >= 0 && y + xi < 8)
						{
							mas[count][0] = x - xi;
							mas[count][1] = y + xi;
							count++;
						}
					}
					else if (CheckersPos1[x - xi][y + xi] != ' ') {
						downright = false;
					}
					if (CheckersPos1[x - xi][y - xi] == ' ' && downleft) {
						circle.setPosition((x - xi) * 80 + 35, (y - xi) * 80 + 95);
						
						if (x - xi >= 0 && y - xi >= 0)
						{
							mas[count][0] = x - xi;
							mas[count][1] = y - xi;
							count++;
						}
					}
					else if (CheckersPos1[x - xi][y - xi] != ' ') {
						downleft = false;
					}
				}
			}
		}
	}
}
delete []a;
}

void NextStep(int x,int y,char CheckersPos1[8][8],int xold1,int yold1,bool correct)
{
	bool ischet = false;
	for (int i =0;i<16;i++) 
	{
			if (mas[i][0] == x && mas[i][1] == y) 
			{
				oldcheck1 = !oldcheck1;
				char betwen;
				betwen = CheckersPos1[x][y];
				CheckersPos1[x][y] = CheckersPos1[xold1][yold1];
				CheckersPos1[xold1][yold1] = betwen;
				for (int j = 0;j < 24;j++) {
					CheckersPos1[masdelcheck[i][j*2]][masdelcheck[i][j*2+1]] = ' ';
				}
				isQueen(CheckersPos1);
				if (correct) 
				{
					realoldcheck = !realoldcheck;
					if (!ischet) {
						for (int i = 0;i < 8;i++)
							for (int j = 0;j < 8;j++)
								oldCheckersPos =oldCheckersPos+ CheckersPos1[j][i];
						chet++;
						ischet = true;
					}
					correct = false;
					LouseorWin();
				}
				break;
			}
	}
	PossibilityNextMove(CheckersPos1,realoldcheck);
}

void autoinsertmas(int* mas1, int* mas2, int lengthinsert,int numstr,int c) {
	if (c < 48) {
		for (int i = 0; i < lengthinsert; i++)
		{
			mas1[i] = mas2[i];
		}
	}
}
int cuttingQueen(char thischeck, int x, int y, int count1, int olddirectionmove, int countdel,int*deletemassive,int countiteration,char CheckersPos1[8][8])
{
	if (count1 < 16) {
		if (thischeck == 'K') {
			thischeck = 'W';
		}
		if (thischeck == 'Q') {
			thischeck = 'B';
		}
		bool isNextCut = false;
		for (int xt = 1;xt < countiteration;xt++) {
			char empitysimbol, empitysimbol1;
			empitysimbol = CheckersPos1[x + xt][y + xt];
			if (empitysimbol == 'K') {
				empitysimbol = 'W';
			}
			if (empitysimbol == 'Q') {
				empitysimbol = 'B';
			}
			empitysimbol1 = CheckersPos1[x + xt+1][y + xt+1];
			if (empitysimbol1 == 'K') {
				empitysimbol1 = 'W';
			}
			if (empitysimbol1 == 'Q') {
				empitysimbol1 = 'B';
			}
			bool isrepeat = true;
			for (int iii = 0;iii < countdel / 2;iii++) {
				if (deletemassive[iii * 2] == x + xt + 1 && deletemassive[iii * 2 + 1] == y + xt + 1) {
					isrepeat = false;
				}
			}
			if (empitysimbol != thischeck && empitysimbol != ' ' && empitysimbol1 == ' ' && olddirectionmove != 4 && x + xt + 1 < 8 && y + xt + 1 < 8 && isrepeat && countdel < 47)
			{
				int countdel1 = countdel;
				countdel1 += 2;
				int* mas2 = new int[countdel1];
				autoinsertmas(mas2, deletemassive, countdel1 - 2, 686, count1);
				mas2[countdel1 - 2] = x + xt;
				mas2[countdel1 - 1] = y + xt;
				for (int xit = xt + 1; xit < countiteration + 1; xit++)
				{
					if (x + xit < 8 && y + xit < 8) {
						if (CheckersPos1[x + xit][y + xit] == ' ') {
							int count_new = cuttingQueen(thischeck, x + xit, y + xit, count1, 1, countdel + 2, mas2, countiteration, CheckersPos1);
							if (count1 < count_new) {
								isNextCut = true;
								count1 = count_new;
							}
						}
					}
				}
				if (!isNextCut) {
					for (int xit = xt + 1;xit < countiteration + 1;xit++)
					{
						if (xit + x < 8 && xit + y < 8 && CheckersPos1[x + xit][y + xit] == ' ') {
							mas[count1][0] = x + xit;
							mas[count1][1] = y + xit;
							autoinsertmas(masdelcheck[count1], mas2, countdel1, 709, count1);
							count1++;
						}
						else
						{
							break;
						}
					}
				}
				delete []mas2;

				break;
			}
			else if ((empitysimbol != ' ' && empitysimbol1 != ' ') || empitysimbol == thischeck) {
				break;
			}
		}
		isNextCut = false;
		for (int xt = 1;xt < countiteration;xt++) {
			char empitysimbol, empitysimbol1;
			empitysimbol = CheckersPos1[x + xt][y - xt];
			if (empitysimbol == 'K') {
				empitysimbol = 'W';
			}
			if (empitysimbol == 'Q') {
				empitysimbol = 'B';
			}
			empitysimbol1 = CheckersPos1[x + xt+1][y - xt-1];
			if (empitysimbol1 == 'K') {
				empitysimbol1 = 'W';
			}
			if (empitysimbol1 == 'Q') {
				empitysimbol1 = 'B';
			}
			bool isrepeat = true;
			for (int iii = 0;iii < countdel / 2;iii++) {
				if (deletemassive[iii * 2] == x + xt + 1 && deletemassive[iii * 2 + 1] == y - xt - 1) {
					isrepeat = false;
				}
			}
			if (empitysimbol != thischeck && empitysimbol != ' ' && empitysimbol1 == ' ' && olddirectionmove != 3 && x + xt + 1 < 8 && y - xt - 1 >= 0 && isrepeat && countdel < 47) {
				int countdel1 = countdel;
				countdel1 += 2;
				int* mas2 = new int[countdel1];
				autoinsertmas(mas2, deletemassive, countdel1 - 2, 755, count1);
				mas2[countdel1 - 2] = x + xt;
				mas2[countdel1 - 1] = y - xt;
				for (int xit = xt + 1; xit < countiteration + 1; xit++)
				{
					if (x + xit < 8 && y - xit >= 0) {
						if (CheckersPos1[x + xit][y - xit] == ' ') {
							int count_new = cuttingQueen(thischeck, x + xit, y - xit, count1, 2, countdel + 2, mas2, countiteration, CheckersPos1);
							if (count1 < count_new) {
								isNextCut = true;
								count1 = count_new;
							}
						}
					}
				}
				if (!isNextCut) {
					for (int xit = xt + 1;xit < countiteration + 1;xit++)
					{
						if (xit + x < 8 && y - xit >= 0 && CheckersPos1[x + xit][y - xit] == ' ') {
							mas[count1][0] = x + xit;
							mas[count1][1] = y - xit;
							autoinsertmas(masdelcheck[count1], mas2, countdel1, 778, count1);
							count1++;
						}
						else
						{
							break;
						}
					}
				}
				delete []mas2;
				break;
			}
			else if ((empitysimbol != ' ' && empitysimbol1 != ' ') || empitysimbol == thischeck) {
				break;
			}
		}
		isNextCut = false;
		for (int xt = 1;xt < countiteration;xt++) {
			char empitysimbol, empitysimbol1;
			empitysimbol = CheckersPos1[x - xt][y + xt];
			if (empitysimbol == 'K') {
				empitysimbol = 'W';
			}
			if (empitysimbol == 'Q') {
				empitysimbol = 'B';
			}
			empitysimbol1 = CheckersPos1[x - xt-1][y + xt+1];
			if (empitysimbol1 == 'K') {
				empitysimbol1 = 'W';
			}
			if (empitysimbol1 == 'Q') {
				empitysimbol1 = 'B';
			}
			bool isrepeat = true;
			for (int iii = 0;iii < countdel / 2;iii++) 
			{
				if (deletemassive[iii * 2] == x - xt - 1 && deletemassive[iii * 2 + 1] == y + xt + 1) 
				{
					isrepeat = false;
				}
			}
			if (empitysimbol != thischeck && empitysimbol != ' ' && empitysimbol1 == ' ' && olddirectionmove != 2 && x - xt - 1 >= 0 && y + xt + 1 < 8 && isrepeat && countdel < 47) 
			{
				int countdel1 = countdel;
				countdel1 += 2;
				int* mas2 = new int[countdel1];
				autoinsertmas(mas2, deletemassive, countdel1 - 2, 823, count1);
				mas2[countdel1 - 2] = x - xt;
				mas2[countdel1 - 1] = y + xt;
				for (int xit = xt + 1; xit < countiteration + 1; xit++)
				{
					if (x - xit >= 0 && y + xit < 8) 
					{
						if (CheckersPos1[x - xit][y + xit] == ' ') 
						{
							int count_new = cuttingQueen(thischeck, x - xit, y + xit, count1, 3, countdel + 2, mas2, countiteration, CheckersPos1);
							if (count1 < count_new) 
							{
								isNextCut = true;
								count1 = count_new;
							}
						}
					}
				}
				if (!isNextCut) {
					for (int xit = xt + 1;xit < countiteration + 1;xit++)
					{
						if (x - xit >= 0 && xit + y < 8 && CheckersPos1[x - xit][y + xit] == ' ')
						{
							mas[count1][0] = x - xit;
							mas[count1][1] = y + xit;
							autoinsertmas(masdelcheck[count1], mas2, countdel1, 846, count1);
							count1++;
						}
						else
						{
							break;
						}
					}
				}
				delete []mas2;
				break;
			}
			else if ((empitysimbol != ' ' && empitysimbol1 != ' ') || empitysimbol == thischeck) {
				break;
			}
		}
		isNextCut = false;
		for (int xt = 1;xt < countiteration;xt++) 
		{
			char empitysimbol, empitysimbol1;
			empitysimbol = CheckersPos1[x - xt][y - xt];
			if (empitysimbol == 'K') {
				empitysimbol = 'W';
			}
			if (empitysimbol == 'Q') {
				empitysimbol = 'B';
			}
			empitysimbol1 = CheckersPos1[x - xt	-1][y - xt-1];
			if (empitysimbol1 == 'K') {
				empitysimbol1 = 'W';
			}
			if (empitysimbol1 == 'Q') {
				empitysimbol1 = 'B';
			}
			bool isrepeat = true;
			for (int iii = 0;iii < countdel / 2;iii++) 
			{
				if (deletemassive[iii * 2] == x - xt - 1 && deletemassive[iii * 2 + 1] == y - xt - 1) {
					isrepeat = false;
				}
			}
			if (empitysimbol != thischeck && empitysimbol != ' ' && empitysimbol1 == ' ' && olddirectionmove != 1 && x - xt - 1 >= 0 && y - xt - 1 >= 0 && isrepeat && countdel < 47) 
			{
				int countdel1 = countdel;
				countdel1 += 2;
				int* mas2 = new int[countdel1];
				autoinsertmas(mas2, deletemassive, countdel1 - 2, 891, count1);
				mas2[countdel1 - 2] = x - xt;
				mas2[countdel1 - 1] = y - xt;
				for (int xit = xt + 1; xit < countiteration + 1; xit++)
				{
					if (x - xit >= 0 && y - xit >= 0) {
						if (CheckersPos1[x - xit][y - xit] == ' ') {
							int count_new = cuttingQueen(thischeck, x - xit, y - xit, count1, 4, countdel + 2, mas2, countiteration, CheckersPos1);
							if (count1 < count_new) {
								isNextCut = true;
								count1 = count_new;
							}
						}
					}
				}
				if (!isNextCut) {
					for (int xit = xt + 1;xit < countiteration + 1;xit++)
					{
						if (x - xit >= 0 && y - xit >= 0 && CheckersPos1[x - xit][y - xit] == ' ') {
							mas[count1][0] = x - xit;
							mas[count1][1] = y - xit;
							autoinsertmas(masdelcheck[count1], mas2, countdel1, 914, count1);
							count1++;
						}
						else
						{
							break;
						}
					}
				}
				delete []mas2;
				break;
			}
			else if ((empitysimbol != ' ' && empitysimbol1 != ' ') || empitysimbol == thischeck) {
				break;

			}
		}
	}
	return count1;
}

void rendernextstep(RenderWindow *RW){
	CircleShape circle = CircleShape(10);
	circle.setScale(Vector2f(2, 2));
	circle.setFillColor(Color::Green);
	for (int i = 0;i < 16;i++)
	{
		if (mas[i][0] > 0 || mas[i][1] > 0) {
			circle.setPosition((mas[i][0]) * 80 + 35, (mas[i][1]) * 80 + 95);
			RW->draw(circle);
		}
	}
}
void isQueen(char CheckersPos1[8][8]) 
{
	for (int i = 0;i < 8;i++) {
		if (CheckersPos1[i][0] == 'B') {
			CheckersPos1[i][0] = 'Q';
		}
    } 
	for (int i = 0;i < 8;i++) {
		if (CheckersPos1[i][7] == 'W') {
			CheckersPos1[i][7] = 'K';
		}
	}
}
void Restart();
void Easy() {
	isPlay = true;
	level = 2;
	Restart();
}
void Medium() {
	isPlay = true;
	level = 4;
	Restart();
}
void Hard() {
	isPlay = true;
	level = 6;
	Restart();
}
void Menu(){
	isPlay = false;
	Restart();
}
void Restart()
{
	oldcheck1 = !true;
	realoldcheck = false;
	isclick = false;
	deletemas();
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			masHighLight[i][j] = false;
			}
		}
	oldCheckersPos = "";
	chet = 0;
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if ((i + j) % 2 == 1) {
				if (i < 3) {
					CheckersPos[j][i] = 'W';
				}
				else if (i > 4) {
					CheckersPos[j][i] = 'B';
				}
				else {
					CheckersPos[j][i] = ' ';
				}
			}
			oldCheckersPos += CheckersPos[j][i];
		}
	}
	bool endcalculate = true;
	bool stepcompl = true;
}

void Writeln() {
	iswrite = !iswrite;
	if (chet > 1) {
		int j = 0, k = 0;
		for (int i = (chet - 2) * 64;i < (chet-1) * 64;i++) 
		{
			CheckersPos[k][j] = oldCheckersPos[i];
			k++;
			if (k > 7) {
				j++;
				k = 0;
			}
		}
		string ds = "";
		for (int i = 0;i<(chet - 1) * 64;i++)
		{
			ds+= oldCheckersPos[i];
		}
		oldCheckersPos = ds;
		chet-=2;
	}
}

int main()
{
	font.loadFromFile("17838.ttf");
	srand(NULL);
	RenderWindow window(VideoMode(680, 780), "Chekers");
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			if ((i + j)%2 == 1) {
				if (i < 3) {
					CheckersPos[j][i] = 'W';
				}
				else if (i > 4) {
					CheckersPos[j][i] = 'B';
				}
				else {
					CheckersPos[j][i] = ' ';
				}
			}
			oldCheckersPos += CheckersPos[j][i];
		}
	}
	chet = 0;
	window.setFramerateLimit(30);
	int a = -100, b = -100;
	Button but(&window,"Easy",Color(150,150,150),100,100,200,50,Easy);
	Button but1(&window, "MEDIUM", Color(150, 150, 150), 100, 200, 200, 50, Medium);
	Button but2(&window, "HARD", Color(150, 150, 150), 100, 300, 200, 50, Hard);
	Button but3(&window, "Menu", Color(150, 150, 150), 300, 10 , 100, 50, Menu);
	Button rest(&window, "RESTART", Color(150, 150, 150), 10, 10, 200, 50, Restart);
	Button Write(&window, "<", Color(150, 150, 150), 600, 10, 50, 50, Writeln);
	while (window.isOpen())
	{
			window.clear(Color::White);
			if (!isPlay) {
				but.Render(Mouse::getPosition(window).x, Mouse::getPosition(window).y, &isclick);
				but1.Render(Mouse::getPosition(window).x, Mouse::getPosition(window).y, &isclick);
				but2.Render(Mouse::getPosition(window).x, Mouse::getPosition(window).y, &isclick);
			}
			Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();
					if (event.type == Event::MouseButtonPressed)
					{
						Vector2i pos = Mouse::getPosition(window);
						a = pos.x;
						b = pos.y;
						if (isPlay)
						{
							if (realoldcheck)
							{
								NextStep((a - 20) / 80, (b - 80) / 80, CheckersPos, xold, yold, true);
								drawnextstep((a - 20) / 80, (b - 80) / 80, &window, CheckersPos, realoldcheck);
							}
						}
						isQueen(CheckersPos);
						if (isclick == 1) {
							isclick = 2;
						}
						else if(isclick == 0)
						{
							isclick = 1;
						}
					}
					else
					{
						isclick = 0;
					}
				}
				if (isPlay)
				{
					but3.Render(Mouse::getPosition(window).x, Mouse::getPosition(window).y, &isclick);
					rest.Render(Mouse::getPosition(window).x, Mouse::getPosition(window).y, &isclick);
					Write.Render(Mouse::getPosition(window).x, Mouse::getPosition(window).y, &isclick);
					draw(&window);
					DrawCheckers(&window);
					if (endcalculate) {
						rendernextstep(&window);
					}
				}
				window.display();
				if (isPlay)
				{
					if (!realoldcheck) {
						bool VremCheck = realoldcheck;
						//cout << "Beststep = " << thebeststep(realoldcheck, CheckersPos, level, true) << endl;
						if (endcalculate&&stepcompl) {
							stepcompl = false;
							endcalculate = false;
							thread tr(thebeststep, realoldcheck, CheckersPos, level, true);
							tr.detach();
						}
						realoldcheck = VremCheck;
						drawnextstep(empityX, empityY, &window, CheckersPos, realoldcheck);
						if (endcalculate) {
							if (mas[NumberStep][0] != -1) {
								NextStep(mas[NumberStep][0], mas[NumberStep][1], CheckersPos, empityX, empityY, true);
								stepcompl = true;
							}
						}
					}
				}
	}
	return 0;
}
