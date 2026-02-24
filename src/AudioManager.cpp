#include "audio/AudioManager.hpp"
#include "audio/AudioMap.hpp"

ee::audio::AudioManager::AudioManager()
{
    ma_engine_config m_config = ma_engine_config_init();
    ma_result result = ma_engine_init(&m_config, &m_engine);
    if (result != MA_SUCCESS)
        throw std::runtime_error("Error <ith the initialisation of miniaudio");
}

int ee::audio::AudioManager::load(const char *_path)
{
    ma_sound sound;
    if (ma_sound_init_from_file(&m_engine, _path, 0, nullptr, nullptr, &sound) != MA_SUCCESS)

        return -1;

    m_sounds.push_back(sound);
    return m_sounds.size() - 1;
}

int ee::audio::AudioManager::load(const char *_path, SoundGroupHandle _grpHandle)
{

    if (_grpHandle > m_soundsGroups.size())
        return -1;

    if (_grpHandle == m_soundsGroups.size())
    {
        ma_sound_group newGroup;
        if (ma_sound_group_init(&m_engine, 0, nullptr, &newGroup) != MA_SUCCESS)
            return -1;
        m_soundsGroups.push_back(newGroup);
    }

    ma_sound sound;
    if (ma_sound_init_from_file(&m_engine, _path, 0, &m_soundsGroups[_grpHandle], NULL, &sound) != MA_SUCCESS)
        return -1;

    m_sounds.push_back(sound);
    return m_sounds.size() - 1;
}
void ee::audio::AudioManager::play(SoundHandle _handle, uint64_t _framedelay)
{
    if (_handle > m_sounds.size() - 1)
        return;

    ma_sound_set_fade_in_pcm_frames(&m_sounds[_handle], 0., 1., static_cast<ma_uint64>(_framedelay));
    ma_sound_start(&m_sounds[_handle]);
}

void ee::audio::AudioManager::stop(SoundHandle _handle, uint64_t _framedelay)
{

    if (_handle > m_sounds.size() -1)
        return;


    ma_sound_set_stop_time_in_pcm_frames(&m_sounds[_handle], ma_engine_get_time_in_pcm_frames(&m_engine) + static_cast<ma_uint64>(_framedelay));
}

void ee::audio::AudioManager::setVolume(SoundHandle _handle, float _volume)
{
    if (_handle > m_sounds.size() -1)
        return;

    ma_sound_set_volume(&m_sounds[_handle], _volume);
}

void ee::audio::AudioManager::setGroupVolume(SoundGroupHandle _grpHandle, float _volume)
{
    if (_grpHandle > m_soundsGroups.size() - 1)
        return;

    ma_sound_group_set_volume(&m_soundsGroups[_grpHandle], _volume);
}

void ee::audio::AudioManager::setGeneralVolume(float _volume)
{
    ma_engine_set_volume(&m_engine, _volume);
}
