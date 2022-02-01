#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "raylib.h"

#define MAX_ROADS 5
#define MAX_CARS 20
#define NUM_LEVELS 6

typedef struct Player
{
  Rectangle rect;
  Texture2D *texture;
  int speed;
  int max_speed;
  int acceleration;
  Color color;
} Player;

typedef struct Car
{
  Rectangle rect;
  Texture2D *texture;
  bool honked;
  int speed;
  int start_offset;
  Color color;
} Car;

typedef enum RoadDirection
{
  NS = 0,
  WE
} RoadDirection;

typedef struct Road
{
  Rectangle asphalt;
  Rectangle sidewalk;
  Rectangle line;
  RoadDirection direction;
  int num_cars;
  int car_spacing;
} Road;

typedef struct Level
{
  int num_roads;
  int road_offsets[MAX_ROADS];
  int cars_per_road[MAX_ROADS];
  int car_spacing[MAX_ROADS];
  int car_speeds[MAX_ROADS];
} Level;

void NextLevel(void);
void LoadLevel(void);
void InitRoad(Road *road, RoadDirection dir, int offset);
void DrawRoads();
void RandomLevel(Level *level);
int RandInRange(int min, int max);
void AddRoad(Road road);
void AddCar(Car car);

#endif