#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <maxmod.h>		// maxmod library
#include "soundbank.h"		// created by building project
#include "soundbank_bin.h"	// created by building project

#include "characterData.h"
#include "enemyData.h"
#include "background.TXT"

typedef unsigned short u16;

struct enemy
{
int xPos;
int yPos;
int startLocation;
u16 health;
u16 healthStart;
u16 width;
u16 height;
u16 speed;
u16 scoreValue;
u16 enemyType; //0 = Regular Enemy. 1 = Tracker Enemy
u16 spawned;
u16 spawnRate; //1 - 100. 100= Always spawn.
};

struct pickup
{
int xPos;
int yPos;
int startLocation;
u16 width;
u16 height;
u16 speed;
u16 spawned;
u16 spawnRate;
u16 startSpawnRate;
u16 type;
};

struct bullet
{
int xPos;
int yPos;
int startPos;
u16 speed;
u16 ready;
};

//Function prototypes
void clearCD(int a, int z);
void loadColorPalette (unsigned int *color, int memAd);
void loadTileD (unsigned int *tileD, int memAd, int a);
void get_input (void);
void updatePlayerPos(void);
void updateBulletInfo(void);
void updateLaserInfo(void);
void updateEnemyInfo(struct enemy *enemy, int shape, int size, int enemyNum, int firstTileNum, int palette, int enemyType);
void updatePickupInfo(struct pickup *pickup, int pickupNum, int firstTileNum, int palette, int type);
void updateHUD(void);
void checkGameState(void);
int collision(int px, int py, int pWidth, int pHeight, int bx, int by, int bWidth, int bHeight);
void checkCollision(void);
int mapCo(int x, int y);
void countScore (void);
extern void addCount(int* count1, int* count2, int* count3, int* score);
extern void swap(int* a, int* b);
extern void swapPalette(int a, int b);

/*Declarations @@@@@@@@@@@@@@@@@@*/

typedef unsigned short u16;
int playerY = 200;
int playerX = 100;
u16 playerSpeed = 2;
u16 playerTileNum = 1;

struct enemy enemySmall;
struct enemy enemySmall2;
struct enemy enemyLarge;
struct enemy enemyTracker;
struct pickup extraLife;
struct pickup laserPickup;

struct bullet bullets[6];
int laserXpos = 100;
int laserYpos = 200;
u16 laserSpeed = 4;
u16 laserReady = 1;
u16 laserAmmo = 4;
u16 laserDamage = 0;

int score = 0;
int score2 = 0;
int score3 =0;
u16 playerLives = 4;

u16 gameState = 2;

u16 * palpt;
u16 * gfxpt, * gfxpt2;
int i, x, y;

u16 BGScrollX = 0;

/*@@@@@@@@@@@@@@@@@@@@@@@*/

//Definitions for memory addresses and such
#define BG_PAL		((u16 *)(0x05000000)) //Base palette address
#define VRAM_BASE		((u16 *)(0x06000000)) //Base for tile memory
#define OBJ_ENEMY_BASE 12	//Base number for accessing the first enemy in the OBJ holders.
#define OBJ_PICKUP_BASE 56 //Base number for accessing the first pickup in the OBJ holders.
#define BULLET_BASE 68 //Base number for bullets
#define LASER_BASE 92 //Base number for laser

