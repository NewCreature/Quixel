#include "instance.h"
#include "modules/bitmap.h"
#include "modules/canvas/canvas.h"
#include "modules/canvas/canvas_helpers.h"
#include "modules/primitives.h"
#include "ui/canvas_editor/undo.h"
#include "ui/canvas_editor/undo_tool.h"
#include "ui/canvas_editor/undo_selection.h"
#include "ui/canvas_editor/clipboard.h"

int pa_menu_edit_undo(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	char undo_path[1024];

	t3f_debug_message("Enter pa_menu_edit_undo()\n");
	if(app->canvas_editor->undo_count > 0)
	{
		pa_get_undo_path("undo", app->canvas_editor->undo_count - 1, undo_path, 1024);
		pa_apply_undo(app->canvas_editor, undo_path, false);
		al_remove_filename(undo_path);
		app->canvas_editor->undo_count--;
		pa_update_undo_name(app->canvas_editor);
		pa_update_redo_name(app->canvas_editor);
		t3f_refresh_menus();
	}
	t3f_debug_message("Exit pa_menu_edit_undo()\n");

	return 0;
}

int pa_menu_edit_redo(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	char redo_path[1024];

	t3f_debug_message("Enter pa_menu_edit_redo()\n");
	if(app->canvas_editor->redo_count > 0)
	{
		pa_get_undo_path("redo", app->canvas_editor->redo_count - 1, redo_path, 1024);
		pa_apply_redo(app->canvas_editor, redo_path);
		al_remove_filename(redo_path);
		app->canvas_editor->redo_count--;
		pa_update_undo_name(app->canvas_editor);
		pa_update_redo_name(app->canvas_editor);
		t3f_refresh_menus();
	}
	t3f_debug_message("Exit pa_menu_edit_redo()\n");

	return 0;
}

int pa_menu_edit_cut(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	t3f_debug_message("Enter pa_menu_edit_cut()\n");
	if(app->canvas_editor->selection.box.width > 0 && app->canvas_editor->selection.box.height > 0)
	{
		if(!app->canvas_editor->selection.bitmap)
		{
			pa_copy_canvas_to_clipboard(app->canvas_editor, app->canvas_editor->current_layer, app->canvas_editor->selection.box.start_x, app->canvas_editor->selection.box.start_y, app->canvas_editor->selection.box.width, app->canvas_editor->selection.box.height);
			pa_draw_primitive_to_canvas(app->canvas_editor->canvas, app->canvas_editor->current_layer, app->canvas_editor->selection.box.start_x, app->canvas_editor->selection.box.start_y, app->canvas_editor->selection.box.end_x, app->canvas_editor->selection.box.end_y, NULL, al_map_rgba_f(0.0, 0.0, 0.0, 0.0), PA_RENDER_COPY, NULL,  pa_draw_filled_rectangle);
			pa_clear_canvas_editor_selection(app->canvas_editor);
		}
		else
		{
			if(pa_copy_bitmap_to_clipboard(app->canvas_editor, app->canvas_editor->selection.bitmap))
			{
				app->canvas_editor->clipboard.x = app->canvas_editor->selection.box.start_x;
				app->canvas_editor->clipboard.y = app->canvas_editor->selection.box.start_y;
				pa_clear_canvas_editor_selection(app->canvas_editor);
			}
		}
		t3f_refresh_menus();
	}
	t3f_debug_message("Exit pa_menu_edit_cut()\n");

	return 0;
}

int pa_menu_edit_copy(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	t3f_debug_message("Enter pa_menu_edit_copy()\n");
	if(app->canvas_editor->selection.box.width > 0 && app->canvas_editor->selection.box.height > 0)
	{
		if(!app->canvas_editor->selection.bitmap)
		{
			pa_copy_canvas_to_clipboard(app->canvas_editor, app->canvas_editor->current_layer, app->canvas_editor->selection.box.start_x, app->canvas_editor->selection.box.start_y, app->canvas_editor->selection.box.width, app->canvas_editor->selection.box.height);
		}
		else
		{
			if(pa_copy_bitmap_to_clipboard(app->canvas_editor, app->canvas_editor->selection.bitmap))
			{
				app->canvas_editor->clipboard.x = app->canvas_editor->selection.box.start_x;
				app->canvas_editor->clipboard.y = app->canvas_editor->selection.box.start_y;
			}
		}
		t3f_refresh_menus();
	}
	t3f_debug_message("Exit pa_menu_edit_copy()\n");

	return 0;
}

