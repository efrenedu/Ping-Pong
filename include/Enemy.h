#ifndef ENEMY_H
#define ENEMY_H
#include<Game_Object.h>

enum ENEMY_TYPES{
    PIN_PONG_RIVAL=0,CHESS_RIVAL=1,
};
enum ENEMY_DIFICULT{
   EASY=0,NORMAL=1,HARD=2,
   ELITE_EASY=4,ELITE_NORMAL=5,ELITE_HARD=6,
   SUB_BOSS_EASY=7,SUB_BOSS_NORMAL=8,SUB_BOSS_HARD=9,
   BOSS_EASY=10,BOSS_NORMAL=11,BOSS_HARD=12,
   FINAL_BOSS_EASY=12,FINAL_BOSS_NORMAL=13,FINAL_BOSS_HARD=14

};
class Enemy
{
    public:
        Enemy();
        Enemy(int tp,Vector2 vl,float slp,int dific,string targ,float d_targ);
        Enemy(map<string,string> datos);
        void update(Game_Object* g);
        void IA_PONG(Game_Object* g);
        void add_hitPlayer();
        void set_dificultad(int dif);
        void OnTrigger(Game_Object other,Game_Object* g,ManiFold* contact_info);
        void  OnCollision(Game_Object other,Game_Object* g, ManiFold* contact_info);
        void reset();
        void add_score(int val);
        int get_score();
        virtual ~Enemy();

    protected:

    private:
        int type_enem;
        Vector2 base_vel;
        float sleep_time;
        float max_sleep;
        int dificult;
        string target;
        float min_distTarget;
        int hits_player;
        int hits;
        int score;

};

#endif // ENEMY_H