//Memory pointers
u16 * ObjHolderBA = ((u16*) 0x07000000);
u16 * dispcnt = ((u16*) 0x4000000);
u16 * BG0ScrollingX = ((u16*) 0x4000010);
u16 * BG0ScrollingY = ((u16*) 0x4000012);
u16 * mapptBG0; //Map pointers for BG layers.
u16 * mapptBG1;

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	// the vblank interrupt must be enabled for VBlankIntrWait() to work
	irqInit();
	irqSet( IRQ_VBLANK, mmVBlank );
	
	mmInitDefault( (mm_addr)soundbank_bin, 8 ); // Initialising music and sound
	mmStart( MOD_MAINMENU, MM_PLAY_LOOP );

	srand (time(NULL)); //For the rand function to work
	
	//Setting up background layers
	u16 * BG0 = ((u16*) 0x4000008); 
	u16 * BG1 = ((u16*) 0x400000A);
	u16 * BG2 = ((u16*) 0x400000C);
	u16 * BG3 = ((u16*) 0x400000E);
	
	//Display settings
	*dispcnt = ((0 << 0) | (1 << 6) | (1 << 8) | (1 << 9) | (0 << 10) | (0 << 11) | (1 << 12)); 
	
	//				Priority		CBB		  Colour	   		SBB		 Screen Size
	*BG0 = ((1 << 0) | (1 << 2) | (1 << 7) | ( 0 << 8) | (0 << 14));
	*BG1 = ((0 << 0) | (2 << 2) | (0 << 7) | ( 24 << 8) | (0 << 14));
	*BG2 = ((2 << 0) | (0 << 2) | (0 << 7) | ( 0 << 8) | (0 << 14));
	*BG3 = ((3 << 0) | (0 << 2) | (0 << 7) | ( 0 << 8) | (0 << 14));

	
	//Clear Tiles just in case
	clearCD(0x06000000, 0x060005FE); //Clear all BGs
	
	//Load tile colours
	loadColorPalette(colorBlack, 0x050001E0);
	loadColorPalette(colorGreen, 0x050001E2);
	loadColorPalette(colorRed, 0x050001E4);
	loadColorPalette(colorRed, 0x050001DE);
	loadColorPalette(colorOrange, 0x050001E6);
	loadColorPalette(colorLBlue, 0x050001E8);
	loadColorPalette(colorYellow, 0x050001EA);
	loadColorPalette(colorWhite, 0x050001FE);
	
	//Load OBJ Colours
	loadColorPalette(colorDBlue, 0x05000200);
	loadColorPalette(colorGreen, 0x05000202);
	loadColorPalette(colorRed, 0x05000204);
	loadColorPalette(colorOrange, 0x05000206);
	loadColorPalette(colorLBlue, 0x05000208);
	loadColorPalette(colorYellow, 0x0500020A);
	loadColorPalette(colorWhite, 0x05000222);
	loadColorPalette(colorDarkRed, 0x0500020C);
	loadColorPalette(laserTip, 0x0500020E);
	loadColorPalette(colorGrey, 0x05000210);
	loadColorPalette(colorLGrey, 0x05000212);
	
	//Enemy OBJ Colors
	loadColorPalette(laColor1, 0x05000242);
	loadColorPalette(laColor2, 0x05000244);
	loadColorPalette(laColor3, 0x05000246);
	loadColorPalette(laColor4, 0x05000248);
	loadColorPalette(laColor5, 0x0500024A);
	loadColorPalette(laColor6, 0x0500024C);
	loadColorPalette(laColor7, 0x0500024E);
	loadColorPalette(laColor8, 0x05000250);
	
	loadColorPalette(tracker1, 0x05000262);
	loadColorPalette(tracker2, 0x05000264);
	loadColorPalette(tracker3, 0x05000266);
	loadColorPalette(tracker4, 0x05000268);
	loadColorPalette(tracker5, 0x0500026A);
	
	loadColorPalette(colorDBlue, 0x05000280);
	loadColorPalette(colorGreen, 0x05000282);
	loadColorPalette(colorRed, 0x05000284);
	loadColorPalette(colorOrange, 0x05000286);
	loadColorPalette(colorLBlue, 0x05000288);
	loadColorPalette(colorYellow, 0x0500028A);

	//Load Tile/Object Data
	loadTileD (longShip1, 0x06010020, 8);
	loadTileD (longShip2, 0x06010040, 8);
	loadTileD (longShipGo, 0x06010820, 8);
	loadTileD (longShip2, 0x06010840, 8);
	loadTileD (bulletDraw, 0x6010060, 8);
	loadTileD(longShip2, 0x6008020, 8);
	loadTileD(text, 0x6008040, 152);
	loadTileD(numbers, 0x06008400, 80);
	loadTileD(heart, 0x06010420, 8);
	loadTileD(laser, 0x06010C20, 32);
	loadTileD(laserPickupDraw, 0x06010440, 8);
	loadTileD(laserPickupDraw, 0x06008800, 8);
	
	//Enemy OBJ Data
	loadTileD (sprite_enemySmall, 0x6010080, 8);
	loadTileD (sprite_enemyLarge, 0x60100A0, 128);
	loadTileD (sprite_enemyTracker, 0x60102A0, 16);
	
	//Bullet Delcaration
	int b;
	for (b = 0; b <= 5; b++)
	{
		bullets[b].xPos = 250;
		bullets[b].speed = 3;
		bullets[b].ready = 1;
	}
	
	// Enemy Variables
	enemySmall.xPos = 240;
	enemySmall.yPos = 240;
	enemySmall.speed = 3;
	enemySmall.health = 1;
	enemySmall.healthStart = 1;
	enemySmall.width = 8;
	enemySmall.height = 8;
	enemySmall.scoreValue = 1;
	enemySmall.enemyType = 0;
	enemySmall.spawned = 0;
	enemySmall.spawnRate = 60;
	
	enemySmall2.xPos = 240;
	enemySmall2.yPos = 240;
	enemySmall2.speed = 3;
	enemySmall2.health = 1;
	enemySmall2.healthStart = 1;
	enemySmall2.width = 8;
	enemySmall2.height = 8;
	enemySmall2.scoreValue = 1;
	enemySmall2.enemyType = 0;
	enemySmall2.spawned = 0;
	enemySmall2.spawnRate = 60;
	
	enemyLarge.xPos = 240;
	enemyLarge.yPos = 240;
	enemyLarge.speed = 1;
	enemyLarge.health = 10;
	enemyLarge.healthStart = 10;
	enemyLarge.width = 32;
	enemyLarge.height = 32;
	enemyLarge.scoreValue = 5;
	enemyLarge.enemyType = 0;
	enemyLarge.spawned = 0;
	enemyLarge.spawnRate = 2;
	
	enemyTracker.xPos = 240;
	enemyTracker.yPos = 240;
	enemyTracker.speed = 2;
	enemyTracker.health = 3;
	enemyTracker.healthStart = 3;
	enemyTracker.height = 16;
	enemyTracker.width = 8;
	enemyTracker.scoreValue = 3;
	enemyTracker.enemyType = 1;
	enemyTracker.spawned = 0;
	enemyTracker.spawnRate = 5;
	
	//Powerup Variables
	extraLife.xPos = 240;
	extraLife.yPos = 240;
	extraLife.width = 8;
	extraLife.height = 8;
	extraLife.speed = 2;
	extraLife.spawned = 0;
	extraLife.startSpawnRate = 1;
	extraLife.spawnRate = 0;
	extraLife.type = 0;
	
	laserPickup.xPos = 240;
	laserPickup.yPos = 240;
	laserPickup.width = 8;
	laserPickup.height = 8;
	laserPickup.speed = 2;
	laserPickup.spawned = 0;
	laserPickup.startSpawnRate = 3;
	laserPickup.spawnRate = 0;
	laserPickup.type = 1;
	
	
	// main loop
	while (1) 
	{
		mmFrame();
		checkGameState();
		get_input();
		
		switch (gameState)
		{
		case 0: //Main Game
			VBlankIntrWait();
			
			updatePlayerPos();
			updateBulletInfo();
			updateLaserInfo();
			
			updateEnemyInfo(&enemySmall, 0, 0, 0, 4, 4, 0);
			updateEnemyInfo(&enemySmall2, 0, 0, 3, 4, 4, 0);
			updateEnemyInfo(&enemyLarge, 0, 2, 1, 5, 2, 0);
			updateEnemyInfo(&enemyTracker, 2, 0, 2, 21, 3, 1);
			
			updatePickupInfo(&extraLife, 0, 33, 4, 0);
			updatePickupInfo(&laserPickup, 1, 34, 0, 1);
			
			countScore();
			updateHUD();
			
		break;
		
		case 3:
			VBlankIntrWait();
		break;
		}
	}
}

