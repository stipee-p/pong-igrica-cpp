#include <raylib.h>
#include <iostream>

using namespace std;

Color Zelena = {38,185,154,255};
Color Tamno_Zelena = {20,160,133,255};
Color Svijetlo_Zelena = {129,204,184,255};
Color Zuta = {243,213,91,255};

int igrac_rez = 0;
int ai_rez = 0;

class Lopta
{
    public:
        float x,y;
        int brzina_x,brzina_y;
        int radius;

    void Draw()
    {
     DrawCircle(x, y, radius, Zuta);
    }

    void Pomak()
    {
        x += brzina_x;
        y += brzina_y;

        if(y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            brzina_y *= -1;
        }

        if(x + radius >= GetScreenWidth()) 
        {
            ai_rez++;
            Gol();
        }
        
        if(x - radius <= 0)
        {
            igrac_rez++;
            Gol();
        }
    }

    void Gol()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int brzina_random[2] = {-1,1};
        brzina_x *= brzina_random[GetRandomValue(0,1)];
        brzina_y *= brzina_random[GetRandomValue(0,1)];
    }
};

class Reket
{
    protected: 

    void Granice()
    {
        if(y <= 0)
            y = 0;
        if(y + height >= GetScreenHeight())
            y = GetScreenHeight() - height;
    }

    public:
        float x,y;
        float width, height;
        int brzina;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height},0.8,0,WHITE);
    }

    void Pomak()
    {
        if(IsKeyDown(KEY_UP))
            y = y - brzina;
        
        if(IsKeyDown(KEY_DOWN))
            y = y + brzina;
        
        Granice();
    }
};

class AI_Reket: public Reket
{
    public:
        void Pomak(int lopta_y)
        {
            if(y + height/2 > lopta_y)
            {
                y = y - brzina;
            }
            if(y + height/2 < lopta_y)
            {
                y = y + brzina;
            }
            Granice();
        }
};

Lopta lopta;
Reket igrac;
AI_Reket ai_reket;

int main() 
{

    cout << "Strating the game" << endl;
    const int screen_w = 1200;
    const int screen_h = 800;
    InitWindow(screen_w, screen_h, "Pong Igrica");

    SetTargetFPS(60);

    lopta.radius = 20;
    lopta.x = screen_w / 2;
    lopta.y = screen_h / 2;
    lopta.brzina_x = 7;
    lopta.brzina_y = 7;

    igrac.width = 25;
    igrac.height = 120;
    igrac.x = screen_w - igrac.width - 10;
    igrac.y = screen_h/ 2 - igrac.height/2;
    igrac.brzina = 6;

    ai_reket.height = 120;
    ai_reket.width = 25;
    ai_reket.x = 10;
    ai_reket.y = screen_h/2 - ai_reket.height/2;
    ai_reket.brzina = 6;

    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        //Pomak lopte
        lopta.Pomak();
        igrac.Pomak();
        ai_reket.Pomak(lopta.y);

        //Provjera udarca lopte
        if(CheckCollisionCircleRec(Vector2{lopta.x,lopta.y}, lopta.radius, Rectangle{igrac.x , igrac.y, igrac.width, igrac.height}))
        {
            lopta.brzina_x *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{lopta.x,lopta.y}, lopta.radius, Rectangle{ai_reket.x , ai_reket.y, ai_reket.width, ai_reket.height}))
        {
            lopta.brzina_x *= -1;
        }

        //Crtanje objekata
        ClearBackground(Tamno_Zelena);
        DrawRectangle(screen_w/2,0,screen_w/2,screen_h,Zelena);
        DrawCircle(screen_w / 2, screen_h / 2, 150, Svijetlo_Zelena);
        DrawLine(screen_w/2.0, 0, screen_w / 2, screen_h, WHITE);
        lopta.Draw();
        ai_reket.Draw();
        igrac.Draw();
        DrawText(TextFormat("%i",ai_rez),screen_w/4-20,20,80,WHITE);
        DrawText(TextFormat("%i",igrac_rez),3 * screen_w/4-20,20,80,WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}