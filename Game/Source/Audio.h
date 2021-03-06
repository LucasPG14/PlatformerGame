#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool Load(pugi::xml_node& load);

	bool Save(pugi::xml_node& save);

	void SetMusicVolume(int index);

	void SetFxVolume(int index);

	void MusicPause();

private:

	_Mix_Music* music;
	List<Mix_Chunk*> fx;

	uint volumeMusic = 0;
	uint volumeFx = 0;
};
#endif // __AUDIO_H__