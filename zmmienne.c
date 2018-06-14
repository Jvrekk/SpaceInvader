#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
const float FPS = 60.0;
const int windowHeight = 1024;
const int windowWidth = 1280;
int points = 0;
int bulletAmount = 0;
int bulletTimer = 0;
int enemyAmount = 0;
const int playerBitmapSize = 80;

int wybor = -1;

int size = 128;



bool running = true;
bool game = false;
bool menuRun = false;
bool shipChoose = false;
bool startPage = true;
bool gameOver = false;


