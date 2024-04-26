/*
10 - project - video game
Author: Taya
Date: Feb 29, 2024
Create a video game from the following list (marks will be given based on difficulty level of coding)
*/
#include <iostream> 	// include library for input and output
#include <math.h>		// include library for math functions
#include <windows.h>
#include <cctype>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <thread>
#include <algorithm>
#include <fstream>		// include library for reading and writing files
using namespace std;


struct name_time		//Define a structure for the combining the name and score for the leaderboard
{
	int time;			//Declares an int for the users score
	string name;		//Declares a string for the users name on the leaderboard
};

vector<vector <char>> temp_vis;

vector <name_time> easy(1);		//Declares a global vector of the structure type for holding a leaderboard score
vector <name_time> medium(1);		//Declares a global vector of the structure type for holding a leaderboard score
vector <name_time> hard(1);		//Declares a global vector of the structure type for holding a leaderboard score
int depth=0;
int seconds;
bool stop=false;
bool first=true;

bool operator<(const name_time&lhs, const name_time&rhs);			//Declares a bool function with two parameters that helps sorting the leaderboard values

void minesweeper();
void open_spots(char fake_vis[][50], int fake_board[][50], int rows, int cols, int x, int y);
void timer(int cols);
void mine_counter(int mines);
void create_minesweeper(char fake_vis[][50], int fake_board[][50], int rows, int cols, int mines);
string unicode_num(int num);
string ascii_num(int num, int row);
void show_score_minesweeper();			//Declares a void function with no parameters for showing the leaderboard
void add_score_minesweeper(int time, int difficulty);		//Declares a void functions with an int parameter for adding a score to the leaderboard
VOID ErrorExit(LPCSTR);
char get_char( short x, short y );
void Center_text(string text);

HANDLE hStdin;
DWORD fdwSaveOldMode;
//Event type
#define event_type irInBuf[i].EventType
//Keyboard
#define ker irInBuf[i].Event.KeyEvent
#define key ker.wVirtualKeyCode
#define key_pressed ker.bKeyDown
//Mouse
#define mer irInBuf[i].Event.MouseEvent
#define mouse_event mer.dwEventFlags
#define mouse_state mer.dwButtonState
#define left_click FROM_LEFT_1ST_BUTTON_PRESSED
#define middle_click FROM_LEFT_3RD_BUTTON_PRESSED
#define right_click RIGHTMOST_BUTTON_PRESSED
#define MouseX mer.dwMousePosition.X
#define MouseY mer.dwMousePosition.Y

//Next Jobs, F to fix screen, L to load game


int main(){
	minesweeper();
	return 0;
}

