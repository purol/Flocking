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

# define fpss 60

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

void stretchTextureEx(SDL_Renderer* renderer, float x, float y, float w, float h, SDL_Texture* texture, float angle, SDL_RendererFlip flip = SDL_FLIP_NONE) {
    SDL_Rect src, dst;
    SDL_Point center;

    src.x = src.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

    dst.x = (int)x;
    dst.y = (int)y;
    dst.w = (int)w;
    dst.h = (int)h;

    center.x = (int)(w / 2.0);
    center.y = (int)(h / 2.0);

    SDL_RenderCopyEx(renderer, texture, &src, &dst, angle, &center, flip);
}

void stretchTextureEx_revise(SDL_Renderer* renderer, float x, float y, float w, float h, float CX, float CY, SDL_Texture* texture, float angle, SDL_RendererFlip flip = SDL_FLIP_NONE) {
    SDL_Rect src, dst;
    SDL_Point center;

    src.x = src.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

    dst.x = (int)x;
    dst.y = (int)y;
    dst.w = (int)w;
    dst.h = (int)h;

    center.x = (int)CX;
    center.y = (int)CY;

    SDL_RenderCopyEx(renderer, texture, &src, &dst, angle, &center, flip);
} // angle : degree

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

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    bool quit = false;
    SDL_Event event;

    SDL_Texture* back_ground;
    SDL_Texture* creature;

    back_ground = loadTexture("resource\\ground.png");
    creature = loadTexture("resource\\creature.png");

    int total_frame_start = 0;
    int total_frame_end = 0;
    float total_time = 0;
    int total_delay_time = 0;

    int num = 0;
    int temp_num = 0;
    Creature * CRT = NULL;
    Creature* temp_CRT=NULL;

    int i = 0;

    bool Q_key = false;
    bool W_key = false;

    while (!quit) {

        total_frame_start = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN: // q means add a creature, w means delete a creature
                if (event.key.keysym.sym == SDLK_q && Q_key == false) {
                    temp_num = num + 1;
                    temp_CRT = (Creature*)malloc(sizeof(Creature) * temp_num);
                    for (i = 0; i < num; i++) { // copy the information of creature to temp_creature
                        temp_CRT[i].x = CRT[i].x;
                        temp_CRT[i].y = CRT[i].y;
                        temp_CRT[i].angle = CRT[i].angle;
                        temp_CRT[i].velocity = CRT[i].velocity;
                        temp_CRT[i].acceleration = CRT[i].acceleration;
                    }
                    free(CRT); CRT = NULL; // return CRT pointer to null

                    temp_CRT[temp_num - 1].x = 0;
                    temp_CRT[temp_num - 1].y = 0;
                    temp_CRT[temp_num - 1].angle = 0;
                    temp_CRT[temp_num - 1].velocity = 0;
                    temp_CRT[temp_num - 1].acceleration = 0;

                    CRT = temp_CRT; num = temp_num;
                    temp_num = 0; temp_CRT = NULL;

                    Q_key = true;

                }
                else if (event.key.keysym.sym == SDLK_w && W_key == false) {
                    if (num == 0) break;
                    else if (num > 0) {
                        temp_num = num - 1;
                        temp_CRT = (Creature*)malloc(sizeof(Creature) * temp_num);
                        for (i = 0; i < temp_num; i++) { // copy the information of creature to temp_creature
                            temp_CRT[i].x = CRT[i].x;
                            temp_CRT[i].y = CRT[i].y;
                            temp_CRT[i].angle = CRT[i].angle;
                            temp_CRT[i].velocity = CRT[i].velocity;
                            temp_CRT[i].acceleration = CRT[i].acceleration;
                        }
                        free(CRT); CRT = NULL; // return CRT pointer to null

                        CRT = temp_CRT; num = temp_num;
                        temp_num = 0; temp_CRT = NULL;
                    }

                    W_key = true;
                }
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_q && Q_key == true) {
                    Q_key = false;
                }
                if (event.key.keysym.sym == SDLK_w && W_key == true) {
                    W_key = false;
                }

            }
 //           printf("cccc ");
        }

            drawTexture(renderer, 0, 0, back_ground);

      //      drawTexture(renderer, 0, 0, creature);
      //     stretchTextureEx_revise(renderer, 0, 0, 17, 25, 8.5, 16.669, creature, 180);

            for (i = 0; i < num; i ++) {
                stretchTextureEx_revise(renderer, CRT[i].x-8.5, CRT[i].y-16.669, 17, 25, 8.5, 16.669, creature, CRT[i].angle);
 //               printf("asdf ");
            }
 //           printf("%d ",num);
            SDL_RenderPresent(renderer);

            total_frame_end = SDL_GetPerformanceCounter();
            total_time = (float)(total_frame_end - total_frame_start) / ((float)SDL_GetPerformanceFrequency());
            total_delay_time = (int)(((1.0 / fpss) - total_time) * 1000);

            if (total_delay_time > 0) SDL_Delay(total_delay_time);
            else SDL_Delay(1);
            SDL_RenderClear(renderer);

    }


    SDL_Quit();

    return 0;

}