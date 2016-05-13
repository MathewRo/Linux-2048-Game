/*******************************************************************
2048 is a single-player sliding block puzzle game by 19-year-old Italian
web developer Gabriele Cirulli.

HOW TO PLAY: Use your arrow keys to move the tiles. 
When two tiles with the same number touch, they merge into one!

2048 - Linux console implementation using ascii colouring schemes
Feel free to distribute!
*******************************************************************/
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <termios.h>
#include <string.h>
#include <fstream>

#define K_UP 65
#define K_DOWN 66
#define K_LEFT 68
#define K_RIGHT 67


using namespace std;
int indexrandomizer(int);
char getc();
time_t t;
int rs, pileup, wincheck = 0;
int sock;
char *name;	

class MyGame{
	private:
		int m_ar[4][4];
		int b_ar[4][4];
		int score;
		int pscore;
		char press;
	public:
		void initializegrid();
		void projector();
		void addflicker(int);
		void tapkey();
		void ushift();
		void dshift();
		void lshift();
		void rshift();
		void addvalue();
		int tablechart(int);
		int clogcheck();
		void backup();
		void iswin();
		void endascii();
		void clearFunc();
	protected:
		MyGame(int,int);
		virtual ~MyGame(){}
};

class MyGameDB : public MyGame{ // planning for an online hiscore server - Not yet implemented
	public:
		void initnet();
		void chmode();
		static MyGameDB* createnew();
	protected:
		MyGameDB(int, int);
		~MyGameDB(){}
};


int main(){
	std::ios_base::sync_with_stdio(false);
	MyGameDB *game = MyGameDB :: createnew();
	srand(time(&t));
	name  = (char*)malloc(100);
	game->initializegrid();
	game->chmode(); 
	do{
		game->backup();
		game->projector();
		game->tapkey();
		game->projector();
		game->addflicker(200000);
		game->addvalue();	
		game->iswin();
	}while(game->clogcheck());
	game->projector();
	game->addflicker(4000000);	
	rs = indexrandomizer(25);
	game->endascii();
	return 0;
}

MyGameDB* MyGameDB :: createnew(){
	return new MyGameDB(0,0);
}

MyGameDB :: MyGameDB(int val1, int val2):MyGame(val1, val2){
}

MyGame :: MyGame(int val1, int val2):score(val1),pscore(val2){
	int i,j;
	for (i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			m_ar[i][j] = b_ar[i][j]=0;
}

void MyGame :: initializegrid(){ // initial grid setup
	int i, j, m, n;
	i = indexrandomizer(4);
	j = indexrandomizer(4);
	m_ar[i][j] = 1024;
	do{
	m = indexrandomizer(4);
	n = indexrandomizer(4);
	}while(m == i && n == j);
	m_ar[m][n] = 1024;
}

void MyGame :: addflicker(int flick){
	usleep(flick);
} 

void MyGameDB :: chmode(){ // Game startup 
	system("clear");
	string content;
	ifstream startup("2048.txt");
	if (startup.fail()) return;
	while(getline(startup, content))	
		cout << content << endl;
	this->addflicker(3000000);
	system("clear");
	cout << "Enter User Name : " ;
	cin >> name;
	this->addflicker(1000000);
}

void MyGame :: endascii(){ // GAMEOVER ascii art
	system("clear");
        string content;
        ifstream endmode("2047.txt");
        if (endmode.fail()) return;
        while(getline(endmode, content))
                cout << content << endl;
	this->addflicker(4000000);
	this->clearFunc();
}

void MyGame :: backup(){
	int i, j, check;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			b_ar[i][j] = m_ar[i][j];
		}
	}
}

int indexrandomizer(int index){ 
	int pass;
	pass = rand() % index + 0;
	return pass;
}

