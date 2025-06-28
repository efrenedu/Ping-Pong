#include "Physics.h"
#include "Utility.h"


Physic_Material::Physic_Material(){

    friction=1.0f;
    rebote_factor=0.0f;
    dinamic_friction=1.0f;
}

Physic_Material::Physic_Material(float frictn,float dinamic_fr,float rebote){

    friction=frictn;
    rebote_factor=rebote;
    dinamic_friction=dinamic_fr;
}

float Physic_Material::get_friction(){
   return friction;
}
float Physic_Material::get_rebote(){
   return rebote_factor;
}
float Physic_Material::get_dinamic_friction(){
  return dinamic_friction;
}


Collider::Collider(){

   figure_Collider=Figure();
   material=Physic_Material();
   trigger=false;
   layer=0;
   active=false;
   offset_pivot=Vector2(0,0);

}
Collider::Collider(Figure f,Physic_Material mat,bool is_trigger,int layr){
   figure_Collider=f;
   material=mat;
   trigger=is_trigger;
   layer=layr;
   active=true;
   offset_pivot=Vector2(0,0);
}
Vector2 Collider::get_pivot(){
    Vector2 pivote=figure_Collider.get_center();
    pivote=pivote+offset_pivot;
    return pivote;
}
void Collider::set_offsetPivot(Vector2 v){
     offset_pivot=v;
}


Figure Collider::get_figure(){
    return figure_Collider;
}
Physic_Material Collider::get_material(){
    return material;
}
int Collider::get_layer(){
    return layer;
}
bool Collider::is_trigger(){
   return trigger;
}
bool Collider::is_active(){
   return active;
}
void Collider::set_trigger(bool val){
   trigger=val;
}

RigidBody::RigidBody(){

  mass=0.0f;
  last_force=Vector2(0,0);
  active=false;
  ground=false;
  pendiente=false;
  use_gravity=false;
  max_angle_pendiente=0.0f;
  coyote=false;
  coyote_max_time=0.0f;
  last_normFloor=Vector2(0,0);
  in_coyote=false;
  Inertia=1.0f;
  angular_step=0.0f;

}
RigidBody::RigidBody(float masa,bool use_grav,float angle_pend,bool coyot,float coyot_t,float Inert,float angular_s){

  mass=masa;
  last_force=Vector2(0,0);
  active=true;
  use_gravity=use_grav;
  ground=false;
  pendiente=false;
  max_angle_pendiente=angle_pend;
  coyote=coyot;
  coyote_max_time=coyot_t;
  last_normFloor=Vector2(0,0);
  in_coyote=false;
  Inertia=Inert;
  angular_step=angular_s;

}
float RigidBody::get_angularStep(){
   return angular_step;
}
Vector2 RigidBody::get_angularVel(){
   return angular_vel;
}
void RigidBody::update_torque(){
       Vector2 resistencia=Vector2(angular_vel.x*Inertia,angular_vel.y*Inertia);
       angular_vel=Vector2(angular_vel.x-(resistencia.x*Timer::get_delta_time()),angular_vel.y-(resistencia.y*Timer::get_delta_time()));


}


void RigidBody::add_torque(){


   if(lock_rot==true){
        angular_vel=Vector2(0,0);
        return;
   }
      Vector2 resistencia=Vector2(last_force.x*Inertia,last_force.y*Inertia);
      angular_vel=Vector2(angular_vel.x+((last_force.x-resistencia.x)*Timer::get_delta_time()),angular_vel.y+((last_force.y-resistencia.y)*Timer::get_delta_time()));



     if(lock_pos.x!=0){
          angular_vel.x=0;
     }
     if(lock_pos.y!=0){
        angular_vel.y=0;
     }
      angular_vel.x=Math_G::Clamp(angular_vel.x,-10,10);
      angular_vel.y=Math_G::Clamp(angular_vel.y,-10,10);




}


 void RigidBody::set_coyote(bool val){
    in_coyote=val;
 }
bool RigidBody::get_coyote_state(){
   return in_coyote;
}
 bool RigidBody::is_active(){
     return active;

 }
 Vector2 RigidBody::get_lastNorm_Floor(){
    return last_normFloor;
 }
void RigidBody::set_NormFloor(Vector2 n){
    last_normFloor=n;
}
Vector2 RigidBody:: get_last_force(){

    return last_force;
}
void RigidBody::set_force(Vector2 f){

   last_force=f;
}
void RigidBody::add_force(Vector2 f){
    last_force=last_force+f;

}
float RigidBody::get_mass(){
    return mass;
}
bool RigidBody::apply_gravity(){
   return use_gravity;
}
void RigidBody::set_Ground(bool val){
   ground=val;
}
bool RigidBody::OnGround(){
   return ground;
}
bool RigidBody::in_pendiente(){
  return pendiente;
}
void RigidBody::set_pendiente(bool pend){

   pendiente=pend;
}
float RigidBody::get_angle_pendiente(){
   return max_angle_pendiente;
}
bool RigidBody::use_coyote_time(){
  return coyote;
}
float RigidBody::get_coyote_time(){
    return coyote_max_time;
}
Vector2  RigidBody::get_lock_Position(){
    return lock_pos;
}
bool  RigidBody::get_lock_rot(){
  return lock_rot;

}
bool  RigidBody::is_kinematic(){
   return kinematic;
}
void  RigidBody::set_kinematic(bool val){
    kinematic=val;

}
void  RigidBody::set_lockPos(Vector2 lock){
   lock_pos=lock;
}
void  RigidBody::set_lockRot(bool val){
   lock_rot=val;
}

ManiFold:: ManiFold(){
num_contactos=0;
penetracion=0;


}


Vector2 ManiFold::get_normal(){
   return normal_direccion;
}
vector<Vector2> ManiFold::get_contact(){
   return contact_points;
}
void ManiFold::set_normal(Vector2 norm){
   normal_direccion=norm;
}
void ManiFold::set_contacts(vector<Vector2> contacts){
    if(contact_points.size()>0){
        contact_points.clear();
    }
   contact_points=contacts;
   num_contactos=contacts.size();

}
void ManiFold::set_penetracion(float penetr){
   penetracion=penetr;
}
float ManiFold::get_penetracion(){
   return penetracion;
}