void minesweeper(){    
	SetConsoleOutputCP(CP_UTF8);
	HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
	srand(time(0));
	int rows,cols,mines,flags;
	string bomb="\U0001F4A3";
	string explode="\U0001F4A5";
	string flag="\U0001F6A9";
	string blank="\U0001F532";
	string emoji="\U0001F642";
    int menu_choice=0;
    int difficulty=0;
    int end_choice;
    bool restart;
    int lose;
    int colored=0;
    int prev_state;
    int prev_event;
    int colorX,colorY;
    int firstX,firstY;
    int temp_mines;
    string password;
    
	DWORD cNumRead, fdwMode,i;
    INPUT_RECORD irInBuf[128];
    
    fstream highscore;			//Create a file object for reading/writing
    highscore.open("highscores.csv");		// attempt to open the highscore file
    if (!highscore){		//If the file does not open
        highscore.open("highscores.csv", ios::out);		//Create and open a highscore file for writing
        highscore<<"Highscores"<<endl;		//Write to the highscore file
    }	
    highscore.close();		//close the highscore file
	

    hStdin = GetStdHandle(STD_INPUT_HANDLE);		// Get the standard input handle. 
    if (hStdin == INVALID_HANDLE_VALUE){
		ErrorExit("GetStdHandle");
    }
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode)){		// Save the current input mode, to be restored on exit. 
    	ErrorExit("GetConsoleMode");
    }
	do{
	    fdwMode = ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS;		// Enable the window and mouse input events. 
	    if (!SetConsoleMode(hStdin, fdwMode)){
			ErrorExit("SetConsoleMode");
		}
		menu_choice=0;
		seconds=0;
		end_choice=1;
		lose=1;
		difficulty=0;
		restart=true;
		firstX=-1;
		firstY=-1;
//		stop=false;
	    Center_text("    __  ________   ______________       __________________  __________ ");
		cout<<"\n";
		Center_text("   /  |/  /  _/ | / / ____/ ___/ |     / / ____/ ____/ __ \\/ ____/ __ \\");
		cout<<"\n";
		Center_text("  / /|_/ // //  |/ / __/  \\__ \\| | /| / / __/ / __/ / /_/ / __/ / /_/ /");
		cout<<"\n";
		Center_text(" / /  / // // /|  / /___ ___/ /| |/ |/ / /___/ /___/ ____/ /___/ _, _/ ");
		cout<<"\n";
		Center_text("/_/  /_/___/_/ |_/_____//____/ |__/|__/_____/_____/_/   /_____/_/ |_|  ");
		cout<<"\n\n\n\n";
		
		
		Center_text("   ___  __                __ __    __          ____       _ __ ");			//For all 7<y<13
		cout<<"\n";
		Center_text("  / _ \\/ /__ ___ __      / // /__ / /__       / __ \\__ __(_) /_");			//Play is 26<x<45
		cout<<"\n";
		Center_text(" / ___/ / _ `/ // /     / _  / -_) / _ \\     / /_/ / // / / __/");			//Help is 50<x<68
		cout<<"\n";
		Center_text("/_/  /_/\\_,_/\\_, /     /_//_/\\__/_/ .__/     \\___\\_\\_,_/_/\\__/ ");	//Quit is 73<x<91
		cout<<"\n";
		Center_text("            /___/                /_/                           ");
		cout<<"\n\n\n\n";
		
		
		Center_text("   __               __        __                    __");		//16<y<21
		cout<<"\n";
		Center_text("  / /  ___ ___ ____/ /__ ____/ /  ___  ___ ________/ /");		//32<x<87
		cout<<"\n";
		Center_text(" / /__/ -_) _ `/ _  / -_) __/ _ \\/ _ \\/ _ `/ __/ _  / ");
		cout<<"\n";
		Center_text("/____/\\__/\\_,_/\\_,_/\\__/_/ /_.__/\\___/\\_,_/_/  \\_,_/  ");
		cout<<"\n\n\n\e[1m\e[6m";
		Center_text("Click to select");
		cout<<"\n\n\e[0m";
		
		cout<<"\e[?25l";
		
	    // Loop to read and handle the next 500 input events. 	
		cout<<flush;
		while (menu_choice==0){
	        if (!ReadConsoleInput(hStdin,irInBuf,128,&cNumRead)){
	            ErrorExit("ReadConsoleInput");
	        }
	        for (i = 0; i < cNumRead; i++){
				if (event_type==MOUSE_EVENT){ // mouse input 
					if (mouse_event==0 and mouse_state == left_click and MouseY>7 and MouseY<13){
						if (MouseX>31 and MouseX<51){
				            menu_choice=1;
						}
						else if (MouseX>54 and MouseX<72){		
				            menu_choice=2;
						}
						else if (MouseX>76 and MouseX<95){		
				            menu_choice=3;
						}
			        }
					if (mouse_event==0 and mouse_state == left_click and MouseX>36 and MouseX<91 and MouseY>15 and MouseY<20){
						menu_choice=4;
					}
			        else if (mouse_event==1){
						if (MouseX>31 and MouseX<51 and MouseY>7 and MouseY<13){
							if (colored!=1){
								cout<<"\e[s\e[?25l\e[9;52H";
								cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
				        		cout<<"\e[30;103m   ___  __         ";			//For all 7<y<13
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"  / _ \\/ /__ ___ __";			//Play is 26<x<45
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<" / ___/ / _ `/ // /";			//Help is 50<x<68
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"/_/  /_/\\_,_/\\_, / ";	//Quit is 73<x<91
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"            /___/  \e[0;0m";
								cout<<"\e[u";
								colored=1;
							}
						}
						else if (MouseX>54 and MouseX<72 and MouseY>7 and MouseY<13){	
							if (colored!=2){
								cout<<"\e[s\e[?25l\e[9;73H";
								cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
				        		cout<<"\e[30;103m   __ __    __   ";			//For all 7<y<13
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"  / // /__ / /__ ";			//Play is 26<x<45
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<" / _  / -_) / _ \\";			//Help is 50<x<68
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"/_//_/\\__/_/ .__/";	//Quit is 73<x<91
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"          /_/    \e[0;0m";
								cout<<"\e[u";
								colored=2;
							}
						}
						else if (MouseX>76 and MouseX<95 and MouseY>7 and MouseY<13){		
							if (colored!=3){
								cout<<"\e[s\e[9;96H";
								cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
				        		cout<<"\e[30;103m  ____       _ __ ";			//For all 7<y<13
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<" / __ \\__ __(_) /_";			//Play is 26<x<45
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"/ /_/ / // / / __/";			//Help is 50<x<68
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"\\___\\_\\_,_/_/\\__/ ";	//Quit is 73<x<91
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"                  \e[0;0m";
								cout<<"\e[u";
								colored=3;
							}
						}
						else if (MouseX>36 and MouseX<91 and MouseY>15 and MouseY<20){		
							if (colored!=4){
								cout<<"\e[s\e[17;92H";
								cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
				        		cout<<"\e[30;103m   __               __        __                    __";			//For all 7<y<13
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"  / /  ___ ___ ____/ /__ ____/ /  ___  ___ ________/ /";			//Help is 50<x<68
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<" / /__/ -_) _ `/ _  / -_) __/ _ \\/ _ \\/ _ `/ __/ _  / ";	//Quit is 73<x<91
								
								cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
								cout<<"/____/\\__/\\_,_/\\_,_/\\__/_/ /_.__/\\___/\\_,_/_/  \\_,_/  \e[0;0m";
								cout<<"\e[u";
								colored=4;
							}
						}	
						else if (colored==1){
							cout<<"\e[s\e[9;1H\e[9;52H";
							cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			        		cout<<"   ___  __         ";			//For all 7<y<13
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"  / _ \\/ /__ ___ __";			//Play is 26<x<45
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<" / ___/ / _ `/ // /";			//Help is 50<x<68
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"/_/  /_/\\_,_/\\_, / ";	//Quit is 73<x<91
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"            /___/  ";
							cout<<"\e[u";
							colored=0;
						}
						else if (colored==2){
							cout<<"\e[s\e[9;1H\e[9;73H";
							cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			        		cout<<"   __ __    __   ";			//For all 7<y<13
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"  / // /__ / /__ ";			//Play is 26<x<45
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<" / _  / -_) / _ \\";			//Help is 50<x<68
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"/_//_/\\__/_/ .__/";	//Quit is 73<x<91
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"          /_/    ";
							cout<<"\e[u";
							colored=0;
						}
						else if (colored==3){
							cout<<"\e[s\e[9;1H\e[9;96H";
							cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			        		cout<<"  ____       _ __ ";			//For all 7<y<13
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<" / __ \\__ __(_) /_";			//Play is 26<x<45
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"/ /_/ / // / / __/";			//Help is 50<x<68
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"\\___\\_\\_,_/_/\\__/ ";	//Quit is 73<x<91
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"                  ";
							cout<<"\e[u";
							colored=0;
						}
						else if (colored==4){								
							cout<<"\e[s\e[18;1H\e[17;92H";
							cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
			        		cout<<"   __               __        __                    __";			//For all 7<y<13
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"  / /  ___ ___ ____/ /__ ____/ /  ___  ___ ________/ /";			//Help is 50<x<68
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<" / /__/ -_) _ `/ _  / -_) __/ _ \\/ _ \\/ _ `/ __/ _  / ";	//Quit is 73<x<91
							
							cout<<"\e[1B\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
							cout<<"/____/\\__/\\_,_/\\_,_/\\__/_/ /_.__/\\___/\\_,_/_/  \\_,_/  ";
							cout<<"\e[u";
							colored=0;
						}
	//		        	cout<<"\tX: "<<MouseX<<"\tY: "<<MouseY<<"\tChar: "<<get_char(MouseX,MouseY)<<endl;
					}
	            }
	        }
	    }
	    cout<<flush;
	    system("CLS");
	    
	    fdwMode = ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS;		// Enable the window and mouse input events. 
	    if (!SetConsoleMode(hStdin, fdwMode)){
	    	ErrorExit("SetConsoleMode");
		}
	        
	    if (menu_choice==1){
			Center_text("Difficulty");
			cout<<"\n\n\n";
			Center_text("Easy");
			cout<<"\n\n";
			Center_text("Medium");
			cout<<"\n\n";
			Center_text("Hard");
			cout<<"\n\n";
			Center_text("Custom");\
			cout<<"\n\n\e[1m\e[6m";
			Center_text("Press Escape to Return");
			cout<<"\e[0m";
			while (difficulty==0){
		        if (!ReadConsoleInput(hStdin,irInBuf,128,&cNumRead)){
		            ErrorExit("ReadConsoleInput");
		        }
		        for (i = 0; i < cNumRead; i++){
		        	if (event_type == KEY_EVENT){	 // keyboard input 
	                	if (!key_pressed and key==VK_ESCAPE){
							difficulty=-1;
							break;
						}
					}
					else if (event_type==MOUSE_EVENT){ // mouse input 
						if (mouse_event==0 and mouse_state == left_click){
							if (MouseX>61 and MouseX<66 and MouseY==3){
					            difficulty=1;
							}
							else if (MouseX>60 and MouseX<67 and MouseY==5){		
					            difficulty=2;
							}
							else if (MouseX>61 and MouseX<66 and MouseY==7){		
					            difficulty=3;
							}
							else if (MouseX>60 and MouseX<67 and MouseY==9){		
					            difficulty=4;
							}
				        } 
				        else if (mouse_event==1){
				        	if (MouseX>61 and MouseX<66 and MouseY==3){
				        		if (colored!=1){
					        		cout<<"\e[s\e[?25l\e[4;67H\b\b\b\b";
					        		cout<<"\e[30;103mEasy\e[0;0m\e[u";
									colored=1;
								}
							}
							else if (MouseX>60 and MouseX<67 and MouseY==5){
				        		if (colored!=2){
					        		cout<<"\e[s\e[?25l\e[6;68H\b\b\b\b\b\b";
					        		cout<<"\e[30;103mMedium\e[0;0m\e[u";
									colored=2;
								}
							}
							else if (MouseX>61 and MouseX<66 and MouseY==7){
				        		if (colored!=3){
					        		cout<<"\e[s\e[?25l\e[8;67H\b\b\b\b";
					        		cout<<"\e[30;103mHard\e[0;0m\e[u";
									colored=3;
								}
							}
							else if (MouseX>60 and MouseX<67 and MouseY==9){
				        		if (colored!=4){
					        		cout<<"\e[s\e[?25l\e[10;68H\b\b\b\b\b\b";
					        		cout<<"\e[30;103mCustom\e[0;0m\e[u";
									colored=4;
								}
							}
							else if (colored==1){
								cout<<"\e[s\e[?25l\e[4;67H\b\b\b\bEasy";
								colored=0;
							}
							else if (colored==2){
								cout<<"\e[s\e[?25l\e[6;68H\b\b\b\b\b\bMedium";
								colored=0;							
							}
							else if (colored==3){
								cout<<"\e[s\e[?25l\e[8;67H\b\b\b\bHard";
								colored=0;	
							}
							else if (colored==4){
								cout<<"\e[s\e[?25l\e[10;68H\b\b\b\b\b\bCustom";
								colored=0;	
							}
//				        	cout<<"\tX: "<<MouseX<<"\tY: "<<MouseY<<"\tChar: "<<get_char(MouseX,MouseY)<<endl;
						}
		            }
		        }
		    }
			
		    cout<<flush;
		    system("CLS");
		    
			if (difficulty==-1){
				continue;
			}
		    
		    if (difficulty==1){
		    	rows=9;
		    	cols=9;
		    	mines=10;
			}
			else if (difficulty==2){
		    	rows=16;
		    	cols=16;
		    	mines=40;
			}
			else if (difficulty==3){
		    	rows=16;
		    	cols=30;
		    	mines=99;
			}
			else{
				SetConsoleMode(hStdin, fdwSaveOldMode);
				cout<<"\e[?25h";
		    	cout<<"How many rows do you want: ";
				cin>>rows;
				while (rows<0 or rows>24){
					cout<<"Change the row count! Retry.\n";
			    	cout<<"How many rows do you want: ";
					cin>>rows;
				}
		    	cout<<"\nHow many columns do you want: ";
				cin>>cols;
				while (cols<8 or cols>30){
					cout<<"Change the column count! Retry.\n";
			    	cout<<"How many columns do you want: ";
					cin>>cols;
				}
		    	cout<<"\nHow many mines do you want: ";
				cin>>mines;
				while (mines<0 or mines>=rows*cols){
					cout<<"There must be less mines than squares! Retry.\n";
			    	cout<<"How many mines do you want: ";
					cin>>cols;
				}
				cout<<"\e[?251";
			}
			char visible[rows][cols];		//Y for on, N for off, F for flag (show flag emoji but not number, basically no)
			int board[rows][cols];			//init all to 0, 2 for loops that check all spaces around after first spot is picked, bomb is -1
			char fake_vis[rows][50];
			int fake_board[rows][50];
			while (restart==true){
				cout<<"\e[?251";
				seconds=0;
				lose=1;
				firstX=-1;
				restart=true;
				end_choice=1;
				firstY=-1;
				temp_vis.resize(rows);
				for (int i=0;i<rows;i++){
					temp_vis[i].resize(cols,'N'); 
				}
				fill( &visible[0][0], &visible[0][0] + sizeof(visible), 'N' );
				memset( &board[0][0], 0, sizeof(board) );
				for (int i=0;i<rows;i++){
					for (int j=0;j<cols;j++){
	//					board[i][j]=0;
					}
				}
				copy(&visible[0][0], &visible[0][0]+rows*cols,&fake_vis[0][0]);
				copy(&board[0][0], &board[0][0]+rows*cols,&fake_board[0][0]);
				
			    cout<<flush;
			    system("CLS");
			    colored=0;
				temp_mines=mines;
				flags=mines;
			    
			    create_minesweeper(fake_vis,fake_board,rows,cols,flags);
//				cout<<"\e[1A\e[2K\r";
			    
			    fdwMode = ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS;		// Enable the window and mouse input events. 
			    if (!SetConsoleMode(hStdin, fdwMode)){
			    	ErrorExit("SetConsoleMode");
				}
				
				cout<<"\e[?251";
			    while (lose==1){
			        if (!ReadConsoleInput(hStdin,irInBuf,128,&cNumRead)){
			            ErrorExit("ReadConsoleInput");
			        }
			        for (i = 0; i < cNumRead; i++){
			        	if (event_type == KEY_EVENT){	 // keyboard input 
							if (!key_pressed){
			                	if (key==VK_ESCAPE){
									restart=false;
									end_choice=0;
									lose=0;
									break;
								}
//								else if (key==0x53){
//									fstream save;									//Create a file object for reading/writing
//								    save.open("save_game.txt");					// attempt to open the highscore file
//								    if (!save){										//If the file does not open
//								        save.open("save_game.txt", ios::out);		//Create and open a highscore file for writing
//								    }	
//							        save<<rows<<","<<cols<<","<<temp_mines<<","<<mines<<","<<flags<<","<<seconds<<endl;					//Write to the highscore file
//							        for (int j=0;j<rows;j++){
//							        	for (int k=0;k<cols-1;k++){
//							        		save<<board[j][k]<<",";
//										}
//										save<<board[j][cols-1];
//										save<<endl;
//									}
//							        for (int j=0;j<rows;j++){
//							        	for (int k=0;k<cols-1;k++){
//							        		save<<visible[j][k]<<",";
//										}
//										save<<visible[j][cols-1];
//										save<<endl;
//									}
//								    save.close();		
//								}
								else if (key==0x52){
									temp_vis.resize(rows);
									for (int i=0;i<rows;i++){
										temp_vis[i].resize(cols,'N');
									}
									copy(&visible[0][0], &visible[0][0]+rows*cols,&fake_vis[0][0]);
									copy(&board[0][0], &board[0][0]+rows*cols,&fake_board[0][0]);
									
								    cout<<flush;
								    system("CLS");
								    colored=0;
								    create_minesweeper(fake_vis,fake_board,rows,cols,flags);	 
								    fdwMode = ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS;		// Enable the window and mouse input events. 
								    if (!SetConsoleMode(hStdin, fdwMode)){
								    	ErrorExit("SetConsoleMode");
									}
									
								}
							}
						}
						else if (event_type==MOUSE_EVENT){ // mouse input 
							if (mouse_event==0 and mouse_state==right_click){
								if (MouseX>0 and MouseX<(2*cols)+1 and MouseY>5 and MouseY<rows+6){
					        		MouseX++;
									if (MouseX%2==0){
										MouseX++;
									}
					        		MouseY++;
									cout<<"\e[s";
					        		if (visible[MouseY-7][(MouseX/2)-1]=='N'){
						        		cout<<"\e[s\e[?25l\e["<<MouseY<<";"<<MouseX<<"H\b";
										cout<<"\e[48;2;123;123;123m";
										cout<<"\e[38;2;189;189;189m";
										cout<<flag;
										cout<<"\e[0;0m\e[u";
										visible[MouseY-7][(MouseX/2)-1]='F';
										flags--;
										mine_counter(flags);
										if (board[MouseY-7][(MouseX/2)-1]==-1){
											mines--;
										}
										for (int i=0;i<rows;i++){
											for (int j=0;j<cols;j++){
												if (board[i][j]>=0 and visible[i][j]!='Y'){
													lose=1;
													break;
												}
												if (board[i][j]>=0 and visible[i][j]=='Y'){
													lose=2;
												}
											}
											if (lose==1){
												break;
											}
										}
										if (lose==2){
											break;
										}
									}
									else if (visible[MouseY-7][(MouseX/2)-1]=='F'){
						        		cout<<"\e[s\e[?25l\e["<<MouseY<<";"<<MouseX<<"H\b";
										cout<<"\e[48;2;123;123;123m";
										cout<<"\e[38;2;189;189;189m";
										cout<<blank;
										cout<<"\e[0;0m\e[u";
										visible[MouseY-7][(MouseX/2)-1]='N';
										flags++;
										mine_counter(flags);
										if (board[MouseY-7][(MouseX/2)-1]==-1){
											mines++;
										}
										for (int i=0;i<rows;i++){
											for (int j=0;j<cols;j++){
												if (board[i][j]>=0 and visible[i][j]!='Y'){
													lose=1;
													break;
												}
												if (board[i][j]>=0 and visible[i][j]=='Y'){
													lose=2;
												}
											}
											if (lose==1){
												break;
											}
										}
										if (lose==2){
											break;
										}
									}
									cout<<"\e[u";
								}
					        } 
					        else if (mouse_state==left_click){
					        	if (MouseX>cols-2 and MouseX<cols+3 and MouseY>0 and MouseY<4){
					        		if (colored==0){
						        		cout<<"\e[s\e[?25l\e[3;"<<cols+2<<"H\b";
						        		cout<<"\e[30;103m";
						        		cout<<emoji;
										cout<<"\e[0;0m\e[u";
										colored=1;
										colorX=cols+2;
										colorY=3;
									}	
								}
					        	if (MouseX>0 and MouseX<(2*cols)+1 and MouseY>5 and MouseY<rows+6){
					        		MouseX++;
									if (MouseX%2==0){
										MouseX++;
									}
					        		MouseY++;
					        		if (colored==1){
						        		cout<<"\e[s\e[?25l\e["<<colorY<<";"<<colorX<<"H\b";
										cout<<"\e[48;2;123;123;123m";
										cout<<"\e[38;2;189;189;189m";
						        		if (visible[colorY-7][(colorX/2)-1]=='N'){
											cout<<blank;
										}
										else if (visible[colorY-7][(colorX/2)-1]=='Y'){
											cout<<unicode_num(board[colorY-7][(colorX/2)-1]);
										}
										else{
											cout<<flag;
										}
										
										cout<<"\e[0;0m\e[u";
									}	
					        		if (visible[MouseY-7][(MouseX/2)-1]=='N'){
						        		cout<<"\e[s\e[?25l\e["<<MouseY<<";"<<MouseX<<"H\b";
						        		cout<<"\e[30;103m";
						        		if (visible[MouseY-7][(MouseX/2)-1]=='N'){
											cout<<blank;
										}
										else if (visible[MouseY-7][(MouseX/2)-1]=='Y'){
											cout<<unicode_num(board[MouseY-7][(MouseX/2)-1]);
										}
										else{
											cout<<flag;
										}
										cout<<"\e[3;"<<cols+2<<"H\b";
										cout<<"\e[48;2;123;123;123m";
										cout<<"\U0001F62E";
										
										cout<<"\e[0;0m\e[u";
										colored=1;
										colorX=MouseX;
										colorY=MouseY;
									}
								}
								
	//				        	cout<<"\tX: "<<MouseX<<"\tY: "<<MouseY<<"\tChar: "<<get_char(MouseX,MouseY)<<endl;
							}
							else{
								cout<<"\e[s\e[?25l\e["<<colorY<<";"<<colorX<<"H\b";
								cout<<"\e[48;2;123;123;123m";
								cout<<"\e[38;2;189;189;189m";
				        		if (visible[colorY-7][(colorX/2)-1]=='N'){
									cout<<blank;
								}
								else if (visible[colorY-7][(colorX/2)-1]=='Y'){
									cout<<unicode_num(board[colorY-7][(colorX/2)-1]);
								}
								else if (visible[colorY-7][(colorX/2)-1]=='F'){
									cout<<flag;
								}
								else{
									cout<<emoji;
								}	
				        		
								cout<<"\e[3;"<<cols+2<<"H\b";
								cout<<"\U0001F642";
								
								cout<<"\e[0;0m\e[u";
								colored=0;
							}
			           		if (mouse_state>0){	
								prev_state=mouse_state;
							}
							if (mouse_event>0){
								prev_event=mouse_event;
							}
							                                                   
							if (mouse_event==0 and mouse_state==0 and prev_state==left_click){
								if (MouseX>cols-2 and MouseX<cols+3 and MouseY>0 and MouseY<4){
									lose=3;
					        		break;
								}
								if (MouseX>0 and MouseX<(2*cols)+1 and MouseY>5 and MouseY<rows+6){
					        		MouseX++;
									if (MouseX%2==0){
										MouseX++;
									}
					        		MouseY++;
					        		if (visible[MouseY-7][(MouseX/2)-1]=='N'){
										visible[MouseY-7][(MouseX/2)-1]='Y';
						        		cout<<"\e[s\e[?25l\e["<<MouseY<<";"<<MouseX<<"H\b";
										cout<<"\e[48;2;123;123;123m";
										cout<<"\e[38;2;189;189;189m";
										cout<<unicode_num(board[MouseY-7][(MouseX/2)-1]);
										cout<<"\e[0;0m\e[u";
										
										//Lose Condition
										if (board[MouseY-7][(MouseX/2)-1]==-1){
											visible[MouseY-7][(MouseX/2)-1]='Y';
	//										board[MouseY-7][(MouseX/2)-1]=-2;
											cout<<"\e[s\e["<<MouseY<<";"<<MouseX<<"H\b";
											cout<<"\e[48;2;123;123;123m";
											cout<<"\e[38;2;189;189;189m";
											cout<<explode;
											cout<<"\e[0;0m\e[u";
											lose=0;
											break;
										}
										
										//Win Condition
										for (int i=0;i<rows;i++){
											for (int j=0;j<cols;j++){
												if (board[i][j]>=0 and visible[i][j]!='Y'){
													lose=1;
													break;
												}
												if (board[i][j]>=0 and visible[i][j]=='Y'){
													lose=2;
												}
											}
											if (lose==1){
												break;
											}
										}
										if (lose==2){
											break;
										}
										
										if (firstX==-1 and firstY==-1){
//											timer(cols);
											thread x(timer,cols);
											x.detach();
											cout<<"\e[0m";
											cout<<"\e[?251";
											x.~thread();
											firstX=(MouseX/2)-1;
											firstY=MouseY-7;
											
											while (temp_mines>0){
												MouseX=rand()%cols;
												MouseY=rand()%rows;
												
												if (board[MouseY][MouseX]!=-1 and visible[MouseY][MouseX]!='Y'){
													board[MouseY][MouseX]=-1;
													temp_mines--;
												}
											}
											temp_mines=mines;
											for (int x=0;x<rows;x++){
												for (int y=0;y<cols;y++){
													if (board[x][y]==-1){
														continue;
													}
													if (y>0){
														if (board[x][y-1]==-1){
															board[x][y]++;
														}
													}
													if (y>0 and x>0){
														if (board[x-1][y-1]==-1){
															board[x][y]++;
														}
													}
													if (x>0){
														if (board[x-1][y]==-1){
															board[x][y]++;
														}
													}
													if (y<cols-1 and x>0){
														if (board[x-1][y+1]==-1){
															board[x][y]++;
														}
													}
													if (y<cols-1){
														if (board[x][y+1]==-1){
															board[x][y]++;
														}
													}
													if (y<cols-1 and x<rows-1){
														if (board[x+1][y+1]==-1){
															board[x][y]++;
														}
													}
													if (x<rows-1){
														if (board[x+1][y]==-1){
															board[x][y]++;
														}
													}
													if (y>0 and x<rows-1){
														if (board[x+1][y-1]==-1){
															board[x][y]++;
														}
													}
												}
											}
											cout<<"\e[s\e[?25l\e["<<firstY+7<<";"<<((firstX+1)*2)+1<<"H\b";
											cout<<"\e[48;2;123;123;123m";
											cout<<"\e[38;2;189;189;189m";
											cout<<unicode_num(board[firstY][firstX]);
											cout<<"\e[0;0m\e[u";
//											cout<<"hi"<<board[firstY][firstX]<<" "<<firstX<<" "<<firstY;
											for (int i=0;i<rows;i++){
												for (int j=0;j<cols;j++){
													if (board[i][j]>=0 and visible[i][j]!='Y'){
														lose=0;
														break;
													}
													if (board[i][j]>=0 and visible[i][j]=='Y'){
														lose=2;
													}
												}
												if (lose==0){
													lose=1;
													break;
												}
											}
											if (lose==2){
//												cout<<"\e[s\e[?25l\e["<<colorY<<";"<<colorX<<"H\b";
//												cout<<"\e[48;2;123;123;123m";
//												cout<<"\e[38;2;189;189;189m";
//												cout<<unicode_num(board[colorY-7][(colorX/2)-1]);
//												cout<<"\e[0;0m\e[u";
												break;
											}
											colorY=firstY+7;
											colorX=((firstX+1)*2)+1;
											MouseY=colorY;
											MouseX=colorX;
//											cout<<"hi"<<board[MouseY-7][(MouseX/2)-1]<<" "<<colorX<<" "<<colorY<<"  "<<MouseX<<" "<<MouseY;
										}
//										cout<<"\e["<<rows+9<<";1H\e[0J";
										if (board[MouseY-7][(MouseX/2)-1]==0){
											copy(&visible[0][0], &visible[0][0]+rows*cols,&fake_vis[0][0]);
											copy(&board[0][0], &board[0][0]+rows*cols,&fake_board[0][0]);
//											cout<<"hi"<<board[MouseY-7][(MouseX/2)-1]<<" | "<<MouseX<<" "<<MouseY<<endl;
											temp_vis[MouseY-7][(MouseX/2)-1]='Y';
											depth=0;
											open_spots(fake_vis,fake_board,rows,cols,(MouseX/2)-1,MouseY-7);
//											cout<<endl;
											for (int i=0;i<rows;i++){
												for (int j=0;j<cols;j++){
//													cout<<temp_vis[i][j]<<board[i][j]<<" ";
													if (temp_vis[i][j]=='Y' and visible[i][j]=='N'){
//														cout<<j<<i<<" "<<board[j][i];
														visible[i][j]='Y';
														cout<<"\e[s\e[?25l\e["<<i+7<<";"<<((j+1)*2)+1<<"H\b";
														cout<<"\e[48;2;123;123;123m";
														cout<<"\e[38;2;189;189;189m";
														cout<<unicode_num(board[i][j]);
														cout<<"\e[0;0m\e[u";
													}
												}
//												cout<<endl;
											}	
											temp_vis.clear();
											temp_vis.resize(rows);
											for (int i=0;i<rows;i++){
												temp_vis[i].resize(cols,'N');
											}
//											system("PAUSE");
										}
									}
								}
							}
			            }
			        }
	//		        win == if all 0-8 are open
			    }
			    stop=true;
			    if (lose==3){
					usleep(100000);
					cout<<"\e[0;0m";
			    	continue;
				}
				if (end_choice==0){
					usleep(100000);
					cout<<"\e[0;0m";
			    	break;
				}
			    
			    
				cout<<"\e[s\e[3;"<<cols+2<<"H\b";
				cout<<"\e[48;2;123;123;123m";
				if (lose==0){	
	//				cout<<"\U0001F635";
					cout<<"\U0001F480";
					for (int i=0;i<rows;i++){
						for (int j=0;j<cols;j++){
							if (board[i][j]==-1 and visible[i][j]=='N'){
								cout<<"\e["<<i+7<<";"<<((j+1)*2)+1<<"H\b";
								cout<<"\e[48;2;123;123;123m";
								cout<<"\e[38;2;189;189;189m";
								cout<<bomb;
								cout<<"\e[0;0m";
								visible[i][j]='Y';
							}
							if (board[i][j]>=0 and visible[i][j]=='F'){
								cout<<"\e["<<i+7<<";"<<((j+1)*2)+1<<"H\b";
								cout<<"\e[48;2;123;123;123m";
								cout<<"\e[38;2;189;189;189m";
		//						cout<<board[i][j]<<" ";
								cout<<"\u274C";
								cout<<"\e[0;0m";
							}
						}
					}
				}
				else{
					cout<<"\U0001F60E";
					for (int i=0;i<rows;i++){
						for (int j=0;j<cols;j++){
							if (board[i][j]==-1 and visible[i][j]=='N'){
								cout<<"\e["<<i+7<<";"<<((j+1)*2)+1<<"H\b";
								cout<<"\e[48;2;123;123;123m";
								cout<<"\e[38;2;189;189;189m";
								cout<<flag;
								cout<<"\e[0;0m";
								visible[i][j]='Y';
							}
						}
					}
				}
				usleep(100000);
				cout<<"\e[0;0m\e[u";
//				cout<<"\e[1A\e[2K\r";
	//			system("PAUSE");
				mines=temp_mines;
				if (difficulty<4 and lose==2){
					cout<<"\e[0;0m\e["<<rows+8<<";1H";
					SetConsoleMode(hStdin, fdwSaveOldMode);
					if (MessageBox( NULL, TEXT( "Would you like to add your score to the Leaderboard?"), TEXT( "Minesweeper.exe"), MB_ICONQUESTION | MB_YESNO | MB_APPLMODAL )==6){	
						cout<<"\e[?25h";
						add_score_minesweeper(seconds, difficulty);		//Calls add_score to add the money on the leaderboard
					}
					cout<<"\e[?251";
					cout<<"\e["<<rows+8<<";1H\e[0J";
					fdwMode = ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS;		// Enable the window and mouse input events. 
				    if (!SetConsoleMode(hStdin, fdwMode)){
				    	ErrorExit("SetConsoleMode");
					}
//					cout<<"\e[?251";
				}
				
				while (end_choice==1){
			        if (!ReadConsoleInput(hStdin,irInBuf,128,&cNumRead)){
			            ErrorExit("ReadConsoleInput");
			        }
			        for (i = 0; i < cNumRead; i++){
			        	if (event_type == KEY_EVENT){	 // keyboard input 
		                	if (!key_pressed and key==VK_ESCAPE){
								restart=false;
								end_choice=0;
								break;
							}
						}
						else if (event_type==MOUSE_EVENT){ // mouse input 
							if (mouse_state==left_click){
					        	if (MouseX>cols-2 and MouseX<cols+3 and MouseY>0 and MouseY<4){
					        		if (colored==0){
						        		cout<<"\e[s\e[?25l\e[3;"<<cols+2<<"H\b";
						        		cout<<"\e[30;103m";
						        		cout<<emoji;
										cout<<"\e[0;0m\e[u";
										colored=1;
									}	
								}
	//				        	cout<<"\tX: "<<MouseX<<"\tY: "<<MouseY<<"\tChar: "<<get_char(MouseX,MouseY)<<endl;
							}
							else if (colored==1){
				        		cout<<"\e[s\e[?25l\e[3;"<<cols+2<<"H\b";
								cout<<"\e[48;2;123;123;123m";
				        		cout<<emoji;
								cout<<"\e[0;0m\e[u";
								colored=0;
							}
			           		if (mouse_state>0){	
								prev_state=mouse_state;
							}
							if (mouse_event>0){
								prev_event=mouse_event;
							}
							
							if (mouse_event==0 and mouse_state==0 and prev_state==left_click){
								if (MouseX>cols-2 and MouseX<cols+3 and MouseY>0 and MouseY<4){
									end_choice=0;
					        		break;
								}
							}
			            }
			        }
			    }
			}
		}
	    else if (menu_choice==2){
			cout<<"                   __  __                   __            ____   __             \n";				//Rules Title Card ASCII Art
			cout<<"                  / / / /____  _      __   / /_ ____     / __ \\ / /____ _ __  __\n";			//More Rules Title Card ASCII Art
			cout<<"                 / /_/ // __ \\| | /| / /  / __// __ \\   / /_/ // // __ `// / / /\n";			//More Rules Title Card ASCII Art
			cout<<"                / __  // /_/ /| |/ |/ /  / /_ / /_/ /  / ____// // /_/ // /_/ / \n";				//More Rules Title Card ASCII Art
			cout<<"               /_/ /_/ \\____/ |__/|__/   \\__/ \\____/  /_/    /_/ \\__,_/ \\__, /  \n";		//More Rules Title Card ASCII Art
			cout<<"                                                                       /____/   ";				//More Rules Title Card ASCII Art
			cout<<"\n-----------------------------------------------------------------------------------------------\n";		//Line of Separation
			
			cout<<"Object of the Game:\n";				//Rules of Blackjack
			cout<<"\tMinesweeper is a game where mines are hidden in a grid of squares. If you click on a mine you lose the game!\n";				//Rules of Blackjack
			
			cout<<"\nThe Play:\n";				//Rules of Blackjack
			cout<<"\tThe first click is always safe. Open squares with a left click and put flags on mines with a right click.";
			cout<<"\n\tSafe squares have numbers telling you how many mines touch the square.";
			cout<<"\n\tWhen you open a square that does not touch any mines, it will be empty and open adjacent squares.";
			cout<<"\n\tYou can use the number clues to solve the game by opening all of the safe squares.";
			cout<<"\n\tWhen a number touches the same number of squares, those squares must be mines.\n";
			
			cout<<"\nWinning:\n";				//Rules of Blackjack
			cout<<"\tThe game ends when all safe squares have been opened.";
			cout<<"\n\tA counter shows the number of mines without flags, and a clock shows your time in seconds.\n";
			
			cout<<"\nExtra:\n";				//Rules of Blackjack
			cout<<"\tPress Escape to exit a game.";
			cout<<"\n\tPress R to refresh the screen, in the case of a visual error.";
			cout<<"\n\tPress the emoji to restart the game, even in the middle of one.";
			cout<<"\n\tThe leaderboard saves the best 3 times for each difficulty level.\n\n";
			system("PAUSE");
		}
	    else if (menu_choice==3){
			cout<<flush;		//flushes the output stream
			system ("CLS");		//clears the screen
			cout<<"   ______                   __ __                  __\n";		//Goodbye Title Card ASCII Art
			cout<<"  / ____/____   ____   ____/ // /_   __  __ ___   / /\n";		//Goodbye Title Card ASCII Art
			cout<<" / / __ / __ \\ / __ \\ / __  // __ \\ / / / // _ \\ / / \n";	//Goodbye Title Card ASCII Art
			cout<<"/ /_/ // /_/ // /_/ // /_/ // /_/ // /_/ //  __//_/  \n";		//Goodbye Title Card ASCII Art
			cout<<"\\____/ \\____/ \\____/ \\__,_//_.___/ \\__, / \\___/(_)   \n";	//Goodbye Title Card ASCII Art
			cout<<"                                  /____/             \n";			//Goodbye Title Card ASCII Art
			break;		//breaks out of the do-while loop
		}
		else{
			show_score_minesweeper();			//Calls show_score() to display the top three scores
			system("PAUSE");
			if (!easy.empty() or !medium.empty() or !hard.empty()){
				if (MessageBox( NULL, TEXT( "Would you like to clear the leaderboard?"), TEXT( "Minesweeper.exe"), MB_ICONQUESTION | MB_YESNO | MB_APPLMODAL)==6){ 
					cout<<"\e[?25h\n";
					Center_text("Enter the password: ");		//Tells the user what to input
					cin>>password;			//Prompts the user to input
					if (password=="71737"){			//if the password is correct
						remove("highscores.csv");		//Delete the highscore file
						Center_text("Leaderboard Cleared.");		//Tells the user that the leaderboard was cleared
						cout<<"\n";
						
						fstream scores;									//Create a file object for reading/writing
					    scores.open("highscores.csv");					// attempt to open the highscore file
					    if (!scores){										//If the file does not open
					        scores.open("highscores.csv", ios::out);		//Create and open a highscore file for writing
					        scores<<"Highscores"<<endl;					//Write to the highscore file
					    }	
					    scores.close();									//close the highscore file
					}	
					else{
						Center_text("WRONG PASSWORD!");
						cout<<"\n";
					}
					cout<<"\e[?251";
				}
			}
		}
		cout<<"\e[0m";
	    cout<<flush;
	    system("CLS");
	} while (menu_choice!=0);
}

