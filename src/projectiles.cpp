#include "projectiles.h"
#include "Component.h"

Proyectil::Proyectil(){

base_force=156.0f;
type_p=0;
max_life_time=-1.0f;
desactive_ondie=false;
reset_proy=false;

}
Proyectil::Proyectil(float f,int t,float max_life,Vector2 dr,bool desactive,vector<string> sfxs_asociados){
   base_force=f;
   type_p=t;
   life_time=0.0f;
   initial_dir=dr;
   max_life_time=max_life;
  desactive_ondie=desactive;
  sfx=sfxs_asociados;
  reset_proy=false;
}
Proyectil::Proyectil(map<string,string> args)
{
    reset_proy=false;
    base_force=156.0f;
    type_p=PROYECTIL_TYPES::BALL;
    max_life_time=-1.0f;
    life_time=0.0f;
    desactive_ondie=false;
   initial_dir=Vector2(1,1);

   string arg=args["Proyectil_Velocity"];
   if(arg!=""){
       base_force=Math_G::StrToFloat(arg);
   }
   arg=args["Proyectil_Type"];
   if(arg!=""){

        if(arg=="Ball"){
            type_p=PROYECTIL_TYPES::BALL;
        }
        else if(arg=="Shoot"){
              type_p=PROYECTIL_TYPES::SHOOT;
         }
         else if(arg=="pin-pong"){
             type_p=PROYECTIL_TYPES::PIN_PONG_BALL;
         }
  }
   arg=args["Proyectil_Desactive"];
   if(arg=="False"){
         desactive_ondie=false;
  }
   arg=args["Proyectil_LifeTime"];
   if(arg!=""){
       max_life_time=Math_G::StrToFloat(arg);
   }
   arg=args["Proyectil_Dir"];
   if(arg!=""){
             string temp_val="";
             bool x_comp=false;
             for(int i_dr=0;i_dr<arg.length();i_dr++){
                      unsigned char c=arg[i_dr];
                       if(c==','){
                          if(temp_val!=""){
                                  float val=Math_G::StrToFloat(temp_val);
                                  initial_dir.x=val;
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
                       initial_dir.y=val;
                       temp_val="";

             }
    }

  int count_sfx=0;
  arg=args["Proyectil_SFX_Count"];
  if(arg!=""){
       count_sfx=Math_G::StrToFloat(args["Proyectil_SFX_Count"]);
       for(int i=0;i<count_sfx;i++){
            string sfx_id="Proyectil_SFX_"+to_string(i+1);
            arg=args[sfx_id];
            if(arg!=""){
                  string source=arg;
                  sfx.push_back(source);
            }
       }
  }


}
 Vector2 Proyectil:: update_ping_pong(Vector2 forc){
        Vector2 f=forc.normalize();
        Vector2 f2=forc;
        Vector2 dir=Vector2(0,0);
          if(f.x==0 && f.y==0){

              int next_dir=0;
              string rand_n=to_string(Math_G::get_random(100));
              char last=rand_n[rand_n.length()-1];
              string temp_n=""+string(1,last);
              int last_n=Math_G::StrToInt(temp_n);

              if(last_n<5){
                   next_dir=1;
              }
              else{
                  next_dir=-1;
              }
              dir.x=1;
              dir.y=next_dir;
          }
          else if(f.x==0 && f.y!=0){

              dir.x=1;
              dir.y=f.y;
          }
           else if(f.x!=0 && f.y==0){

              dir.x=f.x;
              dir.y=1;
           }


        if(dir.magnitud()==0){
             dir.y=(f2.y>=0)?1:-1;
             dir.x=(f2.x>=0)?1:-1;
        }



         if(Timer::get_delta_time()<1.0f){
                 dir.x=dir.x*base_force*Timer::get_delta_time();
                 dir.y=dir.y*base_force*Timer::get_delta_time();

        }

           return dir;
 }

void Proyectil::restart(Game_Object* g)
{

   reset_proy=false;
   life_time=0.0f;
   g->reset();



}
void Proyectil::update(Game_Object* g){


  if(g==NULL){
       return;
  }

    if(reset_proy){
        restart(g);
    }
   RigidBody*rb=Component::get_rigidBody(g->get_name());
   if(rb!=NULL){

        if(type_p==PROYECTIL_TYPES::PIN_PONG_BALL){
            Vector2 rb_f=rb->get_last_force();
            Vector2 f=update_ping_pong(rb_f);
            if(f.magnitud()!=0.0f){
               rb->set_force(f);
           }
        }

   }
   if(max_life_time>0.0f){

       life_time=Timer::get_delta_time();
       if(life_time>=max_life_time){




             if(desactive_ondie){
                 g->set_active(false);
             }
             else{
                  int index=-1;
                 Game_Object::Find(g->get_name(),index);
                 if(index!=-1){
                        Game_Object::destroy_obj(*g,index);
                 }
             }



       }
   }

}
void Proyectil::OnTrigger(Game_Object other,Game_Object* g,ManiFold* contact_info){
      if(other.get_tag()=="zona_player"){

         Enemy* enem=Component::get_Enemy("bar_enemy");
          if(enem!=NULL){
              enem->add_score(1);
              enem->reset();
          }

          AudioManager::Play_Sfx(sfx[1],0,0);
          reset_proy=true;
      }
      else if(other.get_tag()=="zona_enemy"){
          Player* py=Component::get_Player("player_bar");
          if(py!=NULL){
              py->add_score(1);
              Enemy* enem=Component::get_Enemy("bar_enemy");
              if(enem!=NULL){
                  enem->reset();
              }
          }
          AudioManager::Play_Sfx(sfx[1],0,0);
          reset_proy=true;
      }
}
void Proyectil:: OnCollision(Game_Object other,Game_Object* g, ManiFold* contact_info){
       AudioManager::Play_Sfx(sfx[0],0,0);
}
