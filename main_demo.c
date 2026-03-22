#define UNICODE
#define _UNICODE
#define Short_Wall 36
#include <windows.h>

//Structs
typedef struct
{
    float x;
    float y;
    int active;
    
} ShortWall;

//Setting up stuff
ShortWall walls[30];
//Variables
//Remember ANYTHING is possible as long as you have A language when I mean a language ANY language similiar to C Just remember that.
int playing = 1;
void* memory_buffer; //bPtr
HDC memory_dc;
int bW = 288;
int bH = 216;

float g = 0.1635; //A little stronger than gravity by x10 realgravity is 0.1635
//X positions
float playerX = 25;
float playerY = 0;

int PplayerX = 0;
int PplayerY = 0;
//Velocites
float playerYvel = 0;
float playerXvel = 0;
//Accelerations
float playerXacc = 0;
float playerYacc = 0;
//Plr State; Values Ranging form 0-1 ints
int player_state = 0;
//Player Cooldowns & stopping double jump scaling
DWORD jumpcooldown_current = 0;
DWORD jumpcooldown_max = 250;
DWORD jumpcooldown_lastjump = 0;
int falling = 0;
int GroundOffset = 0;
int GroundOffset2 = 0;
int playerRun = 0;
int playerWalk = 0;

int colorpalette[256] = {0x000000, 0x4D4DFF, 0x004ECC, 0xFFD800, 0xFFFFFF, 0xEFC700, 0x639AFC, 0xBED4FC, 0x572903, 0xA05E12, 735005};
// 0/-1 = Black/Transparent, 1 = NESBLUE/SKY, 2 = Fur Peng, 3 = Peng Feet 1, 4 = White, 5 = Peng Feet2, 7 = Ice Blue 1, 6 = Ice Blue 2, 8 = DarkDirt, 9 = Light Dirt

//States & Stats & Words | Basically Other Section
int GameOverStartx = 112;
int GameOverY = 112;
int GameState = 0;
int TitleNameX = 144 - 39;
int Middlex = 144;
int Middley = 108;
int Rmin = 0;
int Rmax = 1;
//Bg/Tiles data
int SnowIceTile[64] = {
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    7, 4, 4, 7, 7, 4, 4, 4,
    6, 7, 6, 6, 7, 4, 6, 7,
    7, 6, 7, 6, 6, 4, 7, 6,
    7, 7, 7, 7, 6, 6, 7, 7,
    7, 7, 7, 6, 7, 7, 7, 7,
};
int SnowCoveredDirt[64] = {
    7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7,
    -1, -1, 7, 7, -1, -1, 7, 7,
    8, 8, -1, -1, 8, 8, -1, -1,
    9, 9, 8, 8, 9, 9, 8, 8,
    9, 8, 9, 9, 9, 9, 8, 9,
    9, 8, 9, 8, 8, 9, 8, 9,
    8, 9, 9, 9, 8, 9, 9, 8,
};
int IceWallLower[64] = {
    7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7,
    6, 7, 6, 7, 7, 6, 6, 6,
    7, 6, 7, 6, 6, 6, 7, 6,
    4, 7, 7, 6, 7, 6, 4, 4,
    4, 4, 4, 7, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
};
int IceWallUpper[64] = {
    0, 4, 4, 4, 4, 0, 0, 0,
    4, 4, 4, 4, 4, 4, 4, 0,
    4, 4, 6, 4, 6, 4, 4, 4,
    4, 6, 7, 6, 7, 6, 4, 4,
    6, 7, 7, 7, 7, 6, 6, 4,
    7, 7, 7, 7, 6, 7, 7, 6,
    7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7,
};
int DirtUpper[64] = {
    9, 9, 9, 8, 9, 9, 9, 9,
    8, 8, 9, 8, 8, 8, 8, 9,
    8, 9, 9, 8, 8, 9, 9, 8,
    9, 9, 8, 8, 9, -1, 8, 8,
    8, 8, 8, 8, -1, 10, 9, 8,
    8, 8, -1, 9, -1, 10, -1, 8,
    8, -1, 8, -1, 10, 10, 10, -1,
    -1, 8, 10, 8, 10, 10, 10, 10,
};
int DirtLower[64] = {
    10, 10, 10, 8, 10, 10, 8, 10,
    10, 10, 8, 8, 8, 8, 10, 8,
    8, 8, 8, 8, 10, 10, 8, 8,
    10, 8, 8, 10, 10, 8, 8, 8,
    10, 8, 10, 8, 10, 8, 10, 8,
    10, 8, 10, 10, 8, 10, 10, 8,
    10, 8, 10, 8, 10, 8, 10, 8,
    8, 10, 8, 8, 8, 10, 8, 10,
};
// < 0 = Black

