#include "Basic_Game_Data.h"

Node::Node(){
   name="";

   id=-1;
}

Node::Node(map<string, string>  params,string nombre,int id_e ){
   parametros=params;
   name=nombre;
   id=id_e;


}
void Node::add_parameter(string param,string value){
   parametros[param]=value;


}
string Node::get_parameter_data(string parameter){
   string res="";
   map<string,string>::iterator it;
   for(it=parametros.begin();it!=parametros.end();it++){
      string p=it->first;
      if(p==parameter){
          res=it->second;
          break;
      }
   }
   return res;
}
map<string,string> Node:: get_parametros(){
  return parametros;

}
string Node:: get_tag_name(){
    return name;

}
int Node::get_id(){
   return id;
}
string Node::to_str(){

   string res="";
   res+="etiqueta name:"+name+"\n";
   res+="id:"+to_string(id)+"\n";
   map<string,string>::iterator it;
   res+="parametros:\n";

   for(it=parametros.begin();it!=parametros.end();it++){
     res+="-"+it->first+"="+it->second+"\n";
  }

  return res;

}
map<string ,vector<Node>> XML_parser::etiquetas;
int XML_parser::id_e=0;

vector<Node> XML_parser::get_etiquetas_by_Name(string n){
  vector<Node> res;
  map<string,vector<Node>> ::iterator it;
  for(it=etiquetas.begin();it!=etiquetas.end();it++){
    string tag=it->first;
    if(tag==n){
        res=it->second;
        break;
    }
  }
  return res;
}

map<string ,vector<Node>> XML_parser::get_XML_data(){
  return etiquetas;
}

void XML_parser::read_xml(string ruta){

  ifstream stream;
  stream.open(ruta,ios::in);


  if(stream.good()){
     string linea="";
     getline(stream,linea);              //omite encabezado

     map<string, vector<Node>> lista_etiquetas;

     string last_etiqueta="";
     int last_id=-1;
     string layer_map="";

     while(getline(stream,linea)){

       int start_index=-1;

       for(unsigned int s=0;s<linea.length();s++){

           if(linea[s]=='<'){
              start_index=s;
              s=linea.length();
          }
       }

       if(start_index!=-1){
            //es una etiqueta
            string temp_data="";
            string temp_parameter="";
            string etiqueta_name="";
            map<string,string> parametros;
            bool add_node=true;



            for(unsigned int i=start_index+1;i<linea.length();i++){
                if(linea[i]==' '){
                    if(etiqueta_name==""){
                        //necesita agregar etiqueta
                        etiqueta_name=temp_data;
                        temp_data="";

                    }
                    else{
                        if(temp_parameter!=""){
                        //parametro que separa valores con espacio vacios
                           temp_data+=";";
                        }
                    }

                }
                else if(linea[i]=='"' ){
                    //parametro de etiqueta
                    if(temp_parameter==""){

                           temp_data.erase(temp_data.begin()+(temp_data.length()-1));
                           temp_parameter=temp_data;
                           temp_data="";

                    }
                    else{
                        parametros[temp_parameter]=temp_data;
                        temp_data="";
                        temp_parameter="";
                    }



                }

                else if(linea[i]=='>' ){
                        //final de linea
                        if(temp_data.length()>2){
                            if(temp_data=="/data"){

                                vector<Node> posibles=lista_etiquetas[last_etiqueta];
                                vector<Node> ::iterator it_pol;
                                for(it_pol=posibles.begin();it_pol!=posibles.end();it_pol++){
                                     Node n=*it_pol;
                                     int id_temp=n.get_id();


                                     if(id_temp==last_id){

                                        size_t index=std::distance(std::begin(lista_etiquetas[last_etiqueta]),it_pol) ;
                                        string next_parm="mapa";
                                        string next_val=layer_map;

                                        n.add_parameter(next_parm,next_val);


                                        lista_etiquetas[last_etiqueta].pop_back();
                                        lista_etiquetas[last_etiqueta].push_back(n);




                                     }
                                }
                                last_etiqueta="";
                                last_id=-1;
                                layer_map="";
                                add_node=false;

                            }
                            else if(temp_data=="/object"){
                                last_etiqueta="";
                                last_id=-1;
                                add_node=false;
                            }
                            else if(temp_data=="/properties"){
                                add_node=false;
                            }
                        }

                        temp_data="";
                }

                else{
                      //agregar informacion
                      temp_data=temp_data+=linea[i];

                }


            }

            //crear nodo
            if(etiqueta_name!=""){

                 if(etiqueta_name=="object" || etiqueta_name=="layer"){

                      last_etiqueta=etiqueta_name;
                      last_id=id_e;
                    }
                    else if(etiqueta_name=="polygon"){
                                vector<Node> posibles=lista_etiquetas[last_etiqueta];
                                vector<Node> ::iterator it_pol;
                                for(it_pol=posibles.begin();it_pol!=posibles.end();it_pol++){
                                     Node n=*it_pol;
                                     int id_temp=n.get_id();


                                     if(id_temp==last_id){

                                        size_t index=std::distance(std::begin(lista_etiquetas[last_etiqueta]),it_pol) ;
                                        string next_parm="points";
                                        string next_val="";
                                        try{
                                             next_val=parametros.at(next_parm);
                                             next_val+=";";
                                        }
                                        catch(...){
                                              printf("points not found");
                                        }


                                        n.add_parameter(next_parm,next_val);


                                        lista_etiquetas[last_etiqueta].pop_back();
                                        lista_etiquetas[last_etiqueta].push_back(n);




                                     }
                                }

                                add_node=false;
                             }
                else if(etiqueta_name=="property"){
                    if(last_etiqueta!="" && last_id!=-1){
                                vector<Node> posibles=lista_etiquetas[last_etiqueta];
                                vector<Node> ::iterator it_pol;
                                for(it_pol=posibles.begin();it_pol!=posibles.end();it_pol++){
                                     Node n=*it_pol;
                                     int id_temp=n.get_id();


                                     if(id_temp==last_id){

                                        size_t index=std::distance(std::begin(lista_etiquetas[last_etiqueta]),it_pol) ;

                                        string next_parm1="name";
                                        string next_val1="";
                                        string next_parm2="value";
                                        string next_val2="";
                                        try{
                                             next_val1=parametros.at(next_parm1);
                                             next_val2=parametros.at(next_parm2);

                                        }
                                        catch(...){
                                              printf("property not found");
                                        }


                                        n.add_parameter(next_val1,next_val2);


                                        lista_etiquetas[last_etiqueta].pop_back();
                                        lista_etiquetas[last_etiqueta].push_back(n);




                                     }
                                }
                    }
                    add_node=false;
                }
                else if(etiqueta_name=="properties"){
                    add_node=false;
                }
                 else if(etiqueta_name=="data"){
                    add_node=false;

                 }

                 if(add_node==true){

                     Node nueva=Node(parametros,etiqueta_name,id_e);
                     lista_etiquetas[etiqueta_name].push_back(nueva);
                     id_e+=1;

                 }
            }

       }
       else{

          //array data  layer


          for(unsigned int i=0;i<linea.length();i++){




                int c=linea[i]-48;
                if(c<0 && i==linea.length()-1){
                    layer_map=layer_map+";";
                }
                else{

                     if(c<0){
                       layer_map+=",";
                     }
                     else{
                         layer_map+=to_string(c);
                     }

                }
          }



       }

     }


    stream.close();
    etiquetas=lista_etiquetas;
   }
   else{

       printf("error  xml of map not found");

   }
}