void open_spots(char fake_vis[][50], int fake_board[][50], int rows, int cols, int x, int y){
	char visible[rows][cols];		//Y for on, N for off, F for flag (show flag emoji but not number, basically no)
	int board[rows][cols];			//ini=t all to 0, 2 for loops that check all spaces around after first spot is picked, bomb is -1
	copy(&fake_vis[0][0], &fake_vis[0][0]+rows*cols,&visible[0][0]);
	copy(&fake_board[0][0], &fake_board[0][0]+rows*cols,&board[0][0]);
//	cout<<x<<y<<endl;
//	cout<<visible[y][x]<<" "<<board[y][x]<<endl;
	depth++;
	
	if (board[y][x]==0){
		if (y>0){
			if (board[y][x-1]>=0){
				y--;
				visible[y][x]='Y';
				if (board[y][x]==0 and temp_vis[y][x]=='N'){
					temp_vis[y][x]='Y';
					open_spots(fake_vis,fake_board,rows,cols,x,y);
				}
				temp_vis[y][x]='Y';
				y++;
			}
		}
		if (y>0 and x>0){
			if (board[y-1][x-1]>=0){
				x--;
				y--;
				visible[x][y]='Y';
				if (board[y][x]==0 and temp_vis[y][x]=='N'){
					temp_vis[y][x]='Y';
					open_spots(fake_vis,fake_board,rows,cols,x,y);
				}
				temp_vis[y][x]='Y';
				x++;
				y++;
			}
		}
		if (x>0){
			if (board[y][x-1]>=0){
				x--;
				visible[x][y]='Y';
				if (board[y][x]==0 and temp_vis[y][x]=='N'){
					temp_vis[y][x]='Y';
					open_spots(fake_vis,fake_board,rows,cols,x,y);
				}
				temp_vis[y][x]='Y';
				x++;
			}
		}
		if (y<rows-1 and x>0){
			if (board[y+1][x-1]>=0){
				x--;
				y++;
				visible[x][y]='Y';
				if (board[y][x]==0 and temp_vis[y][x]=='N'){
					temp_vis[y][x]='Y';
					open_spots(fake_vis,fake_board,rows,cols,x,y);
				}
				temp_vis[y][x]='Y';
				x++;
				y--;
			}
		}
		if (y<rows-1){
			if (board[y+1][x]>=0){
				y++;
				visible[x][y]='Y';
				if (board[y][x]==0 and temp_vis[y][x]=='N'){
					temp_vis[y][x]='Y';
					open_spots(fake_vis,fake_board,rows,cols,x,y);
				}
				temp_vis[y][x]='Y';
				y--;
			}
		}
		if (y<rows-1 and x<cols-1){
			if (board[y+1][x+1]>=0){
				x++;
				y++;
				visible[x][y]='Y';
				if (board[y][x]==0 and temp_vis[y][x]=='N'){
					temp_vis[y][x]='Y';
					open_spots(fake_vis,fake_board,rows,cols,x,y);
				}
				temp_vis[y][x]='Y';
				x--;
				y--;
			}
		}
		if (x<cols-1){
			if (board[y][x+1]>=0){
				x++;
				visible[x][y]='Y';
				if (board[y][x]==0 and temp_vis[y][x]=='N'){
					temp_vis[y][x]='Y';
					open_spots(fake_vis,fake_board,rows,cols,x,y);
				}
				temp_vis[y][x]='Y';
				x--;
			}
		}
		if (y>0 and x<cols-1){
			if (board[y-1][x+1]>=0){
				x++;
				y--;
				visible[x][y]='Y';
				if (board[y][x]==0 and temp_vis[y][x]=='N'){
					temp_vis[y][x]='Y';
					open_spots(fake_vis,fake_board,rows,cols,x,y);
				}
				temp_vis[y][x]='Y';
				x--;
				y++;
			}
		}
	}
	depth--;
//	cout<<"depth: "<<depth<<endl;
}

