#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <time.h>
#include <math.h>
#define SQ 24
#define SCREEN_HEIGHT 500 //�]�w�C���������� 
#define SCREEN_WIDTH 500 //�]�w�C�������e��
#define GRID_NUM 20 //�]�w�C����}�C���l�ƶq 
#define LEFT_MARGINE 30 //�]�w����� 
#define TOP_MARGINE 45 //�]�w�W��� 
#define INIT_SPEED  80 //�]�w��l�t�� 
#define PLAYONE_COLOR  GREEN //�]�w���a�@�C��
#define PLAYTWO_COLOR  RED //�]�w���a�G�C��

//�ŧi�Ѥl���� 
enum Chess {
	EMPTY, //�ť� 
	PLAY_ONE, //���a�@ 
	PLAY_TWO //���a�G 
};

//�ŧi�Ѥl��l 
struct Location{
	int row; 
	int col;
};

void openWindow(); //�}�Ҵ������� 
void closeGame(); //�B�z�C�������޿� 
void playGame(int field[][GRID_NUM]); //�C���i���޿� 
bool checkFive(int field[][GRID_NUM], Location chessLoc); //�P�_�O�_���ѳs�u 
void showGameWinMsg(); //�C�������T��
void showInfo(); //��ܹC��������T 
void drawChessboard(int field[][GRID_NUM]); //ø�s�C���ϰ� 
void drawSquare(int row, int col, int color); //ø�s��� 
void drawFocus(int field[][GRID_NUM], Location focus); //ø�s�ؼФQ�r 
void drawLink(Location src, Location dest); //ø�s�s�u 
void cleanFocus(int field[][GRID_NUM], Location focus);//�M���Q�r 
Location* getChessDown(int field[][GRID_NUM], Location *focusPtr); //���o���a���l��m 
void setChess(int field[][GRID_NUM], Location chessLoc); //�]�w���l 

Location* PLAYONE_AI(int field[][GRID_NUM], Location *focusPtr);//��@�q�������޿� 

char key; //�ާ@���� 
int speed; //���ʳt�� 
int currentPlayer = PLAY_ONE;
int totalTime = 0; //�����C���ɶ� 
bool IFPlayAI = false; //�O�_�}��AI�Ҧ� 
int weightboard[20][20];
int G[GRID_NUM][GRID_NUM];
bool first=true;
void Giveweight(int who,int array[],int mode,int x,int y);
void judge(int who,int x,int y);

