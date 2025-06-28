#ifndef SINGLETON_H
#define SINGLETON_H
#include <string>
#include<map>
#include<iostream>
#include<fstream>
using namespace std;

class Singleton
{
    public:
        static void update();
        static void On_load();
        static void Init(float sfx,float bgm);
        static string get_key(string accion);
        static void modific_key(string key,string val);
        static void modific_config();
        static void get_config_inf(map<string,string>val);
        static bool key_asignada(string val);
        static void set_dificultad(int val);
        static int get_victory();
    protected:

    private:

        static bool in_menuP;
        static bool use_GamePad;
        static map<string,string> controles_keys;
        static map<int,int> opciones_selected;
        static string pantalla_actual;
        static map<string,string> data_config;
        static int dificultad;
        static int ganador;


};

#endif // SINGLETON_H
