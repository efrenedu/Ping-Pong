#ifndef FIGURE_H
#define FIGURE_H
#include<math structs.h>
#include<vector>
#include<iostream>


class Figure{
public:
      Figure();
      Figure(Vector2 dim,Vector2 offset);
      Figure(vector<Vector2> pts,Vector2 offset);
      Figure(float radius,Vector2 offset);

      vector<Vector2> get_vertices();
      Vector2 get_center();
      Vector2 get_dim();
      Vector2 get_halfDim();
      float get_radius();
      int get_type();
      vector<Vector2> get_normals();
      int get_near_vertex_Index(Vector2 pt,Vector2 offset_figure);
      void project_vertices(Vector2 axis,float* minF,float *maxF,Vector2 offset);
      void project_Circle(Vector2 axis,float* minF,float *maxF,Vector2 offset);


private:
    vector<Vector2> points;
    vector<Vector2> normals;
    float radio;
    Vector2 dimension;
    Vector2 centro;
    Vector2 halfDim;
    int id;
    Vector2 offset_fig;

};

struct Line{

public:
    Line();
    Line(Vector2 p1,Vector2 p2);
    Vector2 get_direccion();
    Vector2 get_normal();
    float get_magnitud();
    void project_vertices(Vector2 axis,float* min_p,float* max_p);

    vector<Vector2>  get_points();

private:
    Vector2 pt1;
    Vector2 pt2;
    Vector2 direccion;
    Vector2 normal;
    float magnitude;

};

struct Ray{
public:
    Ray();
    Ray(Vector2 orign,Vector2 dir,float dist);
    float get_distance();

    Vector2 get_dir();
    Vector2 get_origen();
    vector<Vector2> get_box_data();
private:
    Vector2 origin;
    Vector2 direccion;
    float distance;
};
#endif // FIGURE_H
