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

#define ENEMYAMOUNT  1000

extern const float FPS;
extern const int windowHeight;
extern const int windowWidth;
extern int points;
extern const int playerBitmapSize;
extern int wybor ;
extern int size;
extern int enemyAmount;
extern int bulletAmount;
extern int bulletTimer;

extern bool running;
extern bool game;
extern bool menuRun;
extern bool shipChoose;
extern bool startPage;
extern bool gameOver;


struct sheep {
	float x;
	float y;
	int hp;
	int ammo;
	int dmg;
	float movementSpeed;
	void(*playerMovement)(struct sheep* player, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyboard);
	void(*playerShooting)(struct sheep* player);
};

struct enemyShip {
	float x;
	float y;
	int hp;
	int maxHp;
	int alive;
	float movementSpeed;
};

struct enemyMissles {
	float x;
	float y;
	float movementSpeed;
};

struct bullets {
	int x;
	int y;
	int speed;
};		


void playerShooting(struct sheep* player, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyboard) { 

	int bulletX = player->x;
	int bulletY = player->y;

	if (al_key_down(&keyboard, ALLEGRO_KEY_SPACE) && player->ammo > 0) {
		player->ammo--;
		al_draw_filled_rectangle(bulletX + 63, bulletY, bulletX + 73, 0, al_map_rgba(255, 215, 0, 100));
	}
}

void playerMovement(struct sheep *player, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyboard) {
	if (al_key_down(&keyboard, ALLEGRO_KEY_RIGHT)) player->x += player->movementSpeed;
	if (al_key_down(&keyboard, ALLEGRO_KEY_LEFT))  player->x -= player->movementSpeed;

	if (player->x >= windowWidth - playerBitmapSize) player->x = windowWidth - playerBitmapSize;
	if (player->x <= 0) player->x = 0;
}



void enemyLogic(ALLEGRO_BITMAP *enemyCharacter, ALLEGRO_BITMAP *enemyCharacter50hp, struct enemyShip* enemy, struct enemyMissles* missleArr,
	struct sheep *player, ALLEGRO_BITMAP *missle, ALLEGRO_KEYBOARD_STATE keyboard, ALLEGRO_EVENT event) {  // rysowanie i poruszanie sie statku przeciwnika
	

	int moveTO = rand() % windowWidth;

	if (enemy->hp > 0) {

		al_draw_bitmap(missle, missleArr->x, missleArr->y += missleArr->movementSpeed, 0);
		if (missleArr->y >= windowHeight) {
			missleArr->y = enemy->y;
			missleArr->x = enemy->x + 60;
		}
		if (!(player->x + 100 <= missleArr->x + 28 ||   player->x+28 >= missleArr->x + 38 || 
			  player->y + 128 <= missleArr->y      ||   player->y - 128 >= missleArr->y + 64)) 
		{ 	
			player->hp -= 25;
			missleArr->y = enemy->y;
			missleArr->x = enemy->x + 60;
		}

		if (player->x <= enemy->x + 30 && player->x >= enemy->x - 30 && al_key_down(&keyboard, ALLEGRO_KEY_SPACE) && player->ammo>0) {
			enemy->hp--;
		}
		if (moveTO >= enemy->x) {
			if (enemy->hp > 25)
				al_draw_bitmap(enemyCharacter, enemy->x += 3, enemy->y - 103, 0);
			else
				al_draw_bitmap(enemyCharacter50hp, enemy->x += 3, enemy->y - 103, 0);
		}

		if (moveTO <= enemy->x) {
			if (enemy->hp > 25)
				al_draw_bitmap(enemyCharacter, enemy->x -= 3, enemy->y - 103, 0);
			else
				al_draw_bitmap(enemyCharacter50hp, enemy->x -= 3, enemy->y - 103, 0);
		}

		al_draw_filled_rectangle(enemy->x, enemy->y - 30, enemy->x + enemy->maxHp * 3.2, enemy->y - 20, al_map_rgba(0, 0, 0, 255));
		al_draw_filled_rectangle(enemy->x, enemy->y - 30, enemy->x + enemy->hp * 3.2, enemy->y - 20, al_map_rgba(0, 215, 0, 100));
		
	}
	else{
		enemy->x = -200;
		enemy->alive = 1;
	}
	

}


