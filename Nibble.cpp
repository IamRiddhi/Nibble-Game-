




/* 
 * File:   Nibble Game.cpp
 * Author: Riddhi Dutta
 *
 * Created on 12 June, 2017, 11:19 AM
 */







#include <cstdlib>
#include<iostream>
using namespace std;




#include<stdio.h>
#include <conio.h>
#include<time.h>
#include <string.h>
#include <ctype.h>
#include<windows.h>	
#include<mmsystem.h>
#define startx 25
#define starty 5
#define lengthGameSpace 120
#define breadthGameSpace 50
#define backgroundGameColor 1
#define ascii 254
typedef struct
{
    char *name ;
    char *fileName ;
} Sound ;
Sound sounds[] =    {
                       {"Introduction", "Pacman_Introduction.wav"},
                       {"Munch Food", "Pacman_Munch.wav"},
                       {"Eat Ghost", "Pacman_EatGhost.wav"},
                       {"Eat Fruit", "Pacman_EatFruit.wav"},
                       {"Extra Man", "Pacman_ExtraMan.wav"},
                       {"Intermission", "Pacman_Intermission.wav"},
                       {"Dies", "Pacman_Death.wav"},
                    } ;


void clrscr(void)
{
   system("cls") ;	//clear screen.
}
int wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
	return csbi.dwCursorPosition.X;
}
int wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
	return csbi.dwCursorPosition.Y;
}
void gotoxy(short x, short y) 
{
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);		
}
void SetConsoleColour(WORD* Attributes, DWORD Colour)
{
    CONSOLE_SCREEN_BUFFER_INFO Info;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdout, &Info);
    *Attributes = Info.wAttributes;
    SetConsoleTextAttribute(hStdout, Colour);
}

void ResetConsoleColour(WORD Attributes)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
WORD Attributes = 0;
#define ChangeColor(f,b) SetConsoleColour(&Attributes,(f) | ((b)<<4))


int getKey(void)
{
    int ch = getch();
    if (ch == 0 || ch == 224)
        ch = 500 + getch();
    return ch;
}
int speed=100;
int level=1;
int keyHitChar=-32767;
int checkIncrease=0; 
class Food;
class Obstacle;

class Snake
{
    private: int length;
             typedef struct array
             {
                   
                           int xcord;
                           int ycord;
                           int ch;
                           int dir;//-1 for left +1 for right -2 for down +2 for up.
             }array;
             array ar[1000];
      
     friend int collision_Food_Eaten(Snake &s, Food &f);
     friend int collision_Food_SnakeBody(Food &f, Snake &s);
     friend int collision_Snake_Obstacle(Snake &s, Obstacle & o);
     friend int collision_Snake_Border(Snake &s);
     
