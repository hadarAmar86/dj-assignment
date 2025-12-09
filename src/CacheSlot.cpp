#include "CacheSlot.h"
//constractor
CacheSlot::CacheSlot() : 
    track(nullptr), 
    last_access_time(0), 
    occupied(false){
}
// storing the pointer wrapper in the slot
void CacheSlot::store(PointerWrapper<AudioTrack> track_ptr, uint64_t access_time) {
    track = std::move(track_ptr);
    last_access_time = access_time;
    occupied = true;
}
//accsses to the copy of the track from cache
AudioTrack* CacheSlot::access(uint64_t access_time) {
    if (!occupied) {
        return nullptr;
    }
    
    last_access_time = access_time;
    return track.get();
}
// cleaning the slot by using delete of the wrapper
void CacheSlot::clear() {
    track.reset(nullptr);
    occupied = false;
    last_access_time = 0;
}