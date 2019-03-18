/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "sound.h"

int main()                                    // Main function
{
  sound_t* sound;
  sound = sound_run(0, 1);
  sound_wave(sound, 0, SINE);
  while(1)
  { 
    if(get_state(2))  {
      sound_volume(sound, 0, 127);
      sound_note(sound, 0, C7);
      pause(100);
      sound_note(sound, 0, D7);
      pause(100);
      sound_note(sound, 0, E7);
      pause(500);
      sound_volume(sound, 0, 0);
    }
    if(get_state(3))  {
      sound_volume(sound, 0, 127);
      sound_note(sound, 0, E7);
      pause(100);
      sound_note(sound, 0, D7);
      pause(100);
      sound_note(sound, 0, C7);
      pause(500);
      sound_volume(sound, 0, 0);
    }            

    
  }  
}
