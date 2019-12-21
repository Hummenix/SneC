#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "snake.h"


int apple_random(int max_number,
                 int grid_size) { // will create a random number smaller than max_number and on the same grid as snec
    int return_value = rand() % max_number;
    return_value -= (return_value % grid_size);
    return return_value;
}

int check_collision_with_snake(Snake_list *snake_head, Snake_list *part) {
    int valid = 0;
    Snake_list *current = snake_head;

    if (snake_head->x_coordinate == part->x_coordinate && snake_head->y_coordinate == part->y_coordinate) {
        valid = 0;
    } else {
        valid = 1;
    }

    while (current->next_body_part != NULL) {
        if (current->x_coordinate == part->x_coordinate && current->y_coordinate == part->y_coordinate) {
            valid = 0;
            break;
        } else {
            valid = 1;
        }
        current = current->next_body_part;
    }
    return valid;
}

void move_head(Snake_list *snake_head, int delta_x, int delta_y) {
    Snake_list *new = NULL;
    new = malloc(sizeof(new));
    new->x_coordinate = snake_head->x_coordinate;
    new->y_coordinate = snake_head->y_coordinate;
    new->next_body_part = snake_head->next_body_part;
    snake_head->next_body_part = new;


    snake_head->x_coordinate = snake_head->x_coordinate + delta_x;
    snake_head->y_coordinate = snake_head->y_coordinate + delta_y;
}

