#include "Animation.h"


Frame::Frame(){
  duracion=0;
  valid_frame=false;
}
Frame::Frame(int dur,SDL_Rect rect){
   duracion=dur/1000.0f;
   sprite_pos=rect;
   valid_frame=true;
}
SDL_Rect Frame::get_rect_sprite(){
   return sprite_pos;
}
bool Frame::is_valid(){
    return valid_frame;
}
float Frame::get_duracion(){
   return duracion;
}

Condition_Transition::Condition_Transition(){
   condicion_name="";
   valor="";
   operacion="";
   val_type=-1;
}
Condition_Transition::Condition_Transition(string cond_name,string cond_val,string oper){
   condicion_name=cond_name;
   valor=cond_val;
   operacion=oper;
   val_type=0;   //string val
   if(valor!="" ){

      if(Math_G::is_number(valor)==true){
           bool is_float=false;
           for(int i=0;i<valor.length();i++){
               unsigned char c=valor[i];
               if(c=='.' || c==','){
                  is_float=true;
                 i=valor.length();
             }

          }
          if(is_float){
              val_type=2;   //valor float
          }
          else{
             val_type=1;     //valor entero
          }
      }

   }

}
bool Condition_Transition::condition_clear(string param_name,string param_val){
   bool res=false;
   if(condicion_name==param_name ){
       if(operacion=="equal"){
          if(valor==param_val){
              res=true;
          }
       }
       else{

           if(Math_G::is_number(param_val) && Math_G::is_number(valor)){
                float v1=Math_G::StrToFloat(valor);
                float v2=Math_G::StrToInt(param_val);
                 if(operacion=="lower"){
                     if(v2<v1){
                       res=true;
                      }
                  }
                  else if(operacion=="lower or equal"){
                        if(v2<=v1){
                         res=true;
                       }
                  }
                  else if(operacion=="upper" ){
                        if(v2>v1){
                         res=true;
                       }
                  }
                  else if(operacion=="upper or equal"){
                        if(v2>=v1){
                          res=true;
                        }
                  }
            }
       }



}

   return res;
}
string Condition_Transition::get_name(){
   return condicion_name;
}
string  Condition_Transition::get_valor(){
       return valor;
}
string  Condition_Transition::get_operacion_type(){
     return operacion;
}
Transition::Transition(){
   operador_verificacion="";
   anim_from="";
   anim_to="";
}
Transition::Transition(string from,string to,bool wait_finish,string operador){
   operador_verificacion=operador;
   anim_from=from;
   anim_to=to;
}
void Transition::add_condition(Condition_Transition cond){
   condiciones.push_back(cond);
}
vector<Condition_Transition> Transition::get_condiciones(){
   return condiciones;

}

string Transition::get_operador_condiciones(){
    return operador_verificacion;
}

vector<string> Transition::get_anim_names(){
  vector<string> res;
  res.push_back(anim_from);
  res.push_back(anim_to);
  return res;

}

Animation::Animation(){

    cicle=false;
    frame_actual=0;
    playing=false;
    name="default";

}
Animation::Animation( string nombre,bool is_cicle){

   cicle=is_cicle;
   frame_actual=0;
   playing=false;
   name=nombre;

}
bool Animation::is_cicle(){
   return cicle;
}
string Animation::get_name(){
   return name;
}
vector<Transition>Animation :: get_transitions(){

   return transiciones;
}
void Animation::add_frame(Frame frm){

   frames.push_back(frm);
}
void Animation::add_Transition(Transition tr){
  transiciones.push_back(tr);


}
Frame Animation::get_Frame_Actual(){
  if(frame_actual>=0 && frame_actual<frames.size()){
     return frames.at(frame_actual);

  }
  else{
      return Frame();
  }


}
bool Animation::is_last_frame(){
   if(frame_actual>=frames.size()-1){
       return true;
   }
   else{
    return false;
   }
}
void Animation::next_frame(){

    if(frame_actual>=frames.size()-1){
        if(cicle==true){
            frame_actual=0;
        }
    }
    else{
        frame_actual+=1;
    }

}