void closeOperation(ALLEGRO_EVENT event, bool *game) { 
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		*game = false;
}

void gwazdki(int ilosc) {
	int i;
	srand(time(0));

	for (i = 0; i < ilosc; i++) {
		int x = rand() % windowWidth;
		int y = rand() % windowHeight;
		al_draw_filled_circle(x, y, 2, al_map_rgba(200, 200, 200, 60));
	}
}

void menuButtons(ALLEGRO_FONT *font48, ALLEGRO_KEYBOARD_STATE keyboard) {

	ALLEGRO_COLOR colorGold = al_map_rgb(255, 215, 0);
	ALLEGRO_COLOR colorButton = al_map_rgb(51, 75, 102);
	ALLEGRO_COLOR colorFocus = al_map_rgb(85, 125, 170);

	al_draw_filled_rectangle(windowWidth / 2 - 150, 250, windowWidth / 2 + 150, 350,colorButton);
	al_draw_filled_rectangle(windowWidth / 2 - 150, 450, windowWidth / 2 + 150, 550, colorButton);
	al_draw_filled_rectangle(windowWidth / 2 - 150, 650, windowWidth / 2 + 150, 750, colorButton);

	gwazdki(100);

	if (wybor == 0) al_draw_filled_rectangle(windowWidth / 2 - 150, 250, windowWidth / 2 + 150, 350, colorFocus);
	if (wybor == 1)  al_draw_filled_rectangle(windowWidth / 2 - 150, 450, windowWidth / 2 + 150, 550, colorFocus);
	if (wybor == 2) al_draw_filled_rectangle(windowWidth / 2 - 150, 650, windowWidth / 2 + 150, 750, colorFocus);


	al_draw_textf(font48, colorGold , windowWidth / 2 - 125, 280, 0, "Rozpocznij Gre");
	al_draw_textf(font48, colorGold, windowWidth / 2 - 100, 480, 0, "Zmien statek");
	al_draw_textf(font48, colorGold, windowWidth / 2 - 55, 680, 0, "Koniec");

	if (al_key_down(&keyboard, ALLEGRO_KEY_ENTER)) {
		if (wybor == 0) {
			game = true;
			menuRun = false;
		}
		if (wybor == 1) {
			shipChoose = true;
			menuRun = false;
		}
		if (wybor == 2) {
			running = false;
		}


	};
	if (al_key_down(&keyboard, ALLEGRO_KEY_1)) {
		wybor = 0;
	};
	if (al_key_down(&keyboard, ALLEGRO_KEY_2)) {
		wybor = 1;
	};
	if (al_key_down(&keyboard, ALLEGRO_KEY_3)) {
		wybor = 2;
	};
}

void shipChooseButtonsDrawing() {

	ALLEGRO_COLOR colorButton = al_map_rgb(51, 75, 102);
	ALLEGRO_COLOR colorFocus = al_map_rgb(85, 125, 170);

	ALLEGRO_BITMAP *ship1 = al_load_bitmap("ship1Large.png");
	ALLEGRO_BITMAP *ship2 = al_load_bitmap("ship2Large.png");
	ALLEGRO_BITMAP *ship3 = al_load_bitmap("ship3Large.png");


	al_draw_filled_rectangle(windowWidth / 2 - 550, 300, windowWidth / 2 - 250, 600, colorButton);
	al_draw_filled_rectangle(windowWidth / 2 - 150, 300, windowWidth / 2 + 150, 600, colorButton);
	al_draw_filled_rectangle(windowWidth / 2 + 250, 300, windowWidth / 2 + 550, 600, colorButton);

	if (wybor == 0) al_draw_filled_rectangle(windowWidth / 2 - 550, 300, windowWidth / 2 - 250, 600, colorFocus);
	if (wybor == 1) al_draw_filled_rectangle(windowWidth / 2 - 150, 300, windowWidth / 2 + 150, 600, colorFocus);
	if (wybor == 2) al_draw_filled_rectangle(windowWidth / 2 + 250, 300, windowWidth / 2 + 550, 600, colorFocus);


	al_draw_bitmap(ship1, windowWidth / 2 - 530, 322, 0);
	al_draw_bitmap(ship2, windowWidth / 2 - 130, 322, 0);
	al_draw_bitmap(ship3, windowWidth / 2 + 270, 322, 0);

	gwazdki(100);

	al_destroy_bitmap(ship1);
	al_destroy_bitmap(ship2);
	al_destroy_bitmap(ship3);

}

