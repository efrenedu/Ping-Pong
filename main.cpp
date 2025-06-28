

//Using SDL and standard IO
#include<SDL_mixer.h>
#include <stdio.h>
#include<Physic_Engine.h>
#include <Event_Manager.h>
#include<Component.h>
#include <cctype>
#include<chrono>
#include <Singleton.h>


//Screen dimension constants
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
bool use_text=true;
bool playing=true;

float next_physic=0.0f;

void draw_escena(SDL_Renderer* render);
void draw(Game_Object g,SDL_Renderer* render);
void update_escena();


void Inputs_escena();
void draw_UI(Game_Object u,SDL_Renderer* render);
bool set_config(map<string,string> config,SDL_Window* win);
map<string,string> leer_Config(string ruta);

int main( int argc, char* args[] )
{



   SDL_Window* window = NULL;
   SDL_Surface* screenSurface = NULL;
   SDL_Renderer* renderer=NULL;
   float worl_scale=2.5f;
   SDL_Window* second_win = NULL;
  SDL_Renderer* second_renderer=NULL;




   if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO |  SDL_INIT_TIMER |SDL_INIT_EVENTS) < 0 )
   {
      printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );

    }
   else
   {
     if(!(IMG_Init(IMG_INIT_PNG))){
      printf( "png init fail" );

     }

     if(!(IMG_Init(IMG_INIT_JPG))){
       printf( "jpg init fail" );
     }
     if( Mix_OpenAudio( 44100, AUDIO_F32SYS, 2, 2048 ) < 0 ){
           printf( "fail loading mix" );
     }

    if(!(Mix_Init(MIX_INIT_OGG))){printf( "fail loading ogg format"); }
    if(!(Mix_Init(MIX_INIT_MP3))){ printf( "fail loading mp3 format" );}

     int flags=SDL_RENDERER_ACCELERATED  | SDL_WINDOW_HIDDEN;
     window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,flags);
     second_win =SDL_CreateWindow( "iniciando", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_SHOWN);

     if( window == NULL )
     {
      printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
     }
     else
      {


       if(second_win!=NULL){
          second_renderer=SDL_CreateRenderer(second_win,-1,0);
          SDL_SetRenderDrawColor(second_renderer,0,0,0,255);
          SDL_RenderClear(second_renderer);
          SDL_RenderPresent(second_renderer);
       }
       if(TTF_Init()<0){

          cout<<"fallo al iniciar TTF"<<endl;
          use_text=false;
       }
          //INICIA SDL

       // screenSurface = SDL_GetWindowSurface( window );



          AudioManager::set_volumeSfx(0.6f);
          AudioManager::set_volumeMusic(0.5f);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_RenderSetLogicalSize(renderer,SCREEN_WIDTH,SCREEN_HEIGHT);


        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_Event evento;

        vector<string> cursores;
        Event_Manager::set_cursores(cursores);


        Scene_manager::set_scale_world(worl_scale);
        Scene_manager::set_renderer(renderer);
        Input::init();


       string src_config="config.json";
        map<string,string> config=leer_Config(src_config);
       playing=set_config(config,window);
       SDL_DestroyWindow(second_win);
       second_win=NULL;
       SDL_DestroyRenderer(second_renderer);
       second_renderer=NULL;




        while(playing==true){



            if(Scene_manager::is_loading()){

                draw_escena(renderer);

            }


            std::thread hilo_update(update_escena);
            if(hilo_update.joinable()){
                hilo_update.join();
            }
           // update_escena();


            Scene_manager::update();
            Screen::update(window);


            Scene escena=Scene_manager::get_active_escene();
            Input::update();
            draw_escena(renderer);
            SDL_RenderPresent(renderer);
            playing=Event_Manager::play_game();


            if( SDL_PollEvent( &evento )) {


                  Input::get_event(evento);

                  switch(evento.type){
                  case SDL_QUIT:
                      {
                          playing=false;
                          break;
                      }


                  }
               }



             SDL_Delay( Timer::get_ticks_frame());

          }


       }
   }


    Singleton::modific_config();
    //libera sdl de la memoria
    Texture_manager::reset_textures();
    AudioManager::liberar();
    SDL_DestroyWindow( window );
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    Mix_Quit();
    Event_Manager::liberar();
    return 0;
}