Animator::Animator(){
    loaded=false;
   animacion_actual=-1;
   num_animations=0;
   cont_frame=0.0f;
}
Animator::Animator(string src,int w_spr,int h_spr){

     num_animations=0;
     animacion_actual=-1;
     read_source(src,w_spr,h_spr);
     cont_frame=0.0f;


}
void Animator::read_source(string src,int w_spr,int h_spr){
      src=string(SDL_GetBasePath())+"assets/game_data/"+src;
     ifstream stream;
     string linea="";
     stream.open(src,ios::in);
     loaded=false;

     if(stream.good()){

         getline(stream,linea);


         string animation_actual="";
         int spacing_sprite=0;
         Animation* temp_anim=NULL;
         Transition* temp_transition=NULL;
         Vector2 animation_dir=Vector2(0,0);
         Vector2 star_p=Vector2(0,0);
         while(getline(stream,linea)){

             string temp_data="";
             map<string,string> parameters;
             string temp_parameter="";
             string temp_tag="";

             int start_index=-1;
             for(int l=0;l<linea.length();l++){
                   unsigned char c=linea[l];
                   if(c=='<'){
                      start_index=l+1;
                      l=linea.length();
                   }
             }
             if(start_index!=-1){
                for(int i=start_index;i<linea.length();i++){

                    unsigned char c=linea[i];
                    if(c==' '){
                        if(temp_tag==""){
                            temp_tag=temp_data;
                            temp_data="";
                        }
                    }
                    else if(c=='"'){

                             if(temp_parameter==""){

                              temp_data.erase(temp_data.begin()+(temp_data.length()-1));
                              temp_parameter=temp_data;
                              temp_data="";

                            }
                          else{
                                 parameters[temp_parameter]=temp_data;
                                 temp_data="";
                                 temp_parameter="";
                          }



                      }
                    else if(c=='>'){

                        if(temp_data.length()>2){
                            //cierra conjunto
                            if(temp_data=="/Animation"){

                                add_animation(*temp_anim);


                                animation_actual="";
                                animation_dir=Vector2(0,0);
                                star_p=Vector2(0,0);
                                delete(temp_anim);
                                delete(temp_transition);
                                temp_anim=NULL;
                                temp_transition=NULL;



                            }
                            else if(temp_data=="/Transition"){
                                if(temp_anim!=NULL){
                                    temp_anim->add_Transition(*temp_transition);
                                }
                                delete(temp_transition);
                                temp_transition=NULL;
                            }
                        }
                        else{
                            //cierra etiqueta simple
                        }
                    }
                    else{
                        temp_data+=c;
                    }
                }
                if(temp_tag=="Animation"){

                    string nam=parameters["name"];
                    string cicle=parameters["cicle"];
                    string temp_dir=parameters["direction"];
                    string temp_x=parameters["start_x"];
                    string temp_y=parameters["start_y"];
                    star_p=Vector2(atoi(temp_x.c_str()),atoi(temp_y.c_str()));

                    if(temp_dir=="down" || temp_dir=="up"){
                          animation_dir=(temp_dir=="down")?Vector2(0,1):Vector2(0,-1);
                    }
                    else if(temp_dir=="rigth" || temp_dir=="left"){
                          animation_dir=(temp_dir=="rigth")?Vector2(1,0):Vector2(-1,0);
                    }
                    bool is_cicle=(cicle=="0")?false:true;

                    temp_anim=new Animation(nam,is_cicle);

                }
                else if(temp_tag=="Transition"){
                    string from_anim=parameters["from"];
                    string to_anim=parameters["to"];
                    string wait_finish_s=parameters["wait_finish"];
                    string oper=parameters["operator"];
                    bool wait_finish=(wait_finish_s=="0")?false:true;
                    temp_transition=new Transition(from_anim,to_anim,wait_finish,oper);

                }
                else if(temp_tag=="Frame"){
                       if(temp_anim!=NULL){
                           int duration=Math_G::StrToInt(parameters["duration"]);
                           int index=Math_G::StrToInt(parameters["index"]);

                           SDL_Rect p_sprite;

                           p_sprite.x=star_p.x+((animation_dir.x*(w_spr*index))+(animation_dir.x*(spacing_sprite*index)));
                           p_sprite.y=star_p.y+((animation_dir.y*(h_spr*index))+(animation_dir.y*(spacing_sprite*index)));
                           p_sprite.w=w_spr;
                           p_sprite.h=h_spr;

                           Frame f=Frame(duration,p_sprite);
                           temp_anim->add_frame(f);
                       }
                }
                else if(temp_tag=="Condition"){
                       if(temp_transition!=NULL){
                           string cond=parameters["cond_name"];
                           string valor=parameters["value"];
                           string oper=parameters["operator"];


                           Condition_Transition cond_t=Condition_Transition(cond,valor,oper);

                           temp_transition->add_condition(cond_t);
                       }
                }
                else if(temp_tag=="Animator"){
                      spacing_sprite=Math_G::StrToInt(parameters["spacing_sprite"]);
                }
                else if(temp_tag=="Parameter"){
                      string p_name=parameters["id"];
                      string p_val=parameters["value"];
                      string p_type=parameters["type"];
                      add_parameter(p_name,p_val);
                }
             }


         }


         params_iniciales=parametros_animator;
         loaded=true;
     }
     else{
        printf(("fail to open animator data:"+src).c_str());
     }
}
void Animator::add_animation(Animation anim){
   animaciones.push_back(anim);
   if(animacion_actual==-1 || anim.get_name()=="iddle"){
       animacion_actual=num_animations;
   }
   num_animations+=1;


}
bool Animator::is_loaded(){
    return loaded;
}
Animation Animator::get_actual_Animation(){
    if(animacion_actual>=0 && animacion_actual<animaciones.size()){
        return animaciones.at(animacion_actual);
    }
    else {
        return Animation();
    }
}

