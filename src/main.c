#include <raylib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#define PADDLESPEED 400.0
#define PADDLEMAXSPEED 500.0

// Paddle /
typedef struct {
	Rectangle rect;
} Paddle;

Paddle createPaddle(Vector2 pos) {
    return (Paddle) {
        (Rectangle) { pos.x, pos.y, 25, 200 },
    };
}

void paddle_update(Paddle* self, bool p1) {
    // Controls
    if (p1) {
        if (IsKeyDown(KEY_W)) {
            self->rect.y -= PADDLESPEED * GetFrameTime();
        } 
        if (IsKeyDown(KEY_S)) {
            self->rect.y += PADDLESPEED * GetFrameTime();
        }
    } else {
       
    }

    if (self->rect.y < 0) {
        self->rect.y = 0;
    } else if (self->rect.y > GetScreenHeight() - self->rect.height) {
        self->rect.y = GetScreenHeight() - self->rect.height;
    }
    
    DrawRectangleRec(self->rect, WHITE);
}
////////////////////////////////////////

// Ball
typedef struct {
    Rectangle rect;
    Vector2 vel;
} Ball;

Ball createBall(Vector2 pos) {
    return (Ball) {
        (Rectangle) { pos.x, pos.y, 50, 50 },
        (Vector2) { 0, 0 }
    };
}

void ball_update(Ball* self, Paddle p1, Paddle p2) {
    if (self->rect.y <= 0 || self->rect.y + self->rect.height >= GetScreenHeight()) {
        self->vel.y *= -1;
    }
    
    if (CheckCollisionRecs(self->rect, p1.rect) || CheckCollisionRecs(self->rect, p2.rect)) {
        self->vel.x *= -1;
        self->vel.x += 20;
    }

    self->rect.x += self->vel.x * GetFrameTime();
    self->rect.y += self->vel.y * GetFrameTime();

    DrawRectangleRec(self->rect, WHITE);
}

////////////////////////////////////////

// etc
void restart(Paddle* p1, Paddle* p2, Ball* ball, int p) {
    *p1 = createPaddle((Vector2) { 10, (GetScreenHeight() - 200) / 2. });
    *p2 = createPaddle((Vector2) { GetScreenWidth() - 35, (GetScreenHeight() - 200) / 2. });
    *ball = createBall((Vector2) { GetScreenWidth() / 2. - 50, GetScreenHeight() / 2. - 50 });
    
    if (p == 0) {
        p = -1;
    }

    ball->vel.x = 300 * p;
    ball->vel.y = 100;
}
////////////////////////////////////////

// "ai"
void ai(Paddle* p2, Ball ball) {
    if (p2->rect.y < ball.rect.y) {
        p2->rect.y += PADDLESPEED * GetFrameTime();
    } 
    
    if (p2->rect.y + p2->rect.height > ball.rect.y) {
        p2->rect.y -= PADDLESPEED * GetFrameTime();
    }
}
////////////////////////////

int main(void) {
    InitWindow(1280, 720, "Pong");
    
    Paddle p1;
    Paddle p2;
    Ball ball;
    Vector2 score = { 0, 0 };

    restart(&p1, &p2, &ball, 0);

    SetTargetFPS(0); 
    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(BLACK);
            
            // Score stuff
            DrawText(TextFormat("%i\t\t\t\t\t\t\t\t%i", (int) score.x, (int) score.y), (GetScreenWidth() / 2.) - 60, 30, 20, WHITE);
            if (ball.rect.x <= 0) {
                score.y += 1;
                restart(&p1, &p2, &ball, 1);
            } else if (ball.rect.x >= GetScreenWidth() - ball.rect.width) {
                score.x += 1;
                restart(&p1, &p2, &ball, 0);
            }

            // Board
            DrawLine(GetScreenWidth() / 2., 0, GetScreenWidth() / 2., GetScreenHeight(), WHITE);
            
            // Updates
            paddle_update(&p1, true);
            paddle_update(&p2, false);
            ai(&p2, ball);

            ball_update(&ball, p1, p2);

        EndDrawing();
    }
    CloseWindow();        

    return 0;
}