//Sprite Data
int pengI[144] = {0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 2, 2, 2, 2, 0, 0,
                  0, 0, 2, 2, 2, 2, 2, 2, 0,
                  0, 0, 2, 2, -1, 2, 2, 2, 0,
                  0, 0, 2, 2, 2, 2, 3, 3, 3,
                  0, 0, 2, 2, 2, 2, 2, 2, 0,
                  0, 2, 2, 2, 4, 4, 2, 2, 0,
                  2, 2, 2, 4, 4, 4, 4, 2, 2,
                  2, 2, 2, 4, 4, 4, 4, 2, 2,
                  2, 2, 4, 4, 4, 4, 4, 2, 2,
                  2, 2, 4, 4, 4, 4, 4, 2, 0,
                  0, 2, 2, 4, 4, 4, 4, 2, 0,
                  0, 0, 2, 4, 4, 4, 2, 0, 0,
                  0, 0, 3, 3, 0, 5, 5, 0, 0};
int pengJump[144] = {
                  0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 2, 2, 2, 2, 0, 0,
                  0, 0, 2, 2, 2, 2, 2, 2, 0,
                  0, 0, 2, 2, -1, 2, 2, 2, 0,
                  0, 0, 2, 2, 2, 2, 3, 3, 3,
                  0, 0, 2, 2, 2, 2, 2, 2, 0,
                  0, 2, 2, 2, 4, 4, 2, 2, 0,
                  2, 2, 2, 4, 4, 4, 4, 2, 2,
                  2, 2, 2, 4, 4, 4, 4, 2, 2,
                  2, 2, 4, 4, 4, 4, 4, 2, 2,
                  2, 2, 4, 4, 4, 4, 4, 2, 0,
                  0, 2, 2, 4, 4, 4, 4, 2, 0,
                  0, 0, 3, 4, 4, 5, 2, 0, 0,
                  0, 0, 3, 0, 0, 5, 0, 0, 0
};
int pengW1[144] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 2, 2, -1, 2, 2, 2, 0,
    0, 0, 2, 2, 2, 2, 3, 3, 3,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 4, 4, 4, 2, 2, 0,
    2, 2, 4, 4, 4, 4, 2, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 2, 4, 4, 4, 4, 2, 0, 0,
    0, 0, 3, 3, 0, 5, 0, 0, 0,
};
int pengW2[144] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 2, 2, -1, 2, 2, 2, 0,
    0, 0, 2, 2, 2, 2, 3, 3, 3,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 0,
    2, 2, 2, 4, 4, 4, 2, 2, 2,
    2, 2, 4, 4, 4, 4, 2, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 0,
    2, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 2, 4, 4, 4, 4, 2, 0, 0,
    0, 0, 3, 3, 5, 0, 0, 0, 0,
};
int pengW3[144] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 2, 2, -1, 2, 2, 2, 0,
    0, 0, 2, 2, 2, 2, 3, 3, 3,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 4, 4, 2, 2, 0,
    2, 2, 2, 4, 4, 4, 4, 2, 2,
    2, 2, 2, 4, 4, 4, 4, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 2,
    0, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 2, 2, 4, 4, 4, 4, 2, 0,
    0, 0, 2, 4, 4, 4, 2, 5, 5,
    0, 0, 0, 3, 3, 0, 0, 0, 0,
};
int pengW4[144] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 2, 2, -1, 2, 2, 2, 0,
    0, 0, 2, 2, 2, 2, 3, 3, 3,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 4, 4, 2, 2, 0,
    2, 2, 2, 4, 4, 4, 4, 2, 2,
    2, 2, 2, 4, 4, 4, 4, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 2, 2, 4, 4, 4, 4, 2, 0,
    0, 0, 2, 4, 4, 4, 2, 0, 0,
    0, 0, 3, 3, 0, 0, 5, 5, 0,
};
int pengW5[144] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 2, 2, -1, 2, 2, 2, 0,
    0, 0, 2, 2, 2, 2, 3, 3, 3,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 4, 4, 4, 2, 2, 0,
    2, 2, 4, 4, 4, 4, 2, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 3, 4, 4, 4, 4, 2, 0, 0,
    0, 3, 0, 0, 0, 0, 5, 5, 0,
};
int pengW6[144] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 2, 2, -1, 2, 2, 2, 0,
    0, 0, 2, 2, 2, 2, 3, 3, 3,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 0,
    2, 2, 2, 4, 4, 4, 2, 2, 2,
    2, 2, 4, 4, 4, 4, 2, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 0,
    2, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 3, 3, 4, 4, 4, 2, 0, 0,
    0, 0, 0, 0, 0, 5, 5, 0, 0,
};
int pengW7[144] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 2, 2, -1, 2, 2, 2, 0,
    0, 0, 2, 2, 2, 2, 3, 3, 3,
    0, 0, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 4, 4, 2, 2, 0,
    2, 2, 2, 4, 4, 4, 4, 2, 2,
    2, 2, 2, 4, 4, 4, 4, 2, 2,
    2, 2, 4, 4, 4, 4, 4, 2, 2,
    0, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 2, 4, 4, 4, 4, 4, 2, 0,
    0, 2, 2, 4, 4, 4, 4, 2, 0,
    0, 0, 2, 3, 3, 4, 2, 0, 0,
    0, 0, 0, 0, 0, 5, 5, 0, 0,
};

