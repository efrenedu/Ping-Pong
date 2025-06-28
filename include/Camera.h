#ifndef CAMERA_H
#define CAMERA_H
#include<Utility.h>
#include<Physics.h>
#include<data_render.h>

class Camera
{
    public:
        Camera();
        Camera(Vector2 c,Vector2 half_ar,bool actv,string look);
        void move_camera(Vector2 desplz);
        void apply_zoom(float val);
        float get_zoom();
        Vector2 get_pos();
        void set_pos(Vector2 c);
        bool in_camera(SDL_Rect rect_obj);
        bool is_active();
        virtual ~Camera();
        Vector2 get_Half_dim();
        void set_looking_index(int index);
        int get_looking_index();
        Vector2 get_lastTarget_pos();
        void set_lastTargetPos(Vector2 p);
        string get_looking_target();
        void look_at(Vector2 target);
        static void update_camActive(bool update_Quat);
        static tuple<vector<int>,vector<int>>& get_inCam();
        static tuple<vector<int>,vector<int>>& get_inCamDraw();
        static void update_tree();

        static Camera& get_camara(int index);
        static void set_camaras(vector<Camera>cams,int active_cam);
        static Camera& get_ActiveCamera(bool& found);
        static void editActiveCamera(Camera c);
        static float distance_update;

    protected:

    private:
        Vector2 Center;
        Vector2 Half_Area;
        Vector2 offset;
        Vector2 last_target_pos;
        bool active;
        float zoom;
        int looking;
        string look_name;
        static tuple<vector<int>,vector<int>> in_cam;
        static tuple<vector<int>,vector<int>> in_camDraw;


        static int cam_activa;
        static vector<Camera> cameras_list;

};

#endif // CAMERA_H
