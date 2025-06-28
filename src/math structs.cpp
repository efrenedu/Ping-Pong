#include "math structs.h"
#include<iostream>

Vector2::Vector2(){
   x=0;
   y=0;
}
Vector2::Vector2(float xval,float yval){
   x=xval;
   y=yval;
}
string Vector2::to_str(){
   string msg="";
   msg=msg+"( "+to_string(x)+" , "+to_string(y)+" )";
   return msg;
}
Vector2 Vector2 ::from_string(string s){
    bool valid=false;
    Vector2 vec =Vector2(0,0);
    string temp="";
    int num_tokens=0;
    float first=0.0f;
    float second=0.0f;

    for (int i=0;i<s.length();i++){
        char c=s[i];
        if(c=='-'){

            first=atof(temp.c_str());
            num_tokens=1;
            temp="";

        }
        else{
            temp=temp+c;
        }
    }

    if(num_tokens==1 && temp!=""){
        second=atof(temp.c_str());
        vec.x=first;
        vec.y=second;
    }

    return vec;
}
Vector2 Vector2::normalize(){
   Vector2 res=Vector2(x,y);
   float mag=magnitud();
   if(mag>0){
      res.x=x/mag;
      res.y=y/mag;
   }
   return res;

}
float Vector2::magnitud(){
   float res=0.0f;
   res=sqrt(pow(x,2)+pow(y,2));
   return res;
}

Vector2 Vector2::operator+(Vector2 const& vec){
    Vector2 res=Vector2(x,y);
    res.x=res.x+vec.x;
    res.y=res.y+vec.y;
    return res;
}

Vector2 Vector2::operator-(Vector2 const& vec){
    Vector2 res=Vector2(x,y);
    res.x=res.x-vec.x;
    res.y=res.y-vec.y;
    return res;
}
 bool Vector2:: operator >=(Vector2 const& other){
       if(other.x>=x && other.y>=y){
           return true;
       }
       return false;
 }
bool Vector2:: operator >(Vector2 const& other){
    if(other.x>x && other.y>y){
           return true;
       }
       return false;
}
bool Vector2:: operator <=(Vector2 const& other){
    if(other.x<=x && other.y<=y){
           return true;
       }
       return false;
}
bool Vector2:: operator < (Vector2 const& other){
 if(other.x<x && other.y<y){
           return true;
    }
       return false;
}

float Vector2::dot(Vector2 vec){
    float res=(x*vec.x)+(y*vec.y);
    return res;
 }
bool Vector2::operator == (Vector2 const &other){
   if(other.x==x && other.y==y){
    return true;
   }
   return false;
}
bool Vector2::operator != (Vector2 const &other){
   if(other.x!=x || other.y!=y){
       return true;
   }
   return false;
}

 Transform::Transform(){
     position=Vector2(0,0);
     rotation=0;
     scale=Vector2(0,0);
 }
 Transform::Transform(Vector2 p,float r, Vector2 s){
     position=p;
     rotation=r;
     scale=s;
 }
 string Transform::to_str(){
      string msg;
      msg="position: "+position.to_str()+" rotation: "+to_string(rotation)+" escale: "+scale.to_str();
      return msg;
 }

Vector2 Transform:: Get_Position(){

    return position;
}
float Transform:: get_rotation(){
    return rotation;
}
Vector2 Transform:: Get_Scale(){
    return scale;
}
void Transform:: Set_Position(Vector2 p){
    position.x=p.x;
    position.y=p.y;
}
void Transform:: Translate(Vector2 p){
    position.x+=p.x;
    position.y+=p.y;
}
void Transform:: Rotate(float angle){
    rotation+=angle;
    if(rotation==360){
        rotation=0;
    }
    else if(rotation<0)
    {
        rotation=360-abs(rotation);
    }
    else if(rotation>360){
        rotation=rotation-360;

    }




}
void Transform :: set_scale(Vector2 scal){

    scale.x=scal.x;
    scale.y=scal.y;
}

int Math_G::StrToInt(string val){
  int res=0;
  bool negative=false;
  for(int i=0;i<val.length();i++){
       unsigned char c=val[i];
       if(c!=' '){
           if(c=='-'){
               negative=true;
           }
           else{
               int code=(int)c;
               if(code>=48 && code<=57){
                 code=code-48;
                 int multiplic=val.length()-(i+1);
                 int valor_n=pow(10,multiplic)*code;
                 res=res+valor_n;
              }
           }

       }
  }

  if(negative){
    res=res*-1;
  }
  return res;

}
float Math_G::Clamp(float val,float minimo,float maximo){
      if(val>maximo){
        val=maximo;
      }
      if(val<minimo){
        val=minimo;
      }
      return val;
}
float Math_G::StrToFloat(string val){

   float res=0.0f;
   string enteros="";
   string decimales="";
   bool negative=false;
   bool get_decimal=false;
   int num_decimales=0;
   for(int i=0;i<val.length();i++){
       unsigned char c=val[i];
       if(c!=' '){
           if(c=='-'){
             negative=true;
           }
           else if(c=='.' || c==','){
               get_decimal=true;
           }
           else{
              int code=(int)c;
              if(code>=48 && code<=57){
                if(get_decimal==false){
                      enteros+=c;
                }
                else{
                      decimales+=c;
                      num_decimales=num_decimales+1;
                 }
              }

           }
       }
   }

   int val_entero=StrToInt(enteros);
   res=0.0f+((float)val_entero);
   int val_decimal=StrToInt(decimales);

   int multiplic=pow(10,num_decimales);
   if(multiplic!=0){

       float f_val=(float)val_decimal/(float)multiplic;

       res=res+f_val;
   }

  if(negative){
      res=res*-1;
  }
   return res;
}
bool Math_G::is_number(string val){
   bool res=true;
   if(val==""){
       res=false;
   }
   else{
       for(int i=0;i<val.length();i++){
           unsigned char c=val[i];
           if(c==' '){
              res=false;
              i=val.length();
           }
           else{

                if(c=='.' || c==','){
                    if(i==0 || i==val.length()-1){
                        res=false;
                        i=val.length();
                    }
                }
                else if(c=='-'){
                    if(i!=0){
                        res=false;
                        i=val.length();
                    }
                }
                else{
                    int code=(int)c;
                    if(code<48 || code>57){
                        res=false;
                        i=val.length();
                    }
                }
           }
       }
   }
   return res;
}

int Math_G::get_random(int mod){
   if(seed==-1){
     int rand_n=rand() % 100 + 1;
     seed=rand_n;
   }

   int val=(param1_Random*seed)+param2_Random;


   while(val>mod){
      val=val%mod;
   }

   seed=val;
   return val;
}
void Math_G::set_seedRand(int sd){
   seed=sd;
}

int Math_G::param1_Random=7;
int Math_G::seed=-1;
int Math_G::param2_Random=3;