void shipChooseDrawStats(ALLEGRO_FONT *font24, struct sheep *player) {
	ALLEGRO_COLOR colorGold = al_map_rgb(255, 215, 0);

	al_draw_textf(font24, colorGold, 560, windowHeight - 320, 0, "HP %d", player->hp);
	al_draw_textf(font24, colorGold, 560, windowHeight - 300, 0, "AD %d", player->dmg);
	al_draw_textf(font24, colorGold, 560, windowHeight - 280, 0, "AMMO %d", player->ammo);
	al_draw_textf(font24, colorGold, 560, windowHeight - 260, 0, "MS %3f", player->movementSpeed);
}
 void spawner(int enemyAmount, int x, struct enemyShip* enemy, struct enemyMissles* enemyMisslesAr) {
	for (int i = enemyAmount; i < x + enemyAmount; i++) {
		enemy[i].x = rand() % windowWidth;
		enemy[i].y = 103 + rand()%200;
		enemy[i].hp = 40;
		enemy[i].maxHp = 40;
		enemy[i].alive = 0;
		enemy[i].movementSpeed = 3.1;
	}
	for (int i = enemyAmount; i < x + enemyAmount; i++) {
		enemyMisslesAr[i].x = enemy[i].x;
		enemyMisslesAr[i].y = enemy[i].y;
		enemyMisslesAr[i].movementSpeed = rand() % 5 + 5.1;
	}
}

void bulletShooting(int bulletAmount, struct bullets* bullet, struct sheep *player) {
	bullet[bulletAmount].x = player->x;
	bullet[bulletAmount].y = player->y;
	bullet[bulletAmount].speed = 20;
}
int howManySpawn = 5;
int counter(int enemyAmount, struct enemyShip enemy[ENEMYAMOUNT]) {
	int sumaPkt = 0;
	for (int i = 0; i < enemyAmount; i++) {
		sumaPkt += enemy[i].alive;
	}
	return sumaPkt;
}
void collisionDetector(struct enemyShip enemy[ENEMYAMOUNT],struct bullets bullet[ENEMYAMOUNT]) {
	

	for(int i = 0;i<ENEMYAMOUNT;i++){
		bullet[i].y -= bullet[i].speed;
		al_draw_filled_rectangle(bullet[i].x + 60, bullet[i].y - 100, bullet[i].x + 70, bullet[i].y - 120, al_map_rgba(255, 255, 255, 100));//bullet
		for (int j = 0; j < ENEMYAMOUNT; j++) {
			if (!(bullet[i].x + 10 <= enemy[j].x || bullet[i].x >= enemy[j].x + 128 ||  // 1 width 2 width
				bullet[i].y + 20 <= enemy[j].y || bullet[i].y - 10 >= enemy[j].y + 100))  // 1 height 2 height
			{
			enemy[j].hp -= 20;
			bullet[i].y = -999;
			}
		}
	}
}


