#include "Singleton.h"
#include "Utility.h"
#include "Game_Object.h"
#include "Component.h"
#include "Scene_data.h"
#include "Event_Manager.h"


bool Singleton::in_menuP=true;
int Singleton::ganador=-1;

int Singleton::dificultad=ENEMY_DIFICULT::EASY;
map<string,string> Singleton::data_config;
bool Singleton::use_GamePad=false;
map<string,string> Singleton::controles_keys;
map<int,int> Singleton::opciones_selected;
void Singleton::update(){

if(Scene_manager::get_active_escene().get_name()=="Inicio"){
   int index=-1;
   int index2=-1;
   Game_Object g=UI_object::Find("Pantalla2",index);
   Game_Object g2=UI_object::Find("Pantalla1",index2);
   if(index!=-1){
       if(g.is_active()){
           ScrollBar *scroll1=Component::get_scrollBar("scroll1");
           ScrollBar *scroll2=Component::get_scrollBar("scroll2");
           if(scroll1!=NULL && scroll2!=NULL){

                float sfx_vol=scroll1->get_desplazamiento();
                float music_vol=scroll2->get_desplazamiento();
                AudioManager::set_volumeMusic(music_vol);
                AudioManager::set_volumeSfx(sfx_vol);
           }
               Event_Manager::execute_event(EVENT_TYPES::SELECT_RADIO,"dificultad,"+to_string(dificultad)+",");

       }
   }
   if(index2!=-1){
      Player_UI* ply=Component::get_playerUI("player01");
      if(ply!=NULL && g2.is_active()){
           if(ply->is_inModal()){
                if(Input::Key_Down(Singleton::get_key("Exit"))){

                     Event_Manager::execute_event(EVENT_TYPES::HIDE_UI,"Modal");
                     Event_Manager::execute_event(EVENT_TYPES::RETURN_MSGBOX,"player01,False");
                      Event_Manager::execute_event(EVENT_TYPES::CHANGE_PANTALLA_UI,"player01,Pantalla1");

                }


           }
      }
   }

}

else{
    if(ganador==-1){

        Player * ply=Component::get_Player("player_bar");
        Enemy* enem=Component::get_Enemy("bar_enemy");
        if(ply!=NULL && enem!=NULL){
             int score_ply=ply->get_score();
             int score_enem=enem->get_score();


             if(score_ply>10 || score_enem>10){
                 if(abs(score_ply-score_enem)>=2){

                     if(score_ply>score_enem){
                         ganador=0;
                         AudioManager::Play_Sfx("Victory",0,0);
                     }
                  else if(score_enem>score_ply){
                       ganador=1;
                       AudioManager::Play_Sfx("Lose",0,0);
                  }

                   string victory_win=(ganador==0)?"Victory":"Lose";
                   Event_Manager::execute_event(EVENT_TYPES::SHOW_UI,victory_win);
                   Event_Manager::execute_event(EVENT_TYPES::PAUSE_GAME,"");
                   Event_Manager::execute_event(EVENT_TYPES::CHANGE_PANTALLA_UI,"player_bar,"+victory_win);

               }
            }
        }
    }



  }


}

