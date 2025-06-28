#include "Component.h"

 map<string,Player> Component:: player_comps;
 map<string,Button>  Component:: button_comps;
 map<string,Label>  Component:: labels_comps;
 map<string,Image_UI>  Component:: images_comps;
map<string,ScrollBar>  Component:: scroll_comps;
map<string,ValueBar>  Component:: bars_comps;
map<string,Render>  Component:: render_comps;
 map<string,RigidBody>  Component:: rigid_comps;
 map<string,Collider>  Component:: colliders_comps;
 map<string,Transform>  Component:: transform_comps;
 map<string,RadioButton>  Component::radio_comps;
  map<string,CheckButton>  Component::checks_comps;
 map<string,Animator>  Component::anim_comps;
 map<string,Proyectil> Component::proyectiles_comps;
 map<string ,Enemy> Component::Enemy_comps;
 map<string,Player_UI> Component::PlayerUI_comps;
 map<string,vector<bool>> Component::objs_comps;

 int Component::num_comps=16;

 void  Component::set_defaultComps(string name,int index){
      map<string,vector<bool>> ::iterator it;
      it=objs_comps.find(name);
       if(it==objs_comps.end()){
           vector<bool> v;
           for(int i=0;i<num_comps;i++){
               v.push_back(false);
           }
           v[index]=true;
           objs_comps[name]=v;
       }
       else{
        objs_comps[name][index]=true;
       }
 }
 void Component::add_comp(string name,Button comp){
     button_comps[name]=comp;
     map<string,vector<bool>> ::iterator it;
     set_defaultComps(name,COMP_TYPES::COMP_BUTTON);

 }
 void Component::add_comp(string name,Label comp){
    labels_comps[name]=comp;
     set_defaultComps(name,COMP_TYPES::COMP_LABEL);
 }
 void Component::add_comp(string name,RadioButton comp){
    radio_comps[name]=comp;
     set_defaultComps(name,COMP_TYPES::COMP_RADIOBUTTON);
 }
 void Component::add_comp(string name,CheckButton comp){
      checks_comps[name]=comp;
       set_defaultComps(name,COMP_TYPES::COMP_CHECKBUTTON);
 }
void Component::add_comp(string name,Image_UI comp){
     images_comps[name]=comp;
      set_defaultComps(name,COMP_TYPES::COMP_IMAGE_UI);
}
void Component::add_comp(string name,ScrollBar comp){
      scroll_comps[name]=comp;
       set_defaultComps(name,COMP_TYPES::COMP_SCROLLBAR);
}
 void Component::add_comp(string name,ValueBar comp){
     bars_comps[name]=comp;
      set_defaultComps(name,COMP_TYPES::COMP_VALUEBAR);
 }
void Component::add_comp(string name,Player comp){
    player_comps[name]=comp;
     set_defaultComps(name,COMP_TYPES::COMP_PLAYER);
}
 void Component::add_comp(string name,RigidBody comp){

       rigid_comps[name]=comp;
      set_defaultComps(name,COMP_TYPES::COMP_RIGIDBODY);
 }
 void Component::add_comp(string name,Collider comp){
     colliders_comps[name]=comp;
      set_defaultComps(name,COMP_TYPES::COMP_COLLIDER);
 }
void Component::add_comp(string name,Transform comp){
    transform_comps[name]=comp;
    set_defaultComps(name,COMP_TYPES::COMP_TRANSFORM);
}
void Component::add_comp(string name,Render comp){
    render_comps[name]=comp;
    set_defaultComps(name,COMP_TYPES::COMP_RENDER);
}
void Component::add_comp(string name,Animator comp){
    anim_comps[name]=comp;
    set_defaultComps(name,COMP_TYPES::COMP_ANIMATOR);
}

void Component::add_comp(string name, Proyectil comp){
  proyectiles_comps[name]=comp;
  set_defaultComps(name,COMP_TYPES::COMP_PROYECTIL);

}
void Component:: add_comp(string name,Enemy comp){
   Enemy_comps[name]=comp;
  set_defaultComps(name,COMP_TYPES::COMP_ENEMY);

}
void Component::add_comp(string name,Player_UI comp){
   PlayerUI_comps[name]=comp;
   set_defaultComps(name,COMP_TYPES::COMP_PLAYER_UI);
}




 Button* Component:: get_Button(string name){
      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
           objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_BUTTON];
      if(val==false){

        return NULL;
      }

      return &button_comps[name];


 }

