#ifndef DATA_RENDER_H
#define DATA_RENDER_H
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>
#include<map>
#include<vector>
#include<tuple>
#include<Camera.h>
#include<fstream>
#include<iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>



using namespace std;




class Texture{

public:
    Texture();
    Texture(int firstid, string src,SDL_Renderer* renderer);
    Texture(string src,SDL_Renderer* render);
    void liberar_texture();

    void read_tsx(string src,SDL_Renderer* renderer);
    bool is_loaded();
    int get_firstId();
    int get_lastId();
    SDL_Rect get_rect(int gid);
    SDL_Texture* get_texture(int index);
    bool is_multiple();

private:
    int firstGid;
    int last_id;
    int spacing;
    int columns;
    int tile_count;
    int sprites_width;
    int sprites_heigth;
    bool texture_loaded;
    int margin;
    vector<SDL_Texture*> tex;
    bool multiple;
    int num_texturas;
};


struct Sprite{
public:
    Sprite();
    Sprite(SDL_Rect rct_spr,vector<int> tex_index);
    void set_Pos_Sprite(SDL_Rect rct_spr);
    void change_texture(SDL_Rect rct_spr,vector<int> tex_index);
    SDL_Rect get_rect_sprite();
    vector<int> get_TextureIndex();
    bool is_empty_sprite();

private:
    vector<int> texture_index;
    SDL_Rect rect_sprite;
    bool empty_sprite;


};

class Texture_manager{
public:
    static void set_textures(vector<Texture> lista );
    static Sprite get_sprite(int gId);
    static void reset_textures();
    static SDL_Texture* get_texture(int index,int index2);
    static vector<int> get_indexs(SDL_Rect *r,int gId);

private:
    static vector<Texture> lista_texturas;

};




struct Tile{
public:
    Tile();
    Tile(int w,int h,int tx,int ty);
    void add_sprite(int layer,Sprite spr);
    map<int,Sprite> get_Sprites_data();
    bool is_valid();
    SDL_Rect get_rect();
    int get_count_layers();

    static void set_tiles(vector<Tile> tls);
    static vector<Tile>& get_tiles();


private:
    int num_layers;
    int width;
    int heigth;
    int x;
    int y;
    map<int,Sprite> layers_sprite;
    bool validado;

    static vector<Tile> mapa_escena;


};


class Font_Manager{

public:
    static void add_font(string name,string src,int sz);
    static TTF_Font* get_font(string name);

private:
   static map<string,TTF_Font*> fuentes;


};
#endif // DATA_RENDER_H
