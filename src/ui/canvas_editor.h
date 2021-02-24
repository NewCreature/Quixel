#ifndef QUIXEL_CANVAS_EDITOR_H
#define QUIXEL_CANVAS_EDITOR_H

#include "t3gui/t3gui.h"
#include "canvas.h"

#define QUIXEL_CANVAS_EDITOR_SIGNAL_NONE         0
#define QUIXEL_CANVAS_EDITOR_SIGNAL_DELETE_LAYER 1

#define QUIXEL_CANVAS_EDITOR_TOOL_PIXEL            0
#define QUIXEL_CANVAS_EDITOR_TOOL_LINE             1
#define QUIXEL_CANVAS_EDITOR_TOOL_RECTANGLE        2
#define QUIXEL_CANVAS_EDITOR_TOOL_FILLED_RECTANGLE 3
#define QUIXEL_CANVAS_EDITOR_TOOL_OVAL             4
#define QUIXEL_CANVAS_EDITOR_TOOL_FILLED_OVAL      5

#define QUIXEL_COLOR_PICKER_SHADES 11
#define QUIXEL_COLOR_PICKER_SCALE   8

typedef struct
{

	ALLEGRO_BITMAP * bg_tile;

	int bg_scale;
	int view_x, view_y;
	int view_zoom;
	int hover_x, hover_y;
	int current_layer;
	int current_tool;
	int tool_state;
	ALLEGRO_COLOR base_color;
	ALLEGRO_COLOR last_base_color;
	ALLEGRO_COLOR left_color;
	ALLEGRO_COLOR right_color;
	ALLEGRO_COLOR pick_color[QUIXEL_COLOR_PICKER_SHADES];
	ALLEGRO_COLOR start_color;
	ALLEGRO_COLOR shade_color;
	ALLEGRO_COLOR alpha_color;

	T3GUI_ELEMENT * shade_slider_element;
	T3GUI_ELEMENT * alpha_slider_element;

	int signal;
	QUIXEL_CANVAS * canvas;

} QUIXEL_CANVAS_EDITOR;

QUIXEL_CANVAS_EDITOR * quixel_create_canvas_editor(QUIXEL_CANVAS * cp);
void quixel_destroy_canvas_editor(QUIXEL_CANVAS_EDITOR * cep);

#endif
