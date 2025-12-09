#include "WAVTrack.h"
#include <iostream>

//constractor
WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

//implementing load according to WAV
void WAVTrack::load() {
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title
              << "\" at " << sample_rate <<"Hz/"<< bit_depth<<"bit (uncompressed)...\n";
    long size = duration_seconds * sample_rate * (bit_depth / 8) * 2;
    std::cout << "  → Estimated file size: "<< size <<" bytes\n";
    std::cout << "  → Fast loading due to uncompressed format.\n";
    // NOTE: Use exactly 2 spaces before the arrow (→) character

}
//implementing analyze_beatgrid according to WAV
void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // Requirements:
    // 1. Print analysis message with track title
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    long long beats = (duration_seconds / 60.0) * bpm;

    // 3. Print number of beats and mention uncompressed precision
    std::cout<<"  → Estimated beats: "<<beats <<"  → Precision factor: 1 (uncompressed audio)\n";
    // should print "  → Estimated beats: <beats>  → Precision factor: 1.0 (uncompressed audio)"
}
//implementing get_quality_score according to WAV
double WAVTrack::get_quality_score() const {
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    // NOTE: Cast beats to integer when printing
    double base_score = 70.0;
    if (sample_rate >= 44100)
    {
        base_score += 10;
        if (sample_rate >= 96000)
        {
            base_score += 5;
        }
    }
    if (bit_depth >= 16)
    {
        base_score += 10;
        if (bit_depth >= 24)
        {
            base_score += 5;
        }
    }

    return base_score;
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    WAVTrack* newWAV = new WAVTrack(*this);
    return PointerWrapper<AudioTrack>(newWAV);
}