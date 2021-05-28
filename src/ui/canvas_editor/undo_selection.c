#include "t3f/t3f.h"
#include "modules/canvas/canvas.h"
#include "modules/canvas/canvas_helpers.h"
#include "canvas_editor.h"
#include "undo.h"

static void copy_bitmap_to_target(ALLEGRO_BITMAP * bp, ALLEGRO_BITMAP * target_bp)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_TRANSFORM identity;

	al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP | ALLEGRO_STATE_TRANSFORM | ALLEGRO_STATE_BLENDER);
	al_identity_transform(&identity);
	al_set_target_bitmap(target_bp);
	al_use_transform(&identity);
	al_clear_to_color(al_map_rgba_f(0.0, 0.0, 0.0, 0.0));
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
	al_draw_bitmap(bp, 0, 0, 0);
	al_restore_state(&old_state);
}

bool quixel_make_unfloat_selection_undo(QUIXEL_CANVAS_EDITOR * cep, const char * fn)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_BITMAP * bp = NULL;
	ALLEGRO_BITMAP * float_bp = NULL;
	ALLEGRO_FILE * fp = NULL;

	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	bp = al_create_bitmap(cep->selection.box.width, cep->selection.box.height);
	al_restore_state(&old_state);
	if(!bp)
	{
		goto fail;
	}
	float_bp = al_create_sub_bitmap(cep->scratch_bitmap, 0, 0, cep->selection.box.width, cep->selection.box.height);
	if(!float_bp)
	{
		goto fail;
	}
	quixel_render_canvas_to_bitmap(cep->canvas, cep->current_layer, cep->current_layer + 1, cep->selection.box.start_x, cep->selection.box.start_y, cep->selection.box.width, cep->selection.box.height, 0, bp);
	fp = al_fopen(fn, "wb");
	if(!fp)
	{
		printf("fail: %s\n", fn);
		goto fail;
	}
	quixel_write_undo_header(fp, QUIXEL_UNDO_TYPE_UNFLOAT_SELECTION, "Unfloat Selection");
	al_fwrite32le(fp, cep->current_layer);
	al_fwrite32le(fp, cep->selection.box.start_x);
	al_fwrite32le(fp, cep->selection.box.start_y);
	al_fwrite32le(fp, cep->selection.box.width);
	al_fwrite32le(fp, cep->selection.box.height);
	if(!al_save_bitmap_f(fp, ".png", bp))
	{
		printf("fail: %s\n", fn);
		goto fail;
	}

	if(!al_save_bitmap_f(fp, ".png", float_bp))
	{
		printf("fail: %s\n", fn);
		goto fail;
	}
	al_destroy_bitmap(bp);
	al_destroy_bitmap(float_bp);
	al_fclose(fp);
	return true;

	fail:
	{
		if(fp)
		{
			al_fclose(fp);
		}
		if(bp)
		{
			al_destroy_bitmap(bp);
		}
		if(float_bp)
		{
			al_destroy_bitmap(bp);
		}
		return false;
	}
}

bool quixel_make_unfloat_selection_redo(QUIXEL_CANVAS_EDITOR * cep, const char * fn)
{
	return quixel_make_unfloat_selection_undo(cep, fn);
}

bool quixel_make_float_selection_undo(QUIXEL_CANVAS_EDITOR * cep, const char * fn)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_BITMAP * bp = NULL;
	ALLEGRO_FILE * fp = NULL;

	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	bp = al_create_bitmap(cep->selection.box.width, cep->selection.box.height);
	al_restore_state(&old_state);
	if(!bp)
	{
		goto fail;
	}
	quixel_render_canvas_to_bitmap(cep->canvas, cep->current_layer, cep->current_layer + 1, cep->selection.box.start_x, cep->selection.box.start_y, cep->selection.box.width, cep->selection.box.height, 0, bp);
	fp = al_fopen(fn, "wb");
	if(!fp)
	{
		printf("fail: %s\n", fn);
		goto fail;
	}
	quixel_write_undo_header(fp, QUIXEL_UNDO_TYPE_FLOAT_SELECTION, "Float Selection");
	al_fwrite32le(fp, cep->current_layer);
	al_fwrite32le(fp, cep->selection.box.start_x);
	al_fwrite32le(fp, cep->selection.box.start_y);
	al_fwrite32le(fp, cep->selection.box.width);
	al_fwrite32le(fp, cep->selection.box.height);
	if(!al_save_bitmap_f(fp, ".png", bp))
	{
		printf("fail: %s\n", fn);
		goto fail;
	}
	al_destroy_bitmap(bp);
	al_fclose(fp);
	return true;

	fail:
	{
		if(fp)
		{
			al_fclose(fp);
		}
		if(bp)
		{
			al_destroy_bitmap(bp);
		}
		return false;
	}
}

bool quixel_make_float_selection_redo(QUIXEL_CANVAS_EDITOR * cep, const char * fn)
{
	return quixel_make_float_selection_undo(cep, fn);
}

