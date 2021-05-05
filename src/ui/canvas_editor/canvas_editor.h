#ifndef QUIXEL_CANVAS_EDITOR_H
#define QUIXEL_CANVAS_EDITOR_H

#include "t3gui/t3gui.h"
#include "modules/canvas/canvas.h"
#include "modules/box.h"

#define QUIXEL_CANVAS_EDITOR_SIGNAL_NONE         0
#define QUIXEL_CANVAS_EDITOR_SIGNAL_DELETE_LAYER 1

#define QUIXEL_BACKUP_INTERVAL (10 * 60 * 60) // 10 minutes

#define QUIXEL_TOOL_PIXEL            0
#define QUIXEL_TOOL_LINE             1
#define QUIXEL_TOOL_RECTANGLE        2
#define QUIXEL_TOOL_FILLED_RECTANGLE 3
#define QUIXEL_TOOL_OVAL             4
#define QUIXEL_TOOL_FILLED_OVAL      5
#define QUIXEL_TOOL_FLOOD_FILL       6
#define QUIXEL_TOOL_DROPPER          7
#define QUIXEL_TOOL_SELECTION        8

#define QUIXEL_TOOL_STATE_OFF        0
#define QUIXEL_TOOL_STATE_DRAWING    1
#define QUIXEL_TOOL_STATE_EDITING    2

#define QUIXEL_COLOR_PICKER_SHADES 11
#define QUIXEL_COLOR_PICKER_SCALE  12

#define QUIXEL_COLOR_PALETTE_MAX  128

typedef struct
{

	int layer;
	QUIXEL_BOX box;

	bool moving;
	int pin_x, pin_y;
	bool floating;

} QUIXEL_SELECTION_DATA;

typedef struct
{

	ALLEGRO_BITMAP * scratch_bitmap;
	ALLEGRO_BITMAP * peg_bitmap;
	int scratch_offset_x; // offset of scratch in relation to view
	int scratch_offset_y;
	int tool_offset_x;
	int tool_offset_y;
	int tool_top, tool_bottom, tool_left, tool_right;
	int view_x, view_y;
	int view_zoom;
	int hover_x, hover_y;
	int click_x, click_y;
	ALLEGRO_COLOR click_color;
	int release_x, release_y;
	int current_layer;
	int current_frame;
	int current_tool;
	ALLEGRO_SYSTEM_MOUSE_CURSOR old_cursor;
	ALLEGRO_SYSTEM_MOUSE_CURSOR current_cursor;
	int tool_state;
	QUIXEL_SELECTION_DATA selection;
	bool modified;
	bool update_title;
	ALLEGRO_COLOR left_base_color;
	ALLEGRO_COLOR last_left_base_color;
	ALLEGRO_COLOR left_color;
	ALLEGRO_COLOR last_left_color;
	ALLEGRO_COLOR left_shade_color;
	ALLEGRO_COLOR last_left_shade_color;
	ALLEGRO_COLOR left_alpha_color;
	ALLEGRO_COLOR last_left_alpha_color;
	ALLEGRO_COLOR right_base_color;
	ALLEGRO_COLOR last_right_base_color;
	ALLEGRO_COLOR right_color;
	ALLEGRO_COLOR last_right_color;
	ALLEGRO_COLOR right_shade_color;
	ALLEGRO_COLOR last_right_shade_color;
	ALLEGRO_COLOR right_alpha_color;
	ALLEGRO_COLOR last_right_alpha_color;
	ALLEGRO_COLOR pick_color[QUIXEL_COLOR_PICKER_SHADES];
	ALLEGRO_COLOR start_color;
	ALLEGRO_COLOR hover_color;
	ALLEGRO_COLOR palette[QUIXEL_COLOR_PALETTE_MAX];

	T3GUI_ELEMENT * left_shade_slider_element;
	T3GUI_ELEMENT * left_alpha_slider_element;
	T3GUI_ELEMENT * right_shade_slider_element;
	T3GUI_ELEMENT * right_alpha_slider_element;
	T3GUI_ELEMENT * editor_element;

	int signal;
	QUIXEL_CANVAS * canvas;
	char canvas_path[4096];

	int undo_count;
	char undo_name[256];
	int redo_count;
	char redo_name[256];

	int backup_tick;

} QUIXEL_CANVAS_EDITOR;

QUIXEL_CANVAS_EDITOR * quixel_create_canvas_editor(QUIXEL_CANVAS * cp);
void quixel_destroy_canvas_editor(QUIXEL_CANVAS_EDITOR * cep);

void quixel_center_canvas_editor(QUIXEL_CANVAS_EDITOR * cep, int frame);
void quixel_unfloat_canvas_editor_selection(QUIXEL_CANVAS_EDITOR * cep, QUIXEL_BOX * bp);

#endif