#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//*****************************************************************************
// TITLE:			Networked Pong
// 
// NAME:		    Martin Wickham,
//							Scott Hendrickson,
//							Kody Greenbaum
// LAB SECTION:		<301, 302, 303, or 304>
// LAB DAY:			Monday, Wednesday
//
// CLASS:			ECE 210
// DATE: 			FALL 2012
// 
// DESCRIPTION:		A pong game with powerups and a networked scoreboard
//*****************************************************************************
/* Initialization Functions */
void	LEDBARInit(void);
void 	DIPSWInit(void);
void	PBSwInit(void);	
void 	RGB_LEDInit(void);
void 	sysTickInit(void);
void  potentiometersInit(void);
/* Inputs, outputs, and wait timers */			
int 	readPotentiometer0(void);
int 	readPotentiometer1(void);
int 	readPotentiometer2(void);
int 	read_PBSwitchNum(int SwitchNumber);
int		read_Switches(void);
void  LEDBAROutput(int value);
void 	turnOn(char color);
void 	turnOff(char color);
void  sysTickWait1mS(int waitTime);
void	RIT128x96x4Init(int freq);
void  RIT128x96x4Clear(void);
void	RIT128x96x4StringDraw(char *text, int x, int y, int brightness);
void	RIT128x96x4ImageDraw(const unsigned char *pucImage, unsigned long ulX,
                     unsigned long ulY, unsigned long ulWidth,
                     unsigned long ulHeight);
/* Xbee Functions */
void 	Xbee_ConfigureAddresses(int destination, int ownAddr);
void 	Xbee_Send(int message);
void	Xbee_SendArray(char *s, int messageLength);


#define ballWidth 6
#define ballHeight 6
static unsigned char ball[18] =
{ 0x06, 0xAA, 0x60,
	0x6A, 0xDD, 0xA6,
	0xAD, 0xFF, 0xDA,
	0xAD, 0xFF, 0xDA,
	0x6A, 0xDD, 0xA6,
	0x06, 0xAA, 0x60 };

#define paddle_sprite_topWidth 24
#define paddle_sprite_topHeight 4
static unsigned char paddle_sprite_top[48] =
{ 0x00, 0x37, 0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA, 0x73, 0x00,
	0x37, 0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA, 0x73,
	0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA,
	0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD };

#define paddle_short_topWidth 12
#define paddle_short_topHeight 4
static unsigned char paddle_short_top[24] =
{ 0x00, 0x37, 0xAD, 0xDA, 0x73, 0x00,
  0x37, 0xAD, 0xFF, 0xFF, 0xDA, 0x73,
	0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA,
	0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD };

#define paddle_long_topWidth 36
#define paddle_long_topHeight 4
static unsigned char paddle_long_top[72] =
{ 0x00, 0x37, 0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA, 0x73, 0x00,
	0x37, 0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA, 0x73,
	0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA,
	0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD };

	
#define paddle_sprite_bottomWidth 24
#define paddle_sprite_bottomHeight 4
static unsigned char paddle_sprite_bottom[48] =
{ 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD,
	0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA,
	0x37, 0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA, 0x73,
	0x00, 0x37, 0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA, 0x73, 0x00 };

#define paddle_short_bottomWidth 12
#define paddle_short_bottomHeight 4
static unsigned char paddle_short_bottom[24] =
{ 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD,
	0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA,
  0x37, 0xAD, 0xFF, 0xFF, 0xDA, 0x73,
	0x00, 0x37, 0xAD, 0xDA, 0x73, 0x00};

#define paddle_long_bottomWidth 36
#define paddle_long_bottomHeight 4
static unsigned char paddle_long_bottom[72] =
{ 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD,
	0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA,
	0x37, 0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA, 0x73,
	0x00, 0x37, 0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA, 0x73, 0x00};

#define fastballWidth 8
#define fastballHeight 7
static unsigned char fastball_data[] =
{ 0x00, 0x00, 0x00, 0x00,
	0xF0, 0x00, 0xF0, 0x00,
	0x0F, 0x00, 0x0F, 0x00,
	0x00, 0xF0, 0x00, 0xF0,
	0x0F, 0x00, 0x0F, 0x00,
	0xF0, 0x00, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x00};