bool quixel_apply_unfloat_selection_undo(QUIXEL_CANVAS_EDITOR * cep, ALLEGRO_FILE * fp, const char * action, bool revert)
{
	char undo_path[1024];
	ALLEGRO_BITMAP * bp = NULL;
	int layer;

	quixel_make_unfloat_selection_redo(cep, quixel_get_undo_path("redo", cep->redo_count, undo_path, 1024));
	cep->redo_count++;
	layer = al_fread32le(fp);
	cep->selection.box.start_x = al_fread32le(fp);
	cep->selection.box.start_y = al_fread32le(fp);
	cep->selection.box.width = al_fread32le(fp);
	cep->selection.box.height = al_fread32le(fp);
	bp = al_load_bitmap_flags_f(fp, ".png", ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	if(!bp)
	{
		goto fail;
	}
	quixel_import_bitmap_to_canvas(cep->canvas, bp, layer, cep->selection.box.start_x, cep->selection.box.start_y);
	al_destroy_bitmap(bp);
	bp = al_load_bitmap_flags_f(fp, ".png", ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	if(!bp)
	{
		goto fail;
	}
	copy_bitmap_to_target(bp, cep->scratch_bitmap);
	al_destroy_bitmap(bp);
	cep->selection.floating = true;
	return true;

	fail:
	{
		if(bp)
		{
			al_destroy_bitmap(bp);
		}
		return false;
	}
}

bool quixel_apply_float_selection_undo(QUIXEL_CANVAS_EDITOR * cep, ALLEGRO_FILE * fp, const char * action, bool revert)
{
	char undo_path[1024];
	ALLEGRO_BITMAP * bp = NULL;
	int layer;

	quixel_make_float_selection_redo(cep, quixel_get_undo_path("redo", cep->redo_count, undo_path, 1024));
	cep->redo_count++;
	layer = al_fread32le(fp);
	cep->selection.box.start_x = al_fread32le(fp);
	cep->selection.box.start_y = al_fread32le(fp);
	cep->selection.box.width = al_fread32le(fp);
	cep->selection.box.height = al_fread32le(fp);
	quixel_initialize_box(&cep->selection.box, cep->selection.box.start_x, cep->selection.box.start_y, cep->selection.box.width, cep->selection.box.height, cep->selection.box.bitmap);
	quixel_update_box_handles(&cep->selection.box, cep->view_x, cep->view_y, cep->view_zoom);
	bp = al_load_bitmap_flags_f(fp, ".png", ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	if(!bp)
	{
		goto fail;
	}
	quixel_import_bitmap_to_canvas(cep->canvas, bp, layer, cep->selection.box.start_x, cep->selection.box.start_y);
	cep->selection.floating = false;
	al_destroy_bitmap(bp);
	cep->selection.floating = false;
	return true;

	fail:
	{
		if(bp)
		{
			al_destroy_bitmap(bp);
		}
		return false;
	}
}

bool quixel_apply_unfloat_selection_redo(QUIXEL_CANVAS_EDITOR * cep, ALLEGRO_FILE * fp, const char * action)
{
	char undo_path[1024];
	ALLEGRO_BITMAP * bp = NULL;
	int layer;

	quixel_make_unfloat_selection_undo(cep, quixel_get_undo_path("undo", cep->undo_count, undo_path, 1024));
	cep->undo_count++;
	layer = al_fread32le(fp);
	cep->selection.box.start_x = al_fread32le(fp);
	cep->selection.box.start_y = al_fread32le(fp);
	cep->selection.box.width = al_fread32le(fp);
	cep->selection.box.height = al_fread32le(fp);
	bp = al_load_bitmap_flags_f(fp, ".png", ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	if(!bp)
	{
		goto fail;
	}
	quixel_import_bitmap_to_canvas(cep->canvas, bp, layer, cep->selection.box.start_x, cep->selection.box.start_y);
	al_destroy_bitmap(bp);
	bp = al_load_bitmap_flags_f(fp, ".png", ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	if(!bp)
	{
		goto fail;
	}
	copy_bitmap_to_target(bp, cep->scratch_bitmap);
	al_destroy_bitmap(bp);
	cep->selection.floating = true;
	return true;

	fail:
	{
		if(bp)
		{
			al_destroy_bitmap(bp);
		}
		return false;
	}
}

bool quixel_apply_float_selection_redo(QUIXEL_CANVAS_EDITOR * cep, ALLEGRO_FILE * fp, const char * action)
{
	char undo_path[1024];
	ALLEGRO_BITMAP * bp = NULL;
	int layer;

	quixel_make_float_selection_undo(cep, quixel_get_undo_path("undo", cep->undo_count, undo_path, 1024));
	cep->undo_count++;
	layer = al_fread32le(fp);
	cep->selection.box.start_x = al_fread32le(fp);
	cep->selection.box.start_y = al_fread32le(fp);
	cep->selection.box.width = al_fread32le(fp);
	cep->selection.box.height = al_fread32le(fp);
	quixel_initialize_box(&cep->selection.box, cep->selection.box.start_x, cep->selection.box.start_y, cep->selection.box.width, cep->selection.box.height, cep->selection.box.bitmap);
	quixel_update_box_handles(&cep->selection.box, cep->view_x, cep->view_y, cep->view_zoom);
	bp = al_load_bitmap_flags_f(fp, ".png", ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	if(!bp)
	{
		goto fail;
	}
	copy_bitmap_to_target(bp, cep->scratch_bitmap);
	al_destroy_bitmap(bp);
	cep->selection.floating = true;
	return true;

	fail:
	{
		if(bp)
		{
			al_destroy_bitmap(bp);
		}
		return false;
	}
}