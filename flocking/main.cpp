#include <SDL.h>
# include <stdio.h>
# include <stdlib.h>
# include <SDL_image.h>
# include <SDL_opengl.h>
# include <math.h>
# include <time.h>

#pragma comment (lib,"SDL2")
#pragma comment (lib,"SDL2main")
#pragma comment (lib,"SDL2_image")
#pragma comment (lib, "opengl32.lib")

# define fpss 60
# define screen_h 700
# define screen_w 1000
# define pi 3.141592
# define max_speed 200
# define max_ang_speed 300
# define effective_distance 100.0
# define ang_vel_study 20
# define vel_study 30.0
# define coh_ang_vel_study 3.0
# define seperation_dis_cons 40
# define sep_ang_vel_study 10

typedef struct creature {
    float x;
    float y;
    float angle; // degree
    float velocity;
    float ang_vel;
}Creature;

int ren_num(int min, int max);
void move(Creature* cr, int num);
void alignment(Creature* cr, int num);
void cohesion(Creature* cr, int num);
void clean_ang_vel(Creature* cr, int num);
void seperation(Creature* cr, int num);

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

    window = SDL_CreateWindow("Flocking", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_w, screen_h, SDL_RENDERER_ACCELERATED);
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

    srand((unsigned int)time(NULL));

    num = 2;
    CRT = (Creature*)malloc(sizeof(Creature) * 2);
    CRT[0].x = 350;
    CRT[0].y=150;
    CRT[0].angle=180;
    CRT[0].velocity=200;
    CRT[0].ang_vel=0;
    CRT[1].x = 700;
    CRT[1].y = 530;
    CRT[1].angle = 270;
    CRT[1].velocity = 200;
    CRT[1].ang_vel = 0;

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
                        temp_CRT[i].ang_vel = CRT[i].ang_vel;
                    }
                    free(CRT); CRT = NULL; // return CRT pointer to null

                    temp_CRT[temp_num - 1].x = ren_num(0,screen_w);
                    temp_CRT[temp_num - 1].y = ren_num(0,screen_h);
                    temp_CRT[temp_num - 1].angle = ren_num(0, 360);
                    temp_CRT[temp_num - 1].velocity = ren_num(max_speed, max_speed);
                    temp_CRT[temp_num - 1].ang_vel = ren_num(-max_ang_speed, max_ang_speed);

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
                            temp_CRT[i].ang_vel = CRT[i].ang_vel;
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
  //          printf("%d ",num);
            alignment(CRT, num);
            cohesion(CRT, num);
            seperation(CRT, num);
            move(CRT, num);
            clean_ang_vel(CRT, num);

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