#define longWidth 8
#define longHeight 7
static unsigned char long_data[] =
{ 0x0A, 0xFF, 0xFA, 0x00,
	0xAA, 0x00, 0x0A, 0xA0,
	0xF0, 0x0F, 0x00, 0xF0,
	0xF0, 0xFF, 0xF0, 0xF0,
	0xF0, 0x0F, 0x00, 0xF0,
	0xAA, 0x00, 0x0A, 0xA0,
	0x0A, 0xFF, 0xFA, 0x00};

#define shortWidth 8
#define shortHeight 7
static unsigned char short_data[] =
{ 0x0A, 0xFF, 0xFA, 0x00,
	0xAA, 0x00, 0x0A, 0xA0,
	0xF0, 0x00, 0x00, 0xF0,
	0xF0, 0xFF, 0xF0, 0xF0,
	0xF0, 0x00, 0x00, 0xF0,
	0xAA, 0x00, 0x0A, 0xA0,
	0x0A, 0xFF, 0xFA, 0x00};

#define multiWidth 8
#define multiHeight 7
static unsigned char multiball_data[] =
{ 0x00, 0xAF, 0xA0, 0x00,
	0x00, 0xFF, 0xF0, 0x00,
	0x00, 0xAF, 0xA0, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0xAF, 0xA0, 0xAF, 0xA0,
	0xFF, 0xF0, 0xFF, 0xF0,
	0xAF, 0xA0, 0xAF, 0xA0};



#define TRUE 1
#define FALSE 0


#define PADDLE_SHORT_WIDTH 12
#define PADDLE_MID_WIDTH 24
#define PADDLE_LONG_WIDTH 36

#define PADDLE1_Y 10
#define PADDLE2_Y 86
#define MAX_PADDLE_SPEED 600
#define START_DX -1.0f
#define START_DY 1.0f
#define SCREEN_HEIGHT 96
#define SCREEN_WIDTH 128
#define FIELD_WIDTH 122
#define FIELD_LEFT 3
#define FIELD_RIGHT 125
#define POTEN_MAX 970
#define ANGLE_MAX 0.78539816339

#define NUM_BALLS 20
#define NUM_MULTIBALLS 5

#define MULTI_DELAY 20

#define MULTIBALL 0
#define FASTBALL 1
#define LONG_PADDLE 2
#define SHORT_PADDLE 3
#define NUM_POWERS 4

#define DEST_ADDR 8
#define OWN_ADDR 9
#define SEND_ARRAY_LEN 6

//#define SCOREBOARD


typedef unsigned short int bool;

struct player;

typedef struct sprite {
	int width;
	int height;
	unsigned char *data;
	int xOffset;
	int yOffset;
} Sprite;

typedef struct ball {
	bool active;
	float x;
	float y;
	float speed;
	float dx;
	float dy;
	struct player *lastHit;
	Sprite sprite;
} Ball;

typedef struct paddle {
	float x;
	float y;
	float collX;
	float collTime;
	Sprite shortSprite;
	Sprite midSprite;
	Sprite longSprite;
} Paddle;

typedef struct player {
	bool ai;
	int powerStatus[NUM_POWERS];
	Paddle paddle;
	int points;
} Player;

typedef struct power {
	bool active;
	bool available;
	int prb;
	int val;
	int x;
	int y;
	bool decr;
	Sprite sprite;
} Power;

/* Our Functions */
void	update(void);
void	updatePaddle(void);
void	updateBalls(void);
void	updatePowers(void);
void	updateScreen(void);
void	updateColl(int player);
void	drawSprite(Sprite *s, float x, float y);
void	scoreP1(void);
void	scoreP2(void);
void	reset(bool p1Score);
int		paddleWidth(int player);
Sprite *paddleSprite(int player);
void	send(void);

float max(float a, float b);
float min(float a, float b);
float clamp(float a, float cl, float ch);
float mix(float a, float b, float z);
float mod(float a, float b);

//*****************************************************************************
//
// Main Program:
//
//*****************************************************************************
Player players[2];
Ball balls[NUM_BALLS];
Power powers[NUM_POWERS];
int time;
char game_stats[6];

