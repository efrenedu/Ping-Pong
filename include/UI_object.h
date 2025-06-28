#ifndef UI_OBJECT_H
#define UI_OBJECT_H
#include<Game_Object.h>


enum UI_TYPES{
   BUTTON=0, TEXTFIELD=1, IMAGE=2, LABEL=3, RADIOBUTTON=4, CHECKBUTTON=5, SCROLLBAR=6, VALUEBAR=7

};
enum BUTTON_MODES{
   ONLY_KEYBOARD=0,ONLY_MOUSE=1,

};

struct Data_Text{

   public:
       Data_Text();
       Data_Text(string text,string font,int sz,SDL_Color letra_color,Vector2 offset_f,int min_chars);
       string get_text();
       string get_font();
       Vector2 get_offset();
       int get_size();
       SDL_Color get_color();
       void set_color(SDL_Color colr);
       void set_text(string value);
       void set_offset(Vector2 offset_f);



   private:
      string texto;
      string fuente;
      int size_letras;
      SDL_Color color;
      Vector2 initial_offset;
      Vector2 offset_font;
      int minimo_characters;


};

struct Icon_UI{
public:
    Icon_UI();
    Icon_UI(SDL_Rect r,vector<int> indexs);
    SDL_Rect get_rect();
    vector<int> get_indexs();
private:
    vector<int> tex_index;
    SDL_Rect rect;

};


class UI_object

{
    public:
        UI_object();
        UI_object(vector<string> sfx);
        virtual ~UI_object();
        void update(Game_Object* g);
        string get_audioName(int index);
        void set_eventsClick(string data);
        void set_eventsSelected(string data);



        static vector<Game_Object>& get_UI();
        static void set_UI(vector<Game_Object>& objects);
        static Game_Object Find(string name,int& index);
        static void EditUI(Game_Object g,int index);
        static void add_UI(Game_Object g);
        static void Destroy_UI(Game_Object g,int index);
        static map<int,Game_Object> get_radioGroup(string id);


        static void limpiar_UI();


    protected:
        bool mouse_collision(SDL_Rect r);
        bool in_mouseCollision;
        void on_click(Game_Object *g);
        void on_enter(Game_Object *g);
        void on_exit(Game_Object *g);
        void on_drag(Game_Object *g);
        map<int,string> get_events_selected();
        map<int,string> get_events_click();


    private:
        int layer;
        static vector<Game_Object>UI_escena;
        vector<string> audios_asociado;
        map<int,string> evs_click;
        map<int ,string> evs_selected;



};


class Button:UI_object{
     public:
         Button();
         Button(float esper,vector<Icon_UI> icons,vector<SDL_Color> colors_text,vector<Vector2> dims,vector<string> sfx);
         Data_Text get_textData(bool& with_text);
         void add_text(Data_Text t);
         void reset(Game_Object *g);
         void update(Game_Object* g);
         void set_eventsClick(string data);
        void set_eventsSelected(string data);
     protected:
         void execute_events();
         void on_click(Game_Object *g);
         void on_enter(Game_Object *g);
         void on_exit(Game_Object *g);
     private:
         float max_esperaEv;
         float espera;
         bool has_text;
         Data_Text text_d;
         vector<SDL_Color> text_colors;
         vector<Icon_UI> iconos;
         vector<Vector2> dims_text;




};

class Label:UI_object{

  public:
      Label();
      Label(float esper,string sel_obj,SDL_Color select_clr,Data_Text t,vector<string> sfx);
      Data_Text get_textData(bool& with_text);
      void set_textColor(SDL_Color colr);
      void set_text(string val);
      void set_selected(bool val,string obj_name);
      void trigger_label(string obj_name);
      void reset();
      void update();
      void set_eventsClick(string data);
        void set_eventsSelected(string data);

