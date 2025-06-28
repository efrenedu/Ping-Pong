#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include<Animation.h>
#include<AudioManager.h>


class Game_Object
{
    public:
        Game_Object();
        Game_Object(SDL_Rect rect,bool active_obj,string nam,string tg,string parentNAME,int ui_tp=-1);
        virtual ~Game_Object();
        string to_str();
        bool is_active();
        SDL_Rect get_object_rect();
        void basic_update();
        void update_physic(Vector2 f,Vector2 fix_pos);
        void update();
        void OnCollision(Game_Object other,ManiFold* contact_info);
        void OnTrigger(Game_Object other,ManiFold* contact_info);
        bool in_collision(string other_tag, string other_name,bool trigger);
        void reset_collisions_data();
        void set_flip(bool val);
        bool get_flip();
        void set_active(bool val);
        string get_name();
        string get_tag();
        void reset();
        int parent_id;
        string parent_name;
        string get_ParentName();
        int get_parentId();
        void set_parent(string nam,int id_p);
        Vector2 get_initPos();
        void set_parentOffset(Vector2 offs);
        Vector2 get_parentOffset();
        int get_UIid();
        void reset_UI();
        void set_pause(bool val);
        bool get_pause();

        void reset_QuatreeData();
        void add_QuatreeData(string root, string id);
        map<string,vector<string>> get_QuatreeData();

        //metodos de la clase
        static vector<Game_Object>& get_objs();
        static Game_Object Find(string name,int& index);
        static void set_objs(vector<Game_Object>& objects);
        static void edit_obj(Game_Object g,int index);
        static void destroy_obj(Game_Object g,int index);
        static void add_obj(Game_Object g);
        static void limpiar_objs();
        static void pause_game(bool val);
        static bool game_is_paused();

//metodos para agregar componentes


    protected:

    private:

        string name;
        string tag;
       bool active;
       float w;
       float h;
       bool flipped;
       int angl;
       Vector2 initial_pos;
       float initial_angle;
       bool initial_active;
       int UI_id;
       Vector2 offset_parent;
       map<string,vector<string>> Quatree_data;
       Vector2 last_pos;
       bool pause;
       static bool paused_game;


       static vector<Game_Object> objs;


       map<string,vector<string>> last_collision;
       map<string,vector<string>> last_triggers;
};



#endif // GAME_OBJECT_H