void clearCD(int a, int z)			//Clearing data from memory
{
	int i;
	for (i = a; i <= z; i++)
	{
		unsigned int * CD = ((unsigned int*) i);
			CD[0] = (0 << 28);
			CD[1] = (0 << 28);
			CD[2] = (0 << 28);
			CD[3] = (0 << 28);
			CD[4] = (0 << 28);
			CD[5] = (0 << 28);
			CD[6] = (0 << 28);
			CD[7] = (0 << 28);
	}
}

void loadColorPalette (unsigned int *color, int memAd) //Loading color palettes into memory as defined in characterData.h and enemyData.h
{	
	u16 * tileColor = ((u16*) memAd);
	tileColor[0] = ((color[0] << 0)  | (color[1] << 5) | (color[2] << 10));
}

void loadTileD (unsigned int *tileD, int memAd, int a) //Loading color palettes into memory as defined in characterData.h and enemyData.h and background.txt
{
	unsigned int * tile = ((unsigned int*) memAd);
	int x = 0 , i;
	for (i = 0; i < a; i++)
	{
		tile[i] = 
			(
				(tileD[x]<<0) | 
				(tileD[x+1] << 4) | 
				(tileD[x+2] << 8) | 
				(tileD[x+3] << 12) | 
				(tileD[x+4] << 16) | 
				(tileD[x+5] << 20) |
				(tileD[x+6] << 24) | 
				(tileD[x+7] << 28)
			);
		x += 8;
	}
}

void updatePlayerPos(void) 
{
	//									Y Cor.		Shape Lookup
		ObjHolderBA [4] = ((playerY << 0) | (1 << 14));
	//									X Cor.					H. Flip		V. Flip		Size Lookup
		ObjHolderBA [5] = ((playerX<< 0) | (0 << 12) | (0 << 13) | (0 << 14));
	//								Tile Num.	Priority		Pallete
		ObjHolderBA [6] = ((playerTileNum << 0) | (0 << 10) | (0 << 12));
	//						Rotate and Scale
		ObjHolderBA [7] = (0 << 15);
}

void updateBulletInfo(void)
{	
	int b;
	int n = BULLET_BASE;
	for (b = 0; b <= 5; b++)
	{
		//									Y Cor.		Shape Lookup
			ObjHolderBA [n] = ((bullets[b].yPos << 0) | (0 << 14));
		//											X Cor.							H. Flip		V. Flip		Size Lookup
			ObjHolderBA [n + 1] = (((bullets[b].xPos += bullets[b].speed) << 0) | (0 << 12) | (0 << 13) | (0 << 14));
		//									Tile Num.		Priority		Palette
			ObjHolderBA [n + 2] = ((3<< 0) | (0 << 10) | (0 << 12));
		//									Rotate and Scale
			ObjHolderBA [n + 3] = (0 << 15);
				
		if (bullets[b].xPos > (playerX + 240)) {bullets[b].ready = 1; bullets[b].yPos = 200;}
		n += 4;
	}
}

