#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h> 
#include <stdbool.h>

const float FPS = 60.0;

const int windowHeight = 720;
const int windowWidth = 1280;

const int playerBitmapSize = 50;



struct sheep {
	int x;
	int y;
	int hp;
	int movementSpeed;
	void(*playerDrawing)(struct point*);
		void(*playerMovement)(struct point* player);
};

void nic() {

}

void playerDrawing(struct point* p, int *x) {  // test 
	printf("x %d \n",*x);		              //  funkcji
}								             //	  struktury 
							         
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


int main(void)
{	

	struct sheep player = {windowWidth/2, windowHeight, 3, 5,playerDrawing}; // dziala jak konstruktor 


	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_KEYBOARD_STATE keyboard;
	ALLEGRO_BITMAP *kwadrat;  // ----- tmp player
	

	al_init();

	display = al_create_display(windowWidth, windowHeight);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	kwadrat = al_create_bitmap(playerBitmapSize, playerBitmapSize);   // ----- tmp player

	al_clear_to_color(al_map_rgb(0, 0, 0));
	
	al_set_target_bitmap(kwadrat);                 // ----- tmp player
	al_clear_to_color(al_map_rgb(0, 255, 0));		// ----- tmp player

	al_install_keyboard();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	al_init_image_addon();
	al_init_primitives_addon();


	bool running = true;

	al_start_timer(timer);
	while (running) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);


		closeOperation(event, &running);		
		if (event.type == ALLEGRO_EVENT_TIMER) {
			al_set_target_bitmap(al_get_backbuffer(display));
			al_clear_to_color(al_map_rgba_f(255, 0, 0, 0)); // cos tu jest namieszane z R G B
			al_draw_bitmap(kwadrat, player.x,player.y - playerBitmapSize, 1); // ----- tmp Player

			al_get_keyboard_state(&keyboard);

			player.playerDrawing(&player, &player.x);    // test: OK

			//player.playerMovement(&player,event,keyboard);   // nie dziala 
			playerMovement(&player, event, keyboard);      // dziala 
			
			
			al_flip_display();
		}
	}

	al_destroy_display(display);
	al_uninstall_keyboard();
		al_destroy_timer(timer);
		al_destroy_bitmap(kwadrat);
	return 0;
}