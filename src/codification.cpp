#include "codification.h"

 bool Codification::contain_chr(std::string cad, unsigned char c){

       bool res=false;

       for(int i=0;i<cad.length();i++){
           unsigned char temp=cad[i];
           if(temp==c){

               res=true;
           }
       }
       return res;
 }
