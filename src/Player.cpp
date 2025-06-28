#include "player.h"
#include "Game_Object.h"
#include "Scene_data.h"
#include "Event_Manager.h"
#include "Component.h"
#include "Physic_Engine.h"
#include "Singleton.h"

Player::Player()
{
    p_init=Vector2(0,0);
    obj_name="";
    score=0;
   /* inicio_salto=0.0f;
    jumping=false;
    altura_Salto=65.0f;
    coyote=0.0f;
    end_coyote=0.0f;*/

}
Player::Player(string  name,Vector2 p,string script_data)
{
    p_init=p;
    obj_name=name;
    score=0;
   /* inicio_salto=0.0f;
    jumping=false;
    altura_Salto=65.0f;
    coyote=0.0f;
    end_coyote=0.0f;
    last_ground=false;*/

    if(script_data!=""){


        string temp="";
        string temp_var="";
        string temp_value="";

        for(int i=0;i<script_data.length();i++){

            unsigned char c=script_data[i];
            if(c=='|'){
                // asignamos valores a variables
                if(temp!="" && temp_var!="" && temp_value==""){
                    temp_value=temp;
                }
                if(temp_var=="Player_hp"){
                  //variable hp detectada
                }
                temp="";
                temp_var="";
                temp_value="";
            }
            else if(c==','){
                  //conjunto de valores como x,y de un vector

                 temp="";
            }
            else if(c==';'){
                  //siguiente conjunto de valores como varios vectores
                 temp="";
            }
            else if(c==':'){

                //obtenemos variable
                temp_var=temp;
                temp="";
                temp_value="";
            }
            else{
                temp+=c;
            }
        }
    }

}
Player::~Player()
{
    //dtor

}
void Player::update(Game_Object * g ){


   string key_down=Singleton::get_key("Down");
   string key_up=Singleton::get_key("Up");
   if(g==NULL){
       return;
   }
    Transform *t=Component::get_Transform(g->get_name());
    if(t==NULL){
        return;
    }

    string key_exit=Singleton::get_key("Exit");
    if(Input::Key_Down(key_exit) && Singleton::get_victory()==-1){

           int index_m=-1;
           bool modal_active=false;
           Game_Object g_m=UI_object::Find("Modal",index_m);
           if(index_m!=-1){

               if(g_m.is_active()){
                   modal_active=true;
               }
           }

           if(modal_active==false){

              string next_escena="Inicio";
              Event_Manager::execute_event(EVENT_TYPES::ADD_EVENT_COLA,g->get_name()+"-"+to_string(EVENT_TYPES::LOAD_SCENE)+"-"+next_escena);
              Event_Manager::execute_event(EVENT_TYPES::SHOW_UI,"Modal");
              Event_Manager::execute_event(EVENT_TYPES::PAUSE_GAME,"");
              Event_Manager::execute_event(EVENT_TYPES::CHANGE_PANTALLA_UI,g->get_name()+",Modal");

           }
           else{

              Event_Manager::execute_event(EVENT_TYPES::HIDE_UI,"Modal");
              Event_Manager::execute_event(EVENT_TYPES::UNPAUSE_GAME,"");
              Event_Manager::execute_event(EVENT_TYPES::RETURN_MSGBOX,g->get_name()+",False");

           }



    }

    Collider *col=Component::get_Collider(g->get_name());
    RigidBody* rb=Component::get_rigidBody(g->get_name());

    if(rb!=NULL && col!=NULL){
        if(Game_Object::game_is_paused() && Singleton::get_victory()==-1){
                rb->set_force(Vector2(0,0));
                return;
         }
         Vector2 dire=Vector2(0,0);
         float vel=200.0f;
         if(Input::Key_Down(key_down)|| Input::Key_Pressed(key_down)){
              dire.y=-1;
         }
         else if(Input::Key_Down(key_up)|| Input::Key_Pressed(key_up)){
            dire.y=1;
         }

         dire.y=dire.y*vel*Timer::get_delta_time();

         rb->set_force(dire);

    }

   Label* lbl=Component::get_label("score_player");
   if(lbl!=NULL){


      string next_score="";
      if(score<10){
        next_score="0"+to_string(score);
      }
      else{
        next_score=to_string(score);
      }


      lbl->set_text(next_score);
   }
  /* bool ground=false;
   bool wall=false;
   if(rb!=NULL){
       float coyote_max=rb->get_coyote_time();

   }
    if(col!=NULL ){
         Vector2 c=col->get_figure().get_center()+t->Get_Position();
         c=c+g->get_parentOffset();
         c.y=c.y-(col->get_figure().get_halfDim().y*0.8f);
         Vector2 dire=Vector2(0,-1);

         float dist=15.0f;
         Ray r=Ray(c,dire,dist);
         if(Physic_Engine::Raycast_Collision(r,g->get_name())==true){
             ground=true;

         }

    }

    bool in_coyote=false;
    if(coyote>0.0f){
        in_coyote=true;
        coyote=coyote+Timer::get_delta_time();
        if(coyote>=rb->get_coyote_time()){
           coyote=0.0f;
        }
    }
    if(ground==false && last_ground==true){
        in_coyote=true;
        coyote=Timer::get_delta_time();
    }



    last_ground=ground;
    float vel=565.0f;
    float velY=280.0f;
   Vector2 f=Vector2(0,0);
   Vector2 last_f=Vector2(0,0);

  if(rb!=NULL){

       rb->set_coyote(in_coyote);
       f.y=rb->get_last_force().y;


       if(Input::Key_Pressed("Left") || Input::Key_Down("Left")){

          f.x=-vel*Timer::get_fixed_deltaTime();
          g->set_flip(true);

       }
        else if(Input::Key_Pressed("Right") || Input::Key_Down("Right") ){

          f.x=vel*Timer::get_fixed_deltaTime();
           g->set_flip(false);

       }
      if(f.x!=0){
            if(rb->in_pendiente()==true ){
                 Vector2 last_rampa=rb->get_lastNorm_Floor();
                 if(last_rampa.x!=0 && last_rampa.y!=0){

                      float max_ang=rb->get_angle_pendiente();
                      float ang=atan2(last_rampa.x,last_rampa.y)*57.29f;
                      ang=abs(ang);
                      if(ang<=max_ang){
                          f.y=f.y+((vel*(Timer::get_fixed_deltaTime()))*1.0f);
                      }

                 }


          }
          else{
            if(jumping==false){
                if(f.y>0){
                    f.y=0;
                }
            }
          }
      }
      if(Input::Key_Down("X")){


         if(jumping==false && (ground==true || in_coyote==true)){

            inicio_salto=t->Get_Position().y;
            jumping=true;
            if(col!=NULL){
                    col->set_trigger(true);

            }
            f.y=0.0f;
            AudioManager::Play_Sfx("jump",0,0);

         }



       }
       if(jumping==true){

           f.y=f.y+(velY*Timer::get_fixed_deltaTime());

           Transform *t=Component::get_Transform(g->get_name());
          if(t!=NULL){
               if(t->Get_Position().y>=inicio_salto+altura_Salto){
                   jumping=false;
                   f.y=0.0f;
                   if(col!=NULL){
                    col->set_trigger(false);
                   }
               }
          }
       }
       last_f=rb->get_last_force();
       rb->set_force(f);

   }

   Animator* anim=Component::get_Animator(g->get_name());
   if(anim!=NULL){
      anim->set_parameter_value("velX",to_string(abs(last_f.x)));
      anim->set_parameter_value("velY",to_string(abs(last_f.y)));
   }


if(Input::Key_Down("W")){
     Transform* t=Component::get_Transform("Machu pichu");

}


if(Input::Key_Down("P")){


    string obj_ui="Pantalla01";

     int index=0;
     Game_Object gui=UI_object::Find(obj_ui,index);

     if(index!=-1){
          gui.set_active(true);
          UI_object::EditUI(gui,index);

     }


}
if(Input::Key_Down("Escape")){

    Event_Manager::execute_event(EVENT_TYPES::EXIT_GAME,"");

}
*/

}
 int Player:: get_score(){
    return score;
 }