int* pengWalkCycle[] = {pengI, pengW1, pengW2, pengW3, pengW4, pengW5, pengW6, pengW7, pengJump};
int pengWCInd = 0;
int pengLength = sizeof(pengWalkCycle) / sizeof(pengWalkCycle[0]);
DWORD penganim_timer = 0;


//DAFont
int letterA[64] = {0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 4, 4, 4, 4, 0, 0,
                   0, 4, 4, 4, 4, 4, 4, 0,
                   0, 4, 4, 0, 0, 4, 4, 0,
                   0, 4, 4, 4, 4, 4, 4, 0,
                   0, 4, 4, 0, 0, 4, 4, 0,
                   0, 4, 4, 0, 0, 4, 4, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,};
int letterB[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 4, 4, 0, 0, 4, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 4, 4, 0, 0, 4, 0,
    0, 0, 4, 4, 0, 0, 4, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterC[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterD[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 4, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterE[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterF[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterG[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 0, 4, 4, 4, 4, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterH[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterI[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterJ[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 4, 4, 4, 0,
    0, 0, 0, 0, 0, 4, 4, 0,
    0, 0, 0, 0, 0, 4, 4, 0,
    0, 0, 4, 0, 0, 4, 4, 0,
    0, 0, 4, 4, 4, 4, 4, 0,
    0, 0, 0, 4, 4, 4, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterK[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 4, 4, 0, 0,
    0, 4, 4, 4, 4, 0, 0, 0,
    0, 4, 4, 0, 4, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterL[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 0, 0, 0, 
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterM[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 0, 4, 0, 0,
    0, 4, 4, 4, 0, 4, 4, 0,
    0, 4, 4, 0, 4, 0, 4, 0,
    0, 4, 4, 0, 4, 0, 4, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterN[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 0, 0, 4, 0,
    0, 4, 4, 4, 0, 0, 4, 0,
    0, 4, 4, 0, 4, 0, 4, 0,
    0, 4, 4, 0, 4, 0, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 
};
// O is special as to save space we resuse as 0
int letterO[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterP[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 4, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 4, 4, 4, 0, 0,
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterQ[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 0,
    0, 4, 4, 0, 0, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 0, 0,
    0, 4, 4, 0, 4, 4, 0, 0,
    0, 0, 4, 4, 4, 4, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterR[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 4, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 4, 4, 4, 0, 0,
    0, 4, 4, 4, 4, 0, 0, 0,
    0, 4, 4, 0, 4, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterS[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 0,
    0, 4, 4, 0, 0, 4, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 0, 0, 0, 4, 0, 0, 0,
    0, 4, 0, 0, 4, 4, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterS2[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 0, 0, 0, 0, 4, 4, 0,
    0, 0, 0, 0, 0, 4, 4, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterT[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0, 
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterU[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 4, 4, 0, 0, 4, 4, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterV[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 0, 4, 4, 0, 4, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterW[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4 ,4, 0, 0, 0, 4, 0,
    0, 4, 4, 0, 0, 0, 4, 0,
    0, 4, 4, 0, 4, 0, 4, 0,
    0, 4, 4, 0, 4, 0, 4, 0,
    0, 4, 4, 4, 0, 4, 4, 0,
    0, 0, 4, 4, 0, 4, 0, 0,
};
int letterX[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4 ,4, 0, 0, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 0,
    0, 4, 4, 0, 0, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterY[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 4, 0, 0,
    0, 4, 4, 0, 0, 4, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int letterZ[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 0, 0, 0, 4, 4, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int number1[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int number2[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 4, 0, 0, 4, 0, 0,
    0, 0, 0, 0, 4, 0, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int number3[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 4, 0, 4, 4, 0, 0,
    0, 0, 0, 0, 4, 0, 0, 0,
    0, 0, 0, 0, 4, 4, 0, 0,
    0, 0, 4, 0, 4, 4, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int number4[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 0, 0, 4, 4, 0, 0,
    0, 4, 0, 0, 4, 4, 0, 0,
    0, 4, 0, 0, 4, 4, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 4, 4, 0, 0,
    0, 0, 0, 0, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int number5[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 4, 0, 0,
    0, 0, 0, 0, 0, 4, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,

};
int number6[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 4, 4, 0, 4, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 0, 0, 0,
    0, 0, 4, 4, 0, 4, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int number7[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 4, 4, 4, 0,
    0, 0, 0, 0, 0, 4, 4, 0,
    0, 0, 0, 0, 4, 4, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 4, 4, 0, 0, 0, 0,
    0, 4, 4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int number8[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 4, 0, 4, 4, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 4, 0, 4, 4, 0, 0,
    0, 0, 4, 0, 4, 4, 0, 0,
    0, 0, 0, 4, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int number9[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 4, 4, 0, 4, 4, 0, 0,
    0, 0, 4, 4, 4, 4, 0, 0,
    0, 0, 0, 0, 4, 4, 0, 0,
    0, 0, 0, 0, 4, 4, 0, 0,
    0, 0, 0, 0, 4, 4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

void Draw8x8(int *pixela, int manualx, int manualy, int array8x8[]) {
    int offy = 0;
    int offx = 0;
    for(int i = 0; i < 64; i++) {
        int pxPosY = offy + manualy;
        int pxPosX = manualx + offx;
        if(pxPosX < 288 && pxPosX >= 0 && pxPosY < 216 && pxPosY >= 0){
            int coord = ((pxPosY) * 288) + (pxPosX);

            if(coord <= (62208) && coord >= 0 && array8x8[i] != 0 && array8x8[i] != -1){
                pixela[coord] = colorpalette[array8x8[i]];
            }else if(((offy + manualy) * 288) + (manualx + offx) && array8x8[i] == -1){
                pixela[coord] = colorpalette[array8x8[0]];
            };
        }
        
        offx++;
        if(offx >= 8){
            offx = 0;
            offy++;
        };
    };
}
// This does not manage pengWCInd index for ptr
void DrawPlr(int *pixela, int x, int y, int array16x9or16x16[]){
    int offx = 0;
    int offy = 0;
    if(player_state == 0) {
        //remeber Init, Condition, Increment
        for(int i = 0; i < 144; i++) {
            int pxPosY = offy + y;
            int pxPosX = offx + x;
            if(pxPosX >= 0 && pxPosX < 288 && pxPosY >= 0 && pxPosY < 216){
                int coord = ((pxPosY) * 288) + (pxPosX);
                if(coord <= (62208) && coord >= 0 && array16x9or16x16[i] != 0 && array16x9or16x16[i] != -1){
                    pixela[coord] = colorpalette[array16x9or16x16[i]];
                } else if(array16x9or16x16[i] == -1 && array16x9or16x16[i] != 0){
                    pixela[coord] = colorpalette[0];
                };
            };
            offx++;
            if(offx >= 9){
                offx = 0;
                offy++;
            };
        };
    };
    if(player_state > 0) {
        for(int i = 0; i < 256; i++) {
            int pxPosY = offy + y;
            int pxPosX = offx + x;
            if(pxPosX >= 0 && pxPosX < 288 && pxPosY >= 0 && pxPosY < 216){
                int coord = ((pxPosY) * 288) + (pxPosX);
                if(coord <= (62208) && coord >= 0 && array16x9or16x16[i] != 0 && array16x9or16x16[i] != -1){
                    pixela[coord] = colorpalette[array16x9or16x16[i]];
                } else if(array16x9or16x16[i] == -1 && array16x9or16x16[i] != 0){
                    pixela[coord] = colorpalette[0];
                };
            };
            offx++;
            if(offx >= 16){
                offx = 0;
                offy++;
            };
        };
    };
};

LRESULT CALLBACK WindowProcessMessage(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow){
    WNDCLASS window_class = {0};
    const wchar_t window_class_name[] = L"My Window Class";
    window_class.lpszClassName = window_class_name;
    window_class.lpfnWndProc = WindowProcessMessage;
    window_class.hInstance = hInstance;
    window_class.hCursor = LoadCursor (NULL, IDC_ARROW);

    RegisterClass(&window_class);

    int width = 1440;
    int height = 1080;

    HWND window_handle = CreateWindow(window_class_name, L"Penguin. RUN, RUN, RUN!!!", WS_POPUP | WS_VISIBLE, 0, 0, width, height, NULL, NULL, hInstance, NULL);
    SetWindowPos(window_handle, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
    if(window_handle == NULL) {return -1;};

    HDC hdc = GetDC(window_handle);
    memory_dc = CreateCompatibleDC(hdc);

    BITMAPINFO bitmap_info = {0};
    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
    bitmap_info.bmiHeader.biWidth = bW;
    bitmap_info.bmiHeader.biHeight = - bH;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;

    HBITMAP bitmap_handle = CreateDIBSection(memory_dc, &bitmap_info, DIB_RGB_COLORS, &memory_buffer, NULL, 0);
    SelectObject(memory_dc, bitmap_handle);
    ReleaseDC(window_handle, hdc);
    ShowWindow(window_handle, nCmdShow);
// Main Loop
    //Early Variables
    DWORD CooldownJ2 = 0;
    int anim_cooldown = 35;
    int slowdown_wait = 150;
    DWORD last_tick = 0;
    int random = 0;
    DWORD lastrandomcheck = 0;
    int last_pixelmove = 0;
    int current_pixelmove = 0;

    //Setting things up
    for(int i = 0; i < 30; i++){
        walls[i].x = 0;
        walls[i].y = 0;
        walls[i].active = 0;
    }

    //Actaull Loop
    while(playing){
        MSG message;
        while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&message);
            DispatchMessage(&message);
        };
        if(GroundOffset <= -288){
            GroundOffset = 288;
        }

        playerWalk = 0;
        playerRun = 0;
        falling = 0;
        jumpcooldown_current = GetTickCount();
        
        if(playerY < 168) {
            playerYvel += g;
            falling = 1;
        }
//Plr Movement
        if((GetAsyncKeyState(VK_SPACE) & 0x8000) && jumpcooldown_current - jumpcooldown_lastjump >= 150 && !falling) {
            playerYvel = -3.1415;
            jumpcooldown_lastjump = GetTickCount();
        };

        if((GetAsyncKeyState(0x44) & 0x8000) && !falling && jumpcooldown_current - penganim_timer >= anim_cooldown && !playerRun) {
            pengWCInd += 1;
            penganim_timer = GetTickCount();
            if(pengWCInd == pengLength - 2) {
                pengWCInd = 0;
            }
            playerXvel = 2;
            playerWalk = 1;
        } else if(jumpcooldown_current - penganim_timer >= (anim_cooldown + 5) && playerYvel < 0) {
            pengWCInd = pengLength - 1;
        } else if(jumpcooldown_current - penganim_timer >= (anim_cooldown + 5)){
            pengWCInd = 0;
        };

        //Checking for spawning of walls
        int randomBi = jumpcooldown_current % 8;
        int ablespawn = 1;
        for(int i = 0; i < 30; i++) {
            if(walls[i].active){
                if (walls[i].x >= (288)){
                    ablespawn = 0;
                }
                walls[i].x -= playerXvel;
            }
        }
        for(int i = 0; i < 30; i++) {
            if(!walls[i].active && ablespawn) {
                walls[i].x = 288;
                walls[i].y = 168;
                walls[i].active = 1;
                break;
            }
            if(walls[i].active && walls[i].x < 0) {
                walls[i].active = 0;
            }
        }

        playerY += playerYvel;
        GroundOffset -= playerXvel;
        current_pixelmove += playerXvel;

        if(!playerWalk && !playerRun && playerXvel > 0 && jumpcooldown_current - last_tick > slowdown_wait) {
            playerXvel -= 1;
            if(playerXvel < 0) {
                playerXvel = 0;
            }
            last_tick = GetTickCount();
        }

        if(playerY >= 168){
            if(falling){jumpcooldown_lastjump = GetTickCount();}
            playerY = 168;
            playerYvel = 0;
            falling = 0;
        };
        
        int PplayerX = (int)(playerX + 0.5);
        int PplayerY = (int)(playerY + 0.5);
//Render
        // Renderer
        Sleep(16.6666666667); //60Frame Cap
        unsigned int* pixel = (unsigned int*)memory_buffer;
        // Screen Reset
        for(int i = 0; i < (bW * bH); i++ ) {
            pixel[i] = colorpalette[1]; //1 = Blue. Blue = Sky = Reset
        };
        GameState = 1;
        if(GameState == 0) {
            int GameStartY = GameOverY - 50;
            int GameStartGameY = 112;
            int GameStartGameX = Middlex - 32;
            Draw8x8(pixel, TitleNameX, GameStartY, letterP);
            Draw8x8(pixel, TitleNameX + 7, GameStartY, letterE);
            Draw8x8(pixel, TitleNameX + 15, GameStartY, letterN);
            Draw8x8(pixel, TitleNameX + 23, GameStartY, letterG);
            Draw8x8(pixel, TitleNameX + 31, GameStartY, letterU);
            Draw8x8(pixel, TitleNameX + 38, GameStartY, letterI);
            Draw8x8(pixel, TitleNameX + 46, GameStartY, letterN);
            Draw8x8(pixel, TitleNameX + 62, GameStartY, letterR);
            Draw8x8(pixel, TitleNameX + 70, GameStartY, letterU);
            Draw8x8(pixel, TitleNameX + 78, GameStartY, letterN);

            Draw8x8(pixel, TitleNameX, GameStartGameY, letterS2);
            Draw8x8(pixel, TitleNameX + 8, GameStartGameY, letterT);
            Draw8x8(pixel, TitleNameX + 16, GameStartGameY, letterA);
            Draw8x8(pixel, TitleNameX + 24, GameStartGameY, letterR);
            Draw8x8(pixel, TitleNameX + 32, GameStartGameY, letterT);
            Draw8x8(pixel, TitleNameX + 40, GameStartGameY, letterG);
            Draw8x8(pixel, TitleNameX + 48, GameStartGameY, letterA);
            Draw8x8(pixel, TitleNameX + 56, GameStartGameY, letterM);
            Draw8x8(pixel, TitleNameX + 64, GameStartGameY, letterE);
        };

        if(GameState == 1) {
            if (player_state == 0){
                DrawPlr(pixel, PplayerX, PplayerY, pengWalkCycle[pengWCInd]);
            }
            int TileA = 288 + GroundOffset;
            int TileN = 0 + GroundOffset;
            int TileB = -288 + GroundOffset;
            //Remember Init, Condition, Inc
            for(int i = 0; i < 36; i++){
                // Screen, X, Y, Image
                Draw8x8(pixel, TileN, 184, SnowIceTile);
                Draw8x8(pixel, TileA, 184, SnowIceTile);
                Draw8x8(pixel, TileB, 184, SnowIceTile);
                TileN += 8;
                TileA += 8;
                TileB += 8;
            }
            TileN = 0 + GroundOffset;
            TileA = 288 + GroundOffset;
            TileB = -288 + GroundOffset;
            for(int i = 0; i < 36; i++){
                // Screen, X, Y, Image
                Draw8x8(pixel, TileN, 192, SnowCoveredDirt);
                Draw8x8(pixel, TileA, 192, SnowCoveredDirt);
                Draw8x8(pixel, TileB, 192, SnowCoveredDirt);
                TileN += 8;
                TileA += 8;
                TileB += 8;
            }
            TileN = 0 + GroundOffset;
            TileA = 288 + GroundOffset;
            TileB = -288 + GroundOffset;
            for(int i = 0; i < 36; i++){
                // Screen, X, Y, Image
                Draw8x8(pixel, TileN, 200, DirtUpper);
                Draw8x8(pixel, TileA, 200, DirtUpper);
                Draw8x8(pixel, TileB, 200, DirtUpper);
                TileN += 8;
                TileA += 8;
                TileB += 8;
            }
            TileN = 0 + GroundOffset;
            TileA = 288 + GroundOffset;
            TileB = -288 + GroundOffset;
            for(int i = 0; i < 36; i++){
                // Screen, X, Y, Image
                Draw8x8(pixel, TileN, 208, DirtLower);
                Draw8x8(pixel, TileA, 208, DirtLower);
                Draw8x8(pixel, TileB, 208, DirtLower);
                TileN += 8;
                TileA += 8;
                TileB += 8;
            }
            for(int i = 0; i > 30; i++) {
                if(walls[i].active) {
                    Draw8x8(pixel, walls[i].x, walls[i].y, IceWallUpper);
                    Draw8x8(pixel, walls[i].x, walls[i].y, IceWallLower);
                }
            }
        }

        if(GameState == 2) {
            Draw8x8(pixel, GameOverStartx, GameOverY, letterG);
            Draw8x8(pixel, GameOverStartx + 8, GameOverY, letterA);
            Draw8x8(pixel, GameOverStartx + 16, GameOverY, letterM);
            Draw8x8(pixel, GameOverStartx + 23, GameOverY, letterE);
            Draw8x8(pixel, GameOverStartx + 39, GameOverY, letterO);
            Draw8x8(pixel, GameOverStartx + 47, GameOverY, letterV);
            Draw8x8(pixel, GameOverStartx + 55, GameOverY, letterE);
            Draw8x8(pixel, GameOverStartx + 63, GameOverY, letterR);
        };

        HDC hdcWindow = GetDC(window_handle);

        StretchBlt(hdcWindow, 0, 0, 1440, 1080, memory_dc, 0, 0, bW, bH, SRCCOPY);
        ReleaseDC(window_handle, hdcWindow);
        //Bcs I am so tired, a check.
        if(pengWCInd > 7) {
            pengWCInd = 0;
        };
    };
//Resolution of Game = 288x216
//This uses RGB no AA included
    return 0;
};

LRESULT CALLBACK WindowProcessMessage(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam){
    switch(message){
        case WM_QUIT:
        case WM_DESTROY: {
            playing = 0;
        } break;

        default: { //Message failure to handle
            return DefWindowProc(window_handle, message, wParam, lParam);
        } break;
    };
    return 0;
};
