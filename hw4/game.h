#ifndef __game_h__
#define __game_h__

#include <stdio.h>
#include <stdlib.h>
#include "gamestate.h"
#include "extensionboard.h"

#ifdef __cplusplus
    extern "C"
    {
#endif
        #include <jansson.h>
	#include "array2d.h"
#ifdef __cplusplus
    }
#endif

class Game 
{
	public:
		int	gameid;
		int 	colors;
		int 	movesmade;
		int 	currentscore;		

		Array2d boardstate;

		Model model;
		ExtensionBoard *extensionBoard;
		GameState *gameState;
		void deserialize(FILE * in);
		void serialize(FILE* out);
		void storeGameDef(int gameid, int colors, Array2d boardstate_gamedef_data);
};

#endif
