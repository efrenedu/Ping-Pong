#include "Physic_Engine.h"
#include "Component.h"

int Physic_Engine::num_steps=3;
float Physic_Engine::multiplic_step=0.0f;

void Physic_Engine::update_physic( vector<Game_Object>& objs)
{
    if(Scene_manager::is_loading()==true){
        return;
    }

    tuple<vector<int>,vector<int>> in_cam=Camera::get_inCam();
    int sz=tuple_size<decltype(in_cam)>::value;
    if(sz<2){
        return;
    }
    vector<int> objs_indexs=std::get<1>(in_cam);

    if(multiplic_step==0.0f){
        multiplic_step=1.0f/(num_steps+0.0f);

    }


    vector<int>::iterator it_rb;



    map<string,vector<Vector2>> fixed_bodies;

    vector<int> bodies_block;
    vector<int> bodies_noBlock;


   int index_temp=-1;

    for(it_rb=objs_indexs.begin();it_rb!=objs_indexs.end();it_rb++){


        int indice=*it_rb;

        if(indice>=0 && indice<objs.size()){
           Game_Object g=objs[indice];



           RigidBody* rb=Component::get_rigidBody(g.get_name());
           Collider* col=Component::get_Collider(g.get_name());
           if(g.is_active() && rb!=NULL && col!=NULL && g.get_pause()==false){
             bool in_colision=false;
             float torque_total=0.0f;


             g.reset_collisions_data();
             fixed_bodies[g.get_name()].push_back(Vector2(0,0));


             Vector2 next_force=rb->get_last_force();
             Vector2 fix_p=Vector2(0,0);




             if(use_gravity && rb->apply_gravity()==true &&col->is_trigger()==false ){


               if(rb->get_coyote_state()==false){
                   Vector2 last_normFloor=rb->get_lastNorm_Floor();
                   Vector2 gravity_dir=Vector2(0,1);
                   if(last_normFloor.x!=0 && last_normFloor.y!=0){
                       gravity_dir=last_normFloor;
                       gravity_dir.x=gravity_dir.x*-1;

                    }

                    float mass=rb->get_mass();
                    float G_multipl=mass*gravedad*Timer::get_delta_time();

                   Vector2 g_force=Vector2(gravity_dir.x*G_multipl,gravity_dir.y*G_multipl);
                   next_force=next_force+g_force;

               }


            }
            if(rb->get_lock_Position().y!=0){
                next_force.y=0;
            }
            if(rb->get_lock_Position().x!=0){
                next_force.x=0;
            }


          vector<int>::iterator it_cols;

         int num_colis=0;



        for(it_cols=objs_indexs.begin();it_cols!=objs_indexs.end();it_cols++){
           int indice2=*it_cols;
           if(indice2>=0 && indice2<objs.size()){
             Game_Object g2=objs.at(indice2);
             ManiFold* fold=new ManiFold();
             Collider* col2=Component::get_Collider(g2.get_name());
             RigidBody* rb2=Component::get_rigidBody(g2.get_name());

             if(col2!=NULL && g.is_active() && g2.is_active()&& g2.get_name()!=g.get_name() && rb2==NULL){

               for(int i=0;i<num_steps+1;i++){
                  Vector2 force_step=Vector2(0,0);
                  float porcent=multiplic_step*(i);
                  force_step.x=next_force.x*porcent;
                  force_step.y=next_force.y*porcent;


                 if(in_Collision(g,g2,force_step,Vector2(0,0),*col,*col2,fold)){


                      num_colis+=1;



                      Vector2 fix=Vector2(0,0);

                      if(col2->is_trigger()==false && col->is_trigger()==false){
                           Vector2 move_step=force_step;

                           Vector2 n_step=fold->get_normal();
                           n_step.x=n_step.x*-1;
                           n_step.y=n_step.y*-1;



                           next_force=Aplicar_fuerzas_RigidCollider(*rb,*col,*col2,next_force,Vector2(0,0),&fix_p,fold);



                           force_step.x=abs(force_step.x)*n_step.x;
                           force_step.y=abs(force_step.y)*n_step.y;

                           fix_p.x=fix_p.x+force_step.x;
                           fix_p.y=fix_p.y+force_step.y;

                           fixed_bodies[g.get_name()].push_back(fold->get_normal());

                           in_colision=true;




                           g.OnCollision(g2,fold);
                      }
                      else{

                            g.OnTrigger(g2,fold);

                       }


                        i=num_steps;
                     }
                 }  //fin for steps
              }

               if(fold!=NULL){
                delete(fold);
              }

            }
          }

          if(in_colision==false ){
              rb->set_NormFloor(Vector2(0,0));
              bodies_noBlock.push_back(indice);
          }
          else{
              bodies_block.push_back(indice);
          }

          g.update_physic(next_force,fix_p);

          objs[indice]=g;
        }

      }
    }



  int num_locks=bodies_block.size();
  vector<int> bodies;
   int search_t=0;
  if(bodies_block.size()>0){
      search_t=1;
      bodies.insert(bodies.end(),std::begin(bodies_block),std::end(bodies_block));

  }
  if(bodies_noBlock.size()>0){
      bodies.insert(bodies.end(),std::begin(bodies_noBlock),std::end(bodies_noBlock));

  }


  bodies_block.clear();
  bodies_noBlock.clear();


while(search_t!=-1){


  vector<int>::iterator it_rb2;
//colisiones entre rigid bodies



  int index_it=0;
  for(it_rb2=bodies.begin();it_rb2!=bodies.end();it_rb2++){
     int indice1=*it_rb2;
     if(indice1>=0 && indice1<objs.size()){
       if((index_it<num_locks && search_t==1) || search_t==0){
         Game_Object A=objs[indice1];
         RigidBody* rbA=Component::get_rigidBody(A.get_name());
         Collider* col=Component::get_Collider(A.get_name());

         vector<int>::iterator it_rb3;
         for(it_rb3=bodies.begin();it_rb3!=bodies.end();it_rb3++){
             int indice2=*it_rb3;
             if(indice2>=0 && indice2<objs.size()){
             Game_Object B=objs[indice2];
             RigidBody *rbB=Component::get_rigidBody(B.get_name());
             Collider* col2=Component::get_Collider(B.get_name());

             ManiFold* fold2=new ManiFold();
             if(A.get_name()!=B.get_name() &&(A.is_active()==true && B.is_active()==true && rbA!=NULL && rbB!=NULL && col!=NULL && col2!=NULL)==true ){

                Vector2 forceA=rbA->get_last_force();
                Vector2 forceB=rbB->get_last_force();

                for(int i=0;i<num_steps+1;i++){
                    Vector2 force_stepA=Vector2(0,0);
                    Vector2 force_stepB=Vector2(0,0);
                     float porcent=multiplic_step*(i);
                    force_stepA.x=forceA.x*porcent;
                    force_stepA.y=forceA.y*porcent;
                    force_stepB.x=forceB.x*porcent;
                    force_stepB.y=forceB.y*porcent;

                    if(in_Collision(A,B,force_stepA,force_stepB,*col,*col2,fold2)){



                       vector<Vector2> direcciones_B;
                       vector<Vector2> direcciones_A;

                        if(fixed_bodies.find(B.get_name())!=fixed_bodies.end()){
                            direcciones_B=fixed_bodies[B.get_name()];
                        }
                        if(fixed_bodies.find(A.get_name())!=fixed_bodies.end()){
                            direcciones_A=fixed_bodies[A.get_name()];
                        }


                        vector<Vector2> ::iterator it_b;
                        vector<Vector2> ::iterator it_a;
                        vector<Vector2> recibe_forces;
                        Vector2 recibeA=Vector2(1,1);
                        Vector2 recibeB=Vector2(1,1);

                        bool A_bloq=false;
                        bool B_bloq=false;

                        for(it_b=direcciones_B.begin();it_b!=direcciones_B.end();it_b++){
                            Vector2 dr=fold2->get_normal();

                            Vector2 dr2=*it_b;

                            if(dr2.dot(dr)>0){
                                  B_bloq=true;
                            }
                            if(dr.x*dr2.x>0){
                                recibeB.x=0;
                            }
                            if(dr.y*dr2.y>0){
                                recibeB.y=0;
                            }


                        }
                        for(it_a=direcciones_A.begin();it_a!=direcciones_A.end();it_a++){
                            Vector2 dr=fold2->get_normal();

                            Vector2 dr2=*it_a;
                            if(dr2.dot(dr)<0){
                                A_bloq=true;
                            }
                            if(dr.x*dr2.x<0){
                                recibeA.x=0;
                            }
                            if(dr.y*dr2.y<0){
                                recibeA.y=0;
                            }


                        }

                        Vector2 lock_pos_A=rbA->get_lock_Position();
                        Vector2 lock_pos_B=rbB->get_lock_Position();
                        if(lock_pos_A.x!=0){
                               recibeA.x=0;
                        }
                        if(lock_pos_A.y!=0){
                            recibeA.y=0;
                        }
                        if(lock_pos_B.x!=0){
                               recibeB.x=0;
                        }
                        if(lock_pos_B.y!=0){
                            recibeB.y=0;
                        }
                        recibe_forces.push_back(recibeA);
                        recibe_forces.push_back(recibeB);

                        Vector2 a_fix=Vector2(0,0);
                        Vector2 b_fix=Vector2(0,0);



                        if(col->is_trigger()==false && col2->is_trigger()==false){




                           Vector2 forceB_before=forceB;
                           Vector2 forceA_before=forceA;




                           Aplicar_fuerzas_RigidvsRigid(*rbA,*rbB,*col,*col2,&forceA,&forceB,&a_fix,&b_fix,recibe_forces,fold2);

                            force_stepA.x=force_stepA.x*recibeA.x;
                            force_stepA.y=force_stepA.y*recibeA.y;
                            force_stepB.x=force_stepB.x*recibeB.x;
                            force_stepB.y=force_stepB.y*recibeB.y;




                           a_fix.x=a_fix.x+force_stepA.x;
                           a_fix.y=a_fix.y+force_stepA.y;
                           b_fix.x=b_fix.x+force_stepB.x;
                           b_fix.y=b_fix.y+force_stepB.y;



                           if(recibe_forces.at(0).dot(fold2->get_normal())!=0){

                               fixed_bodies[A.get_name()].push_back(fold2->get_normal());

                           }
                           if(recibe_forces.at(1).dot(fold2->get_normal())!=0){
                               fixed_bodies[B.get_name()].push_back(Vector2(fold2->get_normal().x*-1,fold2->get_normal().y*-1));

                           }




                           A.OnCollision(B,fold2);
                           B.OnCollision(A,fold2);





                           A.update_physic(forceA,a_fix);
                           B.update_physic(forceB,b_fix);
                        }
                        else{

                            A.OnTrigger(B,fold2);
                            B.OnTrigger(A,fold2);



                        }
                        objs[indice1]=A;
                        objs[indice2]=B;
                        i=num_steps;
                }
             }
           }
             if(fold2!=NULL){
                delete(fold2);
            }

         }
       }

       }

       else{
            Game_Object A=objs[indice1];
            vector<Vector2> dirs=fixed_bodies[A.get_name()];
            if(dirs.size()<=0){
                 bodies_noBlock.push_back(indice1);
            }
            else{
                vector<Vector2>::iterator it_dirs;
                bool found_bloq=false;
                for(it_dirs=dirs.begin();it_dirs!=dirs.end();it_dirs++){
                      Vector2 v=*it_dirs;
                      if(v.x!=0 || v.y!=0){
                           found_bloq=true;
                           break;
                      }
                }
                if(found_bloq){
                    bodies_block.push_back(indice1);
                }
                else{
                    bodies_noBlock.push_back(indice1);
                }
            }


       }

     }
     index_it=index_it+1;
  }

  if(bodies_block.size()>0){
       num_locks=bodies_block.size();
       search_t=1;
  }
  else{
    search_t=0;
  }
  bodies.clear();
  if(bodies_block.size()>0){
    bodies.insert(bodies.end(),std::begin(bodies_block),std::end(bodies_block));
    bodies_block.clear();
  }
  if(bodies_noBlock.size()>0){
    bodies.insert(bodies.end(),std::begin(bodies_noBlock),std::end(bodies_noBlock));
    bodies_noBlock.clear();
  }
  if(bodies.size()<=0){
      search_t=-1;
  }


}


}
void Physic_Engine::Aplicar_fuerzas_RigidvsRigid(RigidBody rA,RigidBody rB,Collider colA,Collider colB, Vector2* forceA,Vector2* forceB,Vector2* fix_A,Vector2* fix_B,vector<Vector2> recibe_forces,ManiFold* fold){
//calculos entre dos cuerpos dinamicos


    Vector2 next_forceA=*forceA;
    Vector2 next_forceB=*forceB;
    //sacamos masas inversas
    float massA=0.0f;
    float massB=0.0f;
    float massInv_A=0.0f;
    float massInv_B=0.0f;
    float massInv_T=0.0f;

    if(rA.get_mass()!=0.0f){
       massInv_A=1.0f/rA.get_mass();
       massA=rA.get_mass();
    }
    if(rB.get_mass()!=0.0f){
       massInv_B=1.0f/rB.get_mass();
       massB=rB.get_mass();
    }

   float massT=massA+massB;
   massInv_T=massInv_A+massInv_B;


   float boundingA=colA.get_material().get_rebote();
   float boundingB=colB.get_material().get_rebote();
   float boundingG=(boundingA>boundingB)?boundingA:boundingB;

   float frictA=colA.get_material().get_friction();
   float frictB=colB.get_material().get_friction();
   float dinFrictA=colA.get_material().get_dinamic_friction();
   float dinFrictB=colB.get_material().get_dinamic_friction();
   float dinamicF=(dinFrictA+dinFrictB)/2.0f;
   float staticF=(frictA+frictB)/2.0f;


   Vector2 frict_forceA=Vector2(0,0);
   Vector2 frict_forceB=Vector2(0,0);



  if(fold==NULL){
       return;
   }

   float penetracion=fold->get_penetracion();
   Vector2 norm=fold->get_normal();



   Vector2 react=Vector2(norm.x,norm.y);
   Vector2 forceT_vec=Vector2(0,0);
   forceT_vec.x=abs(forceA->x)+abs(forceB->x);
   forceT_vec.y=abs(forceA->y)+abs(forceB->y);

   react.x=react.x*forceT_vec.x;
   react.y=react.y*forceT_vec.y;

   react.x=react.x+(react.x*boundingG);
   react.y=react.y+(react.y* boundingG);



   Vector2 reactA=Vector2(0,0);
   Vector2 reactB=Vector2(0,0);
   Vector2 recibeA=Vector2(0,0);
   Vector2 recibeB=Vector2(0,0);
   if(recibe_forces.size()>=2){
      recibeA=recibe_forces.at(0);
     recibeB=recibe_forces.at(1);
   }




      Vector2 n=Vector2(abs(norm.x),abs(norm.y));
      frict_forceA=Vector2(rA.get_last_force().x*-1,rA.get_last_force().y*-1);
      frict_forceB=Vector2(rB.get_last_force().x*-1,rB.get_last_force().y*-1);

      frict_forceA.x=frict_forceA.x*n.y*recibeA.x;
      frict_forceA.y=frict_forceA.y*n.x*recibeA.y;
      frict_forceB.x=frict_forceB.x*n.y*recibeB.x;
      frict_forceB.y=frict_forceB.y*n.x*recibeB.y;


      float magA=(Vector2(rA.get_last_force().x*n.y,rA.get_last_force().y*n.x)).magnitud();
      float magB=(Vector2(rB.get_last_force().x*n.y,rB.get_last_force().y*n.x)).magnitud();

      if(magA<=0.0001f){
          frict_forceA.x=frict_forceA.x*staticF;
          frict_forceA.y=frict_forceA.y*staticF;
      }
      else{

          frict_forceA.x=frict_forceA.x*dinamicF;
          frict_forceA.y=frict_forceA.y*dinamicF;
      }
      if(magB<=0.0001f){
          frict_forceB.x=frict_forceB.x*staticF;
          frict_forceB.y=frict_forceB.y*staticF;
      }
      else{

           frict_forceB.x=frict_forceB.x*dinamicF;
           frict_forceB.y=frict_forceB.y*dinamicF;
      }



      fix_A->x=norm.x*penetracion*recibeA.x;
      fix_A->y=norm.y*penetracion*recibeA.y;

      fix_B->x=(norm.x*-1)*penetracion*recibeB.x;
      fix_B->y=(norm.y*-1)*penetracion*recibeB.y;

      if(fix_A->x!=0 ){
        fix_B->x=0;
      }
      if(fix_A->y!=0 ){
        fix_B->y=0;
      }


      Vector2 temp_reactA=Vector2(0,0);
      Vector2 temp_reactB=Vector2(0,0);
      float porcentA=0.0f;
      float porcentB=0.0f;
      if(massInv_T>0.0f){
          porcentA=(massInv_A/massInv_T);
          porcentB=(massInv_B/massInv_T);
      }
      temp_reactA.x=react.x*porcentA;
      temp_reactA.y=react.y*porcentA;
      temp_reactB.x=react.x*porcentB;
      temp_reactB.y=react.y*porcentB;

      reactA.x=(react.x-(temp_reactB.x*recibeB.x))*recibeA.x;
      reactA.y=(react.y-(temp_reactB.y*recibeB.y))*recibeA.y;
      reactB.x=(react.x-(temp_reactA.x*recibeA.x))*recibeB.x;
      reactB.y=(react.y-(temp_reactA.y*recibeA.y))*recibeB.y;










   forceA->x=forceA->x+reactA.x;
   forceA->y=forceA->y+reactA.y;
   forceB->x= forceB->x-reactB.x;
   forceB->y=forceB->y-reactB.y;


   fix_A->x=fix_A->x+frict_forceA.x;
   fix_A->y=fix_A->y+frict_forceA.y;
   fix_B->x=fix_B->x+frict_forceB.x;
   fix_B->y=fix_B->y+frict_forceB.y;

   forceA->x=forceA->x+frict_forceA.x;
   forceA->y=forceA->y+frict_forceA.y;
   forceB->x=forceB->x+frict_forceB.x;
   forceB->y=forceB->y+frict_forceB.y;





}
Vector2 Physic_Engine::Aplicar_fuerzas_RigidCollider(RigidBody rA,Collider colA,Collider colB,Vector2 forceA,Vector2 forceB,Vector2 *f_fix,ManiFold* fold){


//calculos de un cuerpo dinamico contra un cuerpo estatico
   Vector2 next_force=forceA;

   float penetracion=fold->get_penetracion();
   Vector2 norm=fold->get_normal();
     if(norm.dot(next_force)>0){
        return next_force;
     }



   float boundingA=colA.get_material().get_rebote();
   float boundingB=colB.get_material().get_rebote();
   float boundingG=(boundingA>boundingB)?boundingA:boundingB;



   float n_x=abs(norm.x);
   float n_y=abs(norm.y);
   Vector2 react=Vector2(n_x*-next_force.x,n_y*-next_force.y);

   react.x=react.x+(react.x*boundingG);
   react.y=react.y+(react.y*boundingG);

   float frictA=colA.get_material().get_friction();
   float frictB=colB.get_material().get_friction();
   float dinFrictA=colA.get_material().get_dinamic_friction();
   float dinFrictB=colB.get_material().get_dinamic_friction();
   float dinamicF=(dinFrictA+dinFrictB)/2.0f;
   float staticF=(frictA+frictB)/2.0f;
   Vector2 frict=rA.get_last_force();
   frict.x=frict.x*-1;
   frict.y=frict.y*-1;


   float relForce=rA.get_last_force().magnitud();


    f_fix->x=norm.x*penetracion;
   f_fix->y=norm.y*penetracion;

   bool rampa=false;
   if((n_x>=0.99 || n_y>=0.99f)==false){
        rampa=true;
       frict=next_force;
   }
   if(relForce!=0){



           if(rampa==true && rA.get_last_force().x==0.0){

               frict.x=frict.x*staticF*n_y;
               frict.y=frict.y*staticF*n_x;
           }
           else{
             frict.x=frict.x*dinamicF*n_y;
             frict.y=frict.y*dinamicF*n_x;
           }

   }
   else{


        frict.x=frict.x*staticF*n_y;
        frict.y=frict.y*staticF*n_x;


   }

   next_force=next_force+react+frict;
   return next_force;


}

