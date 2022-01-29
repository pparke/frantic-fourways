/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Ending Screen Functions Definitions (Init, Update, Draw, Unload)
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

#include <stdio.h>
#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

// Ending Screen Initialization logic
void InitEndingScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
}

// Ending Screen Update logic
void UpdateEndingScreen(void)
{
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxSelect);
    }
}

// Ending Screen Draw logic
void DrawEndingScreen(void)
{
    Color hwy_green;
    hwy_green.r = 0;
    hwy_green.g = 133;
    hwy_green.b = 61;
    hwy_green.a = 255;
    Rectangle sign_rect = {10, 40, GetScreenWidth() - 20, 300};
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
    DrawRectangle(60, 160, 30, GetScreenHeight(), GRAY);
    DrawRectangle(GetScreenWidth() - 90, 160, 30, GetScreenHeight(), GRAY);
    DrawRectangleRounded(sign_rect, 0.1, 5, RED);
    DrawRectangleRoundedLines(sign_rect, 0.1, 5, 5, WHITE);
    const int center_x = screenWidth / 2;

    const char *title = "GAME OVER";
    const float title_size = 60;
    const float title_spacing = 1;
    Vector2 title_v = MeasureTextEx(font, title, title_size, title_spacing);

    DrawTextEx(font, title, (Vector2){center_x - (title_v.x / 2), 80}, title_size, title_spacing, WHITE);

    const char *sub_title = "Press Enter or Tap";
    const float sub_title_size = 40;
    const Vector2 sub_title_v = MeasureTextEx(font, sub_title, sub_title_size, title_spacing);
    DrawTextEx(font, sub_title, (Vector2){center_x - (sub_title_v.x / 2), 250}, sub_title_size, title_spacing, WHITE);

    char close_call_text[80];
    sprintf(close_call_text, "Close Calls %d", close_calls);
    const Vector2 close_call_v = MeasureTextEx(font, close_call_text, sub_title_size, title_spacing);
    DrawTextEx(font, close_call_text, (Vector2){center_x - (close_call_v.x / 2), 140}, sub_title_size, title_spacing, WHITE);

    char level_text[80];
    sprintf(level_text, "Level %d", level + 1);
    const Vector2 level_v = MeasureTextEx(font, level_text, sub_title_size, title_spacing);
    DrawTextEx(font, level_text, (Vector2){center_x - (level_v.x / 2), 180}, sub_title_size, title_spacing, WHITE);
}

// Ending Screen Unload logic
void UnloadEndingScreen(void)
{
    // TODO: Unload ENDING screen variables here!
}

// Ending Screen should finish?
int FinishEndingScreen(void)
{
    return finishScreen;
}