void Player::OnTrigger(Game_Object other,Game_Object* g,ManiFold* contact_info){

/*
     if(other->get_tag()=="floor"){
            Collider* col=Component::get_Collider(g->get_name());
            if(col!=NULL){
                if(col->is_trigger()){
                      Vector2 n=contact_info->get_normal();
                      if((n.y==1 || n.y==-1)==false){
                          col->set_trigger(false);
                      }
                }
            }
     }
     if(other->get_tag()=="Dead"){


         g->reset();

     }
     else if(other->get_tag()=="enemy"){

     }

     else if(other->get_tag()=="item"){

     }
     else if(other->get_tag()=="checkpoint"){

     }

     else if(other->get_tag()=="zona"){

     }
     else if(other->get_tag()=="interactuable"){

     }
     else if(other->get_tag()=="meta"){

     }
     */
}
void  Player::OnCollision(Game_Object other,Game_Object* g,ManiFold* contact_info){

     if(other.get_name()=="ball"){
          Enemy* enem=Component::get_Enemy("bar_enemy");
          if(enem!=NULL){
                 enem->add_hitPlayer();
          }
     }
    /* if(other.get_tag()=="floor"){


           if(contact_info!=NULL){

               if(contact_info->get_normal().y<0 && jumping==true){

                   Collider* c_other=Component::get_Collider(other.get_name());
                   Transform* t=Component::get_Transform(other.get_name());
                   Transform* t_ply=Component::get_Transform(g->get_name());
                    Collider* c_ply=Component::get_Collider(g->get_name());


                   if(c_other!=NULL && t!=NULL && t_ply!=NULL && c_ply!=NULL){
                       Vector2 c=c_other->get_figure().get_center()+(t->Get_Position()+other.get_parentOffset());
                       Vector2 c2=c_ply->get_figure().get_center()+(t_ply->Get_Position()+g.get_parentOffset());
                       if(c.y>c2.y){

                          jumping=false;;

                       }
                   }

               }
           }
     }*/
}

