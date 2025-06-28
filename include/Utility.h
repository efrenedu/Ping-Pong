#ifndef UTILITY_H
#define UTILITY_H
#include<stdlib.h>
#include<string>
#include<vector>
#include<iostream>
#include <map>
#include <SDL.h>
#include<math structs.h>

class Timer
{
    public:

       static void update();
       static double get_delta_time();
       static double get_fixed_deltaTime();
       static double get_ticks_frame();
    protected:

    private:
        static int counted_frames;
        static int target_fps;
        static double tick_perFrame;
        static double delta;
        static double fixed_deltaTime;
        static double first;
        static unsigned int Now;
        static unsigned int last;
};

enum MOUSE_BUTTONS{
   LEFT=1,RIGTH=3
};
class Input{
public:
   static bool Mouse_Up(int boton);
   static bool Mouse_Down(int boton);
   static bool Mouse_Pressed(int boton);
   static void update();
   static void get_event(SDL_Event ev);
   static void init();
   static bool Key_Down(std::string key);
   static bool Key_up(std::string key);
   static bool Key_Pressed(std::string key);
   static Vector2 get_Mouse_pos();
  static void reset();
  static string get_lastKey();


private:
   static std::map<std::string,int> keyboard_states;
   static std::map<int,int>  Gamepads_states;
   static std::map<int,int>  mouse_states;

   static std::map<std::string,float> keyboard_timers;
   static std::map<int,float>  Gamepads_timers;
   static std::map<int,float>  mouse_timers;
   static std::map<std::string,float> keyboard_Maxtimers;
   static std::map<int,float>  Gamepads_Maxtimers;
   static std::map<int,float>  mouse_Maxtimers;

   static float max_espera;
   static int steps_espera;
   static int count_keys;
   static Vector2 last_mouse_pos;
   static std::vector<std::string>keys_list;
   static string last_keyInput;

};
#endif // UTILITY_H
