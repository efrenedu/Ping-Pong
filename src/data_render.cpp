#include "data_render.h"




void Texture_manager::set_textures(vector<Texture>lista ){
    Texture_manager::reset_textures();
    vector<Texture>::iterator it;
    lista_texturas=lista;

}
vector<int>Texture_manager:: get_indexs(SDL_Rect * r,int gId){
    vector<int > res;
    vector<Texture>::iterator it;

   if(gId>0){
        int index_tex=0;
        for(it=lista_texturas.begin();it!=lista_texturas.end();it++){
            Texture t=*it;
            if(t.is_loaded() && t.get_firstId()>0){

                int first_id=t.get_firstId();
                int last_id=t.get_lastId();
                if(gId>=first_id && gId<=last_id){
                    //id pertence a textura


                    vector<int> i_texs;
                    i_texs.push_back(index_tex);

                     if(t.is_multiple()){
                        int second_index=gId-first_id;
                        i_texs.push_back(second_index);
                        if(r!=NULL){
                           r->x=0;
                           r->y=0;
                           r->w=-1;
                           r->h=-1;
                        }
                     }
                     else{
                         SDL_Rect rect=t.get_rect(gId);
                         if(r!=NULL){
                            r->x=rect.x;
                            r->y=rect.y;
                            r->w=rect.w;
                            r->h=rect.h;
                        }
                         i_texs.push_back(0);
                     }
                   res=i_texs;
                   break;
                }
            }
            index_tex+=1;
        }
   }
   if(res.size()<=0){
    res.push_back(-1);
    res.push_back(-1);
    if(r!=NULL){
       r->x=-1;
       r->y=-1;
       r->w=-1;
       r->h=-1;
      }
   }
   return res;
}

void Texture_manager::reset_textures(){
    if(lista_texturas.size()>0){
        vector<Texture>::iterator it;
        for(it=lista_texturas.begin();it!=lista_texturas.end();it++){
            Texture t=*it;
            t.liberar_texture();
        }

        lista_texturas.clear();
    }
}

Sprite Texture_manager::get_sprite(int gId){
    Sprite res=Sprite();
    vector<Texture>::iterator it;
   if(gId>0){


        vector<Texture>::iterator it;
        int index_tex=0;
        for(it=lista_texturas.begin();it!=lista_texturas.end();it++){
            Texture t=*it;

            if(t.is_loaded() && t.get_firstId()>0){

                int first_id=t.get_firstId();
                int last_id=t.get_lastId();
                if(gId>=first_id && gId<=last_id){

                    //id pertence a textura

                    SDL_Rect rect;
                    vector<int> i_texs;
                    i_texs.push_back(index_tex);
                     if(t.is_multiple()){
                        int second_index=gId-first_id;
                        i_texs.push_back(second_index);
                        rect.x=0;
                        rect.y=0;
                        rect.w=-1;
                        rect.h=-1;
                     }
                     else{


                         rect=t.get_rect(gId);
                         i_texs.push_back(0);
                     }
                     res=Sprite(rect,i_texs);
                   break;
                }
            }
            index_tex+=1;
        }



   }
   return res;

}

vector<Texture> Texture_manager::lista_texturas;

SDL_Texture* Texture_manager::get_texture(int index,int index2){

    SDL_Texture *res=NULL;
    int cant=lista_texturas.size();
    if(cant>0){
        if(index>=0 && index<cant){
            Texture t=lista_texturas.at(index);
            res=t.get_texture(index2);
        }
    }
    return res;
}

Sprite::Sprite(){

    empty_sprite=true;

}
Sprite::Sprite(SDL_Rect rct_spr,vector<int> tex_index){
    rect_sprite=rct_spr;
    texture_index=tex_index;
     empty_sprite=false;
}
bool Sprite::is_empty_sprite(){
     return empty_sprite;
}
void  Sprite:: set_Pos_Sprite(SDL_Rect rct_spr){
  rect_sprite=rct_spr;
}
void Sprite::change_texture(SDL_Rect rct_spr,vector<int> tex_indexs){
     rect_sprite=rct_spr;
    texture_index=tex_indexs;

}

SDL_Rect Sprite::get_rect_sprite(){
    return  rect_sprite;
 }
vector<int> Sprite::get_TextureIndex(){
   return texture_index;
}

Tile::Tile(){
     num_layers=0;
     width=0;
     heigth=0;
     x=0;
     y=0;
     validado=false;

 }
