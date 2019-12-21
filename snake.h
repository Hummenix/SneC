//
// Created by hml on 20.12.19.
//

#ifndef CSNEK_SNAKE_H
#define CSNEK_SNAKE_H


typedef struct Snake_list {
    int x_coordinate;
    int y_coordinate;
    struct Snake_list *next_body_part;
} Snake_list;

typedef Snake_list Apple;




#endif //CSNEK_SNAKE_H