void Giveweight(int who,int array[],int mode,int x,int y){
	printf("|");
		for(int i=0;i<7;i++)
			printf("%d",array[i]);
		printf("/");
		printf("\n");
	int oponent=-3-who;
	bool visit=false;
	int sample1[SQ][7]={
	{0,2,1,1,1,1,0},
	{0,2,1,1,1,0,1},
	{0,2,1,0,1,1,1},
	{0,2,1,1,0,1,1},
	{0,2,1,1,1,0,0},
	{0,0,1,1,1,1,0},
	{0,0,1,1,1,0,0},
	{0,0,1,1,1,0,1},
	{0,0,1,1,0,1,1},
	{0,0,1,0,1,1,1},
	{2,0,1,1,1,0,1},
	{2,0,1,1,0,1,1},
	{2,0,1,0,1,1,1},
	{1,1,1,0,1,1,0},
	{1,1,1,0,1,0,0},
	{2,1,1,1,0,0,0},
	{2,1,1,0,1,0,0},
	{2,1,1,0,1,1,0},
	{2,1,1,0,1,1,1},
	{2,0,1,1,1,0,0},
	{0,0,1,0,0,0,0},
	{0,0,1,1,1,1,2},
	{0,0,1,2,0,0,0},
	{0,1,1,1,1,2,0}
	};
	
	int weight[SQ]={1000,1000,1000,1000,1000,600,1000,1200,1000,1000,1000,1000,1000,1000,1000,600,600,1000,1000,1000,100,1000,30,1000};
	int sample2[SQ][7];
	//printf("%d\n",weight[20]);
	for(int i=0;i<SQ;i++)
		for(int j=0;j<7;j++){
			if(sample1[i][j]==1)
				sample2[i][j]=who;
			else if(sample1[i][j]==2)
				sample2[i][j]=oponent;
			else
			sample2[i][j]=sample1[i][j];
		}
	int i,j;
	for(i=0;i<SQ;i++){
		for(j=0;j<7;j++) 
			if(array[j]!=sample2[i][j])
				break;
		if(j == 7){
			printf("%d",i);
			visit=true;
			if(mode==1){
				for(int u=0;u<7;u++){
					if(array[u]!=who&&array[u]!=oponent){
						if(array[u+1]==who&&array[u-1]==who)
							weightboard[x][y-u-(-2)]=weightboard[x][y-u-(-2)]+500;
						if(array[u+1]==who&&array[u-1]!=oponent){
							printf("s");
								weightboard[x][y-u-(-2)]=weightboard[x][y-u-(-2)]+weight[i];
						}
						
						else if(array[u-1]==who)						 
							weightboard[x][y-u-(-2)]=weightboard[x][y-u-(-2)]+weight[i];
										
					}	
				}
			}
			if(mode==2){
				for(int u=0;u<7;u++){
					if(array[u]!=who&&array[u]!=oponent){
						if(array[u+1]==who&&array[u-1]==who)
							weightboard[x+u+(-2)][y-u-(-2)]=weightboard[x+u+(-2)][y-u-(-2)]+500;
						if(array[u+1]==who&&array[u-1]!=oponent)
							weightboard[x+u+(-2)][y-u-(-2)]=weightboard[x+u+(-2)][y-u-(-2)]+weight[i];
						else if(array[u-1]==who)
							weightboard[x+u+(-2)][y-u-(-2)]=weightboard[x+u+(-2)][y-u-(-2)]+weight[i];
					}	
				}
			}
			if(mode==3){
				for(int u=0;u<7;u++){
					if(array[u]!=who&&array[u]!=oponent){
						if(array[u+1]==who&&array[u-1]==who)
							weightboard[x+u+(-2)][y]=weightboard[x+u+(-2)][y]+500;
						if(array[u+1]==who&&array[u-1]!=oponent)
							weightboard[x+u+(-2)][y]=weightboard[x+u+(-2)][y]+weight[i];
						else if(array[u-1]==who)
							weightboard[x+u+(-2)][y]=weightboard[x+u+(-2)][y]+weight[i];
					}	
				}
			}
			if(mode==4){
				for(int u=0;u<7;u++){
					if(array[u]!=who&&array[u]!=oponent){
						if(array[u+1]==who&&array[u-1]==who)
							weightboard[x+u+(-2)][y+u+(-2)]=weightboard[x+u+(-2)][y+u-(-2)]+500;
						if(array[u+1]==who&&array[u-1]!=oponent)
							weightboard[x+u+(-2)][y+u+(-2)]=weightboard[x+u+(-2)][y+u-(-2)]+weight[i];
						else if(array[u-1]==who)
							weightboard[x+u+(-2)][y+u+(-2)]=weightboard[x+u+(-2)][y+u-(-2)]+weight[i];
					}	
				}
			}
			if(mode==5){
				for(int u=0;u<7;u++){
					if(array[u]!=who&&array[u]!=oponent){
						if(array[u+1]==who&&array[u-1]==who)
							weightboard[x][y+u+(-2)]=weightboard[x][y+u+(-2)]+500;
						if(array[u+1]==who&&array[u-1]!=oponent)
							weightboard[x][y+u+(-2)]=weightboard[x][y+u+(-2)]+weight[i];
						else if(array[u-1]==who)
							weightboard[x][y+u+(-2)]=weightboard[x][y+u+(-2)]+weight[i];
					}	
				}
			}
			if(mode==6){
				for(int u=0;u<7;u++){
					if(array[u]!=who&&array[u]!=oponent){
						if(array[u+1]==who&&array[u-1]==who)
							weightboard[x-u-(-2)][y+u+(-2)]=weightboard[x-u-(-2)][y+u+(-2)]+500;
						if(array[u+1]==who&&array[u-1]!=oponent)
							weightboard[x-u-(-2)][y+u+(-2)]=weightboard[x-u-(-2)][y+u+(-2)]+weight[i];
						else if(array[u-1]==who)
							weightboard[x-u-(-2)][y+u+(-2)]=weightboard[x-u-(-2)][y+u+(-2)]+weight[i];
					}	
				}
			}
			if(mode==7){
				for(int u=0;u<7;u++){
					if(array[u]!=who&&array[u]!=oponent){
						if(array[u+1]==who&&array[u-1]==who)
							weightboard[x-u-(-2)][y]=weightboard[x-u-(-2)][y]+500;
						if(array[u+1]==who&&array[u-1]!=oponent)
							weightboard[x-u-(-2)][y]=weightboard[x-u-(-2)][y]+weight[i];
						else if(array[u-1]==who)
							weightboard[x-u-(-2)][y]=weightboard[x-u-(-2)][y]+weight[i];
					}	
				}
			}
			if(mode==8){
				for(int u=0;u<7;u++){
					if(array[u]!=who&&array[u]!=oponent){
						if(array[u+1]==who&&array[u-1]==who)
							weightboard[x-u-(-2)][y-u-(-2)]=weightboard[x-u-(-2)][y-u-(-2)]+500;
						if(array[u+1]==who&&array[u-1]!=oponent)
							weightboard[x-u-(-2)][y-u-(-2)]=weightboard[x-u-(-2)][y-u-(-2)]+weight[i];
						else if(array[u-1]==who)
							weightboard[x-u-(-2)][y-u-(-2)]=weightboard[x-u-(-2)][y-u-(-2)]+weight[i];
					}	
				}
			}
		}
	}
				
}
void judge(int who,int x,int y){
	bool flag;
	G[x][y]=who;
	for(int j=1;j<=8;j++){
        int sample[7];
		for(int i=-2;i<5;i++){
			if(j==1){ 
				if(G[x][y-i]>0)
					sample[2+i]=0;
				else
					sample[2+i]=G[x][y-i];
			} 
        	if(j==2){
				if(G[x+i][y-i]>0)
					sample[2+i]=0;
				else
					sample[2+i]=G[x+i][y-i];
			}
           	if(j==3){
				if(G[x+i][y]>0)
					sample[2+i]=0;
				else
					sample[2+i]=G[x+i][y];	
			}
        	if(j==4){
				if(G[x+i][y+i]>0)
					sample[2+i]=0;
				else
					sample[2+i]=G[x+i][y+i];
			}
           	if(j==5){
				if(G[x][y+i]>0)
					sample[2+i]=0;
				else
					sample[2+i]=G[x][y+i];   
			}
           	if(j==6){
				if(G[x-i][y+i]>0)
					sample[2+i]=0;
				else
					sample[2+i]=G[x-i][y+i];   
			}
           	if(j==7){
				if(G[x-i][y]>0)
					sample[2+i]=0;
				else
					sample[2+i]=G[x-i][y];   
			}
           	if(j==8){
				if(G[x-i][y-i]>0)
					sample[2+i]=0;
				else
					sample[2+i]=G[x-i][y-i];   
			}
        }
        Giveweight(who,sample,j,x,y);      
    }
    
}   
int main( )
{  	
	for(int i=0;i<20;i++)
		for(int j=0;j<20;j++){
			weightboard[i][j]=0;
			G[i][j]=0;
		}
	openWindow();
	while(TRUE){
	
		speed = INIT_SPEED;
		
		//�]�w�C�����M��ê�� 
	   	int field[GRID_NUM][GRID_NUM] = {0};
	   	
		playGame(field); //�i��C��
		if (key == 'q' || key == 'Q')
			closeGame(); //�p�G���a��J'q'���}�C��	
		else if (key == 's' || key == 'S')
			continue; //�p�G���a��J's' �~��C�� 		    
	}
}