bool Physic_Engine::in_Collision(Game_Object A,Game_Object B,Vector2 next_forceA, Vector2 next_forceB, Collider colA,Collider colB,ManiFold* fold){



        Transform* transA=Component::get_Transform(A.get_name());
        Transform* transB=Component::get_Transform(B.get_name());
        if(transA==NULL || transB==NULL){
             return false;
        }
        Vector2 posA=transA->Get_Position()+A.get_parentOffset();

        int fig_typeA=colA.get_figure().get_type();
        int angA=0;





        Vector2 posB=transB->Get_Position()+B.get_parentOffset();
        int angB=0;

        int fig_typeB=colB.get_figure().get_type();




        bool res=false;

        if(Collision_square_square(colA,colB,(posA+next_forceA),(posB+next_forceB),0,0,NULL)==true){


                //posible collision


                if(fig_typeA==1 ){
                    //rigidbody es un cuadrado
                    if(fig_typeB==0){
                           if(Collision_Circle_Square(colB,colA,posB+next_forceB,posA+next_forceA,angA,fold)==true){
                            Vector2 n=fold->get_normal();
                            vector<Vector2> old_ctcts=fold->get_contact();
                              if(old_ctcts.size()>0){
                                  Vector2 n_inverse= Vector2(n.x,n.y);
                                  vector<Vector2> ctcts;
                                  Vector2 contact=colA.get_figure().get_center();

                                  Vector2 real_ctct=old_ctcts[0]+(posB+next_forceB);
                                  Vector2 c=contact+(posA+next_forceA);
                                  Vector2 d=real_ctct-c;
                                  contact=(contact+d)+(Vector2(n_inverse.x*fold->get_penetracion(),n_inverse.y*fold->get_penetracion()));
                                  ctcts.push_back(contact);
                                  fold->set_contacts(ctcts);
                              }
                            n.x=n.x*-1;
                            n.y=n.y*-1;
                            fold->set_normal(n);
                            res=true;

                        }

                    }
                    else if(fig_typeB==1){
                          if(Collision_square_square(colA,colB,(posA+next_forceA),(posB+next_forceB),angA,angB,fold)==true){
                            res=true;
                          }

                    }
                    else if(fig_typeB==2){
                          if(Collision_Polygon_Polygon (colA,colB,(posA+next_forceA),(posB+next_forceB),angA,angB,fold)==true){
                             res=true;
                          }

                    }
                }
                else if(fig_typeA==0){

                    //rigidbody es un circulo
                    if(fig_typeB==0){
                          if(Collision_Circle_Circle(colA,colB,(posA+next_forceA),(posB+next_forceB),fold)==true){
                             res=true;
                          }

                    }
                    else if(fig_typeB==1){


                        if(Collision_Circle_Square(colA,colB,posA+next_forceA,posB+next_forceB,angB,fold)==true){
                            res=true;


                        }

                    }
                    else if(fig_typeB==2 ){

                         if(Collision_circle_Polygon(colA,colB,(posA+next_forceA),(posB+next_forceB),angB,fold)==true){
                             res=true;

                          }


                    }
                }
                else if(fig_typeA==2){

                    //rigidbody es un polygono
                    if(fig_typeB==0){
                         if(Collision_circle_Polygon(colB,colA,(posB+next_forceB),(posA+next_forceA),angA,fold)==true){
                              Vector2 n=fold->get_normal();

                              vector<Vector2> old_ctcts=fold->get_contact();
                              if(old_ctcts.size()>0){
                                  Vector2 n_inverse= Vector2(n.x,n.y);
                                  vector<Vector2> ctcts;
                                  Vector2 contact=colA.get_figure().get_center();

                                  Vector2 real_ctct=old_ctcts[0]+(posB+next_forceB);
                                  Vector2 c=contact+(posA+next_forceA);
                                  Vector2 d=real_ctct-c;
                                  contact=(contact+d)+(Vector2(n_inverse.x*fold->get_penetracion(),n_inverse.y*fold->get_penetracion()));
                                  ctcts.push_back(contact);
                                  fold->set_contacts(ctcts);
                              }

                              n.x=n.x*-1;
                              n.y=n.y*-1;
                              fold->set_normal(n);
                             res=true;

                          }
                    }
                    else if(fig_typeB==1 || fig_typeB==2){
                        if(Collision_Polygon_Polygon (colA,colB,(posA+next_forceA),(posB+next_forceB),angA,angB,fold)==true){
                             res=true;
                          }
                    }

                }
            }

   return res;
}

