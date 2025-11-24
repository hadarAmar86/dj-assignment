#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    std::string curr_title = track.get_title();
    if (cache.contains(curr_title))
    {
        cache.get(curr_title);
        return 1;
    }
    AudioTrack* raw_clone = track.clone().release();
    if (!raw_clone)
    {
    std::cerr << "[ERROR] Track \"" << raw_clone->get_title() 
              << "\" failed to clone in loadTrackToCache()." << std::endl;
    return;
    }
    raw_clone->load();
    raw_clone->analyze_beatgrid();
    if (cache.put(track.clone()))
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

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    return nullptr; // Placeholder
}
