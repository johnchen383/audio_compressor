#define MINIMP3_IMPLEMENTATION
extern "C" {
    #include "minimp3_ex.h"
}
#include <iostream>
#include <fstream>
#include <vector>

std::vector<uint8_t> ReadFile(const std::string& filename) {
    std::ifstream f(filename, std::ios::binary);
    if (!f) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return {};
    }
    return std::vector<uint8_t>(std::istreambuf_iterator<char>(f), {});
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: audio_compressor <file.mp3>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::vector<uint8_t> mp3Data = ReadFile(filename);
    std::cout << "Read " << mp3Data.size() << " bytes from MP3 file." << std::endl;

    if (mp3Data.empty()) {
        std::cerr << "MP3 file is empty or couldn't be read." << std::endl;
        return 1;
    }

    mp3dec_ex_t dec;
    int open_status = mp3dec_ex_open_buf(&dec, mp3Data.data(), mp3Data.size(), MP3D_SEEK_TO_SAMPLE);
    if (open_status != 0) {
        std::cerr << "mp3dec_ex_open_buf() failed with code: " << open_status << std::endl;
        return 1;
    }

    std::cout << "Decoder initialized successfully." << std::endl;
    std::cout << "Expecting " << dec.samples << " total samples." << std::endl;
    std::cout << "Channels: " << dec.info.channels << ", Sample Rate: " << dec.info.hz << std::endl;

    std::vector<int16_t> pcm(dec.samples);
    size_t samples_read = mp3dec_ex_read(&dec, pcm.data(), dec.samples);
    mp3dec_ex_close(&dec);

    std::cout << "Decoded " << samples_read << " samples." << std::endl;

    return 0;
}