void Player::add_score(int val){
    score=score+val;
}
void Player::reset(string g_name){

}
Player_UI::Player_UI(){
  active=false;
  pantalla_actual="";
  opcion_actual=0;
  maxima_opcion=0;
  correct=false;
  obj_capture="";
   ev_capture=-1;
   capturando_input=false;
   capturando_scrolls=false;
   esperando=false;
   in_modal=false;

}
Player_UI::  Player_UI(map<string,string>data){
   correct=false;
   obj_capture="";
   ev_capture=-1;
   capturando_input=false;
   esperando=false;
   capturando_scrolls=false;
   in_modal=false;


   if(data.size()>0){
       string arg=data["Player_UI_Pantallas"];
       if(arg!=""){
           string temp="";
           for(int i=0;i<arg.length();i++){
               unsigned char c=arg[i];
               if(c==','){
                   if(temp!=""){
                     lista_pantallas.push_back(temp);
                   }

                   temp="";
               }
               else{
                   temp+=c;
               }
           }
           if(temp!=""){
             lista_pantallas.push_back(temp);
           }
       }
       arg=data["Player_UI_Opcion"];
       if(arg!=""){
           string temp="";
           string temp_key="";
           for(int i=0;i<arg.length();i++){
               unsigned char c=arg[i];
               if(c==';'){
                   if(temp!="" && temp_key!=""){
                     int val=Math_G::StrToInt(temp);
                     opciones_maximas[temp_key]=val;

                   }
                   temp="";
                   temp_key="";

               }
               else if(c==':'){
                  if(temp!=""){
                      temp_key=temp;
                      temp="";
                  }
               }
               else{
                   temp+=c;
               }
           }
       }
       arg=data["Player_UI_Obs"];
       if(arg!=""){
           string temp="";
           string temp_key="";

           for(int i=0;i<arg.length();i++){
               unsigned char c=arg[i];
               if(c==';'){
                   if(temp!=""){
                      names_objs_opciones[temp_key].push_back(temp);
                   }
                   temp_key="";
                   temp="";

               }
               else if(c==','){
                  if(temp!="" && temp_key!=""){
                     names_objs_opciones[temp_key].push_back(temp);
                     temp="";
                  }
               }
               else if(c==':'){
                   if(temp!=""){
                      temp_key=temp;
                      temp="";
                   }
               }
               else{
                   temp+=c;
               }
           }
       }
       arg=data["Player_UI_Active"];
       if(arg=="True"){
          active=true;
       }
       else{
        active=false;
       }
       if( lista_pantallas.size()>0){

            pantalla_actual=lista_pantallas[0];
            opcion_actual=0;
            correct=true;


            if(opciones_maximas.find(pantalla_actual)!=opciones_maximas.end()){
                maxima_opcion=opciones_maximas[pantalla_actual];
            }
            else{
                maxima_opcion=0;
            }
       }
   }
}
Player_UI::Player_UI(bool initial_active,vector<string> pantallas_names,map<string,int>opciones_pantallas,map<string,vector<string>>opciones_objs){
  obj_capture="";
   ev_capture=-1;
   capturando_input=false;
   capturando_scrolls=false;
   esperando=false;
   in_modal=false;
   t_modal=0.0f;

   correct=true;
   active=initial_active;
   lista_pantallas=pantallas_names;
    opciones_maximas=opciones_pantallas;
    names_objs_opciones=opciones_objs;
    opcion_actual=0;
    pantalla_actual="";
    if(lista_pantallas.size()>0){
        pantalla_actual=lista_pantallas[0];
        if(opciones_maximas.find(pantalla_actual)!=opciones_maximas.end()){
            maxima_opcion=opciones_maximas[pantalla_actual];

        }
    }
}
bool Player_UI::is_correct(){
   return correct;
}
void Player_UI::set_active(bool val){
   active=val;
}
void Player_UI::capture_input(string obj_asociado,int ev_asociado){
   capturando_input=true;
   if(capturando_scrolls){
      capturando_scrolls=false;
   }
   ev_capture=ev_asociado;
   obj_capture=obj_asociado;

}
void Player_UI::update(){



  if(t_modal>0.0f && t_modal<0.5f){
      t_modal+=Timer::get_delta_time();
      if(t_modal>=0.5){
           t_modal=0.0f;
      }

  }

  if(active && esperando==false){
      if(pantalla_actual!=""){



           if(opciones_maximas.find(pantalla_actual)!=opciones_maximas.end()){


               int index=-1;
               Game_Object g=UI_object::Find(pantalla_actual,index);
               if(index!=-1){
                   if(g.is_active()){


                      string key_down=Singleton::get_key("Down");
                      string key_up=Singleton::get_key("Up");
                      string key_left=Singleton::get_key("Left");
                      string key_right=Singleton::get_key("Right");
                      string Action_key=Singleton::get_key("Action");
                      string exit_key=Singleton::get_key("Exit");

                      int last_radio=radio_actual;
                      int max_op=opciones_maximas[pantalla_actual];
                      int last_option=opcion_actual;
                      bool cancel_capture=false;

                      if(capturando_input==true && in_modal==false){

                           string next_key=Input::get_lastKey();

                           if(next_key!="" ){


                               cancel_capture=true;
                               if(next_key!=exit_key){


                                  bool repetida=Singleton::key_asignada(next_key);


                                  if(ev_capture!=-1 && repetida==false){

                                     string UI_tp="";
                                     int index_m=-1;
                                     Game_Object g_m=UI_object::Find(obj_capture,index_m);
                                     if(index_m!=-1){
                                           int tp=g_m.get_UIid();
                                           if(tp==UI_TYPES::BUTTON){
                                                UI_tp="Button";
                                           }
                                           else if(tp==UI_TYPES::LABEL){
                                              UI_tp="Label";
                                           }
                                           else if(tp==UI_TYPES::IMAGE){
                                               UI_tp="Image";
                                           }
                                     }

                                     Singleton::modific_key(obj_capture,next_key);
                                    string arg=obj_capture+","+UI_tp+",Text,"+next_key;
                                    Event_Manager::execute_event(ev_capture,arg);

                                 }
                               }
                               ev_capture=-1;
                               obj_capture="";
                           }



                      }

                      if(Input::Key_Down(key_down)==true && capturando_input==false && in_modal==false){
                          if(in_radioGroup){
                            in_radioGroup=false;
                            radios.clear();
                            radio_actual=0;
                          }
                          if(opcion_actual+1<max_op){
                              opcion_actual=opcion_actual+1;
                              if(capturando_scrolls){
                                 capturando_scrolls=false;
                                 obj_capture="";
                              }

                          }
                      }
                      else if(Input::Key_Down(key_up)==true && capturando_input==false && in_modal==false){
                           if(in_radioGroup){
                            in_radioGroup=false;
                            radios.clear();
                            radio_actual=0;

                          }
                           if(opcion_actual-1>=0){
                               opcion_actual=opcion_actual-1;
                                if(capturando_scrolls){
                                 capturando_scrolls=false;
                                 obj_capture="";
                              }
                           }
                      }
                      else if(Input::Key_Down(key_left)==true){
                            if(in_modal){
                               if(opcion_actual-1>=0){
                                  opcion_actual-=1;
                               }
                            }
                           else if(in_radioGroup){
                               if(radio_actual-1>=0){
                                     radio_actual-=1;
                                }
                           }
                      }
                      else if(Input::Key_Down(key_right)==true ){
                          if(in_modal){
                            if(opcion_actual+1<max_op){
                                opcion_actual+=1;
                            }
                          }
                          else if(in_radioGroup){
                                if(radio_actual+1<radios.size()){
                                     radio_actual+=1;
                                }
                          }
                      }


                      if(capturando_scrolls && obj_capture!="" && in_modal==false){
                          string key_left=Singleton::get_key("Left");
                          string key_rigth=Singleton::get_key("Right");
                          float val=20.0f;
                          int dir=0;
                          if(Input::Key_Down(key_left)){

                               dir=-1;
                          }
                          else if(Input::Key_Down(key_rigth)){
                               dir=1;
                          }
                          if(dir!=0){

                               ScrollBar* bar=Component::get_scrollBar(obj_capture);
                               if(bar!=NULL){

                                   bar->move_bar(dir,val,obj_capture);
                               }
                          }
                      }
                     if(names_objs_opciones.find(pantalla_actual)!=names_objs_opciones.end() && capturando_input==false){
                            vector<string> objs=names_objs_opciones[pantalla_actual];

                            if(lista_pantallas.size()>0 ){

                                if(Input::Key_Down(exit_key) && in_modal==false && Singleton::get_victory()==-1  && t_modal==0.0f){
                                      if(pantalla_actual==lista_pantallas[0] ){


                                             Event_Manager::execute_event(EVENT_TYPES::ADD_EVENT_COLA,"player01-0-");
                                             Event_Manager::execute_event(EVENT_TYPES::SHOW_UI,"Modal,");
                                            Event_Manager::execute_event(EVENT_TYPES::CHANGE_PANTALLA_UI,"player01,Modal");


                                      }
                                      else{
                                          in_radioGroup=false;
                                          radios.clear();
                                          radio_actual=0;

                                          int index_p=-1;
                                          int index_t=0;
                                          vector<string> ::iterator it_p;
                                          for(it_p=lista_pantallas.begin();it_p!=lista_pantallas.end();it_p++){
                                               string n=*it_p;
                                               if(n==pantalla_actual){
                                                 index_p=index_t;
                                               }
                                               index_t+=1;
                                          }

                                          if(index_p!=-1){

                                              if(index_p-1>=0){
                                                  Event_Manager::execute_event(EVENT_TYPES::HIDE_UI,pantalla_actual+",");
                                                  index_p=index_p-1;
                                                  Event_Manager::execute_event(EVENT_TYPES::SHOW_UI,lista_pantallas[index_p]+",");
                                                  set_pantalla(lista_pantallas[index_p]);

                                              }
                                              else{
                                                 Event_Manager::execute_event(EVENT_TYPES::EXIT_GAME,"");
                                              }
                                          }
                                      }

                                }
                            }


                            if(opcion_actual>=0 && opcion_actual<objs.size() ){
                                  string name_obj=objs[opcion_actual];
                                  int index=-1;

                                  Game_Object g=UI_object::Find(name_obj,index);
                                  int type_ui=-1;
                                  if(index!=-1){

                                     type_ui=g.get_UIid();
                                  }



                                  if(type_ui==UI_TYPES::LABEL){
                                    Label* labl=Component::get_label(name_obj);
                                    if(labl!=NULL){

                                        if(Action_key!=""){
                                            if(Input::Key_Down(Action_key)){

                                                labl->trigger_label(name_obj);
                                             }
                                             else{
                                                if(last_option!=opcion_actual){
                                                    labl->set_selected(true,name_obj);
                                               }
                                             }
                                         }
                                          else{
                                               if(last_option!=opcion_actual){
                                                     labl->set_selected(true,name_obj);
                                              }
                                         }
                                  }

                            }


                            if(last_option!=opcion_actual){
                                if(last_option>=0 && last_option<objs.size()){
                                   string name_obj2=objs[last_option];
                                   int index2=-1;
                                   Game_Object g2=UI_object::Find(name_obj2,index2);
                                   int type_ui2=-1;
                                   if(index2!=-1){

                                      type_ui2=g2.get_UIid();
                                  }


                                   if(type_ui2==UI_TYPES::LABEL){
                                       Label* lbl2=Component::get_label(name_obj2);
                                        if(lbl2!=NULL){

                                            lbl2->set_selected(false,name_obj2);
                                        }
                                  }
                                }
                            }

                    }


                 }


                 if(cancel_capture && capturando_input){
                     capturando_input=false;
                 }
                 if(in_radioGroup){
                     if(last_radio>=0 && last_radio<radios.size() && (last_radio!=radio_actual)){
                         RadioButton* r_old=Component::get_radioButton(radios[last_radio]);
                         if(r_old!=NULL){
                             r_old->set_selected(false);
                         }
                     }

                      if(radio_actual>=0 && radio_actual<radios.size()){
                        RadioButton* r=Component::get_radioButton(radios[radio_actual]);
                         if(r!=NULL){
                           r->set_selected(true);
                        }
                      }



                 }





             }
          }
      }

    }
  }

}

