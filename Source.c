#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h> 
#include <stdbool.h>

int windowHeight = 720;
int windowWidth = 1280;

struct sheep {
	float x;
	float y;
	int hp;
	int movementSpeed;
	void(*playerDrawing)(struct point*);
		void(*playerMovement)(struct sheep* , ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE keyboard);
};

void nic() {

}

void playerDrawing(struct point* p) {  // test 
	printf("dziala");		          //  funkcji
}								     //	  struktury 
							         
void playerMovement(struct sheep *player, ALLEGRO_EVENT event,ALLEGRO_KEYBOARD_STATE keyboard) {
	if (al_key_down(&keyboard, ALLEGRO_KEY_RIGHT)) player->x += player->movementSpeed;
	if (al_key_down(&keyboard, ALLEGRO_KEY_LEFT))  player->x -= player->movementSpeed;
}


void closeOperation(ALLEGRO_EVENT event, bool *running) { // test przeslania eventu
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		*running = false;
}


int main(void)
{	

	struct sheep player = {windowWidth/2, windowHeight, 3, 5,playerDrawing}; // dziala jak konstruktor 
	player.playerDrawing(&player);										    // test


	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_KEYBOARD_STATE keyboard;
	ALLEGRO_BITMAP *kwadrat;  // ----- tmp player

	al_init();

	display = al_create_display(windowWidth, windowHeight);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);
	kwadrat = al_create_bitmap(50, 50);   // ----- tmp player

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
			al_draw_bitmap(kwadrat, player.x,player.y-50, 1); // ----- tmp Player

			al_get_keyboard_state(&keyboard);

			player.playerMovement(&player,event,&keyboard);   // movement test
			
			
			al_flip_display();
		}
	}

	al_destroy_display(display);
	al_uninstall_keyboard();
		al_destroy_timer(timer);
		al_destroy_bitmap(kwadrat);
	return 0;
}