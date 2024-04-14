#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
typedef struct {
    bool isActive;
    bool isSelected;
    float x, y;
    int w, h;
    SDL_Point target;
    SDL_Color color;
    float speed; //should this be a float?
    SDL_Texture* texture;
} Entity;

void move_entity(Entity *entity);
void render_entity(SDL_Renderer *renderer, Entity *entity);
void generate_random_entity(Entity *entity);
