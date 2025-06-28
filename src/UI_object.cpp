#include "UI_object.h"
#include "Event_Manager.h"
#include "Component.h"
#include "Singleton.h"



 Data_Text::Data_Text(){

    offset_font=Vector2(0,0);
 }
Data_Text:: Data_Text(string text,string font,int sz,SDL_Color letra_color,Vector2 offset_f,int min_chars){
    texto=text;
    color=letra_color;
    fuente=font;
    size_letras=sz;
    offset_font=offset_f;
    minimo_characters=min_chars;
    initial_offset=offset_font;


}
string Data_Text::get_text(){
   return texto;
}

string Data_Text::get_font(){
   return fuente;
}
Vector2  Data_Text::get_offset(){
   return offset_font;
}
int  Data_Text::get_size(){
    return size_letras;
}
SDL_Color  Data_Text::get_color(){
    return color;
}
void  Data_Text::set_color(SDL_Color colr){
     color=colr;
}
void  Data_Text::set_text(string value){
      texto=value;
      if(minimo_characters>0){
          if(texto.length()<minimo_characters){
               float dif=minimo_characters-texto.length();
               int margen=0;

               margen=floor(dif/2.0f)*(size_letras*0.5f);

               offset_font=Vector2(initial_offset.x+margen,initial_offset.y);

          }

      }
}

 void  Data_Text::set_offset(Vector2 offset_f){
      offset_font=offset_f;
 }



Icon_UI::Icon_UI(){


}

Icon_UI::Icon_UI(SDL_Rect temp_rect,vector<int>indexs){

   rect=temp_rect;
   tex_index=indexs;


}

SDL_Rect Icon_UI:: get_rect(){

    return rect;
 }
vector<int> Icon_UI::get_indexs(){
      return tex_index;
}
UI_object::UI_object()
{
    in_mouseCollision=false;



}
UI_object::UI_object(vector<string> sfx)
{
    in_mouseCollision=false;
     audios_asociado=sfx;


}

UI_object::~UI_object()
{

}
string UI_object::  get_audioName(int index){
    if(index>=0 && index<audios_asociado.size()){
         return audios_asociado[index];
    }
    return "";
}
void UI_object:: update(Game_Object* g){

}

bool UI_object::mouse_collision(SDL_Rect r){

     Vector2 mouse_pos=Vector2(0,0);
     mouse_pos=Input::get_Mouse_pos();
     SDL_Rect rect=r;

     Vector2 half_size=Vector2(rect.w*0.5f,rect.h*0.5f);

     Vector2 c=Vector2(rect.x+half_size.x,rect.y+half_size.y);

     if(c.x+half_size.x>=mouse_pos.x && c.x-half_size.x<=mouse_pos.x){


       if(c.y-half_size.y<=mouse_pos.y && c.y+half_size.y >=mouse_pos.y){
           in_mouseCollision=true;

           return true;
       }
     }
     return false;
}