Tile::Tile(int w,int h,int tx,int ty ){
    num_layers=0;
    width=w;
    heigth=h;
    x=tx;
    y=ty;
    validado=true;


}

bool Tile::is_valid(){
    return validado;
}
map<int,Sprite> Tile:: get_Sprites_data(){

    return layers_sprite;

}
void Tile::add_sprite(int layer,Sprite spr){
     layers_sprite[layer]=spr;
     num_layers+=1;
}
int Tile::get_count_layers(){
   return num_layers;
}

vector<Tile> Tile:: mapa_escena;

void Tile::set_tiles(vector<Tile> tls){
   mapa_escena=tls;
}
vector<Tile>& Tile::get_tiles(){
    return mapa_escena;
}

SDL_Rect Tile::get_rect(){

    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    rect.w=width;
    rect.h=heigth;

    return rect;
 }
bool Texture::is_loaded(){
   return texture_loaded;
}
bool Texture::is_multiple(){
   return multiple;
}
SDL_Rect Texture:: get_rect(int gid){
   SDL_Rect res;
   int temp_id=gid-firstGid+1;
   int row_sprite=-1;
   int col_sprite=-1;
   int temp_col=1;
   int num_ciclos=0;

   while(col_sprite==-1){

       if(temp_id>=temp_col && temp_id<=temp_col+(columns-1)){

          col_sprite=num_ciclos;
       }
       else{
        temp_col+=columns;
        num_ciclos+=1;
       }
       if(temp_col>tile_count){
         col_sprite=-2;
       }
   }

  if(col_sprite>=0){



      row_sprite=gid-((col_sprite*columns)+1);
      if(row_sprite>=columns){

        row_sprite=(gid-(firstGid))-(col_sprite*columns);

      }


      res.x=row_sprite*(sprites_width+spacing)+margin;
      res.y=col_sprite*(sprites_heigth+spacing)+margin;
      res.w=sprites_width;
      res.h=sprites_heigth;
  }

   return res;

}
void Texture::liberar_texture(){

   for(int i=0;i<tex.size();i++){

     if(tex.at(i)!=NULL){
         SDL_DestroyTexture(tex.at(i));
         tex[i]=NULL;
     }

   }


}
Texture::Texture(){
   firstGid=-1;
   texture_loaded=false;
   margin=0;
   multiple=false;
}

Texture::Texture(int firstid,string src,SDL_Renderer* renderer){

   firstGid=firstid;
   texture_loaded=false;
   multiple=false;
   margin=0;
   read_tsx(src,renderer);

}
Texture::Texture(string src,SDL_Renderer* render){

     firstGid=-1;
     src=src.substr(src.find_last_of("/\\") + 1);
     src=string(SDL_GetBasePath())+"assets/images/"+src;
     multiple=false;

     texture_loaded=false;
     if(src!=""){

                SDL_Surface* loadedSurface = IMG_Load(src.c_str() );
                if(loadedSurface==NULL ){
                      printf(("failed to load image:"+src+"\n").c_str());

                  }
                else{
                    tex.push_back(SDL_CreateTextureFromSurface(render,loadedSurface));
                    if(tex.at(0)!=NULL){

                        texture_loaded=true;

                    }
                    else{
                         printf(("failed to create texture:"+src+"\n").c_str());

                    }
                }
                SDL_FreeSurface(loadedSurface);


            }
 }
