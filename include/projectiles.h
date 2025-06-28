#ifndef PROJECTILES_H
#define PROJECTILES_H
#include<Game_Object.h>


enum PROYECTIL_TYPES{
   BALL=0, SHOOT=1 ,PIN_PONG_BALL=2,
};


class Proyectil{
public:
    Proyectil();
    Proyectil(map<string,string> args);
    Proyectil(float f,int t,float max_life,Vector2 dr,bool desactive,vector<string> sfx_asociados);
    void update(Game_Object*g);
    void restart(Game_Object*g);
    void OnTrigger(Game_Object other,Game_Object* g,ManiFold* contact_info);
    void  OnCollision(Game_Object other,Game_Object* g, ManiFold* contact_info);

    Vector2 update_ping_pong(Vector2 forc);

private:
    float base_force;
    int type_p;
    Vector2 initial_dir;
    float life_time;
    float max_life_time;
    bool desactive_ondie;
    bool reset_proy;

   vector<string> sfx;
};




#endif // PROJECTILES_H
