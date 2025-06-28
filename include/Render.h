#ifndef RENDER_H
#define RENDER_H
#include <data_render.h>


class Render
{
    public:
        Render();
        Render(Sprite spr,bool visibl,string AlpMap_src,int layr);

        Sprite& get_sprite();
        void update_spriteRect(SDL_Rect r);
        void make_alphaMap();
        SDL_Texture *get_alphaMap();
        bool is_visible();
        void liberar();
        int get_layer();


        virtual ~Render();

    protected:

    private:
        int layer;
        Sprite sprite;
        bool visible;
        bool use_AlphaMap;
        SDL_Texture *alpha_map;
};

#endif // RENDER_H