void Singleton::modific_config(){


  if(data_config.size()<=0){
    return;
  }
  string data="'Config_Game':\n{\n";
  data+="'Scenes':[\n";
  data+="'List':'"+data_config["Scenes:List"]+"'\n";
  data+="'Format':'"+data_config["Scenes:Format"]+"'\n";
  data+="]\n";

  data+="'Windows':[\n";
  data+="'Name':'"+data_config["Windows:Name"]+"'\n";
  data+="'Size':'"+data_config["Windows:Size"]+"'\n";
  data+="'Resizable':'"+data_config["Windows:Resizable"]+"'\n";
  data+="'FullScreen':'"+data_config["Windows:FullScreen"]+"'\n";
  data+="'Icon':'"+data_config["Windows:Icon"]+"'\n";
  data+="]\n";

  data+="'Logo':[\n";
  data+="'Src':'"+data_config["Logo:Src"]+"'\n";
  data+="'Time':'"+data_config["Logo:Time"]+"'\n";
  data+="'Sfx':'"+data_config["Logo:Sfx"]+"'\n";
  data+="]\n";

  data+="'Controls':[\n";
  map<string,string>::iterator it;
  for(it=controles_keys.begin();it!=controles_keys.end();it++){
      string key=it->first;
      string val=it->second;
      data+="'"+key+"':'"+val+"'\n";
  }
  data+="]\n";

  data+="'Volume':[\n";
  data+="'BGM':'"+to_string(AudioManager::get_volumeBgm())+"'\n";
  data+="'SFX':'"+to_string(AudioManager::get_volumeSfx())+"'\n";
  data+="]\n";
  data+="'Dificultad':[\n";
  data+="'Mode':'"+to_string(dificultad)+"'\n";
  data+="]\n";
  data+="}";

  string src_dir=string(SDL_GetBasePath())+"/assets/game_data/config.json";
  ofstream stream(src_dir.c_str(),ios::out);
  stream<<data<<endl;
  stream.close();

}
void Singleton::get_config_inf(map<string,string> val){
     data_config=val;
     vector<string> keys={"Up","Down","Action","Exit","Left","Right"};
     vector<string>::iterator it;
     for(it=keys.begin();it!=keys.end();it++){
          string temp_key=*it;
          string id_key="Controls:"+temp_key;
          if(data_config.find(id_key)!=data_config.end()){
              controles_keys[temp_key]=data_config[id_key];
          }


     }
     float bgm=0.0f;
     float sfx=0.0f;
     if(data_config.find("Volume:BGM")!=data_config.end()){

        string temp_bgm=data_config["Volume:BGM"];
        bgm=Math_G::StrToFloat(temp_bgm);

    }
    if(data_config.find("Volume:SFX")!=data_config.end()){

        string temp_sfx=data_config["Volume:SFX"];
        sfx=Math_G::StrToFloat(temp_sfx);

    }

    AudioManager::set_volumeMusic(bgm);
    AudioManager::set_volumeSfx(sfx);

    if(data_config.find("Dificultad:Mode")!=data_config.end()){
         string temp_dif=data_config["Dificultad:Mode"];
         if(temp_dif!=""){
              dificultad=Math_G::StrToInt(temp_dif);
         }

    }
}
void Singleton::modific_key(string key,string val){
      if(controles_keys.find(key)!=controles_keys.end()){
           controles_keys[key]=val;
      }
}
bool Singleton:: key_asignada(string val){

 bool res=false;
  map<string,string>::iterator it;
  for(it=controles_keys.begin();it!=controles_keys.end();it++){
      string v_key=it->second;
      if(val==v_key){
          res=true;
          break;
      }
  }
  return res;
}
string Singleton :: get_key(string accion){
     if(controles_keys.find(accion)!=controles_keys.end()){
         return controles_keys[accion];
     }
     return "";
}
void Singleton::set_dificultad(int val){

   dificultad=val;

}
void Singleton::On_load(){


   ganador=-1;
   if(Scene_manager::get_active_escene().get_name()=="Inicio"){

       float bgm=AudioManager::get_volumeBgm();
       float sfx=AudioManager::get_volumeSfx();
       int index1=-1;
       int index2=-1;
       Game_Object g1=UI_object::Find("scroll1",index1);
       Game_Object g2=UI_object::Find("scroll2",index2);




       if(index1==-1 || index2==-1){
           return;
      }

       ScrollBar* scroll1=Component::get_scrollBar("scroll1");
       ScrollBar* scroll2=Component::get_scrollBar("scroll2");
       if(scroll1!=NULL && scroll2!=NULL){
          scroll1->set_desplazamiento(sfx,&g1);
          scroll2->set_desplazamiento(bgm,&g2);
       }
       Player_UI* ply=Component::get_playerUI("player01");
       if(ply!=NULL){
           ply->set_pantalla(0);
       }


       map<string,string>::iterator it;

       for(it=controles_keys.begin();it!=controles_keys.end();it++){
           string name_obj=it->first;
           Label* lbl=Component::get_label(name_obj);
           if(lbl!=NULL){
              lbl->set_text(it->second);
           }
       }
   }
   else{



       Enemy * enem=Component::get_Enemy("bar_enemy");
       if(enem!=NULL){
          enem->set_dificultad(dificultad);
       }
   }
}
void Singleton::Init(float sfx,float bgm){


    sfx=Math_G::Clamp(sfx,0.0,1.0);
    bgm=Math_G::Clamp(bgm,0.0,1.0);
    AudioManager::set_volumeMusic(bgm);
    AudioManager::set_volumeSfx(sfx);
    int index1=-1;
    int index2=-1;
    Game_Object g1=UI_object::Find("scroll1",index1);
    Game_Object g2=UI_object::Find("scroll2",index2);

    if(index1==-1 || index2==-1){
        return;
    }
    ScrollBar* scroll1=Component::get_scrollBar("scroll1");
    ScrollBar* scroll2=Component::get_scrollBar("scroll2");
    if(scroll1!=NULL && scroll2!=NULL){
          scroll1->set_desplazamiento(sfx,&g1);
          scroll2->set_desplazamiento(bgm,&g2);
    }

}
int Singleton::get_victory(){
   return ganador;
}