void UI_object::on_enter(Game_Object* g){

}
void UI_object::on_exit(Game_Object* g){

}
void UI_object::on_drag(Game_Object* g){

}
void UI_object::on_click(Game_Object* g){

}
void UI_object::set_eventsClick(string data){
 if(data!=""){


              string temp="";
              string temp_value="";
              string temp_args="";

             for(int i=0;i<data.length();i++){

                  unsigned char c=data[i];
                  if(c=='|'){
                    // asignamos valores a variables

                     if(temp!=""  ){
                          if(temp_value==""){
                             temp_value=temp;
                          }

                          else {
                               if(temp_args==""){
                                   temp_args=temp;
                               }
                               else{
                                    temp_args=temp_args+","+temp;
                               }

                           }

                       }

                  int id_ev=Math_G::StrToInt(temp_value);
                  evs_click[id_ev]=temp_args;
                  temp="";
                  temp_value="";
                  temp_args="";
                }
               else if(c==':'){
                    temp_value=temp;
                    temp="";
               }
               else{
                   temp+=c;
               }
           }


  }
}
void UI_object::set_eventsSelected(string data){
 if(data!=""){


              string temp="";
              string temp_value="";
              string temp_args="";

             for(int i=0;i<data.length();i++){

                  unsigned char c=data[i];
                  if(c=='|'){
                    // asignamos valores a variables

                     if(temp!=""  ){
                          if(temp_value==""){
                             temp_value=temp;
                          }

                          else {
                               if(temp_args==""){
                                   temp_args=temp;
                               }
                               else{
                                    temp_args=temp_args+","+temp;
                               }

                           }

                       }

                  int id_ev=Math_G::StrToInt(temp_value);
                  evs_selected[id_ev]=temp_args;
                  temp="";
                  temp_value="";
                  temp_args="";
                }
               else if(c==':'){
                    temp_value=temp;
                    temp="";
               }
               else{
                   temp+=c;
               }
           }


  }
}
map<int,string> UI_object::get_events_click(){
   return evs_click;
}
map<int,string> UI_object::get_events_selected(){
   return evs_selected;
}

 vector<Game_Object> UI_object::UI_escena;
 vector<Game_Object>& UI_object::get_UI(){
     return UI_escena;
 }
 Game_Object UI_object::Find(string name,int& index){
    Game_Object res=Game_Object();
    int index_g=0;
    vector<Game_Object>::iterator it;
    for(it=UI_escena.begin();it!=UI_escena.end();it++){
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
 void UI_object::set_UI(vector<Game_Object>& objects){
      UI_escena=objects;
      vector<Game_Object> ::iterator it;
      int index=0;
      for(it=UI_escena.begin();it!=UI_escena.end();it++){
           Game_Object g=*it;
           string parent=g.get_ParentName();
           if(parent!=""){
                int index_parent=0;
                bool found=false;
                vector<Game_Object> ::iterator it_parent;
                for(it_parent=UI_escena.begin();it_parent!=UI_escena.end();it_parent++){
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
              UI_escena[index]=g;
           }
           index++;
      }
 }
 void  UI_object::EditUI(Game_Object g,int index){
     if(index<0 || index>=UI_escena.size()){
          return;
     }
     Game_Object g2=UI_escena[index];
     if(g2.get_name()==g.get_name()){
          UI_escena[index]=g;
     }
 }
  void UI_object::Destroy_UI(Game_Object g,int index){
        if(index<0 || index>=UI_escena.size()){
             return;
        }
        vector<Game_Object>::iterator it;
        string name_g=g.get_name();
        Game_Object target=UI_escena[index];
        if(target.get_name()!=name_g){
            return;
        }

        //borramos parentesco de los childs
        int index_g=0;
        for(it=UI_escena.begin();it!=UI_escena.end();it++){
            Game_Object g2=*it;
            if(g2.get_ParentName()==name_g){
                  g2.set_parent("",-1);
                UI_escena[index_g]=g2;
            }
            index_g++;
        }
        UI_escena.erase(UI_escena.begin()+index);

  }
void UI_object::add_UI(Game_Object g){
       if(g.get_name()==""){
         UI_escena.push_back(g);
       }

  }
map<int,Game_Object> UI_object::get_radioGroup(string id){
   map<int,Game_Object> res;
    int index_g=0;
    vector<Game_Object>::iterator it;
    for(it=UI_escena.begin();it!=UI_escena.end();it++){
         Game_Object g=*it;
         if(g.get_UIid()==UI_TYPES::RADIOBUTTON){

            RadioButton *r=Component::get_radioButton(g.get_name());
            if(r!=NULL){
               if(r->get_radioId()==id){
                   res[index_g]=g;
               }
           }

         }
         index_g++;

    }
    return res;
}


void UI_object::limpiar_UI(){
   if(UI_escena.size()>0){
        UI_escena.clear();
   }

}

Button:: Button(){

}
Button:: Button(float esper,vector<Icon_UI> icons,vector<SDL_Color> colors_text,vector<Vector2> dims,vector<string> sfx):UI_object(sfx)
{
    iconos=icons;
    dims_text=dims;
    text_colors=colors_text;
    has_text=false;
    max_esperaEv=esper;
    espera=0.0f;

}

void Button::set_eventsClick(string data){
     UI_object::set_eventsClick(data);
}
void Button::set_eventsSelected(string data){
     UI_object::set_eventsSelected(data);
}

Data_Text Button:: get_textData(bool& with_text){
    Data_Text res=Data_Text();
    if(has_text==true){
        res= text_d;
        with_text=has_text;
    }
    return res;
}
void Button::add_text(Data_Text t){
   text_d=t;
   has_text=true;
}
void Button:: on_click(Game_Object* g)
{
    string sfx=get_audioName(0);
    if(sfx!=""){
        AudioManager::Play_Sfx(sfx,0,0);
    }
    if(max_esperaEv==0.0f){
        execute_events();
    }
    else{
         espera=Timer::get_delta_time();
    }

       Render* render_comp=Component::get_Render(g->get_name());

       if(iconos.size()>=3 && render_comp!=NULL){

           Icon_UI icn=iconos[2];

            render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

        }

    if(has_text){
        if(text_colors.size()>=3){
            text_d.set_color(text_colors[2]);
        }
    }
}
void Button::execute_events(){

        map<int,string>evs_list=get_events_click();

         map<int,string>::iterator it;
        for(it=evs_list.begin();it!=evs_list.end();it++){
           int id=it->first;
           string args=it->second;


           Event_Manager::execute_event(id,args);

       }
}
void Button::  reset(Game_Object* g)
{
    in_mouseCollision=false;
    on_exit(g);

}
void Button:: on_enter(Game_Object* g)
{

     string sfx=get_audioName(1);
     if(sfx!=""){
        AudioManager::Play_Sfx(sfx,0,0);
     }
     Render* render_comp=Component::get_Render(g->get_name());

     if(render_comp!=NULL){
       if(iconos.size()>=2 ){


           Icon_UI icn=iconos[1];
           render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

       }
     }

    if(has_text){
        if(text_colors.size()>=2){

            text_d.set_color(text_colors[1]);
        }
    }

}
void Button:: on_exit(Game_Object* g)
{

       Render* render_comp=Component::get_Render(g->get_name());
       if(render_comp!=NULL){
           if(iconos.size()>=1 ){

               Icon_UI icn=iconos[0];

               render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

           }
       }

    if(has_text){
        if(text_colors.size()>=1){
            text_d.set_color(text_colors[0]);
        }
    }
}
void Button:: update(Game_Object* g)
{
    if(g==NULL){
        return;
    }
    if(espera>0.0f){
        espera=espera+Timer::get_delta_time();
        if(espera>=max_esperaEv){
             execute_events();
              espera=0.0f;
        }

    }
    bool last_mCol=in_mouseCollision;
    bool coli=mouse_collision(g->get_object_rect());



        if(coli==true){
            if(Input::Mouse_Down(MOUSE_BUTTONS::LEFT)){
                on_click(g);
            }
            else{

                  if(last_mCol==false){
                       on_enter(g);
                    }

            }
        }
        else{
            if(in_mouseCollision==true ){
                 reset(g);
            }
        }





}

 Label::Label(){


 }
Label::Label(float esper,string sel_obj,SDL_Color select_clr,Data_Text t,vector<string> sfx):UI_object(sfx){
      text_d=t;
      selected=false;
      init_color=text_d.get_color();
      select_color=select_clr;
      espera=0.0f;
      espera_inicio=0.016f;
      max_espera=esper;
      obj_select_Asoc=sel_obj;


}
Data_Text Label::get_textData(bool& with_text){
    with_text=true;
    return text_d;
}
void Label::set_textColor(SDL_Color colr){
   text_d.set_color(colr);
}
void Label::set_text(string val){
     text_d.set_text(val);

}
void Label::set_eventsClick(string data){
     UI_object::set_eventsClick(data);
}
void Label::set_eventsSelected(string data){
     UI_object::set_eventsSelected(data);
}
void Label::update(){

   if(espera_inicio>0.00f){
      espera_inicio=espera_inicio+Timer::get_delta_time();
      if(espera_inicio>=0.4f){
        espera_inicio=0.0f;
      }
   }
   if(espera>0.0f){
      espera=espera+Timer::get_delta_time();
      if(espera>=max_espera){
          espera=0.0f;
          execute_events();
      }
   }
}
void Label:: execute_events(){

     map<int,string> evs=get_events_click();
     map<int,string>::iterator it;
     for(it=evs.begin();it!=evs.end();it++){
           int id=it->first;
           string args=it->second;


           Event_Manager::execute_event(id,args);
       }

}
void Label::set_selected(bool val,string obj_name){

  if(espera_inicio==0.0f){
    string sfx=get_audioName(1);
    if(sfx!=""){
        AudioManager::Play_Sfx(sfx,0,0);
    }
  }
   selected=val;


   if(selected){
       text_d.set_color(select_color);
       map<int,string> evs_sel=get_events_selected();
       map<int,string>::iterator it;
       for(it=evs_sel.begin();it!=evs_sel.end();it++){
           int ev=it->first;
           string args=it->second;
            Event_Manager::execute_event(ev,args);

       }


       if(obj_select_Asoc!=""){

          int index=-1;
          Game_Object g=UI_object::Find(obj_select_Asoc,index);
          if(index!=-1){
               g.set_active(true);
               UI_object::EditUI(g,index);
               Animator *anim=Component::get_Animator(obj_select_Asoc);
              if(anim!=NULL){
                  anim->set_parameter_value("Selected","True");

               }
          }
       }
   }
   else{
      text_d.set_color(init_color);
      if(obj_select_Asoc!=""){

          int index=-1;
          Game_Object g=UI_object::Find(obj_select_Asoc,index);
          if(index!=-1){
               g.set_active(false);
               UI_object::EditUI(g,index);
          }

       }
   }


}
void Label::trigger_label(string obj_name){
    if(get_events_click().size()<=0){
        return;
    }
    string sfx=get_audioName(0);
    if(sfx!=""){

        AudioManager::Play_Sfx(sfx,0,0);
    }
    if(max_espera<=0.0f){
          execute_events();
    }
    else{
        espera=Timer::get_delta_time();
    }

}

void Label::reset(){
    text_d.set_color(init_color);
}

RadioButton::RadioButton(){
    activ_pend=false;
}
RadioButton::RadioButton(vector<Icon_UI> icons,bool select,string value,string rId,vector<string> sfx):UI_object(sfx){


    activ_pend=false;
    iconos=icons;
    init_selected=select;
    if(init_selected==true){
        activ_pend=true;
    }
    radioId=rId;
    radioValue=value;
    has_text=false;


}

void RadioButton::set_eventsClick(string data){
     UI_object::set_eventsClick(data);
}
void RadioButton::set_eventsSelected(string data){
     UI_object::set_eventsSelected(data);
}
bool RadioButton::RadioButton::is_selected()
{
      return selected;
}
string RadioButton::get_selectedValue()
{
     return radioValue;
}
void RadioButton::execute_events(){

    map<int,string> evs=get_events_selected();
    map<int,string>::iterator it;
    for(it=evs.begin();it!=evs.end();it++){
          int ev=it->first;
          string arg=it->second;
          Event_Manager::execute_event(ev,arg);
    }


  }
void RadioButton::update(Game_Object* g)
{
    if(g==NULL){
        return;
    }
    if(activ_pend==true){
        selected=true;
        on_click(g);
        activ_pend=false;
    }

    if(mouse_collision(g->get_object_rect())){
        if(Input::Mouse_Down(MOUSE_BUTTONS::LEFT)){
            if(selected==false){
                set_selected(true);
                on_click(g);
                map<int,Game_Object> radioGroup=UI_object::get_radioGroup(radioId);
                map<int,Game_Object>::iterator it;
               for(it=radioGroup.begin();it!=radioGroup.end();it++){
                   int index=it->first;
                    Game_Object g2=it->second;
                   if(g2.get_name()!=g->get_name()){
                           RadioButton *r=Component::get_radioButton(g2.get_name());
                           if(r!=NULL){
                               r->set_selected(false);
                           }
                   }
               }
            }

        }
        else{
            if(selected==false){
                on_enter(g);
            }
        }
    }
    else{
        if(selected==false){
            on_exit(g);
        }
        else{
             on_click(g);
        }
    }

}
void RadioButton::on_click(Game_Object* g)
{



    Render* render_comp=Component::get_Render(g->get_name());

        if(iconos.size()>=3 && render_comp!=NULL){

           Icon_UI icn=iconos[2];

           render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

       }


}

void RadioButton::on_enter(Game_Object* g)
{

    Render* render_comp=Component::get_Render(g->get_name());

    if(iconos.size()>=2 && render_comp!=NULL){

          Icon_UI icn=iconos[1];

          render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

       }

}
void RadioButton::on_exit(Game_Object* g)
{

      Render* render_comp=Component::get_Render(g->get_name());

    if(iconos.size()>=1 && render_comp!=NULL){

        Icon_UI icn=iconos[0];

        render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

      }

}
void RadioButton::reset(Game_Object* g)
{


       selected=init_selected;



       if(selected){
        on_click(g);
       }
       else{
         on_exit(g);
       }




}
void RadioButton::set_selected(bool val)
{


    bool last_s=selected;
    selected=val;
    if(selected && last_s==false){

        string sfx=get_audioName(0);
        if(sfx!=""){
            AudioManager::Play_Sfx(sfx,0,0);
        }

       execute_events();
    }
}
string RadioButton::get_radioId(){
   return radioId;
}
void RadioButton:: set_text(Data_Text t){
   has_text=true;
   text_d=t;
}
Data_Text RadioButton::get_textData(bool& with_text){
   if(has_text){
      with_text=true;
      return text_d;
   }
   with_text=false;
   return Data_Text();
}
CheckButton::CheckButton(){

}
CheckButton::CheckButton(vector<Icon_UI> icons,bool select,string value,Data_Text t,vector<string> sfx):UI_object(sfx){
     iconos=icons;
     selected=select;
     checkValue=value;
     init_select=select;
     text_d=t;
}
bool CheckButton::is_selected(){
   return selected;
}
string CheckButton::get_selectedValue(){
  return checkValue;
}
void CheckButton::reset(Game_Object* g){
    set_selected(init_select);
    if(selected){
        on_click(g);
    }
    else{
         on_exit(g);
    }

}
void CheckButton::set_selected(bool val){
    selected=val;
}
void CheckButton::update(Game_Object* g){

    if(g==NULL){
        return;
    }
    if(mouse_collision(g->get_object_rect())){
        if(Input::Mouse_Down(MOUSE_BUTTONS::LEFT)){
            if(selected==false){
                selected=true;
                on_click(g);

            }
            else{
                selected=false;
                on_enter(g);
            }
        }
        else{
            if(selected==false){
                on_enter(g);
            }

        }
    }
    else{
        if(selected==false){
            on_exit(g);
        }
    }
}
void CheckButton::on_click(Game_Object* g){

       Render* render_comp=Component::get_Render(g->get_name());

       string sfx=get_audioName(0);
       if(sfx!=""){
           AudioManager::Play_Sfx(sfx,0,0);
       }
       if(iconos.size()>=3 && render_comp!=NULL){

           Icon_UI icn=iconos[2];

           render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

       }

}
void CheckButton::on_enter(Game_Object* g){

     Render* render_comp=Component::get_Render(g->get_name());

     if(iconos.size()>=2 && render_comp!=NULL){

        Icon_UI icn=iconos[1];

        render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

    }
}
void CheckButton::on_exit(Game_Object* g){

     Render* render_comp=Component::get_Render(g->get_name());

     if(iconos.size()>=1 && render_comp!=NULL){

        Icon_UI icn=iconos[0];

        render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

    }
}
Data_Text CheckButton:: get_textData(bool& with_text){
    with_text=true;
    return text_d;
 }
ValueBar::ValueBar(){
    has_text=false;
    has_colors=false;
    has_icono=false;
 }
ValueBar::ValueBar(int sentd,float inicial_value,vector<Icon_UI> estads,bool cut,vector<string> sfx):UI_object(sfx){
      if(sentd==0){
        sentido=Vector2(1,0);
      }
      else{
        sentido=Vector2(0,1);
      }
      init_val=inicial_value;
      set_value(init_val);
      has_text=false;
      has_colors=false;
      has_icono=false;
      estados=estads;
      cut_src=cut;
}
void ValueBar::update(Game_Object* g){
     if(g==NULL){
        return;
     }
     Render* render_comp=Component::get_Render(g->get_name());

     if(estados.size()>0 && render_comp!=NULL){
        if(value<=0.2f){
            Icon_UI icn=estados[2];
            render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

        }
        else if(value<=0.6f){
             Icon_UI icn=estados[1];
             render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

        }
        else{
             Icon_UI icn=estados[0];
             render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());

        }
     }
}
bool ValueBar::cut_sprite(){
   return cut_src;
}
void ValueBar::set_value(float val){

    if(val<0.0f){
        val=0.0f;
    }
    else if(val>1.0f){
        val=1.0f;
    }

    value=val;


}
float ValueBar::get_value(){
    return value;
}
Vector2 ValueBar::get_sentido(){
    return sentido;
}
void ValueBar::reset(Game_Object* g){
    set_value(init_val);
    update(g);
}
void ValueBar::add_text(Data_Text t){
    text_d=t;
    has_text=true;
}
Data_Text ValueBar:: get_textData(bool& with_text){
   with_text=false;
   Data_Text res=Data_Text();
   if(has_text){
        with_text=true;
       return res;
   }
}

void ValueBar::add_iconBg(Icon_UI icono){
    icon_bg=icono;
    has_icono=true;
}
void ValueBar::add_BgColors(vector<SDL_Color> colors){
    bg_colors=colors;
    if(bg_colors.size()>1){
        has_colors=true;
    }

}
Icon_UI ValueBar::get_BgIcon(bool& with_icon){
     Icon_UI res=Icon_UI();
     with_icon=false;
     if(has_icono){
         with_icon=true;
         res=icon_bg;
     }
     return res;
}
vector<SDL_Color> ValueBar::get_BgColors(bool& withColors){
     vector<SDL_Color> res;
     withColors=false;
     if(has_colors){
         withColors=true;
         res=bg_colors;
     }
     return res;
}
Image_UI::Image_UI(){
  has_text=false;
}
Image_UI::Image_UI(vector<string> sfx):UI_object(sfx){
   has_text=false;
}
void Image_UI::set_eventsClick(string data){
    UI_object::set_eventsClick(data);
}
void Image_UI::set_eventsSelected(string data){
  UI_object::set_eventsSelected(data);
}
void Image_UI::change_image(Game_Object* g,int gId){
    if(g==NULL){
        return;
    }
    SDL_Rect r;
    vector<int> indexs=Texture_manager::get_indexs(&r,gId);
     Render* render_comp=Component::get_Render(g->get_name());

    if(indexs.size()>=2){
         if(indexs.at(0)!=-1 && indexs.at(1)!=-1 && render_comp!=NULL){
            render_comp->get_sprite().change_texture(r,indexs);
        }
    }



}
void Image_UI::change_image(Game_Object* g,string text_name,bool multiple){

}
void Image_UI::add_text(Data_Text text){
  has_text=true;
  text_d=text;
}

Data_Text Image_UI::get_textData(bool & with_text){
  Data_Text res=Data_Text();
  with_text=false;
   if(has_text){
        res=text_d;
        with_text=true;
   }
   return res;
}
ScrollBar::ScrollBar(){
    selected=false;
    init_pos=Vector2(0,0);
}
ScrollBar::ScrollBar(bool r_desplz,int sent,SDL_Color bg_color,SDL_Color fg_color,vector<Icon_UI>icons,SDL_Rect bg_r,Vector2 obj_dims,vector<string> sfx):UI_object(sfx){

      reset_desplz=r_desplz;
     desplz=0.0f;
     iconos=icons;
     selected=false;
     bg_rect=bg_r;

     if(sent==0){
        sentido=Vector2(1,0);
        bg_rect.h=obj_dims.y;
     }
     else{
        sentido=Vector2(0,1);
        bg_rect.w=obj_dims.x;
     }
     colors_bg.push_back(bg_color);
     colors_bg.push_back(fg_color);
     init_pos=Vector2(0,0);
}
 void ScrollBar::update(Game_Object* g){
     if(g==NULL){
        return;
     }

            if(init_pos.x==0 && init_pos.y==0){
              Transform* t=Component::get_Transform(g->get_name());
               if(t!=NULL){
                   init_pos=t->Get_Position();

                   if(desplz>0){
                      set_desplazamiento(desplz,g);
                   }
               }
            }



      if(mouse_collision(g->get_object_rect())){
        if(Input::Mouse_Down(MOUSE_BUTTONS::LEFT) || Input::Mouse_Pressed(MOUSE_BUTTONS::LEFT)){
           if(selected==false){
              string sfx=get_audioName(0);
              if(sfx!=""){
                 AudioManager::Play_Sfx(sfx,0,0);
              }
              selected=true;
           }
           on_drag(g);
        }
        else{
            if(selected){
                selected=false;
            }
            on_enter(g);
        }
      }
      else{

        if(selected){
            on_drag(g);
            if(Input::Mouse_Up(MOUSE_BUTTONS::LEFT)==true || Input::Mouse_Pressed(MOUSE_BUTTONS::LEFT)==false){
                selected=false;
            }
        }
        else{
           on_exit(g);
        }
      }
 }

void ScrollBar::set_desplazamiento(float val,Game_Object* g){
   if(g==NULL){
      return;
   }
   if(val<0.0f){
      val=0.0f;
   }
   if(val>1.0f){
    val=1.0f;
   }
   desplz=val;


   if(init_pos.x==0 && init_pos.y==0){
       return;
   }
   Transform* t=Component::get_Transform(g->get_name());
   if(t==NULL){
      return;
   }

   SDL_Rect r_obj=g->get_object_rect();


   if(sentido.x!=0){
           Vector2 next_p=init_pos;
           next_p.x=next_p.x+(desplz*abs(bg_rect.w-r_obj.w));
            t->Set_Position(next_p);
     }
    else{
           Vector2 next_p=init_pos;
            next_p.y=next_p.y-(desplz*abs(bg_rect.h-r_obj.h));
            t->Set_Position(next_p);
         }

}

 void ScrollBar::on_drag(Game_Object* g){


    SDL_Rect r_obj=g->get_object_rect();
    Vector2 halfS=Vector2(r_obj.w*0.5f,r_obj.h*0.5f);

    Vector2 m_pos=Input::get_Mouse_pos();
    Transform * trans=Component::get_Transform(g->get_name());
    if(trans==NULL ){
        return;
    }
    Vector2 old_pos=trans->Get_Position();
    Vector2 next_pos=old_pos;


    if(sentido.x==0){
        m_pos.y=m_pos.y*-1;
        next_pos.y=m_pos.y+halfS.y;
        float bg_max=bg_rect.y+bg_rect.h;
        float bg_min=bg_rect.y;
        bg_min=bg_min*-1;
        bg_max=bg_max*-1;


        if(next_pos.y-r_obj.h<bg_max){
            next_pos.y=bg_max+r_obj.h;

        }
        else if(next_pos.y>bg_min){
            next_pos.y=bg_min;

        }
       float max_desplz=abs(bg_rect.h-r_obj.h);
       if( max_desplz>0){
          desplz=abs(init_pos.y-next_pos.y)/ max_desplz;

       }
    }
    else{


        next_pos.x=m_pos.x-halfS.x;
        float bg_max=bg_rect.x+bg_rect.w;
        float bg_min=bg_rect.x;


        if(next_pos.x+r_obj.w>bg_max){
            next_pos.x=bg_max-r_obj.w;

        }
        else if(next_pos.x<bg_min){
            next_pos.x=bg_min;

        }
       float max_desplz=abs(bg_rect.w-r_obj.w);
       if( max_desplz>0){
          desplz=abs(init_pos.x-next_pos.x)/ max_desplz;

       }
    }

    trans->Set_Position(next_pos);

    Render* render_comp=Component::get_Render(g->get_name());

    if(iconos.size()>=3 && render_comp!=NULL){
       Icon_UI icn=iconos[2];
       render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());
   }
 }
 void ScrollBar::on_enter(Game_Object* g){
     Render* render_comp=Component::get_Render(g->get_name());

    if(iconos.size()>=2 && render_comp!=NULL){
       Icon_UI icn=iconos[1];
       render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());
   }
 }
 void ScrollBar::on_exit(Game_Object* g){
     Render* render_comp=Component::get_Render(g->get_name());

   if(iconos.size()>=1 && render_comp!=NULL){
       Icon_UI icn=iconos[0];
       render_comp->get_sprite().change_texture(icn.get_rect(),icn.get_indexs());
   }
 }