    public:  Snake(int l)
             {
                length=l;
                for(int i=0;i<length;i++)
                {
                    
                    ar[i].ch==ascii;
                    ar[i].xcord=startx+10+i;
                    ar[i].ycord=starty+10;
                    ar[i].dir=-1;//left
                
                }
             }
            void increaseSnakeLength()
             {
                //length+=1;
                //checkIncrease=1;
                ar[length]=ar[length-1];
                length++;
                
//                if(length % 5 == 0)
//                    speed-= 5; // decreasing delay by 10milliseconds i.e increasing speed. 
               
             }
            int collision_Snakes()
            {
                for(int i=1;i<length;i++)
                    if(ar[0].xcord==ar[i].xcord && ar[0].ycord==ar[i].ycord)
                    {
                        return 1;
                    }
                return 0;
            }
            void dyingBody()
            {
                int segments,k=0,c=0;
                if(length % 5 == 0)
                    segments =5;
                else if(length % 3 ==0 )
                    segments =3;
                else 
                    segments=2;
                while(c<segments)
                {
                    for(int i=k;i<length;i+=segments)
                    {
                        ChangeColor(backgroundGameColor,backgroundGameColor);
                        gotoxy(ar[i].xcord,ar[i].ycord);
                        cout<<" ";
                        Sleep(100);
                        ResetConsoleColour(Attributes);
                    }
                            k++;
                            c++;
                }       
            }
            int getLength()
            {
                return length;
            }
            void moveSnake()
            {
                
                //DELETING LAST COORDINATE
                ChangeColor(backgroundGameColor,backgroundGameColor);
                    gotoxy(ar[length-1].xcord,ar[length-1].ycord);
                    cout<<" ";
                ResetConsoleColour(Attributes);
                
                for(int i=length-1;i>=1;i--)
                {
                    ar[i]=ar[i-1];
                }
                
                if(keyHitChar==572)  //UP
                    ar[0].dir=2;
                else if(keyHitChar==580)  //DOWN
                    ar[0].dir=-2;
                else if(keyHitChar==577)  //RIGHT
                    ar[0].dir=1;
                else if(keyHitChar==575) //LEFT
                    ar[0].dir=-1;
                
                keyHitChar=-32767; //RESETTING.
                
                if(ar[0].dir==-1) //LEFT
                {
                    ar[0].xcord--;
                }
                else if(ar[0].dir==1) //RIGHT
                {
                    ar[0].xcord++;
                }
                else if(ar[0].dir==2)// UP
                {
                    ar[0].ycord--;
                }
                else if(ar[0].dir==-2) //DOWN.
                {
                    ar[0].ycord++;
                }
                
                //DRAWING
                ChangeColor(backgroundGameColor,14);
                gotoxy(ar[0].xcord,ar[0].ycord);
                    cout<<(char)ar[0].ch;
                ResetConsoleColour(Attributes);
            }
            int  checkDirectionChangePossibilty()
            {
                //NO U TURN POSSIBLE
                if(ar[0].dir==-1 && keyHitChar==577)
                {
                    keyHitChar=-32767; //RESETTING
                    return 0;
                }
                if(ar[0].dir==1 && keyHitChar==575)
                {
                    keyHitChar=-32767;
                    return 0;
                }
                if(ar[0].dir==-2 && keyHitChar==572)
                {
                    keyHitChar=-32767;
                    return 0;
                }
                if(ar[0].dir==2 && keyHitChar==580)
                {
                    keyHitChar=-32767;
                    return 0;
                }
                moveSnake();
                return 1;
            }
            void drawSnake()   //FUNCTION for DRAWING SNAKE  ONLY ONCE AT THE START.
            {
                ChangeColor(backgroundGameColor,14);
                gotoxy(ar[0].xcord,ar[0].ycord);
                for(int i=0;i<length;i++)
                    cout<<(char)ar[i].ch;
                ResetConsoleColour(Attributes);
            }
};

class Food
{
    private: int color;
             int xcord;
             int ycord;
    friend int collision_Food_Eaten(Snake &s, Food &f);
    friend int collision_Food_Obstacle(Food &f, Obstacle &o);
    friend int collision_Food_SnakeBody(Food &f, Snake &s);
    
    public:  Food()
             {
                Set();
             }
             void Set()
             {
                   color=(rand() % 14);
                   while(color==backgroundGameColor || color ==4 )
                       color=(rand() % 14);
                   xcord=(rand() % (lengthGameSpace-2) ) + (startx+1) ;
                   ycord=(rand() % (breadthGameSpace-2) ) + (starty+1) ;
               
             }
              void draw()
              {
                  gotoxy(xcord,ycord);
                  ChangeColor(color,color);
                  cout<<(char)178;
                  ResetConsoleColour(Attributes);
              }
              void setVisibiltyOff()
              {
                  gotoxy(xcord,ycord);
                  ChangeColor(backgroundGameColor,backgroundGameColor);
                  cout<<" ";
                  ResetConsoleColour(Attributes);
                  Set();
              }
};

class Obstacle
{
    private:   typedef struct obstacle_array
                  {
                        int xcord;
                        int ycord;
                        int ch;
                        int color;
                  }obstacle_array;
               obstacle_array oa[1000];
               int length;
               
    friend int collision_Food_Obstacle(Food &f, Obstacle & o);
    friend int collision_Snake_Obstacle(Snake &s, Obstacle & o);
    
