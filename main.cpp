#include <cstdlib>
#include <optional>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#define GREY CLITERAL(Color) {51, 51, 51, 51}
#define BACKGROUND CLITERAL(Color) {20, 19, 33}

#define MAX_DISTANCE 100
#define MAX_BALLS 50

#define WIDTH 1280
#define HEIGHT 720 

float getRandom(float a, float b)
{
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

//Forward decleration so the following vector can be used inside the class itself
class Ball;

std::vector<Ball*> balls;

class Ball
{
public:
    Vector2 pos, vel;
    float size;
    Ball()
    {

    }
    Ball(float x, float y)
    {
        pos.x = x;
        pos.y = y;
        vel.x = getRandom(-1, 1);
        vel.y = getRandom(-1, 1);
        size = getRandom(1.5, 5);
    }
    void Render()
    {
        DrawCircle(pos.x, pos.y, size, WHITE);
    }
    void Update()
    {
        //Update position and do collision detection
        pos.x += vel.x;
        pos.y += vel.y;
        if(pos.x - (size / 2) <= 0 || pos.x + (size / 2) >= WIDTH) vel.x = -vel.x;
        if(pos.y - (size / 2) <= 0 || pos.y + (size / 2) >= HEIGHT) vel.y = -vel.y;       

        //Figure out which two balls are the closest under MAX_DISTANCE. If a second ball can't be found, no triangle is drawn.
        float closestDist = 9999, secondClosestDist = 9999;
        std::optional<Ball> closestBall, secondClosestBall;
        for(Ball* b : balls)
        {
            if(b == this) continue;
            float distance = Vector2Distance(pos, b->pos);
            if(distance <= MAX_DISTANCE)
            {
                if(std::min(distance, closestDist) == distance)
                {
                    closestDist = distance;
                    closestBall = *b;
                }
                else if(std::min(distance, secondClosestDist) == distance)
                {
                    secondClosestDist = distance;
                    secondClosestBall = *b;
                }
            }
        } 
        if(secondClosestBall)
        {
            if(secondClosestDist < MAX_DISTANCE)        
            {                        
                DrawTriangle(pos, closestBall->pos, secondClosestBall->pos, GREY);
            }
        }
    }
};

//Compare if two Ball classes are the same. Crude, but works.
inline bool operator==(const Ball &b, const Ball &b2) 
{
    if(b.pos.x == b2.pos.x && b.pos.y == b2.pos.y && b.size == b2.size && b.vel.x == b2.vel.x && b.vel.y == b2.vel.y ) return true;
    return false;
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Balls Render Raylib");
    SetTargetFPS(60);

    //Fill up balls vector
    for(int i = 0; i < MAX_BALLS; ++i) 
    {
        Ball* b = new Ball(getRandom(0, WIDTH),getRandom(0, HEIGHT));
        balls.emplace_back(b);
    }
    while(!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BACKGROUND);
            for(Ball* b : balls)
            {
                b->Update();
                b->Render();
            }
        }
        EndDrawing();
        DrawFPS(0, 0);
    }
    return EXIT_SUCCESS;
}