int main(void) {
	int c;
	//Initializing the LEDBAR, RGB LED, DIPSwitches and Pushbuttons, and a wait timer
	LEDBARInit();
	DIPSWInit();
	PBSwInit();
	RGB_LEDInit();
	sysTickInit();
	RIT128x96x4Init(1000000);
	potentiometersInit();
	RIT128x96x4Clear();
	
	players[0].ai = read_Switches() & 0x0002;
	players[0].points = 0;
	players[0].paddle.y = PADDLE1_Y;

	players[0].paddle.midSprite.data = paddle_sprite_top;
	players[0].paddle.midSprite.height = paddle_sprite_topHeight;
	players[0].paddle.midSprite.width = paddle_sprite_topWidth;
	players[0].paddle.midSprite.xOffset = 0;
	players[0].paddle.midSprite.yOffset = paddle_sprite_topHeight + ballHeight/2;

	players[0].paddle.shortSprite.data = paddle_short_top;
	players[0].paddle.shortSprite.height = paddle_short_topHeight;
	players[0].paddle.shortSprite.width = paddle_short_topWidth;
	players[0].paddle.shortSprite.xOffset = 0;
	players[0].paddle.shortSprite.yOffset = paddle_short_topHeight + ballHeight/2;

	players[0].paddle.longSprite.data = paddle_long_top;
	players[0].paddle.longSprite.height = paddle_long_topHeight;
	players[0].paddle.longSprite.width = paddle_long_topWidth;
	players[0].paddle.longSprite.xOffset = 0;
	players[0].paddle.longSprite.yOffset = paddle_long_topHeight + ballHeight/2;
	

	players[1].ai = read_Switches() & 0x0001;
	players[1].points = 0;
	players[1].paddle.y = PADDLE2_Y;
	
	players[1].paddle.midSprite.data = paddle_sprite_bottom;
	players[1].paddle.midSprite.height = paddle_sprite_bottomHeight;
	players[1].paddle.midSprite.width = paddle_sprite_bottomWidth;
	players[1].paddle.midSprite.xOffset = 0;
	players[1].paddle.midSprite.yOffset = -ballHeight/2;
	
	players[1].paddle.shortSprite.data = paddle_short_bottom;
	players[1].paddle.shortSprite.height = paddle_short_bottomHeight;
	players[1].paddle.shortSprite.width = paddle_short_bottomWidth;
	players[1].paddle.shortSprite.xOffset = 0;
	players[1].paddle.shortSprite.yOffset = -ballHeight/2;

	players[1].paddle.longSprite.data = paddle_long_bottom;
	players[1].paddle.longSprite.height = paddle_long_bottomHeight;
	players[1].paddle.longSprite.width = paddle_long_bottomWidth;
	players[1].paddle.longSprite.xOffset = 0;
	players[1].paddle.longSprite.yOffset = -ballHeight/2;



	for (c = 0; c < NUM_POWERS; c++) {
		players[0].powerStatus[c] = 0;
		players[1].powerStatus[c] = 0;
	}
	
	srand(2);
	
	powers[MULTIBALL].active = FALSE;
	powers[MULTIBALL].available = TRUE;
	powers[MULTIBALL].prb = (int) (0.01 * RAND_MAX);
	powers[MULTIBALL].val = NUM_MULTIBALLS * MULTI_DELAY;
	powers[MULTIBALL].decr = FALSE;
	powers[MULTIBALL].sprite.data = multiball_data;
	powers[MULTIBALL].sprite.height = multiHeight;
	powers[MULTIBALL].sprite.width = multiWidth;
	powers[MULTIBALL].sprite.yOffset = multiHeight/2;
	powers[MULTIBALL].sprite.xOffset = multiWidth/2;
	
	powers[FASTBALL].active = FALSE;
	powers[FASTBALL].available = TRUE;
	powers[FASTBALL].prb = (int) (0.01 * RAND_MAX);
	powers[FASTBALL].val = 1;
	powers[FASTBALL].decr = FALSE;
	powers[FASTBALL].sprite.data = fastball_data;
	powers[FASTBALL].sprite.height = fastballHeight;
	powers[FASTBALL].sprite.width = fastballWidth;
	powers[FASTBALL].sprite.yOffset = fastballHeight/2;
	powers[FASTBALL].sprite.xOffset = fastballWidth/2;
	
	powers[LONG_PADDLE].active = FALSE;
	powers[LONG_PADDLE].available = TRUE;
	powers[LONG_PADDLE].prb = (int) (0.01 * RAND_MAX);
	powers[LONG_PADDLE].val = 100;
	powers[LONG_PADDLE].decr = TRUE;
	powers[LONG_PADDLE].sprite.data = long_data;
	powers[LONG_PADDLE].sprite.height = longHeight;
	powers[LONG_PADDLE].sprite.width = longWidth;
	powers[LONG_PADDLE].sprite.yOffset = longHeight/2;
	powers[LONG_PADDLE].sprite.xOffset = longWidth/2;
	
	powers[SHORT_PADDLE].active = FALSE;
	powers[SHORT_PADDLE].available = TRUE;
	powers[SHORT_PADDLE].prb = (int) (0.01 * RAND_MAX);
	powers[SHORT_PADDLE].val = 100;
	powers[SHORT_PADDLE].decr = TRUE;
	powers[SHORT_PADDLE].sprite.data = short_data;
	powers[SHORT_PADDLE].sprite.height = shortHeight;
	powers[SHORT_PADDLE].sprite.width = shortWidth;
	powers[SHORT_PADDLE].sprite.yOffset = shortHeight/2;
	powers[SHORT_PADDLE].sprite.xOffset = shortWidth/2;
	
	for (c = 0; c < NUM_BALLS; c++) {
		balls[c].active = FALSE;
		balls[c].lastHit = players;
		balls[c].sprite.data = ball;
		balls[c].sprite.height = ballHeight;
		balls[c].sprite.width = ballWidth;
		balls[c].sprite.xOffset = ballWidth/2;
		balls[c].sprite.yOffset = ballHeight/2;
	}
	
	time = 0;
	reset(TRUE);
	
#ifdef SCOREBOARD	
	Xbee_ConfigureAddresses(DEST_ADDR, OWN_ADDR);
#endif
	
	RIT128x96x4StringDraw("pong", 0,8,15);
	sysTickWait1mS(100);
	RIT128x96x4Clear();
	
	send();
	while(1) {
		update();
		time++;
		sysTickWait1mS(readPotentiometer1());
		//sysTickWait1mS(20);
	}
}  	

