#include "Game_Object.h"
#include "Scene_data.h"
#include "Player.h"
#include "UI_object.h"
#include "Component.h"

Game_Object::Game_Object()
{
   name="";
   tag="";
   w=0;
   h=0;
   active=false;
   angl=0;
   flipped=false;
   UI_id=-1;
   offset_parent=Vector2(0,0);
   pause=false;
}
Game_Object ::Game_Object(SDL_Rect rect,bool active_obj,string nam,string tg,string parentNAME,int ui_tp){



   tag=tg;
   name=nam;
   w=rect.w;
   h=rect.h;
   angl=0;
   flipped=false;
   parent_id=-1;
   parent_name=parentNAME;
   offset_parent=Vector2(0,0);
   active=active_obj;
   initial_active=active;
   initial_pos=Vector2(rect.x,rect.y+rect.h);
   initial_angle=angl;
   UI_id=ui_tp;
   last_pos=initial_pos;
   pause=false;


}
string Game_Object::get_ParentName(){
   return parent_name;
}
int Game_Object::get_parentId(){
   return parent_id;
}
void Game_Object:: set_parent(string nam,int id_p){
       parent_name=nam;
       parent_id=id_p;
       if(parent_name=="" && parent_id==-1){
           Transform* t=Component::get_Transform(name);
           if(t!=NULL){
               Vector2 p=t->Get_Position();
               p=p+offset_parent;
               t->Set_Position(p);
               offset_parent=Vector2(0,0);
           }
       }

}
void Game_Object::reset(){
   Transform* trans=Component::get_Transform(name);
   if(trans!=NULL){
       trans->Set_Position(initial_pos);
   }


   angl=initial_angle;
   flipped=false;
   active=initial_active;
   RigidBody* rb=Component::get_rigidBody(name);

   if(rb!=NULL){
       rb->set_force(Vector2(0,0));
   }
   if(tag=="Player"){

        Player *p=Component::get_Player(name);
        if(p!=NULL){
           p->reset(name);
        }

   }
}

void Game_Object::set_flip(bool val){
   flipped=val;
}

bool Game_Object::get_flip(){
   return flipped;
}
void Game_Object::set_active(bool val){
   active=val;
   if(UI_id!=-1 && val==false){
      reset_UI();
   }
}
string Game_Object::to_str(){

  string msg="";
  msg="name: "+name+", tag: "+tag;
   Transform* trans=Component::get_Transform(name);
   if(trans!=NULL){
       trans->Set_Position(initial_pos);
        msg=msg+", transform:{ "+trans->to_str()+"}";
   }

 // msg=msg+", sprite:{src: "+sprite_src+" , size: "+to_string(rect_sprite.w)+"x"+to_string(rect_sprite.h)+" , position: ("+to_string(rect_sprite.x)+","+to_string(rect_sprite.y)+") }";
  return msg;

}

SDL_Rect Game_Object:: get_object_rect(){
    SDL_Rect temp_rect;
    temp_rect.w=w;
    temp_rect.h=h;
    Vector2 p=Vector2(0,0);
    Transform* trans=Component::get_Transform(name);
    if(trans!=NULL){
       p=trans->Get_Position();
   }
   else{
      p.x=temp_rect.x;
      p.y=-temp_rect.y;
   }

    temp_rect.x=p.x;
    temp_rect.y=-p.y;

    return temp_rect;
}


bool Game_Object::is_active(){
   return active;
}
Game_Object::~Game_Object()
{
    //dtor
}