bool Physic_Engine::Collision_Circle_Circle(Collider A,Collider B,Vector2 posA,Vector2 posB,ManiFold* fold){

   bool res=false;
   Figure figA=A.get_figure();
   Figure figB=B.get_figure();
   Vector2 cA=figA.get_center();
   Vector2 cB=figB.get_center();
   float rA=figA.get_radius();
   float rB=figB.get_radius();
   cA=cA+posA;
   cB=cB+posB;


   float distance=(cA-cB).magnitud();

   float penetracion=distance-(rA+rB);
   if(penetracion<=0){
        res=true;
        if(fold!=NULL){
            Vector2 n=(cA-cB).normalize();
            fold->set_normal(n);
            penetracion=abs(penetracion);
            fold->set_penetracion(penetracion);
            Vector2 n_inverse=Vector2(n.x*-1,n.y*-1);

            Vector2 contact=figA.get_center()+Vector2(n_inverse.x*(rA),n_inverse.y*(rA));

            vector<Vector2> ctcts;
            ctcts.push_back(contact);
            fold->set_contacts(ctcts);
       }
   }


   return res;
}
bool Physic_Engine::Collision_square_square(Collider A, Collider B,Vector2 posA,Vector2 posB,int angA,int angB,ManiFold *fold){


   bool res=false;

   Vector2 dimA=A.get_figure().get_dim();
   Vector2 dimB=B.get_figure().get_dim();
   if(dimA.x!=dimA.y){
       if(angA!=180 && angA!=0 ){
           return Collision_Polygon_Polygon(A,B,posA,posB,angA,angB,fold);
       }
   }
   else{
       if(angA!=180 && angA!=0 && angA!=90 && angA!=270){
           return Collision_Polygon_Polygon(A,B,posA,posB,angA,angB,fold);
       }
   }
   if(dimB.x!=dimB.y){
       if(angB!=180 && angB!=0){
           return Collision_Polygon_Polygon(A,B,posA,posB,angA,angB,fold);
       }
   }
   else{
       if(angB!=180 && angB!=0 && angB!=90 && angB!=270){
           return Collision_Polygon_Polygon(A,B,posA,posB,angA,angB,fold);
       }
   }



   Figure figA=A.get_figure();
   Figure figB=B.get_figure();

   Vector2 cA=figA.get_center();
   Vector2 cB=figB.get_center();
   Vector2 halfA=figA.get_halfDim();
   Vector2 halfB=figB.get_halfDim();

   if(fold==NULL){
    dimA.x=dimA.x*1.25f;
    dimA.y=dimA.y*1.25f;
   }

   cA=cA+posA;
   cB=cB+posB;



   if(cA.x+halfA.x>=cB.x-halfB.x && cA.x-halfA.x<=cB.x+halfB.x){


       if(cA.y-halfA.y<=cB.y+halfB.y && cA.y+halfA.y >=cB.y-halfB.y){
           res=true;

           if(fold!=NULL){
               float penetr=1000.0f;
               Vector2 norm=Vector2(0,0);
               Vector2 dist_centers=Vector2(abs(cA.x-cB.x),abs(cA.y-cB.y));
               Vector2 penetracion_v=Vector2(0,0);
               penetracion_v.x=abs(dist_centers.x-(halfB.x+halfA.x));
               penetracion_v.y=abs(dist_centers.y-(halfB.y+halfA.y));
               if(penetracion_v.x<penetracion_v.y){

                    if(cA.x>cB.x){
                        norm=Vector2(1,0);
                    }
                    else{
                         norm=Vector2(-1,0);
                    }
                    penetr=abs(penetracion_v.x);
               }
               else if(penetracion_v.y<penetracion_v.x){
                   if(penetracion_v.x>halfA.x){

                         if(cA.y>cB.y){
                            norm=Vector2(0,1);
                         }
                         else{
                             norm=Vector2(0,-1);
                         }
                   }
                   else{
                      norm=(cB-cA).normalize();
                     norm.x=norm.x*-1;
                   }
                     penetr=abs(penetracion_v.y);
               }
               else{
                     if(cA.y>cB.y){
                        norm=Vector2(0,1);
                    }
                    else{
                         norm=Vector2(0,-1);
                    }
                     penetr=abs(penetracion_v.y);
               }


               fold->set_normal(norm);
               fold->set_penetracion(penetr);


             }
       }

   }





   return res;
}
bool Physic_Engine::Collision_Circle_Square(Collider A, Collider B,Vector2 posA,Vector2 posB,int angB,ManiFold *fold){
   bool res=false;

   Vector2 dimRect=B.get_figure().get_dim();
   if(dimRect.x!=dimRect.y){
        if( angB!=0 &&  angB!=180){
            return Collision_circle_Polygon(A,B,posA,posB,angB,fold);

        }
   }
   else{
       if(angB!=180 && angB!=0 && angB!=90 && angB!=270){
            return Collision_circle_Polygon(A,B,posA,posB,angB,fold);

        }
   }


   Figure figA=A.get_figure();
   Figure figB=B.get_figure();

   Vector2 cA=figA.get_center();
   cA=cA+posA;
   Vector2 cB=figB.get_center();
   cB=cB+posB;

   float radious_A=figA.get_radius();
   Vector2 dimB=figB.get_dim();
   Vector2 halfB=figB.get_halfDim();

   Vector2 p1_c=Vector2(cA.x+radious_A,cA.y-radious_A);
  Vector2 p2_c=Vector2(cA.x-radious_A,cA.y+radious_A);

    Vector2 p1_sq=Vector2(cB.x-halfB.x,cB.y+halfB.y);
  Vector2 p2_sq=Vector2(cB.x+halfB.x,cB.y-halfB.y);


   if(p1_c.x>=p1_sq.x && p2_c.x<=p2_sq.x){


       if(p1_c.y<=p1_sq.y && p2_c.y >=p2_sq.y){
           res=true;





           if(fold!=NULL){

             float penetr=1000.0f;
             Vector2 norm=Vector2(0,0);
             Vector2 dist_centers=Vector2(abs(cA.x-cB.x),abs(cA.y-cB.y));
             Vector2 penetracion_v=Vector2(0,0);
             penetracion_v.x=abs(dist_centers.x-(halfB.x+radious_A));
             penetracion_v.y=abs(dist_centers.y-(halfB.y+radious_A));

             if(penetracion_v.x<penetracion_v.y){

                       if(cA.x<cB.x){
                         norm.x=-1;
                       }
                       else{
                        norm.x=1;
                       }


                    penetr=abs(penetracion_v.x);


            }
            else if(penetracion_v.y<penetracion_v.x){
                if(penetracion_v.x>radious_A){

                      if(cA.y<cB.y){
                         norm.y=-1;
                       }
                       else{
                        norm.y=1;
                       }
                     penetr=abs(penetracion_v.y);
                }
                else{
                     norm=(cB-cA).normalize();
                     norm.x=norm.x*-1;

                     penetr=abs(penetracion_v.y);

                }
            }
            else{

                         if(cA.y<cB.y){
                             norm.y=-1;
                          }
                          else{
                             norm.y=1;
                            }
                          penetr=abs(penetracion_v.y);


               }

              fold->set_normal(norm);
              fold->set_penetracion(penetr);
              Vector2 n_inverse=Vector2(norm.x*-1,norm.y*-1);

              Vector2 ctct=figA.get_center()+Vector2(n_inverse.x*radious_A,n_inverse.y*radious_A);
              vector<Vector2> contacts;
              contacts.push_back(ctct);
              fold->set_contacts(contacts);






           }

       }

   }

   return res;
}

