/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Title Screen Functions Definitions (Init, Update, Draw, Unload)
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
// Global includes
//----------------------------------------------------------------------------------
extern const int screenWidth;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitTitleScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
}

// Title Screen Update logic
void UpdateTitleScreen(void)
{
    // Press enter or tap to change to GAMEPLAY screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxSelect);
    }
}

// Title Screen Draw logic
void DrawTitleScreen(void)
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
    DrawRectangleRounded(sign_rect, 0.1, 5, hwy_green);
    DrawRectangleRoundedLines(sign_rect, 0.1, 5, 5, WHITE);
    const int center_x = screenWidth / 2;

    const char *title = "Frantic Fourways";
    const float title_size = 60;
    const float title_spacing = 1;
    Vector2 title_v = MeasureTextEx(font, title, title_size, title_spacing);

    DrawTextEx(font, title, (Vector2){center_x - (title_v.x / 2), 80}, title_size, title_spacing, WHITE);

    const char *sub_title = "Press Enter or Tap";
    const float sub_title_size = 40;
    const Vector2 sub_title_v = MeasureTextEx(font, sub_title, sub_title_size, title_spacing);
    DrawTextEx(font, sub_title, (Vector2){center_x - (sub_title_v.x / 2), 180}, sub_title_size, title_spacing, WHITE);
}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
    // TODO: Unload TITLE screen variables here!
}

// Title Screen should finish?
int FinishTitleScreen(void)
{
    return finishScreen;
}