bool set_config(map<string,string> config,SDL_Window* win){

        bool res=true;
        vector<string> params_search={"Scenes:List","Scenes:Format","Windows:Name","Windows:Size","Windows:Resizable","Windows:FullScreen","Windows:Icon","Logo:Src","Logo:Time","Logo:Sfx"};
        vector<string> ::iterator it_search;
        Singleton::get_config_inf(config);

        for(it_search=params_search.begin();it_search!=params_search.end();it_search++){
           map<string,string>::iterator it_temp;
           it_temp=config.find(*it_search);
           if(it_temp==config.end()){
               res=false;
              SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Configuration Error","Error to Read Config data", win);
              break;
           }
        }




        if(res==true){
             SDL_SetWindowTitle(win,config["Windows:Name"].c_str());
             string icon_src=config["Windows:Icon"];
             if(icon_src!=""){
                   icon_src=string(SDL_GetBasePath())+"/assets/images/"+icon_src;
                   SDL_Surface *surf_icon=IMG_Load(icon_src.c_str());

                   if(surf_icon!=NULL){

                      SDL_SetWindowIcon(win,surf_icon);
                      SDL_FreeSurface(surf_icon);
                   }
             }
             if(config["Windows:FullScreen"]=="True"){
                  SDL_SetWindowFullscreen(win,SDL_WINDOW_FULLSCREEN);

             }
             string sz=config["Windows:Size"];
             int w=0;
             int h=0;
             string temp="";
             for(int j=0;j<sz.length();j++){
                 unsigned char c=sz[j];
                 if(c=='x'){
                    if(temp!=""){
                       w=atoi(temp.c_str());
                       temp="";
                    }
                 }
                 else{
                    temp+=c;
                 }
             }
             if(temp!=""){
                h=atoi(temp.c_str());
             }
             Vector2 dim_win=Vector2(w,h);
             Screen::set_size(dim_win);
             SDL_SetWindowSize(win,w,h);


             string lista_escenas=config["Scenes:List"];
             temp="";

             for(int i=0;i<lista_escenas.length();i++){
                   unsigned char c=lista_escenas[i];
                   if(c==','){

                       Scene_manager::add_scene(temp);
                       temp="";
                   }
                   else{
                    temp+=c;
                   }
             }
             if(temp!=""){
                  Scene_manager::add_scene(temp);
                  temp="";
             }


             config.clear();
             Scene_manager::load_scene(0);
             Scene_manager::update();
             SDL_ShowWindow(win);
        }

    return res;
}
void draw(Game_Object g, SDL_Renderer* render){


}


