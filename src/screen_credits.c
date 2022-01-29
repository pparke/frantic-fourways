/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Credits Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Credits Screen Functions Definition
//----------------------------------------------------------------------------------

// Credits Screen Initialization logic
void InitCreditsScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
}

// Credits Screen Update logic
void UpdateCreditsScreen(void)
{
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxSelect);
    }
}

// Credits Screen Draw logic
void DrawCreditsScreen(void)
{
    const float title_size = 60;
    const float text_size = 20;
    const int spacing = 1;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

    DrawCenteredText(font, "Credits", 60, title_size, spacing, WHITE);
    DrawCenteredText(font, "Game Engine: Ramon Santamaria (@raysan5)", 160, text_size, spacing, WHITE);
    DrawCenteredText(font, "Music: xaphoon by Apoxode", 200, text_size, spacing, WHITE);
    DrawCenteredText(font, "Font: Highway Gothic by Tom Oetken", 240, text_size, spacing, WHITE);
    DrawCenteredText(font, "Car Sprites: Muhammad Amiruddin", 280, text_size, spacing, WHITE);
}

// Credits Screen Unload logic
void UnloadCreditsScreen(void)
{
    // TODO: Unload Credits screen variables here!
}

// Credits Screen should finish?
int FinishCreditsScreen(void)
{
    return finishScreen;
}