    public:    Obstacle()
                {
                     length= (lengthGameSpace)/(1.2);
                    oa[0].xcord= (startx+1) + ( (lengthGameSpace+startx-2) - length - (startx+1) )/2 ;
                    oa[0].ycord= ( (breadthGameSpace+starty-2)- (starty+1) -1 )/2 + (starty+1) ;  
                    oa[0].ch= 223;
                    oa[0].color=4;
                    for(int i=1; i<length ; i++)
                    {
                        oa[i].xcord= oa[i-1].xcord + 1;
                        oa[i].ycord= oa[0].ycord;
                        oa[i].ch= oa[0].ch;
                        oa[i].color = oa[0].color;
                    }    
                        
                }
                void drawObstacle()
                {
                    
                        gotoxy(oa[0].xcord, oa[0].ycord);
                        ChangeColor(oa[0].color,oa[0].color);
                        for(int i=0; i<length ; i++)
                            cout<<(char)oa[i].ch;
                        ResetConsoleColour(Attributes);
                }
                int drawObstacle1(int xstart)
                {
                    //setting up coordinates
                    length= (breadthGameSpace)/(1.5);
                    oa[0].xcord= xstart + ( lengthGameSpace-4 )/4 ;
                    oa[0].ycord= ( (breadthGameSpace+starty-1)- length -starty )/2 + (starty+1) ;  
                    oa[0].ch= 223;
                    oa[0].color=4;
                    for(int i=1; i<length ; i++)
                    {
                        oa[i].xcord= oa[0].xcord;
                        oa[i].ycord= oa[i-1].ycord+1;
                        oa[i].ch= oa[0].ch;
                        oa[i].color = oa[0].color;
                    }    
                    
                    //drawing
                        
                        ChangeColor(oa[0].color,oa[0].color);
                        for(int i=0; i<length ; i++)
                        {
                            gotoxy(oa[i].xcord, oa[i].ycord);
                            cout<<(char)oa[i].ch;
                        }
                        ResetConsoleColour(Attributes);
                        
                        //return
                        return oa[0].xcord;
                }
                int drawObstacle2(int ystart)
                {
                    //setting up coordinates
                    length= (breadthGameSpace)/(2);
                    oa[0].xcord= (startx+1) + ( (lengthGameSpace+startx-2) - length - (startx+1) )/2 ;
                    oa[0].ycord=  (breadthGameSpace-2)/3 + ystart ;  
                    oa[0].ch= 223;
                    oa[0].color=4;
                    for(int i=1; i<length ; i++)
                    {
                        oa[i].xcord= oa[i-1].xcord + 1;
                        oa[i].ycord= oa[0].ycord;
                        oa[i].ch= oa[0].ch;
                        oa[i].color = oa[0].color;
                    }    
                    
                    //drawing
                        
                        ChangeColor(oa[0].color,oa[0].color);
                        for(int i=0; i<length ; i++)
                        {
                            gotoxy(oa[i].xcord, oa[i].ycord);
                            cout<<(char)oa[i].ch;
                        }
                        ResetConsoleColour(Attributes);
                        
                        //return
                        return oa[0].ycord;
                }
                