string unicode_num(int num){
	string unicode_num;
	
	switch(num){
		case 0:
//			unicode_num="\uFF10";
			unicode_num="  ";
			break;
		case 1:
			cout<<"\e[38;2;0;0;255m";
			unicode_num="\uFF11";
			break;
		case 2:
			cout<<"\e[38;2;0;123;0m";
			unicode_num="\uFF12";
			break;
		case 3:
			cout<<"\e[38;2;255;0;0m";
			unicode_num="\uFF13";
			break;
		case 4:
			cout<<"\e[38;2;0;0;123m";
			unicode_num="\uFF14";
			break;
		case 5:
			cout<<"\e[38;2;123;0;0m";
			unicode_num="\uFF15";
			break;
		case 6:
			cout<<"\e[38;2;0;123;123m";
			unicode_num="\uFF16";
			break;
		case 7:
			cout<<"\e[38;2;0;0;0m";
			unicode_num="\uFF17";
			break;
		case 8:
			cout<<"\e[38;2;123;123;123m";
			unicode_num="\uFF18";
			break;
		case -1:
			unicode_num="\U0001F4A3";
			break;
	}
	
	return unicode_num;
}

string ascii_num(int num, int row){
	string unicode_num;
    if (num==-1){
    	if (row==1){
			unicode_num="\u2800\u2800";
		}
		else if (row==2){
			unicode_num="\u257A\u2578";
		}
		else{
			unicode_num="\u2800\u2800";
		}
		return unicode_num;
	}
	 
	if (row==1){
		unicode_num="\u250f\u2513\u2800\u257b\u257A\u2513\u257a\u2513\u257B\u257B\u250F\u2578\u250F\u2578\u257A\u2513\u250f\u2513\u250F\u2513";
	}
	else if (row==2){
		unicode_num="\u2503\u2503\u2800\u2503\u250F\u251b\u257a\u252b\u2517\u252B\u2517\u2513\u2523\u2513\u2800\u2503\u2523\u252b\u2517\u252B";
	}
	else{
		unicode_num="\u2517\u251b\u2800\u2579\u2517\u2578\u257a\u251b\u2800\u2579\u257A\u251B\u2517\u251B\u2800\u2579\u2517\u251b\u257A\u251B";
	}
	if (num>0){
		unicode_num.erase(0,((num)*6));
	}
    unicode_num.erase(6);   
	return unicode_num;
}

