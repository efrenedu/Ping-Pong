#ifndef MATH STRUCTS_H
#define MATH STRUCTS_H

#include<string>
#include<cmath>
using namespace std;

struct Vector2{
public:
    float x;
    float y;
    Vector2();
    Vector2(float xval, float yval);
    static Vector2 from_string(string s);
    Vector2 normalize();
    float magnitud();
    string to_str();
    Vector2 operator+(Vector2 const& vec);
    Vector2 operator-(Vector2 const& vec);
    bool operator == (Vector2 const &other);
    bool operator != (Vector2 const &other);
    bool operator >=(Vector2 const& other);
    bool operator >(Vector2 const& other);
    bool operator <=(Vector2 const& other);
    bool operator < (Vector2 const& other);

    float dot(Vector2 vec);


};

class Math_G{
public:
   static bool is_number(string val);
   static float StrToFloat(string val);
   static int StrToInt(string val);
   static float Clamp(float val,float minimo,float maximo);
   static int get_random(int mod);
   static void set_seedRand(int sd);
private:
    static int seed;
    static int param1_Random;
    static int param2_Random;

};

struct Transform{
public:

       Transform();
       Transform(Vector2 p,float r, Vector2 s);
       Vector2 Get_Position();
       float get_rotation();
       Vector2 Get_Scale();
       void Set_Position(Vector2 p);
       void Translate(Vector2 p);
       void Rotate(float angle);
       void set_scale(Vector2 scal);

       string to_str();
private:
        Vector2 position;
       int rotation;
       Vector2 scale;
};




#endif // MATH STRUCTS_H
