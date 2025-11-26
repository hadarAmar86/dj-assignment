#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}

int DJControllerService::loadTrackToCache(AudioTrack& track) {
    std::string curr_title = track.get_title();
    if (cache.contains(curr_title))
    {
        cache.get(curr_title);
        return 1;
    }
    PointerWrapper<AudioTrack> p_clone = track.clone();
    AudioTrack* raw_clone = p_clone.get();
    if (!p_clone)
    {
        std::cerr << "[ERROR] Track: \"" << curr_title << "\" failed to clone in loadTrackToCache()." << std::endl;
        return -1;
    }
    raw_clone->load();
    raw_clone->analyze_beatgrid();
    if (cache.put(std::move(p_clone)))
    {
        return -1;
    }
    return 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}


AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return cache.get(track_title);
}
