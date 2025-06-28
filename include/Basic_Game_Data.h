#ifndef BASIC_GAME_DATA_H
#define BASIC_GAME_DATA_H


#include <map>
#include <vector>
#include<string>
#include<fstream>
#include<iostream>
using namespace std;

struct Node{
public:
    Node();
    Node(map<string, string> params,string nombre,int id_e );
    string to_str();
    int get_id();
    map<string,string> get_parametros();
    string get_parameter_data(string parameter);
    string get_tag_name();
    void add_parameter(string param,string value);
private:
    map<string, string> parametros;
    string name;
    int id;

};

class XML_parser{

  public:
     static void read_xml(string ruta);
     static vector<Node> get_etiquetas_by_Name(string n);
     static map<string ,vector<Node>> get_XML_data();
  private:
     static map<string ,vector<Node>> etiquetas;
     static int id_e;

};


#endif // BASIC_GAME_DATA_H