void Game_Object::update_physic(Vector2 f,Vector2 fix_pos){



      if(abs(fix_pos.y)<=0.0005f){
        fix_pos.y=0;
      }
      if(abs(fix_pos.x)<=0.0005f){
        fix_pos.x=0;
      }
       Transform* trans=Component::get_Transform(name);
      if(trans==NULL){
          return;
      }
      RigidBody* rbG=Component::get_rigidBody(name);
      if(rbG!=NULL){

        rbG->set_force(f);
        rbG->add_torque();
        rbG->update_torque();
        float rot_vel=rbG->get_angularVel().x;

        float step_angle=rbG->get_angularStep();

        float ang_change=rot_vel*step_angle;
        if(abs(rot_vel)<1){
            ang_change=0;
        }

        trans->Rotate(ang_change);



      }


      Vector2 p=trans->Get_Position();
      Vector2 old_p=p;

      p=p+f;
      p=p+fix_pos;
      trans->Set_Position(p);

      Vector2 next_p=trans->Get_Position();




}
void Game_Object::basic_update(){


    Animator *anim=Component::get_Animator(name);
    Render * render_obj=Component::get_Render(name);
    if(anim!=NULL && render_obj!=NULL){
        SDL_Rect spr_next=anim->update();
        render_obj->update_spriteRect(spr_next);

    }
}
void Game_Object::update(){



  if(UI_id!=-1){
    switch(UI_id){
       case UI_TYPES::BUTTON:{

           Button* btn=Component::get_Button(get_name());
           if(btn!=NULL){
               btn->update(this);
           }
        break;
      }
      case UI_TYPES::CHECKBUTTON:{
          CheckButton* check=Component::get_checkButton(get_name());
           if( check!=NULL){
                check->update(this);
           }
         break;
      }
      case UI_TYPES::RADIOBUTTON:{
          RadioButton* radio=Component::get_radioButton(get_name());
           if( radio!=NULL){
                radio->update(this);
           }
         break;
      }
      case UI_TYPES::SCROLLBAR:{
          ScrollBar* scroll=Component::get_scrollBar(get_name());
           if( scroll!=NULL){
                scroll->update(this);
           }
           break;
      }
      case UI_TYPES::VALUEBAR:{
          ValueBar* bar=Component::get_ValueBar(get_name());
          if(bar!=NULL){
               bar->update(this);
          }
         break;
      }
      case UI_TYPES::LABEL:{
          Label* lbl=Component::get_label(get_name());
          if(lbl!=NULL){
               lbl->update();
          }
       }
       case UI_TYPES::IMAGE:{
          basic_update();
       }

    }
  }
  else{


     Transform * t=Component::get_Transform(name);
     if(t!=NULL){
          Vector2 p=t->Get_Position();
          float dif=abs(p.x-last_pos.x)+abs(p.y-last_pos.y);
          SDL_Rect r=get_object_rect();
          float max_v=(r.w<r.h)?r.w:r.h;
          if(dif>=max_v){
             last_pos=p;
             Collider *col=Component::get_Collider(name);
             Vector2 p=t->Get_Position();
             p=p+offset_parent;
             Vector2 dm=Vector2(0,0);
             if(col!=NULL){
                p=p+col->get_figure().get_center();
                dm=col->get_figure().get_dim();
             }
             else{
                SDL_Rect r=get_object_rect();
                dm=Vector2(r.w,r.h);
                p=p+Vector2(dm.x*0.5f,dm.y*0.5f);
             }

             Quatree::modifc_obj(*this,p,dm);

          }

     }
     if(tag=="Player" ){


        Player *p=Component::get_Player(get_name());
        if(p!=NULL){
            p->update(&(*this));
        }
        Player_UI* p_UI=Component::get_playerUI(get_name());
         if(p_UI!=NULL){
              p_UI->update();
         }

     }
     else if(tag=="Proyectil"){
        Proyectil* proy=Component::get_Proyectil(name);
        if(proy!=NULL){
            proy->update(&(*this));
        }
     }
     else if(tag=="Enemy"){
         Enemy* enem=Component::get_Enemy(name);
        if(enem!=NULL){
            enem->update(&(*this));
        }
     }
  }
}
void Game_Object:: OnTrigger(Game_Object other,ManiFold* contact_info){

    last_triggers[other.get_tag()].push_back(other.get_name());


    if(tag=="Player" ){

        Player *p=Component::get_Player(name);
        if(p!=NULL){

            p->OnTrigger(other,this,contact_info);
        }

   }
   else if(tag=="Enemy"){
        Enemy* enem=Component::get_Enemy(name);
        if(enem!=NULL){
            enem->OnTrigger(other,this,contact_info);
         }
   }
    else if(tag=="Proyectil"){
         Proyectil* proy=Component::get_Proyectil(name);
          if(proy!=NULL){
             proy->OnTrigger(other,this,contact_info);
          }
    }
}

 void Game_Object:: OnCollision(Game_Object other,ManiFold* contact_info){





         last_collision[other.get_tag()].push_back(other.get_name());
         RigidBody* rbG=Component::get_rigidBody(name);
         Collider* col=Component::get_Collider(name);

          if(contact_info!=NULL && rbG!=NULL && col!=NULL ){



             if(contact_info->get_normal().y>=0.0  ){


                Vector2 n=contact_info->get_normal();
                float ang=abs(atan2(n.x,n.y)*57.29f);

                Collider* col2=Component::get_Collider(other.get_name());
                if(col2!=NULL){
                   if(ang<=rbG->get_angle_pendiente() && ang>0 &&  col2->get_figure().get_type()!=0){
                         rbG->set_pendiente(true);
                  }

               }
               if(rbG->in_pendiente()==false){
                    if(abs(n.y)>=0.99f){

                           rbG->set_NormFloor(n);
                    }
               }
               else{

                 rbG->set_NormFloor(n);
               }

            }

            if(tag=="Player"){
                Player* pl=Component::get_Player(name);
                if(pl!=NULL){
                     pl->OnCollision(other,this,contact_info);
                }

            }
            else if(tag=="Enemy"){
                Enemy* enem=Component::get_Enemy(name);
                if(enem!=NULL){
                     enem->OnCollision(other,this,contact_info);
                }
            }
            else if(tag=="Proyectil"){
                Proyectil* proy=Component::get_Proyectil(name);
                if(proy!=NULL){
                    proy->OnCollision(other,this,contact_info);
                }
            }
         }

 }
