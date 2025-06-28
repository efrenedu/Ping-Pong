#ifndef SCENE_DATA_H
#define SCENE_DATA_H
#include<UI_object.h>
#include<Basic_Game_Data.h>
#include<cstring>
#include<Screen.h>
#include<thread>


enum OBJ_TYPE{
  UI=0, GAME_OBJ=1,TILE=2
};

struct Scene{
public:
    Scene();
    Scene(string name,vector<Game_Object> objs,int texture_background,Vector2 p_begin,Vector2 p_end);
    string get_name();
    int get_id_texture_background();
    vector<int> get_colliders_indexs();
    vector<int> get_rigid_bodies_indexs();
    vector<int> get_other_sprites_indexs();
    vector<Vector2> get_mapa_inicio_fin();
    void reset();






private:
    string nombre;
    int id_texture_background;
    vector<int>RigidBodies_scene;
    vector<int>colliders_scene;
    vector<int>other_sprites;
    Vector2 inicio_mapa;
    Vector2 fin_mapa;


};

class Scene_manager{
public:
    static void load_scene(string name);
    static void load_scene(int index);
    static void add_scene(string name);
    static void set_scene(Scene s);
    static void reset();
    static Scene get_active_escene();
    static void set_scale_world(float val);
    static void set_renderer(SDL_Renderer* rendr);
    static bool is_loading();
    static void reset_esceneObjects();
    static void update();
    static float get_wScale();

private:
   static void read_escena(string name,vector<Game_Object>&obsj,vector<Game_Object>& ui_objs,vector<Camera>& cams,int& cam_activa,vector<Tile>& tls);
   static Scene escena_activa;
   static bool loading_scene;
   static bool previous_loaded;
   static float w_scale;
   static SDL_Renderer* renderer;
   static bool on_reset;
   static string enCola;
   static string escena_actual;
   static vector<string> escenas_list;



};

struct Quatree_Node{
public:
    Quatree_Node();
    Quatree_Node(Vector2 dm,Vector2 p,int profund,vector<int> tls,vector<int>objs_i,string n_id,string root_id);
    bool is_parent();
    vector<Quatree_Node> get_node_contains(Vector2 p,Vector2 dm);
    bool in_collision(Vector2 p,Vector2 dm);
    int get_profundidad();
    Vector2 get_dimension();
    Vector2 get_center();
    bool is_valid();
    vector<int> get_tiles();
    vector<Quatree_Node> search_node(map<int,vector<int>>indexs);
    string get_id();
    string get_root();
    map<string,int> get_objs();
    void remove_object(Game_Object g,vector<string> ids_search);
    void add_object(Game_Object g,Vector2 pos,Vector2 dm);
    void search_object(string nam);
private:

    string id;
    string root;
    map<string,int> objs;
    vector<int> tiles;
    vector<Quatree_Node> childs;
    bool has_childs;
    Vector2 dimension;
    Vector2 center;
    int profund_node;
    bool validad;


};
class Quatree{
public:

    static void make_tree( int profundidad,Scene s);
    static tuple<vector<int>,vector<int>>get_elements_in_radious(Vector2 p,float radio);
    static bool is_valid();
    static void modifc_obj(Game_Object g,Vector2 pos,Vector2 dm);
    static bool update();
    static void remove_fromQ(Game_Object g);
    static void add_toQ(Game_Object g,Vector2 pos,Vector2 dm);
    static void search_Q(string nam);




private:
     static vector<Quatree_Node> nodos_q;
     static Vector2 size_q;
     static Vector2 center_q;
     static bool validado;
     static int max_profund;
     static map<string,tuple<Game_Object,Vector2,Vector2>>Cola;
     static bool updating;

};

#endif // SCENE_DATA_H