                int drawObstacle3(int xstart)
                {
                    //setting up coordinates
                    length= (breadthGameSpace)/(4);
                    oa[0].xcord= xstart + ( lengthGameSpace-4 )/4 +10;
                    oa[0].ycord= ( (breadthGameSpace+starty-1)- length -starty )/2 + (starty+1) ;  
                    oa[0].ch= 223;
                    oa[0].color=4;
                    for(int i=1; i<length ; i++)
                    {
                        oa[i].xcord= oa[0].xcord;
                        oa[i].ycord= oa[i-1].ycord+1;
                        oa[i].ch= oa[0].ch;
                        oa[i].color = oa[0].color;
                    }    
                    
                    //drawing
                        
                        ChangeColor(oa[0].color,oa[0].color);
                        for(int i=0; i<length ; i++)
                        {
                            gotoxy(oa[i].xcord, oa[i].ycord);
                            cout<<(char)oa[i].ch;
                        }
                        ResetConsoleColour(Attributes);
                        
                        //return
                        return oa[0].xcord;
                }
};
int drawBox(int x, int y)
{
    gotoxy(x,y); //(x,y) are coordinates of top left corner of game Space
    
    ChangeColor(0,0);
    
    for(int i=1;i<=lengthGameSpace;i++) //drawing upper length of the rectangular box of size 120.
        cout<<(char)178;
    
    x=wherex()-1;
    y=wherey();
    
    for(int i=1;i<=breadthGameSpace;i++)  // drawing right breadth of recatngular box of size 50.
    {
        gotoxy(x,y+i);
        cout<<(char)178;
    }
    
    y=wherey();
    x=wherex()-1;
    
    for(int i=1;i<=lengthGameSpace;i++)  // drawing lower length of recatngular box of size 120.
    {
        gotoxy(x-i,y);
        cout<<(char)178;
    }
    
    x=wherex()-1;
    y=wherey();
    
    for(int i=1;i<=breadthGameSpace;i++)  // drawing left breadth of recatngular box of size 50.
    {
        gotoxy(x,y-i);
        cout<<(char)178;
    }
    
    ResetConsoleColour(Attributes);
    return wherey();  //returns y coordinate.
}

void drawBackground()
{
    ChangeColor(backgroundGameColor,backgroundGameColor);
    int m,n;
    m=(starty+breadthGameSpace)-1;
    n=(startx+lengthGameSpace)-2;
    for(int i=starty+1;i<=m;i++)
    {
        for(int j=startx;j<=n;j++)
        {
            gotoxy(j,i);
            cout<<" ";
        }
    }
    ResetConsoleColour(Attributes);
}
int collision_Food_Eaten(Snake &s, Food &f)
{
    if(s.ar[0].xcord==f.xcord && s.ar[0].ycord==f.ycord)
        return 1;
    else 
        return 0;
}
int collision_Food_Obstacle(Food &f, Obstacle & o)
{
    for(int i=0;i<o.length;i++)
        if(o.oa[i].xcord==f.xcord && o.oa[i].ycord==f.ycord)
            return 1;
    return 0;
}
int collision_Food_SnakeBody(Food &f, Snake &s)
{
    for(int i=0;i<s.length;i++)
        if(s.ar[i].xcord==f.xcord && s.ar[i].ycord==f.ycord)
            return 1;
    return 0;
}

int collision_Snake_Obstacle(Snake &s, Obstacle & o)
{
    for(int i=0;i<o.length;i++)
        if(o.oa[i].xcord==s.ar[0].xcord && o.oa[i].ycord==s.ar[0].ycord)
        {
            
           // o.drawObstacle();
            return 1;
        }
    return 0;
}
int collision_Snake_Border(Snake &s)
{
    if(s.ar[0].ycord==starty || s.ar[0].ycord== (starty + breadthGameSpace-1) || s.ar[0].xcord==startx || s.ar[0].xcord== (startx + lengthGameSpace-1))
    {
       // drawBox(startx,starty);
        return 1;
    }
    return 0;
}


