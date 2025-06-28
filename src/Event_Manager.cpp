#include "Event_Manager.h"
#include "Player.h"
#include "Component.h"
#include "Singleton.h"

void Event_Manager::execute_event(int ev,string args){



      switch(ev){
        case EVENT_TYPES::EXIT_GAME:
           {

              exit_game();
              break;
           }
        case EVENT_TYPES::SET_CURSOR:
            {

                if(args!="" && args.length()==1){


                    int index=Math_G::StrToInt(args);
                    set_cursor(index);
                }
                break;
            }
         case EVENT_TYPES::HIDE_OBJECT:{
               if(args!=""){
                  show_Object(false,args,false);
               }
               break;
         }
         case EVENT_TYPES::SHOW_OBJECT:{
              if(args!=""){
                  show_Object(true,args,false);
               }
                break;
         }
         case EVENT_TYPES::HIDE_UI:{
               if(args!=""){
                  show_Object(false,args,true);
               }
                break;
         }
         case EVENT_TYPES::SHOW_UI:{
              if(args!=""){
                  show_Object(true,args,true);
               }
                break;
         }
         case EVENT_TYPES:: LOAD_SCENE:{
            Scene_manager::load_scene(args);
            break;
         }
         case EVENT_TYPES::CHANGE_PANTALLA_UI:{
            change_Pantalla_UI(args);
           break;
         }
         case EVENT_TYPES::GET_TEXT_INPUT:{
            input_textUI(args);
           break;
         }
         case EVENT_TYPES::CHANGE_TEXT_UI:{
            modific_UI(args);
           break;
         }
         case EVENT_TYPES::GET_SCROLL_INPUT:{


            Input_ScrollUI(args);
            break;

         }
         case EVENT_TYPES::RETURN_MSGBOX:{

              get_resDialog( args);
              break;
         }
         case EVENT_TYPES::ADD_EVENT_COLA:{
            add_EventCola(args);
            break;
          }
          case EVENT_TYPES::PAUSE_GAME:{
             Game_Object::pause_game(true);
             break;
          }
          case EVENT_TYPES::UNPAUSE_GAME:{
             Game_Object::pause_game(false);
             break;
          }
          case EVENT_TYPES::SET_DIFICULTAD:{

              if(args!=""){
                  int val=Math_G::StrToInt(args);
                   Singleton::set_dificultad(val);
              }

           }
           case EVENT_TYPES::SELECT_RADIO:{
                select_radioGroup(args);
                break;
            }
            case EVENT_TYPES::GET_RADIO_INPUT:{
                radio_input(args);
            }

        default:
           {
               break;
           }

     }
}
void Event_Manager::radio_input(string args){
     if(args==""){
          return;
       }

     string playerId="";
     vector<string> radios;
     int index=0;
     string temp="";
     for(int i=0;i<args.length();i++){
          unsigned char c=args[i];
          if(c==','){
            if(index==0){
                playerId=temp;
            }

            else {
                  radios.push_back(temp);
            }

            temp="";
            index+=1;
          }
          else{
            temp+=c;
          }
     }
     if(temp!="" ){
        radios.push_back(temp);
     }

     if(radios.size()>0 && playerId!=""){
          Player_UI* ply=Component::get_playerUI(playerId);
          if(ply!=NULL){
              ply->captureRadios(radios);
          }
     }

}
 void Event_Manager::select_radioGroup(string args){
      if(args==""){
        return;
     }

     string groupId="";
     int index=0;
     string val="";
     string temp="";
     for(int i=0;i<args.length();i++){
          unsigned char c=args[i];
          if(c==','){
            if(index==0){
                groupId=temp;
            }

            else if(index==1){
                  val=temp;
            }

            temp="";
            index+=1;
          }
          else{
            temp+=c;
          }
     }
     if(temp!="" ){
        val=temp;
     }


   vector<Game_Object> lista=UI_object::get_UI();
   map<int,Game_Object> grupo=UI_object::get_radioGroup(groupId);
   map<int,Game_Object>::iterator it;
   for(it=grupo.begin();it!=grupo.end();it++){
       int index_g=it->first;
       if(index_g>=0 && index_g<lista.size()){
         Game_Object g=lista[index_g];
         string nam=g.get_name();
         RadioButton *r=Component::get_radioButton(nam);
         if(r!=NULL){
              string val_r=r->get_selectedValue();
              if(val_r!=val){
                  r->set_selected(false);
              }
              else{
                r->set_selected(true);
              }
         }
       }

   }
 }
 void Event_Manager::add_EventCola(string args){
   if(args==""){
        return;
     }

     string player_obj="";
     int ev_id=-1;
     string args_ev="";
     int index=0;
     string temp="";
     for(int i=0;i<args.length();i++){
          unsigned char c=args[i];
          if(c=='-'){
            if(index==0){
                player_obj=temp;
            }
            else if(index==1){
                if(temp!=""){
                    ev_id=Math_G::StrToInt(temp);
                }
            }
            else if(index==2){
                  args_ev=temp;
            }

            temp="";
            index+=1;
          }
          else{
            temp+=c;
          }
     }
     if(temp!="" ){
        args_ev=temp;
     }
     if(player_obj!="" ){
          Player_UI * ply=Component::get_playerUI(player_obj);
          if(ply!=NULL){

               ply->add_colaEv(ev_id,args_ev);
          }
     }

 }
 void Event_Manager::get_resDialog(string args){
     if(args==""){
        return;
     }

     string player_obj="";
     bool resultado=false;
     int index=0;
     string temp="";
     for(int i=0;i<args.length();i++){
          unsigned char c=args[i];
          if(c==','){
            if(index==0){
                player_obj=temp;
            }
            else if(index==1){
                if(temp=="False"){
                    resultado=false;
                }
                else{
                    resultado=true;
                }
            }

            temp="";
            index+=1;
          }
          else{
            temp+=c;
          }
     }
     if(temp!="" ){
         if(temp=="False"){
            resultado=false;
            }
        else{
            resultado=true;
        }
     }
     if(player_obj!="" ){
          Player_UI * ply=Component::get_playerUI(player_obj);
          if(ply!=NULL){

               ply->execute_cola(resultado);
          }
     }
 }
