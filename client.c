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
    
    Entity pugs[5];

    for(int i = 0; i < 5; i++) {
	generate_random_entity(&pugs[i]);
    }

    SDL_bool running = SDL_TRUE;
    int counter = 0;
    while (running) {
	SDL_Delay(10);
	counter = counter % 100;
	counter += 1;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = SDL_FALSE;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Send pugs to the mouse position
		for (int i =0; i < 5; i++) {
                    pugs[i].target.x = event.button.x - pugs[i].w / 2;
                    pugs[i].target.y = event.button.y - pugs[i].h / 2;
		}
            }
        }
	
	for(int i =0; i<5; i++) {
		move_entity(&pugs[i]);
	}
	// Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white background
        SDL_RenderClear(renderer);


	
        for (int i = 0; i < 5; i++) {
            render_entity(renderer, &pugs[i]);
        }

        // Update the screen
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