//�}�Ҵ�������
void openWindow(){
	initwindow(SCREEN_WIDTH + LEFT_MARGINE * 3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Backgammon");
}

//�B�z�C�������޿� 
void closeGame(){
	exit(0);
}

//�C���i���޿�
void playGame(int field[][GRID_NUM]){
	
	drawChessboard(field); //ø�s�C���ϰ� 
	Location focus = {0, 0};
	currentPlayer = PLAY_ONE;
	Location* chessLocPtr;
	key = NULL;
	
	while(true){
		showInfo(); //��ܮɶ��M���Ƹ�T
		if(IFPlayAI){
			if(currentPlayer == PLAY_ONE)
				chessLocPtr = PLAYONE_AI(field, &focus);
			else
				chessLocPtr = getChessDown(field, &focus);	
		}else
			chessLocPtr = getChessDown(field, &focus);
			
		if(chessLocPtr != NULL){
			setChess(field, *chessLocPtr);
			int i,j;
			for(i=0;i<GRID_NUM;i++){
				for(j=0;j<GRID_NUM;j++)
					printf("%d ", field[i][j]);
				printf("\n");	
			}
			printf("\n");
			for(i=0;i<GRID_NUM;i++){
				for(j=0;j<GRID_NUM;j++)
					printf("%6d ", weightboard[i][j]);
				printf("\n");	
			}
			printf("\n");

			if(checkFive(field, *chessLocPtr)) //�P�_�O�_�ŦX�C����������A 
				showGameWinMsg(); //��ܹC�������T���A�õ��ݪ��a��J�ﶵ
			else{
				if(currentPlayer == PLAY_ONE){
					currentPlayer = PLAY_TWO;
				}else{
					currentPlayer = PLAY_ONE;
				}
			}	 
		}else{
			//�u����J�j�p�g��a, q �M s�A�t�Τ~�������ñN��J���G�ߨ�~�h���ݳB�z 
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S')
				return;				
			else if (key == 'a'){ 
				//�M�w�O�_���ܼҦ� �A�D�n�����a�Ҧ��M�q���ޱ���AI�Ҧ� 
				IFPlayAI = !IFPlayAI;
				key = NULL;
			}
		}
		delay(speed); //�M�w�C�^�X�U�ѳt�� 			
	}		
}

//ø�s�C���ϰ�A�̾ڹC�����x�}�]�wø�s���� 
void drawChessboard(int field[][GRID_NUM]){
   	int row = 0, col = 0;
   	int startX = 0, startY = 0, endX = 0, endY = 0;
   	cleardevice(); //�M�z�ù��e�� 
   	int squareHeight = SCREEN_HEIGHT / GRID_NUM;
   	int squareWidth = SCREEN_WIDTH / GRID_NUM;
   	setlinestyle(SOLID_LINE, 0, 1); 
   	setcolor(YELLOW);
	for(row = 0; row <= GRID_NUM; row ++){
		startX = LEFT_MARGINE + 0 * squareWidth + 0;
		startY =  TOP_MARGINE + row * squareHeight;
		endX = startX + GRID_NUM * squareWidth;
		line(startX, startY, endX, startY);
	}
	
	for(col = 0; col <=  GRID_NUM; col++){
		startX = LEFT_MARGINE + col * squareWidth;
		startY =  TOP_MARGINE + 0 * squareHeight + 0;
		endY = startY + GRID_NUM * squareHeight;
		line(startX, startY, startX, endY);
	}
}

//���o���a���l��m
Location* getChessDown(int field[][GRID_NUM], Location *focusPtr){
		
	char keyPress;
	if(kbhit()) 
		keyPress = getch();
		
	if(keyPress == ' ' && field[focusPtr->row][focusPtr->col] == EMPTY){
		judge(-2,focusPtr->row,focusPtr->col);
		return focusPtr;
	}
		
	cleanFocus(field, *focusPtr);
	//decide focus moving direction	
	switch(keyPress){
		case KEY_RIGHT:
			if(focusPtr->col < GRID_NUM - 1)
				focusPtr->col = focusPtr->col + 1;
			break;			
		case KEY_LEFT:
			if(focusPtr->col > 0)
				focusPtr->col = focusPtr->col - 1;
			break;		
		case KEY_UP:
			if(focusPtr->row > 0)
				focusPtr->row = focusPtr->row - 1;
			break;				
		case KEY_DOWN:
			if(focusPtr->row < GRID_NUM - 1)
				focusPtr->row = focusPtr->row + 1;
			break;				
	}
	
	drawFocus(field, *focusPtr);
	//�u����J�j�p�g��a, q �M s�A�t�Τ~�������ñN��J���G�ߨ�~�h���ݳB�z 
	if (keyPress == 'q' || keyPress == 'Q' || keyPress == 's' || keyPress == 'S' || keyPress == 'a' || keyPress == 'A'){
		key = keyPress;
	}
	return NULL;
}

//�]�w���l
void setChess(int field[][GRID_NUM], Location chessLoc){
	if(field[chessLoc.row][chessLoc.col] == EMPTY){
	
		field[chessLoc.row][chessLoc.col] = currentPlayer;
		if(currentPlayer == PLAY_ONE){
			drawSquare(chessLoc.row, chessLoc.col, PLAYONE_COLOR);
		}else{
			drawSquare(chessLoc.row, chessLoc.col, PLAYTWO_COLOR);
		}
	}
}

//ø�s�ؼФQ�r
void drawFocus(int field[][GRID_NUM], Location focus){
	int focusX = 0, focusY = 0;
	int squareHeight = SCREEN_HEIGHT / GRID_NUM;
   	int squareWidth = SCREEN_WIDTH / GRID_NUM;
   	focusX = LEFT_MARGINE + focus.col * squareWidth + squareWidth / 2;
   	focusY =  TOP_MARGINE + focus.row * squareHeight + squareHeight / 2;
   	setlinestyle(SOLID_LINE, 0, 1); 
   	setcolor(YELLOW);
   	line(focusX -5, focusY, focusX + 5, focusY);
   	line(focusX, focusY - 5, focusX, focusY + 5);
}

//�M���ؼФQ�r
void cleanFocus(int field[][GRID_NUM], Location focus){
	if(field[focus.row][focus.col] == PLAY_ONE)
		setcolor(PLAYONE_COLOR);
	else if(field[focus.row][focus.col] == PLAY_TWO)
		setcolor(PLAYTWO_COLOR);	
	else
		setcolor(BLACK);
			
	int focusX = 0, focusY = 0;
	int squareHeight = SCREEN_HEIGHT / GRID_NUM;
   	int squareWidth = SCREEN_WIDTH / GRID_NUM;
   	focusX = LEFT_MARGINE + focus.col * squareWidth + squareWidth / 2;
   	focusY =  TOP_MARGINE + focus.row * squareHeight + squareHeight / 2;
   	setlinestyle(SOLID_LINE, 0, 1); 
   	
   	line(focusX -5, focusY, focusX + 5, focusY);
   	line(focusX, focusY - 5, focusX, focusY + 5);
}

//ø�s���
void drawSquare(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_NUM;
   	int SquareWidth = SCREEN_WIDTH / GRID_NUM;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + 1;
   	top =  TOP_MARGINE + row * squareHeight + 1;
	right = left + SquareWidth - 1;
   	bottom = top + squareHeight - 1;
   	 
	setfillstyle(SOLID_FILL,color); //�]�wø�s���󪺬���ߩM�C�� 
   	bar(left , top, right, bottom);	//�]�wø�s�������m 
}

//�ˬd�O�_�������Ѥl�s�u 
bool checkFive(int field[][GRID_NUM], Location chessLoc){
	int i = 0, count = 1;
	int row = chessLoc.row;
	int col = chessLoc.col;
	Location head, tail; 
	head = chessLoc;
	tail = chessLoc;
	for(i = 1; i < 5; i ++){
		if(row - i >= 0 && field[row-i][col] == currentPlayer){
			count ++;
			head.row = row - i;
			head.col = col;
		}
		else
			break;	
	}
	for(i = 1; i < 5; i ++){
		if(row + i < GRID_NUM && field[row+i][col] == currentPlayer){
			count ++;
			tail.row = row + i;
			tail.col = col;
		}
		else
			break;
	}
	if(count >= 5)	
	{
		drawLink(head, tail);
		return true;
	}
	
	count = 1;
	for(i = 1; i < 5; i ++){
		if(col - i >= 0 && field[row][col-i] == currentPlayer){
			count ++;
			head.row = row;
			head.col = col - i;			
		}
		else
			break;	
	}
	for(i = 1; i < 5; i ++){
		if(col + i < GRID_NUM && field[row][col+i] == currentPlayer){
			count ++;
			tail.row = row;
			tail.col = col + i;			
		}
		else
			break;
	}
	if (count >= 5)	
	{
		drawLink(head, tail);
		return true;
	}	
	
	count = 1;
	for(i = 1; i < 5; i ++)
		if(row - i >= 0 && col - i >= 0 && field[row-i][col-i] == currentPlayer){
			count ++;
			head.row = row - i;
			head.col = col - i;			
		}
		else
			break;	
	for(i = 1; i < 5; i ++)
		if(row + i < GRID_NUM && col + i < GRID_NUM && field[row+i][col+i] == currentPlayer){
			count ++;
			tail.row = row + i;
			tail.col = col + i;			
		}
		else
			break;
	if (count >= 5)	
	{
		drawLink(head, tail);
		return true;
	}
	
	count = 1;
	for(i = 1; i < 5; i ++)
		if(row - i >= 0 && col + i < GRID_NUM && field[row-i][col+i] == currentPlayer){
			count ++;
			head.row = row - i;
			head.col = col + i;	
		}
		else
			break;	
	for(i = 1; i < 5; i ++)
		if(row + i < GRID_NUM && col - i >= 0 && field[row+i][col-i] == currentPlayer){
			count ++;
			tail.row = row + i;
			tail.col = col - i;
		}
		else
			break;
	if (count >= 5)	
	{
		drawLink(head, tail);
		return true;
	}	
		
	return false;		
}

//ø�s�s�u 
void drawLink(Location src, Location dest){
	int srcX = 0, srcY = 0, destX = 0, destY = 0;
	int squareHeight = SCREEN_HEIGHT / GRID_NUM;
   	int squareWidth = SCREEN_WIDTH / GRID_NUM;
   	srcX = LEFT_MARGINE + src.col * squareWidth + squareWidth / 2;
   	srcY = TOP_MARGINE + src.row * squareHeight + squareHeight / 2;
	destX = LEFT_MARGINE + dest.col * squareWidth + squareWidth / 2;
   	destY = TOP_MARGINE + dest.row * squareHeight + squareHeight / 2;
   	setlinestyle(SOLID_LINE, 0, 2); 
   	setcolor(YELLOW);
   	line(srcX, srcY, destX, destY);
}

//�C�������T��
void showGameWinMsg(){
   	//cleardevice(); //�M�z�Ҧ��ù���ơA�p�G�Ʊ�u��ܰT���ɡA�������� 
	int i = 0;
	char msg1[20] = "";
	if(currentPlayer == PLAY_ONE){
		strcat(msg1, "Player One Win!!");
	}else{
		strcat(msg1, "Player Two Win!!");
	}
			
	char msg2[40] = "press [q] to quit or [s] to restart!!";
   	for(; ; i++){
	   	setcolor(i%14);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 5);
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);
	   	
	   	setcolor(WHITE);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 2);
	   	outtextxy(20, SCREEN_HEIGHT / 2 + 50, msg2);
	   	
	   	delay(200);
	   	
	   	setcolor(BLACK);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 5);
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);	   
		   	
	   	if(kbhit()) {
			key = getch();	
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S') {
				return; 				
			}
		}
	}	
}

