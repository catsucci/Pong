#include <iostream>
#include <raylib.h>

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int artificialPlayer_score = 0;

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update()
    {
        x += speed_x;
        y += speed_y;

        if ((y + radius) >= GetScreenHeight() || (y - radius) <= 0)
        {
            speed_y *= -1;
        }
        if ((x + radius) >= GetScreenWidth())
        {
            artificialPlayer_score++;
            Reset();
        }
        if ((x - radius) <= 0)
        {
            player_score++;
            Reset();
        }
    }

    void Reset()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
protected:
    void NotJailBrake()
    {
        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }
        NotJailBrake();
    }
};

class ArtificialPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y -= speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y += speed;
        }
        NotJailBrake();
    }
};

Ball ball;
Paddle player;
ArtificialPaddle artificialPlayer;

int main()
{

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Pong");
    SetTargetFPS(60);

    // Initializing the ball object
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.radius = 20;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // Initializing the player paddle object
    player.width = 25;
    player.height = 120;
    player.x = screenWidth - player.width - 10;
    player.y = screenHeight / 2 - player.height / 2;
    player.speed = 10;

    // Initializing the artificial player paddle object
    artificialPlayer.width = 25;
    artificialPlayer.height = 120;
    artificialPlayer.x = 10;
    artificialPlayer.y = screenHeight / 2 - artificialPlayer.width / 2;
    artificialPlayer.speed = 10;

    while (!WindowShouldClose())
    {
        // Event handling

        // Update positions
        ball.Update();
        player.Update();
        artificialPlayer.Update(ball.y);

        // Checking for collisions
        if (CheckCollisionCircleRec(
                Vector2{ball.x, ball.y},
                ball.radius,
                Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(
                Vector2{ball.x, ball.y},
                ball.radius,
                Rectangle{artificialPlayer.x,
                          artificialPlayer.y,
                          artificialPlayer.width,
                          artificialPlayer.height}))
        {
            ball.speed_x *= -1;
        }

        // Drawing
        BeginDrawing();
        ClearBackground(Dark_Green);
        DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, Green);
        DrawCircle(screenWidth / 2, screenHeight / 2, 150, Light_Green);
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
        ball.Draw();
        artificialPlayer.Draw();
        player.Draw();
        DrawText(
            TextFormat("%i", artificialPlayer_score),
            screenWidth / 4 - 20,
            20,
            80,
            WHITE);
        DrawText(
            TextFormat("%i", player_score),
            3 * screenWidth / 4 - 20,
            20,
            80,
            WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}