void update() {
	updatePaddle();
	updateBalls();
	updatePowers();
	updateScreen();
}

void updateBalls() {
	float collX;
	float trueSpeed;
	float percent;
	float lastX;
	float lastY;
	float angle;
	Ball *b;
	int c;
	
	for(c = 0; c < NUM_BALLS; c++) {
		if (!balls[c].active)
			continue;
		
		b = &balls[c];
		
		lastX = b->x;
		lastY = b->y;
		b->x += b->dx;
		b->y += b->dy;
		
		if (b->x >= FIELD_RIGHT) {
			b->dx = -b->dx;
			b->x = 2*FIELD_RIGHT - b->x - 1;
		} else if (b->x < FIELD_LEFT) {
			b->dx = -b->dx;
			b->x = 2*FIELD_LEFT - b->x;
		}
		
		if (b->y >= players[1].paddle.y) {
			percent = (players[1].paddle.y - lastY) / (float) (b->y - lastY);
			collX = mix(lastX, b->x, percent);
			if (collX >= players[1].paddle.x && collX < players[1].paddle.x + paddleWidth(1)) {
				b->speed += .7;
				trueSpeed = 3*log(b->speed + 1);
				if (players[1].powerStatus[FASTBALL]) {
					players[1].powerStatus[FASTBALL] = 0;
					trueSpeed *= 2.5;
					powers[FASTBALL].available = TRUE;
					send();
				}
				angle = mix(-ANGLE_MAX, ANGLE_MAX, (collX - players[1].paddle.x) / (float) paddleWidth(1));
				b->dx = trueSpeed * sin(angle);
				b->dy = -trueSpeed * cos(angle);
				b->y = players[1].paddle.y + (1-percent)*b->dy; //TODO: fix this;
				b->x = collX + (1-percent)*b->dx;

				b->lastHit = &players[1];

				updateColl(0);
				updateColl(1);

			} else {
				b->active = FALSE;
				scoreP1();
			}
		} else if (b->y <= players[0].paddle.y) {
			percent = (players[0].paddle.y - lastY) / (float) (b->y - lastY);
			collX = mix(lastX, b->x, percent);
			if (collX >= players[0].paddle.x && collX < players[0].paddle.x + paddleWidth(0)) {
				b->speed += .7;
				trueSpeed = 3*log(b->speed + 1);
				if (players[0].powerStatus[FASTBALL]) {
					players[0].powerStatus[FASTBALL] = 0;
					trueSpeed *= 2.5;
					powers[FASTBALL].available = TRUE;
					send();
				}
				angle = mix(-ANGLE_MAX, ANGLE_MAX, (collX - players[0].paddle.x) / (float) paddleWidth(0));
				b->dx = trueSpeed * sin(angle);
				b->dy = trueSpeed * cos(angle);
				b->y = players[0].paddle.y + (1-percent)*b->dy; //TODO: fix this;
				b->x = collX + (1-percent)*b->dx;
				
				b->lastHit = &players[0];

				updateColl(0);
				updateColl(1);
				
			} else {
				b->active = FALSE;
				scoreP2();
			}
		}
	}
}