static void paste_helper(PA_CANVAS_EDITOR * cep, int pos, int ox, int oy)
{
	ALLEGRO_STATE old_state;
	int x, y;

	t3f_debug_message("Enter pa_menu_edit_paste()\n");
	if(cep->clipboard.bitmap)
	{
		al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
		al_set_new_bitmap_flags(ALLEGRO_NO_PREMULTIPLIED_ALPHA);
		if(cep->selection.bitmap)
		{
			pa_handle_unfloat_canvas_editor_selection(cep, &cep->selection.box);
		}
		cep->selection.bitmap = al_clone_bitmap(cep->clipboard.bitmap);
		if(cep->selection.bitmap)
		{
			switch(pos)
			{
				case 0:
				{
					x = cep->view_x + cep->view_width / 2 - al_get_bitmap_width(cep->selection.bitmap) / 2;
					y = cep->view_y + cep->view_height / 2 - al_get_bitmap_height(cep->selection.bitmap) / 2;
					break;
				}
				case 1:
				{
					x = cep->clipboard.x;
					y = cep->clipboard.y;
					break;
				}
				case 2:
				{
					x = cep->view_x + (t3f_mouse_x + ox) / cep->view_zoom - al_get_bitmap_width(cep->selection.bitmap) / 2;
					y = cep->view_y + (t3f_mouse_y + oy) / cep->view_zoom - al_get_bitmap_height(cep->selection.bitmap) / 2;
					break;
				}
			}
			pa_initialize_box(&cep->selection.box, x, y, al_get_bitmap_width(cep->selection.bitmap), al_get_bitmap_height(cep->selection.bitmap), cep->peg_bitmap);
			pa_update_box_handles(&cep->selection.box, cep->view_x, cep->view_y, cep->view_zoom);
			cep->current_tool = PA_TOOL_SELECTION;
		}
		al_restore_state(&old_state);
		t3f_refresh_menus();
	}
	t3f_debug_message("Exit pa_menu_edit_paste()\n");
}

int pa_menu_edit_paste(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	int pos = 0;
	int ox = 0;
	int oy = 0;

	if(id < 0)
	{
		pos = 2;
		ox = -app->ui->element[PA_UI_ELEMENT_CANVAS_EDITOR]->x;
		oy = -app->ui->element[PA_UI_ELEMENT_CANVAS_EDITOR]->y;
	}
	paste_helper(app->canvas_editor, pos, ox, oy);

	return 0;
}

int pa_menu_edit_paste_in_place(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	paste_helper(app->canvas_editor, 1, 0, 0);

	return 0;
}

int pa_menu_edit_flip_horizontal(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	bool need_float = false;

	if(!app->canvas_editor->selection.bitmap)
	{
		pa_handle_float_canvas_editor_selection(app->canvas_editor, &app->canvas_editor->selection.box);
		need_float = true;
	}
	if(app->canvas_editor->selection.bitmap)
	{
		pa_flip_bitmap(app->canvas_editor->selection.bitmap, true, false);
	}
	if(need_float)
	{
		pa_handle_unfloat_canvas_editor_selection(app->canvas_editor, &app->canvas_editor->selection.box);
	}
	return 0;
}

int pa_menu_edit_flip_vertical(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	bool need_float = false;

	if(!app->canvas_editor->selection.bitmap)
	{
		pa_handle_float_canvas_editor_selection(app->canvas_editor, &app->canvas_editor->selection.box);
		need_float = true;
	}
	if(app->canvas_editor->selection.bitmap)
	{
		pa_flip_bitmap(app->canvas_editor->selection.bitmap, false, true);
	}
	if(need_float)
	{
		pa_handle_unfloat_canvas_editor_selection(app->canvas_editor, &app->canvas_editor->selection.box);
	}
	return 0;
}

int pa_menu_edit_turn_clockwise(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	bool need_float = false;

	if(!app->canvas_editor->selection.bitmap)
	{
		pa_handle_float_canvas_editor_selection(app->canvas_editor, &app->canvas_editor->selection.box);
		need_float = true;
	}
	if(app->canvas_editor->selection.bitmap)
	{
		pa_turn_bitmap(&app->canvas_editor->selection.bitmap, 1);
		pa_initialize_box(&app->canvas_editor->selection.box, app->canvas_editor->selection.box.start_x, app->canvas_editor->selection.box.start_y, al_get_bitmap_width(app->canvas_editor->selection.bitmap), al_get_bitmap_height(app->canvas_editor->selection.bitmap), app->canvas_editor->peg_bitmap);
		pa_update_box_handles(&app->canvas_editor->selection.box, app->canvas_editor->view_x, app->canvas_editor->view_y, app->canvas_editor->view_zoom);
	}
	return 0;
}

