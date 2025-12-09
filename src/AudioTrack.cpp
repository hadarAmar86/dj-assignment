#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string& title, const std::vector<std::string>& artists, 
                      int duration, int bpm, size_t waveform_samples)
    : title(title), artists(artists), duration_seconds(duration), bpm(bpm), 
      waveform_data(nullptr), waveform_size(waveform_samples) {

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }
    #ifdef DEBUG
    std::cout << "AudioTrack created: " << title << " by " << std::endl;
    for (const auto& artist : artists) {
        std::cout << artist << " ";
    }
    std::cout << std::endl;
    #endif
}

//destructor of AudioTrack, deletes the pointer to the array in order to prevent memory leaks
AudioTrack::~AudioTrack() {
    #ifdef DEBUG
    std::cout << "AudioTrack destructor called for: " << title << std::endl;
    #endif
    delete[] waveform_data;
}

//
AudioTrack::AudioTrack(const AudioTrack& other) : title(other.title),artists(other.artists),
      duration_seconds(other.duration_seconds), bpm(other.bpm), waveform_data(nullptr),  waveform_size(other.waveform_size)
{
    if (other.waveform_data) { // check other.waveform_data is not NULL
        waveform_data = new double[waveform_size]; // Allocate a new array -> Deep copy: create a new array to avoid pointing to the same memory
        for (size_t i = 0; i < waveform_size; ++i) {
            waveform_data[i] = other.waveform_data[i];// coping the data
        }
    } else {
        waveform_data = nullptr; 
    }

    #ifdef DEBUG
    std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;
    #endif

    
}
//operator = :  track1 = track2;
// cleaning the first object, coping the data from the other object- deep copy
AudioTrack& AudioTrack::operator=(const AudioTrack& other) {
    
    #ifdef DEBUG
    std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    #endif
    if (this == &other) return *this;// **preventing self coping (track1 = track1)
    delete[] waveform_data;//prevent memory leaks
    waveform_data = nullptr;//Reset pointer to avoid dangling pointer issues - pointing to a deleted address.
    copyFrom(other);
    return *this;// returning reference to the object
}

//move - steal resources, empting other, not doing deep copy (not new), if the object is heavy.
// not alocating new memory. creating new object from rvalue
AudioTrack::AudioTrack(AudioTrack&& other) noexcept : title(std::move(other.title)), artists(std::move(other.artists)),
      duration_seconds(other.duration_seconds), bpm(other.bpm), waveform_data(other.waveform_data), waveform_size(other.waveform_size)
{
    #ifdef DEBUG
    std::cout << "AudioTrack move constructor called for: " << other.title << std::endl;
    #endif
    title = std::move(other.title);
    other.waveform_data = nullptr;//Reset pointer to avoid dangling pointer issues
    other.waveform_size = 0;
}

//move assignment operator - like move constractor - stealing, but working on an object that was already in use, so doing delete
AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {

    #ifdef DEBUG
    std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
    #endif
   if (this == &other)//preventing self implemention
        return *this;

    //deleting former data
    delete[] waveform_data;
    waveform_data = nullptr;

    //moving data
    title = std::move(other.title);
    artists = std::move(other.artists);
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;
    waveform_data = other.waveform_data;
    other.waveform_data = nullptr;
    other.waveform_size = 0;
    other.duration_seconds = 0;
    other.bpm = 0;
    //reterning reference
    return *this;
}
//deep copy
void AudioTrack::copyFrom(const AudioTrack& other)
{
    title = other.title;
    artists = other.artists;
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;

    if (other.waveform_data) {
        waveform_data = new double[waveform_size];
        for (size_t i = 0; i < waveform_size; ++i) {
            waveform_data[i] = other.waveform_data[i];
        }
    } else {
        waveform_data = nullptr;
    }
}

void AudioTrack::get_waveform_copy(double* buffer, size_t buffer_size) const {
    if (buffer && waveform_data && buffer_size <= waveform_size) {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}

void AudioTrack::set_bpm(int other_bpm)
{
    if (other_bpm < 0)
    {
        std::cout << "[ERROR] Cant set negative bpm" << std::endl;
    }
}