#include "Enemy.h"
#include "Component.h"

Enemy::Enemy()
{
    //ctor
}

Enemy::~Enemy()
{
    //dtor
}
 Enemy::Enemy(int tp,Vector2 vl,float slp,int dific,string targ,float d_targ){
     type_enem=tp;
     base_vel=vl;
     sleep_time=0.0f;
     max_sleep=slp;
     dificult=dific;
     target=targ;
     min_distTarget=d_targ;
     score=0;

 }
Enemy:: Enemy(map<string,string> datos){

     hits_player=0;
     hits=0;
     score=0;
    string arg=datos["Enemy_type"];
    type_enem=ENEMY_TYPES::PIN_PONG_RIVAL;

    if(arg!=""){

        if(arg=="pin-pong-rival"){
            type_enem=ENEMY_TYPES::PIN_PONG_RIVAL;
        }
    }

    arg=datos["Enemy_Sleep"];
    sleep_time=0.0f;
    max_sleep=-1.0f;
    if(arg!=""){
        max_sleep=Math_G::StrToFloat(arg);
    }
    dificult=0;
    arg=datos["Enemy_Dificult"];
    if(arg!=""){
          dificult=Math_G::StrToInt(arg);
    }
    arg=datos["Enemy_Vel"];
    if(arg!=""){
             string temp_val="";
             bool x_comp=false;
             for(int i_dr=0;i_dr<arg.length();i_dr++){
                      unsigned char c=arg[i_dr];
                       if(c==','){
                          if(temp_val!=""){
                                  float val=Math_G::StrToFloat(temp_val);
                                  base_vel.x=val;
                                  x_comp=true;
                                  temp_val="";

                            }
                        }
                       else{
                            if(c!=' '){
                                  temp_val+=c;
                                 }
                            }
                        }

            if(temp_val!="" && x_comp==true){
                     float val=Math_G::StrToFloat(temp_val);
                       base_vel.y=val;
                       temp_val="";

             }
    }

     target=datos["Enemy_Target"];
     min_distTarget=0.0f;
     if(target!=""){
          arg=datos["Enemy_MinDistTarget"];
          if(arg!=""){
              min_distTarget=Math_G::StrToFloat(arg);
          }
     }

 }
void Enemy:: update(Game_Object* g){
   if(type_enem==ENEMY_TYPES::PIN_PONG_RIVAL){
       IA_PONG(g);

   }
}
void Enemy::add_hitPlayer(){
    hits_player=hits_player+1;

}

void Enemy::IA_PONG(Game_Object* g){

   if(target==""){
       return;
   }
   Label* lbl=Component::get_label("score_enemy");
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
   Transform* t=Component::get_Transform(target);
   Transform* t_g=Component::get_Transform(g->get_name());
   Collider* col1=Component::get_Collider(target);
   Collider* col2=Component::get_Collider(g->get_name());
  RigidBody* rb=Component::get_rigidBody(g->get_name());


   if(t!=NULL && t_g!=NULL && col1!=NULL && col2!=NULL && rb!=NULL){
      if(sleep_time==0.0f){

          Vector2 pos_target=t->Get_Position()+col1->get_figure().get_center();
          Vector2 pos_object=t_g->Get_Position()+col2->get_figure().get_center();

          Vector2 dif=(pos_target-pos_object);

          if(abs(dif.y)>min_distTarget){
              dif.x=0;
              dif=dif.normalize();
              Vector2 next_vel=base_vel;

              float reducc=0.0f;
              float dificult_reducc=0.0f;
              float reducc_hits=0.0f;
              float reducc_play=0.0f;


              if(dificult==ENEMY_DIFICULT::EASY){
                    dificult_reducc=0.34f;
                     if(hits>0){
                          reducc_hits=((float)hits)*0.15f;
                          if(reducc_hits>0.20f){
                             reducc_hits=0.20f;
                          }
                    }
                     if(hits_player>0){
                          reducc_play=((float)hits_player)*0.18f;
                          if(reducc_play>0.25f){
                             reducc_play=0.25f;
                          }
                     }

              }
              else if(dificult==ENEMY_DIFICULT::NORMAL){
                      dificult_reducc=0.25f;
                     if(hits>0){
                          reducc_hits=((float)hits)*0.07f;
                          if(reducc_hits>0.2f){
                             reducc_hits=0.2f;
                          }
                    }
                     if(hits_player>0){
                          reducc_play=((float)hits_player)*0.8f;
                          if(reducc_play>0.25f){
                             reducc_play=0.25f;
                          }
                     }
              }
              else if(dificult==ENEMY_DIFICULT::HARD){
                    dificult_reducc=0.27f;
                  if(hits>0){
                          reducc_hits=((float)hits)*0.04f;
                          if(reducc_hits>0.2f){
                             reducc_hits=0.2f;
                          }
                    }
                     if(hits_player>0){
                          reducc_play=((float)hits_player)*0.04f;
                          if(reducc_play>0.2f){
                             reducc_play=0.2f;
                          }
                     }
              }
              reducc=reducc_hits+reducc_play+dificult_reducc;


              next_vel.y=next_vel.y-(next_vel.y*reducc);

              dif.y=dif.y*next_vel.y*Timer::get_delta_time();
              sleep_time=Timer::get_delta_time();
              rb->set_force(dif);
          }


      }
      else{
          rb->set_force(Vector2(0,0));
      }

   }
   if(sleep_time>0){
       sleep_time=sleep_time+Timer::get_delta_time();
       if(sleep_time>=max_sleep){
           sleep_time=0.0f;
       }
   }
}
int Enemy::get_score(){
   return score;
}
void Enemy::set_dificultad(int dif){
    dificult=dif;

}
void Enemy::OnTrigger(Game_Object other,Game_Object* g,ManiFold* contact_info){

}

void Enemy:: OnCollision(Game_Object other,Game_Object* g, ManiFold* contact_info){

    if(other.get_name()=="ball"){
          hits=hits+1;

    }
}
void Enemy::reset(){
      hits=0;
      hits_player=0;
}
void Enemy::add_score(int val){
    score=score+val;
}