void updateScore(int score)
{
    ChangeColor(4,7);
    gotoxy(30,4);
    cout<<"SCORE:-"<<score;
    ResetConsoleColour(Attributes);
}
void updateLife(int life)
{
    ChangeColor(4,7);
    gotoxy(50,4);
    cout<<"LIVES REMAINING:-"<<life;
    ResetConsoleColour(Attributes);
}
void updateLevel()
{
    ChangeColor(4,7);
    gotoxy(90,4);
    cout<<"LEVEL:-"<<level;
    ResetConsoleColour(Attributes);
}
void Level3(int score,int life,int length,Food &f)
{
    clrscr();
    speed-=25;
    int y,ystart,xstart;
    Obstacle o1,o2,o3,o4;
    //LIVES
    for(int k=0;k<life;k++)
    {
        Snake s(length);
        xstart=startx;
        ystart=starty;
        updateLife(life-k);
        updateLevel();
        drawBackground();
        y=drawBox(startx,starty); //Drawing the Game Environment or Game Space.
        
        ystart=o1.drawObstacle2(ystart);
        ystart=o2.drawObstacle2(ystart);
        ystart=starty;
        
        xstart=o3.drawObstacle3(xstart);
        xstart=o4.drawObstacle3(xstart);
        xstart=startx;
        
        s.drawSnake();

        while(collision_Food_SnakeBody(f,s) || collision_Food_Obstacle(f,o1) || collision_Food_Obstacle(f,o2) || collision_Food_Obstacle(f,o3) || collision_Food_Obstacle(f,o4))
            f.Set();
        f.draw();
          updateScore(score);
        while(1)
        {
            Sleep(speed);
            if(kbhit())
            {
                keyHitChar=getKey();
                if(s.checkDirectionChangePossibilty()==0)
                    s.moveSnake();
            }
            else
            {
                s.moveSnake();
            }
            if(collision_Food_Eaten(s,f))
            {
                
                f.setVisibiltyOff();
                while(collision_Food_SnakeBody(f,s) || collision_Food_Obstacle(f,o1) || collision_Food_Obstacle(f,o2) || collision_Food_Obstacle(f,o3) || collision_Food_Obstacle(f,o4))
                    f.Set();
                f.draw();
                score++;
                s.increaseSnakeLength();
               updateScore(score);
               PlaySound(TEXT(sounds[2].fileName), NULL, SND_ASYNC) ;
            }
           
            
            else if(s.collision_Snakes() || collision_Snake_Obstacle(s,o1) || collision_Snake_Obstacle(s,o2) || collision_Snake_Obstacle(s,o3) || collision_Snake_Obstacle(s,o4) ||collision_Snake_Border(s))
            {
                ystart=o1.drawObstacle2(ystart);
                ystart=o2.drawObstacle2(ystart);
                ystart=starty;

                xstart=o3.drawObstacle3(xstart);
                xstart=o4.drawObstacle3(xstart);
                xstart=startx;
                break;
            }
        }  
          /*End:*/ Sleep(50);
            PlaySound(TEXT(sounds[6].fileName), NULL, SND_ASYNC) ;
            s.dyingBody();
            ystart=o1.drawObstacle2(ystart);
            ystart=o2.drawObstacle2(ystart);
            ystart=starty;

            xstart=o3.drawObstacle3(xstart);
            xstart=o4.drawObstacle3(xstart);
            xstart=startx;
          
        length=s.getLength();
     } 
    drawBox(startx,starty);
    updateLife(0);
}

void Level2(int score , int life ,int length,Food &f)
{
    clrscr();
    speed-=25;
    int y,xstart;
    Obstacle o1,o2,o3;
    //LIVES
    for(int k=0;k<life;k++)
    {
        Snake s(length);
        xstart=startx;
        updateLife(life-k);
        updateLevel();
        drawBackground();
        y=drawBox(startx,starty); //Drawing the Game Environment or Game Space.
        
        xstart=o1.drawObstacle1(xstart);
        xstart=o2.drawObstacle1(xstart);
        xstart=o3.drawObstacle1(xstart);
        xstart=startx;
        
        s.drawSnake();

        while(collision_Food_SnakeBody(f,s) || collision_Food_Obstacle(f,o1) || collision_Food_Obstacle(f,o2) || collision_Food_Obstacle(f,o3))
            f.Set();
        f.draw();
          updateScore(score);
        while(1)
        {
            Sleep(speed);
            if(kbhit())
            {
                keyHitChar=getKey();
                if(s.checkDirectionChangePossibilty()==0)
                    s.moveSnake();
            }
            else
            {
                s.moveSnake();
            }
            if(collision_Food_Eaten(s,f))
            {
                
                f.setVisibiltyOff();
                while(collision_Food_SnakeBody(f,s) || collision_Food_Obstacle(f,o1) || collision_Food_Obstacle(f,o2) || collision_Food_Obstacle(f,o3))
                    f.Set();
                f.draw();
                score++;
                s.increaseSnakeLength();
               updateScore(score);
               PlaySound(TEXT(sounds[2].fileName), NULL, SND_ASYNC) ;
            }
           
            if(score==4)
            {
                level++;
                Level3(score,life-k,s.getLength(),f);
                return;
            }
            else if(s.collision_Snakes() || collision_Snake_Obstacle(s,o1) || collision_Snake_Obstacle(s,o2) || collision_Snake_Obstacle(s,o3) || collision_Snake_Border(s))
            {
                xstart=o1.drawObstacle1(xstart);
                xstart=o2.drawObstacle1(xstart);
                xstart=o3.drawObstacle1(xstart);
                xstart=startx;
                break;
            }
        }  
          /*End:*/ Sleep(50);
                   PlaySound(TEXT(sounds[6].fileName), NULL, SND_ASYNC) ;
          s.dyingBody();
          xstart=o1.drawObstacle1(xstart);
          xstart=o2.drawObstacle1(xstart);
          xstart=o3.drawObstacle1(xstart);
          xstart=startx;
          
        length=s.getLength();
     } 
    drawBox(startx,starty);
    updateLife(0);
}


