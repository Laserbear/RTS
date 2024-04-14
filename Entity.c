#include "Entity.h"

void move_entity(Entity *entity) {
    if (!entity->isActive) {
        return;
    }
    float dx = entity->target.x - entity->x;
    float dy = entity->target.y - entity->y;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance > 0) {
        float move_x = (dx / distance) * entity->speed;
        float move_y = (dy / distance) * entity->speed;

        // Apply movement
        entity->x += (int)round(move_x);
        entity->y += (int)round(move_y);

        // Check if overshooting the target, then adjust directly
        if ((fabs((float)entity->x - entity->target.x) < fabs(move_x)) ||
            (fabs((float)entity->y - entity->target.y) < fabs(move_y))) {
            entity->x = entity->target.x;
            entity->y = entity->target.y;
        }
    }

}

void render_entity(SDL_Renderer *renderer, Entity *entity) {
    //Draw our boy
    //Figure out how to put sprites in here
    if (!entity->isActive) {
	return;
    }
    if (entity->isSelected) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for selected
    } else {
        SDL_SetRenderDrawColor(renderer, entity->color.r, entity->color.g, entity->color.b, entity->color.a);
    }
    SDL_Surface* loadedSurface = IMG_Load("sprites/pug.jpg");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    SDL_Rect rect = {(int)round(entity->x), (int)round(entity->y), entity->w, entity->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    //SDL_RenderFillRect(renderer, &rect);


}

void generate_random_entity(Entity *entity) {
        entity->x = rand()%500;
	entity->y = rand()%500; 
	entity->w = 50;
	entity->h = 50;
        entity->target.x = entity->x;
        entity->target.y = entity->y;
        entity->color = (SDL_Color){rand()%256 + 1, rand()%256 + 1, rand()%256 + 1, 255};
        entity->speed = 1;
	entity->isActive = false;
	entity->isSelected = false;
}

