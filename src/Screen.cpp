#include "Screen.h"
#include "iostream"
using namespace std;

bool Screen::modific_win=false;
bool Screen::iniciado=false;
vector<SDL_DisplayMode> Screen::modes;

Vector2 Screen::size_win=Vector2(0,0);
Vector2 Screen::get_size(){
   return size_win;
}
 void Screen::set_size(Vector2 dim){
     size_win=dim;
 }
void Screen::change_size(Vector2 sz){
   modific_win=true;
   size_win=sz;

}
void Screen::update(SDL_Window* win){
    if(iniciado==false){
        iniciado=true;
        int Count{SDL_GetNumDisplayModes(0)};
        SDL_DisplayMode Mode;
        for (int i{0}; i<Count; ++i) {
            SDL_GetDisplayMode(0, i, &Mode);
            modes.push_back(Mode);
        }


    }
    if(modific_win==true){
          modific_win=false;
          SDL_SetWindowSize(win,size_win.x,size_win.y);

    }
}