void updateLaserInfo(void)
{
	//									Y Cor.		Shape Lookup
	ObjHolderBA [LASER_BASE] = ((laserYpos << 0) | (1 << 14));
	//									X Cor.																H. Flip		V. Flip		Size Lookup
	ObjHolderBA [LASER_BASE + 1] = (((laserXpos += laserSpeed) << 0) | (0 << 12) | (0 << 13) | (1 << 14));
	//														Tile Num.	Priority		Pallete
	ObjHolderBA [LASER_BASE + 2] = ((97 << 0) | (0 << 10) | (0 << 12));
	//													Rotate and Scale
	ObjHolderBA [LASER_BASE + 3] = (0 << 15);
	
	if (laserXpos > (playerX + 240)) {laserReady = 1; laserYpos = 200; laserDamage = 0;}
}

void updateEnemyInfo(struct enemy *enemy, int shape, int size, int enemyNum, int firstTileNum, int palette, int enemyType)
{
	int palette2 = (palette + 6);
	int b;
	enemy->startLocation = rand() % 118 + 8; //Randomise enemy start location
	int spawnC = rand() % 1000; //Set constant to be compared to spawn rate
	if(enemy->spawned == 1)
	{
			swapPalette( 0x05000248, 0x05000250);
			//			Drawing the enemies													Y Cor.							Shape Lookup
			ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4))] = ((enemy->yPos << 0) | (shape << 14));
			//																																			X Cor.		H. Flip		V. Flip		Size Lookup
			ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4)) + 1] = ((enemy->xPos -= enemy->speed << 0) | (0 << 12) | (0 << 13) | (size << 14));
			//																												Tile Num.	Priority		Pallete
			ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4)) + 2] = (( firstTileNum << 0) | (0 << 10) | (palette << 12));
			//																					Rotate and Scale
			ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4)) + 3] = (0 << 15);
			
			if ((enemy->xPos) < 1) //if enemy goes off the left of the screen, move it off screen
			{
				enemy->yPos = 200;
				enemy->xPos = 240;
				enemy->health = enemy->healthStart;
				ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4))] = (enemy->yPos << 0);
				ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4)) + 1] = (enemy->xPos -= enemy->speed << 0);
				enemy->spawned = 0;
			}
			
			for (b = 0; b <= 5; b++)
			{
				if (collision(bullets[b].xPos, bullets[b].yPos, 8, 8, enemy->xPos, enemy->yPos, enemy->width, enemy->height) == 1) // if enemy is hit with a bullet reduce health and move bullet back to off screen
				{
					enemy->health--;
					bullets[b].yPos = 200;
					swap(&palette, &palette2);
					ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4)) + 2] = (palette << 12);
				}
			}
			
			if ((collision(laserXpos, laserYpos, 32, 8, enemy->xPos, enemy->yPos, enemy->width, enemy->height) == 1) && (laserDamage == 1)) // if enemy is hit with a bullet reduce health and move bullet back to off screen
				{
					enemy->health--;
					swap(&palette, &palette2);
					ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4)) + 2] = (palette << 12);
				}
			
			if (enemy->health == 0) // if enemy is out of health move it off screen
			{
				mmEffect( SFX_ENEMYDEATH );
				mmEffectRelease( SFX_ENEMYDEATH );
				enemy->yPos = 200;
				enemy->xPos = 240;
				score += enemy->scoreValue;
				enemy->health = enemy->healthStart;
				ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4))] = (enemy->yPos << 0);
				ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4)) + 1] = (enemy->xPos -= enemy->speed << 0);
				enemy->spawned = 0;
			}
			
			if (collision(enemy->xPos, enemy->yPos, enemy->width, enemy->height, playerX, playerY, 14, 5) == 1) //if enemy hits player move it off screen
			{
				enemy->yPos = 200;
				enemy->xPos = 240;
				ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4))] = (enemy->yPos << 0);
				ObjHolderBA [(OBJ_ENEMY_BASE + (enemyNum * 4)) + 1] = (enemy->xPos -= enemy->speed << 0);
				enemy->spawned = 0;
				
				playerLives -= 1;
				
				mmEffectRate( mmEffect(SFX_PLAYERGAINLIFE), 512 );
				mmEffectRelease( SFX_PLAYERGAINLIFE );
			}
			
			if (enemy->enemyType == 1) //Code for the tracker enemy AI
			{
				if (enemy->yPos > playerY){enemy->yPos--;}
				if (enemy->yPos < playerY){enemy->yPos++;}
			}
	}
	else if (enemy->spawned == 0)
	{
		if (spawnC <= enemy->spawnRate)	//Checking to see if enemy will spawn
		{
			enemy->yPos = enemy->startLocation;
			enemy->xPos = 240;
			enemy->spawned = 1;
		}
	}
}