void mine_counter(int mines){
	bool neg=false;
	
	if (mines<0){
		neg=true;
		mines*=-1;
	}
	if (mines<1000){
		cout<<"\e[s\e[38;2;255;0;0m\e[48;2;0;0;0m";
		cout<<"\e[2;8H\b\b\b\b\b\b";
		if (neg==true){
			cout<<ascii_num(-1,1);
		}
		else{
			cout<<ascii_num(mines/100,1);
		}
		cout<<ascii_num((mines/10)%10,1)<<ascii_num(mines%10,1);
		cout<<"\e[3;8H\b\b\b\b\b\b";
		if (neg==true){
			cout<<ascii_num(-1,2);
		}
		else{
			cout<<ascii_num(mines/100,2);
		}
		cout<<ascii_num((mines/10)%10,2)<<ascii_num(mines%10,2);
		cout<<"\e[4;8H\b\b\b\b\b\b";
		if (neg==true){
			cout<<ascii_num(-1,3);
		}
		else{
			cout<<ascii_num(mines/100,3);
		}
		cout<<ascii_num((mines/10)%10,3)<<ascii_num(mines%10,3);
		cout<<"\e[0m\e[u";
		
	}
}

void timer(int cols){
	int true_secs=0;
	stop=false;
	while(stop==false){
		if (seconds<1000){
			cout<<"\e[s\e[38;2;255;0;0m\e[48;2;0;0;0m";
			cout<<"\e[2;"<<(cols+1)*2<<"H\b\b\b\b\b\b";
			cout<<ascii_num(seconds/100,1)<<ascii_num((seconds/10)%10,1)<<ascii_num(seconds%10,1);
			cout<<"\e[3;"<<(cols+1)*2<<"H\b\b\b\b\b\b";
			cout<<ascii_num(seconds/100,2)<<ascii_num((seconds/10)%10,2)<<ascii_num(seconds%10,2);
			cout<<"\e[4;"<<(cols+1)*2<<"H\b\b\b\b\b\b";
			cout<<ascii_num(seconds/100,3)<<ascii_num((seconds/10)%10,3)<<ascii_num(seconds%10,3);
			cout<<"\e[0m\e[u";
		}
	
		if (first==false){
			sleep(1);
		}
		else{
			first=false;
			sleep(1);
		}
		
		if (seconds<999){
			seconds++;
		}
		else{
			true_secs++;
		}
		
	}
	seconds+=true_secs;
	
}