void Event_Manager::Input_ScrollUI(string args){
     if(args==""){
        return;
     }
     string player_obj="";
     string obj_asociado="";
     int index=0;
     string temp="";
     for(int i=0;i<args.length();i++){
          unsigned char c=args[i];
          if(c==','){
            if(index==0){
                player_obj=temp;
            }
            else if(index==1){
                obj_asociado=temp;
            }

            temp="";
            index+=1;
          }
          else{
            temp+=c;
          }
     }
     if(temp!="" && obj_asociado==""){
         obj_asociado=temp;
          temp="";
     }
     if(player_obj!="" && obj_asociado!=""){
          Player_UI * ply=Component::get_playerUI(player_obj);
          if(ply!=NULL){

               ply->capture_scrolls(obj_asociado);
          }
     }
}

 void Event_Manager::input_textUI(string arg){

     if(arg==""){
        return;
     }
     string player_obj="";
     string obj_asociado="";
     string ev_asociado="";
     int index=0;
     string temp="";
     for(int i=0;i<arg.length();i++){
          unsigned char c=arg[i];
          if(c==','){
            if(index==0){
                player_obj=temp;
            }
            else if(index==1){
                obj_asociado=temp;
            }
            else if(index==2){
                 ev_asociado=temp;
            }
            temp="";
            index+=1;
          }
          else{
            temp+=c;
          }
     }
     if(temp!="" && ev_asociado==""){
          ev_asociado=temp;
          temp="";
     }
     if(player_obj!="" && obj_asociado!="" && ev_asociado!=""){
          Player_UI * ply=Component::get_playerUI(player_obj);
          if(ply!=NULL){
               int ev=Math_G::StrToInt(ev_asociado);
               ply->capture_input(obj_asociado,ev);
          }
     }

 }
 void Event_Manager::modific_UI(string arg){
     if(arg==""){
        return;
     }
     string UI_name="";
     string UI_type="";
     string modific_type="";
     string valor_modific="";

     int index=0;
     string temp="";
     for(int i=0;i<arg.length();i++){
          unsigned char c=arg[i];
          if(c==','){
            if(index==0){
               UI_name=temp;
            }
            else if(index==1){
               UI_type=temp;
            }
            else if(index==2){
                 modific_type=temp;
            }
            else if(index==3){
                  valor_modific=temp;
            }
            temp="";
            index+=1;
          }
          else{
            temp+=c;
          }
     }
     if(temp!="" && valor_modific==""){
          valor_modific=temp;
     }
     if(UI_name!="" && UI_type!="" && modific_type!="" && valor_modific!=""){
           if(UI_type=="Button"){
               Button* btn=Component::get_Button(UI_name);
               if(btn!=NULL){

               }
           }
           else if(UI_type=="Label"){
               Label* lbl=Component::get_label(UI_name);
               if(lbl!=NULL){
                   if(modific_type=="Text"){
                      lbl->set_text(valor_modific);
                   }
               }
           }
           else if(UI_type=="Image"){
               Image_UI * img=Component::get_ImageUI(UI_name);
               if(img!=NULL){

               }
           }
     }

 }

