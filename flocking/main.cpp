#include <SDL.h>
# include <stdio.h>
# include <stdlib.h>
# include <SDL_image.h>
# include <SDL_opengl.h>
# include <math.h>

#pragma comment (lib,"SDL2")
#pragma comment (lib,"SDL2main")
#pragma comment (lib,"SDL2_image")
#pragma comment (lib, "opengl32.lib")

typedef struct creature {
    float x;
    float y;
    float angle;
    float velocity;
    float acceleration;
}Creature;

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Texture* loadTexture(const char* file) {
	SDL_Surface* surface;
	SDL_Texture* texture;

	surface = IMG_Load(file);
	if (surface == NULL) {
		printf("%s Can not be read.\n", file);
		return NULL;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) {
		printf("can not create texture.\n");
	}
	SDL_FreeSurface(surface);
	return texture;
}

void drawTexture(SDL_Renderer* renderer, float x, float y, SDL_Texture* texture) {
    SDL_Rect src, dst;

    src.x = src.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

    dst.x = (int)x;
    dst.y = (int)y;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, texture, &src, &dst);
}

int main(int argc, char* argv[])

{

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Flocking", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 700, SDL_RENDERER_ACCELERATED);
    renderer = SDL_CreateRenderer(window, SDL_VIDEO_RENDER_OGL, 0);

    bool quit = false;
    SDL_Event event;

    SDL_Texture* texture;

    texture = loadTexture("resource\\abc.png");


    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            }
        }
        {
            drawTexture(renderer, 0, 0, texture);
            SDL_RenderPresent(renderer);
        }
        SDL_Delay(1);
    }


    SDL_Quit();

    return 0;

}