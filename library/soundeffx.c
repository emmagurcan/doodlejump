#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "soundeffx.h"
#include "math.h"

// Music Declaration
const char *BACKGROUND_MUSIC;
const char *PROMPT_TEXTURE;

// Background Music Declaration
Mix_Music *GAME_MUSIC = NULL;
char *game_music_file = "media/patakas_world.wav";

// Sound Effects Declaration
Mix_Chunk *boing = NULL; // will figure out w 

void soundeffx_init() {
    Mix_OpenAudio(44100, AUDIO_S16SYS, 4, 2048);
    Mix_Volume(-1, 50);
    GAME_MUSIC = Mix_LoadMUS(game_music_file);
}

void soundeffx_play_background_music(){
    Mix_PlayMusic(GAME_MUSIC, -1);
}

void soundeffx_set_game_music_volume(int volume){
    Mix_Volume(-1, volume);
}

void soundeffx_close() {
    Mix_FreeMusic(GAME_MUSIC);
    Mix_Quit();
}