void draw_escena(SDL_Renderer* render){

//dibujamos background


      if(Scene_manager::is_loading()==true){
         SDL_SetRenderDrawColor(render,0,0,0,255);
         SDL_RenderClear(render);
         SDL_RenderPresent(render);

         return;
       }

       Scene escena=Scene_manager::get_active_escene();
       vector<Game_Object>& objs=Game_Object::get_objs();
       vector<Game_Object>& ui_escena=UI_object::get_UI();

       vector<Game_Object>::iterator it_ob1;


      bool found=false;
      Camera cam=Camera::get_ActiveCamera(found);


      if(cam.is_active()==false || found==false){
       //no hay camara no se dibuja nada
         SDL_SetRenderDrawColor(render,0,0,0,255);
         SDL_RenderClear(render);
         cout<<"fail cam"<<endl;
          return ;
      }


      Vector2 desplz_cam=cam.get_pos();

      Vector2 area_cam=cam.get_Half_dim();

      desplz_cam.y=desplz_cam.y*-1;
      desplz_cam.x=desplz_cam.x-area_cam.x;
      desplz_cam.y=desplz_cam.y-area_cam.y;









      tuple<vector<int>, vector<int>> in_cam=Camera::get_inCamDraw();
      int sz=tuple_size<decltype(in_cam)>::value;


      if(sz<2){

          cout<<"no quatree found"<<endl;
          SDL_SetRenderDrawColor(render,0,0,0,255);
          SDL_RenderClear(render);

         return;
      }



//datos a del quatree
   vector<int> mapa_indexs=std::get<0>(in_cam);
   vector<int> ::iterator it;
   vector<Tile> lista_tls=Tile::get_tiles();
   vector<int>objs_indexs=std::get<1>(in_cam);
   vector<int>::iterator it_objs;


   if((objs_indexs.size()<=0 && mapa_indexs.size()<=0)==false ){


     SDL_SetRenderDrawColor(render,0,0,0,0);
     SDL_RenderClear(render);
     int id_background=escena.get_id_texture_background();
     if(id_background!=-1){

          SDL_RenderCopy(render,Texture_manager::get_texture(id_background,0), NULL, NULL);

      }




      for(it=mapa_indexs.begin();it!=mapa_indexs.end();it++){
         int index_tl=*it;
         if(index_tl>=0 && index_tl<lista_tls.size()){

             Tile tl=lista_tls[index_tl];
             int num_layers=tl.get_count_layers();
             if(num_layers>0){

                SDL_Rect rect=tl.get_rect();
                rect.x=rect.x-desplz_cam.x;
                rect.y=rect.y-desplz_cam.y;
                map<int,Sprite> sprites_layers=tl.get_Sprites_data();

                for(int i=0;i<num_layers;i++){

                    Sprite spr=sprites_layers[i];
                    if(spr.is_empty_sprite()==false){
                        SDL_Rect rect_sprite=spr.get_rect_sprite();
                        vector<int> texture_index=spr.get_TextureIndex();
                         SDL_Texture* tx=NULL;
                        if(texture_index.size()>=2){
                            tx=Texture_manager::get_texture(texture_index.at(0),0);
                        }
                        if(tx!=NULL){
                            SDL_RenderCopy(render,tx, &rect_sprite, & rect);
                        }

                    }



                 }

             }
          }


       }


      map<int,vector<int>>pendientes_objs;

      for(it_objs= objs_indexs.begin();it_objs!= objs_indexs.end();it_objs++){

        int index=*it_objs;
        if(index>=0 && index<objs.size()){
          Game_Object g=objs[index];


          if(g.is_active() ){

              Render* render_comp=Component::get_Render(g.get_name());
              if(render_comp!=NULL){



                  int layer_draw=render_comp->get_layer();

                  if(render_comp->is_visible() && layer_draw==0){



                      Sprite spr=render_comp->get_sprite();
                      SDL_Rect pos_obj=g.get_object_rect();
                      Vector2 parent_offset=g.get_parentOffset();
                      parent_offset.y=parent_offset.y*-1;
                       pos_obj.x=pos_obj.x+parent_offset.x;
                      pos_obj.y=pos_obj.y+parent_offset.y;


                      pos_obj.x=pos_obj.x-desplz_cam.x;
                      pos_obj.y=pos_obj.y-desplz_cam.y;

                     SDL_Rect rect_sprite=spr.get_rect_sprite();
                     vector<int> texture_index=spr.get_TextureIndex();



                       int angle=0;
                       Transform* t=Component::get_Transform(g.get_name());
                       if(t!=NULL){
                          angle=t->get_rotation();
                       }


                     int flip=g.get_flip();

                     SDL_Texture* tx=NULL;
                     if(texture_index.size()>=2){
                         tx=Texture_manager::get_texture(texture_index.at(0),0);
                     }
                     if(tx!=NULL){



                        if(flip==true){
                               SDL_RenderCopyEx(render, tx,&rect_sprite,& pos_obj,angle,NULL, SDL_FLIP_HORIZONTAL);
                           }
                        else{

                               // SDL_RenderCopy(render,Texture_manager::get_texture(texture_index), &rect_sprite, & rect);
                               SDL_RenderCopyEx(render, tx,&rect_sprite,& pos_obj,angle,NULL, SDL_FLIP_NONE);

                           }
                     }
               }
               else{
                   if(render_comp->is_visible()){
                       pendientes_objs[layer_draw].push_back(index);
                  }
               }




            }

          }
        }
     }

     if(pendientes_objs.size()>0){

        map<int,vector<int>>::iterator pend_o;
        for(pend_o=pendientes_objs.begin();pend_o!=pendientes_objs.end();pend_o++){

            vector<int> temp_indexs=pend_o->second;
            vector<int>::iterator it_o;
            for(it_o=temp_indexs.begin();it_o!=temp_indexs.end();it_o++){
                int index_temp=*it_o;
                if(index_temp>=0 && index_temp<objs.size()){
                  Game_Object g=objs[index_temp];
                  Render* render_comp=Component::get_Render(g.get_name());
                  if(render_comp!=NULL){
                      Sprite spr=render_comp->get_sprite();
                      SDL_Rect pos_obj=g.get_object_rect();
                      Vector2 parent_offset=g.get_parentOffset();
                      parent_offset.y=parent_offset.y*-1;
                      pos_obj.x=pos_obj.x+parent_offset.x;
                      pos_obj.y=pos_obj.y+parent_offset.y;


                      pos_obj.x=pos_obj.x-desplz_cam.x;
                      pos_obj.y=pos_obj.y-desplz_cam.y;

                       SDL_Rect rect_sprite=spr.get_rect_sprite();
                       vector<int> texture_index=spr.get_TextureIndex();

                       int angle=0;
                       Transform* t=Component::get_Transform(g.get_name());
                       if(t!=NULL){
                          angle=t->get_rotation();
                       }


                       int flip=g.get_flip();
                       SDL_Texture* tx=NULL;
                       if(texture_index.size()>=2){
                           tx=Texture_manager::get_texture(texture_index.at(0),0);
                        }

                       if(tx!=NULL){

                           if(flip==true){
                                  SDL_RenderCopyEx(render, tx,&rect_sprite,& pos_obj,angle,NULL, SDL_FLIP_HORIZONTAL);
                              }
                           else{
                               // SDL_RenderCopy(render,Texture_manager::get_texture(texture_index), &rect_sprite, & rect);
                                 SDL_RenderCopyEx(render, tx,&rect_sprite,& pos_obj,angle,NULL, SDL_FLIP_NONE);

                              }
                       }

                   }
                 }
              }
           }
        }


   }









   vector<Game_Object>::iterator it_ui;
   map<int,vector<int>> pendientes_UI;
   int index_UI=0;
   for(it_ui=ui_escena.begin();it_ui!=ui_escena.end();it_ui++){
         Game_Object u=*it_ui;

         if(u.is_active()){
             Render* render_comp=Component::get_Render(u.get_name());

             if(render_comp!=NULL){
                  int lyr=render_comp->get_layer();

                  if(lyr==0){
                       draw_UI(u,render);
                  }
                  else{

                      pendientes_UI[lyr].push_back(index_UI);

                  }
             }
         }
         index_UI++;
          //EN IF ACTIVE==TRUE


   } //end for UI

   if(pendientes_UI.size()>0){

      map<int,vector<int>>::iterator it_pendU;
      for(it_pendU=pendientes_UI.begin();it_pendU!=pendientes_UI.end();it_pendU++){
           vector<int> ui_temps=it_pendU->second;
           vector<int> ::iterator it_pend2;
           for(it_pend2=ui_temps.begin();it_pend2!=ui_temps.end();it_pend2++){
                int index_temp=*it_pend2;
                if(index_temp>=0 && index_temp<ui_escena.size()){
                   Game_Object u=ui_escena[index_temp];
                   draw_UI(u,render);
                }
             }
          }
       }




}

