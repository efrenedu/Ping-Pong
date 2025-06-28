#ifndef ANIMATION_H
#define ANIMATION_H
#include<Render.h>



struct Condition_Transition{
public:
    Condition_Transition();
    Condition_Transition(string cond_name,string cond_val,string oper);
    bool condition_clear(string param_name,string param_val);
    string get_name();
    string get_valor();
    string get_operacion_type();
private:
    string condicion_name;
    string valor;
    string operacion;
    int val_type;

};
struct Transition{
public:
    Transition();
    Transition(string from,string to ,bool wait_finish,string operador);
    void add_condition(Condition_Transition cond);
    vector<string> get_anim_names();
    vector<Condition_Transition> get_condiciones();
    string get_operador_condiciones();

private:
    vector<Condition_Transition> condiciones;
    string operador_verificacion;
    string anim_from;
    string anim_to;


};

struct Frame{
public:
    Frame();
    Frame(int dur,SDL_Rect rect);
    SDL_Rect get_rect_sprite();
    float get_duracion();
    bool is_valid();
private:
    SDL_Rect sprite_pos;
    float duracion;
    bool valid_frame;
};
struct Animation{
public:
    Animation();
    Animation( string nombre,bool is_cicle);
    void add_Transition(Transition tr);
    void add_frame(Frame frm);
    Frame get_Frame_Actual();
    void update_animation();
    bool is_last_frame();
    bool is_playing();
    string get_name();
    vector<Transition> get_transitions();
    bool is_cicle();
    void next_frame();

private:
    vector<Frame> frames;
    vector<Transition> transiciones;
    bool cicle;
    bool playing;
    int frame_actual;
    string name;
    int num_frames;


};
class Animator{
public:
    Animator();
    Animator(string src,int w_spr,int h_spr);
    void add_animation(Animation anim);
    Animation get_actual_Animation();
    Animation get_Animation(int index);
    int get_Animation_index(string name);
    Animation get_Animation(string name_anim);
    void add_parameter(string name_param, string value);
    void set_parameter_value(string param,string value);
    string get_parameter_value(string param);
    bool is_loaded();
    SDL_Rect update();
    void read_source(string src,int w_spr,int h_spr);
    void reset_params();
    void reset();
private:

    int num_animations;
    int animacion_actual;
    vector<Animation> animaciones;
    map<string,string> parametros_animator;
    map<string,string> params_iniciales;
    int num_parametros;
    bool loaded;
    float cont_frame;


};


#endif // ANIMATION_H