bool Game_Object::in_collision(string other_tag, string other_name,bool trigger){

        bool found=false;
        map<string,vector<string>>::iterator it_m;
        if(trigger==false){
             for(it_m=last_collision.begin();it_m!=last_collision.end();it_m++){
                 string tg=it_m->first;
                 if(tg==other_tag){
                     vector<string> res=it_m->second;
                      vector<string>::iterator it_v;

                      for(it_v=res.begin();it_v!=res.end();it_v++){
                         string nm=*it_v;
                         if(nm==other_name){
                              found=true;
                               break;
                         }
                     }
                    break;
                }
            }
        }
        else{

            for(it_m=last_triggers.begin();it_m!=last_triggers.end();it_m++){
                 string tg=it_m->first;
                 if(tg==other_tag){
                     vector<string> res=it_m->second;
                      vector<string>::iterator it_v;

                      for(it_v=res.begin();it_v!=res.end();it_v++){
                         string nm=*it_v;
                         if(nm==other_name){
                              found=true;
                               break;
                         }
                     }
                    break;
                }
            }
        }
   return found;
}

void Game_Object::reset_collisions_data(){


    last_collision.clear();
    last_triggers.clear();
    RigidBody* rbG=Component::get_rigidBody(name);
    if(rbG!=NULL){
         rbG->set_Ground(false);
         rbG->set_pendiente(false);
    }



}

string Game_Object::get_name(){
   return name;
}


string Game_Object:: get_tag(){
   return tag;
}
int Game_Object::get_UIid(){
  return UI_id;
}

