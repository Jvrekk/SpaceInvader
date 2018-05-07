#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_ttf.h>/
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const float FPS = 60.0;

const int windowHeight = 720;
const int windowWidth = 1280;

const int playerBitmapSize = 80;


struct sheep {
	float x;
	float y;
	int hp;
	int ammo;
	float movementSpeed;
	void(*playerDrawing)(struct point* player, ALLEGRO_BITMAP *kwadrat);
		void(*playerMovement)(struct point* player, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyboard);
			void(*playerShooting)(struct point* player);
};

struct enemyShip {
	float x;
	float y;
	int hp;
	float movementSpeed;
	void(*enemyDrawing)(struct point* player, ALLEGRO_BITMAP *kwadrat);
		void(*playerMovement)(struct point* player, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyboard);
			void(*playerShooting)(struct point* player);
};

void playerShooting(struct sheep* player, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyboard) {
	
	int bulletX = player->x;
	int bulletY = player->y;
	

	if (al_key_down(&keyboard, ALLEGRO_KEY_SPACE)) 
	al_draw_filled_rectangle(bulletX+63, bulletY, bulletX + 73, 0, al_map_rgba(255, 215, 0, 100));

}

void playerDrawing(struct sheep* player, ALLEGRO_BITMAP *kwadrat , ALLEGRO_FONT *font18) {  // rysowanie bitmapy pod playera 
	                                                                                       //	printf("x %fl \n",player->x);		
			al_draw_bitmap(kwadrat, player->x,player->y - playerBitmapSize, 1); 
			al_draw_textf(font18, al_map_rgb(255, 0, 255), windowWidth - 120, 20, 0,"Player HP: %d", player->hp);	//  wypisanie hp 
}




void enemyDrawing(struct enemyShip* enemy, ALLEGRO_BITMAP *kwadrat) {  // rysowanie bitmapy pod enemy mozna polaczyc z enemyLogic 
                                                                       //	printf("x %fl \n", player->x); 
	al_draw_bitmap(kwadrat, enemy->x, enemy->y - playerBitmapSize, 1);
}




void enemyLogic(ALLEGRO_BITMAP *enemyCharacter,struct enemyShip* enemy) {  // rysowanie i poruszanie sie statku przeciwnika


	bool right = false;
	bool left  = false;
	
	float moveTO = rand() % windowWidth;
																		//	printf("%f , m1 = \n", moveTO);  losowana wart x

	if (enemy->hp > 0) {

		if (moveTO >= enemy->x) {
			right = true;
			left = false;
		}

		if (moveTO <= enemy->x) {
			right = false;
			left = true;
		}

		if (right == true) {
			al_draw_bitmap(enemyCharacter, enemy->x += 3.1, enemy->y - 103, NULL);
		}
		else {
			al_draw_bitmap(enemyCharacter, enemy->x -= 3.1, enemy->y - 103, NULL);
		}
	}

}
							         
void playerMovement(struct sheep *player, ALLEGRO_EVENT event,ALLEGRO_KEYBOARD_STATE keyboard) {
	if (al_key_down(&keyboard, ALLEGRO_KEY_RIGHT)) player->x += player->movementSpeed;
	if (al_key_down(&keyboard, ALLEGRO_KEY_LEFT))  player->x -= player->movementSpeed;

	if (player->x >= windowWidth - playerBitmapSize) player->x = windowWidth - playerBitmapSize;
	if (player->x <= 0) player->x =  0;	
}

void closeOperation(ALLEGRO_EVENT event, bool *running) { // test przeslania eventu
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		*running = false;
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



int main(void)
{	
	srand(time(NULL));

	struct sheep player = {windowWidth/2, windowHeight, 3, 20 , 5.1 ,playerDrawing,playerMovement}; // dziala jak konstruktor x,y,hp,ammo,speed
	struct enemyShip enemy = { windowWidth / 2, 103, 3, 5.1 };


	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_KEYBOARD_STATE keyboard;
	ALLEGRO_BITMAP *kwadrat;  // ----- tmp player
	ALLEGRO_BITMAP *kwadratPodEnemy;  // ----- tmp enem

	
	ALLEGRO_BITMAP *playerCharacter = NULL;
	ALLEGRO_BITMAP *enemyCharacter= NULL; 
	
	


	al_init();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();	

	ALLEGRO_FONT *font18 = al_load_font("arial.ttf", 18, 0);  // wczytanie fonta , size , flags 	


	display = al_create_display(windowWidth, windowHeight);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	kwadrat = al_create_bitmap(playerBitmapSize, playerBitmapSize);   // ----- tmp player


	kwadratPodEnemy = al_create_bitmap(playerBitmapSize, playerBitmapSize);   // ----- tmp enemy
	


	al_clear_to_color(al_map_rgb(0, 0, 0));
	
	al_set_target_bitmap(kwadrat);                 // ----- tmp player
	al_clear_to_color(al_map_rgb(0, 255, 0));	  // ----- tmp player

	al_set_target_bitmap(kwadratPodEnemy);           // ----- tmp enemy
	al_clear_to_color(al_map_rgb(0, 255, 0));		// ----- tmp enemy



	playerCharacter = al_load_bitmap("dbg.png");
	enemyCharacter = al_load_bitmap("enemy.png");

	al_install_keyboard();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	
	if (playerCharacter == NULL || enemyCharacter == NULL ) {
		puts("blad ladowania zdj ");
	}

	bool running = true;
	
		
	al_start_timer(timer);

	while (running) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);

		closeOperation(event, &running);		
		if (event.type == ALLEGRO_EVENT_TIMER) {


			al_set_target_bitmap(al_get_backbuffer(display));			
			al_clear_to_color(al_map_rgb(27, 39, 53));					// tlo okna
			al_draw_text(font18, al_map_rgb(255, 0, 255), windowWidth - 70, 0, 0, "LVL 0-1");// text poziom 

			al_get_keyboard_state(&keyboard);			

			gwazdki(100);


			
			enemyDrawing(&enemy, kwadratPodEnemy);
			enemyLogic(enemyCharacter,&enemy);

			player.playerDrawing(&player, kwadrat, font18);
			player.playerMovement(&player, event, keyboard);
			playerShooting(&player, event, keyboard);
			al_draw_bitmap(playerCharacter, player.x, player.y - 130, NULL); 

			

			al_flip_display();
		}
	}
	al_destroy_display(display);
	al_uninstall_keyboard();
	al_shutdown_image_addon();
	al_destroy_bitmap(playerCharacter);
		al_destroy_timer(timer);
		al_destroy_bitmap(kwadrat);
	return 0;
}