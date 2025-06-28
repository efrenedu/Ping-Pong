#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H
#include "Scene_data.h"


enum EVENT_TYPES{
   EXIT_GAME=0,SHOW_CURSOR=1,HIDE_CURSOR=2,
   SHOW_OBJECT=3, HIDE_OBJECT=4,SHOW_UI=5,HIDE_UI=6,
   SET_CURSOR=7, LOAD_SCENE=8,CHANGE_PANTALLA_UI=9,
   GET_TEXT_INPUT=10,CHANGE_TEXT_UI=11,GET_SCROLL_INPUT=12,
   RETURN_MSGBOX=13,ADD_EVENT_COLA=14,PAUSE_GAME=15,UNPAUSE_GAME=16,
   SET_DIFICULTAD=17,SELECT_RADIO=18,GET_RADIO_INPUT=19,

};
enum CURSOR_INDEX{
    ARROW=0,HAND=1,IBEAM=2
};
class Event_Manager
{
    public:
       static void execute_event(int ev,string args);
        static bool play_game();
        static void liberar();
        static void set_cursores(vector<string> src);
        static int get_last_cursor();



    private:
        static void exit_game();
        static void show_cursor(bool val);
        static void set_cursor(int index);
        static void show_Object(bool val,string arg,bool is_ui);
        static bool playing_game;
        static void change_Pantalla_UI(string arg);
        static vector<SDL_Cursor *> cursores;
        static int last_cursor;
        static void input_textUI(string arg);
        static void modific_UI(string arg);
        static void Input_ScrollUI(string arg);
        static void get_resDialog(string args);
        static void add_EventCola(string args);
        static void select_radioGroup(string args);
        static void radio_input(string args);


};


#endif // EVENT_MANAGER_H
