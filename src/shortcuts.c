#include "t3f/t3f.h"
#include "instance.h"
#include "ui/menu/menu_file_proc.h"
#include "ui/menu/menu_edit_proc.h"
#include "ui/menu/menu_layer_proc.h"
#include "modules/canvas/canvas_helpers.h"

void pa_handle_shortcuts(APP_INSTANCE * app)
{
	int x, y, width, height;

	if((t3f_key[ALLEGRO_KEY_LCTRL] || t3f_key[ALLEGRO_KEY_RCTRL] || t3f_key[ALLEGRO_KEY_COMMAND]) && t3f_key[ALLEGRO_KEY_X])
	{
		pa_menu_edit_cut(0, app);
		t3f_key[ALLEGRO_KEY_X] = 0;
	}
	else if((t3f_key[ALLEGRO_KEY_LCTRL] || t3f_key[ALLEGRO_KEY_RCTRL] || t3f_key[ALLEGRO_KEY_COMMAND]) && t3f_key[ALLEGRO_KEY_C])
	{
		pa_menu_edit_copy(0, app);
		t3f_key[ALLEGRO_KEY_C] = 0;
	}
	else if((t3f_key[ALLEGRO_KEY_LCTRL] || t3f_key[ALLEGRO_KEY_RCTRL] || t3f_key[ALLEGRO_KEY_COMMAND]) && t3f_key[ALLEGRO_KEY_V])
	{
		pa_menu_edit_paste(0, app);
		t3f_key[ALLEGRO_KEY_V] = 0;
	}
	else if(t3f_key[ALLEGRO_KEY_DELETE])
	{
		pa_menu_edit_delete(0, app);
		t3f_key[ALLEGRO_KEY_DELETE] = 0;
	}
	else if((t3f_key[ALLEGRO_KEY_LCTRL] || t3f_key[ALLEGRO_KEY_RCTRL] || t3f_key[ALLEGRO_KEY_COMMAND]) && t3f_key[ALLEGRO_KEY_Z])
	{
		pa_menu_edit_undo(0, app);
		t3f_key[ALLEGRO_KEY_Z] = 0;
	}
	else if((t3f_key[ALLEGRO_KEY_LCTRL] || t3f_key[ALLEGRO_KEY_RCTRL] || t3f_key[ALLEGRO_KEY_COMMAND]) && t3f_key[ALLEGRO_KEY_Y])
	{
		pa_menu_edit_redo(0, app);
		t3f_key[ALLEGRO_KEY_Y] = 0;
	}
	else if((t3f_key[ALLEGRO_KEY_LCTRL] || t3f_key[ALLEGRO_KEY_RCTRL] || t3f_key[ALLEGRO_KEY_COMMAND]) && t3f_key[ALLEGRO_KEY_PGUP])
	{
		pa_menu_layer_move_up(0, app);
		t3f_key[ALLEGRO_KEY_PGUP] = 0;
	}
	else if((t3f_key[ALLEGRO_KEY_LCTRL] || t3f_key[ALLEGRO_KEY_RCTRL] || t3f_key[ALLEGRO_KEY_COMMAND]) && t3f_key[ALLEGRO_KEY_PGDN])
	{
		pa_menu_layer_move_down(0, app);
		t3f_key[ALLEGRO_KEY_PGDN] = 0;
	}
	else if((t3f_key[ALLEGRO_KEY_LCTRL] || t3f_key[ALLEGRO_KEY_RCTRL] || t3f_key[ALLEGRO_KEY_COMMAND]) && t3f_key[ALLEGRO_KEY_S])
	{
		pa_menu_file_save(0, app);
		t3f_key[ALLEGRO_KEY_S] = 0;
	}
	else if((t3f_key[ALLEGRO_KEY_LCTRL] || t3f_key[ALLEGRO_KEY_RCTRL] || t3f_key[ALLEGRO_KEY_COMMAND]) && t3f_key[ALLEGRO_KEY_A])
	{
		pa_menu_file_save_as(0, app);
		t3f_key[ALLEGRO_KEY_A] = 0;
	}
	else if((t3f_key[ALLEGRO_KEY_LCTRL] || t3f_key[ALLEGRO_KEY_RCTRL] || t3f_key[ALLEGRO_KEY_COMMAND]) && t3f_key[ALLEGRO_KEY_O])
	{
		pa_menu_file_load(0, app);
		t3f_key[ALLEGRO_KEY_O] = 0;
	}
	else if(t3f_key[ALLEGRO_KEY_C])
	{
		pa_get_canvas_dimensions(app->canvas_editor->canvas, &x, &y, &width, &height, 0);
		app->canvas_editor->view_x = x + width / 2 - (app->canvas_editor->editor_element->w / 2) / app->canvas_editor->view_zoom;
		app->canvas_editor->view_y = y + height / 2 - (app->canvas_editor->editor_element->h / 2) / app->canvas_editor->view_zoom;
		app->canvas_editor->view_fx = app->canvas_editor->view_x;
		app->canvas_editor->view_fy = app->canvas_editor->view_y;
		t3f_key[ALLEGRO_KEY_C] = 0;
	}
	else if(t3f_key[ALLEGRO_KEY_PGUP])
	{
		app->canvas_editor->current_layer++;
		t3f_key[ALLEGRO_KEY_PGUP] = 0;
	}
	else if(t3f_key[ALLEGRO_KEY_PGDN])
	{
		if(app->canvas_editor->current_layer > 0)
		{
			app->canvas_editor->current_layer--;
		}
		t3f_key[ALLEGRO_KEY_PGDN] = 0;
	}
	else if(t3f_key[ALLEGRO_KEY_H])
	{
		app->canvas_editor->canvas->layer[app->canvas_editor->current_layer]->flags ^= PA_CANVAS_FLAG_HIDDEN;
		t3f_key[ALLEGRO_KEY_H] = 0;
	}
}