void Game_Object::set_pause(bool val){
   pause=val;
}
bool Game_Object::get_pause(){
   return pause;
}

 void Game_Object:: reset_UI(){

    switch(UI_id){
       case UI_TYPES::BUTTON:{

           Button* btn=Component::get_Button(get_name());
           if(btn!=NULL){
               btn->reset(this);
           }
        break;
      }
      case UI_TYPES::CHECKBUTTON:{
          CheckButton* check=Component::get_checkButton(get_name());
           if( check!=NULL){
                check->reset(this);
           }
         break;
      }
      case UI_TYPES::RADIOBUTTON:{
          RadioButton* radio=Component::get_radioButton(get_name());
           if( radio!=NULL){
                radio->reset(this);
           }
         break;
      }
      case UI_TYPES::SCROLLBAR:{
          ScrollBar* scroll=Component::get_scrollBar(get_name());
           if( scroll!=NULL){
                scroll->reset(this);
           }
           break;
      }
      case UI_TYPES::VALUEBAR:{
           ValueBar* bar=Component::get_ValueBar(get_name());
           if( bar!=NULL){
                bar->reset(this);
           }
           break;
      }
      case UI_TYPES::LABEL:{
          Label* lbl=Component::get_label(get_name());
          if(lbl!=NULL){
              lbl->reset();
          }
        }
    }
 }
 vector<Game_Object> Game_Object::objs;
 vector<Game_Object>& Game_Object::get_objs(){
     return objs;
 }
 Game_Object Game_Object::Find(string name,int& index){
    Game_Object res=Game_Object();
    int index_g=0;
    vector<Game_Object>::iterator it;
    for(it=objs.begin();it!=objs.end();it++){
         Game_Object g=*it;
         if(g.get_name()==name){
            res=g;
            index=index_g;
            break;
         }
         index_g++;

    }
    return res;
 }
 void Game_Object::set_objs(vector<Game_Object>& objects){
      objs=objects;
      vector<Game_Object> ::iterator it;
      bool cam_activa=false;
      Camera cam=Camera::get_ActiveCamera(cam_activa);
      int index=0;
      for(it=objs.begin();it!=objs.end();it++){
           Game_Object g=*it;

           if(cam_activa){

              if(g.get_name()==cam.get_looking_target()){
                  cam.set_looking_index(index);
                  Camera::editActiveCamera(cam);
              }
           }
           string parent=g.get_ParentName();
           if(parent!=""){
                int index_parent=0;
                bool found=false;
                vector<Game_Object> ::iterator it_parent;
                for(it_parent=objs.begin();it_parent!=objs.end();it_parent++){
                    Game_Object g2=*it_parent;
                    string temp_name=g2.get_name();
                    if(temp_name!=g.get_name()){
                        if(temp_name==parent){
                           found=true;
                           g.set_parent(parent,index_parent);
                           break;
                        }
                    }
                    index_parent++;
                }
                if(found==false){
                     g.set_parent("",-1);
                }
              objs[index]=g;
           }
           index++;
      }
 }
 void Game_Object::edit_obj(Game_Object g,int index){
     if(index<0 || index>=objs.size()){
          return;
     }
     if(paused_game){
        return;
     }
     Game_Object g2=objs[index];
     if(g2.get_name()==g.get_name()){
          objs[index]=g;
     }
 }

 void Game_Object:: pause_game(bool val){
   paused_game=val;

   vector<Game_Object>::iterator it;
   int index=0;
   for(it=objs.begin();it!=objs.end();it++){

       Game_Object g=*it;
       if(g.get_tag()!="Player"){
         g.set_pause(val);
       }
       objs[index]=g;
       index+=1;
   }

}
  void Game_Object::destroy_obj(Game_Object g,int index){
        if(paused_game){
             return;
        }
        if(index<0 || index>=objs.size()){
             return;
        }
        vector<Game_Object>::iterator it;
        string name_g=g.get_name();
        Game_Object target=objs[index];
        if(target.get_name()!=name_g){
            return;
        }

        //borramos parentesco de los childs
        int index_g=0;
        for(it=objs.begin();it!=objs.end();it++){
            Game_Object g2=*it;
            if(g2.get_ParentName()==name_g){
                  g2.set_parent("",-1);
                objs[index_g]=g2;
            }
            index_g++;
        }
        objs.erase(objs.begin()+index);

  }
void Game_Object::add_obj(Game_Object g){
       if(paused_game){
           return;
       }
       if(g.get_name()==""){
         objs.push_back(g);
       }

  }

Vector2 Game_Object::get_initPos(){
  return initial_pos;
}
 void Game_Object::set_parentOffset(Vector2 offs){
     offset_parent=offs;
 }
  Vector2 Game_Object::get_parentOffset(){
      return offset_parent;
  }
void Game_Object::reset_QuatreeData(){
    Quatree_data.clear();
}
 void Game_Object::add_QuatreeData(string root,string id){
       Quatree_data[root].push_back(id);
 }
 map<string,vector<string>> Game_Object::get_QuatreeData(){
     return Quatree_data;
 }


void Game_Object::limpiar_objs(){


   if(objs.size()>0){
      objs.clear();
   }
}
 bool Game_Object::game_is_paused(){
    return paused_game;
 }
bool Game_Object::paused_game=false;
