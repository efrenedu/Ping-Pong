#include "Render.h"

Render::Render()
{
    //ctor
    alpha_map=NULL;
    visible=false;
    use_AlphaMap=false;
    sprite=Sprite();
    layer=0;
}
 Render:: Render(Sprite spr,bool visibl,string alphaMap_src,int layr){
    visible=visibl;
    use_AlphaMap=false;
    alpha_map=NULL;
    sprite=spr;
    layer=layr;
    if(alphaMap_src!=""){

    }

 }
Render::~Render()
{
    //dtor
}
 Sprite& Render::get_sprite(){
    return sprite;
 }
 int Render::get_layer(){
    return layer;
 }
void Render::update_spriteRect(SDL_Rect r){
     sprite.set_Pos_Sprite(r);
}
void Render::make_alphaMap(){
   if(alpha_map!=NULL){
       SDL_DestroyTexture(alpha_map);
       alpha_map=NULL;
   }

}
bool Render::is_visible(){
   return visible;
}
SDL_Texture* Render::get_alphaMap(){
    if(alpha_map!=NULL){
        return alpha_map;
    }
    return NULL;
}

void Render::liberar(){
   if(alpha_map!=NULL){
       SDL_DestroyTexture(alpha_map);

   }
}
