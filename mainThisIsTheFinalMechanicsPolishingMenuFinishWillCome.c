#define UNICODE
#define _UNICODE
#define Short_Wall 36
#include <windows.h>
 
//Structs
typedef struct {
    float x;
    float y;
    int active;
 
}ShortWall;
 
 
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
int backward = 0;
int forward = 0;
//Velocites
float playerYvel = 0;
float playerXvel = 0;
//Accelerations
float playerXacc = 0;
float playerYacc = 0;
float playermaxacc = 0.5;
float playermaxvel = 2.9;
float playermaxvelWalk = 1.8;
float playermaxaccWalk = 0.4;
float Predator_dist = 0;
//Plr State; Values Ranging form 0-1 ints
int player_state = 0;
float snow_friction = 0.9;
//Player Cooldowns & stopping double jump scaling
DWORD jumpcooldown_current = 0;
DWORD jumpcooldown_max = 250;
DWORD jumpcooldown_lastjump = 0;
int falling = 0;
int GroundOffset = 0;
int GroundOffset2 = 0;
int playerRun = 0;
int playerWalk = 0;
int enemyvel = 0;
 
int colorpalette[256] = {0x000000, 0x4D4DFF, 0x004ECC, 0xFFD800, 0xFFFFFF, 0xEFC700, 0x639AFC, 0xBED4FC, 0x572903, 0xA05E12, 0x0B371D, 0x735005, 0xFF8707, 0x724A27, 0x562E02};
// 0/-1 = Black/Transparent, 1 = NESBLUE/SKY, 2 = Fur Peng, 3 = Peng Feet 1, 4 = White, 5 = Peng Feet2, 7 = Ice Blue 1, 6 = Ice Blue 2, 8 = DarkDirt, 9 = Light Dirt
//No.12 = Orange
//No. 13 = Brown
// No. 14 = dark brown
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
int Efox[256] = {
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 12, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 12, -1, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   -1, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   4, 4, 4, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0,
   0, 0, 0, 4, 4, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0,
   0, 0, 0, 0, 4, 4, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
   0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 12, 0, 0, 4,
   0, 0, 0, 0, 0, 13, 0, 13, 0, 0, 14, 0, 14, 0, 0, 0,
   0, 0, 0, 0, 0, 13, 0, 13, 0, 0, 14, 0, 14, 0, 0, 0,
};
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
 
            if(coord < (62208) && coord >= 0 && array8x8[i] > 0){
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
                if(coord < (62208) && coord >= 0 && array16x9or16x16[i] > 0){
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
                if(coord < (62208) && coord >= 0 && array16x9or16x16[i] > 0){
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
void DrawPlrFlip(int *pixela, int x, int y, int array16x9or16x16[]){
    int offx = 8;
    int offy = 0;
    if(player_state == 0) {
        //remeber Init, Condition, Increment
        for(int i = 0; i < 144; i++) {
            int pxPosY = offy + y;
            int pxPosX = x + offx;
            if(pxPosX >= 0 && pxPosX < 288 && pxPosY >= 0 && pxPosY < 216 && offx >= 0){
                int coord = ((pxPosY) * 288) + (pxPosX);
                if(coord < (62208) && coord >= 0 && array16x9or16x16[i] > 0){
                    pixela[coord] = colorpalette[array16x9or16x16[i]];
                } else if(array16x9or16x16[i] == -1 && array16x9or16x16[i] != 0){
                    pixela[coord] = colorpalette[0];
                };
            };
            offx--;
            if(offx < 0){
                offx = 8;
                offy++;
            };
        };
    };
    if(player_state > 0) {
        int offx = 15;
        int offy = 0;
        for(int i = 0; i < 256; i++) {
            int pxPosY = offy + y;
            int pxPosX = offx + x;
            if(pxPosX >= 0 && pxPosX < 288 && pxPosY >= 0 && pxPosY < 216 && offx >= 0){
                int coord = ((pxPosY) * 288) + (pxPosX);
                if(coord < (62208) && coord >= 0 && array16x9or16x16[i] != 0 && array16x9or16x16[i] != -1){
                    pixela[coord] = colorpalette[array16x9or16x16[i]];
                } else if(array16x9or16x16[i] == -1 && array16x9or16x16[i] != 0){
                    pixela[coord] = colorpalette[0];
                };
            };
            offx--;
            if(offx < 0){
                offx = 15;
                offy++;
            };
        };
    };
};
int my_round(float x){
    if(x < 0){
       return (int)(x - 0.5);
    } else if(x > 0){
       return (int)(x + 0.5);
    } else{ return 0;}
}
int collision_detection(float px, float py, float pw, float ph, float wx, float wy, float ww, float wh){
    if(px < wx + ww && py < wy + wh && py + ph > wy && px + pw > wx){
        return 1;
    } return 0;
}
void ConvertNumToFont(int x, int y, int *pixela, char STRING[]) {
    int LENGTH = lstrlenA(STRING);
    int newx = x;
    int newy = y;
    for(int i = 0; i < LENGTH; i++){
        char current = STRING[i];
        int* ArrayNum = 0;
        if(current == '0'){
            ArrayNum = letterO;
        }
        else if(current == '1'){
            ArrayNum = number1;
        }
        else if(current == '2'){
            ArrayNum = number2;
        }
        else if(current == '3'){
            ArrayNum = number3;
        }
        else if(current == '4'){
            ArrayNum = number4;
        }
        else if(current == '5'){
            ArrayNum = number5;
        }
        else if(current == '6'){
            ArrayNum = number6;
        }
        else if(current == '7'){
            ArrayNum = number7;
        }
        else if(current == '8'){
            ArrayNum = number8;
        }
        else if(current == '9'){
            ArrayNum = number9;
        }
        else {ArrayNum = letterA;}
        Draw8x8(pixela, newx, newy, ArrayNum);
        newx += 8;
    }
}
 
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
 
    HWND window_handle = CreateWindow(window_class_name, L"2 Game Collection", WS_POPUP | WS_VISIBLE, 0, 0, width, height, NULL, NULL, hInstance, NULL);
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
    playerY = 20;
    int anim_cooldown = 30;
    int slowdown_wait = 150;
    DWORD last_tick = 0;
    int random = 0;
    DWORD lastrandomcheck = 0;
    int last_pixelmove = 0;
    float current_pixelmove = 0;
    int anim_cooldown_run = 20;
    DWORD last_check_time = GetTickCount();
    int SpawnMaxLimit = 8000;
    int can_moveX = 1;
    int playerW = 0;
    int playerH = 16;
    int OnGround = 0;
    int CollisionBefore = 0;
    DWORD CooldownSlide = 0;
    enemyvel = 3;
    Predator_dist = -1000;
 
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
                GroundOffset = 0;
            }
            can_moveX = 1;
        if(GameState == 1){
    //Resetting variables
            CollisionBefore = 0;
            playerWalk = 0;
            playerRun = 0;
            playerXacc = 0;
            falling = 0;
            jumpcooldown_current = GetTickCount();
            backward = 0;
            forward = 0;
            if(player_state == 0) {
                playerW = 9;
                playerH = 16;
            }else{playerW = 16;
                playerH = 9;}
            if(playerY >= 168) {
                OnGround = 1;
                playerY = 168;
            }
        //Gravity
            if(!OnGround) {
                playerYvel += g;
                falling = 1;
            }
 
    //Plr Movement
        // Sliding
            if((GetAsyncKeyState(0x53) & 0x8000) && OnGround && jumpcooldown_current - CooldownSlide >= 150) {
                player_state = 1;
                playerXvel = 0;
                CooldownSlide = GetTickCount();
            }
            if((GetAsyncKeyState(0x57) & 0x8000) && OnGround) {
                player_state = 0;
            }
        //Jumping
            if((GetAsyncKeyState(VK_SPACE) & 0x8000) && jumpcooldown_current - jumpcooldown_lastjump >= 150 && OnGround && !player_state) {
                playerYvel = -3.1415;
                jumpcooldown_lastjump = GetTickCount();
                pengWCInd = pengLength - 1;
                OnGround = 0;
            };
            //Running
            if((GetAsyncKeyState(VK_LSHIFT) & 0x8000) && jumpcooldown_current - penganim_timer >= 30 && OnGround && !player_state) {
                pengWCInd += 1;
                penganim_timer = GetTickCount();
                if(pengWCInd >= pengLength - 2) {
                    pengWCInd = 0;
                };
 
                playerRun = 1;
                playerXacc += 5;
            }
        //Going Backwards
            if((GetAsyncKeyState(0x41) & 0x8000) && jumpcooldown_current - penganim_timer >= 30 && !playerRun && OnGround && !player_state) {
                pengWCInd += 1;
                penganim_timer = GetTickCount();
                if(pengWCInd >= pengLength - 2) {
                    pengWCInd = 0;
                };
 
                playerXacc = -4;
                playerWalk = 1;
                backward = 1;
                penganim_timer = GetTickCount();
            }
            // Walking
            if((GetAsyncKeyState(0x44) & 0x8000) && jumpcooldown_current - penganim_timer >= anim_cooldown && !backward && !playerRun && OnGround  && !player_state) {
                pengWCInd += 1;
                penganim_timer = GetTickCount();
                if(pengWCInd >= pengLength - 2) {
                    pengWCInd = 0;
                };
 
                playerXacc = 4;
                playerWalk = 1;
                forward = 1;
                penganim_timer = GetTickCount();
            }
            if(jumpcooldown_current - penganim_timer >= (anim_cooldown + 5) && playerYvel < 0) {
                pengWCInd = pengLength - 1;
            } else if(jumpcooldown_current - penganim_timer >= (anim_cooldown + 5)){
                pengWCInd = 0;
            };
            if(player_state){
                playerXacc = 2.5;
                playerXvel += playerXacc;
                if(playerXvel >= 8) {playerXvel = 8;}
            }
        // Maxxing Vel & Acc
            if(can_moveX  && !player_state){
                playerXvel += playerXacc;}
        // APPLYING Friction
            if(playerXvel >= snow_friction && !falling) {
                playerXvel -= (snow_friction);
            } else if(playerXvel <= -(snow_friction) && !falling) {
                playerXvel += (snow_friction);
            }
        //Absolute MAximum if walking fails
            if(playerXvel >= playermaxvel  && !player_state) {
                playerXvel = playermaxvel;
            }
            if(playerXvel <= -(playermaxvel) && !player_state) {
                playerXvel = -(playermaxvel);
            }
 
            if(playerXacc >= playermaxacc && !player_state) {
                playerXacc = playermaxacc;
            }
            if(playerXacc <= -(playermaxacc) && !player_state) {
                playerXacc = -(playermaxacc);
            }
 
            //Walking max
            if(playerXvel >= playermaxvelWalk && playerWalk == 1) {
                playerXvel = playermaxvelWalk;
            }
            if(playerXacc >= playermaxaccWalk && playerWalk == 1) {
                playerXacc = playermaxaccWalk;
            }
            if(playerXvel <= -(playermaxvelWalk) && playerWalk == 1) {
                playerXvel = -(playermaxvelWalk);
            }
            if(playerXacc <= -(playermaxaccWalk) && playerWalk == 1) {
                playerXacc = -(playermaxaccWalk);
            }
            if(!can_moveX && !player_state){
                playerXvel = 0;
                playerXacc = 0;
            }
 
    //Checking for spawning of walls
 
        //Ground
            GroundOffset -= playerXvel;
        //Total Disance Traveled
            current_pixelmove += playerXvel;
            if(current_pixelmove < 0) {current_pixelmove = 0;}
        //Wall Stuff
            int randomBi = jumpcooldown_current % 8;
            int ablespawn = 1;
            int clear_space = 1;
        //Wall Loops
 
            for(int i = 0; i < 30; i++) {
                if(walls[i].x >= (280 + current_pixelmove) && walls[i].active) {
                    clear_space = 0;
                }
                if(walls[i].active && walls[i].x >= (current_pixelmove + 288)){
                    ablespawn = 0;
                }   
            }
            if(jumpcooldown_current - last_check_time <= SpawnMaxLimit) {
                ablespawn = 0;
            }
        //Spawining first active
            if(randomBi >= 6) {
                ablespawn = 0;
            }
            for(int i = 0; i < 30; i++) {
                if(!walls[i].active && ablespawn && clear_space) {
                    walls[i].x = current_pixelmove + 288;
                    walls[i].y = 168;
                    walls[i].active = 1;
                    last_check_time = GetTickCount();
                    SpawnMaxLimit -= 50;
                    break;
                }
                if(walls[i].active && walls[i].x < (current_pixelmove - 8)) {
                    walls[i].active = 0;
                }
            }
            if(SpawnMaxLimit <= 0) {
                SpawnMaxLimit = 8000;
            }
 
            playerY += playerYvel;
 
            Predator_dist += enemyvel;
            Predator_dist -= playerXvel;
    //PlayerSlowdown
            if(!playerWalk && !playerRun && playerXvel > 0 && jumpcooldown_current - last_tick > slowdown_wait) {
                playerXvel -= 0.6;
                if(playerXvel < 0) {
                    playerXvel = 0;
                }
                last_tick = GetTickCount();
            }
            if(!playerWalk && !playerRun && playerXvel < 0 && jumpcooldown_current - last_tick > slowdown_wait) {
                playerXvel += 0.6;
                if(playerXvel > 0) {
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
            if(Predator_dist >= 5) {
                GameState = 2;
            }
            //Collisions
 
            for(int i = 0; i < 30; i++) {
                if(collision_detection(playerX + 1, playerY + 1, playerW - 1, playerH - 2, (walls[i].x - current_pixelmove), 168, 8, 16) && !player_state) {
                    if(walls[i].active){
                        if(playerXvel > 0) {
                            current_pixelmove = (walls[i].x) - (playerX + playerW);
                            CollisionBefore = 1;
                        } else if(playerXvel < 0) {
                            current_pixelmove = (walls[i].x + 8) - playerX;
                            CollisionBefore = 1;
                        }
                    }
                }
                if(collision_detection(playerX + 1, playerY, playerW - 2, playerH, (walls[i].x - current_pixelmove), 168, 8, 16) && falling && !player_state) {
                    if(walls[i].active){
                        if(playerYvel > 0) {
                            playerY = 168 - playerH;
                            playerYvel = 0;
                            falling = 0;
                            OnGround = 1;
                            CollisionBefore = 1;
                        }
                    }
                }
                if(collision_detection(playerX, playerY, playerW, playerH, (walls[i].x - current_pixelmove), 168, 8, 16) && player_state) {
                    GameState = 1;
                    player_state = 0;
                }
                if(!collision_detection(playerX, playerY, playerW, playerH, (walls[i].x - current_pixelmove), 168, 8, 16) && !CollisionBefore && !player_state) {
                    OnGround = 0;
                }
            }
        }
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
    //Menu
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
    //Game Rendering
        if(GameState == 1) {
            int roundedcurrent_pixelmove = my_round(current_pixelmove);
            int GroundOffset = roundedcurrent_pixelmove % 8;
            for(int i = 0; i < 37; i++){
                int GroundX = i * 8;
                GroundX -= GroundOffset;
                Draw8x8(pixel, GroundX, 184, SnowIceTile);
                Draw8x8(pixel, GroundX, 192, SnowCoveredDirt);
                Draw8x8(pixel, GroundX, 200, DirtUpper);
                Draw8x8(pixel, GroundX, 208, DirtLower);
            }
        //Walls
            for(int i = 0; i < 30; i++) {
                if(walls[i].active) {
                    int wallx = (int)((walls[i].x - roundedcurrent_pixelmove) + 0.5);
                    int wally = (int)(walls[i].y + 0.5);
                    Draw8x8(pixel, wallx, wally, IceWallUpper);
                    Draw8x8(pixel, wallx, (wally + 8), IceWallLower);
                }
            }
 
            char StringB[7];
            int newVal = (current_pixelmove < 0) ? -current_pixelmove : current_pixelmove;
            wsprintfA(StringB, "%d", (int)(newVal));
            ConvertNumToFont(0, 0, pixel, StringB);
            int DistAway = 0;
            Draw8x8(pixel, DistAway + 8, 8, letterD);
            Draw8x8(pixel, DistAway + 16, 8, letterI);
            Draw8x8(pixel, DistAway + 24, 8, letterS);
            Draw8x8(pixel, DistAway + 32, 8, letterT);
            Draw8x8(pixel, DistAway + 40, 8, letterA);
            Draw8x8(pixel, DistAway + 48, 8, letterN);
            Draw8x8(pixel, DistAway + 56, 8, letterC);
            Draw8x8(pixel, DistAway + 64, 8, letterE);
            Draw8x8(pixel, DistAway + 80, 8, letterA);
            Draw8x8(pixel, DistAway + 88, 8, letterW);
            Draw8x8(pixel, DistAway + 96, 8, letterA);
            Draw8x8(pixel, DistAway + 104, 8, letterY);
            newVal = (Predator_dist < 0) ? -Predator_dist : Predator_dist;
            wsprintfA(StringB, "%d", (int)(newVal));
            ConvertNumToFont(DistAway + 112, 8, pixel, StringB);
 
        //Player Always on top
    //Player
            if (player_state == 0 && pengWCInd < pengLength){
                if(playerXvel < 0){DrawPlrFlip(pixel, PplayerX, PplayerY, pengWalkCycle[pengWCInd]);}
                else {DrawPlr(pixel, PplayerX, PplayerY, pengWalkCycle[pengWCInd]);}
            }else if(player_state > 0) {DrawPlr(pixel, PplayerX, PplayerY, Efox);}
        }
 
    // GameOver
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