void Player_UI::captureRadios(vector<string> rds){
  if(rds.size()<=0){
     return;
  }

  in_radioGroup=true;
   if(radios.size()>0){
      radios.clear();
   }
   radios=rds;
   radio_actual=0;
}
void Player_UI::add_colaEv(int ev,string args){

   if(in_modal==false){
       esperando=true;
   }

   cola_events[ev]=args;


 }
void Player_UI::capture_scrolls(string obj_asociado){
   capturando_scrolls=true;
   if(capturando_input){
      capturando_input=false;
   }
   obj_capture=obj_asociado;
}
void Player_UI::set_pantalla(string name){

   vector<string>::iterator it;
   int index=0;
   if(in_modal){
       t_modal=Timer::get_delta_time();

   }
   else{
      t_modal=0.0f;
   }

   if(names_objs_opciones.find(pantalla_actual)!=names_objs_opciones.end()){
       vector<string> temp_o=names_objs_opciones[pantalla_actual];
       if(opcion_actual>=0 && opcion_actual<temp_o.size()){
            string old_obj=temp_o[opcion_actual];
            int index_o=-1;
            int type_ui_o=-1;
            Game_Object g_o=UI_object::Find(old_obj,index_o);
            if(index_o!=-1){
               type_ui_o=g_o.get_UIid();
            }

            if(type_ui_o==UI_TYPES::LABEL){

                    Label* lbl_o=Component::get_label(old_obj);
                    if(lbl_o!=NULL){
                      lbl_o->set_selected(false,old_obj);
                    }
            }
       }
   }
   for(it=lista_pantallas.begin();it<lista_pantallas.end();it++){
       string n=*it;
       if(n==name){

           string temp_n=name.substr(0,5);
           if(temp_n=="Modal"){
              in_modal=true;
              esperando=false;
              t_modal=Timer::get_delta_time();
           }
           else{
              in_modal=false;

           }
           pantalla_actual=lista_pantallas[index];
           if(opciones_maximas.find(pantalla_actual)!=opciones_maximas.end()){
               maxima_opcion=opciones_maximas[pantalla_actual];
               opcion_actual=0;
                 if(names_objs_opciones.find(pantalla_actual)!=names_objs_opciones.end()){
                     vector<string>objs=names_objs_opciones[pantalla_actual];
                     if(opcion_actual>=0 && opcion_actual<objs.size()){
                        string name_obj=objs[opcion_actual];
                        int index=-1;
                        int type_ui=-1;
                        Game_Object g=UI_object::Find(name_obj,index);
                        if(index!=-1){
                            type_ui=g.get_UIid();
                        }

                        if(type_ui==UI_TYPES::LABEL){

                              Label* lbl=Component::get_label(name_obj);
                              if(lbl!=NULL){
                                  lbl->set_selected(true,name_obj);
                              }
                        }



                     }
                 }

           }
       }
       index=index+1;
   }
}