 private:
       void execute_events();
      SDL_Color init_color;
      SDL_Color select_color;
      Data_Text text_d;
      bool selected;
      float espera;
      float espera_inicio;
      float max_espera;
      string obj_select_Asoc;





};
class Image_UI:UI_object{
public:
    Image_UI();
    Image_UI(vector<string> sfx);
    void change_image(Game_Object *g,int gId);
    void change_image(Game_Object *g,string text_name,bool multiple=false);
    void add_text(Data_Text t);
    Data_Text get_textData(bool& with_text);
    void set_eventsClick(string data);
    void set_eventsSelected(string data);
private:
    bool has_text;
    Data_Text text_d;



};

class RadioButton:UI_object{
   public:
       RadioButton();
       RadioButton(vector<Icon_UI> icons,bool select,string value,string rId,vector<string> sfx);
       bool is_selected();
       string get_selectedValue();
       void update(Game_Object* g);
       void reset(Game_Object *g);
       void set_selected(bool val);
       void set_text(Data_Text t);
       string get_radioId();
       Data_Text get_textData(bool& with_text);
       void set_eventsClick(string data);
        void set_eventsSelected(string data);

   protected:
         void on_click(Game_Object *g);
         void on_enter(Game_Object *g);
         void on_exit(Game_Object *g);
         void execute_events();

   private:
      bool selected;
      bool init_selected;
      string radioId;
      string radioValue;
      vector<Icon_UI> iconos;
      Data_Text text_d;
      bool activ_pend;
      bool has_text;
      map<int,string>evs;

};

class CheckButton:UI_object{
 public:
    CheckButton();
    CheckButton(vector<Icon_UI> icons,bool select,string value,Data_Text t,vector<string> sfx);
    bool is_selected();
    string get_selectedValue();
    Data_Text get_textData(bool& with_text);
    void reset(Game_Object *g);
    void set_selected(bool val);
    void update(Game_Object* g);
protected:
      void on_click(Game_Object *g);
      void on_enter(Game_Object *g);
      void on_exit(Game_Object *g);
 private:

      bool selected;
      bool init_select;
      string checkValue;
      vector<Icon_UI> iconos;
      Data_Text text_d;
};

class ValueBar:UI_object{
 public:
     ValueBar();
     ValueBar(int sentd,float inicial_value,vector<Icon_UI> estads,bool cut,vector<string> sfx);
     void set_value(float val);
     float get_value();
     Vector2 get_sentido();
     void reset(Game_Object *g);
     void add_text(Data_Text t);
     Data_Text get_textData(bool& with_text);
     void set_textColor(SDL_Color colr);
     void add_iconBg(Icon_UI icono);
     void add_BgColors(vector<SDL_Color> colors);
     Icon_UI get_BgIcon(bool& with_icon);
     vector<SDL_Color> get_BgColors(bool& withColors);
     void update(Game_Object* g);
     bool cut_sprite();
 private:
    float value;
    float init_val;
    Vector2 sentido;
    bool has_text;
    Data_Text text_d;
    vector<SDL_Color> bg_colors;
    Icon_UI icon_bg;
    bool has_colors;
    bool has_icono;
    vector<Icon_UI> estados;
    bool cut_src;

};

class ScrollBar:UI_object{
 public:
     ScrollBar();
     ScrollBar(bool r_desplz,int sent,SDL_Color bg_color,SDL_Color fg_color,vector<Icon_UI> icons,SDL_Rect bg_r,Vector2 obj_dims,vector<string> sfx);
     void update(Game_Object* g);
     Vector2 get_sentido();
     void reset(Game_Object *g);
     void on_drag(Game_Object* g);
     void on_enter(Game_Object* g);
     void on_exit(Game_Object* g);
     float get_desplazamiento();
     void set_desplazamiento(float val,Game_Object* g);
     void activate_rastro(SDL_Color clr);
     bool use_rastro(SDL_Color* clr);
     SDL_Rect get_bgRect();
     void move_bar(int dir,float val,string obj_name);
     vector<SDL_Color> get_BGcolors();
 private:
    Vector2 sentido;
    bool selected;
    vector<SDL_Color> colors_bg;
    SDL_Rect bg_rect;
    vector<Icon_UI> iconos;
    Vector2 init_pos;
    float desplz;
    bool reset_desplz;
    bool show_rastro;
    SDL_Color rastro_color;

};


#endif // UI_OBJECT_H