void scoreP1() {
	players[0].points++;
	send();
	reset(TRUE);
}
void scoreP2() {
	players[1].points++;
	send();
	reset(FALSE);
}
void reset(bool p1Score) {
	int c;
	int sum = 0;
	for (c = 0; c < NUM_BALLS; c++) {
		if (balls[c].active)
			sum++;
	}
	if (sum <= NUM_BALLS - NUM_MULTIBALLS)
		powers[MULTIBALL].available = TRUE;
	if (sum != 0)
		return;
	
	balls[0].dx = START_DX;
	balls[0].dy = p1Score ? -START_DY : START_DY;
	balls[0].x = SCREEN_WIDTH/2;
	balls[0].y = SCREEN_HEIGHT/2;
	balls[0].speed = 1;
	balls[0].lastHit = p1Score ? &players[1] : &players[0];
	balls[0].active = TRUE;
	
	if (p1Score)
		updateColl(0);
	else
		updateColl(1);
}

void updatePowers() {
	int c, d;
	float b1, b2, m1, m2, x, y;
	Ball *b;
	for (c = 0; c < NUM_POWERS; c++) {

		if (powers[c].decr) {
			if (players[0].powerStatus[c] != 0) {
				players[0].powerStatus[c]--;
				if (players[0].powerStatus[c] == 0) {
					powers[c].available = TRUE;
					updateColl(0);
					send();
				}
			}
			if (players[1].powerStatus[c] != 0) {
				players[1].powerStatus[c]--;
				if (players[1].powerStatus[c] == 0) {
					powers[c].available = TRUE;
					updateColl(1);
					send();
				}
			}
		}

		if (powers[c].active) {
			for (d = 0; d < NUM_BALLS; d++) {
				if (balls[d].active) {
					
					//collision detection
					m1 = -balls[d].dy/balls[d].dx;
					m2 = balls[d].dx/balls[d].dy;
					b1 = balls[d].y - m1*balls[d].x;
					b2 = powers[c].y - m2*powers[c].x;
					x = (b2-b1)/(m1-m2);
					y = m1*x + b1;
					if (((balls[d].y >= y && balls[d].y - balls[d].dy <= y) ||
							 (balls[d].y <= y && balls[d].y - balls[d].dy >= y)) &&
							abs(x - powers[c].x) <= powers[c].sprite.width &&
							abs(y - powers[c].y) <= powers[c].sprite.height)
					{
						balls[d].lastHit->powerStatus[c] = powers[c].val;
						powers[c].active = FALSE;
						powers[c].available = FALSE;
						if (powers[c].decr) {
							updateColl(0);
							updateColl(1);
						}
						send();
						break;
					}
				}
			}
		} else {
			if (powers[c].available && rand() < powers[c].prb) {
				powers[c].x = 10 + (float)(SCREEN_WIDTH-20)*(rand() / (float)RAND_MAX);
				powers[c].y = (float)SCREEN_HEIGHT/4.0f + (float)SCREEN_HEIGHT / 2 * rand()/(float)RAND_MAX;
				powers[c].active = TRUE;
			}
		}
	}
	
	for (c = 0; c < 2; c++) {
		if (players[c].powerStatus[MULTIBALL] != 0) {
			if (players[c].powerStatus[MULTIBALL] % MULTI_DELAY == 0) {
				b = NULL;
				for (d = 0; d < NUM_BALLS; d++) {
					if (!balls[d].active) {
						b = &balls[d];
						break;
					}
				}
				if (b == NULL)
					return; //this should never happen...
				b->active = TRUE;
				b->y = players[c].paddle.y;
				b->x = players[c].paddle.x + paddleWidth(c)/2;
				b->speed = 1;
				b->dy = 1 - 2*c;
				b->dx = 0;
				b->lastHit = &players[c];
				updateColl(1-c);
			}
			if (--players[c].powerStatus[MULTIBALL] == 0)
				send();
		}
	}
}

void updatePaddle() {
	int c;
	for (c = 0; c < 2; c++) {
		if (players[c].ai) {
			if (players[c].paddle.collTime > 0)
				players[c].paddle.x += clamp(
							(players[c].paddle.collX - players[c].paddle.x)/players[c].paddle.collTime--,
							-MAX_PADDLE_SPEED, MAX_PADDLE_SPEED);
		} else {
			players[c].paddle.x = (SCREEN_WIDTH - paddleWidth(c)) * (c == 0 ? readPotentiometer0() : readPotentiometer2()) / POTEN_MAX;
		}
	}
}

