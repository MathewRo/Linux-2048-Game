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

char getc();
time_t t;
int rs, pileup, wincheck = 0;
char *name;	
int m_ar[4][4];
int b_ar[4][4];
int score;
int pscore;
char press;


/**
 * 
 * @param val1
 * @param val2
 */
void MyGame(int val1, int val2){
    score = val1;
    pscore = val2;
    int i,j;
    for (i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            m_ar[i][j] = b_ar[i][j]=0;
}

/**
 * 
 * @param index
 * @return 
 */
int indexrandomizer(int index){ 
    int pass;
    pass = rand() % index + 0;
    return pass;
}

/**
 * 
 */
void initializegrid(){ // initial grid setup
    int i, j, m, n;
    i = indexrandomizer(4);
    j = indexrandomizer(4);
    m_ar[i][j] = 2;
    do{
        m = indexrandomizer(4);
        n = indexrandomizer(4);
    }while(m == i && n == j);
    m_ar[m][n] = 2;
}

/**
 * 
 * @param flick
 */
void addflicker(int flick){
    usleep(flick);
} 

/**
 * 
 */
void chmode(){ // Game startup 
    system("clear");
    std::string content;
    std::ifstream startup("2048.txt");
    if (startup.fail()) return;
    while(getline(startup, content))	
        std::cout << content << std::endl;
    addflicker(3000000);
    system("clear");
    std::cout << "Enter User Name : " ;
    std::cin >> name;
    addflicker(1000000);
}

/**
 * 
 */
void endascii(){ // GAMEOVER ascii art
    system("clear");
    std::string content;
    std::ifstream endmode("2047.txt");
    if (endmode.fail()) return;
    while(getline(endmode, content))
        std::cout << content << std::endl;
    addflicker(4000000);
}

/**
 * 
 */
void backup(){
    int i, j;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            b_ar[i][j] = m_ar[i][j];
        }
    }
}

/**
 * 
 */
void projector(){ // fuction to project the entire grid onto the console
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

/**
 * 
 * @param vl
 * @return 
 */
int tablechart(int vl){ // A function to calculate the score 
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

/**
 * 
 * @return 
 */
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

/**
 * 
 */
void ushift(){ // Up shift Algorithm 
    int i,j,k,scr=0;
    for(i = 0; i < 4; i++){  // Could have done this in much better ways - just a random approach
        for(k = 0; k < 3; k++){	
            for(j = 0; j < 4; j++){
                if(j < 3)	{	
                    if(m_ar[j][i] == 0 && m_ar[j+1][i] == 0){
                    }
                    else if(m_ar[j][i] == m_ar[j+1][i] && m_ar[j][i] != 0){
                        scr = tablechart(m_ar[j][i]);
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

/**
 * 
 * 
 */
void dshift(){ // Down shift Algorithm
    int i,j,k,scr=0; 
    for(i = 0; i < 4; i++){  // Could have done this in much better ways - just a random approach 
        for(k = 0; k < 3; k++){
            for(j = 3; j >= 0; j--){
                if(j > 0) {
                    if(m_ar[j][i] == 0 && m_ar[j-1][i] == 0){
                    }
                    else if((m_ar[j][i] == m_ar[j-1][i]) && m_ar[j][i] != 0){
                        scr = tablechart(m_ar[j][i]);
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

/**
 * 
 */
void rshift(){ // Right shift Algorithm
    int i,j,k,scr=0;
    for(i = 0;i < 4; i++){  // Could have done this in much better ways - just a random approach
        for(k = 0; k < 3; k++){
            for(j = 3; j >= 0; j--){
                if(j > 0) {
                    if(m_ar[i][j] == 0 && m_ar[i][j-1] == 0){
                    }
                    else if((m_ar[i][j] == m_ar[i][j-1]) && m_ar[i][j] != 0){
                        scr = tablechart(m_ar[i][j]);
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

/**
 * 
 */
void lshift(){ // Left shift Algorithm
    int i,j,k,scr=0;
    for(i = 0;i < 4; i++){  // Could have done this in much better ways - just a random approach
        for(k = 0; k < 3; k++){
            for(j = 0; j < 4; j++){
                if(j < 3) {
                    if(m_ar[i][j] == 0 && m_ar[i][j+1] == 0){
                    }
                    else if((m_ar[i][j] == m_ar[i][j+1]) && m_ar[i][j] != 0){
                        scr = tablechart(m_ar[i][j]);
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

/**
 * 
 */
void tapkey(){
    press = getc();
    switch(press){
        case K_UP:
            ushift();		
            break;
        case K_DOWN:
            dshift();
            break;
        case K_LEFT:
            lshift();
            break;
        case K_RIGHT:
            rshift();
            break;
        default:
            tapkey();
            break;
    }
}

/**
 * 
 */
void addvalue(){ // Next digit to pop up in the grid - Using rand()
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

/**
 * 
 * @return 
 */
int clogcheck(){ // checking for the terminal condition
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

/**
 * 
 */
void iswin(){ // check for win
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
                exit(0);
                break;
            default:
                iswin();
        }
    }
}	

/**
 * 
 * @return 
 */
int main(){
    std::ios_base::sync_with_stdio(false);
    MyGame(0, 0);
    srand(time(&t));
    name  = (char*)malloc(100);
    initializegrid();
    chmode(); 
    do{
        backup();
        projector();
        tapkey();
        projector();
        addflicker(200000);
        addvalue();	
        iswin();
    }while(clogcheck());
    projector();
    addflicker(4000000);	
    rs = indexrandomizer(25);
    endascii();
    return 0;
}