void draw_UI(Game_Object u,SDL_Renderer* render){




            Render* render_comp=Component::get_Render(u.get_name());
            bool visibl=false;
            if(render_comp!=NULL){
                  if(render_comp->is_visible()){
                       visibl=true;
                  }
            }

           int UIid=u.get_UIid();

           SDL_Rect ui_pos=u.get_object_rect();
           Vector2 offset_p=u.get_parentOffset();
           offset_p.y=offset_p.y*-1;
           ui_pos.x=ui_pos.x+offset_p.x;
           ui_pos.y=ui_pos.y+offset_p.y;




            if((UIid==UI_TYPES::VALUEBAR || UIid==UI_TYPES::SCROLLBAR) && visibl==true){


                if(UIid==UI_TYPES::SCROLLBAR){
                    ScrollBar* scroll=Component::get_scrollBar(u.get_name());
                    if(scroll!=NULL){
                         SDL_Rect bg_rect=scroll->get_bgRect();
                         vector<SDL_Color> bg_colors=scroll->get_BGcolors();
                         if(bg_colors.size()>=2){
                            SDL_Color bg=bg_colors[0];
                            SDL_Color fg=bg_colors[1];
                            SDL_Color rastro_color;
                            SDL_SetRenderDrawColor(render,bg.r,bg.g,bg.b,bg.a);
                            SDL_RenderFillRect(render,&bg_rect);
                            if(scroll->use_rastro(&rastro_color)){
                                SDL_Rect rastro_rect;
                                rastro_rect.x=bg_rect.x;
                                rastro_rect.y=bg_rect.y;
                                rastro_rect.w=abs(rastro_rect.x-ui_pos.x);
                                rastro_rect.h=abs(rastro_rect.y-ui_pos.y);
                                if(scroll->get_sentido().x!=0){
                                   rastro_rect.h=bg_rect.h;
                                }
                                else{
                                   rastro_rect.w=bg_rect.w;
                                }

                                SDL_SetRenderDrawColor(render,rastro_color.r,rastro_color.g,rastro_color.b,rastro_color.a);
                                SDL_RenderFillRect(render,&rastro_rect);
                         }

                         SDL_SetRenderDrawColor(render,fg.r,fg.g,fg.b,fg.a);
                         SDL_RenderDrawRect(render,&bg_rect);
                         }
                    }
                }
                else{

                    ValueBar* barV=Component::get_ValueBar(u.get_name());
                    if( barV!=NULL){
                          bool has_bgImg=false;
                          bool has_bgColors=false;
                          vector<SDL_Color> bg_colors= barV->get_BgColors(has_bgColors);
                          Icon_UI icono=barV->get_BgIcon(has_bgImg);
                          bool icono_succes=false;
                          if(has_bgImg){
                                vector<int> indexs=icono.get_indexs();
                                 SDL_Texture *tex=NULL;
                                if(indexs.size()>=2){
                                     tex=Texture_manager::get_texture(indexs[0],indexs[1]);
                                }
                                if(tex!=NULL){
                                    icono_succes=true;
                                    SDL_Rect r_icon=icono.get_rect();
                                    if(r_icon.w==-1 || r_icon.h==-1){

                                        SDL_RenderCopyEx(render, tex,NULL,& ui_pos,0,NULL, SDL_FLIP_NONE);
                                    }
                                    else
                                    {
                                            SDL_RenderCopyEx(render, tex,&r_icon,& ui_pos,0,NULL, SDL_FLIP_NONE);
                                    }


                                }

                          }
                          if(has_bgColors && icono_succes==false){
                             if(bg_colors.size()>=2){
                                SDL_Color bg=bg_colors[0];
                                SDL_Color fg=bg_colors[1];
                                SDL_SetRenderDrawColor(render,bg.r,bg.g,bg.b,bg.a);
                                SDL_RenderFillRect(render,&ui_pos);

                                SDL_SetRenderDrawColor(render,fg.r,fg.g,fg.b,fg.a);
                                SDL_RenderDrawRect(render,&ui_pos);
                             }

                          }
                    }
                }



            }




            if(visibl==true && UIid!=UI_TYPES::LABEL){


                  Sprite spr=render_comp->get_sprite();
                   SDL_Rect rect_sprite=spr.get_rect_sprite();

                     if(UIid==UI_TYPES::VALUEBAR){

                           ValueBar* bar=Component::get_ValueBar(u.get_name());
                           if(bar!=NULL){
                               Vector2 sentido=bar->get_sentido();
                               float val=bar->get_value();
                               if(sentido.x==0){
                                  float h=ui_pos.h+0.0f;
                                  ui_pos.h=(h*val);
                                  if(bar->cut_sprite()){
                                    rect_sprite.h=(rect_sprite.h*val);
                                  }

                               }
                              else{
                                     float w=ui_pos.w+0.0f;
                                     ui_pos.w=(w*val);
                                     if(bar->cut_sprite()){
                                        rect_sprite.w=(rect_sprite.w*val);
                                    }

                               }

                           }

                     }


                     vector<int> texture_index=spr.get_TextureIndex();

                     SDL_Texture* tx=NULL;
                     if(texture_index.size()>=2){
                        tx=Texture_manager::get_texture(texture_index.at(0),texture_index.at(1));
                    }

                    if(tx!=NULL){

                           if(rect_sprite.w==-1 || rect_sprite.h==-1){
                               SDL_RenderCopyEx(render, tx,NULL,& ui_pos,0,NULL, SDL_FLIP_NONE);

                           }
                           else{
                                  SDL_RenderCopyEx(render, tx,&rect_sprite,& ui_pos,0,NULL, SDL_FLIP_NONE);


                           }
                    }

            }  //EN IF SPRITE EXIST

            if(use_text==true ){


                 Data_Text d_text=Data_Text();
                 bool comp_text=false;
                 bool w_text=false;
                if(UIid==UI_TYPES::BUTTON){

                      Button* btn=Component::get_Button(u.get_name());
                      if(btn!=NULL){
                         comp_text=true;

                         d_text=btn->get_textData(w_text);

                      }
                }
                else if(UIid==UI_TYPES::LABEL){

                    Label * label=Component::get_label(u.get_name());
                    if(label!=NULL){
                        comp_text=true;
                         d_text=label->get_textData(w_text);

                    }
                }
                else if(UIid==UI_TYPES::RADIOBUTTON){
                    RadioButton * radio=Component::get_radioButton(u.get_name());
                    if(radio!=NULL){
                        comp_text=true;

                         d_text=radio->get_textData(w_text);

                    }
                }
                else if(UIid==UI_TYPES::CHECKBUTTON){
                    CheckButton * check=Component::get_checkButton(u.get_name());
                    if(check!=NULL){
                        comp_text=true;

                         d_text=check->get_textData(w_text);

                    }
                }
                else if(UIid==UI_TYPES::VALUEBAR){
                     ValueBar * bar=Component::get_ValueBar(u.get_name());
                    if(bar!=NULL){
                        comp_text=true;

                         d_text=bar->get_textData(w_text);

                    }
                }
                else if(UIid==UI_TYPES::IMAGE){
                    Image_UI * img=Component::get_ImageUI(u.get_name());
                    if(img!=NULL){
                        comp_text=true;

                         d_text=img->get_textData(w_text);

                    }
                }
                if(comp_text==true && w_text==true){
                     TTF_Font* fuente=Font_Manager::get_font(d_text.get_font());
                      if(fuente!=NULL){


                          string texto=d_text.get_text();
                          SDL_Color colr=d_text.get_color();
                          SDL_Color colr2= { 0, 0, 0, 0  };
                          int sz=d_text.get_size();
                          Vector2 offste_f=d_text.get_offset();


                          SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(fuente,texto.c_str(), colr,(Uint32)ui_pos.w);



                         if(textSurface!=NULL){
                             SDL_Rect textLocation = { ui_pos.x+offste_f.x, ui_pos.y+offste_f.y, textSurface->w, textSurface->h };

                             SDL_Texture * texture_text=NULL;
                             texture_text= SDL_CreateTextureFromSurface(render, textSurface);

                            if(texture_text!=NULL){
                                SDL_RenderCopyEx(render, texture_text,NULL,& textLocation,0,NULL, SDL_FLIP_NONE);
                                SDL_DestroyTexture(texture_text);
                            }

                              SDL_FreeSurface(textSurface);
                         }
                      }
                }


            }// EN IF TEXT DRAW




}
void update_escena()
{






        if(Scene_manager::is_loading()==false ){






        Singleton::update();
        bool update_quat=Quatree::update();
        Camera::update_camActive( update_quat);
          double start=SDL_GetTicks();
         vector<Game_Object>& objs=Game_Object::get_objs();
         vector<Game_Object>& ui_escena=UI_object::get_UI();

          bool found=false;
          Camera cam=Camera::get_ActiveCamera( found);
          if(cam.is_active()==false || found==false){
              return;
          }


          tuple<vector<int>, vector<int>> in_cam=Camera::get_inCam();
          int sz= tuple_size<decltype(in_cam)>::value;
          if(sz<2){
              return;
          }



           Timer::update();



            Physic_Engine::update_physic(objs);
            vector<int> in_camObjs=std::get<1>(in_cam);
            vector<Game_Object>::iterator it_objs;
            vector<int> ::iterator it_cam;



            for(it_cam=in_camObjs.begin();it_cam!=in_camObjs.end();it_cam++){
              int index_g=*it_cam;

              if(index_g>=0 && index_g<objs.size()){
                 Game_Object g=objs[index_g];


                 int index_parent=g.get_parentId();
                 if(index_parent!=-1 ){


                      Game_Object parent=objs.at(index_parent);
                      if(parent.is_active()){
                           Vector2 parent_init=parent.get_initPos();
                           Transform* trans_parent=Component::get_Transform(parent.get_name());
                           if(trans_parent!=NULL){
                              Vector2 parent_end=trans_parent->Get_Position();
                              Vector2 offset=parent_end-parent_init;
                              g.set_parentOffset(offset);
                           }
                      }
                      g.set_active(parent.is_active());


                 }

                 if(g.is_active() && g.get_pause()==false){


                   g.basic_update();
                   g.update();
                 }

                 objs[index_g]=g;

              }
            }




             vector<Game_Object>::iterator it_u;
              int index_ui=0;
              for(it_u=ui_escena.begin();it_u!=ui_escena.end();it_u++){



                      Game_Object u=*it_u;
                      int index_parent=u.get_parentId();
                      if(index_parent!=-1){


                           Game_Object parent=ui_escena[index_parent];
                            if(parent.is_active()){
                               Vector2 parent_init=parent.get_initPos();
                               Transform* trans_parent=Component::get_Transform(parent.get_name());
                               if(trans_parent!=NULL){
                                  Vector2 parent_end=trans_parent->Get_Position();
                                  Vector2 offset=parent_end-parent_init;
                                  u.set_parentOffset(offset);
                               }
                          }
                           u.set_active(parent.is_active());

                      }
                      if(u.is_active()){
                         u.update();
                      }



                   ui_escena[index_ui]=u;
                   index_ui++;
              }

         }



}