void create_minesweeper(char fake_vis[][50], int fake_board[][50], int rows, int cols, int mines){
	char visible[rows][cols];		//Y for on, N for off, F for flag (show flag emoji but not number, basically no)
	int board[rows][cols];			//ini=t all to 0, 2 for loops that check all spaces around after first spot is picked, bomb is -1
	string bomb="\U0001F4A3";
	string explode="\U0001F4A5";
	string flag="\U0001F6A9";
	string blank="\U0001F532";
	string emoji="\U0001F642";
	copy(&fake_vis[0][0], &fake_vis[0][0]+rows*cols,&visible[0][0]);
	copy(&fake_board[0][0], &fake_board[0][0]+rows*cols,&board[0][0]);
//	cout<<"\u250f\u2513 \u2800\u257b \u257A\u2513 \u257a\u2513 \u257B\u257B \u250F\u2578 \u250F\u2578 \u257A\u2513 \u250f\u2513 \u250F\u2513\n";
//	cout<<"\u2503\u2503 \u2800\u2503 \u250F\u251b \u257a\u252b \u2517\u252B \u2517\u2513 \u2523\u2513 \u2800\u2503 \u2523\u252b \u2517\u252B\n";
//	cout<<"\u2517\u251b \u2800\u2579 \u2517\u2578 \u257a\u251b \u2800\u2579 \u257A\u251B \u2517\u251B \u2800\u2579 \u2517\u251b \u257A\u251B\n";
//	stop=true;
	cout<<"\e[48;2;123;123;123m";
	cout<<"\e[48;2;123;123;123m";
	cout<<"\u259B";
	for (int i=0;i<cols*2;i++){
		cout<<"\u2580";
	}
	cout<<"\u259C\n";
	for (int i=0;i<3;i++){
		cout<<"\u258c";
		for (int j=0;j<cols;j++){
			if (cols==8 and j==(ceil(cols/2.0))-1){
				cout<<"\e[48;2;123;123;123m";
				cout<<"\e[38;2;189;189;189m";
				if (i==0){
					cout<<"\u250F\u2501\u2501\u2513";
				}
				else if (i==1){
					cout<<"\u2503"<<emoji<<"\u2503";	
				}
				else if (i==2){
					cout<<"\u2517\u2501\u2501\u251B";	
				}
				
				j+=1;
			}
			else if (cols!=8 and j==(ceil(cols/2.0))-2){
				cout<<"\e[48;2;123;123;123m";
				cout<<"\e[38;2;189;189;189m";
				if (cols%2==0){
					if (i==0){
						cout<<"  \u250F\u2501\u2501\u2513";
					}
					else if (i==1){
						cout<<"  \u2503"<<emoji<<"\u2503";	
					}
					else if (i==2){
						cout<<"  \u2517\u2501\u2501\u251B";	
					}
				}
				else{
					if (i==0){
						cout<<" \u250F\u2501\u2501\u2513 ";
					}
					else if (i==1){
						cout<<" \u2503"<<emoji<<"\u2503 ";	
					}
					else if (i==2){
						cout<<" \u2517\u2501\u2501\u251B ";	
					}
				}
				j+=2;
			}
			else if ((j>=0 and j<3) or (j>cols-4 and j<cols)){
				cout<<"\e[38;2;255;0;0m\e[48;2;0;0;0m";  
				if (j==0){
					cout<<ascii_num(mines/100,i+1);
				}
				else if (j==1){
					cout<<ascii_num((mines/10)%10,i+1);
				}
				else if (j==2){
					cout<<ascii_num(mines%10,i+1);
				}
				else if (j==cols-3){
					cout<<ascii_num(seconds/100,i+1);
				}
				else if (j==cols-2){
					cout<<ascii_num((seconds/10)%10,i+1);
				}
				else if (j==cols-1){
					cout<<ascii_num(seconds%10,i+1);
				}
				else{
					cout<<"  ";
				}
			}
			else{		
				cout<<"\e[48;2;123;123;123m";
				cout<<"\e[38;2;189;189;189m";
				if (j==(cols/2) and i==1){
					cout<<" ";	
				}
				else{
					cout<<"  ";	
				}
				
			}
		}
		cout<<"\e[48;2;123;123;123m";
		cout<<"\e[38;2;189;189;189m";
		cout<<"\u2590";
		cout<<endl;		
	}
	cout<<"\u2599";
	for (int i=0;i<cols*2;i++){
		cout<<"\u2584";
	}
	cout<<"\u259F\n";
	
	cout<<"\e[48;2;123;123;123m";
	cout<<"\e[38;2;189;189;189m\u259B";
	for (int i=0;i<cols*2;i++){
		cout<<"\u2580";
	}
	cout<<"\u259C\n";
	for (int i=0;i<rows;i++){
		cout<<"\u258c";
		for (int j=0;j<cols;j++){
			if (visible[i][j]=='N'){
				cout<<blank;
			}
			else if (visible[i][j]=='Y'){
				cout<<unicode_num(board[i][j]);
			}
			else{
				cout<<flag;
			}
		}
		cout<<"\e[38;2;189;189;189m\u2590";
		cout<<endl;
	}
	cout<<"\u2599";
	for (int i=0;i<cols*2;i++){
		cout<<"\u2584";
	}
	cout<<"\u259F\n";
	cout<<"\e[0m";
	cout<<"\e[?251";
	usleep(100000);
//	system("PAUSE");
}