Label* Component:: get_label(string name){

      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
           objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_LABEL];
      if(val==false){
        return NULL;
      }

     return &labels_comps[name];

}
RadioButton* Component::get_radioButton(string name){

      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
           objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_RADIOBUTTON];
      if(val==false){
        return NULL;
      }
        return &radio_comps[name];


}
 CheckButton* Component::get_checkButton(string name){
      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
           objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_CHECKBUTTON];
      if(val==false){
        return NULL;
      }
      return &checks_comps[name];

 }
 Image_UI* Component::get_ImageUI(string name){

      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
           objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_IMAGE_UI];
      if(val==false){
        return NULL;
      }
        return &images_comps[name];

 }
 ScrollBar* Component::get_scrollBar(string name){
      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
          objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_SCROLLBAR];
      if(val==false){
        return NULL;
      }
          return &scroll_comps[name];


 }
ValueBar* Component::get_ValueBar(string name){
     vector<bool> v=objs_comps[name];
      if(v.size()<=0){
          objs_comps.erase(name);
          return NULL;
      }
     bool val=objs_comps[name][COMP_TYPES::COMP_VALUEBAR];
      if(val==false){
        return NULL;
      }
          return &bars_comps[name];


}
 Player* Component::get_Player(string name){
      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
          objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_PLAYER];
      if(val==false){
        return NULL;
      }
          return &player_comps[name];


 }
 RigidBody* Component::get_rigidBody(string name){
      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
         objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_RIGIDBODY];
      if(val==false){
        return NULL;
      }
          return &rigid_comps[name];


 }
Collider* Component::get_Collider(string name){
       vector<bool> v=objs_comps[name];
      if(v.size()<=0){
         objs_comps.erase(name);
          return NULL;
      }
       bool val=objs_comps[name][COMP_TYPES::COMP_COLLIDER];
      if(val==false){
        return NULL;
      }
        return &colliders_comps[name];
}

Transform* Component::get_Transform(string name){
       vector<bool> v=objs_comps[name];
      if(v.size()<=0){
         objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_TRANSFORM];
      if(val==false){
        return NULL;
      }
        return &transform_comps[name];

}

Render* Component::get_Render(string name){
       vector<bool> v=objs_comps[name];
      if(v.size()<=0){
         objs_comps.erase(name);
          return NULL;
      }
       bool val=objs_comps[name][COMP_TYPES::COMP_RENDER];
      if(val==false){
          return NULL;
      }
      return &render_comps[name];

}

Animator* Component::get_Animator(string name){
      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
         objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_ANIMATOR];
      if(val==false){
          return NULL;
      }
      return &anim_comps[name];

}
Proyectil* Component::get_Proyectil(string name){
       vector<bool> v=objs_comps[name];
      if(v.size()<=0){
         objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_PROYECTIL];
      if(val==false){
          return NULL;
      }
       return &proyectiles_comps[name];
}

Enemy* Component::get_Enemy(string name){
      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
         objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_ENEMY];
      if(val==false){
          return NULL;
      }
      return &Enemy_comps[name];
}
Player_UI* Component::get_playerUI(string name){
      vector<bool> v=objs_comps[name];
      if(v.size()<=0){
         objs_comps.erase(name);
          return NULL;
      }
      bool val=objs_comps[name][COMP_TYPES::COMP_PLAYER_UI];
      if(val==false){
          return NULL;
      }
      return &PlayerUI_comps[name];
}

void Component::reset(){

     anim_comps.clear();
     transform_comps.clear();
     rigid_comps.clear();
     colliders_comps.clear();
     button_comps.clear();
     scroll_comps.clear();
     radio_comps.clear();
     labels_comps.clear();
     bars_comps.clear();
     checks_comps.clear();
     images_comps.clear();
     player_comps.clear();
     proyectiles_comps.clear();
     Enemy_comps.clear();
     PlayerUI_comps.clear();
     map<string,Render>::iterator it;
     for(it=render_comps.begin();it!=render_comps.end();it++){
        Render r=it->second;
        r.liberar();
     }
     render_comps.clear();

     objs_comps.clear();
}