bool Physic_Engine::Collision_Polygon_Polygon(Collider A, Collider B,Vector2 posA,Vector2 posB,int angA,int angB,ManiFold *fold){




   vector<Vector2> normsA=A.get_figure().get_normals();
   vector<Vector2> normsB=B.get_figure().get_normals();
  vector<Vector2>::iterator itA;
  vector<Vector2>::iterator itB;
   float minA=0.0f;
    float minB=0.0f;
    float maxA=0.0f;
    float maxB=0.0f;
    float penetracion=10000.0f;
    Vector2 norm=Vector2(0,0);
    Vector2 cA=A.get_figure().get_center()+posA;
    Vector2 cB=A.get_figure().get_center()+posA;

    Vector2 dir=(cB-cA).normalize();

  for(itA=normsA.begin();itA!=normsA.end();itA++){

    Vector2 axis=*itA;


    A.get_figure().project_vertices(axis,&minA,&maxA,posA);
    B.get_figure().project_vertices(axis,&minB,&maxB,posB);

     if (minA > maxB || minB > maxA) {

        return false;

     }
     float val1=(maxB-minA);
     float val2=(maxA-minB);
     float d=(val1<val2)?val1:val2;
     if(d<penetracion){

         penetracion=d;
         norm=axis;
     }

  }
  for(itB=normsB.begin();itB!=normsB.end();itB++){

    Vector2 axis=*itB;

    A.get_figure().project_vertices(axis,&minA,&maxA,posA);
    B.get_figure().project_vertices(axis,&minB,&maxB,posB);

     if (minA > maxB || minB > maxA) {

        return false;

     }
     float val1=(maxB-minA);
     float val2=(maxA-minB);
     float d=(val1<val2)?val1:val2;
     if(d<penetracion){

         penetracion=d;
         norm=axis;
     }

  }

   if(norm.dot(dir)>0){
       norm.x=norm.x*-1;
       norm.y=norm.y*-1;
   }
   if(fold!=NULL){
       fold->set_normal(norm);
       fold->set_penetracion(penetracion);

   }




   return true;
}
bool Physic_Engine::Collision_circle_Polygon(Collider A, Collider B,Vector2 posA,Vector2 posB,int angB,ManiFold *fold){

   bool res=false;


  Vector2 norm=Vector2(0,0);
  float penetracion=1000.0f;
  Vector2 axis=Vector2(0,0);
  float axis_dept=0.0f;
  float minA=0.0f;
  float minB=0.0f;
  float maxA=0.0f;
  float maxB=0.0f;

 Vector2 centroPolygon=B.get_figure().get_center();
 Vector2 centroCircle=A.get_figure().get_center();
 centroPolygon=centroPolygon+posB;
 centroCircle=centroCircle+posA;

 vector<Vector2> norms=B.get_figure().get_normals();
 vector<Vector2>::iterator it;
 Vector2 dir=centroCircle-centroPolygon;

  for(it=norms.begin();it!=norms.end();it++){

    Vector2 axis=*it;

    A.get_figure().project_Circle(axis,&minA,&maxA,posA);
    B.get_figure().project_vertices(axis,&minB,&maxB,posB);


     if (minA > maxB || minB > maxA) {

        return false;

     }
     float val1=(maxB-minA);
     float val2=(maxA-minB);
     float d=(val1<val2)?val1:val2;
     if(d<penetracion){
        penetracion=d;
        norm=axis;
        fold->set_penetracion(penetracion);
        fold->set_normal(norm);
     }


  }

  Vector2 norm_res=fold->get_normal();
  if(norm_res.dot(dir)<0.0f){
      norm_res.x=norm_res.x*-1;
      norm_res.y=norm_res.y*-1;
      fold->set_normal(norm_res);
  }

  vector<Vector2> ctcts;
  Vector2 contact=A.get_figure().get_center();
  float r_circle=A.get_figure().get_radius();
  Vector2 n_inverse=Vector2(norm_res.x*-1,norm_res.y*-1);
  contact=contact+Vector2(n_inverse.x*r_circle,n_inverse.y*r_circle);
  ctcts.push_back(contact);
  fold->set_contacts(ctcts);

  return true;


}


 bool Physic_Engine::Collision_square_square(Vector2 A_size,Vector2 B_size,Vector2 A_center,Vector2 B_center){

    Vector2 halfA=A_size;
    halfA.x=halfA.x*0.5f;
    halfA.y=halfA.y*0.5f;
    Vector2 halfB=B_size;
    halfB.x=halfB.x*0.5f;
    halfB.y=halfB.y*0.5f;




      if(A_center.x+halfA.x>=B_center.x-halfB.x && A_center.x-halfA.x<=B_center.x+halfB.x){


       if(A_center.y-halfA.y<=B_center.y+halfB.y && A_center.y+halfA.y >=B_center.y-halfB.y){
          return true;

       }
      }

    return false;
 }
 bool Physic_Engine::Collision_Ray_Line(Ray& ray, Line segmento){


     bool res=false;
     Vector2 p1_ray=ray.get_origen();
     Vector2 dir=ray.get_dir();
     Vector2 p2_ray=ray.get_origen()+(Vector2(dir.x*ray.get_distance(),dir.y*ray.get_distance()));
     Line ln_ray=Line(p1_ray,p2_ray);

    Vector2 axis1=segmento.get_normal();
    Vector2 axis2=ln_ray.get_normal();
    vector<Vector2> axis;
    axis.push_back(axis1);
    axis.push_back(axis2);
    vector<Vector2>::iterator it;
    float penetracion=0.0f;
    vector<Vector2> pts=segmento.get_points();
    if(pts.size()>=2){
        Vector2 p1=pts[0];
        Vector2 p2=pts[1];

    }
    for(it=axis.begin();it!=axis.end();it++){

         Vector2 axis_actual=*it;
         float minA=0.0f;
         float maxA=0.0f;
          float minB=0.0f;
         float maxB=0.0f;
         segmento.project_vertices(axis_actual,&minA,&maxA);
         ln_ray.project_vertices(axis_actual,&minB,&maxB);
          if (minA > maxB || minB > maxA) {

               return false;

          }
         float val1=(maxB-minA);
         float val2=(maxA-minB);
        float d=(val1<val2)?val1:val2;
        if(d<penetracion){

             penetracion=d;

        }

    }



    return true;

 }
 bool Physic_Engine::Raycast_Collision(Ray& ray,string asociado){
     tuple<vector<int>,vector<int>> in_cam=Camera::get_inCam();
     int sz=tuple_size<decltype(in_cam)>::value;
     if(sz<2){
           return false;
     }



    vector<Vector2> box_ray=ray.get_box_data();
    if(box_ray.size()<2){
        return false;
    }


    vector<int> objs_indexs=std::get<1>(in_cam);
    vector<int>::iterator it_objs;
    vector<Game_Object> objs=Game_Object::get_objs();

     bool res=false;
     for(it_objs=objs_indexs.begin();it_objs!=objs_indexs.end();it_objs++){

        int index=*it_objs;
        if(index>=0 && index<objs.size()){

            Game_Object g=objs[index];
            Collider *col=Component::get_Collider(g.get_name());
            Transform* t=Component::get_Transform(g.get_name());
            if(col!=NULL && t!=NULL && asociado!=g.get_name()){
                Figure f=col->get_figure();
                Vector2 pos=t->Get_Position()+g.get_parentOffset();
                Vector2 cent=f.get_center()+pos;

                bool aprox_coli=Collision_square_square(f.get_dim(),box_ray[0],cent,box_ray[1]);


                if((f.get_type()==1 || f.get_type()==2) && aprox_coli==true && col->is_trigger()==false){


                   vector<Vector2> vertexs=f.get_vertices();
                    vector<Line> lines;
                    for(int i=0;i<vertexs.size();i++){
                        Vector2 v1;
                        Vector2 v2;
                        if(i+1<vertexs.size()){
                            v1=vertexs[i];
                            v2=vertexs[i+1];
                        }
                        else{
                            v1=vertexs[i];
                            v2=vertexs[0];
                        }
                        v1=v1+pos;
                        v2=v2+pos;

                       Line ln=Line(v1,v2);

                       lines.push_back(ln);
                     }

                   vector<Line>::iterator it_ln;


                   for(it_ln=lines.begin();it_ln!=lines.end();it_ln++){
                        Line ln=*it_ln;



                        if(ln.get_normal().dot(ray.get_dir())<0){

                             if(Collision_Ray_Line(ray,ln)){

                               res=true;
                               break;
                           }
                        }

                   }
                }
                else if(f.get_type()==0 && aprox_coli==true){


                }




            }
        }
        if(res==true){
            break;
        }
     }
    return res;
 }

float Physic_Engine::gravedad=-9.8f;
bool Physic_Engine::use_gravity=true;
