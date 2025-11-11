#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define WIDTH 400
#define HEIGHT 400
#define TITLE "Balls and their admirers"
#define BALL_COUNT 100
#define FPS 60
#define VEL_MAX 5
#define RADIUS_MAX 20
#define RADIUS_MIN 5

Color COLORS[] = {
    LIGHTGRAY, GRAY,   DARKGRAY, YELLOW,     GOLD,      ORANGE,  PINK,
    RED,       MAROON, GREEN,    LIME,       DARKGREEN, SKYBLUE, BLUE,
    DARKBLUE,  PURPLE, VIOLET,   DARKPURPLE, BEIGE,     BROWN,   DARKBROWN,
};

// Definition of Ball
// Ball stores state and other properties
// YOUR CODE START

typedef struct Ball {
    int posx, posy;       // Position coordinates
    int velx, vely;       // Velocity components
    int radius;           // Radius of the ball
    Color color;            // Color (from raylib)
    struct Ball *follows;   // Pointer to the ball it follows
} Ball;

//YOUR CODE END

Ball balls[BALL_COUNT];

// Initializes a ball with random values
Ball *init_ball_random(Ball *p) {
  // Randomly initialize state and properties
  // YOUR CODE HERE
  
  Ball *init_ball_random(Ball *p) {
    // Assign random position
    p->posx = (int)(rand() % WIDTH);
    p->posy = (int)(rand() % HEIGHT);

    // Assign random velocity
    p->velx = ((int)(rand() % (2 * VEL_MAX + 1)) - VEL_MAX);
    p->vely = ((int)(rand() % (2 * VEL_MAX + 1)) - VEL_MAX);

    // Assign random radius
    p->radius = RADIUS_MIN + (rand() % (RADIUS_MAX - RADIUS_MIN + 1));
    
    // Assign random color
    p->color = COLORS[rand() % (sizeof(COLORS)/sizeof(COLORS[0]))];

    // Assign a random leader (follows), different from itself
  //YOUR CODE END
    return p;
}
  // Find a leading ball other than the initialized ball itself.
  Ball *leader; // Represents the leading ball that this ball will follow
  // YOUR CODE HERE
  int leader_idx;
    do {
        leader_idx = rand() % BALL_COUNT;
    } while (&balls[leader_idx] == p);
    p->follows = &balls[leader_idx];
//YOUR CODE END
  return p;
}

// Initialize all `balls`
void init_balls_random() {
  // YOUR CODE HERE
  void init_balls_random() {
    for (size_t i = 0; i < BALL_COUNT; ++i) {
        init_balls[i] = (Ball*)malloc(sizeof(Ball));
        init_ball_random(&balls[i]);
    }
}
//YOUR CODE END
}

Ball *draw_ball(Ball *p) {
  DrawCircle(p->posx, p->posy, p->radius, p->color);
  return p;
}

// Updates the positions of balls according to their velocities
Ball *update_pos(Ball *p) {
  p->posx = (WIDTH + p->posx + p->velx) %
            WIDTH; // `WIDTH +` because C uses truncated division
  p->posy = (HEIGHT + p->posy + p->vely) % HEIGHT;
  return p;
}

// Updates the velocity of a ball so that it follows the leading ball
Ball *update_vel_for_following(Ball *p) {
  int errx = p->follows->posx - p->posx;
  int erry = p->follows->posy - p->posy;
  p->velx = errx > 0 ? 1 : -1;
  p->vely = erry > 0 ? 1 : -1;
  return p;
}

// Update all elements
void update_elements() {
  for (size_t i = 0; i < _Countof balls; ++i) {
    draw_ball(update_pos(update_vel_for_following(&balls[i])));
  }
}

int main() {
  InitWindow(WIDTH, HEIGHT, TITLE);
  SetTargetFPS(FPS);

  init_balls_random();

  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();
    update_elements();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }
}