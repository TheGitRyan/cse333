#include "game.h"
#include "gamestate.h"
#include "extensionboard.h"
#include <stdlib.h>

#ifndef __cplusplus
extern "C" {
#endif
	#include "model.h"
	#include "array2d.h"
#ifndef __cplusplus
}
#endif

int hashCandy(int color, int state) {
	return color;
}

void Game::deserialize(FILE *in)
{

	json_t *game_info;
	json_error_t error;

	game_info = json_loadf(in, 0, &error);

	Array2d extensioncolors_data;

	ExtensionBoard *eb = new ExtensionBoard();
        Model model;
        GameState *gameState = new GameState();
		
	json_t *gamedef = json_object_get(game_info, "gamedef");
	
	if (gamedef == NULL) {
		printf("No gamedef in GameInstance json object\n");
		exit (EXIT_FAILURE);
	}


	// game def
	json_t *gameid_json = json_object_get(gamedef, "gameid");
	json_t *extensioncolor_json = json_object_get(gamedef, "extensioncolor");
	json_t *boardstate_gamedef_json = json_object_get(gamedef, "boardstate");
	json_t *colors_json = json_object_get(gamedef, "colors");
	// get game def info
	int gameid = json_integer_value(gameid_json);
	int colors = json_integer_value(colors_json);

	// extension color info
	json_t *extensioncolor_rows_json = json_object_get(extensioncolor_json, "rows");
	json_t *extensioncolor_cols_json = json_object_get(extensioncolor_json, "columns");
	int extensioncolor_rows = json_integer_value(extensioncolor_rows_json);
	int extensioncolor_cols = json_integer_value(extensioncolor_cols_json);

	json_t *extensioncolors_data_json = json_object_get(extensioncolor_json, "data");
       	extensioncolors_data = Array2d_get_array_data(extensioncolors_data_json, extensioncolor_rows, extensioncolor_cols);
	
	// board state info
	json_t *boardstate_gamedef_rows_json = json_object_get(boardstate_gamedef_json, "rows");
	json_t *boardstate_gamedef_cols_json = json_object_get(boardstate_gamedef_json, "columns");
	int boardstate_gamedef_rows = json_integer_value(boardstate_gamedef_rows_json);
	int boardstate_gamedef_cols = json_integer_value(boardstate_gamedef_cols_json);
	json_t *boardstate_gamedef_data_json = json_object_get(boardstate_gamedef_json, "data");
	Array2d boardstate_gamedef_data = Array2d_get_array_data(boardstate_gamedef_data_json, boardstate_gamedef_rows, boardstate_gamedef_cols);

	if (json_object_size(game_info)== (size_t) 1) {
		eb->setBoard(extensioncolors_data, NULL);

        	Array2d modelArr = (Array2d)malloc(sizeof(Array2d_s));
		modelArr->rows = boardstate_gamedef_rows;
		modelArr->cols = boardstate_gamedef_cols;
		modelArr->pData = (Array2d_value_t*) malloc(sizeof(Array2d_value_t) * modelArr->rows * modelArr->cols);
		for (int i = 0; i < boardstate_gamedef_rows; i++) {
			for (int j = 0; j < boardstate_gamedef_cols; j++) {
				int *vals = eb->get(j, 1);
				Array2d_set(modelArr, i, j, (Array2d_value_t)vals[0]);
				free(vals);
			}
		}
		model = model_load(modelArr);
		
        	gameState->setBoard(boardstate_gamedef_data);
		Array2d boardstate_gamedef_data_copy = Array2d_get_array_data(boardstate_gamedef_data_json, boardstate_gamedef_rows, boardstate_gamedef_cols);
        	this->storeGameDef(gameid, colors, boardstate_gamedef_data_copy);
	} else {
		json_t *gamestate = json_object_get(game_info, "gamestate");	
		// game state
		json_t *boardcandies_json = json_object_get(gamestate, "boardcandies");
		json_t *boardstate_gamestate_json = json_object_get(gamestate, "boardstate");
		json_t *movesmade_json = json_object_get(gamestate, "movesmade");
		json_t *currentscore_json = json_object_get(gamestate, "currentscore");

		// boardcandies info and array
		json_t *boardcandies_rows_json = json_object_get(boardcandies_json, "rows");
		json_t *boardcandies_cols_json = json_object_get(boardcandies_json, "columns");
		int boardcandies_rows = json_integer_value(boardcandies_rows_json);
		int boardcandies_cols = json_integer_value(boardcandies_cols_json);
		
		json_t *boardcandies_data_json = json_object_get(boardcandies_json, "data");
		Array2d boardcandies_array = (Array2d)malloc(sizeof(Array2d_s));
	       	if (boardcandies_array == NULL) {
	                printf("err out of memory");
	                return;
	        }

        	boardcandies_array->pData = (Array2d_value_t*)malloc(sizeof(Array2d_value_t) * boardcandies_rows * boardcandies_cols);
		boardcandies_array->rows = boardcandies_rows;
	        boardcandies_array->cols = boardcandies_cols;
		
		for (int i = 0; i < boardcandies_rows; i++) {
			for (int j = 0; j < boardcandies_cols; j++) {
				int offset = i * boardcandies_rows + j;
				json_t *element = json_array_get(boardcandies_data_json, (size_t)offset);
				json_t *element_color_json = json_object_get(element, "color");
				json_t *element_type_json = json_object_get(element, "type");
				int element_color = json_integer_value(element_color_json);
				int element_type = json_integer_value(element_type_json);
				*(boardcandies_array->pData + offset) = (Array2d_value_t)hashCandy(element_color, element_type);
			}
		}

		// board state array
		json_t *boardstate_gamestate_rows_json = json_object_get(boardstate_gamestate_json, "rows");
	        json_t *boardstate_gamestate_cols_json = json_object_get(boardstate_gamestate_json, "columns");
	        int boardstate_gamestate_rows = json_integer_value(boardstate_gamestate_rows_json);
	        int boardstate_gamestate_cols = json_integer_value(boardstate_gamestate_cols_json);
	        json_t *boardstate_gamestate_data_json = json_object_get(boardstate_gamestate_json, "data");
	        Array2d boardstate_gamestate_data = Array2d_get_array_data(boardstate_gamestate_data_json, boardstate_gamestate_rows, boardstate_gamestate_cols);

		int movesmade = json_integer_value(movesmade_json);
		this->movesmade = movesmade;
		int currentscore = json_integer_value(currentscore_json);
		this->currentscore = currentscore;
	
		// extension offset array
		json_t *extensionoffset_json = json_object_get(gamestate, "extensionoffset");
		Array2d extensionoffset = Array2d_get_array_data(extensionoffset_json, boardcandies_rows, 1); 
		
		eb->setBoard(extensioncolors_data, extensionoffset);
                model = model_load(boardcandies_array);
                gameState->setBoard(boardstate_gamestate_data);
                this->storeGameDef(gameid, colors, boardstate_gamedef_data);
	}
	this->gameState = gameState;
	this->extensionBoard = eb;
	this->model = model;
}

