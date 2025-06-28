#include "AudioManager.h"
#include "math structs.h"

vector<AudioClip> AudioManager::sfx;
vector<MusicClip> AudioManager::musics;
float AudioManager::volume_sfx=0.0f;
float AudioManager::volume_bgm=0.0f;

void AudioManager::set_AudioSources(vector<string> source_sfx, vector<string> source_musics){
    vector<string>::iterator it_sfx;
    vector<string>::iterator it_musics;

    for(it_sfx=source_sfx.begin();it_sfx!=source_sfx.end();it_sfx++){

        string source=*it_sfx;
        AudioClip clip=AudioClip(source);
        sfx.push_back(clip);
    }
    for(it_musics=source_musics.begin();it_musics!=source_musics.end();it_musics++){

        string source=*it_musics;
        MusicClip clip=MusicClip(source);
        musics.push_back(clip);
    }



}
void AudioManager::Play_Music(int index,int loops){
    if(index>=0 && index<musics.size()){
         MusicClip bgm=musics.at(index);

          if(bgm.is_loaded() && bgm.get_clip()!=NULL){
             Mix_PlayMusic(bgm.get_clip(),loops);
         }
         else{
             cout<<"music not loaded"<<endl;
         }
    }
}
void AudioManager:: Play_Music(string name,int loops){
       if(name==""){
          return;
       }
       MusicClip bgm=get_music(name);
       if(bgm.is_loaded()){
             if(Mix_PlayingMusic()==1){
                Mix_PauseMusic();
             }
             if(bgm.get_clip()!=NULL){
                 Mix_PlayMusic(bgm.get_clip(),loops);
             }
       }
       else{
           cout<<"music "<<name<<" not loaded"<<endl;
       }
}
void AudioManager::Play_Sfx(string name,int channel,int play_count){
    if(name==""){
        return;
    }
    AudioClip clip=get_clip(name);

     if(clip.is_loaded() && clip.get_clip()!=NULL){


            if(Mix_Playing(channel)!=0){
                  Mix_PlayChannel(channel+1,clip.get_clip(),play_count);

            }
            else{
                Mix_PlayChannel(channel,clip.get_clip(),play_count);

            }


    }
    else{
           cout<<"sfx "<<name<<" not loaded"<<endl;
    }

}
void AudioManager::set_volumeMusic(float val){

  if(val>1.0f){
    val=1.0f;
  }
  else if(val<0.0f){
    val=0.0f;
  }
  float next_vol=(val*val)*MIX_MAX_VOLUME;
  Mix_VolumeMusic(next_vol);
  volume_bgm=val;
}
float AudioManager::get_volumeSfx(){
   return volume_sfx;
}
float AudioManager::get_volumeBgm(){
   return volume_bgm;
}
void AudioManager::set_volumeSfx(float val){


  if(val>1.0f){
    val=1.0f;
  }
  else if(val<0.0f){
    val=0.0f;
  }
  float next_vol=(val*val)*MIX_MAX_VOLUME;
  Mix_Volume(0,next_vol);
  Mix_Volume(1,next_vol);
  volume_sfx=val;

}
void AudioManager:: Stop_Music(string name){
    MusicClip bgm=get_music(name);
    if(bgm.is_loaded()){
            if(Mix_PlayingMusic()==1){
                Mix_PauseMusic();
            }
       }
}
 void AudioManager:: Reanude_Music(string name){
     MusicClip bgm=get_music(name);
      if(bgm.is_loaded()){
            if(Mix_PausedMusic()==1){
                Mix_ResumeMusic();
            }
       }
 }
AudioClip AudioManager:: get_clip(string name){
    vector<AudioClip>::iterator it;
    AudioClip clip=AudioClip();
    for(it=sfx.begin();it!=sfx.end();it++){
        AudioClip aud=*it;
        if(aud.get_name()==name){
             clip=*it;
        }
    }
    return clip;
}
MusicClip AudioManager:: get_music(string name){
    vector<MusicClip>::iterator it;
    MusicClip clip=MusicClip();
    for(it=musics.begin();it!=musics.end();it++){
        MusicClip m=*it;
        if(m.get_name()==name){
            clip=*it;
        }
    }
    return clip;
}


void AudioManager::liberar(){
   vector<AudioClip>::iterator it1;
   vector<MusicClip>::iterator it2;
   if(Mix_PlayingMusic()==1){
      Mix_PauseMusic();
   }
   for(it1=sfx.begin();it1!=sfx.end();it1++){

        AudioClip cp=*it1;
        cp.liberar();
    }
    for(it2=musics.begin();it2!=musics.end();it2++){

        MusicClip msc=*it2;
        msc.liberar();
    }

    musics.clear();
    sfx.clear();
}

MusicClip::MusicClip(){
   loaded=false;
    name="";
}
MusicClip::MusicClip(string source){
     clip=Mix_LoadMUS(source.c_str());
     loaded=false;
     name="";
     if(clip!=NULL){
         loaded=true;
         name=source.substr(source.find_last_of("/\\") + 1);
         name=name.substr(0,name.length()-4);
     }
}
void MusicClip:: liberar(){
    if(clip!=NULL){
       Mix_FreeMusic(clip);
       clip=NULL;
    }

    loaded=false;
}
bool MusicClip::is_loaded(){
    return loaded;
 }
string MusicClip::get_name(){
   return name;
}
Mix_Music* MusicClip::get_clip(){
    return clip;
 }
AudioClip::AudioClip(){
    loaded=false;
     name="";
}
AudioClip::AudioClip(string source){




    clip=Mix_LoadWAV(source.c_str());
    loaded=false;
    name="";
     if(clip!=NULL){
         loaded=true;
         name=source.substr(source.find_last_of("/\\") + 1);
         name=name.substr(0,name.length()-4);
     }
     else{
        cout<<source<<" not loaded"<<endl;
     }
}
void AudioClip:: liberar(){
    if(clip!=NULL){
        Mix_FreeChunk(clip);
        clip=NULL;
    }

    loaded=false;
 }
bool AudioClip::is_loaded(){
    return loaded;
 }
string AudioClip::get_name(){
   return name;
}
Mix_Chunk* AudioClip::get_clip(){
    return clip;
}