void playGame_Level1()// Game Controller Function.
{
    
    int y;
    int score=0;  //No of FOODS DEVOURED.
    int life=3;
   // int level=1;
    int length=2;
    Food f;
    Obstacle o;
    //LIVES
    for(int k=0;k<life;k++)
    {
        Snake s(length);
        updateLife(life-k);
        updateLevel();
        drawBackground();
        y=drawBox(startx,starty); //Drawing the Game Environment or Game Space.
        
        o.drawObstacle();
        s.drawSnake();

        while(collision_Food_SnakeBody(f,s) || collision_Food_Obstacle(f,o))
            f.Set();
        f.draw();
          updateScore(score);
        while(1)
        {
            Sleep(speed);
            if(kbhit())
            {
                keyHitChar=getKey();
                if(s.checkDirectionChangePossibilty()==0)
                    s.moveSnake();
            }
            else
            {
                s.moveSnake();
            }
            if(collision_Food_Eaten(s,f))
            {
                
                f.setVisibiltyOff();
                while(collision_Food_SnakeBody(f,s) || collision_Food_Obstacle(f,o))
                    f.Set();
                f.draw();
                score++;
                s.increaseSnakeLength();
               updateScore(score);
               PlaySound(TEXT(sounds[2].fileName), NULL, SND_ASYNC) ;
            }
            //if(score !=0 && score % 5 ==0)
            if(score==2)
            {
                level++;
                Level2(score,life-k,s.getLength(),f);
                
                return;
            }
            else if(s.collision_Snakes() || collision_Snake_Obstacle(s,o) || collision_Snake_Border(s))
            {
                o.drawObstacle();
                break;
            }
        }
          
            Sleep(50);
          s.dyingBody();
          PlaySound(TEXT(sounds[6].fileName), NULL, SND_ASYNC) ;
          o.drawObstacle();
          
        length=s.getLength();  
       
    } 
    drawBox(startx,starty);
    updateLife(0);
}   
int main(int argc, char** argv)
{
    time_t t;
    srand(time(&t));
    gotoxy(0,0);
    ShowConsoleCursor(false);
    
    gotoxy(20,25);
    ChangeColor(4,7);
    cout<<"WELCOME TO NIBBLE GAME, AN OLD GAME CREATED BY MICROSOFT, DOS VERSION 1990,A HUMBLE TRY TO RECREATE IT.";
    ResetConsoleColour(Attributes);
    PlaySound(TEXT(sounds[0].fileName), NULL, SND_SYNC) ;
    clrscr();
    
    playGame_Level1();
    
    gotoxy(60,25);
    ChangeColor(backgroundGameColor,2);
    PlaySound(TEXT(sounds[4].fileName), NULL, SND_SYNC) ;
    cout<<"THANK YOU FOR PLAYING!    DEVELOPED BY RIDDHI DUTTA";
    ResetConsoleColour(Attributes);
    PlaySound(TEXT(sounds[5].fileName), NULL, SND_SYNC) ;
    //getch();
    clrscr();
    return 0;
}

