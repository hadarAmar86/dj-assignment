#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */

DJLibraryService::~DJLibraryService() {
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();
}

 void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    int count = 0;
    for (auto& track : library_tracks) // prevent unecessary copies
    {
        if (track.type == "MP3")
        {
            MP3Track* new_mp3_track = new MP3Track(track.title,track.artists,track.duration_seconds,
                                                track.bpm,track.extra_param1,track.extra_param2);
            library.push_back(new_mp3_track);
            std::cout << "MP3: MP3Track created: " << new_mp3_track->get_bitrate() << " kbps" << std::endl;
            count+=1;
        }
        else
        {
            if (track.type == "WAV")
            {
                WAVTrack* new_wav_track = new WAVTrack(track.title,track.artists,track.duration_seconds,
                                                track.bpm,track.extra_param1,track.extra_param2);
                library.push_back(new_wav_track);
                std::cout << "WAV: WAVTrack created: " << new_wav_track->get_sample_rate() << "Hz/" << new_wav_track->get_bit_depth() << std::endl;
                count+=1;
            }
        }
    }
    std::cout << "[INFO] Track library built: "<< count << " tracks loaded\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    AudioTrack* found_track = playlist.find_track(track_title);
    if (found_track)
    {
        return found_track;
    }
    return nullptr;
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    int count = 0;
    playlist = Playlist(playlist_name);
    int real_index = 1;
    for (int index_track : track_indices)
    {
        if (index_track > 0 && index_track <= library.size())
        {
            count += 1;
            PointerWrapper<AudioTrack> p_clone = library[index_track-1]->clone();
            AudioTrack* raw_clone = p_clone.release();
            if (!p_clone)
            {
                std::cerr << "[ERROR] failed to clone in loadPlaylistFromIndices()." << std::endl;
            }
            raw_clone->load();
            raw_clone->analyze_beatgrid();
            playlist.add_track(raw_clone);
            std::cout << "Added '" << raw_clone->get_title() << "' to playlist '" << playlist_name << std::endl;
        }
        else
        {
            std::cout << "[WARNING] Invalid track index: " << index_track << std::endl;
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << count << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> plalist_title_tracks;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for (AudioTrack* track : tracks)
    {
        plalist_title_tracks.push_back(track->get_title());
    }
    return plalist_title_tracks;
}