//��ܹC��������T
void showInfo(){
	totalTime += speed;
	char timeMsg[45] = " Time:";
	char modeMsg[20] = "";
	char optMsg1[50] = "press [q] to quit, [s] to restart or";
	char optMsg2[50] = "press [a] to change mode.";
	
	char time[10];
	
	sprintf(time, "%5d", totalTime/1000);
	strcat(timeMsg, time);
	strcat(timeMsg, " sec.");
	
	setcolor(WHITE); //�]�w��r�C�� 
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1); //�]�w�r���A�����Ϋ����M�r���j�p 
   	outtextxy(0, 0, timeMsg); //�̾ڧ��п�X��r��ù�
   	
   	if(currentPlayer == PLAY_ONE){
   		outtextxy(30, 20, "PLAYER ONE ROUND");
		setfillstyle(SOLID_FILL,PLAYONE_COLOR);  	  
	}else{
		outtextxy(30, 20, "PLAYER TWO ROUND"); 
		setfillstyle(SOLID_FILL,PLAYTWO_COLOR);  
	}
   	bar(5 , 20, 25, 40);		
	   		
   	if(IFPlayAI){
   		strcat(modeMsg, "AI Mode    ");  		
	}else{
   		strcat(modeMsg, "Player Mode");  		
	}
			
   	setcolor(LIGHTMAGENTA);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(SCREEN_HEIGHT - LEFT_MARGINE * 2, 0, modeMsg);
   	
   	setcolor(LIGHTGREEN);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, TOP_MARGINE + (GRID_NUM + 1) * SCREEN_HEIGHT / GRID_NUM, optMsg1);
	outtextxy(0, TOP_MARGINE + (GRID_NUM + 1) * SCREEN_HEIGHT / GRID_NUM + 20, optMsg2);	      	
}

//�q�����a�@ 
Location* PLAYONE_AI(int field[][GRID_NUM],  Location *focusPtr){
	int row;
	int col;
	cleanFocus(field, *focusPtr);
	srand((unsigned)time(NULL));
	if(first){
		row= 10;  
		col= 10; 
		judge(-1,row,col);
		first=false;
	} 
	else{
	 	int max=0,a,b;
    	for(int i=0;i<20;i++)
			for(int j=0;j<20;j++)
				if(weightboard[i][j]>max){
					if(G[i][j]!=-1&&G[i][j]!=-2){
						a=i;
						b=j;
						max=weightboard[i][j];
					}
				}
		judge(-1,a,b);
		row=a;
		col=b;
	}
	focusPtr->row = row;
	focusPtr->col = col;
	return focusPtr;
}