map<string,string> leer_Config(string ruta){

 map<string,string> res;
  ifstream stream;
   char comilla_simple = '\'';

  stream.open(string(SDL_GetBasePath())+"/assets/game_data/"+ruta,ios::in);
  if(stream.good()){
      string linea="";
      string param_name="";
      string second_param="";

      getline(stream,linea);              //omite encabezado

     while(getline(stream,linea)){

       string temp="";
       for(int i=0;i<linea.length();i++){
           unsigned char c=linea[i];
           int ch=c;
           bool white_space=false;
           if(isspace(ch)!=0){
               white_space=true;
            }

           if(c!=' ' && c!='"' && c!=comilla_simple && white_space==false){


              if(param_name==""){
                  if(c==':'){
                      param_name=temp;
                      temp="";
                      i=linea.length();
                  }
                  else{
                     temp+=c;
                  }
              }
              else{
                 if(c==']'){
                    param_name="";
                    second_param="";
                    temp="";
                 }
                 else{

                    if( second_param==""){
                        if(c!=':'){
                            temp+=c;

                        }
                        else{
                           second_param=temp;
                           temp="";
                        }
                    }
                    else{

                       temp+=c;
                    }
                 }
              }



           }// en if c!=' '


       }//end for

        if( param_name!="" && second_param!=""){

            string id=param_name+":"+second_param;
            res[id]=temp;
            second_param="";
            temp="";
        }


     }//end while
     stream.close();
   }  //end stream


   return res;                        //devuelve todas las lineas -1(encabezados)

}