void updatePickupInfo(struct pickup *pickup, int pickupNum, int firstTileNum, int palette, int type)
{
			pickup->startLocation = rand() % 118 + 8; //Randomise pickup start location
			int spawnC = rand() % 1000 + 1;
			u16 effect;
	if(pickup->spawned == 1)
	{
				//																									Y Cor.							Shape Lookup
			ObjHolderBA [(OBJ_PICKUP_BASE + (pickupNum * 4))] = ((pickup->yPos << 0) | (0 << 14));
			//																																			X Cor.		H. Flip		V. Flip		Size Lookup
			ObjHolderBA [(OBJ_PICKUP_BASE + (pickupNum * 4)) + 1] = ((pickup->xPos -= pickup->speed << 0) | (0 << 12) | (0 << 13) | (0 << 14));
			//																												Tile Num.	Priority		Pallete
			ObjHolderBA [(OBJ_PICKUP_BASE + (pickupNum * 4)) + 2] = (( firstTileNum << 0) | (0 << 10) | (palette << 12));
			//																					Rotate and Scale
			ObjHolderBA [(OBJ_PICKUP_BASE + (pickupNum * 4)) + 3] = (0 << 15);
			
			if ((pickup->xPos) < 1) //if pickup goes off the left of the screen, move it back to the right
			{
				pickup->yPos = 200;
				pickup->xPos = 240;
				ObjHolderBA [(OBJ_PICKUP_BASE + (pickupNum * 4))] = (pickup->yPos << 0);
				ObjHolderBA [(OBJ_PICKUP_BASE + (pickupNum * 4)) + 1] = (pickup->xPos -= pickup->speed << 0);
				pickup->spawned = 0;
			}
			
			if (collision(pickup->xPos, pickup->yPos, pickup->width, pickup->height, playerX, playerY, 14, 5) == 1) //if pickup hits player
			{
				pickup->yPos = 200;
				pickup->xPos = 240;
				ObjHolderBA [(OBJ_PICKUP_BASE + (pickupNum * 4))] = (pickup->yPos << 0);
				ObjHolderBA [(OBJ_PICKUP_BASE + (pickupNum * 4)) + 1] = (pickup->xPos -= pickup->speed << 0);
				pickup->spawned = 0;
				
				effect = 1;
				
				if(effect == 1)
				{
					if (pickup->type == 0) //If it's an extra life
					{
						playerLives += 1;
						effect = 0;
					}
					
					if (pickup->type == 1) //If it's ammo
					{
						laserAmmo += 1;
						effect = 0;
					}
				}

				mmEffect(SFX_PLAYERGAINLIFE);
				mmEffectRelease( SFX_PLAYERGAINLIFE );
			}
	}
	else if (pickup->spawned == 0)
	{
		if (spawnC <= pickup->spawnRate)
		{
			pickup->yPos = pickup->startLocation;
			pickup->xPos = 240;
			pickup->spawned = 1;
		}
	}
	
	if (pickup->type == 0 && playerLives == 4) //If player has full lives, don't spawn extra lives.
	{
		pickup->spawnRate = 0;
	}
	if (pickup->type == 0 && playerLives < 4)
	{
		pickup->spawnRate = pickup->startSpawnRate;
	}
	
	if (pickup->type == 1 && laserAmmo == 4) // If player has max ammo don't spawn any more.
	{
		pickup->spawnRate = 0;
	}
	if (pickup->type == 1 && laserAmmo < 4)
	{
		pickup->spawnRate = pickup->startSpawnRate;
	}
}