int ren_num(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void move(Creature * cr, int num) {
    int i = 0;
    for (i = 0; i < num; i++) {
        cr[i].x = cr[i].x + cr[i].velocity * (1.0 / fpss) * sin(cr[i].angle * pi / 180);
        cr[i].y = cr[i].y - cr[i].velocity * (1.0 / fpss) * cos(cr[i].angle * pi / 180);
        cr[i].angle = cr[i].angle + cr[i].ang_vel * (1.0 / fpss);
        while (cr[i].angle > 360) cr[i].angle = cr[i].angle - 360;
        while (cr[i].angle < 0) cr[i].angle = cr[i].angle + 360;
    }

}

void alignment(Creature* cr, int num) {
    int i = 0;
    int j = 0;
    for (i = 0; i < num; i++) { // i'th creature try to align with theri neighborhood
        float sum_angle = 0;
        float sum_velocity = 0;
        int k = 0;
        for (j = 0; j < num; j++) {
            float distance = 0;
            if (j != i) {
                distance = sqrt((cr[i].x - cr[j].x) * (cr[i].x - cr[j].x) + (cr[i].y - cr[j].y) * (cr[i].y - cr[j].y));
                if (distance < effective_distance) {
                    sum_angle = sum_angle + cr[j].angle;
                    sum_velocity = sum_velocity + cr[j].velocity;
                    k = k + 1;
                }
            }
        }
        if (k > 0) {
            sum_angle = sum_angle / k;
            sum_velocity = sum_velocity / k;
            float ang_del = sum_angle - cr[i].angle;
            if(ang_del >= 0 && ang_del <= 180) cr[i].ang_vel = cr[i].ang_vel + (sum_angle - cr[i].angle) * ang_vel_study;
            else if(ang_del >= 180 && ang_del <= 360) cr[i].ang_vel = cr[i].ang_vel + ((sum_angle - cr[i].angle)-360) * ang_vel_study;
            else if(ang_del <= 0 && ang_del >= -180) cr[i].ang_vel = cr[i].ang_vel + (sum_angle - cr[i].angle) * ang_vel_study;
            else if(ang_del <= -180 && ang_del >= -360)cr[i].ang_vel = cr[i].ang_vel + ((sum_angle - cr[i].angle) + 360) * ang_vel_study;
            cr[i].velocity = cr[i].velocity + (sum_velocity - cr[i].velocity) * ang_vel_study;
        }
        if (cr[i].ang_vel > max_ang_speed) cr[i].ang_vel = max_ang_speed;
        else if (cr[i].ang_vel < -max_ang_speed) cr[i].ang_vel = -max_ang_speed;
        if (cr[i].velocity > max_speed) cr[i].velocity = max_speed;
        else if (cr[i].velocity < -max_speed) cr[i].velocity = -max_speed;
    }

}

void cohesion(Creature* cr, int num) {
    int i = 0;
    int j = 0;
    for (i = 0; i < num; i++) { // i'th creature try to align with theri neighborhood
        float sum_x = 0;
        float sum_y = 0;
        int k = 0;
        for (j = 0; j < num; j++) {
            float distance = 0;
            if (j != i) {
                distance = sqrt((cr[i].x - cr[j].x) * (cr[i].x - cr[j].x) + (cr[i].y - cr[j].y) * (cr[i].y - cr[j].y));
                if (distance < effective_distance) {
                    sum_x = sum_x + cr[j].x;
                    sum_y = sum_y + cr[j].y;
                    k = k + 1;
                }
            }
        }
        if (k > 0) {
            sum_x = sum_x / k;
            sum_y = sum_y / k;
            float del_x = sum_x - cr[i].x;
            float del_y = sum_y - cr[i].y;
            float del_angle = 0;
            float deldel_angle = 0;
            if (del_x > 0) {
                if (del_y < 0) del_angle = 180 * (1 / pi) * atan(-del_x / del_y);
                else if (del_y > 0) del_angle = 90 + 180 * (1 / pi) * atan(del_y / del_x);
            }
            else if (del_x < 0) {
                if (del_y < 0) del_angle = 270 + 180 * (1 / pi) * atan(del_y / del_x);
                else if (del_y > 0) del_angle = 180 + 180 * (1 / pi) * atan(-del_x / del_y);
            }
            if (del_x == 0 && del_y < 0)del_angle = 0;
            else if (del_x == 0 && del_y > 0)del_angle = 180;
            else if (del_x > 0 && del_y == 0)del_angle = 90;
            else if (del_x < 0 && del_y == 0)del_angle = 270;
            else if (del_x == 0 && del_y == 0) del_angle = 0;
            
            deldel_angle = del_angle - cr[i].angle;
            if (deldel_angle >= 0 && deldel_angle <= 180) cr[i].ang_vel = cr[i].ang_vel + deldel_angle * coh_ang_vel_study;
            else if (deldel_angle >= 180 && deldel_angle <= 360) cr[i].ang_vel = cr[i].ang_vel + (deldel_angle - 360) * coh_ang_vel_study;
            else if (deldel_angle <= 0 && deldel_angle >= -180) cr[i].ang_vel = cr[i].ang_vel + deldel_angle * coh_ang_vel_study;
            else if (deldel_angle <= -180 && deldel_angle >= -360)cr[i].ang_vel = cr[i].ang_vel + (deldel_angle + 360) * coh_ang_vel_study;
 
        }
        if (cr[i].ang_vel > max_ang_speed) cr[i].ang_vel = max_ang_speed;
        else if (cr[i].ang_vel < -max_ang_speed) cr[i].ang_vel = -max_ang_speed;
        if (cr[i].velocity > max_speed) cr[i].velocity = max_speed;
        else if (cr[i].velocity < -max_speed) cr[i].velocity = -max_speed;
    }

}

void seperation(Creature* cr, int num) {
    int i = 0;
    int j = 0;
    for (i = 0; i < num; i++) { // i'th creature try to align with theri neighborhood

        for (j = 0; j < num; j++) {
            float distance = 0;
            if (j != i) {
                distance = sqrt((cr[i].x - cr[j].x) * (cr[i].x - cr[j].x) + (cr[i].y - cr[j].y) * (cr[i].y - cr[j].y));
  //              printf("%f ", distance);
                if (distance < effective_distance) {
                    if (i == 1) {
  //                      printf("ddd");
                    }
                    float del_x = (cr[i].x - cr[j].x);
                    float del_y = (cr[i].y - cr[j].y);

                    float del_angle = 0;
                    float deldel_angle = 0;

                    if (del_x > 0) {
                        if (del_y < 0) del_angle = 180 * (1 / pi) * atan(-del_x / del_y);
                        else if (del_y > 0) del_angle = 90 + 180 * (1 / pi) * atan(del_y / del_x);
                    }
                    else if (del_x < 0) {
                        if (del_y < 0) del_angle = 270 + 180 * (1 / pi) * atan(del_y / del_x);
                        else if (del_y > 0) del_angle = 180 + 180 * (1 / pi) * atan(-del_x / del_y);
                    }
                    if (del_x == 0 && del_y < 0)del_angle = 0;
                    else if (del_x == 0 && del_y > 0)del_angle = 180;
                    else if (del_x > 0 && del_y == 0)del_angle = 90;
                    else if (del_x < 0 && del_y == 0)del_angle = 270;
                    else if (del_x == 0 && del_y == 0) del_angle = 0;

                    deldel_angle = del_angle - cr[i].angle;
                    if (deldel_angle >= 0 && deldel_angle <= 180) cr[i].ang_vel = cr[i].ang_vel + deldel_angle * sep_ang_vel_study*(1.0/(1+exp(distance- seperation_dis_cons)));
                    else if (deldel_angle >= 180 && deldel_angle <= 360) cr[i].ang_vel = cr[i].ang_vel + (deldel_angle - 360) * sep_ang_vel_study * (1.0 / (1 + exp(distance - seperation_dis_cons)));
                    else if (deldel_angle <= 0 && deldel_angle >= -180) cr[i].ang_vel = cr[i].ang_vel + deldel_angle * sep_ang_vel_study * (1.0 / (1 + exp(distance - seperation_dis_cons)));
                    else if (deldel_angle <= -180 && deldel_angle >= -360)cr[i].ang_vel = cr[i].ang_vel + (deldel_angle + 360) * sep_ang_vel_study * (1.0 / (1 + exp(distance - seperation_dis_cons)));
                }
            }
        } // put ang vel out of here

        if (cr[i].ang_vel > max_ang_speed) cr[i].ang_vel = max_ang_speed;
        else if (cr[i].ang_vel < -max_ang_speed) cr[i].ang_vel = -max_ang_speed;
        if (cr[i].velocity > max_speed) cr[i].velocity = max_speed;
        else if (cr[i].velocity < -max_speed) cr[i].velocity = -max_speed;
    }

}

void clean_ang_vel(Creature* cr, int num) {
    int i = 0;
    for (i = 0; i < num; i++) { // i'th creature try to align with theri neighborhood
        cr[i].ang_vel = 0;
    }

}