void MyGame :: projector(){ // fuction to project the entire grid onto the console
	system("clear");
	int i,j;
	printf("%64cAI GAMES PROJECT\n\n\n\n",' ');
	for(i=0;i<216;i++)
		printf("\033[104m%2c\033[0m",' ');
	printf("\n\n\n\n%29cSCORE : %4d%62cCONTROLS :%5c^ ",' ',score,' ',' ');
	printf("\n%115c <   >",' ');
	printf("\n\n\n");
	printf("%55c___________________________________\n",' ');
	for(i=0;i<4;i++){
		printf("%50c    |",' ');
		for(j=0;j<4;j++){
			if(m_ar[i][j]!=0){
				switch(m_ar[i][j]){
					case 2:
						printf("\033[45m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 4:
						printf("\033[30;43m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 8:
						printf("\033[44m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 16:
						printf("\033[41m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 32:
						printf("\033[30;106m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 64:
						printf("\033[93;40m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 128:
						printf("\033[30;47m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 256:
						printf("\033[92;100m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 512:
						printf("\033[30;42m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 1024:
						printf("\033[30;101m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 2048:
						printf("\033[93;47m%4d    |\033[0m",m_ar[i][j]);
						break;
					case 4096:	
						printf("\033[45m%4d    |\033[0m",m_ar[i][j]);
						break;

				}}
			else
				printf("%4c    |",' ');
		}
		printf("\n%54c!________!________!________!________!\n",' ');
	}
	printf("\n\n\n");
	if((score-pscore)!= 0){
		pileup=score-pscore;
		printf("%29cADDED : %4d%62cUSER :%8s ",' ',score-pscore,' ',name);
	}
	else if(pileup!=0)
		printf("%29cADDED : %4d%62cUSER :%8s ",' ',pileup,' ',name);
	printf("\n\n\n\n\n");
	for(i=0;i<216;i++)
		printf("\033[104m%2c\033[0m",' ');
	printf("\n");
	pscore = score;
}

void MyGame :: tapkey(){
	press = getc();
	switch(press){
		case K_UP:
			this->ushift();		
			break;
		case K_DOWN:
			this->dshift();
			break;
		case K_LEFT:
			this->lshift();
			break;
		case K_RIGHT:
			this->rshift();
			break;
		default:
			this->tapkey();
			break;
	}
}

char getc(){ // getch implemenation using termios header
	char buf=0;
	struct termios old={0};
	fflush(stdout);
	if(tcgetattr(0, &old)<0)
		perror("tcsetattr()");
	old.c_lflag&=~ICANON;
	old.c_lflag&=~ECHO;
	old.c_cc[VMIN]=1;
	old.c_cc[VTIME]=0;
	if(tcsetattr(0, TCSANOW, &old)<0)
		perror("tcsetattr ICANON");
	if(read(0,&buf,1)<0)
		perror("read()");
	old.c_lflag|=ICANON;
	old.c_lflag|=ECHO;
	if(tcsetattr(0, TCSADRAIN, &old)<0)
		perror ("tcsetattr ~ICANON");
	return buf;
}

void MyGame :: ushift(){ // Up shift Algorithm 
	int i,j,k,scr=0;
	for(i = 0; i < 4; i++){  // Could have done this in much better ways - just a random approach
		for(k = 0; k < 3; k++){	
			for(j = 0; j < 4; j++){
				if(j < 3)	{	
					if(m_ar[j][i] == 0 && m_ar[j+1][i] == 0){
					}
					else if(m_ar[j][i] == m_ar[j+1][i] && m_ar[j][i] != 0){
						scr = this->tablechart(m_ar[j][i]);
						m_ar[j][i] = m_ar[j+1][i] + m_ar[j][i];
						m_ar[j+1][i] = 0;
						if(j == 0) k = k + 2;
						else if(j == 1 || j == 2) k = k + 3;}
					else{
						if(m_ar[j][i] == 0){
							m_ar[j][i] = m_ar[j+1][i];
							m_ar[j+1][i] = 0;}
						else if(m_ar[j+1][i] != 0 && m_ar[j][i] != 0){} 


					}
				}
			}
		}
	}
	score = score + scr;
}					
void MyGame :: dshift(){ // Down shift Algorithm
	int i,j,k,scr=0,check; 
	for(i = 0; i < 4; i++){  // Could have done this in much better ways - just a random approach 
		for(k = 0; k < 3; k++){
			for(j = 3; j >= 0; j--){
				if(j > 0) {
					if(m_ar[j][i] == 0 && m_ar[j-1][i] == 0){
					}
					else if((m_ar[j][i] == m_ar[j-1][i]) && m_ar[j][i] != 0){
						scr = this->tablechart(m_ar[j][i]);
						m_ar[j][i] = m_ar[j-1][i] + m_ar[j][i];
						m_ar[j-1][i] = 0;
						if(j == 3) k = k + 2;
						else if(j == 2 || j == 1) k = k + 3;}
					else{
						if(m_ar[j][i] == 0){
							m_ar[j][i] = m_ar[j-1][i];
							m_ar[j-1][i] = 0;}
						else if(m_ar[j-1][i] != 0 && m_ar[j][i] != 0){}

					}
				}
			}
		}
	}
	score = score + scr;
}

void MyGame :: rshift(){ // Right shift Algorithm
	int i,j,k,scr=0;
	for(i = 0;i < 4; i++){  // Could have done this in much better ways - just a random approach
		for(k = 0; k < 3; k++){
			for(j = 3; j >= 0; j--){
				if(j > 0) {
					if(m_ar[i][j] == 0 && m_ar[i][j-1] == 0){
					}
					else if((m_ar[i][j] == m_ar[i][j-1]) && m_ar[i][j] != 0){
						scr = this->tablechart(m_ar[i][j]);
						m_ar[i][j] = m_ar[i][j-1] + m_ar[i][j];
						m_ar[i][j-1] = 0;
						if(j == 3) k = k + 2;
						else if(j == 1 || j == 2) k = k + 3;}
					else{
						if(m_ar[i][j] == 0){
							m_ar[i][j] = m_ar[i][j-1];
							m_ar[i][j-1] = 0;}
						else if(m_ar[i][j-1] != 0 && m_ar[i][j] != 0){}

					}
				}
			}
		}
	}
	score = score + scr;
}

void MyGame :: lshift(){ // Left shift Algorithm
	int i,j,k,scr=0;
	for(i = 0;i < 4; i++){  // Could have done this in much better ways - just a random approach
		for(k = 0; k < 3; k++){
			for(j = 0; j < 4; j++){
				if(j < 3) {
					if(m_ar[i][j] == 0 && m_ar[i][j+1] == 0){
					}
					else if((m_ar[i][j] == m_ar[i][j+1]) && m_ar[i][j] != 0){
						scr = this->tablechart(m_ar[i][j]);
						m_ar[i][j] = m_ar[i][j+1] + m_ar[i][j];
						m_ar[i][j+1] = 0;
						if(j == 0) k = k + 2;
						else if(j == 1 || j == 2) k = k + 3;}
					else{
						if(m_ar[i][j] == 0){
							m_ar[i][j] = m_ar[i][j+1];
							m_ar[i][j+1] = 0;}
						else if(m_ar[i][j+1] != 0 && m_ar[i][j] != 0){}

					}
				}
			}
		}
	}
	score = score + scr;
}
void MyGame :: addvalue(){ // Next digit to pop up in the grid - Using rand()
	int value,i,j,check=0;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			if(b_ar[i][j] != m_ar[i][j])
				check = check + 1;
		}
	}

	if(check!=0){
		value = indexrandomizer(2);
		printf("%d\n",value);
		do{
			i = indexrandomizer(4);
			j = indexrandomizer(4);		
		}while(m_ar[i][j]!=0);
		if(value==1)
			m_ar[i][j] = 4;
		else
			m_ar[i][j] = 2;
	}
}

int MyGame :: tablechart(int vl){ // A function to calculate the score 
	int multiplier;
	switch(vl){
		case 2:
			multiplier = 2 * vl;
			break;
		case 4:
			multiplier = 4 * vl;
			break;
		case 8:
			multiplier = 6 * vl;
			break;
		case 16:
			multiplier = 8 * vl;
			break;
		case 32:
			multiplier = 10 * vl;
			break;
		case 64:
			multiplier = 12 * vl;
			break;
		case 128:
			multiplier = 14 * vl;
			break;
		case 256:
			multiplier = 16 * vl;
			break;
		case 512:
			multiplier = 18 * vl;
			break;
		case 1024:
			wincheck++;
			multiplier = 20 * vl;
			break;
		case 2048:
			multiplier = 22 * vl;
			break;
		case 4096:
			multiplier = 24 * vl;
			break;
	}
	return multiplier;
}

int MyGame :: clogcheck(){ // checking for the terminal condition
	int i, j, zcheck = 0, hcheck = 0, vcheck = 0;
	for(i = 0; i < 4; i++){
		for(j = 0;j < 4; j++){
			if(m_ar[i][j] == 0)
				zcheck = zcheck + 1;
		}
	}
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			if(j < 3){
				if((m_ar[i][j] == m_ar[i][j+1]))
					hcheck = hcheck + 1;
			}}
	}
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			if(j < 3){
				if((m_ar[j][i] == m_ar[j+1][i]))
					vcheck = vcheck + 1;
			}}
	}
	if(zcheck == 0 && hcheck == 0 && vcheck == 0)
		return 0;
	else
		return 1;
}	
void MyGame :: iswin(){ // check for win
	if(wincheck==1){
		system("clear");
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		printf("%35cCONGRATS!! YOU CRACKED 2048! YOU CAN STILL PROCEED TO GET A BETTER SCORE\n\n\n",' ');
		printf("%63cPROCEED ? (y/n)",' ');
		switch(getc()){	
			case 'y': 
				wincheck++;
				break;
			case 'n':
				system("clear");
				free(name);
				this->clearFunc();
				exit(0);
				break;
			default:
				this->iswin();
		}
	}
}	

void MyGame :: clearFunc(){ // call destructor
	delete this;
}