void Texture::read_tsx(string src,SDL_Renderer* renderer){

       ifstream stream;
       string linea="";
       stream.open(src,ios::in);
       if(stream.good()){

         map<string,string> parametros;
         map<int,string> sources_list;
         string temp_sourceID="";
         int num_images=0;
         string temp_name="";
         string temp_parameter="";
         string temp_data="";
         bool failed=false;

         getline(stream,linea);

         while(getline(stream,linea)){
            int start_index=-1;
            for(int s=0;s<linea.length();s++){

                if(linea[s]=='<'){
                    start_index=s+1;
                   s=linea.length();
                }
            }
            if(start_index!=-1){
                if(linea[start_index+1]!='/'){

                    for(int i=start_index;i<linea.length();i++){
                        if(linea[i]==' '){
                            if(temp_name==""){
                                temp_name=temp_data;
                                temp_data="";
                            }
                        }
                        else if(linea[i]=='"' ){

                             if(temp_parameter==""){

                              temp_data.erase(temp_data.begin()+(temp_data.length()-1));
                              temp_parameter=temp_data;
                              temp_data="";

                         }
                          else{
                            if(temp_name=="tile"){
                                if(multiple==false){
                                    multiple=true;
                                }
                                temp_sourceID=temp_data;
                            }
                            else if(temp_name!="animation" && temp_name!="tile" && temp_name!="frame"){
                                 if(temp_parameter=="source" && temp_sourceID!=""){

                                    sources_list[num_images]=temp_data;
                                    temp_sourceID="";
                                    num_images+=1;
                                 }
                                 else{
                                     parametros[temp_parameter]=temp_data;
                                 }
                             }
                             temp_data="";
                             temp_parameter="";
                          }



                      }
                      else if(linea[i]=='>'){
                        temp_data="";
                        temp_name="";
                      }
                      else{
                          temp_data+=linea[i];
                      }

                    }
                }
            }
            else{
                failed=true;
            }
        }
        if(failed==false){


            string src_tex=parametros["source"];
            src_tex=src_tex.substr(src_tex.find_last_of("/\\") + 1);
            src_tex=string(SDL_GetBasePath())+"assets/images/"+src_tex;


            tile_count=Math_G::StrToInt(parametros["tilecount"]);
            last_id=firstGid+tile_count-1;
            spacing=Math_G::StrToInt(parametros["spacing"]);
            columns=Math_G::StrToInt(parametros["columns"]);
            sprites_width=Math_G::StrToInt(parametros["tilewidth"]);
            sprites_heigth=Math_G::StrToInt(parametros["tileheight"]);

            if(multiple==true){

                for(int j=0;j<num_images;j++){

                    string src=sources_list[j];
                    src=src.substr(src.find_last_of("/\\") + 1);
                    src=string(SDL_GetBasePath())+"assets/images/"+src;

                    if(src!=""){
                        SDL_Surface* loadedSurface = IMG_Load(src.c_str() );
                        if(loadedSurface==NULL ){
                             printf(("failed to load image:"+src+"\n").c_str());

                         }
                         else{
                                SDL_Texture * t=SDL_CreateTextureFromSurface(renderer,loadedSurface);

                              if(t!=NULL){
                                    tex.push_back(t);
                                   texture_loaded=true;
                               }
                             else{
                                   printf(("failed to create texture:"+src+"\n").c_str());

                              }
                        }
                        SDL_FreeSurface(loadedSurface);

                    }
                }
            }
            else if(src_tex!="" && multiple==false){

                SDL_Surface* loadedSurface = IMG_Load(src_tex.c_str() );
                if(loadedSurface==NULL ){
                      printf(("failed to load image:"+src_tex+"\n").c_str());

                  }
                else{
                    tex.push_back(SDL_CreateTextureFromSurface(renderer,loadedSurface));
                    if(tex.at(0)!=NULL){

                        texture_loaded=true;
                    }
                    else{
                         printf(("failed to create texture:"+src_tex+"\n").c_str());

                    }
                }
                SDL_FreeSurface(loadedSurface);


            }




        }

         stream.close();
       }
       else{
          printf(("fail to read tsx file: "+src).c_str());
       }




}
SDL_Texture* Texture::get_texture(int index){
   if(multiple==false){
      index=0;
   }
   if(index>=0 && index<tex.size()){
     return tex.at(index);
   }
   return NULL;

}
int Texture::get_firstId(){
    return firstGid;
}
int Texture::get_lastId(){
    return last_id;
}
void Font_Manager::add_font(string name,string src,int sz){

        map<string,TTF_Font*>::iterator it;
        bool existe=false;
        for(it=fuentes.begin();it!=fuentes.end();it++){
            string nam=it->first;
            if(nam==name){
                existe=true;
                break;
            }
        }
        if(existe==false){


            TTF_Font* fuente=TTF_OpenFont(src.c_str(), sz);
            if(fuente!=NULL){

                 fuentes[name]=fuente;

            }
        }


}
TTF_Font * Font_Manager::get_font(string name){
       TTF_Font * res=NULL;

       map<string,TTF_Font*>::iterator it;

        for(it=fuentes.begin();it!=fuentes.end();it++){
            string nam=it->first;
            if(nam==name){
                res=it->second;
                break;
            }
        }
   return res;
}
map<string,TTF_Font*>Font_Manager::fuentes;
