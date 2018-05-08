#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5\allegro_native_dialog.h>
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


bool running = true;
bool game = false;
bool menuRun = true;
bool shipChoose = false;


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
	float movementSpeed;
};


void playerShooting(struct sheep* player, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyboard) {
	
	int bulletX = player->x;
	int bulletY = player->y;
	

	if (al_key_down(&keyboard, ALLEGRO_KEY_SPACE)) 
	al_draw_filled_rectangle(bulletX+63, bulletY, bulletX + 73, 0, al_map_rgba(255, 215, 0, 100));

}



void enemyLogic(ALLEGRO_BITMAP *enemyCharacter,struct enemyShip* enemy, struct sheep *player, ALLEGRO_BITMAP *missle) {  // rysowanie i poruszanie sie statku przeciwnika
	

	bool right = false;
	bool left  = false;				
	
	float moveTO = rand() % windowWidth;
															//printf("%f , m1 = \n", moveTO);

	if (enemy->hp > 0) {

		
		al_draw_bitmap(missle,misslex, missley+=1, 0);
		if (missley >= windowHeight) {  
			missley = 0;
			misslex = enemy->x + 30;
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

	}

}
							         
void playerMovement(struct sheep *player, ALLEGRO_EVENT event,ALLEGRO_KEYBOARD_STATE keyboard) {
	if (al_key_down(&keyboard, ALLEGRO_KEY_RIGHT)) player->x += player->movementSpeed;
	if (al_key_down(&keyboard, ALLEGRO_KEY_LEFT))  player->x -= player->movementSpeed;

	if (player->x >= windowWidth - playerBitmapSize) player->x = windowWidth - playerBitmapSize;
	if (player->x <= 0) player->x =  0;	
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

void menuButtons(ALLEGRO_KEYBOARD_STATE keyboard) {
	al_draw_filled_rectangle(windowWidth / 2 - 100, 100, windowWidth / 2 + 100, 200, al_map_rgba(0, 0, 0, 255));
	al_draw_filled_rectangle(windowWidth / 2 - 100, 300, windowWidth / 2 + 100, 400, al_map_rgba(0, 0, 0, 255));
	al_draw_filled_rectangle(windowWidth / 2 - 100, 500, windowWidth / 2 + 100, 600, al_map_rgba(0, 0, 0, 255));
	gwazdki(100);
	if (wybor == 0) {
		al_draw_filled_rectangle(windowWidth / 2 - 100, 100, windowWidth / 2 + 100, 200, al_map_rgba(255, 0, 0, 255));

	}
	if (wybor == 1) {
		al_draw_filled_rectangle(windowWidth / 2 - 100, 300, windowWidth / 2 + 100, 400, al_map_rgba(255, 0, 0, 255));

	}
	if (wybor == 2) {
		al_draw_filled_rectangle(windowWidth / 2 - 100, 500, windowWidth / 2 + 100, 600, al_map_rgba(255, 0, 0, 255));

	}
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

	al_destroy_bitmap(ship1);
	al_destroy_bitmap(ship2);
	al_destroy_bitmap(ship3);
	
}
void shipChooseDrawStats(ALLEGRO_FONT *font18, struct sheep *player) {
	
	al_draw_textf(font18, al_map_rgb(255, 0, 255), 300, 100, 0, "HP %f", player->hp);
	al_draw_textf(font18, al_map_rgb(255, 0, 255), 300, 120, 0, "AD %f", player->dmg);
	al_draw_textf(font18, al_map_rgb(255, 0, 255), 300, 140, 0, "AMMO %f", player->ammo);
	al_draw_textf(font18, al_map_rgb(255, 0, 255), 300, 160, 0, "MS %f", player->movementSpeed);

}
int main(void)
{	
	srand(time(NULL));

	struct sheep player = {windowWidth/2, windowHeight, 3, 20 , 5.1,5,playerMovement}; // dziala jak konstruktor x,y,hp,ammo,dmg,speed
	struct enemyShip enemy[10];

	for (int i = 0; i < 10; i++) {
		enemy[i].x = 20;
		enemy[i].y = 103;
		enemy[i].hp = 20;
		enemy[i].movementSpeed = 3.1;
	}

	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_KEYBOARD_STATE keyboard;
	

	ALLEGRO_BITMAP *playerCharacter = NULL;
	ALLEGRO_BITMAP *enemyCharacter = NULL; 
	ALLEGRO_BITMAP *missle= NULL; 
	

	


	al_init();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();	

	ALLEGRO_FONT *font18 = al_load_font("arial.ttf", 18, 0);  // wczytanie fonta , size , flags 	


	display = al_create_display(windowWidth, windowHeight);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	

	al_clear_to_color(al_map_rgb(0, 0, 0));
	


	playerCharacter = al_load_bitmap("ship3.png");
	enemyCharacter = al_load_bitmap("enemy.png");
	missle = al_load_bitmap("missle.png");

	al_install_keyboard();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	
	if (playerCharacter == NULL )	puts("blad ladowania zdj player");
	if (font18 == NULL) puts("nie wczytano czcionki");
	if (enemyCharacter == NULL) puts("blad ladowania zdj enemy ");
	

	al_start_timer(timer);
	
	while (running) {

		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);

		double czas = al_get_time();

		if (event.type == ALLEGRO_EVENT_TIMER) {

			al_set_target_bitmap(al_get_backbuffer(display));
			al_clear_to_color(al_map_rgb(27, 39, 53));					// tlo okna

		if (menuRun) {
			
				al_draw_textf(font18, al_map_rgb(255, 0, 255), windowWidth - 120, 0, 0, "Time: %f",czas); 

				al_get_keyboard_state(&keyboard);

				menuButtons(keyboard);

				al_flip_display();
		}

		if (shipChoose) {
		
				double czas = al_get_time();

				al_draw_textf(font18, al_map_rgb(255, 0, 255), windowWidth - 120, 0, 0, "Time: %f", czas);// text poziom

				al_get_keyboard_state(&keyboard);
			
				shipChooseButtonsDrawing();

				gwazdki(100);
				if (wybor == 0) {
					player.hp = 250;
					player.movementSpeed = 5.1;
					player.dmg = 25;
					player.ammo = 1000;
					al_destroy_bitmap(playerCharacter);
					playerCharacter = al_load_bitmap("ship1.png");
					shipChooseDrawStats(font18, &player);
				}
				if (wybor == 1) {
					player.hp = 500;
					player.movementSpeed = 3.1;
					player.dmg = 40;
					player.ammo = 100;
					al_destroy_bitmap(playerCharacter);
					playerCharacter = al_load_bitmap("ship2.png");
				}
				if (wybor == 2) {
					player.hp = 5000;
					player.movementSpeed = 1.1;
					player.dmg = 250;
					player.ammo = 10;
					al_destroy_bitmap(playerCharacter);
					playerCharacter = al_load_bitmap("ship3.png");
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

		if(game){
			

				al_draw_text(font18, al_map_rgb(255, 0, 255), windowWidth - 70, 0, 0, "LVL 0-1");// text poziom
				al_draw_textf(font18, al_map_rgb(255, 0, 255), windowWidth - 150, 20, 0, "Player HP: %d", player.hp);	//  wypisanie hp 

				al_get_keyboard_state(&keyboard);			

				gwazdki(100);


				for(int i = 0; i <10;i++)
					enemyLogic(enemyCharacter, &enemy[i],&player, missle);
				
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
	al_destroy_bitmap(playerCharacter);
	al_destroy_bitmap(missle);
	al_destroy_bitmap(enemyCharacter);
		al_destroy_timer(timer);
	
	return 0;
}