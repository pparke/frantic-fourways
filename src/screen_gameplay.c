/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
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
*
*
*
*
*   3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "screens.h"
#include "gameplay.h"

//----------------------------------------------------------------------------------
// Global includes
//----------------------------------------------------------------------------------
extern const int screenHeight;
extern const int screenWidth;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static int lives = 5;
static Rectangle goal;
static Player player;
static Car cars[MAX_CARS];
static int next_car = 0;
static time_t t;
static char level_text[80];
static Road player_road;
static Road car_roads[MAX_ROADS];
static int next_road = 0;
static Color grass_green = {30, 157, 53, 255};

static const int num_levels = 6;
static Level levels[num_levels] = {
    {.num_roads = 1, .road_offsets = {200}, .cars_per_road = {1}, .car_spacing = {0}, .car_speeds = {10}},
    {.num_roads = 2, .road_offsets = {200, 400}, .cars_per_road = {1, 1}, .car_spacing = {0, 0}, .car_speeds = {10, 5}},
    {.num_roads = 1, .road_offsets = {350}, .cars_per_road = {2}, .car_spacing = {60}, .car_speeds = {10}},
    {.num_roads = 2, .road_offsets = {300, 345}, .cars_per_road = {2, 2}, .car_spacing = {110, 160}, .car_speeds = {5, 8}},
    {.num_roads = 3, .road_offsets = {200, 300, 400}, .cars_per_road = {1, 1, 1}, .car_spacing = {0, 0, 0}, .car_speeds = {10, 8, 5}},
    {.num_roads = 2, .road_offsets = {280, 420}, .cars_per_road = {3, 3}, .car_spacing = {160, 120}, .car_speeds = {5, 8}},
};
static Level random_level = {};
static Level *current_level;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    /* Intializes random number generator */
    srand((unsigned)time(&t));

    framesCounter = 0;
    finishScreen = 0;
    level = 0;
    lives = 5;
    close_calls = 0;
    sprintf(level_text, "Level %d", 1);

    goal.y = screenHeight - 20;
    goal.x = 0;
    goal.width = screenWidth;
    goal.height = 20;

    InitRoad(&player_road, NS, screenWidth / 2 - 20);

    player.rect.x = screenWidth / 2 - 5;
    player.rect.y = 5;
    player.rect.width = 20;
    player.rect.height = 40;
    player.speed = 0;
    player.max_speed = 25;
    player.acceleration = 1;
    player.color = RED;
    player.texture = &texPlayer;

    LoadLevel();
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsGestureDetected(GESTURE_TAP)) && player.speed > 0)
    {
        player.speed -= player.acceleration;
    }
    else if (IsKeyDown(KEY_DOWN) || || IsKeyDown(KEY_S) || IsGestureDetected(GESTURE_HOLD))
    {
        player.speed += player.acceleration;
    }

    player.rect.y += player.speed;

    if (CheckCollisionRecs(player.rect, goal))
    {
        PlaySound(fxPassed);
        NextLevel();
    }

    for (int i = 0; i < next_car; i++)
    {
        cars[i].rect.x += cars[i].speed;
        if (CheckCollisionRecs(player.rect, cars[i].rect))
        {
            PlaySound(fxCrash);
            lives -= 1;
            if (lives == 0)
            {
                finishScreen = 1;
                PlaySound(fxDead);
                return;
            }
            player.rect.y = 5;
            player.speed = 0;
            break;
        }
        Rectangle honk_zone = {
            .x = cars[i].rect.x + cars[i].rect.width,
            .y = cars[i].rect.y,
            .width = 20,
            .height = cars[i].rect.height};
        if (!cars[i].honked && CheckCollisionRecs(player.rect, honk_zone))
        {
            cars[i].honked = true;
            close_calls += 1;
            PlaySound(fxHonk);
        }

        if (cars[i].rect.x > screenWidth)
        {
            cars[i].rect.x = -cars[i].rect.width + 5;
        }
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), grass_green);
    DrawRoads(car_roads);
    if (level == 0)
    {
        DrawTextEx(font, "PRESS DOWN OR                 LONG PRESS TO DRIVE", (Vector2){30, 210}, 20, 1, YELLOW);
    }
    for (int i = 0; i < next_car; i++)
    {
        DrawRectangleRec(cars[i].rect, cars[i].color);
        DrawTexturePro(*cars[i].texture, (Rectangle){0, 0, -64, 32}, cars[i].rect, (Vector2){0, 0}, 0, WHITE);
    }
    DrawRectangleRec(player.rect, player.color);
    DrawTextureEx(*player.texture, (Vector2){player.rect.x, player.rect.y + player.rect.height}, 270, 0.625, WHITE);

    char lives_text[80];
    sprintf(lives_text, "Lives %d", lives);

    DrawTextEx(font, level_text, (Vector2){20, 10}, 30, 4, WHITE);
    DrawTextEx(font, lives_text, (Vector2){GetScreenWidth() - 130, 10}, 30, 4, WHITE);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // erase cars array
    memset(cars, 0, MAX_CARS * sizeof(Car));
    next_car = 0;
    // erase roads array
    memset(car_roads, 0, MAX_ROADS * sizeof(Road));
    next_road = 0;
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

