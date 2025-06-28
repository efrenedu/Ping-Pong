#ifndef PLAYER_H
#define PLAYER_H
#include <Game_Object.h>

class Player
{
    public:
        Player();
        Player(std::string name,Vector2 p,std::string script_data);
        virtual ~Player();
        void update(Game_Object * g);
        void OnTrigger(Game_Object  other,Game_Object* g,ManiFold* contact_info);
        void  OnCollision(Game_Object other,Game_Object* g, ManiFold* contact_info);
        void reset(string g_name);
        void add_score(int val);
        int get_score();
    protected:

    private:
        Vector2 p_init;
        float inicio_salto;
        string obj_name;
        bool jumping;
        float altura_Salto;
        float coyote;
        float end_coyote;
        bool last_ground;
        int score;

};

class Player_UI{
public :
    Player_UI();
     Player_UI(map<string,string>data);
    Player_UI(bool initial_active,vector<string> pantallas_names,map<string,int>opciones_pantallas,map<string,vector<string>>opciones_objs);
    void set_active(bool val);
    void set_pantalla(string name);
    void set_pantalla(int index);
    void update();
    bool is_correct();
    void capture_input(string obj_asociado,int ev_asociado);
    void capture_scrolls(string obj_asociado);
    void captureRadios(vector<string>rads);
    void execute_cola(bool res);
    void add_colaEv(int ev,string args);
    bool is_inModal();

private:
    string pantalla_actual;
    vector<string> lista_pantallas;
    map<string,int> opciones_maximas;
    map<string,vector<string>> names_objs_opciones;
    bool active;
    int opcion_actual;
    int maxima_opcion;
    bool correct;
    string obj_capture;
    int ev_capture;
    bool capturando_input;
    bool capturando_scrolls;
    map<int,string> cola_events;
    bool esperando;
    bool in_modal;
    bool in_radioGroup;
    vector<string> radios;
    int radio_actual;
    float t_modal;
};
#endif // PLAYER_H
