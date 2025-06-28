#include "Utility.h"


void Timer::update(){
   if(tick_perFrame==0){
       if(target_fps>0){
          tick_perFrame=(1000.0f/(double)target_fps);
          fixed_deltaTime=1.0f/target_fps;
       }
   }
   if(first==0){

    first=SDL_GetTicks();
   }
   last = Now;
   Now = SDL_GetTicks();

   delta = (double)((Now - last) / 1000.0f );
   double temp_dif=(SDL_GetTicks()-first)/1000.0f;
   if(temp_dif>0.00f){
      float avgFPS = counted_frames /((SDL_GetTicks()-first)/1000.0f) ;
      if( avgFPS > 2000000 )
      {
       avgFPS = 0;
      }
   }
   if(counted_frames>=10){
      counted_frames=0;
      first=SDL_GetTicks();

   }
   double elapsed_ms=Now- last;
   if(elapsed_ms<tick_perFrame){


         SDL_Delay(tick_perFrame-elapsed_ms);
    }

  counted_frames++;

}

double Timer::get_delta_time(){
   if(delta<1){
       return delta;
   }
   else{
       return 0.0;
   }
}

double Timer::get_fixed_deltaTime(){
  if(fixed_deltaTime<1){
       return fixed_deltaTime;
   }
   else{
       return 0.0;
   }

}
double Timer::get_ticks_frame(){
  if(tick_perFrame!=0){
    return tick_perFrame;
  }
  else{
      return 0;
  }
}
double Timer::first=0;
int Timer::counted_frames=0;
double Timer::delta=0.0;
unsigned int Timer::Now=0;
unsigned int Timer::last=0;
double Timer::fixed_deltaTime=0.0;
int Timer::target_fps=60;
double Timer::tick_perFrame=0;


std::map<std::string,int>  Input::keyboard_states;
std::map<int,int>  Input:: Gamepads_states;
std::map<int,int>  Input:: mouse_states;
std::map<std::string,float> Input::keyboard_timers;
std::map<int,float> Input:: Gamepads_timers;
std::map<int,float>  Input::mouse_timers;
std::map<std::string,float> Input::keyboard_Maxtimers;
std::map<int,float> Input:: Gamepads_Maxtimers;
std::map<int,float>  Input::mouse_Maxtimers;
float Input:: max_espera=0.2f;
int Input::steps_espera=1;
int Input::count_keys=0;
Vector2 Input::last_mouse_pos=Vector2(0,0);
std::vector<std::string> Input::keys_list;