int pa_menu_edit_turn_counter_clockwise(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	bool need_float = false;

	if(!app->canvas_editor->selection.bitmap)
	{
		pa_handle_float_canvas_editor_selection(app->canvas_editor, &app->canvas_editor->selection.box);
		need_float = true;
	}
	if(app->canvas_editor->selection.bitmap)
	{
		pa_turn_bitmap(&app->canvas_editor->selection.bitmap, -1);
		pa_initialize_box(&app->canvas_editor->selection.box, app->canvas_editor->selection.box.start_x, app->canvas_editor->selection.box.start_y, al_get_bitmap_width(app->canvas_editor->selection.bitmap), al_get_bitmap_height(app->canvas_editor->selection.bitmap), app->canvas_editor->peg_bitmap);
		pa_update_box_handles(&app->canvas_editor->selection.box, app->canvas_editor->view_x, app->canvas_editor->view_y, app->canvas_editor->view_zoom);
	}
	return 0;
}

int pa_menu_edit_delete_layer(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	t3f_debug_message("Enter pa_menu_edit_delete_layer()\n");
	if(!pa_remove_canvas_layer(app->canvas, app->canvas_editor->current_layer))
	{
		printf("Failed to remove layer!\n");
	}
	if(app->canvas_editor->current_layer >= app->canvas->layer_max)
	{
		pa_select_canvas_editor_layer(app->canvas_editor, app->canvas_editor->canvas->layer_max - 1);
	}
	if(app->canvas_editor->current_layer < 0)
	{
		pa_select_canvas_editor_layer(app->canvas_editor, 0);
	}
	t3f_debug_message("Exit pa_menu_edit_delete_layer()\n");
	return 0;
}

int pa_menu_edit_delete(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	char undo_path[1024];

	t3f_debug_message("Enter pa_menu_edit_delete()\n");
	if(app->canvas_editor->selection.box.width > 0 && app->canvas_editor->selection.box.height > 0)
	{
		pa_get_undo_path("undo", app->canvas_editor->undo_count, undo_path, 1024);
		if(!app->canvas_editor->selection.bitmap)
		{
			if(pa_make_tool_undo(app->canvas_editor, "Delete Selection", app->canvas_editor->current_layer, app->canvas_editor->selection.box.start_x, app->canvas_editor->selection.box.start_y, app->canvas_editor->selection.box.width, app->canvas_editor->selection.box.height, undo_path))
			{
				pa_finalize_undo(app->canvas_editor);
			}
			pa_draw_primitive_to_canvas(app->canvas_editor->canvas, app->canvas_editor->current_layer, app->canvas_editor->selection.box.start_x, app->canvas_editor->selection.box.start_y, app->canvas_editor->selection.box.end_x, app->canvas_editor->selection.box.end_y, NULL, al_map_rgba_f(0.0, 0.0, 0.0, 0.0), PA_RENDER_COPY, NULL,  pa_draw_filled_rectangle);
			pa_clear_canvas_editor_selection(app->canvas_editor);
			app->canvas_editor->modified++;
		}
		else
		{
			pa_clear_canvas_editor_selection(app->canvas_editor);
		}
		t3f_refresh_menus();
	}
	t3f_debug_message("Exit pa_menu_edit_delete()\n");

	return 0;
}

int pa_menu_edit_float_selection(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->canvas_editor->selection.box.width > 0 && app->canvas_editor->selection.box.height > 0)
	{
		pa_float_canvas_editor_selection(app->canvas_editor, &app->canvas_editor->selection.box);
		t3f_refresh_menus();
	}
	return 0;
}

int pa_menu_edit_unfloat_selection(int id, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->canvas_editor->selection.bitmap)
	{
		pa_unfloat_canvas_editor_selection(app->canvas_editor, &app->canvas_editor->selection.box);
		pa_clear_canvas_editor_selection(app->canvas_editor);
		t3f_refresh_menus();
	}
	return 0;
}