void checkGameState(void)
{								
	mapptBG1 = ((u16 *) 0x0600C000);
	
	/* Game state 0 == Main Game
	   Game State 1 == Game Over
	   Game State 2 == Start Screen 
	   Game State 3 == Paused
	*/
	if (playerLives == 0)
	{
		gameState = 1;
	}
	
	switch (gameState)
	{
		case 1: //Game Over Screen
			clearCD (0x07000000, 0x07000200) ; //Clear the OBJ Holders.
			clearCD(0x0600C000, 0x0600C008); //Clear 'Lives' from map
			clearCD(0x06000000, 0x060005FE); //Clear the background from map
			clearCD(0x06004000, 0x06005580); // Clear tile data.
			clearCD(0x05000000, 0x050001C2); //Clear palette data
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(10, 8)] = (( 7 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); //G
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(11, 8)] = (( 8 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); //A
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(12, 8)] = (( 9 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // M
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(13, 8)] = (( 17 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // E
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(15, 8)] = (( 10 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // O
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(16, 8)] = (( 11 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // V
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(17, 8)] = (( 17 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // E
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(18, 8)] = (( 12 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // R
			
			
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(9, 9)] = (( 13 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // P
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(10, 9)] = (( 12 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // R
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(11, 9)] = (( 17 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // E
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(12, 9)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(13, 9)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(15, 9)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(16, 9)] = (( 15 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // T
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(17, 9)] = (( 8 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // A
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(18, 9)] = (( 12 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // R
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(19, 9)] = (( 15 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // T
			
			mmEffectCancel( SFX_PLAYERGAINLIFE );
			mmEffectCancelAll();
			mmStop(); //Stop music track
			
		break;
		
		case 0: //Main Game
			clearCD(0x0600C218, 0x0600C266);
			//Load Tile Colours for background using code provided by Mappy.
			loadColorPalette(colorBlack, 0x05000000);
			palpt = (u16 *) BG_PAL;
			for (i=0;i<256;i++) 
			{
				palpt[i] = background_cmap[i];
			}
				loadColorPalette(colorBlack, 0x050001E0);
				loadColorPalette(colorGreen, 0x050001E2);
				loadColorPalette(colorRed, 0x050001E4);
				loadColorPalette(colorRed, 0x050001CC);
				loadColorPalette(colorOrange, 0x050001E6);
				loadColorPalette(colorLBlue, 0x050001E8);
				loadColorPalette(colorYellow, 0x050001EA);
				loadColorPalette(colorWhite, 0x050001FE);
				loadColorPalette(colorDarkRed, 0x050001EC);
				loadColorPalette(colorGrey, 0x050001F0);
				loadColorPalette(colorLGrey, 0x050001F2);
				
				//Load background into memory using code provided by Mappy.
			gfxpt = VRAM_BASE+0x2000;
			gfxpt2 = (u16 *) background_blockgfx;
			for (i=0;i<(sizeof(background_blockgfx)/2);i++) {
				gfxpt[i] = gfxpt2[i];
			}
			//Load background from memory onto BG0
			mapptBG0 = ((u16 *) VRAM_BASE);
			for (y = 0; y < 20; y ++)
			{
				for (x = 0; x < 30; x++)
				{
																// Tile to use									H. Flip			V Flip
					mapptBG0[mapCo(x, y)] = (( backgroundMap[y][x] << 0) | (0 << 10) | (0 << 11));
				}
			}
			*BG0ScrollingX = ( BGScrollX << 0);
			
		break;
		
		case 2: //Start Screen
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(10, 7)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (14 << 12)); // S
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(11, 7)] = (( 13 << 0) | (0 << 10) | (0 << 11) | (14 << 12)); // P
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(12, 7)] = (( 8 << 0) | (0 << 10) | (0 << 11) | (14 << 12)); // A
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(13, 7)] = (( 19 << 0) | (0 << 10) | (0 << 11) | (14 << 12)); // C
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(14, 7)] = (( 17 << 0) | (0 << 10) | (0 << 11) | (14 << 12)); // E
			
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(16, 7)] = (( 7 << 0) | (0 << 10) | (0 << 11) | (14 << 12)); //G
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(17, 7)] = (( 8 << 0) | (0 << 10) | (0 << 11) | (14 << 12)); //A
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(18, 7)] = (( 9 << 0) | (0 << 10) | (0 << 11) | (14 << 12)); // M
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(19, 7)] = (( 17 << 0) | (0 << 10) | (0 << 11) | (14 << 12)); // E
			
						//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(9, 9)] = (( 13 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // P
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(10, 9)] = (( 12 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // R
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(11, 9)] = (( 17 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // E
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(12, 9)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(13, 9)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
			
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(16, 9)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(17, 9)] = (( 15 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // T
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(18, 9)] = (( 8 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // A
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(19, 9)] = (( 12 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // R
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(20, 9)] = (( 15 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // T
			
			VBlankIntrWait();
			updateEnemyInfo(&enemySmall, 0, 0, 0, 4, 0, 0); //Fly the enemy around the title screen

		break;
		
		case 3:
		
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(12, 8)] = (( 13 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); //P
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(13, 8)] = (( 8 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); //A
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(14, 8)] = (( 18 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // U
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(15, 8)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(16, 8)] = (( 17 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // E
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(17, 8)] = (( 20 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // D

			
			
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(9, 9)] = (( 13 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // P
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(10, 9)] = (( 12 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // R
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(11, 9)] = (( 17 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // E
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(12, 9)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(13, 9)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(15, 9)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(16, 9)] = (( 15 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // T
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(17, 9)] = (( 8 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // A
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(18, 9)] = (( 12 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // R
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(19, 9)] = (( 15 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // T
	}
}

void updateHUD(void)
{
	int i;
	int x = 0; 
	mapptBG1 = ((u16 *) 0x0600C000);
	// Load in 'Lives' to BG1
	for (i = 2; i < 7; i++)
	{
		//										Tile to use 	H. Flip			v Flip.		Palette
		mapptBG1[mapCo(x, 0)] = (( i << 0) | (0 << 10) | (0 << 11) | (15 << 12));
		x++;
	}
	// Load 'Score' to screen
	//										Tile to use 	H. Flip			v Flip.		Palette
	mapptBG1[mapCo(21, 0)] = (( 14 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // S
	//										Tile to use 	H. Flip			v Flip.		Palette
	mapptBG1[mapCo(22, 0)] = (( 19 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // C
	//										Tile to use 	H. Flip			v Flip.		Palette
	mapptBG1[mapCo(23, 0)] = (( 10 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // O
	//										Tile to use 	H. Flip			v Flip.		Palette
	mapptBG1[mapCo(24, 0)] = (( 12 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // R
	//										Tile to use 	H. Flip			v Flip.		Palette
	mapptBG1[mapCo(25, 0)] = (( 17 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); // E
	
	// Show lives icons depending on amount of lives left
	if (playerLives == 1){mapptBG1[mapCo(6, 0)] = (( 0 << 0) | (0 << 10) | (0 << 11) | (15 << 12));}
	if (playerLives >= 2)
	{
		//										Tile to use 	H. Flip			v Flip.		Palette
		mapptBG1[mapCo(6, 0)] = (( 1 << 0) | (0 << 10) | (0 << 11) | (15 << 12));
		if ( playerLives == 2){mapptBG1[mapCo(7, 0)] = (( 0 << 0) | (0 << 10) | (0 << 11) | (15 << 12));}
		
		if (playerLives >= 3)
		{
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(7, 0)] = (( 1 << 0) | (0 << 10) | (0 << 11) | (15 << 12));
			if ( playerLives == 3) {mapptBG1[mapCo(8, 0)] = (( 0 << 0) | (0 << 10) | (0 << 11) | (15 << 12));}
			
			if (playerLives == 4)
			{
				//										Tile to use 	H. Flip			v Flip.		Palette
				mapptBG1[mapCo(8, 0)] = (( 1 << 0) | (0 << 10) | (0 << 11) | (15 << 12));
			}
		}
	}
	
	if (laserAmmo != 0)
	{
		if (laserAmmo == 1){mapptBG1[mapCo(13, 0)] = (( 0 << 0) | (0 << 10) | (0 << 11) | (15 << 12));}
		if (laserAmmo >= 2)
		{
			//										Tile to use 	H. Flip			v Flip.		Palette
			mapptBG1[mapCo(13, 0)] = (( 64 << 0) | (0 << 10) | (0 << 11) | (15 << 12));
			if ( laserAmmo == 2){mapptBG1[mapCo(14, 0)] = (( 0 << 0) | (0 << 10) | (0 << 11) | (15 << 12));}
			
			if (laserAmmo >= 3)
			{
				//										Tile to use 	H. Flip			v Flip.		Palette
				mapptBG1[mapCo(14, 0)] = (( 64 << 0) | (0 << 10) | (0 << 11) | (15 << 12));
				if ( laserAmmo == 3) {mapptBG1[mapCo(15, 0)] = (( 0 << 0) | (0 << 10) | (0 << 11) | (15 << 12));}
				
				if (laserAmmo == 4)
				{
					//										Tile to use 	H. Flip			v Flip.		Palette
					mapptBG1[mapCo(15, 0)] = (( 64 << 0) | (0 << 10) | (0 << 11) | (15 << 12));
				}
			}
		}
	}
}

int collision(int ax, int ay, int aWidth, int aHeight, int bx, int by, int bWidth, int bHeight) //Bounding box collision
{
	if (ax < bx + bWidth &&
   ax + aWidth > bx &&
   ay < by + bHeight &&
   aHeight + ay > by) 
	{ 
		return 1;
	}
	else
	{
		return 0;
	}
}

void get_input (void)
{
	scanKeys ();
	u16 keyHeld = keysHeld();
	u16 keyDown = keysDown();
	u16 keyUp = keysUp();
	swapPalette(0x05000206, 0x0500020C);
	
	switch(gameState)  
	{
		case 1: // if on game over screen
		
			if (keyDown & KEY_START)
			{
				gameState = 0;
				clearCD(0x0600C000, 0x0600C264); //Clear BG1
				playerLives = 4;
				laserAmmo = 4;
				playerY = 70;
				playerX = 5;
				score = 0;
				score2 = 0;
				score3 = 0;
				
				playerTileNum = 1;
				
				enemySmall.xPos = 240;
				enemySmall2.xPos = 240;
				enemyTracker.xPos = 240;
				enemyLarge.xPos = 240;
				laserPickup.yPos = 240;
				extraLife.yPos = 240;
				BGScrollX = 0;
				
				mmEffect( SFX_GAMESTART );
				mmStart( MOD_PLAYMUSIC, MM_PLAY_LOOP );
			}
		
		break;
		
		case 2: //if on title screen
		
			if (keyDown & KEY_START) 
			{
				clearCD(0x0600C000, 0x0600C264); // Clear BG1;
				mmStop();
				mmEffect( SFX_GAMESTART );
				gameState = 0;
				mmSetModuleVolume( 200 );
				mmStart( MOD_PLAYMUSIC, MM_PLAY_LOOP );
				playerY = 70;
				playerX = 5;
				
				enemySmall.xPos = 240;
				enemyLarge.xPos = 240;
				enemyTracker.xPos = 240;
			}
		
		break;
		
		case 3:
		
			if (keyDown & KEY_START)
			{
				gameState = 0;
				mmResume();
			}
		
		break;
		
		case 0: //In main game state
		
			if (keyHeld & KEY_UP)
			{
				if (playerY > 8)
				{
					playerY -= playerSpeed;
					playerTileNum = 65;
				}
			}
			
			if (keyUp & KEY_UP)
			{
				playerTileNum = 1;
			}
				
			if (keyHeld & KEY_DOWN)
			{
				if (playerY < 150)
				{
					playerY += playerSpeed;
					playerTileNum = 65;
				}
			}
			
			if (keyUp & KEY_DOWN)
			{
				playerTileNum = 1;
			}
				
			if (keyHeld & KEY_LEFT)
			{
				if (playerX > 1)
				{
					playerX -= playerSpeed;
					playerTileNum = 65;
					BGScrollX--;
				}
			}
			
			if (keyUp & KEY_LEFT)
			{
				playerTileNum = 1;
			}
				
			if (keyHeld & KEY_RIGHT)
			{
				if ((playerX + 16) < 240)
				{
					playerX += playerSpeed;
					playerTileNum = 65;
					BGScrollX++;
				}
			}
			
			if (keyUp & KEY_RIGHT)
			{
				playerTileNum = 1;
			}
			
			if (keyDown & KEY_A)
			{
				if (bullets[0].ready == 1)
				{
					bullets[0].xPos= (playerX + 8);
					bullets[0].yPos = (playerY + 4);
					bullets[0].ready = 0;
					bullets[0].startPos = playerX;
					mmEffect( SFX_BULLETFIRE );
					mmEffectRelease( SFX_BULLETFIRE );
				}
				
				else if ((bullets[1].ready == 1) && (bullets[0].xPos > bullets[0].startPos + 100))
				{
					bullets[1].xPos= (playerX + 8);
					bullets[1].yPos = (playerY + 4);
					bullets[1].ready = 0;
					bullets[1].startPos = playerX;
					mmEffect( SFX_BULLETFIRE );
					mmEffectRelease( SFX_BULLETFIRE );
				}
				
				else if ((bullets[2].ready == 1) && (bullets[1].xPos > bullets[1].startPos + 100))
				{
					bullets[2].xPos= (playerX + 8);
					bullets[2].yPos = (playerY + 4);
					bullets[2].ready = 0;
					bullets[2].startPos = playerX;
					mmEffect( SFX_BULLETFIRE );
					mmEffectRelease( SFX_BULLETFIRE );
				}
				
				else if ((bullets[3].ready == 1) && (bullets[2].xPos > bullets[2].startPos + 100))
				{
					bullets[3].xPos= (playerX + 8);
					bullets[3].yPos = (playerY + 4);
					bullets[3].ready = 0;
					bullets[3].startPos = playerX;
					mmEffect( SFX_BULLETFIRE );
					mmEffectRelease( SFX_BULLETFIRE );
				}
				
				else if ((bullets[4].ready == 1) && (bullets[3].xPos > bullets[3].startPos + 100))
				{
					bullets[4].xPos= (playerX + 8);
					bullets[4].yPos = (playerY + 4);
					bullets[4].ready = 0;
					bullets[4].startPos = playerX;
					mmEffect( SFX_BULLETFIRE );
					mmEffectRelease( SFX_BULLETFIRE );
				}
				
				else if ((bullets[5].ready == 1) && (bullets[4].xPos > bullets[4].startPos + 100))
				{
					bullets[5].xPos= (playerX + 8);
					bullets[5].yPos = (playerY + 4);
					bullets[5].ready = 0;
					bullets[5].startPos = playerX;
					mmEffect( SFX_BULLETFIRE );
					mmEffectRelease( SFX_BULLETFIRE );
				}
			}
			
			if (keyHeld & KEY_B)
			{
				if (laserReady == 1)
				{
					if (laserAmmo != 1)
					{
						laserXpos = playerX;
						laserYpos = playerY + 5;
					}
				}
			}
			
			if (keyUp & KEY_B)
			{
				if (laserAmmo != 1 && laserReady == 1)
				{
					laserAmmo -=1;
					laserDamage = 1;
					laserReady = 0;
					mmEffect(SFX_LASERFIRE);
					mmEffectRelease( SFX_LASERFIRE );
				}
			}
			
			if (keyDown & KEY_START)
			{
				mmPause();
				gameState = 3;
			}
	}
}

int mapCo(int x, int y) //Use for tile placement.
{
	return ((y * 32) + x);
}

void countScore (void)
{
	int count1 = score;
	int count2 = score2;
	int count3 = score3;
	//int numTileBase = 32;

	// if (count1 >= 10)
	// {
		// score2++;
		// score -= 10;
		// count1 = score; 
	// }

	// if (count2 >= 10)
	// {
		// score3++;
		// score2 -= 10;
		// count2 = score2;
	// }
	
	addCount(&count1, &count2, &count3, &score);
	
	//count1 += numTileBase;
	//count2 += numTileBase;
	//count3 += numTileBase;
												// Tile to use 	H. Flip			v Flip.		Palette
	mapptBG1[mapCo(27, 0)] = (( count3 << 0) | (0 << 10) | (0 << 11) | (15 << 12)); 
												// Tile to use 	H. Flip			v Flip.		Palette
	mapptBG1[mapCo(28, 0)] = (( count2 << 0) | (0 << 10) | (0 << 11) | (15 << 12));
												// Tile to use 	H. Flip			v Flip.		Palette
	mapptBG1[mapCo(29, 0)] = (( count1 << 0) | (0 << 10) | (0 << 11) | (15 << 12));
	
}