int main(int argc, char *args[]) {
    //Initialize all the systems of SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        printf("TTF_Init: %s\n", SDL_GetError());
        exit(10);
    }
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(20);
    }

    //Create a window with a title in the centre
    //with dimensions of 800 px width
    //and 600 px height and force it to be shown on screen
    static int width = 800, height = 600;
    static int field_width = 800, field_height = 540;
    SDL_Window *window = SDL_CreateWindow("No step on snec", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    //Create a renderer for the window created above, with the first display driver present
    //and with no additional settings
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Event event;

    static int jump_length = 25;
    static int body_part_size = 20;
    int delta_x = jump_length;
    int delta_y = 0;
    int score = 0;
    //create the snake (its head)
    Snake_list *snake_head = NULL;
    snake_head = malloc(sizeof(Snake_list));
    if (snake_head == NULL) {
        return 1;
    }
    snake_head->x_coordinate = 100;
    snake_head->y_coordinate = 100;
    snake_head->next_body_part = NULL;
    //now the apple
    Apple *apple = NULL;
    apple = malloc(sizeof(Apple));
    if (apple == NULL) {
        return 1;
    }
    apple->next_body_part = NULL;
    do {
        apple->x_coordinate = apple_random(field_width - body_part_size, jump_length);
        apple->y_coordinate = apple_random(field_height - body_part_size, jump_length);
    } while (apple->x_coordinate == snake_head->x_coordinate && apple->y_coordinate == snake_head->y_coordinate);

    //Code that doesn't work the way I want it to. AKA Text stuff
    TTF_Font *Sans = TTF_OpenFont("./Sans.ttf", 24); //this opens a font style and sets a size
    printf("TTF: %i\n", Sans);
    if (Sans == NULL) {
        printf("Oh My Goodness, an error : %s", TTF_GetError());
    }

    SDL_Color White = {255, 255, 255};
    char *text = "Score: ";
    //Note: find out how to append an integer onto a string
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, text,
                                                       White); // TTF_RenderText_Solid can only be used on SDL_Surface
    SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now convert it into a texture

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0;  //controls the rect's x coordinate
    Message_rect.y = field_height; // controls the rect's y coordinte
    Message_rect.w = 400; // controls the width of the rect
    Message_rect.h = 50; // controls the height of the rect



    int gameRunning = 1;
    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameRunning = 0;
                    break;
                case SDL_KEYDOWN:
                    //printf("We got a keyboard event.\n");
                    // printf("Pressed key is: (%d)\n", event.key.keysym);
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            delta_x = 0 - jump_length;
                            delta_y = 0;
                            break;
                        case SDLK_RIGHT:
                            delta_x = jump_length;
                            delta_y = 0;
                            break;
                        case SDLK_UP:
                            delta_x = 0;
                            delta_y = 0 - jump_length;
                            break;
                        case SDLK_DOWN:
                            delta_x = 0;
                            delta_y = jump_length;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    break;
                default:
                    //printf("Unhandled Event!\n");
                    break;

            }
        }
        //Set the draw color of renderer to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //Clear the renderer with the draw color
        SDL_RenderClear(renderer);
        //Update the renderer which will show the renderer cleared by the draw color which is black
        //SDL_RenderPresent(renderer);


        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); //green
        if (snake_head->x_coordinate + delta_x == apple->x_coordinate &&
            snake_head->y_coordinate + delta_y == apple->y_coordinate) {
            //draw the entire snec with rear and make it longer
            score += 1;
            printf("Yummy! Current Score: %i\n", score);


            move_head(snake_head, delta_x, delta_y);

            while (!check_collision_with_snake(snake_head, apple)) {
                apple->x_coordinate = apple_random(field_width - body_part_size, jump_length);
                apple->y_coordinate = apple_random(field_height - body_part_size, jump_length);
            }

            Snake_list *current = snake_head;
            SDL_Rect head;
            head.x = current->x_coordinate, head.y = current->y_coordinate, head.w = body_part_size, head.h = body_part_size;
            SDL_RenderFillRect(renderer, &head);

            while (1) {
                if (current->next_body_part != NULL) {
                    current = current->next_body_part;
                    // define rectangle (body part) to be drawn and draw it
                    SDL_Rect body_part;
                    body_part.x = current->x_coordinate, body_part.y = current->y_coordinate, body_part.w = body_part_size, body_part.h = body_part_size;
                    SDL_RenderDrawRect(renderer, &body_part);
                    //SDL_RenderPresent(renderer);

                } else {
                    break;
                }
            }

        } else {
            // draw entire snek without rear

            move_head(snake_head, delta_x, delta_y);

            if (!check_collision_with_snake(snake_head->next_body_part, snake_head)) {
                printf("The head collided with the body, n00b!\nYour last score was %i.", score);
                gameRunning = 0;
            }

            Snake_list *current = snake_head;
            SDL_Rect body_part;
            body_part.x = current->x_coordinate, body_part.y = current->y_coordinate, body_part.w = body_part_size, body_part.h = body_part_size;
            SDL_RenderFillRect(renderer, &body_part);


            while (1) {
                if (current->next_body_part->next_body_part != NULL) {
                    current = current->next_body_part;
                    // define rectangle (body part) to be drawn and draw it
                    SDL_Rect body_part;
                    body_part.x = current->x_coordinate, body_part.y = current->y_coordinate, body_part.w = body_part_size, body_part.h = body_part_size;
                    SDL_RenderDrawRect(renderer, &body_part);
                    //SDL_RenderPresent(renderer);

                } else {
                    free(current->next_body_part);
                    current->next_body_part = NULL;
                    break;
                }
            }
        }

        //Update the texty stuff

        //free(surfaceMessage);
        //free(Message);
        //char text = "test1337"
        //surfaceMessage = TTF_RenderText_Solid(Sans, text, White); // TTF_RenderText_Solid can only be used on SDL_Surface
        //Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now convert it into a texture

        //Add text to the renderer
        //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

        //DRAW THE APPLE//
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //red
        SDL_Rect draw_apple;
        draw_apple.x = apple->x_coordinate, draw_apple.y = apple->y_coordinate, draw_apple.w = body_part_size, draw_apple.h = body_part_size;
        SDL_RenderFillRect(renderer, &draw_apple);

        SDL_RenderPresent(renderer);


        if (snake_head->x_coordinate > field_width - body_part_size || snake_head->x_coordinate < 0 ||
            snake_head->y_coordinate > field_height - body_part_size || snake_head->y_coordinate < 0) {
            printf("You ran into a wall genius! \nYour latest score was: %i", score);
            gameRunning = 0;
        }
        //Pause for .2 sec (5fps)
        SDL_Delay(200);

    }

    //Destroy the renderer created above
    SDL_DestroyRenderer(renderer);

    //Destroy the window created above
    SDL_DestroyWindow(window);

    //Close all the systems of SDL initialized at the top
    SDL_Quit();

    return 0;
}