int main(void)
{
	srand(time(NULL));

	struct sheep player = { windowWidth / 2, windowHeight, 250, 1000 , 25,5.1,playerMovement }; // dziala jak konstruktor x,y,hp,ammo,dmg,speed
	struct enemyShip enemy[ENEMYAMOUNT];
	struct enemyMissles enemyMisslesAr[ENEMYAMOUNT];
	struct bullets bulletsArr[ENEMYAMOUNT];
	spawner(enemyAmount, howManySpawn,enemy,enemyMisslesAr); //spawnuje poczatkowe statki
	enemyAmount += howManySpawn;

	
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_KEYBOARD_STATE keyboard;
	ALLEGRO_BITMAP *playerCharacter = NULL;
	ALLEGRO_BITMAP *enemyCharacter = NULL;
	ALLEGRO_BITMAP *enemyCharacter50hp = NULL;
	ALLEGRO_BITMAP *missle = NULL;
	ALLEGRO_BITMAP *medKit = NULL;





	al_init();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();

	ALLEGRO_FONT *font18 = al_load_font("arial.ttf", 18, 0);  // wczytanie fonta , size , flags 	
	ALLEGRO_FONT *font24 = al_load_font("arial.ttf", 24, 0);  // wczytanie fonta , size , flags 	
	ALLEGRO_FONT *font48 = al_load_font("arial.ttf", 36, 0);  // wczytanie fonta , size , flags 	
	ALLEGRO_FONT *font72 = al_load_font("startPage.ttf", 72, 0);  // wczytanie fonta , size , flags 	


	display = al_create_display(windowWidth, windowHeight);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);


	al_clear_to_color(al_map_rgb(0, 0, 0));



	playerCharacter = al_load_bitmap("ship1.png");
	enemyCharacter = al_load_bitmap("ufo.png");
	missle = al_load_bitmap("missle.png");
	medKit = al_load_bitmap("medKit.png");
	enemyCharacter50hp = al_load_bitmap("ufo50.png");

	al_install_keyboard();
	al_install_audio();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	al_reserve_samples(3);


	ALLEGRO_SAMPLE *backgroundSong = al_load_sample("backgroundSong.wav");
	ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(backgroundSong);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
	if (backgroundSong == NULL) {
		puts("blad ladowania sample");
	}	if (songInstance == NULL) {
		puts("blad ladowania istance");
	}

	ALLEGRO_SAMPLE *boomEffect = al_load_sample("boomEfect.wav");
	if (boomEffect == NULL) puts("blad ladowania boom");


	ALLEGRO_SAMPLE *shiftShoot = al_load_sample("shiftShoot.wav");
	if (boomEffect == NULL) puts("blad ladowania shoot");

	if (playerCharacter == NULL)	puts("blad ladowania zdj player");
	if (font18 == NULL) puts("nie wczytano czcionki");
	if (font24 == NULL) puts("nie wczytano czcionki");
	if (enemyCharacter == NULL) puts("blad ladowania zdj enemy ");
	if (medKit == NULL) puts("blad ladowania zdj medKit ");

	

	al_start_timer(timer);


	int spawnMedKit = 0;
	while (running) {

		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);

		double czas = al_get_time();
		
		if (event.type == ALLEGRO_EVENT_TIMER) {

			al_set_target_bitmap(al_get_backbuffer(display));
			al_clear_to_color(al_map_rgb(27, 39, 53));					// tlo okna

			if (startPage) {
				al_play_sample_instance(songInstance);
				al_play_sample(boomEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				al_get_keyboard_state(&keyboard);
				gwazdki(150);
				al_draw_text(font72, al_map_rgb(255, 0, 255), windowWidth/2 - 375, windowHeight/2 - 100 , 0, " SPACE INVADERS ");
				al_draw_text(font18, al_map_rgb(255, 0, 255), windowWidth/2 - 130, windowHeight/2  + 30, 0, " press space to start the adventure ");
			
				if (al_key_down(&keyboard, ALLEGRO_KEY_SPACE)) {
					
					startPage = false;
					menuRun = true;
				};
				al_flip_display();
			}


			if (menuRun) {

				al_draw_textf(font18, al_map_rgb(255, 0, 255), windowWidth - 120, 0, 0, "Time: %f", czas);

				al_get_keyboard_state(&keyboard);

				menuButtons(font48, keyboard);

				al_flip_display();
			}

			if (shipChoose) {

				double czas = al_get_time();

				al_draw_textf(font18, al_map_rgb(255, 0, 255), windowWidth - 120, 0, 0, "Time: %f", czas);// text poziom

				al_get_keyboard_state(&keyboard);

				shipChooseButtonsDrawing();


				if (wybor == 0) {
					player.hp = 250;
					player.movementSpeed = 5.1;
					player.dmg = 25;
					player.ammo = 1000;
					al_destroy_bitmap(playerCharacter);
					playerCharacter = al_load_bitmap("ship1.png");
					shipChooseDrawStats(font24, &player);
				}
				if (wybor == 1) {
					player.hp = 500;
					player.movementSpeed = 3.1;
					player.dmg = 40;
					player.ammo = 100;
					al_destroy_bitmap(playerCharacter);
					playerCharacter = al_load_bitmap("ship2.png");
					shipChooseDrawStats(font24, &player);
				}
				if (wybor == 2) {
					player.hp = 5000;
					player.movementSpeed = 1.1;
					player.dmg = 250;
					player.ammo = 10;
					al_destroy_bitmap(playerCharacter);
					playerCharacter = al_load_bitmap("ship3.png");
					shipChooseDrawStats(font24, &player);
				}



				if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE)) {
					menuRun = true;
					shipChoose = false;
				};
				if (al_key_down(&keyboard, ALLEGRO_KEY_1)) {
					wybor = 0;
				};
				if (al_key_down(&keyboard, ALLEGRO_KEY_2)) {
					wybor = 1;
				};
				if (al_key_down(&keyboard, ALLEGRO_KEY_3)) {
					wybor = 2;
				};




				al_flip_display();

			}

			if (game) {
				if (player.hp <= 0) {
					game = false;
					gameOver = true;
				}
				
				al_draw_text(font18, al_map_rgb(255, 255, 255), windowWidth - 70, 0, 0, "LVL 0-1");// text poziom
				al_draw_textf(font18, al_map_rgb(255, 255, 255), windowWidth - 150, 20, 0, "HP: %d", player.hp);	//  wypisanie hp 
				al_draw_textf(font18, al_map_rgb(255, 255, 255), windowWidth - 150, 40, 0, "AMMO: %d", player.ammo);	//  wypisanie ammo 
				al_draw_textf(font18, al_map_rgb(255, 255, 255), windowWidth - 150, 60, 0, "time : %f", czas);	//  wypisanie ammo 
				al_draw_textf(font18, al_map_rgb(255, 255, 255), windowWidth - 150, 80, 0, "pkt : %d", counter(enemyAmount, enemy));	//  wypisanie pkt 
				al_draw_textf(font18, al_map_rgb(255, 255, 255), windowWidth - 80, 80, 0, "/%d", enemyAmount);	//  wypisanie pkt 
				

				al_get_keyboard_state(&keyboard);

				gwazdki(100);
				
				if (counter(enemyAmount, enemy)>= enemyAmount) { //SPAWN ENEMY
					howManySpawn += 5;
					spawner(enemyAmount, howManySpawn, enemy, enemyMisslesAr);
					player.hp += 100;
					player.ammo += 1000;
					enemyAmount += howManySpawn;
					if (enemyAmount >= ENEMYAMOUNT) enemyAmount = 0;
				}

				if (al_key_down(&keyboard, ALLEGRO_KEY_LSHIFT) && player.ammo > 10) { //STRZELANIE BULLETAMI
					bulletTimer++;
					if (bulletTimer > 10) {
						bulletTimer = 0;
						player.ammo -= 10;
						bulletShooting(bulletAmount, bulletsArr, &player);
						bulletAmount++;
						al_play_sample(shiftShoot, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						if (bulletAmount >= ENEMYAMOUNT) bulletAmount = 0;
					}

				}
				for (int i = 0; i <100; i++)
					enemyLogic(enemyCharacter,enemyCharacter50hp, &enemy[i], &enemyMisslesAr[i], &player, missle, keyboard, event);

				collisionDetector(&enemy, &bulletsArr);
				player.playerMovement(&player, event, keyboard);
				playerShooting(&player, event, keyboard);
				al_draw_bitmap(playerCharacter, player.x, player.y - 130, 0);


				if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE)) {
					game = false;
					menuRun = true;

				};
				al_flip_display();
			}
			if (gameOver) {
				al_stop_sample_instance(songInstance);
				al_play_sample(boomEffect, 5.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				al_get_keyboard_state(&keyboard);
				gwazdki(150);
				al_draw_text(font72, al_map_rgb(255, 0, 255), windowWidth / 2 - 275, windowHeight / 2 - 100, 0, " Game Over ");
				al_draw_text(font18, al_map_rgb(255, 0, 255), windowWidth / 2 - 130, windowHeight / 2 + 30, 0, " press ESC to continue ");

				if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE)) {
					gameOver = false;
					running = false;
				};
				al_flip_display();
			}
			closeOperation(event, &running);
		}
	}

	al_destroy_display(display);
	al_uninstall_keyboard();
	al_shutdown_image_addon();
	al_destroy_sample(backgroundSong);
	al_destroy_sample(boomEffect);
	al_destroy_sample(shiftShoot);
	al_destroy_sample_instance(songInstance);
	al_destroy_bitmap(playerCharacter);
	al_destroy_bitmap(missle);
	al_destroy_bitmap(enemyCharacter);
	al_destroy_timer(timer);

	return 0;
}