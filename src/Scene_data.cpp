#include "Scene_data.h"
#include "Event_Manager.h"
#include "Component.h"
#include "Singleton.h"
#include "projectiles.h"
#include "Enemy.h"

Scene Scene_manager::escena_activa;
vector<string> Scene_manager::escenas_list;
string Scene_manager:: enCola="";
string Scene_manager::escena_actual="";
void Scene_manager::reset(){
    if(escena_actual!=""){

         Component::reset();
         AudioManager::liberar();
         Texture_manager::reset_textures();

    }
}
void Scene_manager::set_scene(Scene s){
   if(previous_loaded==false){
       previous_loaded=true;
   }
   escena_activa=s;
   Input::reset();



}
Scene Scene_manager::get_active_escene(){
   return escena_activa;
}
bool Scene_manager::previous_loaded=false;
float Scene_manager::get_wScale(){
     return w_scale;
 }
void Scene_manager::load_scene(string name){

   vector<string>::iterator it;
   for(it=escenas_list.begin();it!=escenas_list.end();it++){
       string n=*it;
       if(n==name){

            enCola=name;
            loading_scene=true;
             break;
       }
   }
   if(enCola==escena_actual){
            enCola="";
            loading_scene=false;
   }

}
void Scene_manager::load_scene(int index){
   if(index<escenas_list.size() && index>=0){
          enCola=escenas_list[index];
          loading_scene=true;
          if(enCola==escena_actual){
            enCola="";
            loading_scene=false;
          }
   }
}
void Scene_manager::add_scene(string name){
     escenas_list.push_back(name);

}
void Scene_manager::read_escena(string name_scene,vector<Game_Object>& objs,vector<Game_Object>&ui_objects,vector<Camera>& cams,int& cam_activa,vector<Tile>& tls){





          if(w_scale==0.0 || renderer==NULL){
              return;
          }



          int multiplic=1;
          string escenas_name=name_scene;
          string ruta_mapa=string(SDL_GetBasePath())+"assets/game_data/"+name_scene+".tmx";
          string escena_nombre=name_scene;
          string end_name=ruta_mapa.substr(ruta_mapa.length()-(1+3),4);

          if(end_name==".tmx"){
            //mapa en formato tiled (coordenadas y invertidas)
            multiplic=-1;
          }


        Vector2 p_begin=Vector2(0,0);
        Vector2 p_end=Vector2(0,0);
          int widthMap=0;
          int heigthMap=0;
          int tileW=0;
          int tileH=0;
          vector<string> musics;
          vector<string> sfx;
          vector<string> names_list;

          cam_activa=-1;

          int id_texture_background=-1;


           //se lee el mapa
           XML_parser::read_xml(ruta_mapa);


           //extraemos data del mapa

           vector<Node> map_xml=XML_parser::get_etiquetas_by_Name("map");

           Node node_map=map_xml.front();
           string W_s=node_map.get_parameter_data("width");
           string H_s=node_map.get_parameter_data("height");
           string tileW_s=node_map.get_parameter_data("tilewidth");
           string tileH_s=node_map.get_parameter_data("tileheight");


           if(W_s!="" && H_s!="" && tileW_s!="" && tileH_s!=""){

               widthMap=Math_G::StrToInt(W_s);
               heigthMap=Math_G::StrToInt(H_s);
               tileW=Math_G::StrToInt(tileW_s);
               tileH=Math_G::StrToInt(tileH_s);

               tileW=round(tileW*w_scale);
               tileH=round(tileH*w_scale);
               p_end.x=tileW*widthMap;
               p_end.y=-tileH*heigthMap;


               //extraemos datos de las texturas
               vector<Node> texturas_xml=XML_parser::get_etiquetas_by_Name("tileset");
                vector<Node>::iterator it_tileset;
                vector<Texture> lista_textura;



                for(it_tileset=texturas_xml.begin();it_tileset!=texturas_xml.end();it_tileset++){

                       Node n=*it_tileset;
                       string f_id=n.get_parameter_data("firstgid");
                       string base_src=n.get_parameter_data("source");
                       if(f_id!="" && base_src!=""){
                           base_src=string(SDL_GetBasePath())+"assets/game_data/"+base_src;
                           int id=Math_G::StrToInt(f_id);
                           Texture tex=Texture(id,base_src,renderer);
                           lista_textura.push_back(tex);


                       }
                }

                vector<Node> backgrounds_xml=XML_parser::get_etiquetas_by_Name("image");
                if(backgrounds_xml.size()>0){
                     vector<Node>::iterator it_backg;
                     for(it_backg=backgrounds_xml.begin();it_backg!=backgrounds_xml.end();it_backg++){
                           Node n=*it_backg;
                           string source=n.get_parameter_data("source");
                           if(source!=""){
                               Texture t=Texture(source,renderer);
                               lista_textura.push_back(t);
                               if(id_texture_background==-1){
                                    id_texture_background=lista_textura.size()-1;

                               }
                           }
                     }

                }
                Texture_manager::set_textures(lista_textura);



             //  extraemos data de las layers
               vector<Node> layers=XML_parser::get_etiquetas_by_Name("layer");
               vector<Node>::iterator it_l;

               Tile tempMap[heigthMap][widthMap];
               int layer_index=0;


               for(it_l=layers.begin();it_l!=layers.end();it_l++){
                  Node n=*it_l;
                  bool visible_layer=true;

                  string layer_dat=n.get_parameter_data("mapa");
                  string visibleLayer_s=n.get_parameter_data("visible");
                  if(visibleLayer_s=="0"){
                    visible_layer=false;
                  }


                  if(layer_dat!="" && visible_layer==true){
                      int first_index=0;
                      int second_index=0;
                      string temp_val="";

                      for(int j=0;j<layer_dat.length();j++){
                          const unsigned char c=layer_dat[j];
                          if(c==';'){
                             if(temp_val!="" && temp_val!=" "){

                               int val=Math_G::StrToInt(temp_val);

                               if(tempMap[second_index][first_index].is_valid()==false){
                                  int ytl=tileH*second_index;
                                  int xtl=tileW*first_index;
                                  Tile tl=Tile(tileW,tileH,xtl,ytl);
                                  tempMap[second_index][first_index]=tl;
                               }

                               if(val>0){
                                    Sprite spr=Texture_manager::get_sprite(val);
                                    tempMap[second_index][first_index].add_sprite(layer_index,spr);
                                }
                                else{
                                     Sprite spr=Sprite();
                                    tempMap[second_index][first_index].add_sprite(layer_index,spr);
                                }


                               temp_val="";
                            }
                            second_index+=1;
                            first_index=0;
                          }
                          else if(c==','){
                            if(temp_val!="" && temp_val!=" "){
                                int val=Math_G::StrToInt(temp_val);

                                if(tempMap[second_index][first_index].is_valid()==false){
                                  int ytl=tileH*second_index;
                                  int xtl=tileW*first_index;
                                  Tile tl=Tile(tileW,tileH,xtl,ytl);
                                  tempMap[second_index][first_index]=tl;
                               }


                                if(val>0){
                                    Sprite spr=Texture_manager::get_sprite(val);
                                    tempMap[second_index][first_index].add_sprite(layer_index,spr);
                                }
                                else{
                                    Sprite spr=Sprite();
                                    tempMap[second_index][first_index].add_sprite(layer_index,spr);

                                }

                                temp_val="";
                            }
                            first_index+=1;
                          }
                          else{
                            temp_val+=c;

                              }
                          }  //fin de for de lectura de gids del mapa
                            layer_index+=1;
                        }


                    }  //fin del for de layer



                   for(int j=0;j<heigthMap;j++){
                       for(int k=0;k<widthMap;k++){
                            tls.push_back(tempMap[j][k]);


                            }

                   }


                   //leemos objetos de la escena

                   vector<Node> objects_xml=XML_parser::get_etiquetas_by_Name("object");
                   vector<Node> ::iterator it_obj;

                   for(it_obj=objects_xml.begin();it_obj!=objects_xml.end();it_obj++){
                        Node n=*it_obj;
                        int object_type=-1;
                        string x_s=n.get_parameter_data("x");
                        string y_s=n.get_parameter_data("y");
                        string tag="default";
                        string script_data="";

                        if(x_s!="" && y_s!=""){

                            string tag=n.get_parameter_data("Tag");
                            string animator_src=n.get_parameter_data("Animator_Source");
                            string name_ob=n.get_parameter_data("Name_Obj");

                            if(tag==""){
                                tag="default";
                            }
                            else{
                                string temp_tg="";
                                for(int i_tgg=0;i_tgg<tag.length();i_tgg++){
                                     unsigned char c=tag[i_tgg];
                                      if(c!=';'){
                                           temp_tg+=c;
                                       }
                                      else{
                                             temp_tg+=' ';
                                         }

                                }
                                tag=temp_tg;

                            }
                            if(name_ob==""){


                                name_ob="default";

                                vector<string>::iterator it;
                                int coincidencias=0;
                                bool change_name=false;
                                for(it=names_list.begin();it!=names_list.end();it++){
                                        string n=*it;
                                        n=n.substr(0,name_ob.length());
                                        if(n==name_ob){
                                            change_name=true;
                                            coincidencias++;

                                        }
                                }
                                if(change_name==true){
                                    string next_id=(coincidencias<10)?"0"+to_string(coincidencias):to_string(coincidencias);
                                    name_ob=name_ob+next_id;
                                }

                                names_list.push_back(name_ob);
                            }
                            else{
                                string temp_nm="";
                                for(int i_tnm=0;i_tnm<name_ob.length();i_tnm++){
                                     unsigned char c=name_ob[i_tnm];
                                      if(c!=';'){
                                           temp_nm+=c;
                                       }
                                      else{
                                             temp_nm+=' ';
                                         }


                                }
                                name_ob=temp_nm;

                                vector<string>::iterator it;
                                int coincidencias=0;
                                bool change_name=false;
                                for(it=names_list.begin();it!=names_list.end();it++){
                                        string n=*it;
                                        n=n.substr(0,name_ob.length());
                                        if(n==name_ob){
                                            change_name=true;
                                            coincidencias++;

                                        }
                                }
                                if(change_name==true){
                                    string next_id=(coincidencias<10)?"0"+to_string(coincidencias):to_string(coincidencias);
                                    name_ob=name_ob+next_id;
                                }

                                names_list.push_back(name_ob);
                            }


                            float x=0.0f;
                            float y=0.0f;
                            x=Math_G::StrToFloat(x_s);
                            y=Math_G::StrToFloat(y_s);



                            string gid_s=n.get_parameter_data("gid");


                            if(gid_s!=""){

                                string temp_tag="";
                                if(tag.size()>2){
                                    temp_tag=tag.substr(0,2);
                                }
                                if(temp_tag=="UI"){


                                   int gid_obj=Math_G::StrToInt(gid_s);

                                   x_s=n.get_parameter_data("width");
                                   y_s=n.get_parameter_data("height");
                                   if(x_s!="" && y_s!=""){

                                       float w_obj=0.0f;
                                       float h_obj=0.0f;
                                       w_obj=Math_G::StrToFloat(x_s);
                                       h_obj=Math_G::StrToFloat(y_s);


                                        SDL_Rect rect_obj;
                                        rect_obj.x=round(x*w_scale);
                                        rect_obj.y=round(y*w_scale);
                                        rect_obj.w=round(w_obj*w_scale);
                                        rect_obj.h=round(h_obj*w_scale);
                                        rect_obj.y=rect_obj.y*-1;

                                        Sprite spr=Sprite();



                                             string active_str=n.get_parameter_data("Active");
                                             bool active_ob=false;
                                             if(active_str=="True"){
                                                active_ob=true;
                                             }
                                             int layer_ui=0;
                                             string layer_s=n.get_parameter_data("UI_layer");
                                             string UI_tp_s=n.get_parameter_data("UI_type");
                                             int UI_type=UI_TYPES::IMAGE;

                                             if(layer_s!=""){
                                                  layer_ui=Math_G::StrToInt(layer_s);
                                             }
                                             if(UI_tp_s!=""){
                                                if(UI_tp_s=="button"){
                                                    UI_type=UI_TYPES::BUTTON;
                                                }

                                                else if(UI_tp_s=="image"){
                                                    UI_type=UI_TYPES::IMAGE;
                                                }
                                                else if(UI_tp_s=="label"){
                                                    UI_type=UI_TYPES::LABEL;
                                                }
                                                else if(UI_tp_s=="text"){
                                                    UI_type=UI_TYPES::TEXTFIELD;
                                                }
                                                else if(UI_tp_s=="radio"){
                                                    UI_type=UI_TYPES::RADIOBUTTON;
                                                }
                                                else if(UI_tp_s=="check"){
                                                    UI_type=UI_TYPES::CHECKBUTTON;
                                                }
                                                else if(UI_tp_s=="scroll"){
                                                     UI_type=UI_TYPES::SCROLLBAR;

                                                }
                                                else if(UI_tp_s=="bar"){
                                                     UI_type=UI_TYPES::VALUEBAR;

                                                }

                                             }

                                             vector<Icon_UI> lista_iconos;
                                             string icons_s=n.get_parameter_data("iconos");
                                             bool has_icons=false;
                                             if(icons_s!="" && UI_type!=UI_TYPES::LABEL){

                                                    string temp="";





                                                for(unsigned int ii=0;ii<icons_s.length();ii++){
                                                    unsigned char c=icons_s[ii];
                                                    if(c==','){
                                                        if(temp!=""){
                                                            int id=Math_G::StrToInt(temp);

                                                            SDL_Rect temp_rect;

                                                            vector<int> indexs=Texture_manager::get_indexs(&temp_rect,id);

                                                            Icon_UI icono=Icon_UI(temp_rect,indexs);
                                                            lista_iconos.push_back(icono);
                                                            temp="";
                                                        }
                                                    }
                                                    else{
                                                        temp+=c;
                                                    }
                                                }
                                                if(temp!=""){
                                                    int id=Math_G::StrToInt(temp);
                                                    SDL_Rect temp_rect;
                                                    vector<int> indexs=Texture_manager::get_indexs(&temp_rect,id);
                                                    Icon_UI icono=Icon_UI(temp_rect,indexs);
                                                    lista_iconos.push_back(icono);
                                                    temp="";
                                                }
                                                if(lista_iconos.size()>=3){
                                                    has_icons=true;
                                                }
                                             }

                                             string font_name_s=n.get_parameter_data("Font_Name");
                                             string font_s=n.get_parameter_data("Font_Src");
                                             string text_s=n.get_parameter_data("Text_Value");
                                             string text_size=n.get_parameter_data("Text_Size");
                                             string text_color_s=n.get_parameter_data("Text_Color");
                                            string text_offset_s=n.get_parameter_data("Text_Offset");
                                            string text_colorClick_s=n.get_parameter_data("Text_Color_Click");
                                            string text_colorOver_s=n.get_parameter_data("Text_Color_Over");
                                            string min_characters_s=n.get_parameter_data("Min_Characters");


                                           vector<SDL_Color> text_colors;
                                           Data_Text text_d=Data_Text();
                                           bool has_text=false;
                                           int min_characters=-1;
                                          if(font_s!="" && font_name_s!=""){
                                                  string temp_text="";
                                                  for(unsigned int i_tt=0;i_tt<text_s.length();i_tt++){
                                                    unsigned char c=text_s[i_tt];
                                                    if(c!=';'){
                                                        temp_text+=c;
                                                    }
                                                    else{
                                                        temp_text+=' ';
                                                    }

                                                  }
                                                  text_s=temp_text;
                                                  font_s=string(SDL_GetBasePath())+"assets/fonts/"+font_s;
                                                  int size_font=12;


                                                  SDL_Color colr_font={255,255,255,255};
                                                  SDL_Color colr_click={255,255,255,255};
                                                  SDL_Color colr_over={255,255,255,255};


                                                  vector<string> colores_ui;
                                                  colores_ui.push_back(text_color_s);
                                                  colores_ui.push_back(text_colorOver_s);
                                                  colores_ui.push_back(text_colorClick_s);
                                                  Vector2 offset_font=Vector2(0,0);
                                                  int index_color=0;
                                                  vector<string>::iterator it_colors;

                                                  for(it_colors=colores_ui.begin();it_colors!=colores_ui.end();it_colors++){
                                                     string colr_val=*it_colors;
                                                     int r=255;
                                                     int g=255;
                                                     int b=255;
                                                     int a=255;

                                                     if(colr_val!=""){

                                                         string temp="";
                                                          int index=0;
                                                          for(int i=0;i<colr_val.length();i++){
                                                              unsigned char c=colr_val[i];
                                                              if(c==','){
                                                                  if(index==0){
                                                                    r=Math_G::StrToInt(temp);

                                                                 }
                                                                  else if(index==1){
                                                                      g=Math_G::StrToInt(temp);
                                                                 }
                                                                   else if(index==2){
                                                                    b=Math_G::StrToInt(temp);
                                                                 }

                                                                 index+=1;
                                                                 temp="";
                                                              }
                                                              else{
                                                                     temp+=c;
                                                              }
                                                        }
                                                        if(temp!=""){
                                                           a=Math_G::StrToInt(temp);

                                                        }
                                                    }
                                                    if(index_color==0){
                                                        colr_font.r=r;
                                                        colr_font.g=g;
                                                        colr_font.b=b;
                                                        colr_font.a=a;
                                                    }
                                                    else if(index_color==1){
                                                         colr_over.r=r;
                                                         colr_over.g=g;
                                                         colr_over.b=b;
                                                         colr_over.a=a;
                                                    }
                                                    else{
                                                        colr_click.r=r;
                                                        colr_click.g=g;
                                                        colr_click.b=b;
                                                        colr_click.a=a;
                                                    }
                                                    index_color++;
                                                  }

                                                  if(text_size!=""){
                                                    size_font=Math_G::StrToInt(text_size);
                                                  }

                                                  if(text_offset_s!=""){
                                                          string temp="";
                                                          for(int i=0;i<text_offset_s.length();i++){
                                                             unsigned char c=text_offset_s[i];
                                                             if(c==','){
                                                                 offset_font.x=Math_G::StrToInt(temp)*w_scale;
                                                                 temp="";
                                                             }
                                                             else{
                                                                temp+=c;
                                                             }
                                                          }
                                                          if(temp!=""){
                                                            offset_font.y=Math_G::StrToInt(temp)*w_scale;
                                                          }
                                                  }

                                                  Font_Manager::add_font(font_name_s,font_s,size_font);
                                                  text_colors.push_back(colr_font);
                                                  text_colors.push_back(colr_over);
                                                  text_colors.push_back(colr_click);
                                                  if(min_characters_s!=""){
                                                      min_characters=Math_G::StrToInt(min_characters_s);

                                                  }
                                                 has_text=true;
                                                 text_d=Data_Text(text_s,font_name_s,size_font,colr_font,offset_font,min_characters);

                                             }


                                           string parent_s=n.get_parameter_data("Parent");
                                           int w_spr=0;
                                           int h_spr=0;


                                           string parent_name="";
                                           if(parent_s!=""){
                                                string temp="";

                                                for(int ic=0;ic<parent_s.length();ic++){
                                                    unsigned char c=parent_s[ic];
                                                     if(c==';'){
                                                        temp+=' ';
                                                    }
                                                    else{
                                                        temp+=c;
                                                    }
                                                }
                                                parent_name=temp;
                                           }
                                           if(UI_type!=UI_TYPES::LABEL){

                                                  bool visible=true;
                                                  string visible_s=n.get_parameter_data("Visible");
                                                  if(visible_s=="False"){
                                                    visible=false;
                                                  }
                                                  int layer_draw=0;
                                                  string layer_draw_s=n.get_parameter_data("Layer_Render");
                                                  if(layer_draw_s!=""){
                                                       layer_draw=Math_G::StrToInt(layer_draw_s);
                                                       if(layer_draw<0){
                                                           layer_draw=0;
                                                       }
                                                  }
                                                  spr=Texture_manager::get_sprite(gid_obj);
                                                  Render render_obj=Render(spr,visible,"",layer_draw);
                                                  Component::add_comp(name_ob,render_obj);
                                                  SDL_Rect r_spr=spr.get_rect_sprite();
                                                  w_spr=r_spr.w;
                                                  h_spr=r_spr.h;
                                             }
                                             else{

                                                  int layer_draw=0;
                                                  string layer_draw_s=n.get_parameter_data("Layer_Render");
                                                  if(layer_draw_s!=""){
                                                       layer_draw=Math_G::StrToInt(layer_draw_s);
                                                       if(layer_draw<0){
                                                           layer_draw=0;
                                                       }
                                                  }
                                                  spr=Sprite();
                                                  Render render_obj=Render(spr,false,"",layer_draw);
                                                  Component::add_comp(name_ob,render_obj);

                                             }

                                            Game_Object ui_temp=Game_Object(rect_obj,active_ob,name_ob,tag,parent_name,UI_type);


                                            Transform trans=Transform();
                                            trans.Set_Position(Vector2(rect_obj.x,rect_obj.y+rect_obj.h));
                                            Component::add_comp(name_ob,trans);

                                            vector<string>sfx;
                                            string sfx_s=n.get_parameter_data("SFX_Source");
                                            string temp_sfx="";
                                            for(int i_sfx=0;i_sfx<sfx_s.length();i_sfx++){
                                                   unsigned char c=sfx_s[i_sfx];
                                                   if(c!=' '){
                                                        if(c==','){
                                                             sfx.push_back(temp_sfx);
                                                             temp_sfx="";
                                                        }
                                                        else{
                                                            temp_sfx+=c;
                                                        }
                                                   }
                                            }
                                            if(temp_sfx!=""){
                                                  sfx.push_back(temp_sfx);
                                            }

                                           string On_select="";
                                           string On_Click="";
                                           map<string,string> params=n.get_parametros();
                                           map<string,string>::iterator it_evs;
                                            for(it_evs=params.begin();it_evs!=params.end();it_evs++){
                                                 string p=it_evs->first;
                                                 string temp_p="";
                                                 for(unsigned int ij=0;ij<p.length();ij++){
                                                    unsigned char c=p[ij];
                                                    if(temp_p=="On_Select"){


                                                        On_select=On_select+it_evs->second+"|";

                                                        ij=p.length();
                                                        temp_p="";
                                                    }
                                                    else if(temp_p=="On_Click"){
                                                        On_Click=On_Click+it_evs->second+"|";
                                                         ij=p.length();
                                                         temp_p="";
                                                    }
                                                    else{
                                                        temp_p+=c;
                                                    }

                                                  }

                                              }






                                            if(UI_type==LABEL && has_text){



                                             float espera_ev=0.0f;
                                             string espera_s=n.get_parameter_data("Espera_Event");
                                             if(espera_s!=""){
                                                   espera_ev=Math_G::StrToFloat(espera_s);
                                               }

                                             string sel_object=n.get_parameter_data("Activate_OnSelected");
                                             SDL_Color select_color={255,255,255,255};
                                             string sel_color_s=n.get_parameter_data("Text_Color_Select");
                                             if(sel_color_s!=""){
                                                  string temp_cl="";
                                                  int index_cl=0;
                                                  for(int i_cl=0;i_cl<sel_color_s.length();i_cl++){
                                                       unsigned char c=sel_color_s[i_cl];
                                                       if(c==','){
                                                           if(temp_cl!=""){
                                                              int val=Math_G::StrToInt(temp_cl);
                                                              if(val>=0){
                                                                  if(index_cl==0){
                                                                     select_color.r=val;
                                                                  }
                                                                  else if(index_cl==1){
                                                                       select_color.g=val;
                                                                  }
                                                                  else if(index_cl==2){
                                                                      select_color.b=val;
                                                                  }
                                                                  else if(index_cl==3){
                                                                       select_color.a=val;
                                                                  }
                                                              }
                                                           }
                                                           temp_cl="";
                                                           index_cl+=1;
                                                       }
                                                       else{
                                                           temp_cl+=c;
                                                       }
                                                  }
                                                if(temp_cl!="" && index_cl==3){
                                                       int val=Math_G::StrToInt(temp_cl);
                                                       select_color.a=val;
                                                }
                                             }


                                                 Label label=Label(espera_ev,sel_object,select_color,text_d,sfx);
                                                 if(On_select!=""){
                                                    label.set_eventsSelected(On_select);
                                                 }
                                                 if(On_Click!=""){
                                                    label.set_eventsClick(On_Click);
                                                 }

                                                 Component::add_comp(name_ob,label);
                                            }
                                            else if(UI_type==BUTTON ){
                                               Button btn=Button();

                                               float espera_ev=0.0f;
                                               string espera_s=n.get_parameter_data("Espera_Event");
                                               if(espera_s!=""){
                                                   espera_ev=Math_G::StrToFloat(espera_s);
                                               }




                                               vector<Vector2> dims;
                                               if(has_icons){

                                                  if(has_text){
                                                      btn=Button(espera_ev,lista_iconos,text_colors,dims,sfx);
                                                      btn.add_text(text_d);
                                                  }
                                                  else{
                                                    vector<SDL_Color> colors;
                                                      btn=Button(espera_ev,lista_iconos,colors,dims,sfx);

                                                  }
                                               }
                                               else{

                                                vector<Icon_UI> icons;
                                                if(has_text){
                                                      btn=Button(espera_ev,icons,text_colors,dims,sfx);
                                                      btn.add_text(text_d);
                                                  }
                                                  else{
                                                    vector<SDL_Color> colors;
                                                      btn=Button(espera_ev,icons,colors,dims,sfx);

                                                  }
                                               }


                                               if(On_select!=""){
                                                   btn.set_eventsSelected(On_select);

                                               }
                                               if(On_Click!=""){
                                                   btn.set_eventsClick(On_Click);
                                               }

                                               Component::add_comp(name_ob,btn);

                                            }
                                            else if(UI_type==RADIOBUTTON){
                                                RadioButton radio=RadioButton();
                                                string radioId=n.get_parameter_data("Radio_Id");
                                                string radioVal=n.get_parameter_data("Radio_Value");
                                                string select_s=n.get_parameter_data("Selected");
                                                bool selected=false;
                                                if(select_s=="True"){
                                                    selected=true;

                                                }
                                                if(has_icons){




                                                      radio=RadioButton(lista_iconos,selected,radioVal,radioId,sfx);
                                                      if(has_text){
                                                        radio.set_text(text_d);
                                                      }


                                                      if(On_select!=""){
                                                           radio.set_eventsSelected(On_select);

                                                      }
                                                      if(On_Click!=""){
                                                            radio.set_eventsClick(On_Click);
                                                      }
                                                      Component::add_comp(name_ob,radio);

                                               }

                                            }
                                           else if(UI_type==CHECKBUTTON){
                                                CheckButton check=CheckButton();

                                                string check_val=n.get_parameter_data("Check_Value");
                                                string select_s=n.get_parameter_data("Selected");
                                                bool selected=false;
                                                if(select_s=="True"){
                                                    selected=true;
                                                }
                                                if(has_icons){

                                                  if(has_text){
                                                      check=CheckButton(lista_iconos,selected,check_val,text_d,sfx);
                                                       Component::add_comp(name_ob,check);
                                                  }
                                               }
                                            }
                                            else if(UI_type==IMAGE){
                                                Image_UI img=Image_UI(sfx);

                                                if(has_text){

                                                    img.add_text(text_d);
                                                }
                                                string anim_s=n.get_parameter_data("Animator_Source");
                                                if(anim_s!=""){
                                                     Animator anim=Animator(anim_s,w_spr,h_spr);
                                                     Component::add_comp(name_ob,anim);
                                                }
                                                if(On_select!=""){
                                                   img.set_eventsSelected(On_select);

                                               }
                                               if(On_Click!=""){
                                                   img.set_eventsClick(On_Click);
                                               }

                                                Component::add_comp(name_ob,img);
                                            }
                                            else if(UI_type==SCROLLBAR){

                                                    ScrollBar scroll=ScrollBar();
                                                    bool reset_desplz=true;
                                                    bool use_rastro=false;
                                                    SDL_Color rastro_color={255,255,255,255};

                                                    string sent_s=n.get_parameter_data("Sentido");
                                                    string bg_rect=n.get_parameter_data("BG_Rect");
                                                   string reset_desplz_s=n.get_parameter_data("Reset_Desplz");

                                                   string rastro_s=n.get_parameter_data("Use_Rastro");
                                                   if(rastro_s=="True"){
                                                       use_rastro=true;
                                                       string rastro_color_s=n.get_parameter_data("Rastro_Color");

                                                       if(rastro_color_s!=""){
                                                              int index_r=0;
                                                              string temprastro="";
                                                               for(int i_r=0;i_r<rastro_color_s.length();i_r++){
                                                                     unsigned char c=rastro_color_s[i_r];
                                                                     if(c==','){

                                                                        if(index_r==0){
                                                                            rastro_color.r=Math_G::StrToInt(temprastro);
                                                                        }
                                                                        else if(index_r==1){
                                                                              rastro_color.g=Math_G::StrToInt(temprastro);
                                                                        }
                                                                         else if(index_r==2){
                                                                              rastro_color.b=Math_G::StrToInt(temprastro);
                                                                         }
                                                                      index_r++;
                                                                       temprastro="";
                                                                  }
                                                                  else{
                                                                       temprastro+=c;
                                                                      }
                                                              }
                                                            if(temprastro!=""){
                                                                 rastro_color.a=Math_G::StrToInt(temprastro);
                                                            }
                                                       }
                                                       else{
                                                           use_rastro=false;
                                                       }
                                                   }
                                                   if(reset_desplz_s=="False"){
                                                       reset_desplz=false;
                                                   }

                                                    if(has_icons && sent_s!="" && bg_rect!=""){
                                                        int sentido=Math_G::StrToInt(sent_s);

                                                        int index1=0;
                                                        string temp_rect="";
                                                        SDL_Rect bg_r;
                                                        for(int j=0;j<bg_rect.length();j++){
                                                            unsigned char c=bg_rect[j];
                                                            if(c==','){
                                                                if(index1==0){
                                                                    bg_r.x=(Math_G::StrToFloat(temp_rect)*w_scale);
                                                                }
                                                                else if(index1==1){
                                                                    bg_r.y=(Math_G::StrToFloat(temp_rect)*w_scale);
                                                                }
                                                                else if(index1==2){
                                                                    bg_r.w=(Math_G::StrToFloat(temp_rect)*w_scale);
                                                                }
                                                                index1++;
                                                                temp_rect="";
                                                            }
                                                            else{
                                                                temp_rect+=c;
                                                            }
                                                        }
                                                        if(temp_rect!=""){
                                                            bg_r.h=(Math_G::StrToFloat(temp_rect)*w_scale);
                                                        }




                                                        int index2=0;
                                                        string bg_s=n.get_parameter_data("BG_Color");
                                                        SDL_Color bg={255,255,255,255};
                                                        if(bg_s!=""){
                                                                string tempbg="";
                                                               for(int k=0;k<bg_s.length();k++){
                                                                     unsigned char c=bg_s[k];
                                                                     if(c==','){
                                                                        if(index2==0){
                                                                            bg.r=Math_G::StrToInt(tempbg);
                                                                        }
                                                                        else if(index2==1){
                                                                              bg.g=Math_G::StrToInt(tempbg);;
                                                                        }
                                                                         else if(index2==2){
                                                                              bg.b=Math_G::StrToInt(tempbg);;
                                                                         }
                                                                      index2++;
                                                                       tempbg="";
                                                                  }
                                                                  else{
                                                                       tempbg+=c;
                                                                      }
                                                              }
                                                            if(tempbg!=""){
                                                                 bg.a=Math_G::StrToInt(tempbg);
                                                            }
                                                        }
                                                        int index3=0;
                                                        string fg_s=n.get_parameter_data("FG_Color");
                                                        SDL_Color fg={0,0,0,255};


                                                        if(fg_s!=""){
                                                              string tempfg="";
                                                               for(int l=0;l<fg_s.length();l++){
                                                                     unsigned char c=fg_s[l];
                                                                     if(c==','){
                                                                        if(index3==0){
                                                                            fg.r=Math_G::StrToInt(tempfg);
                                                                        }
                                                                        else if(index3==1){
                                                                              fg.g=Math_G::StrToInt(tempfg);;
                                                                        }
                                                                         else if(index3==2){
                                                                              fg.b=Math_G::StrToInt(tempfg);;
                                                                         }
                                                                      index3++;
                                                                       tempfg="";
                                                                  }
                                                                  else{
                                                                       tempfg+=c;
                                                                      }
                                                              }
                                                            if(tempfg!=""){
                                                                 fg.a=Math_G::StrToInt(tempfg);
                                                            }
                                                        }

                                                        Vector2 ref_dims=Vector2(rect_obj.w,rect_obj.h);
                                                        bg_r.y=bg_r.y-ref_dims.y;
                                                        scroll=ScrollBar(reset_desplz,sentido,bg,fg,lista_iconos,bg_r,ref_dims,sfx);
                                                        if(use_rastro){

                                                             scroll.activate_rastro(rastro_color);
                                                        }
                                                        Component::add_comp(name_ob,scroll);
                                                    }

                                            }
                                            else if(UI_type==VALUEBAR){
                                                string value_s=n.get_parameter_data("Value");
                                                string sent_s=n.get_parameter_data("Sentido");
                                                string bg_color_s=n.get_parameter_data("BG_Color");
                                                string fg_color_s=n.get_parameter_data("FG_Color");
                                                string bg_icon_s=n.get_parameter_data("BG_Image");
                                                string cut_s=n.get_parameter_data("Cut_Sprite");
                                                bool cut_sprite=false;
                                                if(cut_s=="True"){
                                                    cut_sprite=true;
                                                }
                                                bool has_bg=false;
                                                bool has_bgImg=false;
                                                SDL_Color bg={0,0,0,0};
                                                SDL_Color fg={0,0,0,0};
                                                vector<SDL_Color> colors_bg;
                                                Icon_UI icon_bg;
                                                if(bg_color_s!="" || fg_color_s!=""){
                                                    has_bg=true;
                                                    int index1=0;
                                                    int index2=0;
                                                    string temp="";
                                                    for(int bg_i=0;bg_i<bg_color_s.length();bg_i++){
                                                        unsigned char c=bg_color_s[bg_i];
                                                        if(c==','){
                                                            if(index1==0){
                                                                bg.r=Math_G::StrToInt(temp);
                                                            }
                                                            else if(index1==1){
                                                                bg.g=Math_G::StrToInt(temp);
                                                            }
                                                            else if(index1==2){
                                                                bg.b=Math_G::StrToInt(temp);
                                                            }
                                                            temp="";
                                                            index1++;
                                                        }
                                                        else{
                                                            temp+=c;
                                                        }
                                                    }
                                                    if(temp!=""){
                                                        bg.a=Math_G::StrToInt(temp);
                                                        temp="";
                                                    }

                                                    for(int fg_i=0;fg_i<fg_color_s.length();fg_i++){
                                                        unsigned char c=fg_color_s[fg_i];
                                                        if(c==','){
                                                            if(index2==0){
                                                                fg.r=Math_G::StrToInt(temp);
                                                            }
                                                            else if(index2==1){
                                                                fg.g=Math_G::StrToInt(temp);
                                                            }
                                                            else if(index2==2){
                                                                fg.b=Math_G::StrToInt(temp);
                                                            }
                                                            temp="";
                                                            index2++;
                                                        }
                                                        else{
                                                            temp+=c;
                                                        }
                                                    }
                                                    if(temp!=""){
                                                        fg.a=Math_G::StrToInt(temp);
                                                        temp="";
                                                    }
                                                    colors_bg.push_back(bg);
                                                    colors_bg.push_back(fg);
                                                }
                                                if(bg_icon_s!=""){

                                                    string temp="";
                                                    for(int iij=0;iij<bg_icon_s.length();iij++){
                                                        unsigned char c=bg_icon_s[iij];
                                                        temp+=c;

                                                    }
                                                    if(temp!=""){
                                                        int id=Math_G::StrToInt(temp);
                                                        SDL_Rect temp_rect;
                                                        vector<int> indexs=Texture_manager::get_indexs(&temp_rect,id);
                                                        Icon_UI icono=Icon_UI(temp_rect,indexs);
                                                        icon_bg=icono;
                                                        has_bgImg=true;
                                                    }
                                                }


                                                if(value_s!="" && sent_s!=""){
                                                    int sentido=Math_G::StrToInt(sent_s);
                                                    float val=Math_G::StrToFloat(value_s);
                                                    ValueBar bar=ValueBar(sentido,val,lista_iconos,cut_sprite,sfx);
                                                   if(has_text){
                                                      bar.add_text(text_d);
                                                   }
                                                   if(has_bg){
                                                       bar.add_BgColors(colors_bg);
                                                   }
                                                   if(has_bgImg){
                                                       bar.add_iconBg(icon_bg);
                                                   }
                                                   Component::add_comp(name_ob,bar);
                                                }

                                            }


                                             ui_objects.push_back(ui_temp);
                                   }
                                }
                                else if(tag!="Camera" && tag!="Music" && tag!="Sfx"){
                                //tiene sprite
                                   int gid_obj=Math_G::StrToInt(gid_s);

                                   x_s=n.get_parameter_data("width");
                                   y_s=n.get_parameter_data("height");
                                   if(x_s!="" && y_s!=""){

                                       float w_obj=0.0f;
                                       float h_obj=0.0f;
                                       w_obj=Math_G::StrToFloat(x_s);
                                       h_obj=Math_G::StrToFloat(y_s);


                                        SDL_Rect rect_obj;
                                        rect_obj.x=round(x*w_scale);
                                        rect_obj.y=round(y*w_scale);
                                        rect_obj.w=round(w_obj*w_scale);
                                        rect_obj.h=round(h_obj*w_scale);
                                        rect_obj.y=rect_obj.y*-1;

                                        bool visible=true;
                                        string visible_s=n.get_parameter_data("Visible");
                                        string alpha_map_s=n.get_parameter_data("Alpha_Map");

                                        if(visible_s=="False"){
                                             visible=false;
                                        }

                                        Sprite spr=Texture_manager::get_sprite(gid_obj);

                                        int layer_draw=0;
                                        string layer_draw_s=n.get_parameter_data("Layer_Render");
                                         if(layer_draw_s!=""){
                                              layer_draw=Math_G::StrToInt(layer_draw_s);
                                              if(layer_draw<0){
                                                           layer_draw=0;
                                                }
                                        }
                                        Render render_obj=Render(spr,visible,alpha_map_s,layer_draw);
                                        Component::add_comp(name_ob,render_obj);

                                       //agregamos parametros segun tag
                                            map<string,string> params=n.get_parametros();
                                            map<string,string>::iterator it_scr;
                                            for(it_scr=params.begin();it_scr!=params.end();it_scr++){
                                                string p=it_scr->first;
                                                string temp_p="";
                                                for(int ij=0;ij<p.length();ij++){
                                                    unsigned char c=p[ij];
                                                    if(c==' ' || c=='_'){
                                                        if(temp_p==tag){
                                                            script_data=script_data+it_scr->first+":"+it_scr->second+"|";
                                                        }
                                                        ij=p.length();
                                                        temp_p="";
                                                    }
                                                    else{
                                                        temp_p+=c;
                                                    }

                                                  }

                                                }

                                        string active_str=n.get_parameter_data("Active");
                                        bool active_ob=false;
                                        if(active_str=="True"){
                                                active_ob=true;
                                        }


                                        string parent_s=n.get_parameter_data("Parent");
                                        string parent_name="";
                                        if(parent_s!=""){
                                            string temp="";
                                            for(int ii=0;ii<parent_s.length();ii++){
                                                unsigned char c=parent_s[ii];
                                                 if(c==';'){
                                                        temp+=' ';
                                                    }
                                                else{
                                                    temp+=c;
                                                }
                                            }
                                            parent_name=temp;
                                        }
                                        Game_Object g=Game_Object(rect_obj,active_ob,name_ob,tag,parent_name);

                                        if(animator_src!=""){

                                            SDL_Rect r_sprite=spr.get_rect_sprite();
                                            Animator anim=Animator(animator_src,r_sprite.w,r_sprite.h);
                                            Component::add_comp(name_ob,anim);

                                        }
                                        Transform trans=Transform();
                                        trans.Set_Position(Vector2(rect_obj.x,rect_obj.y+rect_obj.h));
                                        Component::add_comp(name_ob,trans);

                                        if(tag=="Player"){

                                            bool player_comp=true;
                                            if(n.get_parameter_data("Player_Comp")=="False"){
                                                  player_comp=false;
                                            }
                                            if(player_comp){
                                                Player playr=Player(name_ob,trans.Get_Position(),script_data);
                                                Component::add_comp(name_ob,playr);
                                            }


                                             map<string,string> data_pUI;
                                             map<string,string> params_nodo=n.get_parametros();
                                             map<string,string>::iterator it_pr;
                                             for(it_pr=params_nodo.begin();it_pr!=params_nodo.end();it_pr++){
                                                   string id_p=it_pr->first;
                                                   id_p=id_p.substr(0,9);
                                                   if(id_p=="Player_UI"){

                                                      data_pUI[it_pr->first]=it_pr->second;
                                                   }
                                             }

                                            Player_UI p_UI=Player_UI(data_pUI);
                                            if(p_UI.is_correct()){

                                                 Component::add_comp(name_ob,p_UI);
                                            }


                                        }
                                        else if(tag=="Proyectil"){
                                             map<string,string> data_proyectil;
                                             map<string,string> params_nodo=n.get_parametros();
                                             map<string,string>::iterator it_pr;
                                             for(it_pr=params_nodo.begin();it_pr!=params_nodo.end();it_pr++){
                                                   string id_p=it_pr->first;
                                                   id_p=id_p.substr(0,9);
                                                   if(id_p=="Proyectil"){

                                                      data_proyectil[it_pr->first]=it_pr->second;
                                                   }
                                             }



                                              Proyectil proy(data_proyectil);
                                              Component::add_comp(name_ob,proy);
                                        }
                                        else if(tag=="Enemy"){
                                             map<string,string> data_enemy;
                                             map<string,string> params_nodo=n.get_parametros();
                                             map<string,string>::iterator it_enem;
                                             for(it_enem=params_nodo.begin();it_enem!=params_nodo.end();it_enem++){
                                                   string id_p=it_enem->first;
                                                   id_p=id_p.substr(0,5);
                                                   if(id_p=="Enemy"){

                                                      data_enemy[it_enem->first]=it_enem->second;
                                                   }
                                             }
                                             Enemy enem=Enemy(data_enemy);
                                             Component::add_comp(name_ob,enem);
                                        }




                                        string collider_type_s=n.get_parameter_data("Collider_Type");
                                        string collider_porcent_s=n.get_parameter_data("Collider_Porcent");
                                        string collider_off=n.get_parameter_data("Collider_Offset");

                                        float porcentaje=1.0f;
                                        Vector2 collider_Offset=Vector2(0,0);
                                        if(collider_off!=""){
                                                string temp_val="";
                                                bool x_comp=false;
                                                for(int co=0;co<collider_off.length();co++){
                                                    unsigned char c=collider_off[co];
                                                    if(c==','){
                                                        if(temp_val!=""){
                                                             float val=Math_G::StrToFloat(temp_val);
                                                             collider_Offset.x=val;
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
                                                    collider_Offset.y=val;
                                                     temp_val="";


                                              }
                                        }
                                        if(collider_porcent_s!=""){
                                            porcentaje=Math_G::StrToFloat(collider_porcent_s);

                                        }


                                        Figure f=Figure();
                                        if(collider_type_s!=""){
                                           if(collider_type_s=="circle"){

                                              float radio=(rect_obj.w>rect_obj.h)?(rect_obj.w*0.5f)*porcentaje:(rect_obj.h*0.5f)*porcentaje;
                                              f=Figure(radio,collider_Offset);
                                           }
                                           else if(collider_type_s=="square"){
                                               Vector2 dim=Vector2(0,0);
                                               dim.x=rect_obj.w*porcentaje;
                                               dim.y=rect_obj.h*porcentaje;
                                               f=Figure(dim,collider_Offset);
                                           }
                                           else if(collider_type_s=="polygon"){
                                               string point_s=n.get_parameter_data("Points");
                                               Vector2 temp_point=Vector2(0,0);
                                               string temp_p_s="";
                                               vector<Vector2> pts;
                                               Vector2 offset=Vector2(0,0);
                                               for(int d=0;d<point_s.length();d++){
                                                   unsigned char c_pt=point_s[d];
                                                   if(c_pt==','){
                                                       temp_point.x=Math_G::StrToFloat(temp_p_s)*w_scale;
                                                       temp_p_s=="";
                                                   }
                                                   else if(c_pt==';'){
                                                        temp_point.y=Math_G::StrToFloat(temp_p_s)*w_scale;
                                                        temp_p_s=="";


                                                        if(temp_point.x<offset.x){
                                                            offset.x=temp_point.x;
                                                        }
                                                        if(temp_point.y<offset.y){
                                                            offset.y=temp_point.y;
                                                        }
                                                        temp_point.y=temp_point.y*-1;

                                                        pts.push_back(temp_point);
                                                        temp_point=Vector2(0,0);
                                                   }
                                                   else{
                                                       temp_p_s+=c_pt;
                                                   }
                                               }
                                               if(pts.size()>=3){
                                                    offset.y=offset.y*-1;
                                                    f=Figure(pts,offset);
                                               }
                                           }
                                        }
                                        if(f.get_type()!=-1){
                                            //agregamos collider
                                            string trigger_s=n.get_parameter_data("Trigger");
                                            string rb_s=n.get_parameter_data("RigidBody_Mass");
                                            string offset_rot=n.get_parameter_data("Offset_Centro_Rotacion");


                                            float friction=0.0f;
                                            float dinamic_friction=0.0f;
                                            float rebote=0.0f;
                                            int layer_col=0;
                                            Vector2 Offset_rot_center=Vector2(0,0);



                                            if(offset_rot!=""){
                                                string temp_val="";
                                                bool x_comp=false;
                                                for(int r=0;r<offset_rot.length();r++){
                                                    unsigned char c=offset_rot[r];
                                                    if(c==','){
                                                        if(temp_val!=""){
                                                             float val=Math_G::StrToFloat(temp_val);
                                                             Offset_rot_center.x=val;
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
                                                    Offset_rot_center.y=val;
                                                     temp_val="";

                                                }
                                            }


                                            string friction_s=n.get_parameter_data("Static_Friction");
                                            string din_friction_s=n.get_parameter_data("Dinamic_Friction");
                                            string rebote_s=n.get_parameter_data("rebote_factor");
                                            string layer_s=n.get_parameter_data("Physic_Layer");
                                            if(friction_s!=""){
                                               friction=Math_G::StrToFloat(friction_s);
                                            }
                                            if(din_friction_s!=""){
                                                 dinamic_friction=Math_G::StrToFloat(din_friction_s);
                                            }
                                            if(rebote_s!=""){
                                              rebote=Math_G::StrToFloat(rebote_s);
                                            }
                                            if(layer_s!=""){
                                              layer_col=Math_G::StrToInt(layer_s);
                                            }
                                            bool trigger=false;
                                            if(trigger_s=="True"){
                                               trigger=true;
                                            }

                                           Physic_Material material=Physic_Material(friction,dinamic_friction,rebote);
                                           Collider col=Collider(f,material,trigger,layer_col);

                                           col.set_offsetPivot(Offset_rot_center);

                                           Component::add_comp(name_ob,col);

                                            if(rb_s!=""){
                                                //tiene RigidBody
                                                float mass=0.0f;
                                                bool gravity=false;
                                                float max_angle_pendiente=0.0f;
                                                bool coyote=false;
                                                float coyot_t=0.0f;
                                                Vector2 lock_postion=Vector2(0,0);
                                                bool lock_rot=false;
                                                bool is_kinematic=false;
                                                float inertia=1.0f;
                                                float angular_step=0.4f;


                                                string grav_s=n.get_parameter_data("Use_Gravity");
                                                string pendiente_s=n.get_parameter_data("Angle_Pendiente");
                                                string coyote_s=n.get_parameter_data("Use_Coyote");
                                                string coyote_time_s=n.get_parameter_data("Coyote_Time");
                                                string lock_pos_s=n.get_parameter_data("Lock_Position");
                                                string lock_rot_s=n.get_parameter_data("Lock_Rotation");
                                                string  Kinematic_s=n.get_parameter_data("Kinematic");
                                                string Inertia_s=n.get_parameter_data("Inertia");
                                                string angular_step_s=n.get_parameter_data("Angular_Step");

                                                if(grav_s=="True"){
                                                    gravity=true;
                                                }
                                                if(Inertia_s!=""){
                                                    inertia=Math_G::StrToFloat(Inertia_s);
                                                }
                                                if(angular_step_s!=""){
                                                    angular_step=Math_G::StrToFloat(angular_step_s);
                                                }
                                                mass=Math_G::StrToFloat(rb_s);

                                                 if(pendiente_s!=""){
                                                    max_angle_pendiente=Math_G::StrToFloat(pendiente_s);
                                                 }
                                                 if(coyote_s=="True" && coyote_time_s!=""){
                                                    coyote=true;
                                                    coyot_t=Math_G::StrToFloat(coyote_time_s);
                                                 }
                                                 if(Kinematic_s=="True"){
                                                    is_kinematic=true;
                                                 }
                                                 if(lock_rot_s=="True"){
                                                    lock_rot=true;
                                                 }

                                                 if(lock_pos_s!=""){
                                                   string temp_lock="";
                                                   bool x_comp=false;
                                                   for(int lk=0;lk<lock_pos_s.length();lk++){
                                                       unsigned char c=lock_pos_s[lk];
                                                       if(c==','){
                                                            if(temp_lock!=""){
                                                                float val=Math_G::StrToFloat(temp_lock);
                                                                lock_postion.x=val;
                                                                temp_lock="";
                                                                x_comp=true;
                                                            }
                                                       }
                                                       else if(c!=' '){
                                                           temp_lock+=c;
                                                       }
                                                   }
                                                   if(temp_lock!="" && x_comp==true){
                                                       float val=Math_G::StrToFloat(temp_lock);
                                                       lock_postion.y=val;
                                                       temp_lock="";
                                                   }
                                                 }
                                                 RigidBody rb=RigidBody(mass,gravity,max_angle_pendiente,coyote,coyot_t,inertia,angular_step);
                                                 rb.set_kinematic(is_kinematic);
                                                 rb.set_lockPos(lock_postion);
                                                 rb.set_lockRot(lock_rot);


                                                 Component::add_comp(name_ob,rb);

                                            }

                                         }

                                         objs.push_back(g);

                                    }
                                 }
                                 else{

                                    if(tag=="Camera"){
                                        float cam_areaX=0.0f;
                                        float cam_areaY=0.0f;

                                        string cam_areaX_s=n.get_parameter_data("width");
                                        string cam_areaY_s=n.get_parameter_data("height");
                                        string active_cam_s=n.get_parameter_data("Active");
                                        bool active_cam=false;
                                        if(active_cam_s=="True"){

                                            active_cam=true;

                                        }

                                       Vector2 center_cam=Vector2(x*w_scale,-y*w_scale);
                                       Vector2 half_area=Vector2(0,0);
                                        if(cam_areaX_s!="" && cam_areaY_s!=""){

                                           cam_areaX=Math_G::StrToFloat(cam_areaX_s);
                                           cam_areaY=Math_G::StrToFloat(cam_areaY_s);


                                            cam_areaX*=w_scale;
                                            cam_areaY*=w_scale;

                                            half_area=Vector2((cam_areaX*0.5f),(cam_areaY*0.5f));

                                        }

                                        Vector2 screen_sz=Screen::get_size();


                                        cam_areaX=screen_sz.x;
                                        cam_areaY=screen_sz.y;
                                        half_area=Vector2((cam_areaX*0.5f),(cam_areaY*0.5f));

                                        string look_at=n.get_parameter_data("Look_At");
                                        Camera cam=Camera(center_cam,half_area,active_cam,look_at);



                                        if(active_cam==true){

                                            cam_activa=cams.size();
                                        }

                                        cams.push_back(cam);


                                    }
                                    else if(tag=="Sfx"){
                                           string source=n.get_parameter_data("source");
                                           if(source!=""){
                                              source=string(SDL_GetBasePath())+"assets/sfx/"+source;
                                              sfx.push_back(source);
                                           }


                                    }
                                    else if(tag=="Music"){
                                            string source=n.get_parameter_data("source");
                                            if(source!=""){

                                               source=string(SDL_GetBasePath())+"assets/bgm/"+source;
                                               musics.push_back(source);

                                            }
                                    }
                                 }
                            }
                            else{
                                //es collider sin sprite


                                string points_s=n.get_parameter_data("points");
                                Vector2 offset=Vector2(0,0);
                                string active_str=n.get_parameter_data("Active");
                                bool active_ob=false;
                                if(active_str=="True"){
                                       active_ob=true;
                                  }
                                if(points_s!=""){

                                        string p1="";
                                        string p2="";
                                        int index_p=0;

                                        vector<Vector2> points;

                                       for(int p=0;p<points_s.length();p++){
                                          unsigned char c=points_s[p];
                                          if(c==','){
                                              index_p=1;
                                          }
                                          else if(c==';'){


                                            float px=0.0f;
                                            float py=0.0f;
                                            px=Math_G::StrToFloat(p1);
                                            py=Math_G::StrToFloat(p2);


                                            Vector2 p=Vector2(px*w_scale,py*w_scale);
                                            if(p.x<offset.x){
                                                offset.x=p.x;
                                            }
                                            if(p.y<offset.y){

                                                offset.y=p.y;
                                            }

                                            p.y=p.y*-1;
                                            points.push_back(p);
                                            index_p=0;
                                            p1="";
                                            p2="";
                                          }
                                          else{
                                            if(index_p==0){
                                                p1+=c;
                                            }
                                            else{
                                                p2+=c;
                                            }
                                          }
                                       }


                                   if(points.size()>=3){
                                      offset.y=offset.y*-1;
                                      Figure f=Figure(points,offset);
                                      Vector2 dim=f.get_dim();

                                      SDL_Rect rect_obj;
                                      rect_obj.x=round(x*w_scale);
                                      rect_obj.y=round(y*w_scale)+round(dim.y);
                                      rect_obj.w=round(dim.x);
                                      rect_obj.h=round(dim.y);
                                      rect_obj.y=rect_obj.y-offset.y;


                                      rect_obj.y=rect_obj.y*-1;


                                      Sprite spr=Sprite();

                                     string friction_s=n.get_parameter_data("Static_Friction");
                                      string din_friction_s=n.get_parameter_data("Dinamic_Friction");

                                     string rebote_s=n.get_parameter_data("rebote_factor");
                                     string layer_s=n.get_parameter_data("Physic_Layer");
                                     string trigger_s=n.get_parameter_data("Trigger");

                                     bool trigger=false;
                                     float friction=0.0f;
                                     float dinamic_friction=0.0f;
                                     float rebote=0.0f;
                                     int layer=0;
                                     if(friction_s!=""){
                                        friction=Math_G::StrToFloat(friction_s);
                                     }
                                     if(din_friction_s!=""){
                                        dinamic_friction=Math_G::StrToFloat(din_friction_s);

                                     }
                                     if(rebote_s!=""){
                                        rebote=Math_G::StrToFloat(rebote_s);
                                     }
                                     if(layer_s!=""){
                                        layer=Math_G::StrToInt(layer_s);
                                     }
                                     if(trigger_s=="True"){
                                        trigger=true;
                                     }


                                        string parent_s=n.get_parameter_data("Parent");
                                        string parent_name="";
                                        if(parent_s!=""){
                                            string temp="";
                                            for(int ii=0;ii<parent_s.length();ii++){
                                                unsigned char c=parent_s[ii];
                                                 if(c==';'){
                                                        temp+=' ';
                                                    }
                                                else{
                                                    temp+=c;
                                                }
                                            }
                                            parent_name=temp;
                                        }
                                      Game_Object g=Game_Object(rect_obj,active_ob,name_ob,tag,parent_name);
                                      Transform trans=Transform();
                                      trans.Set_Position(Vector2(rect_obj.x,rect_obj.y+rect_obj.h));
                                      Component::add_comp(name_ob,trans);


                                      Physic_Material material=Physic_Material(friction,dinamic_friction,rebote);
                                      Collider col=Collider(f,material,trigger,layer);
                                      Component::add_comp(name_ob,col);

                                      objs.push_back(g);

                                   }

                                }
                                else{
                                     string w_s=n.get_parameter_data("width");
                                     string h_s=n.get_parameter_data("height");
                                     float w=0.0f;
                                     float h=0.0f;
                                     if(w_s!="" && h_s!=""){

                                            w=Math_G::StrToFloat(w_s);
                                            h=Math_G::StrToFloat(h_s);



                                         Vector2 dim=Vector2(w*w_scale,h*w_scale);
                                         Figure f=Figure(dim,Vector2(0,0));

                                         SDL_Rect rect_obj;
                                         rect_obj.x=round(x*w_scale);
                                         rect_obj.y=round(y*w_scale)+round(dim.y);
                                         rect_obj.w=round(dim.x);
                                         rect_obj.h=round(dim.y);
                                         rect_obj.y=rect_obj.y*-1;
                                         Sprite spr=Sprite();


                                        string friction_s=n.get_parameter_data("Static_Friction");
                                        string din_friction_s=n.get_parameter_data("Dinamic_Friction");
                                        string rebote_s=n.get_parameter_data("rebote_factor");
                                        string layer_s=n.get_parameter_data("Physic_Layer");
                                        string trigger_s=n.get_parameter_data("Trigger");

                                        bool trigger=false;
                                        float friction=0.0f;
                                        float dinamic_friction=0.0f;
                                        float rebote=0.0f;
                                        int layer=0;
                                        if(friction_s!=""){
                                           friction=Math_G::StrToFloat(friction_s);
                                        }
                                        if(rebote_s!=""){
                                           rebote=Math_G::StrToFloat(rebote_s);
                                        }
                                        if(layer_s!=""){
                                           layer=Math_G::StrToInt(layer_s);
                                        }
                                        if(trigger_s=="True"){
                                           trigger=true;
                                        }

                                        if(din_friction_s!=""){
                                            dinamic_friction=Math_G::StrToFloat(din_friction_s);
                                        }
                                        string parent_s=n.get_parameter_data("Parent");
                                        string parent_name="";
                                        if(parent_s!=""){
                                            string temp="";
                                            for(int ii=0;ii<parent_s.length();ii++){
                                                unsigned char c=parent_s[ii];
                                                if(c==';'){
                                                        temp+=' ';
                                                    }
                                                else{
                                                    temp+=c;
                                                }
                                            }
                                            parent_name=temp;
                                        }
                                        Game_Object g=Game_Object(rect_obj,active_ob,name_ob,tag,parent_name);
                                        Transform trans=Transform();
                                        trans.Set_Position(Vector2(rect_obj.x,rect_obj.y+rect_obj.h));
                                        Component::add_comp(name_ob,trans);

                                        Physic_Material material=Physic_Material(friction,dinamic_friction,rebote);
                                        Collider col=Collider(f,material,trigger,layer);
                                        Component::add_comp(name_ob,col);

                                        objs.push_back(g);

                                     }
                                }

                            }
                        }

                   }


                   AudioManager::set_AudioSources(sfx,musics);
                   if(musics.size()>0){
                       AudioManager::Play_Music(0,-1);
                   }


                   Scene escena_nueva=Scene(escena_nombre,objs,id_texture_background,p_begin,p_end);
                   Scene_manager::set_scene(escena_nueva);

               }
               else{
                   cam_activa=-1;
                   printf(("failed to load scene:"+ruta_mapa).c_str());
               }

}

 void Scene_manager::set_scale_world(float val){
    w_scale=val;
 }
 void Scene_manager::set_renderer(SDL_Renderer* rendr){
     renderer=rendr;
 }
 bool Scene_manager::is_loading(){
    return loading_scene;
 }

void Scene_manager::update(){
  on_reset=false;
  if(loading_scene==true && enCola!=""){

      reset();
      vector<Game_Object> objs;
      vector<Game_Object> uis;
      vector<Camera> cams;
      int cam_activa;
      vector<Tile> tls;
      Game_Object::pause_game(false);







      read_escena(enCola,objs,uis,cams,cam_activa,tls);




      Camera::set_camaras(cams,cam_activa);
      Game_Object::set_objs(objs);
      UI_object::set_UI(uis);
      Tile::set_tiles(tls);


    Singleton::On_load();

      escena_actual=enCola;
      enCola="";


     Quatree::make_tree(2,escena_activa);
     Camera::update_tree();
     loading_scene=false;
     Timer::update();
  }
}
 void Scene_manager::reset_esceneObjects(){
     on_reset=true;

 }






bool Scene_manager:: loading_scene=false;
float Scene_manager::w_scale=0.0f;
SDL_Renderer*  Scene_manager::renderer=NULL;
bool Scene_manager::on_reset=false;

Scene::Scene(){
  nombre="default";
  id_texture_background=-1;
}
Scene::Scene(string name,vector<Game_Object> objs,int texture_background, Vector2 p_begin,Vector2 p_end){
  nombre=name;

  id_texture_background=texture_background;
  int index=0;


  vector<Game_Object>::iterator it;

  for(it=objs.begin();it!=objs.end();it++){
      Game_Object g=*it;




      RigidBody *rb=Component::get_rigidBody(g.get_name());
      if(rb!=NULL){


        RigidBodies_scene.push_back(index);
      }
      else{

        Collider* col=Component::get_Collider(g.get_name());
        if(col!=NULL){


            colliders_scene.push_back(index);
        }
        else{

              other_sprites.push_back(index);
           }
      }

      index+=1;
  }





  inicio_mapa=p_begin;
  fin_mapa=p_end;

}
int Scene::get_id_texture_background(){
     return id_texture_background;
}


void Scene::reset(){

    other_sprites.clear();
    RigidBodies_scene.clear();
    colliders_scene.clear();


}
string Scene::get_name(){
   return nombre;
}


vector<int> Scene:: get_colliders_indexs(){
     return colliders_scene;
 }





vector<int> Scene::get_other_sprites_indexs(){
     return other_sprites;
}
vector<int>Scene:: get_rigid_bodies_indexs(){
    return RigidBodies_scene;
}
vector<Vector2> Scene::get_mapa_inicio_fin(){
  vector<Vector2> res;
  res.push_back(inicio_mapa);
  res.push_back(fin_mapa);
  return res;
}


Quatree_Node::Quatree_Node(){
   profund_node=0;
   dimension=Vector2(0,0);
   center=Vector2(0,0);
   validad=false;
   id="";
   root="";
}
Quatree_Node::Quatree_Node(Vector2 dm,Vector2 p, int profund,vector<int> tls,vector<int> objs_i,string n_id,string root_id){

 id=n_id;
 validad=true;
 profund_node=profund;
 dimension=dm;
 center=p;
 has_childs=false;
 root=root_id;


  vector<int> temp_tls;
  map<string,int> temp_objs;
  vector<int> objs_indexs;

   vector<int> ::iterator it;
  vector<int> ::iterator it2;
  vector<int> ::iterator it3;

  vector<Game_Object> listado=Game_Object::get_objs();
  vector<Tile> lista_tls=Tile::get_tiles();



    for(it=objs_i.begin();it!=objs_i.end();it++){
        int index_o=*it;
        if(index_o>=0 && index_o<listado.size()){
          Game_Object g=listado.at(index_o);
          Transform* trans=Component::get_Transform(g.get_name());
          Collider* col=Component::get_Collider(g.get_name());

          if(trans!=NULL  ){

            Vector2 p=trans->Get_Position();

            p=p+g.get_parentOffset();
            Vector2 dm=Vector2(0,0);

            if(col!=NULL){

                dm=col->get_figure().get_dim();
               p=p+col->get_figure().get_center();
            }
            else{

                SDL_Rect r=g.get_object_rect();
                dm=Vector2(r.w,r.h);
                p=p+Vector2(dm.x*0.5f,dm.y*0.5f);
            }

            if(in_collision(p,dm)==true){

                if(profund_node<=0){
                   g.add_QuatreeData(get_root(),get_id());
                   Game_Object::edit_obj(g,index_o);
                }
                temp_objs[g.get_name()]=index_o;
                objs_indexs.push_back(index_o);
            }
          }
      }
    }


   for(it3=tls.begin();it3!=tls.end();it3++){
        int index_tl=*it3;
        if(index_tl>=0 && index_tl<lista_tls.size()){
          Tile tl=lista_tls[index_tl];

          SDL_Rect r=tl.get_rect();
          Vector2 p=Vector2(r.x,-r.y);
          bool tile_valid=tl.is_valid();
          Vector2 dm=Vector2(r.w,r.h);
          if(tile_valid==true){
              map<int,Sprite> tl_sprites=tl.get_Sprites_data();
              int num_layers=tl.get_count_layers();
              bool sprite_found=false;
              for(int j=0;j<num_layers;j++){
                  Sprite spr=tl_sprites[j];
                  if(spr.is_empty_sprite()==false ){
                      sprite_found=true;
                      j=num_layers;
                  }
              }

               if(sprite_found==true){
                    if(in_collision(p,dm)==true ){
                       temp_tls.push_back(index_tl);

                 }
              }
          }
      }
   }


   if(profund_node>0){
        has_childs=true;
        Vector2 next_size=Vector2(dm.x*0.5f,dm.y*0.5f);
        tiles=temp_tls;
        objs=temp_objs;


       for(int i=0;i<4;i++){


           Vector2 next_center=Vector2(center.x,center.y);
           if(i==0){

              next_center.x=next_center.x-(next_size.x*0.5f);
               next_center.y=next_center.y+(next_size.y*0.5f);

           }
           else if(i==1){
               next_center.x=next_center.x+(next_size.x*0.5f);
               next_center.y=next_center.y+(next_size.y*0.5f);

           }
           else if(i==2){
               next_center.x=next_center.x-(next_size.x*0.5f);
               next_center.y=next_center.y-(next_size.y*0.5f);

           }
           else{
               next_center.x=next_center.x+(next_size.x*0.5f);
               next_center.y=next_center.y-(next_size.y*0.5f);

           }
           string next_id=id+to_string(i);

           Quatree_Node nod=Quatree_Node(next_size,next_center,(profund_node-1),temp_tls,objs_indexs,next_id,root_id);
           childs.push_back(nod);
       }

   }
   else{




        tiles=temp_tls;
        objs=temp_objs;
    }

}
Vector2 Quatree_Node::get_dimension(){
   return dimension;
}
Vector2 Quatree_Node::get_center(){
   return center;
}

bool Quatree_Node::is_valid(){
  return validad;
}
bool Quatree_Node::in_collision(Vector2 p, Vector2 dm){

    Vector2 A_center=p;
    Vector2 B_center=center;
    Vector2 halfA=dm;
    Vector2 halfB=dimension;

    halfA.x=halfA.x*0.5f;
    halfA.y=halfA.y*0.5f;
    halfB.x=halfB.x*0.5f;
    halfB.y=halfB.y*0.5f;
    if(A_center.x+halfA.x>=B_center.x-halfB.x && A_center.x-halfA.x<=B_center.x+halfB.x){


       if(A_center.y-halfA.y<=B_center.y+halfB.y && A_center.y+halfA.y >=B_center.y-halfB.y){
          return true;

       }
      }
  return false;
}
vector<Quatree_Node> Quatree_Node::get_node_contains(Vector2 p,Vector2 dm){

   vector<Quatree_Node> res;
   if(has_childs==true ){

        vector<Quatree_Node> ::iterator it;
        for(it=childs.begin();it!=childs.end();it++){
              Quatree_Node n=*it;

              if(n.in_collision(p,dm)==true){
                  vector<Quatree_Node> temp_res=n.get_node_contains(p,dm);

                  if(temp_res.size()>0){
                       vector<Quatree_Node>::iterator it2;
                       for(it2=temp_res.begin();it2!=temp_res.end();it2++){
                           Quatree_Node q=*it2;
                           if(q.is_valid()==true){
                               res.push_back(q);
                           }
                       }

                  }
              }
       }
   }
   else{
       if(in_collision(p,dm)==true){

           res.push_back(*this);

       }
   }
   return res;
}
int Quatree_Node::get_profundidad(){
  return profund_node;
}

vector<int> Quatree_Node::get_tiles(){
    return tiles;

}



string Quatree_Node::get_id(){
   return id;
}
string Quatree_Node::get_root(){
   return root;
}
map<string,int> Quatree_Node::get_objs(){
   return objs;
}
void Quatree_Node::remove_object(Game_Object g,vector<string> ids_search){

   bool valid_remove=false;
   vector<string>::iterator it;
   for(it=ids_search.begin();it!=ids_search.end();it++){
       string temp=*it;
       if(temp==id){
          valid_remove=true;
          break;
       }
   }
   if(valid_remove==true){
         if(objs.find(g.get_name())!=objs.end()){
             objs.erase(g.get_name());


         }
   }
   else{
       if(has_childs){
           for(int i=0;i<4;i++){
                Quatree_Node q=childs[i];
                q.remove_object(g,ids_search);
                childs[i]=q;
            }
       }
   }
}
void Quatree_Node::add_object(Game_Object g,Vector2 pos,Vector2 dm){

    if(in_collision(pos,dm)==true){
        if(has_childs==true){
          for(int i=0;i<4;i++){
            Quatree_Node q=childs[i];
            q.add_object(g,pos,dm);
            childs[i]=q;
          }
        }
        else{
            int index=-1;
            g=Game_Object::Find(g.get_name(),index);
            if(index!=-1){
               objs[g.get_name()]=index;
               g.add_QuatreeData(get_root(),get_id());
               Game_Object::edit_obj(g,index);
            }

        }

    }

}
void Quatree_Node::search_object(string nam){
   if(has_childs){
       for(int i=0;i<4;i++){
          childs[i].search_object(nam);
       }
   }
   else{
     if(objs.find(nam)!=objs.end()){
            cout<<"found at" <<id<<"value:"<<objs[nam]<<endl;
            cout<<"pos Node"<<center.to_str()<<endl;
            cout<<"dimension Node"<<dimension.to_str()<<endl;
       }
   }
}
int  Quatree::max_profund=0;
Vector2 Quatree::center_q=Vector2(0,0);
Vector2 Quatree::size_q=Vector2(0,0);
vector<Quatree_Node> Quatree::nodos_q;
bool Quatree::validado=false;
map<string,tuple<Game_Object,Vector2,Vector2>>Quatree::Cola;
bool Quatree::updating=false;

void Quatree::make_tree( int profundidad,Scene s){
   validado=false;
   max_profund=profundidad;
   Vector2 p_begin=Vector2(0,0);
   Vector2 p_end=Vector2(0,0);
   vector<Vector2> begind_end=s.get_mapa_inicio_fin();
   if(begind_end.size()>=2){

      p_begin=begind_end.at(0);
      p_end=begind_end.at(1);
   }



   vector<Game_Object> all_objs=Game_Object::get_objs();
   vector<Game_Object>::iterator it_o;
   vector<int>objs;

   int index_o=0;
   for(it_o=all_objs.begin();it_o!=all_objs.end();it_o++){
       objs.push_back(index_o);
      index_o++;
   }


   vector<Tile> tls=Tile::get_tiles();
   vector<Tile>::iterator it_tl;
   int index_tl=0;
   vector<int> tls_index;
   for(it_tl=tls.begin();it_tl!=tls.end();it_tl++){
       tls_index.push_back(index_tl);
       index_tl++;
   }

   if(nodos_q.size()>0){
       nodos_q.clear();
   }
   if(p_begin==p_end){
      cout<<"error mapa sin dimension"<<endl;
      return;
   }


   size_q=Vector2(0,0);
   size_q.x=abs(p_begin.x-p_end.x),
   size_q.y=abs(p_begin.y-p_end.y);
   center_q=Vector2(0,0);
   center_q.x=p_begin.x+(size_q.x*0.5f);
   center_q.y=p_begin.y-(size_q.y*0.5f);

   Vector2 next_size=Vector2(size_q.x,size_q.y);
   next_size.x=next_size.x*0.5f;
   next_size.y=next_size.y*0.5f;
   for(int i=0;i<4;i++){

      Vector2 next_center=Vector2(0,0);
      if(i==0){
        next_center.x=next_size.x*0.5f;
        next_center.y=next_size.y*0.5f;
      }
      else if(i==1){
         next_center.x=next_size.x+(next_size.x*0.5f);
        next_center.y=next_size.y*0.5f;
      }
      else if(i==2){
        next_center.x=next_size.x*0.5f;
        next_center.y=next_size.y+(next_size.y*0.5f);
      }
      else{
        next_center.x=next_size.x+(next_size.x*0.5f);
        next_center.y=next_size.y+(next_size.y*0.5f);
      }
      next_center.y=next_center.y*-1;


     string next_id=to_string(i);
      Quatree_Node nod=Quatree_Node(next_size,next_center,profundidad,tls_index,objs,next_id,next_id);
      nodos_q.push_back(nod);
   }

   validado=true;

}

 tuple<vector<int>,vector<int>>Quatree::get_elements_in_radious(Vector2 p,float radio){
    vector<int> tls;
    vector<int> objs;
   if(updating){
       return  std::make_tuple(tls,objs);

   }


    vector<Quatree_Node> ::iterator it;
    Vector2 pos=p;
    Vector2 size_p=Vector2(radio,radio);


   for(it=nodos_q.begin();it!=nodos_q.end();it++){
       Quatree_Node n=*it;
       if(n.in_collision(pos,size_p)==true){
            vector<Quatree_Node>temp_res=n.get_node_contains(pos,size_p);

            if(temp_res.size()>0){
                vector<Quatree_Node>::iterator it2;
                for(it2=temp_res.begin();it2!=temp_res.end();it2++){
                      Quatree_Node q=*it2;


                      if(q.is_valid()==true){

                           vector<int> q_tls=q.get_tiles();
                           map<string,int> q_objs=q.get_objs();
                           vector<int>::iterator it_tl;
                           map<string,int>::iterator it_objs;
                           for(it_tl=q_tls.begin();it_tl!=q_tls.end();it_tl++){
                               int val_tl=*it_tl;
                               bool valid_tile=true;
                               for(int k=0;k<tls.size();k++){
                                  int t=tls.at(k);

                                 if(t==val_tl){
                                     valid_tile=false;
                                     k=tls.size();
                                 }
                               }
                               if(valid_tile==true && val_tl>=0){
                                   tls.push_back(val_tl);
                               }
                           }



                           for(it_objs=q_objs.begin();it_objs!=q_objs.end();it_objs++){
                               int index=it_objs->second;

                               bool valid_index=true;
                               for(int j=0;j<objs.size();j++){
                                  if(objs.at(j)==index){
                                     valid_index=false;
                                    j=objs.size();
                                  }
                               }
                               if(valid_index==true && index>=0){
                                   objs.push_back(index);
                               }
                           }

                      }
                }



            }
       }

   }
    std::tuple<vector<int>,vector<int>> res=std::make_tuple(tls,objs);
    return res;
}
bool Quatree::is_valid(){
  return validado;
}

void  Quatree::modifc_obj(Game_Object g,Vector2 pos,Vector2 dm){
     tuple<Game_Object,Vector2,Vector2> t= std::make_tuple(g,pos,dm);
     Cola[g.get_name()]=t;


}
bool Quatree::update(){

    updating=true;
    bool res=false;
    if(Cola.size()>0){
        res=true;
    }
    map<string ,tuple<Game_Object,Vector2,Vector2>>::iterator it_cola;
    for(it_cola=Cola.begin();it_cola!=Cola.end();it_cola++){

       tuple<Game_Object,Vector2,Vector2> temp_tupl=it_cola->second;
       int sz=tuple_size<decltype(temp_tupl)>::value;
       if(sz>=3){
          Game_Object g=std::get<0>(temp_tupl);
          Vector2 pos=std::get<1>(temp_tupl);
          Vector2 dm=std::get<2>(temp_tupl);

          int index=-1;
          Game_Object::Find(g.get_name(),index);
          if(index!=-1){


              remove_fromQ(g);

              g.reset_QuatreeData();
              Game_Object::edit_obj(g,index);
              add_toQ(g,pos,dm);
              Game_Object g2=Game_Object::Find(g.get_name(),index);

              map<string,vector<string>> d=g2.get_QuatreeData();

              if(d.size()<1){
                cout<<"error en quatre modificacion"<<endl;
              }

           }

       }
    }
    Cola.clear();


    updating=false;
    return res;
}
void Quatree::search_Q(string nam){
   for(int i=0;i<4;i++){
        nodos_q[i].search_object(nam);
   }
}
void Quatree::remove_fromQ(Game_Object g){


   map<string,vector<string>> Gq_data=g.get_QuatreeData();
   map<string,vector<string>>::iterator it_Gq;

   for(it_Gq=Gq_data.begin();it_Gq!=Gq_data.end();it_Gq++){
        string id_root=it_Gq->first;
        vector<string> ids_search=it_Gq->second;
        for(int i=0;i<4;i++){
          Quatree_Node q=nodos_q[i];
          if(q.get_id()==id_root){
             q.remove_object(g,ids_search);
             nodos_q[i]=q;

          }

      }

   }


}
void Quatree::add_toQ(Game_Object g,Vector2 pos,Vector2 dm){



     for(int i=0;i<4;i++){
       Quatree_Node q=nodos_q[i];
       if(q.in_collision(pos,dm)){
           q.add_object(g,pos,dm);
           nodos_q[i]=q;
       }
   }
}
