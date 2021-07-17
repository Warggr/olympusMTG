#include "lib3_allegroIO.h"

bool Allegro_io::attack_switch(int leftY, int Ywidth, int topZ, int arrowlength) const {
	bool attacks = false;
	while(1){
		ALLEGRO_EVENT event;
		al_flip_display();
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_KEY_CHAR:
				switch(event.keyboard.keycode){
					case ALLEGRO_KEY_UP:
						attacks = true;
						al_draw_filled_triangle(leftY, topZ, leftY+Ywidth, topZ, leftY + (Ywidth/2), topZ + arrowlength, registeredColors[HIGH1]);
						message("[ATTACKS]");
						//al_flip_display(); //already done by messaging
						break;
					case ALLEGRO_KEY_DOWN:
						attacks = false;
						erase_surface(Rect(leftY, topZ, Ywidth, arrowlength));
						message("[STAYS]");
						//al_flip_display(); //already done by messaging
						break;
					case ALLEGRO_KEY_RIGHT:
					case ALLEGRO_KEY_ENTER:
					case ALLEGRO_KEY_SPACE:
						return attacks;
				}
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				god.call_ragnarok();
		}
	}
}