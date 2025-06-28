#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include<string>
#include<iostream>
#include<vector>
#include <SDL_mixer.h>
using namespace std;



struct AudioClip{
public:
    AudioClip();
    AudioClip(string source);
    void liberar();
    bool is_loaded();
    string get_name();
    Mix_Chunk* get_clip();
private:
    Mix_Chunk* clip;
    string name;
    bool loaded;

};

struct MusicClip{
public:
    MusicClip();
    MusicClip(string source);
    void liberar();
    bool is_loaded();
    string get_name();
    Mix_Music* get_clip();
private:
    Mix_Music* clip;
    string name;
    bool loaded;

};
class AudioManager
{
    public:
         static void set_AudioSources(vector<string> source_sfx, vector<string> source_musics);
         static void Play_Music(string name,int loops);
         static void Play_Music(int index,int loops);

         static void Play_Sfx(string name,int channel,int play_count);
         static void Stop_Music(string name);
         static void Reanude_Music(string name);
         static void Stop_Sfx(string name,int channel);
         static bool is_Playing_Music(string name);
         static void liberar();
         static AudioClip get_clip(string name);
         static MusicClip get_music(string name);
         static void set_volumeSfx(float val);
         static void set_volumeMusic(float val);
         static float get_volumeSfx();
         static float get_volumeBgm();

    protected:

    private:
       static float volume_sfx;
       static float  volume_bgm;
       static vector<AudioClip> sfx;
       static vector<MusicClip> musics;
};




#endif // AUDIOMANAGER_H