void updateScreen() {
	char str[40];
	int c;
	
	RIT128x96x4Clear();

#ifndef SCOREBOARD
	sprintf(str, "%d", players[0].points);
	RIT128x96x4StringDraw(str, 0, 40, 15);
	sprintf(str, "%d", players[1].points);
	RIT128x96x4StringDraw(str, 116, 40, 15);
#endif
	
	drawSprite(paddleSprite(0), players[0].paddle.x, players[0].paddle.y);
	drawSprite(paddleSprite(1), players[1].paddle.x, players[1].paddle.y);

	for (c = 0; c < NUM_POWERS; c++) {
		if (powers[c].active) {
			drawSprite(&powers[c].sprite, powers[c].x, powers[c].y);
		}
	}
	for (c = 0; c < NUM_BALLS; c++) {
		if (balls[c].active) {
			drawSprite(&balls[c].sprite, balls[c].x, balls[c].y);
		}
	}
}

void drawSprite(Sprite *s, float x, float y) {
			RIT128x96x4ImageDraw(s->data, (long) x - s->xOffset,
                     (long) y - s->yOffset, s->width, s->height);
}

void updateColl(int player) {
	float pos;
	int minTime = 1000000;
	int minC = -1;
	int c;
	int ttime;

	if (!players[player].ai)
		return;

	for (c = 0; c < NUM_BALLS; c++) {
		if (balls[c].active) {
			ttime = (players[player].paddle.y - balls[c].y) / balls[c].dy;
			if (ttime > 0 && ttime < minTime) {
				minTime = ttime;
				minC = c;
			}
		}
	}
	
	pos = (balls[minC].x + ((players[player].paddle.y - balls[minC].y) / balls[minC].dy) * balls[minC].dx);
	pos -= FIELD_LEFT; //move pos to be aligned at left
	if (pos > 0) {
		pos = fmod(pos, 2*FIELD_WIDTH);
		if (pos >= FIELD_WIDTH)
			pos = 2*FIELD_WIDTH - pos - 1;
	} else {
		pos = -pos;
		pos = fmod(pos, 2*FIELD_WIDTH);
		if (pos > FIELD_WIDTH)
			pos = 2*FIELD_WIDTH - pos;
	}
	pos += FIELD_LEFT; //move pos back into standard coords
	pos += 6.0f*rand()/RAND_MAX - 3.0f;
	players[player].paddle.collX = clamp(pos - paddleWidth(player)/2, 0, SCREEN_WIDTH - paddleWidth(player));
	players[player].paddle.collTime = minTime;
}

void send() {
#ifdef SCOREBOARD
	int i;
	int powers = 0x0;

//			ARRAY TO SEND
//{score_p1, score_p2, powerup_p1, powerup_p2, extra_p1, extra_p2};

	game_stats[0] = players[0].points & 0x00FF;
	game_stats[1] = players[1].points & 0x00FF;
	
	for(i = 0; i < NUM_POWERS; i++) {
		if(players[0].powerStatus[i] != 0) {
			powers = powers + (1 <<	i);
		}
	}
	game_stats[2] = powers;
	
	for(i = 0; i < NUM_POWERS; i++) {
		if(players[1].powerStatus[i] != 0) {
			powers = powers + (1 <<	i);
		}
	}
	game_stats[3] = powers;
	
	//Extras
	game_stats[4] = 0;
	game_stats[5] = 0;
		
	Xbee_SendArray(game_stats, SEND_ARRAY_LEN);
#endif
}


int paddleWidth(int player) {
	return paddleSprite(player)->width;
}

Sprite *paddleSprite(int player) {
	if (players[player].powerStatus[LONG_PADDLE]) {
		if (!players[1-player].powerStatus[SHORT_PADDLE]) {
			return &players[player].paddle.longSprite;
		}
	} else if (players[1-player].powerStatus[SHORT_PADDLE]) {
			return &players[player].paddle.shortSprite;
	}
	return &players[player].paddle.midSprite;
}

float max(float a, float b) {
	return a>b ? a : b;
}
float min(float a, float b) {
	return a<=b ? a : b;
}
float clamp(float a, float cl, float ch) {
	return max(cl, min(a, ch));
}

float mix(float a, float b, float z) {
	return a*(1-z) + b*z;
}
