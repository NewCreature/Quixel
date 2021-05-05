#include "instance.h"
#include "t3gui/t3gui.h"
#include "ui/canvas_editor/canvas_editor.h"
#include "modules/canvas/canvas_helpers.h"
#include "modules/primitives.h"

static void change_tool(QUIXEL_CANVAS_EDITOR * cep, int tool)
{
	if(cep->current_tool == QUIXEL_TOOL_SELECTION && tool != QUIXEL_TOOL_SELECTION)
	{
		quixel_unfloat_canvas_editor_selection(cep, &cep->selection.box);
		cep->selection.box.width = 0;
		cep->selection.box.height = 0;
	}
	cep->current_tool = tool;
}

int quixel_tool_pixel_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	APP_INSTANCE * app = (APP_INSTANCE *)dp3;

	change_tool(app->canvas_editor, QUIXEL_TOOL_PIXEL);

	return 0;
}

int quixel_tool_line_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	APP_INSTANCE * app = (APP_INSTANCE *)dp3;

	change_tool(app->canvas_editor, QUIXEL_TOOL_LINE);

	return 0;
}

int quixel_tool_rectangle_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	APP_INSTANCE * app = (APP_INSTANCE *)dp3;

	change_tool(app->canvas_editor, QUIXEL_TOOL_RECTANGLE);

	return 0;
}

int quixel_tool_filled_rectangle_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	APP_INSTANCE * app = (APP_INSTANCE *)dp3;

	change_tool(app->canvas_editor, QUIXEL_TOOL_FILLED_RECTANGLE);

	return 0;
}

int quixel_tool_oval_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	APP_INSTANCE * app = (APP_INSTANCE *)dp3;

	change_tool(app->canvas_editor, QUIXEL_TOOL_OVAL);

	return 0;
}

int quixel_tool_filled_oval_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	APP_INSTANCE * app = (APP_INSTANCE *)dp3;

	change_tool(app->canvas_editor, QUIXEL_TOOL_FILLED_OVAL);

	return 0;
}

int quixel_tool_flood_fill_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	APP_INSTANCE * app = (APP_INSTANCE *)dp3;

	change_tool(app->canvas_editor, QUIXEL_TOOL_FLOOD_FILL);

	return 0;
}

int quixel_tool_dropper_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	APP_INSTANCE * app = (APP_INSTANCE *)dp3;

	change_tool(app->canvas_editor, QUIXEL_TOOL_DROPPER);

	return 0;
}

int quixel_tool_selection_button_proc(T3GUI_ELEMENT * d, void *dp3)
{
	APP_INSTANCE * app = (APP_INSTANCE *)dp3;

	change_tool(app->canvas_editor, QUIXEL_TOOL_SELECTION);

	return 0;
}