Animation Animator:: get_Animation(string name_anim){
   vector<Animation>::iterator it;
   Animation res=Animation();

   for(it=animaciones.begin();it!=animaciones.end();it++){
         Animation an=*it;
         if(an.get_name()==name_anim){
            res=an;
            break;
         }

   }
   return res;
}

Animation Animator::get_Animation(int index){
  Animation res=Animation();
  if(index>=0 && index<animaciones.size()){
    res=animaciones.at(index);
  }
  return res;
}

void Animator::add_parameter(string param,string value){
  parametros_animator[param]=value;

}
void Animator::set_parameter_value(string param,string value){
  parametros_animator[param]=value;

}
string Animator::get_parameter_value(string param){

     if(parametros_animator.find(param)!=parametros_animator.end()){
         return parametros_animator[param];
     }
      else{
              return "";

      }







}


int Animator::get_Animation_index(string name){
    int res=-1;
    int index=0;
    vector<Animation>::iterator it;

   for(it=animaciones.begin();it!=animaciones.end();it++){
         Animation an=*it;
         if(an.get_name()==name){
            res=index;
            break;
         }
         index+=1;

   }
   return res;
}
void Animator::reset_params(){
  map<string,string>::iterator it;
  for(it=parametros_animator.begin();it!=parametros_animator.end();it++){
      string param=it->first;
      if(params_iniciales.find(param)!=params_iniciales.end()){
          parametros_animator[param]=params_iniciales[param];

      }
  }
}
SDL_Rect Animator::update(){


  SDL_Rect temp;
  temp.x=0;
  temp.y=0;
  temp.w=0;
  temp.h=0;
  if(loaded==true){

     Animation an=Animation();
     if(animaciones.size()>0){
        if(animacion_actual>=0 && animacion_actual<animaciones.size()){
            an=animaciones.at(animacion_actual);

        }
        else{
           Animation an=animaciones.at(0);
        }
     }
     else{
        return temp;
     }



      Frame f=an.get_Frame_Actual();
      float limit_time=0.0f;
      SDL_Rect r;
      if(f.is_valid()){
        limit_time=f.get_duracion();
        r=f.get_rect_sprite();
      }
      else{
        r=temp;
      }




     cont_frame+=Timer::get_delta_time();

     if(cont_frame>=limit_time){

        cont_frame=0.0f;
        an.next_frame();
        animaciones.at(animacion_actual)=an;


     }





     vector<Transition> transiciones=an.get_transitions();
     vector<Transition>::iterator it_t;

     for(it_t=transiciones.begin();it_t!=transiciones.end();it_t++){
         Transition t=*it_t;
         vector<Condition_Transition> conds=t.get_condiciones();
         vector<Condition_Transition>::iterator it_c;
         bool clear_conds=true;
         for(it_c=conds.begin();it_c!=conds.end();it_c++){
            Condition_Transition cond=*it_c;
            string cond_name=cond.get_name();
            string parameter_val=parametros_animator[cond_name];

            if(cond.condition_clear(cond_name,parameter_val)==false){
                clear_conds=false;
                break;
            }
         }
         if(clear_conds==true){

            string next_anim_name=t.get_anim_names().at(1);
            int next_anim=get_Animation_index(next_anim_name);
            animacion_actual=next_anim;
            break;
         }
     }
      temp=r;
  }

  return temp;
}
