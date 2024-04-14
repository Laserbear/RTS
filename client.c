#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <SDL2/SDL.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Entity.h"

static void client(int port) {
    //create socket
    const int fd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET; 
    addr.sin_port = htons((short) port);
    char addrstr[NI_MAXHOST + NI_MAXSERV + 1];
    snprintf(addrstr, sizeof(addrstr), "127.0.0.1:%d", port);

    if (connect(fd, (struct sockaddr*) &addr, sizeof(addr))) {
	    perror("connection error:");
	    return;
    }

    const char *msg = "greetings from the client";
    

    close(fd);
}

int main(int argc, char *argv[]) {
    printf("hello, gamers!\n");
    if(argc < 2) {
	fprintf(stderr, "Please specify port\n");
	return -1;
    }
    int port;
    sscanf(argv[1], "%d", &port);
    printf("sending message to port %d\n", port);
    client(port);
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
	fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
	return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Age of Pugs", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1440, 1080, 0);

    if (!window) {
	fprintf(stderr, "Failed to create window %s\n", SDL_GetError());
	SDL_Quit();
	return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
	SDL_DestroyWindow(window);
	SDL_Quit();
	fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
	return 1;
    }

    //Set draw color 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderClear(renderer);
    #define NUM_ENTITIES 5000 
    static Entity entities[NUM_ENTITIES];
    static Entity selectedEntities[NUM_ENTITIES];


    for(int i = 0; i < 5; i++) {
	generate_random_entity(&entities[i]);
	entities[i].isActive = true;
    }

    SDL_bool running = SDL_TRUE;

    typedef struct {
       bool selecting;
       SDL_Rect box;  // x, y, w, h
    } SelectionBox;
     
    Entity* initializeEntityArray() {
	Entity* generated_entities = malloc(NUM_ENTITIES * sizeof(Entity));
        if (!generated_entities) {
            return NULL;  // Always check if malloc succeeded
        }
	for(int i = 0; i < NUM_ENTITIES; i++) {
	   generate_random_entity(&generated_entities[i]);
	}
	return generated_entities;
    }

    Entity* getSelected(SelectionBox* selection) {
	//TODO: find intersections and select pugs
        for (int i = 0; i < NUM_ENTITIES; i++) {
	   if (!entities[i].isActive) {
	      continue;
	   }
           SDL_Rect entityRect = {entities[i].x, entities[i].y, entities[i].w, entities[i].h};
           SDL_Rect result; // To store the result of intersection, if needed
           // Directly using SDL_IntersectRect to check for intersection
           if (SDL_IntersectRect(&entityRect, &(selection->box), &result)) {
                // This entity is within the selection box
                // Mark as selected, handle as needed
               selectedEntities[i] = entities[i];
	       entities[i].isSelected = true;
           } else {
	       entities[i].isSelected = false;
	       selectedEntities[i].isActive = false;
	   }
        }
	return selectedEntities;
    }

    SelectionBox selection;
    void process_mouse_input(SDL_Event event, SelectionBox* selection) {
          switch (event.type) {
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        //queue target for curently selected units
			for(int i = 0; i < NUM_ENTITIES; i++) {
			    if (!selectedEntities[i].isActive) {
			        continue;
			    }
			    entities[i].target.x = event.button.x;
			    entities[i].target.y = event.button.y;
			    selectedEntities[i].isActive = false;
			}
                        selection->selecting = true;
                        selection->box.x = event.button.x;
                        selection->box.y = event.button.y;
                        selection->box.w = 0;
                        selection->box.h = 0;
                     }
                    break;
                case SDL_MOUSEBUTTONUP:
                   if (event.button.button == SDL_BUTTON_LEFT) {
                        selection->selecting = false;
                        // Normalize the box dimensions
                        if (selection->box.w < 0) {
                            selection->box.x += selection->box.w;
                            selection->box.w = -selection->box.w;
                        }
                        if (selection->box.h < 0) {
                            selection->box.y += selection->box.h;
                            selection->box.h = -selection->box.h;
                        }
                        // Find collisions to select units
			getSelected(selection); 
                   }
                   break;
                case SDL_MOUSEMOTION:
                   if (selection->selecting) {
                        selection->box.w = event.motion.x - selection->box.x;
                        selection->box.h = event.motion.y - selection->box.y;
                   }
                   break;
            } 
    }
    

    int counter = 0;
    int last_t = SDL_GetTicks()-100;
    int msec = 1;
    while (running) {
	if (counter % 100 == 0 && msec > 0) {
	  //printf("FPS: %d\n", ((int) (100000.0/msec)));
	  msec = SDL_GetTicks() - last_t;
	  last_t = SDL_GetTicks();
	}
	counter = counter % 100;
	counter += 1;
        SDL_Event event;
        while (SDL_PollEvent(&event)) { //process input into commands
            process_mouse_input(event, &selection); 
        }
	if (counter % 100 == 0) {
	     //process commands in queue
	     for(int i =0; i< NUM_ENTITIES; i++) {
	         if (!entities[i].isActive) {
		    continue;
		 }
	          move_entity(&entities[i]);
	     }
	}
	// Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white background
        SDL_RenderClear(renderer);


	
        for (int i = 0; i < NUM_ENTITIES; i++) {
	    if (!entities[i].isActive) {
	        continue;
	    }
            render_entity(renderer, &entities[i]);
        }
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
        SDL_RenderFillRect(renderer, &selection.box);

        // Update the screen
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