void NextLevel(void)
{
    level += 1;

    sprintf(level_text, "Level %d", level + 1);
    player.rect.y = 5;
    player.speed = 0;

    // erase cars array
    memset(cars, 0, MAX_CARS * sizeof(Car));
    next_car = 0;
    // erase roads array
    memset(car_roads, 0, MAX_ROADS * sizeof(Road));
    next_road = 0;

    LoadLevel();
}

void LoadLevel()
{
    if (level >= num_levels)
    {
        RandomLevel(&random_level);
        current_level = &random_level;
    }
    else
    {
        current_level = &levels[level];
    }

    for (int i = 0; i < current_level->num_roads; i++)
    {
        InitRoad(&car_roads[i], WE, current_level->road_offsets[i]);
        for (int j = 0; j < current_level->cars_per_road[i]; j++)
        {
            Car car;
            car.rect.x = -(current_level->car_spacing[i] * j);
            car.rect.y = current_level->road_offsets[i] + 10;
            car.rect.width = 40;
            car.rect.height = 20;
            car.speed = current_level->car_speeds[i];
            car.color = BLUE;
            car.texture = &texBlueCar;
            AddCar(car);
        }
    }
}

void InitRoad(Road *road, enum RoadDirection dir, int offset)
{
    if (dir == NS)
    {
        road->asphalt.y = 0;
        road->asphalt.x = offset;
        road->asphalt.width = 40;
        road->asphalt.height = screenHeight;

        road->sidewalk.y = 0;
        road->sidewalk.x = offset - 5;
        road->sidewalk.width = 50;
        road->sidewalk.height = screenHeight;
    }
    else if (dir == WE)
    {
        road->asphalt.y = offset;
        road->asphalt.x = 0;
        road->asphalt.width = screenWidth;
        road->asphalt.height = 40;

        road->sidewalk.y = offset - 5;
        road->sidewalk.x = 0;
        road->sidewalk.width = screenWidth;
        road->sidewalk.height = 50;
    }
}

void DrawRoads()
{
    DrawRectangleRec(player_road.sidewalk, GRAY);
    for (int i = 0; i < current_level->num_roads; i++)
    {
        DrawRectangleRec(car_roads[i].sidewalk, GRAY);
    }
    DrawRectangleRec(player_road.asphalt, BLACK);
    for (int i = 0; i < current_level->num_roads; i++)
    {
        DrawRectangleRec(car_roads[i].asphalt, BLACK);
    }
    DrawTexture(texFinish, (GetScreenWidth() / 2) - 20, GetScreenHeight() - 20, WHITE);
}

void AddRoad(Road road)
{
    if (next_road < MAX_ROADS)
    {
        car_roads[next_road] = road;
        next_road += 1;
    }
}

int RandInRange(int min, int max)
{
    int diff = max - min;
    return (rand() % diff) + min;
}

void RandomLevel(Level *level)
{
    int num = RandInRange(1, MAX_ROADS);
    level->num_roads = num;

    for (int i = 0; i < num; i++)
    {
        bool ok = false;
        int loop_count = 0;
        while (!ok && loop_count < 10000)
        {
            loop_count += 1;
            level->road_offsets[i] = RandInRange(200, GetScreenHeight() - 100);
            // check for overlaps
            for (int j = 0; j < i; j++)
            {
                int distance = level->road_offsets[i] - level->road_offsets[j];
                if (distance < 55 && distance > -55)
                {
                    // road overlaps too much
                    ok = false;
                    break;
                }
                ok = true;
            }
        }

        level->cars_per_road[i] = RandInRange(1, 4);
        int max_spacing = (GetScreenWidth() / level->cars_per_road[i]) - 50;
        level->car_spacing[i] = RandInRange(50, max_spacing);
        level->car_speeds[i] = RandInRange(3, 12);
    }
}

void AddCar(Car car)
{
    if (next_car < MAX_CARS)
    {
        cars[next_car].rect = car.rect;
        cars[next_car].speed = car.speed;
        cars[next_car].start_offset = car.start_offset;
        cars[next_car].color = car.color;
        cars[next_car].texture = car.texture;
        next_car += 1;
    }
}