SDL_Rect ScrollBar::get_bgRect(){
   return bg_rect;
}
 vector<SDL_Color> ScrollBar::get_BGcolors(){
     return colors_bg;
 }
void ScrollBar::reset(Game_Object* g){

    if(g!=NULL){

       if((init_pos.x==0 && init_pos.y==0)==false){
           if(reset_desplz){
               desplz=0.0f;
                Transform* t=Component::get_Transform(g->get_name());
                 if(t!=NULL){
                     t->Set_Position(init_pos);
                 }

           }



       }
        on_exit(g);
    }

}
float ScrollBar:: get_desplazamiento(){

   return desplz;
}
void ScrollBar::activate_rastro(SDL_Color clr){
    show_rastro=true;
    rastro_color=clr;

}
bool ScrollBar::use_rastro(SDL_Color* clr){
     if(show_rastro){
          if(clr!=NULL){
            clr->r=rastro_color.r;
            clr->g=rastro_color.g;
            clr->b=rastro_color.b;
            clr->a=rastro_color.a;
          }

     }
     else{
        if(clr!=NULL){
          clr->r=0;
          clr->g=0;
          clr->b=0;
          clr->a=0;
        }
     }
     return show_rastro;
}
 Vector2 ScrollBar::get_sentido(){
    return sentido;

 }
