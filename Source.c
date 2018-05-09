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

const int playerBitmapSize = 80;

float missley = 0;
float misslex = 1280 / 2;

int wybor = -1;

int size = 128;

bool running = true;
bool game = false;
bool menuRun = false;
bool shipChoose = false;
bool startPage = true;


void delay(int mseconds) {
	clock_t goal = mseconds + clock();
	while (goal > clock());
}

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

void enemyLogic(ALLEGRO_BITMAP *enemyCharacter, struct enemyShip* enemy, struct enemyMissles* missleArr,
	struct sheep *player, struct bullets* bullet, ALLEGRO_BITMAP *missle, ALLEGRO_KEYBOARD_STATE keyboard, ALLEGRO_EVENT event) {  // rysowanie i poruszanie sie statku przeciwnika


	bool right = false;
	bool left = false;

	float moveTO = rand() % windowWidth;
	//printf("%f , m1 = \n", moveTO);

	if (enemy->hp > 0) {


		al_draw_bitmap(missle, missleArr->x, missleArr->y += missleArr->movementSpeed, 0);
		if (missleArr->y >= windowHeight) {
			missleArr->y = enemy->y;
			missleArr->x = enemy->x + 60;
		}
		if (!(player->x + 100 <= missleArr->x + 28 || player->x+28 >= missleArr->x + 38 ||  // 1 width 2 width
			player->y + 128 <= missleArr->y || player->y - 128 >= missleArr->y + 64))  // 1 height 2 height
		{ 	
			player->hp -= 25;
			missleArr->y = enemy->y;
			missleArr->x = enemy->x + 60;
		}



		if (player->x <= enemy->x + 30 && player->x >= enemy->x - 30 && al_key_down(&keyboard, ALLEGRO_KEY_SPACE) && player->ammo>0) {
			enemy->hp--;
		}
		if (moveTO >= enemy->x) {
			right = true;
			left = false;
		}

		if (moveTO <= enemy->x) {
			right = false;
			left = true;
		}


		if (right == true) {
			al_draw_bitmap(enemyCharacter, enemy->x += 3, enemy->y - 103, 0);


		}
		else {
			al_draw_bitmap(enemyCharacter, enemy->x -= 3, enemy->y - 103, 0);
		}

		al_draw_filled_rectangle(enemy->x, enemy->y - 30, enemy->x + enemy->maxHp * 3.2, enemy->y - 20, al_map_rgba(0, 0, 0, 255));
		al_draw_filled_rectangle(enemy->x, enemy->y - 30, enemy->x + enemy->hp * 3.2, enemy->y - 20, al_map_rgba(0, 215, 0, 100));

	}

}

void playerMovement(struct sheep *player, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyboard) {
	if (al_key_down(&keyboard, ALLEGRO_KEY_RIGHT)) player->x += player->movementSpeed;
	if (al_key_down(&keyboard, ALLEGRO_KEY_LEFT))  player->x -= player->movementSpeed;

	if (player->x >= windowWidth - playerBitmapSize) player->x = windowWidth - playerBitmapSize;
	if (player->x <= 0) player->x = 0;
}

