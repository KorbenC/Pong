/*
===========================================================================

Pong Clone Source Code
Copyright (C) 2011 Korben Carreno.

===========================================================================
*/

/* Libraries */
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>



/* constants and definitions */

const int SCREEN_W = 640;
const int SCREEN_H = 480;
const float FPS = 60;
const int paddle_height = 96;
const int paddle_width = 16;
const int bouncer_size = 16;

enum MYKEYS {
	KEY_UP,
	KEY_DOWN,
	KEY_W,
	KEY_S,
	KEY_SPACE,
	KEY_ESCAPE
};


/* functions */

/*
====================
draw_paddle

Draws the paddle
====================
*/
void draw_paddle(float x, float y)
{
	// fill
	al_draw_filled_rounded_rectangle(x, y, x + paddle_width, y + paddle_height, 3, 3, al_color_html("729fcf"));
	// outline
	al_draw_rounded_rectangle(x, y, x + paddle_width, y + paddle_height, 3, 3, al_color_html("b5edff"), 1.0);
	// shine
	al_draw_filled_rounded_rectangle(x, y, x + paddle_width / 2, y + paddle_height - 10, 3, 3, al_color_html("8abbef"));
}

/*
====================
draw_ball

Draws the ball
====================
*/
void draw_ball(float x, float y)
{
	// fill
	al_draw_filled_circle(x, y, bouncer_size, al_color_html("6be97d"));
	// shadow
	al_draw_filled_circle(x + bouncer_size / 4, y + bouncer_size / 4, bouncer_size / 3 * 2, al_color_html("59ce76"));
	// shine
	al_draw_filled_circle(x - bouncer_size / 3, y - bouncer_size / 3, bouncer_size / 4, al_color_html("9bffaa"));
}




