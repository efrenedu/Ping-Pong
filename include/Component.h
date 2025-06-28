#ifndef COMPONENT_H
#include<UI_object.h>
#include<Game_Object.h>
#include<Player.h>
#include<projectiles.h>
#include<Enemy.h>
#define COMPONENT_H


enum COMP_TYPES{
   COMP_PLAYER=0,COMP_BUTTON=1,COMP_LABEL=2,COMP_IMAGE_UI=3,
   COMP_SCROLLBAR=4,COMP_VALUEBAR=5,COMP_RIGIDBODY=6,
   COMP_COLLIDER=7,COMP_RADIOBUTTON=8,COMP_CHECKBUTTON=9,
   COMP_TRANSFORM=10,COMP_RENDER=11,COMP_ANIMATOR=12,
   COMP_PROYECTIL=13,COMP_ENEMY=14,COMP_PLAYER_UI=15,
};


class Component{
   public :

    static void add_comp(string name,Label comp);
    static void add_comp(string name,Button comp);
    static void add_comp(string name,RadioButton comp);
    static void add_comp(string name,CheckButton comp);
    static void add_comp(string name,Image_UI comp);
    static void add_comp(string name,ScrollBar comp);
    static void add_comp(string name,ValueBar comp);
    static void add_comp(string name,Player comp);
    static void add_comp(string name,RigidBody comp);
    static void add_comp(string name,Collider comp);
    static void add_comp(string name,Transform comp);
    static void add_comp(string name,Render comp);
   static void add_comp(string name,Animator comp);
   static void add_comp(string name,Proyectil comp);
   static void add_comp(string name,Enemy comp);
   static void add_comp(string name,Player_UI comp);



    static Button* get_Button(string name);
    static Label* get_label(string name);
    static RadioButton* get_radioButton(string name);
    static CheckButton* get_checkButton(string name);
    static Image_UI* get_ImageUI(string name);
    static ScrollBar* get_scrollBar(string name);
    static ValueBar* get_ValueBar(string name);
    static Player* get_Player(string name);
    static RigidBody* get_rigidBody(string name);
    static Collider* get_Collider(string name);
    static Transform* get_Transform(string name);
    static Render* get_Render(string name);
    static Animator* get_Animator(string name);
    static Proyectil* get_Proyectil(string name);
    static Enemy* get_Enemy(string name);
    static Player_UI *get_playerUI(string name);
    static void reset();


   private:

      static void set_defaultComps(string name,int index);
      static map<string,Player> player_comps;
      static map<string,Button> button_comps;
      static map<string,Label> labels_comps;
      static map<string,Image_UI> images_comps;
      static map<string,ScrollBar> scroll_comps;
      static map<string,ValueBar> bars_comps;
      static map<string,Render> render_comps;
      static map<string,RigidBody> rigid_comps;
      static map<string,Collider> colliders_comps;
      static map<string,Transform> transform_comps;
      static map<string,RadioButton> radio_comps;
      static map<string ,CheckButton> checks_comps;
      static map<string, Animator> anim_comps;
      static map<string, Proyectil> proyectiles_comps;
      static map<string ,Enemy> Enemy_comps;
      static map<string ,Player_UI> PlayerUI_comps;
      static map<string, vector<bool>> objs_comps;

      static int num_comps;
};

#endif // COMPONENT_H