void closeOperation(ALLEGRO_EVENT event, bool *game) { // test przeslania eventu
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
	al_draw_filled_rectangle(windowWidth / 2 - 150, 250, windowWidth / 2 + 150, 350, al_map_rgba(51, 75, 102, 255));
	al_draw_filled_rectangle(windowWidth / 2 - 150, 450, windowWidth / 2 + 150, 550, al_map_rgba(51, 75, 102, 255));
	al_draw_filled_rectangle(windowWidth / 2 - 150, 650, windowWidth / 2 + 150, 750, al_map_rgba(51, 75, 102, 255));
	gwazdki(100);
	if (wybor == 0) {
		al_draw_filled_rectangle(windowWidth / 2 - 150, 250, windowWidth / 2 + 150, 350, al_map_rgba(85, 125, 170, 255));

	}
	if (wybor == 1) {
		al_draw_filled_rectangle(windowWidth / 2 - 150, 450, windowWidth / 2 + 150, 550, al_map_rgba(85, 125, 170, 255));

	}
	if (wybor == 2) {
		al_draw_filled_rectangle(windowWidth / 2 - 150, 650, windowWidth / 2 + 150, 750, al_map_rgba(85, 125, 170, 255));

	}
	al_draw_textf(font48, al_map_rgb(255, 215, 0), windowWidth / 2 - 125, 280, 0, "Rozpocznij Gre");
	al_draw_textf(font48, al_map_rgb(255, 215, 0), windowWidth / 2 - 100, 480, 0, "Zmien statek");
	al_draw_textf(font48, al_map_rgb(255, 215, 0), windowWidth / 2 - 55, 680, 0, "Koniec");

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

	ALLEGRO_BITMAP *ship1 = al_load_bitmap("ship1Large.png");
	ALLEGRO_BITMAP *ship2 = al_load_bitmap("ship2Large.png");
	ALLEGRO_BITMAP *ship3 = al_load_bitmap("ship3Large.png");


	al_draw_filled_rectangle(windowWidth / 2 - 550, 300, windowWidth / 2 - 250, 600, al_map_rgba(51, 75, 102, 255));
	al_draw_filled_rectangle(windowWidth / 2 - 150, 300, windowWidth / 2 + 150, 600, al_map_rgba(51, 75, 102, 255));
	al_draw_filled_rectangle(windowWidth / 2 + 250, 300, windowWidth / 2 + 550, 600, al_map_rgba(51, 75, 102, 255));

	if (wybor == 0) {
		al_draw_filled_rectangle(windowWidth / 2 - 550, 300, windowWidth / 2 - 250, 600, al_map_rgba(85, 125, 170, 255));
	}
	if (wybor == 1) {
		al_draw_filled_rectangle(windowWidth / 2 - 150, 300, windowWidth / 2 + 150, 600, al_map_rgba(85, 125, 170, 255));
	}
	if (wybor == 2) {
		al_draw_filled_rectangle(windowWidth / 2 + 250, 300, windowWidth / 2 + 550, 600, al_map_rgba(85, 125, 170, 255));
	}
	al_draw_bitmap(ship1, windowWidth / 2 - 530, 322, 0);
	al_draw_bitmap(ship2, windowWidth / 2 - 130, 322, 0);
	al_draw_bitmap(ship3, windowWidth / 2 + 270, 322, 0);

	gwazdki(100);

	al_destroy_bitmap(ship1);
	al_destroy_bitmap(ship2);
	al_destroy_bitmap(ship3);

}
void shipChooseDrawStats(ALLEGRO_FONT *font24, struct sheep *player) {

	al_draw_textf(font24, al_map_rgb(255, 215, 0), 560, windowHeight - 320, 0, "HP %d", player->hp);
	al_draw_textf(font24, al_map_rgb(255, 215, 0), 560, windowHeight - 300, 0, "AD %d", player->dmg);
	al_draw_textf(font24, al_map_rgb(255, 215, 0), 560, windowHeight - 280, 0, "AMMO %d", player->ammo);
	al_draw_textf(font24, al_map_rgb(255, 215, 0), 560, windowHeight - 260, 0, "MS %3f", player->movementSpeed);

}
int enemyAmount = 0;
void spawner(int enemyAmount, int x, struct enemyShip* enemy, struct enemyMissles* enemyMisslesAr) {
	for (int i = enemyAmount; i < x + enemyAmount; i++) {
		enemy[i].x = rand() % windowWidth;
		enemy[i].y = 103 + rand()%200;
		enemy[i].hp = 40;
		enemy[i].maxHp = 40;
		enemy[i].movementSpeed = 3.1;
	}
	for (int i = enemyAmount; i < x + enemyAmount; i++) {
		enemyMisslesAr[i].x = enemy[i].x;
		enemyMisslesAr[i].y = enemy[i].y;
		enemyMisslesAr[i].movementSpeed = rand() % 5 + 2.1;
	}
}
int bulletAmount = 0;
void bulletShooting(int bulletAmount, struct bullets* bullet, struct sheep *player) {
	bullet[bulletAmount].x = player->x;
	bullet[bulletAmount].y = player->y;
	bullet[bulletAmount].speed = 10;
}
int main(void)
{
	srand(time(NULL));

	struct sheep player = { windowWidth / 2, windowHeight, 250, 1000 , 25,5.1,playerMovement }; // dziala jak konstruktor x,y,hp,ammo,dmg,speed
	struct enemyShip enemy[100];
	struct enemyMissles enemyMisslesAr[100];
	struct bullets bulletsArr[100];
	spawner(enemyAmount,5,enemy,enemyMisslesAr); //spawnuje poczatkowe statki
	enemyAmount += 5;


	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_KEYBOARD_STATE keyboard;
	ALLEGRO_BITMAP *playerCharacter = NULL;
	ALLEGRO_BITMAP *enemyCharacter = NULL;
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

	al_install_keyboard();
	al_install_audio();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	al_reserve_samples(2);


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


				al_draw_text(font18, al_map_rgb(255, 255, 255), windowWidth - 70, 0, 0, "LVL 0-1");// text poziom
				al_draw_textf(font18, al_map_rgb(255, 255, 255), windowWidth - 150, 20, 0, "HP: %d", player.hp);	//  wypisanie hp 
				al_draw_textf(font18, al_map_rgb(255, 255, 255), windowWidth - 150, 40, 0, "AMMO: %d", player.ammo);	//  wypisanie ammo 

				al_get_keyboard_state(&keyboard);

				gwazdki(100);

				spawnMedKit++;
				if (spawnMedKit == 300) { //SPAWN ENEMY
					spawnMedKit = 0;
					al_draw_bitmap(medKit, 500, 1, 0);
					spawner(enemyAmount,5, enemy, enemyMisslesAr);
					player.ammo += 200;
					enemyAmount += 5;
					if (enemyAmount >= 100) enemyAmount = 0;
				}

				if (al_key_down(&keyboard, ALLEGRO_KEY_LSHIFT)) { //STRZELANIE BULLETAMI
					bulletShooting(bulletAmount,bulletsArr,&player);
				}
				for (int i = 0; i <100; i++)
					enemyLogic(enemyCharacter, &enemy[i], &enemyMisslesAr[i], &player, &bulletsArr[i], missle, keyboard, event);

				player.playerMovement(&player, event, keyboard);
				playerShooting(&player, event, keyboard);
				al_draw_bitmap(playerCharacter, player.x, player.y - 130, 0);


				if (al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE)) {
					game = false;
					menuRun = true;

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
	al_destroy_sample_instance(songInstance);
	al_destroy_bitmap(playerCharacter);
	al_destroy_bitmap(missle);
	al_destroy_bitmap(enemyCharacter);
	al_destroy_timer(timer);

	return 0;
}