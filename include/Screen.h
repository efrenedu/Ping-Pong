#ifndef SCREEN_H
#define SCREEN_H
#include<math structs.h>
#include<SDL.h>
#include<vector>



class Screen
{
    public:
        static Vector2 get_size();
        static void set_size(Vector2 dim);
        static void update(SDL_Window* win);


    protected:

    private:
       static void change_size(Vector2 sz);
       static bool modific_win;
        static Vector2 size_win;
        static bool iniciado;
        static vector<SDL_DisplayMode> modes;

};

#endif // SCREEN_H
