#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService() : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks\n";
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks\n";
    for (int i = 0; i < 2; ++i) {
        if (decks[i] != nullptr) {
            delete decks[i];
            decks[i] = nullptr;
        }
    }

}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    PointerWrapper<AudioTrack> cloned = track.clone();
    if (!cloned) {
            std::cerr << "[ERROR] Track: \"" << track.get_title()<< "\" failed to clone\n";
            return -1;
    }
    cloned->load();
    cloned->analyze_beatgrid();
    if (decks[0] == nullptr && decks[1] == nullptr) {
            AudioTrack* raw = cloned.release();
            decks[0] = raw;
            active_deck = 0;
            std::cout << "[Load Complete] '"<< decks[0]->get_title()<< "' is now loaded on deck 0\n";
            std::cout << "[Active Deck] Switched to deck 0\n";
            return 0;
    }
    std::cout << "\n=== Loading Track to Deck ===\n";
    size_t target_deck = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << target_deck << "\n";
    if (decks[active_deck] != nullptr && auto_sync) {
            int active_bpm = decks[active_deck]->get_bpm();
            int new_bpm    = cloned->get_bpm();
            if (std::abs(new_bpm - active_bpm) > bpm_tolerance) {
                sync_bpm(cloned);
            }
        }
        if (decks[target_deck] != nullptr) {
            std::cout << "[Unload] Unloading previous deck "<< target_deck<< " ('" << decks[target_deck]->get_title() << "')\n";
            delete decks[target_deck];
            decks[target_deck] = nullptr;
        }
        AudioTrack* raw = cloned.release();
        decks[target_deck] = raw;

        std::cout << "[Load Complete] '"<< decks[target_deck]->get_title()<< "' is now loaded on deck "<< target_deck << "\n";
        size_t old_deck = active_deck;
        if (decks[old_deck] != nullptr && old_deck != target_deck) {
            std::cout << "[Unload] Unloading previous deck "<< old_deck<< " ('" << decks[old_deck]->get_title() << "')\n";
            delete decks[old_deck];
            decks[old_deck] = nullptr;
        }

        active_deck = target_deck;
        std::cout << "[Active Deck] Switched to deck "<< target_deck << "\n";
        return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if (!decks[active_deck] || !track)
    {
        return false;
    }
    int original_bpm = decks[active_deck]->get_bpm();
    int new_bpm = track->get_bpm();
    int diff = std::abs(original_bpm - new_bpm);
    return diff <= bpm_tolerance;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if (!decks[active_deck] || !track)
    {
        std::cerr << "[ERROR] track or current active deck is nullptr" << std::endl;
    }
    else
    {
        int original_bpm = track->get_bpm();
        int average_bpm = (original_bpm + decks[active_deck]->get_bpm())/2;
        track->set_bpm(average_bpm);
        std:: cout << "[Sync BPM] Syncing BPM from "<< original_bpm <<" to " << average_bpm << std::endl;
    }
}