int main(int argc, char **argv)
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *right_paddle = NULL;
	ALLEGRO_BITMAP *left_paddle = NULL;
	ALLEGRO_BITMAP *bouncer = NULL;
	ALLEGRO_FONT *font = NULL;

	float right_paddle_x = (SCREEN_W * .95) - (paddle_width / 2.0);
	float right_paddle_y = (SCREEN_H / 2.0) - (paddle_height / 2.0);

	float left_paddle_x = (SCREEN_W * .05) - (paddle_width / 2.0);
	float left_paddle_y = (SCREEN_H / 2.0) - (paddle_height / 2.0);

	float bouncer_x = SCREEN_W / 2.0 - bouncer_size / 2.0;
	float bouncer_y = SCREEN_H / 2.0 - bouncer_size / 2.0;
	float bouncer_dx = 4.0, bouncer_dy = -4.0;

	bool key [] = { false, false, false, false, false, false };
	bool redraw = true;
	bool done = false;

	if (!al_init()) {
		fprintf(stderr, "failed to initialized allegro\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard\n");
		return -1;
	}


	al_init_primitives_addon();

	al_init_font_addon();
	al_init_ttf_addon();

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST);


	//initialize display ( w, h )
	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		fprintf(stderr, "failed to create display\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer\n");
		return -1;
	}

	right_paddle = al_create_bitmap(paddle_width, paddle_height);
	if (!right_paddle) {
		fprintf(stderr, "failed to create right paddle bitmap\n");
		return -1;
	}

	left_paddle = al_create_bitmap(paddle_width, paddle_height);
	if (!left_paddle) {
		fprintf(stderr, "failed to create left paddle bitmap\n");
		return -1;
	}
	bouncer = al_create_bitmap(bouncer_size, bouncer_size);
	if (!bouncer) {
		fprintf(stderr, "failed to create bouncer bitmap\n");
		return -1;
	}
	font = al_load_font("arial.ttf", 12, 0);
	if (!font){
		fprintf(stderr, "failed to create font\n");
	}


	al_set_target_bitmap(left_paddle);
	al_clear_to_color(al_map_rgb(0, 255, 0));
	al_set_target_bitmap(right_paddle);
	al_clear_to_color(al_map_rgb(0, 255, 0));
	al_set_target_bitmap(bouncer);
	al_clear_to_color(al_map_rgb(0, 0, 255));
	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event queue\n");
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//logic for moving the paddles on input
			if (key[KEY_UP] && right_paddle_y >= 4.0) {
				right_paddle_y -= 4.0;
			}

			if (key[KEY_DOWN] && right_paddle_y <= SCREEN_H - paddle_height - 4.0){
				right_paddle_y += 4.0;
			}

			if (key[KEY_W] && left_paddle_y >= 4.0) {
				left_paddle_y -= 4.0;
			}

			if (key[KEY_S] && left_paddle_y <= SCREEN_H - paddle_height - 4.0){
				left_paddle_y += 4.0;
			}

			//check win
			if (bouncer_x < 0){
				al_stop_timer(timer);
				al_draw_text(font, al_map_rgb(255, 255, 255), 320, 240, ALLEGRO_ALIGN_CENTER, "Player 2 Wins!");
			}
			else if (bouncer_x > 640){
				al_stop_timer(timer);
				al_draw_text(font, al_map_rgb(255, 255, 255), 320, 240, ALLEGRO_ALIGN_CENTER, "Player 1 Wins!");
				//al_rest( 5 );
			}

			//logic for the bouncer
			//if( bouncer_x < 0 || bouncer_x > SCREEN_W - bouncer_size ) {
			//bouncer_dx = -bouncer_dx;
			//}

			if (bouncer_y < 0 || bouncer_y > SCREEN_H - bouncer_size) {
				bouncer_dy = -bouncer_dy;
			}

			if (bouncer_y <= left_paddle_y + (paddle_height) && bouncer_y >= left_paddle_y){
				if (bouncer_x == left_paddle_x + (paddle_width * 2)) {
					bouncer_dx = -bouncer_dx;
				}
			}
			if (bouncer_y <= right_paddle_y + (paddle_height) && bouncer_y >= right_paddle_y){
				if (bouncer_x == right_paddle_x - (paddle_width / 2.0)){
					bouncer_dx = -bouncer_dx;
				}
			}


			bouncer_x += bouncer_dx;
			bouncer_y += bouncer_dy;

			redraw = true;



		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_W:
				key[KEY_W] = true;
				break;

			case ALLEGRO_KEY_S:
				key[KEY_S] = true;
				break;

			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;

			case ALLEGRO_KEY_SPACE:
				key[KEY_SPACE] = true;
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_W:
				key[KEY_W] = false;
				break;

			case ALLEGRO_KEY_S:
				key[KEY_S] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;

			case ALLEGRO_KEY_SPACE:
				key[KEY_SPACE] = false;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 60, ALLEGRO_ALIGN_LEFT, "Ball x, y = %.01f %.01f", bouncer_x, bouncer_y);
			al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 70, ALLEGRO_ALIGN_LEFT, "right paddle y = %.01f", right_paddle_y);
			al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 80, ALLEGRO_ALIGN_LEFT, "left paddle y = %.01f", left_paddle_y);

			draw_paddle(right_paddle_x, right_paddle_y);
			draw_paddle(left_paddle_x, left_paddle_y);
			draw_ball(bouncer_x, bouncer_y);

			//al_draw_text( font, al_map_rgb( 255,255,255 ), 320, 240, ALLEGRO_ALIGN_CENTER, "Hello, this is a test" );
			//al_draw_bitmap( right_paddle, right_paddle_x, right_paddle_y, 0 );
			//al_draw_bitmap( left_paddle, left_paddle_x, left_paddle_y, 0 );
			//al_draw_bitmap( bouncer, bouncer_x, bouncer_y, 0 );

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

		}


	}

	al_destroy_bitmap(bouncer);
	al_destroy_bitmap(right_paddle);
	al_destroy_bitmap(left_paddle);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
