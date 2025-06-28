#include "Camera.h"
#include "Scene_data.h"
#include "Component.h"

vector<Camera> Camera::cameras_list;
int Camera::cam_activa=-1;
float Camera::distance_update=2.0f;
tuple<vector<int>,vector<int>> Camera::in_camDraw;

Camera::Camera()
{
    //ctor
    active=false;
    Center=Vector2(0,0);
    Half_Area=Vector2(0,0);
    zoom=0.0f;
    looking=-1;
    offset=Vector2(0,0);
    last_target_pos=Vector2(0,0);
}

Camera::~Camera()
{
    //dtor
}
Camera::Camera(Vector2 c,Vector2 half_ar,bool actv,string look){
     Center=c;
     Half_Area=half_ar;
     active=actv;
     zoom=0.0f;
     looking=-1;
     look_name=look;
     offset=Vector2(0,0);
      last_target_pos=Vector2(0,0);
}

void Camera:: move_camera(Vector2 desplz){
     Center=Center+desplz;
}
void Camera:: apply_zoom(float val){
   zoom=zoom+val;
}
float Camera:: get_zoom(){
   return zoom;
}
Vector2 Camera:: get_pos(){
    return Center;
}
void Camera:: set_pos(Vector2 c){
    Center=c;
}
tuple<vector<int>,vector<int>> Camera::in_cam;

Vector2 Camera::get_lastTarget_pos(){
    return last_target_pos;
}
void Camera::set_lastTargetPos(Vector2 p){
     last_target_pos=p;
}
void Camera::update_camActive(bool update_Quat){

    if(cameras_list.size()<=0){
        return;
    }

    if(cam_activa<0 || cam_activa>=cameras_list.size()){
         return;
    }
    Camera camA=cameras_list[cam_activa];

    if(camA.is_active()!=true){
        return;
      }

      bool mover_cam=false;
      int look_index=camA.get_looking_index();
      vector<Game_Object> objs=Game_Object::get_objs();
      if(look_index>=0 && look_index<objs.size() ){
        Game_Object target_cam=objs[look_index];
        Transform* target_trans=Component::get_Transform(target_cam.get_name());
       if(target_trans!=NULL){
          Vector2 pos_target=target_trans->Get_Position();
          float dif=(pos_target-camA.get_lastTarget_pos()).magnitud();
          if(dif>=distance_update){
            camA.set_lastTargetPos(pos_target);
            camA.look_at(pos_target);
            mover_cam=true;
            cameras_list[cam_activa]=camA;
          }

       }
    }

    if(mover_cam==true || update_Quat==true){

         Vector2 c=camA.get_pos();
         Vector2 hArea=camA.get_Half_dim();
         float r=(hArea.x>hArea.y)?hArea.x:hArea.y;
         r=r*2.0f;
         in_camDraw=Quatree::get_elements_in_radious(c,r);
         r=r*1.5f;
         in_cam=Quatree::get_elements_in_radious(c,r);


    }






}

tuple<vector<int>,vector<int>>& Camera::get_inCam(){
   return in_cam;
}

bool Camera:: in_camera(SDL_Rect rect_obj){

   bool res=false;
   rect_obj.y=rect_obj.y*-1;   //conversion coord de pantalla a coord cartesianas


   if(Center.x+Half_Area.x>=rect_obj.x && Center.x-Half_Area.x<=rect_obj.x+rect_obj.w){
      if(Center.y-Half_Area.y<=rect_obj.y && Center.y+Half_Area.y>=rect_obj.y-rect_obj.w){

          res=true;
      }

   }
   return res;

}
bool Camera::is_active(){
   return active;
}
Vector2 Camera::get_Half_dim(){
   return Half_Area;
}
void Camera::set_looking_index(int index){
   looking=index;
}
int Camera::get_looking_index(){
   return looking;
}
string Camera::get_looking_target()
{
    return look_name;
}
void  Camera::update_tree(){
    if(cam_activa>=0 && cam_activa<cameras_list.size()){
         Camera camA=cameras_list[cam_activa];
         Vector2 c=camA.get_pos();
         Vector2 hArea=camA.get_Half_dim();
         float r=(hArea.x>hArea.y)?hArea.x:hArea.y;
         r=r*2.0f;
         in_camDraw=Quatree::get_elements_in_radious(c,r);
         r=r*1.5f;
         in_cam=Quatree::get_elements_in_radious(c,r);


    }
}
void Camera::set_camaras(vector<Camera>cams,int active_cam){

    if(cameras_list.size()>0){
         cameras_list.clear();
    }
    cameras_list=cams;
    cam_activa=active_cam;


}
Camera& Camera::get_ActiveCamera(bool& found){
    found=false;
    if(cam_activa>=0 && cam_activa<cameras_list.size()){
        if(cameras_list.size()>0){
            found=true;
              return cameras_list[cam_activa];
        }
    }

    Camera res=Camera();
    return res;

}
void Camera::look_at(Vector2 target){


   Vector2 p=target;
   set_pos(p);
}
void Camera::editActiveCamera(Camera c){
    if(cam_activa>=0 && cam_activa<cameras_list.size()){
          if(cameras_list.size()>0){
               cameras_list[cam_activa]=c;
          }
    }
}
tuple<vector<int>,vector<int>>& Camera::get_inCamDraw(){
   return in_camDraw;
}

