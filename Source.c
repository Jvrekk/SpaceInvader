#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h> 
#include <stdbool.h>

float sheepX = 0;
float sheepY = 0;

int main(void)
{	

	ALLEGRO_DISPLAY *display;

	al_init();

	display = al_create_display(640, 480);
	al_clear_to_color(al_map_rgb(225, 50, 50));

	bool running = true;

	while (running) {

		al_flip_display();
	}

	al_destroy_display(display);
	return 0;
}