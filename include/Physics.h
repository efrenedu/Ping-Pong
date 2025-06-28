#ifndef PHYSICS_H
#define PHYSICS_H
#include<Figure.h>

struct Physic_Material{

public:
    Physic_Material();
    Physic_Material(float frictn,float dinamic_fr,float rebote);
    float get_friction();
    float get_dinamic_friction();
    float get_rebote();
private:
    float friction;
    float dinamic_friction;
    float rebote_factor;

};
struct Collider{
public:
    Collider();
    Collider(Figure f,Physic_Material mat,bool is_trigger,int layr);
    bool is_active();
    Figure get_figure();
    Physic_Material get_material();
    int get_layer();
    bool is_trigger();
    void set_trigger(bool val);
    Vector2 get_pivot();
    void set_offsetPivot(Vector2 v);


private:
    Figure figure_Collider;
    Physic_Material material;
    bool trigger;
    int layer;
    bool active;
    Vector2 offset_pivot;

};


struct RigidBody{
public:
    RigidBody();
    RigidBody(float masa,bool use_grav,float angle_pend, bool coyot,float coyot_t,float Inert,float ang_s);
    void set_force(Vector2 f);
    void add_force(Vector2 f);
    float get_mass();
    bool is_active();
    Vector2 get_last_force();
    bool OnGround();
    void set_Ground(bool val);
    bool apply_gravity();
    bool in_pendiente();
    void set_pendiente(bool pend);
    float get_angle_pendiente();
    bool use_coyote_time();
    float get_coyote_time();
    Vector2 get_lastNorm_Floor();
    void set_NormFloor(Vector2 n);
    void set_coyote(bool val);
    bool get_coyote_state();
    Vector2 get_lock_Position();
    bool get_lock_rot();
    bool is_kinematic();
    void set_kinematic(bool val);
    void set_lockPos(Vector2 lock);
    void set_lockRot(bool val);
    void add_torque();
    void update_torque();
    Vector2 get_angularVel();
    float get_angularStep();

private:
    float mass;
    bool active;
    Vector2 last_force;
    bool ground;
    bool pendiente;
    bool use_gravity;
    float max_angle_pendiente;
    bool coyote;
    float coyote_max_time;
    Vector2 last_normFloor;
    bool in_coyote;
    bool kinematic;
    bool lock_rot;
    Vector2 lock_pos;
    float Inertia;
    Vector2 angular_vel;
    float angular_step;


};


struct ManiFold{
public:
    ManiFold();
    void set_normal(Vector2 norm);
    void set_contacts(vector<Vector2> contacts);
    void set_bodies(vector<RigidBody> rbs);
    void set_penetracion(float penetr);
    Vector2 get_normal();
    vector<Vector2> get_contact();
    vector<RigidBody> get_bodies();
    float get_penetracion();
private:
    RigidBody first;
    RigidBody second;
    Vector2 normal_direccion;
    vector<Vector2> contact_points;
    int num_contactos;
    float penetracion;

};




#endif // PHYSICS_H
