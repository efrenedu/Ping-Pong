#ifndef PHYSIC_ENGINE_H
#define PHYSIC_ENGINE_H
#include<Scene_data.h>

class Physic_Engine
{

    public:

        static void update_physic( vector<Game_Object>& objs);
        static Vector2 Aplicar_fuerzas_RigidCollider(RigidBody rA,Collider colA,Collider colB,Vector2 forceA,Vector2 forceB, Vector2* f_fix,ManiFold* fold);
        static void Aplicar_fuerzas_RigidvsRigid(RigidBody rA,RigidBody rB,Collider colA,Collider colB,Vector2* forceA,Vector2* forceB, Vector2* f_fixA,Vector2* f_fixB,vector<Vector2> recibe_forces,ManiFold* fold);
        static bool Raycast_Collision(Ray& ray,string asociado);

    protected:

    private:
        static int num_steps;
        static float multiplic_step;


        static float gravedad;
        static bool use_gravity;
        static bool Collision_square_square(Collider A, Collider B,Vector2 posA,Vector2 posB,int angA,int angB,ManiFold *fold);
        static bool Collision_square_square(Vector2 A_size,Vector2 B_size,Vector2 A_center,Vector2 B_center);

        static bool Collision_Circle_Circle(Collider A, Collider B,Vector2 posA,Vector2 posB,ManiFold *fold);
        static bool Collision_circle_Polygon(Collider A, Collider B,Vector2 posA,Vector2 posB,int angB,ManiFold *fold);
        static bool Collision_Polygon_Polygon(Collider A, Collider B,Vector2 posA,Vector2 posB,int angA,int angB,ManiFold *fold);
        static bool in_Collision(Game_Object A,Game_Object B,Vector2 next_forceA,Vector2 next_forceB,Collider colA,Collider colB, ManiFold* fold);

        static bool Collision_Circle_Square(Collider A, Collider B,Vector2 posA,Vector2 posB,int angB,ManiFold *fold);
        static bool Collision_Ray_Line(Ray& ray, Line segmento);

};

#endif // PHYSIC_ENGINE_H