bool operator<(const name_time&lhs, const name_time&rhs){			//defines the operator for the structure and makes it accept two arguments to make a comparison
    return lhs.time < rhs.time;				//returns the smaller of the two arguments
}

void add_score_minesweeper(int time, int difficulty){		//Defines a void function with an int paaramter for the players money
	string name;		//Declares a string for the users name
	cout<<"\nWhat is your name: ";		//Tells the user what to input
	cin>>name;			//Prompts the user to input
    ofstream highscore;		// Create an output filestream object
	highscore.open("highscores.csv", ios::app);    //Open the highscores.csv file to add data
    highscore<<difficulty<<","<<name<<","<<time<<endl;		// Send data to the stream
    highscore.close();			// Close the file
}

void show_score_minesweeper(){
	int total_rows=0;		//Declares an int for the total number of rows and initializes it to 0
	int curr_row=0;		//Declares an int for the current row and initializes it to 0
	int time;		//Declares an int for a time from the spreadsheet
	int difficulty;
	int easy_rows=0;
	int medium_rows=0;
	int hard_rows=0;
	string line;		//declares a string for a line in the spreadsheet
	string phrase;		//Declares a string for the two columns of a line combined from the spreadsheet
	string names;		//Declares a string for the name part of the line
	int comma_pos;		//Declares an int for the location of the comma that separates the columns
	ifstream file("highscores.csv");		//opens the highscore file for input
	while (getline(file, line)){		//while the its possible to read a line from the spreadsheet
		total_rows++;			//Increases the total row count by 1
		if (line.at(0)=='1'){
			easy_rows++;
		}
		else if (line.at(0)=='2'){
			medium_rows++;
		}
		else if (line.at(0)=='3'){
			hard_rows++;
		}
	}
	file.close();		//Closes the spreadsheet
	
	easy.resize(0);		//changes the size of the scores vector to the total rows
	medium.resize(0);		//changes the size of the scores vector to the total rows
	hard.resize(0);		//changes the size of the scores vector to the total rows
		
	ifstream file2("highscores.csv");			//opens the highscore file for input
	for (;curr_row<total_rows;curr_row++){		//Until the current row reaches the total row count
		file2>>phrase;		//Reads in a line from the spreadsheet
		if (phrase!="Highscores"){		//If its not the first line
			difficulty=phrase.at(0)-'0';
			phrase.erase(0,2);
			names=phrase;		//names is initializes to the line from the spreadsheet
			comma_pos=phrase.find_last_of(",");		//Finds the location of the final comma (in case one is in the name)
			phrase.erase(0, comma_pos+1);		//removes the name from the line
			names.erase(comma_pos);		//keeps only the name from the line
			time=stoi(phrase);		//convers the score into an int and saves it into score
			
			if (difficulty==1){
				easy.push_back({time,names});		//Puts the score and name into a value in the scores vector
			}
			else if (difficulty==2){
				medium.push_back({time,names});		//Puts the score and name into a value in the scores vector
			}
			else if (difficulty==3){
				hard.push_back({time,names});		//Puts the score and name into a value in the scores vector
			}
		}
	}     
	               
	stable_sort(easy.begin(),easy.end());		//Sorts the scores vector by score while keeping the names with them (lowest to highest)               
	stable_sort(medium.begin(),medium.end());		//Sorts the scores vector by score while keeping the names with them (lowest to highest)               
	stable_sort(hard.begin(),hard.end());		//Sorts the scores vector by score while keeping the names with them (lowest to highest)
	
	SetConsoleMode(hStdin, fdwSaveOldMode);
	
	Center_text("    __                   __          __                         __");
	cout<<"\n";
	Center_text("   / /   ___  ____ _____/ /__  _____/ /_  ____  ____ __________/ /");
	cout<<"\n";
	Center_text("  / /   / _ \\/ __ `/ __  / _ \\/ ___/ __ \\/ __ \\/ __ `/ ___/ __  / ");
	cout<<"\n";
	Center_text(" / /___/  __/ /_/ / /_/ /  __/ /  / /_/ / /_/ / /_/ / /  / /_/ /  ");
	cout<<"\n";
	Center_text("/_____/\\___/\\__,_/\\__,_/\\___/_/  /_.___/\\____/\\__,_/_/   \\__,_/   ");
	cout<<"\n\n";
	Center_text("----------------------------------------------------------------------------------------------------");			//Line of separation
	cout<<endl<<endl;
	if (total_rows==1){		//If there are no scores
		Center_text("Add Some Scores");			//tells the user that the leaderboard is blank
		cout<<endl<<endl;
		return;
	}	
	if (easy_rows>0){
		Center_text("Easy:");
		cout<<"\n";
		if (easy_rows>2){		//If there are more than three scores in the leaderboard
			for (int i=0;i<3;i++){			//From the last score to the third last (inclusive)
				Center_text("#"+to_string(i+1)+" - "+easy[i].name+": "+to_string(easy[i].time)+"s");		//Outputs the name and the score 
				cout<<"\n";
			}
		}
		else if (easy_rows<2){		//Otherwise if there are less that three scores
			for (int i=0;i<easy_rows;i++){		//From the last score to 0
				Center_text("#"+to_string(i+1)+" - "+easy[i].name+": "+to_string(easy[i].time)+"s");		//Outputs the name and the score 
				cout<<"\n";
			}
		}
	}	
	if (medium_rows>0){
		cout<<"\n";
		Center_text("Medium:");
		cout<<"\n";
		if (medium_rows>2){		//If there are more than three scores in the leaderboard
			for (int i=0;i<3;i++){			//From the last score to the third last (inclusive)
				Center_text("#"+to_string(i+1)+" - "+medium[i].name+": "+to_string(medium[i].time)+"s");		//Outputs the name and the score 
				cout<<"\n";
			}
		}
		else if (medium_rows<2){		//Otherwise if there are less that three scores
			for (int i=0;i<medium_rows;i++){		//From the last score to 0
				Center_text("#"+to_string(i+1)+" - "+medium[i].name+": "+to_string(medium[i].time)+"s");		//Outputs the name and the score 
				cout<<"\n";
			}
		}
	}	
	if (hard_rows>0){
		cout<<"\n";
		Center_text("Hard:");
		cout<<"\n";
		if (hard_rows>2){		//If there are more than three scores in the leaderboard
			for (int i=0;i<3;i++){			//From the last score to the third last (inclusive)
				Center_text("#"+to_string(i+1)+" - "+hard[i].name+": "+to_string(hard[i].time)+"s");		//Outputs the name and the score 
				cout<<"\n";
			}
		}
		else if (hard_rows<2){		//Otherwise if there are less that three scores
			for (int i=0;i<hard_rows;i++){		//From the last score to 0
				Center_text("#"+to_string(i+1)+" - "+hard[i].name+": "+to_string(hard[i].time)+"s");		//Outputs the name and the score 
				cout<<"\n";
			}
		}
	}
	
	file2.close();		//Closes the highscore file
}