void Game::serialize(FILE* out) {
	ExtensionBoard *eb = this->extensionBoard;
	GameState *gameState = this->gameState;
	
	Model model = this->model;
	Array2d boardState = this->boardstate;

	//master instance
	json_t* gameInstance = json_object();
	//game def and gamestate objects
	json_t* gamedef = json_object();
	json_t* gamestate = json_object();
	

	//game id create and add
	json_t* gameid_json = json_integer(this->gameid);
	json_object_set_new(gamedef, "gameid", gameid_json);

	json_t* extensioncolor = json_object();
	Array2d extensioncolor_arr = eb->board;

	json_t* extensioncolor_arr_rows = json_integer(extensioncolor_arr->rows);
	json_t* extensioncolor_arr_cols = json_integer(extensioncolor_arr->cols);

	json_object_set_new(extensioncolor, "row", extensioncolor_arr_rows);
	json_object_set_new(extensioncolor, "columns", extensioncolor_arr_cols);
	
	json_t* extensioncolor_arr_json = json_array();

	for (int i = 0; i < extensioncolor_arr->rows; i++) {
		for (int j = 0; j < extensioncolor_arr->cols; j++) {
			int val = (intptr_t)Array2d_get(extensioncolor_arr, i, j);
			json_array_append_new(extensioncolor_arr_json,  json_integer(val));
		}
	}
	
	json_object_set_new(extensioncolor, "data", extensioncolor_arr_json);
	json_object_set_new(gamedef, "extensioncolor", extensioncolor);

	json_t* boardstate_json = json_object();

	json_t* boardstate_rows_json = json_integer(boardState->rows);
	json_t* boardstate_cols_json = json_integer(boardState->cols);

	json_object_set_new(boardstate_json, "rows", boardstate_rows_json);
	json_object_set_new(boardstate_json, "columns", boardstate_cols_json);

	json_t* boardstate_arr_json = json_array();	

	for (int i = 0; i < boardState->rows; i++) {
                for (int j = 0; j < boardState->cols; j++) {
                        int val = (intptr_t)Array2d_get(boardState, i, j);
                        json_array_append_new(boardstate_arr_json,  json_integer(val));
                }
        }
	
	json_object_set_new(boardstate_json, "data", boardstate_arr_json);

	json_object_set_new(gamedef, "boardstate", boardstate_json);
	json_t* colors_json = json_integer(this->colors);

	json_object_set_new(gamedef, "colors", colors_json);
	//game def finished now creating gamestate

	json_t* boardcandies_json = json_object();

	json_t* boardcandies_rows_json = json_integer(model->image_vals->rows);
	json_object_set_new(boardcandies_json, "rows", boardcandies_rows_json);
	json_t* boardcandies_cols_json = json_integer(model->image_vals->cols);
	json_object_set_new(boardcandies_json, "columns", boardcandies_cols_json);
	
	json_t* boardcandies_arr_json = json_array();

	for (int i = 0; i < model->image_vals->rows; i++) {
                for (int j = 0; j < model->image_vals->cols; j++) {
                        int val = (intptr_t)Array2d_get(model->image_vals, i, j);
			json_t* val_json = json_object();
			json_object_set_new(val_json, "color", json_integer(val));
			json_object_set_new(val_json, "type", json_integer(0));

                        json_array_append_new(boardcandies_arr_json, val_json);
                }
        }
	
	json_object_set_new(boardcandies_json, "data", boardcandies_arr_json);
	json_object_set_new(gamestate, "boardcandies", boardcandies_json);
	

	json_t* boardstate_gamedef_json = json_object();
	
	Array2d boardstate_gamedef_arr = gameState->getBoard();
	
	json_t* boardstate_gamedef_rows_json = json_integer(boardstate_gamedef_arr->rows);
	json_object_set_new(boardstate_gamedef_json, "rows", boardstate_gamedef_rows_json);

	json_t* boardstate_gamedef_cols_json = json_integer(boardstate_gamedef_arr->cols);
	json_object_set_new(boardstate_gamedef_json, "columns", boardstate_gamedef_cols_json);

	json_t* boardstate_gamedef_arr_json = json_array();
	
	for (int i = 0; i < boardstate_gamedef_arr->rows; i++) {
		for (int j = 0; j < boardstate_gamedef_arr->cols; j++) {
			int val = (intptr_t)Array2d_get(boardstate_gamedef_arr, i, j);
			json_array_append_new(boardstate_gamedef_arr_json, json_integer(val));
		}
	}

	json_object_set_new(boardstate_gamedef_json, "data", boardstate_gamedef_arr_json);
	json_object_set_new(gamestate, "boardstate", boardstate_gamedef_json);

	json_t* movesmade_json = json_integer(this->movesmade);
	json_object_set_new(gamestate, "movesmade", movesmade_json);

	json_t* currentscore_json = json_integer(this->currentscore);
	json_object_set_new(gamestate, "currentscore", currentscore_json);
	Array2d offsets_arr = eb->getOffsets();

	json_t* offsets_arr_json = json_array();

	for (int j = 0; j < offsets_arr->cols; j++) {
		int val = (intptr_t)Array2d_get(offsets_arr, 0, j);
		json_array_append_new(offsets_arr_json, json_integer(val));
	}
	
	json_object_set_new(gamestate, "extensionsoffset", offsets_arr_json);
	
	json_object_set_new(gameInstance, "gamedef", gamedef);
	json_object_set_new(gameInstance, "gamestate", gamestate);

	json_dumpf(gameInstance, out, 0);
	json_decref(gameInstance);
}

void Game::storeGameDef(int gameid, int colors, Array2d boardstate) {
	this->gameid = gameid;
	this->colors = colors;
	this->boardstate = boardstate;
}
