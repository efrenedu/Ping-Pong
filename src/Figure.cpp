#include "Figure.h"

 Figure::Figure(){
    id=-1;
    dimension=Vector2(0,0);
    centro=Vector2(0,0);
    radio=0.0f;

 }
 Figure::Figure(vector<Vector2> pts,Vector2 offset){
   // poligonos
    id=2;
    points=pts;


    vector<Vector2>::iterator it;
    vector<Vector2>::iterator it_init;
    float max_x=-1000.0f;
    float min_x=1000.0f;
    float max_y=-1000.0f;
    float min_y=1000.0f;
    int index=0;

    int index_p=0;

    for(it_init=points.begin();it_init!=points.end();it_init++){
            Vector2 p=*it_init;

            p=p-offset;
            points.at(index_p)=p;

            index_p=index_p+1;

    }


    for(it=points.begin();it!=points.end();it++){
            Vector2 p=*it;

            if(index+1<points.size()){
                Vector2 p1=p;
                Vector2 p2=points.at(index+1);

                float x_dist=p2.x-p1.x;
                float y_dist=p2.y-p1.y;
                Vector2 norm=Vector2(-y_dist,x_dist);
                norm=norm.normalize();
                normals.push_back(norm);
            }
            else{
                Vector2 p1=p;
                Vector2 p2=points.at(0);

                float x_dist=p2.x-p1.x;
                float y_dist=p2.y-p1.y;
                Vector2 norm=Vector2(-y_dist,x_dist);
                norm=norm.normalize();
                normals.push_back(norm);
            }

            if(p.x>max_x){
                max_x=p.x;
            }
            if(p.y>max_y){
                max_y=p.y;
            }
            if(p.x<min_x){
                min_x=p.x;
            }
            if(p.y<min_y){
                min_y=p.y;
            }
            index+=1;
        }
    float size_x=abs(min_x-max_x);
    float size_y=abs(min_y-max_y);
    dimension=Vector2(size_x,size_y);
    halfDim=Vector2(size_x*0.5f,size_y*0.5f);
    centro=Vector2(size_x*0.5f,(size_y*0.5f)*-1);
    radio=(size_x>size_y)?size_x*0.5f:size_y*0.5f;


 }
  Figure::Figure(Vector2 dim,Vector2 offset){
     //cuadrado
     id=1;
     centro=Vector2(dim.x*0.5f,(dim.y*0.5f)*-1);
     centro=centro+offset;
     dimension=dim;
     radio=(dim.x>dim.y)?dim.x*0.5f:dim.y*0.5f;
     halfDim=Vector2(dim.x*0.5f,dim.y*0.5f);

     for(int i=0;i<4;i++){

        Vector2 p=Vector2(0,0);
        Vector2 norm=Vector2(0,0);
        if(i==0){
           norm.y=1;
        }
        else if(i==1){

            p.x=dim.x;
            norm.x=1;
        }
        else if(i==2){
            p.y=-dim.y;
            p.x=dim.x;
            norm.y=-1;
        }
        else{
            p.y=-dim.y;
            norm.x=-1;
        }

        normals.push_back(norm);
        points.push_back(p);
     }
  }

Figure::Figure(float radius,Vector2 offset){
//para circulos
    id=0;
    radio=radius;
    centro=Vector2(radio,-radio);
    centro=centro+offset;
    dimension=Vector2(radio*2,radio*2);
    halfDim=Vector2(radio,radio);

}


 vector<Vector2> Figure::get_normals(){

        vector<Vector2> norms;
        if(id!=0){
              norms=normals;
        }
        return norms;

 }
 int Figure:: get_type(){
     return id;
 }

vector<Vector2> Figure:: get_vertices(){
   return points;
}
Vector2 Figure::get_center(){
    return centro;
}
Vector2 Figure::get_dim(){
    return dimension;
}
float Figure::get_radius(){
   return radio;

}
Vector2 Figure::get_halfDim(){
  return halfDim;
}
int Figure::get_near_vertex_Index(Vector2 pt,Vector2 offset_figure){
    vector<Vector2>::iterator it;
    int index=0;
    int best_index=0;
    float best_dist=10000;

     for(it=points.begin();it!=points.end();it++){
        Vector2 vertice=*it;
        vertice=vertice+offset_figure;
        float dist=(pt-vertice).magnitud();
        if(dist<best_dist){
            best_dist=dist;
            best_index=index;
        }
        index=index+1;
     }
     return best_index;
}

void Figure::project_vertices(Vector2 axis,float* minF,float *maxF,Vector2 offset){

   float max_val=-10000.0f;
   float min_val=10000.0f;
   vector<Vector2> ::iterator it;
   for(it=points.begin();it!=points.end();it++){
       Vector2 v=*it;
       v=v+offset;
       float proj=v.dot(axis);
       if(proj<min_val){
           min_val=proj;
       }
       if(proj>max_val){
           max_val=proj;
       }
   }

   *minF=min_val;
   *maxF=max_val;

}
void Figure::project_Circle(Vector2 axis,float* minF,float *maxF,Vector2 offset){

   float max_val=(centro+offset).dot(axis)-radio;
   float min_val=(centro+offset).dot(axis)+radio;
   if(min_val>max_val){
       float temp=min_val;
       min_val=max_val;
       max_val=temp;
   }

   *minF=min_val;
   *maxF=max_val;


}


Line::Line(){
   magnitude=0.0f;
   direccion=Vector2(0,0);
   normal=Vector2(0,0);
}
Line::Line(Vector2 p1,Vector2 p2){
    pt1=p1;
    pt2=p2;
    Vector2 dr=p2-p1;
    magnitude=dr.magnitud();
    direccion=dr.normalize();
    normal=Vector2(-direccion.y,direccion.x);

}
void Line::project_vertices(Vector2 axis,float* min_p,float* max_p){
  float max_val=-10000.0f;
   float min_val=10000.0f;

    float proj1=pt1.dot(axis);
    float proj2=pt2.dot(axis);
    min_val=proj1;
    max_val=proj1;

    if(proj2<min_val){
        min_val=proj2;
    }
    if(proj2>max_val){

        max_val=proj2;

   }

   *min_p=min_val;
   *max_p=max_val;
}

Vector2 Line:: get_direccion(){
      return direccion;
 }
Vector2 Line::get_normal(){
    return normal;
}
float Line::get_magnitud(){
    return magnitude;
}

 vector<Vector2>  Line::get_points(){
    vector<Vector2> res;
    res.push_back(pt1);
    res.push_back(pt2);
    return res;
 }

 Ray::Ray(){
    origin=Vector2(0,0);
    distance=0;
    direccion=Vector2(0,0);

 }
 Ray::Ray(Vector2 orign,Vector2 dir,float dist){
     origin=orign;
     direccion=dir;
     distance=dist;
 }
float Ray::get_distance(){
   return distance;
}
Vector2 Ray::get_dir(){
   return direccion;
}
Vector2 Ray::get_origen(){
    return origin;
}
vector<Vector2> Ray::get_box_data(){
    vector<Vector2> res;

   Vector2 c=origin+(Vector2((direccion.x*distance)*0.5f,(direccion.y*distance)*0.5f));
   Vector2 end_r=origin+(Vector2(direccion.x*distance,direccion.y*distance));


   Vector2 sz=Vector2(abs(origin.x-end_r.x),abs(origin.y-end_r.y));

   res.push_back(sz);
   res.push_back(c);
   return res;

}