VOID ErrorExit(LPCSTR lpszMessage){
    fprintf(stderr, "%s\n", lpszMessage);

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    ExitProcess(0);
}


char get_char( short x, short y ){
  CHAR_INFO ci;
  COORD xy = { 0, 0 };
  COORD xy1 = {1,1};
  SMALL_RECT rect = { x, y, x, y };
  return ReadConsoleOutput( GetStdHandle( STD_OUTPUT_HANDLE ), &ci, xy1, xy, &rect )
       ? ci.Char.AsciiChar
       : '\0';
}

void Center_text(string text){
	// This function will only center the text if it is less than the length of the console!
	// Otherwise it will just display it on the console without centering it.
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Get the console handle.
	PCONSOLE_SCREEN_BUFFER_INFO lpScreenInfo = new CONSOLE_SCREEN_BUFFER_INFO(); // Create a pointer to the Screen Info pointing to a temporal screen info.
	GetConsoleScreenBufferInfo(hConsole, lpScreenInfo); // Saves the console screen info into the lpScreenInfo pointer.
	COORD NewSBSize = lpScreenInfo->dwSize; // Gets the size of the screen
	NewSBSize.X=128;
	if (NewSBSize.X > text.size() and (NewSBSize.X-text.size())>0) {
	    int newpos = ((NewSBSize.X - text.size()) / 2); // Calculate the number of spaces to center the specific text.
//		cout<<NewSBSize.X<<" "<<text.size()<<" "<<newpos<<endl;
	    for (int i = 0; i < newpos; i++) cout << " "; // Prints the spaces
	}
	cout<<text;
}