void Input::init(){

     for(int i=0;i<3;i++){
        mouse_states[i]=-1;
        mouse_timers[i]=0.0f;
        mouse_Maxtimers[i]=max_espera;
     }

   std::string chars="A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,";
   chars=chars+"1,2,3,4,5,6,7,8,9,0,Up,Left,Down,Right,";
   chars=chars+"Keypad 1,Keypad 2,Keypad 3,Keypad 4,Keypad 5,Keypad 6,Keypad 7,Keypad 8,Keypad 9,Keypad 0,";
   chars=chars+"Return,Escape,Space,Backspace,Left Shift,Right Shift,Left Alt,Right Alt,Left Ctrl,Right Ctrl,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,";

    std::string temp="";

    for(int j=0;j<chars.length();j++){
        unsigned char c=chars[j];
        if(c==','){

           keyboard_states[temp]=-1;
           keyboard_timers[temp]=0.0;
           keyboard_timers[temp]=max_espera;
           keys_list.push_back(temp);

           temp="";
           count_keys+=1;

        }
        else{
            temp+=c;
        }
    }

     for(int k=0;k<10;k++){
       Gamepads_states[k]=-1;
       Gamepads_timers[k]=0.0f;
       Gamepads_Maxtimers[k]=max_espera;

     }

}
void Input::update(){

    float delta=Timer::get_delta_time();
    if(delta!=0.0f){
       max_espera=steps_espera*delta;
    }
    for(int i=0;i<3;i++){

        if(mouse_timers[i]>0.0f){
            mouse_timers[i]=mouse_timers[i]+Timer::get_delta_time();

            if(mouse_timers[i]>=mouse_Maxtimers[i]){
                    mouse_timers[i]=0.0f;


                    if(mouse_states[i]==0){
                        mouse_states[i]=1;

                    }
                    else if(mouse_states[i]==2){
                        mouse_states[i]=-1;

                    }

            }
        }


     }
      std::map<std::string,float>::iterator it;

      for(it=keyboard_timers.begin();it!=keyboard_timers.end();it++){
          std::string c=it->first;
          double val=it->second;

          if(val>0.0){
              keyboard_timers[c]=val+Timer::get_delta_time();
              if(keyboard_timers[c]>=keyboard_Maxtimers[c]){
                  if(keyboard_states[c]==0){
                    keyboard_states[c]=1;
                    if(last_keyInput==c){
                        last_keyInput="";
                    }

                }
                  else if(keyboard_states[c]==2){
                    keyboard_states[c]=-1;
                    if(last_keyInput==c){
                        last_keyInput="";
                    }
                }
                  keyboard_timers[c]=0.0;

              }
          }
     }
     for(int k=0;k<10;k++){


     }

}
void Input::get_event(SDL_Event  ev){


   switch(ev.type){
      case SDL_KEYDOWN:{


          const char* c=SDL_GetKeyName(ev.key.keysym.sym);

          std::string cad="";
          if(c!=NULL){

            cad=c;
            last_keyInput=cad;


             if(keyboard_states.find(cad)!=keyboard_states.end()){
                if(keyboard_states[cad]==-1 ){


                     keyboard_states[cad]=0;
                    keyboard_timers[cad]=Timer::get_delta_time();
                    keyboard_Maxtimers[cad]=max_espera;



                }
            }

          }



      }
      case SDL_KEYUP:{

          if(ev.key.state==SDL_RELEASED ){
                const char* c=SDL_GetKeyName(ev.key.keysym.sym);
                std::string cad="";
                if(c!=NULL){
                    cad=c;
                  }
                  if(cad!=""){
                    if(keyboard_states.find(cad)!=keyboard_states.end()){
                        keyboard_states[cad]=2;
                         keyboard_timers[cad]=Timer::get_delta_time();
                           keyboard_Maxtimers[cad]=max_espera;
                     }
                  }

          }
      }

      case SDL_MOUSEBUTTONDOWN:{
          last_mouse_pos=Vector2((float)ev.motion.x,(float)ev.motion.y);

          if(ev.button.button=SDL_BUTTON_LEFT){
                 mouse_states[1]=0;
                mouse_timers[1]=Timer::get_delta_time();
                mouse_Maxtimers[1]=max_espera;
          }
          else if(ev.button.button==SDL_BUTTON_RIGHT){

                     mouse_states[2]=0;
                    mouse_timers[2]=Timer::get_delta_time();
                    mouse_Maxtimers[2]=max_espera;
          }

      }
      case SDL_MOUSEBUTTONUP:{
           if(ev.button.button=SDL_BUTTON_LEFT && ev.button.state==SDL_RELEASED){
                last_mouse_pos=Vector2((float)ev.motion.x,(float)ev.motion.y);

                mouse_states[1]=2;

                mouse_timers[1]=Timer::get_delta_time();
                mouse_Maxtimers[1]=max_espera;
          }
          else if(ev.button.button==SDL_BUTTON_RIGHT && ev.button.state==SDL_RELEASED){
               last_mouse_pos=Vector2((float)ev.motion.x,(float)ev.motion.y);

               mouse_states[2]=2;
               mouse_timers[2]=Timer::get_delta_time();
               mouse_Maxtimers[2]=max_espera;
          }
      }
      case SDL_MOUSEMOTION:{
          last_mouse_pos=Vector2((float)ev.motion.x,(float)ev.motion.y);

      }

   }

}

 bool Input:: Mouse_Up(int boton){
     bool res=false;
     if(boton<0 || boton>=3 ){
        return false;
     }
      if(mouse_states[boton]==1){
        res=true;
     }
     return res;

 }
 bool Input:: Mouse_Down(int boton){
     bool res=false;
     if(boton<0 || boton>=3 ){
        return false;
     }
     if(mouse_states[boton]==0){
        res= true;
     }
     return res;
 }
 bool Input:: Mouse_Pressed(int boton){
      bool res=false;
      if(boton<0 || boton>=3 ){
        return false;
     }
      if(mouse_states[boton]==1){
        res= true;
     }
     return res;
 }


 bool Input:: Key_Down(std::string key){
      bool res=false;
      if(keyboard_states.find(key)==keyboard_states.end()){
          return false;
      }
      if(keyboard_states[key]==0){
        res=true;
      }
     return res;
 }
 bool Input:: Key_up(std::string key){
     bool res=false;
     if(keyboard_states.find(key)==keyboard_states.end()){
          return false;
      }
     if(keyboard_states[key]==2){
        res=true;
      }
     return res;
 }
bool Input:: Key_Pressed(std::string key){
     bool res=false;


    if(keyboard_states.find(key)==keyboard_states.end()){
          return false;
      }
     if(keyboard_states[key]==1){
        res=true;
      }
     return res;
  }

Vector2 Input::get_Mouse_pos(){
   return last_mouse_pos;
}
void Input::reset(){

    for(int i=0;i<3;i++){
        mouse_states[i]=-1;
        mouse_timers[i]=0.0f;
     }

   map<string,int>::iterator it;

    for(it=keyboard_states.begin();it!=keyboard_states.end();it++){
         string id_key=it->first;
         keyboard_states[id_key]=-1;
         keyboard_timers[id_key]=0.0;

    }

     for(int k=0;k<10;k++){
       Gamepads_states[k]=-1;
       Gamepads_timers[k]=0.0f;

     }
}
string Input:: get_lastKey(){

  return last_keyInput;
}
string Input::last_keyInput="";
