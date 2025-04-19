cmake_minimum_required(VERSION 3.10)
project(AudioCompressor)

set(CMAKE_CXX_STANDARD 14)

include_directories(third_party/minimp3)

add_executable(audio_compressor main.cc)

