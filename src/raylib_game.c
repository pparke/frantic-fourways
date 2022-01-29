/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "screens.h" // NOTE: Declares global (extern) variables and screens functions

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = 0;
Font font = {0};
Music music = {0};
Sound fxCrash = {0};
Sound fxHonk = {0};
Sound fxSelect = {0};
Sound fxDead = {0};
Sound fxPassed = {0};
Texture2D texPlayer;
Texture2D texBlueCar;
Texture2D texFinish;
const int screenWidth = 450;
const int screenHeight = 800;
int close_calls = 0;
int level = 0;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static int transToScreen = -1;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen); // Change to screen, no transition effect

static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void); // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib game template");

    InitAudioDevice(); // Initialize audio device

    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFontEx("resources/HWYGOTH.ttf", 60, 0, 0);
    GenTextureMipmaps(&font.texture);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    music = LoadMusicStream("resources/xaphoon.ogg");
    fxCrash = LoadSound("resources/crash.wav");
    fxHonk = LoadSound("resources/honk.wav");
    fxSelect = LoadSound("resources/select.wav");
    fxDead = LoadSound("resources/dead.wav");
    fxPassed = LoadSound("resources/passed.wav");

    texPlayer = LoadTexture("resources/player_car.png");
    texBlueCar = LoadTexture("resources/blue_car.png");
    texFinish = LoadTexture("resources/finish.png");

    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);

    // Setup and init first screen
    currentScreen = LOGO;
    InitLogoScreen();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing
    switch (currentScreen)
    {
    case LOGO:
        UnloadLogoScreen();
        break;
    case TITLE:
        UnloadTitleScreen();
        break;
    case GAMEPLAY:
        UnloadGameplayScreen();
        break;
    case ENDING:
        UnloadEndingScreen();
        break;
    default:
        break;
    }

    // Unload global data loaded
    UnloadFont(font);
    UnloadMusicStream(music);
    UnloadSound(fxCrash);
    UnloadSound(fxHonk);

    CloseAudioDevice(); // Close audio context

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen(int screen)
{
    // Unload current screen
    switch (currentScreen)
    {
    case LOGO:
        UnloadLogoScreen();
        break;
    case TITLE:
        UnloadTitleScreen();
        break;
    case GAMEPLAY:
        UnloadGameplayScreen();
        break;
    case ENDING:
        UnloadEndingScreen();
        break;
    default:
        break;
    }

    // Init next screen
    switch (screen)
    {
    case LOGO:
        InitLogoScreen();
        break;
    case TITLE:
        InitTitleScreen();
        break;
    case GAMEPLAY:
        InitGameplayScreen();
        break;
    case ENDING:
        InitEndingScreen();
        break;
    default:
        break;
    }

    currentScreen = screen;
}

// Request transition to next screen
static void TransitionToScreen(int screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen)
            {
            case LOGO:
                UnloadLogoScreen();
                break;
            case TITLE:
                UnloadTitleScreen();
                break;
            case CREDITS:
                UnloadCreditsScreen();
                break;
            case GAMEPLAY:
                UnloadGameplayScreen();
                break;
            case ENDING:
                UnloadEndingScreen();
                break;
            default:
                break;
            }

            // Load next screen
            switch (transToScreen)
            {
            case LOGO:
                InitLogoScreen();
                break;
            case TITLE:
                InitTitleScreen();
                break;
            case GAMEPLAY:
                InitGameplayScreen();
                break;
            case ENDING:
                InitEndingScreen();
                break;
            default:
                break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    }
    else // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = -1;
        }
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    UpdateMusicStream(music); // NOTE: Music keeps playing between screens

    if (!onTransition)
    {
        switch (currentScreen)
        {
        case LOGO:
        {
            UpdateLogoScreen();

            if (FinishLogoScreen())
                TransitionToScreen(TITLE);
        }
        break;
        case TITLE:
        {
            UpdateTitleScreen();

            if (FinishTitleScreen())
                TransitionToScreen(GAMEPLAY);
        }
        break;
        case CREDITS:
        {
            UpdateCreditsScreen();

            if (FinishCreditsScreen())
                TransitionToScreen(TITLE);
        }
        break;
        case GAMEPLAY:
        {
            UpdateGameplayScreen();

            if (FinishGameplayScreen() == 1)
                TransitionToScreen(ENDING);
        }
        break;
        case ENDING:
        {
            UpdateEndingScreen();

            if (FinishEndingScreen() == 1)
                TransitionToScreen(CREDITS);
        }
        break;
        default:
            break;
        }
    }
    else
        UpdateTransition(); // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    switch (currentScreen)
    {
    case LOGO:
        DrawLogoScreen();
        break;
    case TITLE:
        DrawTitleScreen();
        break;
    case CREDITS:
        DrawCreditsScreen();
        break;
    case GAMEPLAY:
        DrawGameplayScreen();
        break;
    case ENDING:
        DrawEndingScreen();
        break;
    default:
        break;
    }

    // Draw full screen rectangle in front of everything
    if (onTransition)
        DrawTransition();

    //DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void DrawCenteredText(Font font, const char *text, int yPosition, float fontSize, float spacing, Color tint)
{
    const int center_x = GetScreenWidth() / 2;
    const Vector2 text_v = MeasureTextEx(font, text, fontSize, spacing);
    DrawTextEx(font, text, (Vector2){center_x - (text_v.x / 2), yPosition}, fontSize, spacing, tint);
}