void ScrollBar:: move_bar(int dir,float val,string obj_name){
    int index=-1;
    Game_Object g=UI_object::Find(obj_name,index);
    if(index==-1){
        return;
    }
    Transform * trans=Component::get_Transform(g.get_name());
    if(trans==NULL ){
        return;
    }
    SDL_Rect r_obj=g.get_object_rect();
    Vector2 halfS=Vector2(r_obj.w*0.5f,r_obj.h*0.5f);

    Vector2 next_pos=trans->Get_Position();
    if(sentido.x!=0){
         next_pos.x=next_pos.x+(dir*val);
          float bg_max=bg_rect.x+bg_rect.w;
          float bg_min=bg_rect.x;


          if(next_pos.x+r_obj.w>bg_max){
               next_pos.x=bg_max-r_obj.w;

           }
           else if(next_pos.x<bg_min){
               next_pos.x=bg_min;

           }
           float max_desplz=abs(bg_rect.w-r_obj.w);
           if( max_desplz>0){
               desplz=abs(init_pos.x-next_pos.x)/ max_desplz;

           }
    }
    else{
          float bg_max=bg_rect.y+bg_rect.h;
          float bg_min=bg_rect.y;
          bg_min=bg_min*-1;
          bg_max=bg_max*-1;

          next_pos.y=next_pos.y+(dir*val);

          if(next_pos.y-r_obj.h<bg_max){
            next_pos.y=bg_max+r_obj.h;

           }
           else if(next_pos.y>bg_min){
               next_pos.y=bg_min;

           }
          float max_desplz=abs(bg_rect.h-r_obj.h);
          if( max_desplz>0){
             desplz=abs(init_pos.y-next_pos.y)/ max_desplz;

          }
    }
    trans->Set_Position(next_pos);




}