void Event_Manager:: change_Pantalla_UI(string arg){

   string name_ply="";
   string name_pantalla="";
   string temp="";
   for(int i=0;i<arg.length();i++){
       unsigned char c=arg[i];
       if(c==','){
           if(name_ply==""){
               name_ply=temp;
           }
           else{
              name_pantalla=temp;
           }
           temp="";
       }
       else{
           temp+=c;
       }
   }
   if(temp!=""){
       name_pantalla=temp;
       temp="";
   }
   if(name_ply!="" && name_pantalla!=""){
       Player_UI* ply=Component::get_playerUI(name_ply);
       if(ply!=NULL){
          ply->set_pantalla(name_pantalla);
       }
   }
}
void Event_Manager::liberar(){
    for(int i=0;i<cursores.size();i++){

        if(cursores.at(i)!=NULL){
           SDL_FreeCursor(cursores.at(i));
           cursores.at(i)=NULL;
        }
    }


}
void Event_Manager::exit_game(){
   playing_game=false;
}
bool Event_Manager::play_game(){
    return playing_game;
}
void Event_Manager::show_Object(bool val,string arg,bool is_ui){



      vector<string> afectados;
      string temp="";
      for(unsigned int i=0;i<arg.length();i++){
           unsigned char c=arg[i];
           if(c==','){
               afectados.push_back(temp);
               temp="";
           }
           else{
               temp+=c;
           }
      }
      if(temp!="" && temp!=","){
          afectados.push_back(temp);
          temp="";
      }

      vector<string>::iterator it;
      for(it=afectados.begin();it!=afectados.end();it++){
           int index=-1;
           string name=*it;

           if(is_ui==false){
             Game_Object g=Game_Object::Find(name,index);
             if(index!=-1){
                g.set_active(val);
                Game_Object::edit_obj(g,index);
             }

           }
           else{

             Game_Object g=UI_object::Find(name,index);


             if(index!=-1){
                 g.set_active(val);
                 UI_object::EditUI(g,index);
             }

           }

      }
}
void Event_Manager::set_cursores(vector<string> src){
    Vector2 curs_pos=Input::get_Mouse_pos();
      if(src.size()>=3){

        vector<string> ::iterator it;
        for(it=src.begin();it!=src.end();it++){
            string src_cursor=*it;
            if(src_cursor!=""){
                SDL_Surface *surface = IMG_Load(src_cursor.c_str());
                SDL_Cursor *cursor = SDL_CreateColorCursor(surface,curs_pos.x,curs_pos.y);
                cursores.push_back(cursor);
                SDL_FreeSurface(surface);
            }
        }
      }
      else if(src.size()==2){
        string src_cursor_arrow=src.at(0);
        if(src_cursor_arrow!=""){
                SDL_Surface *surface = IMG_Load(src_cursor_arrow.c_str());
                SDL_Cursor *cursor = SDL_CreateColorCursor(surface,curs_pos.x,curs_pos.y);
                cursores.push_back(cursor);
                SDL_FreeSurface(surface);
            }
        string src_cursor_hand=src.at(0);
        if(src_cursor_hand!=""){
                SDL_Surface *surface = IMG_Load(src_cursor_hand.c_str());
                SDL_Cursor *cursor2 = SDL_CreateColorCursor(surface,curs_pos.x,curs_pos.y);
                cursores.push_back(cursor2);
                SDL_FreeSurface(surface);
            }
        SDL_Cursor* cursor3;
        cursor3= SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
        cursores.push_back(cursor3);
      }
      else if(src.size()==1){
        string src_cursor_arrow=src.at(0);
        if(src_cursor_arrow!=""){
                SDL_Surface *surface = IMG_Load(src_cursor_arrow.c_str());
                SDL_Cursor *cursor = SDL_CreateColorCursor(surface,curs_pos.x,curs_pos.y);
                cursores.push_back(cursor);
                SDL_FreeSurface(surface);
        }
        SDL_Cursor* cursor2;
        cursor2 = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        SDL_Cursor* cursor3;
        cursor3 = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);


        cursores.push_back(cursor2);
        cursores.push_back(cursor3);
      }
      else{

        SDL_Cursor* cursor1;
        cursor1 = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
        SDL_Cursor* cursor2;
        cursor2 = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        SDL_Cursor* cursor3;
        cursor3 = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);

        cursores.push_back(cursor1);
        cursores.push_back(cursor2);
        cursores.push_back(cursor3);

      }
     SDL_SetCursor(cursores.at(0));
}
void Event_Manager::show_cursor(bool val){
     if(val==true){
         SDL_ShowCursor(SDL_ENABLE);
     }
     else{
          SDL_ShowCursor(SDL_DISABLE);
     }
}
void Event_Manager::set_cursor(int index){

     if(index==CURSOR_INDEX::ARROW || index==CURSOR_INDEX::HAND || index==CURSOR_INDEX::IBEAM){
           SDL_SetCursor(cursores.at(index));
           last_cursor=index;
     }
}
bool Event_Manager::playing_game=true;
vector<SDL_Cursor *> Event_Manager::cursores;
int Event_Manager::last_cursor=0;
int Event_Manager::get_last_cursor(){
   return last_cursor;
}