void Player_UI:: set_pantalla(int index){

   if(in_modal){
      t_modal=Timer::get_delta_time();
      cout <<"modal"<<endl;
   }
   else{
       t_modal=0.0f;
   }
   if(names_objs_opciones.find(pantalla_actual)!=names_objs_opciones.end()){
       vector<string> temp_o=names_objs_opciones[pantalla_actual];
       if(opcion_actual>=0 && opcion_actual<temp_o.size()){
            string old_obj=temp_o[opcion_actual];
            int index_o=-1;
            int type_ui_o=-1;
            Game_Object g_o=UI_object::Find(old_obj,index_o);
            if(index_o!=-1){
               type_ui_o=g_o.get_UIid();
            }

            if(type_ui_o==UI_TYPES::LABEL){

                    Label* lbl_o=Component::get_label(old_obj);
                    if(lbl_o!=NULL){
                      lbl_o->set_selected(false,old_obj);
                    }
            }
       }

   }
   if(index>=0 && index<lista_pantallas.size()){
       pantalla_actual=lista_pantallas[index];
       string temp_n=pantalla_actual.substr(0,5);
       if(temp_n=="Modal"){
            in_modal=true;
             esperando=false;
             t_modal=Timer::get_delta_time();
        }
        else{
            in_modal=false;
        }
       if(opciones_maximas.find(pantalla_actual)!=opciones_maximas.end()){
          maxima_opcion=opciones_maximas[pantalla_actual];
          opcion_actual=0;
          if(names_objs_opciones.find(pantalla_actual)!=names_objs_opciones.end()){
                     vector<string>objs=names_objs_opciones[pantalla_actual];
                     if(opcion_actual>=0 && opcion_actual<objs.size()){
                         string name_obj=objs[opcion_actual];
                        int index=-1;
                        int type_ui=-1;
                        Game_Object g=UI_object::Find(name_obj,index);
                        if(index!=-1){
                            type_ui=g.get_UIid();
                        }

                        if(type_ui==UI_TYPES::LABEL){

                              Label* lbl=Component::get_label(name_obj);
                              if(lbl!=NULL){
                                  lbl->set_selected(true,name_obj);
                              }
                        }
                     }
                 }
        }
   }

}


void Player_UI::execute_cola(bool res){
  esperando=false;

  if(res==true){
     map<int,string>::iterator it;
     for(it=cola_events.begin();it!=cola_events.end();it++){
         int ev=it->first;
         string arg=it->second;

         Event_Manager::execute_event(ev,arg);
     }

  }
  cola_events.clear();

}
 bool Player_UI:: is_inModal(){
   if(t_modal==0.0f && in_modal){
      return true;
   }
   return false;

 }
