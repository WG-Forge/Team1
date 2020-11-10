cmake_minimum_required(VERSION 3.17)
project(Rail_simulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(Rail_simulator
        libs/rapidjson/error/en.h
        libs/rapidjson/error/error.h
        libs/rapidjson/internal/biginteger.h
        libs/rapidjson/internal/clzll.h
        libs/rapidjson/internal/diyfp.h
        libs/rapidjson/internal/dtoa.h
        libs/rapidjson/internal/ieee754.h
        libs/rapidjson/internal/itoa.h
        libs/rapidjson/internal/meta.h
        libs/rapidjson/internal/pow10.h
        libs/rapidjson/internal/regex.h
        libs/rapidjson/internal/stack.h
        libs/rapidjson/internal/strfunc.h
        libs/rapidjson/internal/strtod.h
        libs/rapidjson/internal/swap.h
        libs/rapidjson/allocators.h
        libs/rapidjson/cursorstreamwrapper.h
        libs/rapidjson/document.h
        libs/rapidjson/encodedstream.h
        libs/rapidjson/encodings.h
        libs/rapidjson/filereadstream.h
        libs/rapidjson/filewritestream.h
        libs/rapidjson/fwd.h
        libs/rapidjson/istreamwrapper.h
        libs/rapidjson/memorybuffer.h
        libs/rapidjson/memorystream.h
        libs/rapidjson/ostreamwrapper.h
        libs/rapidjson/pointer.h
        libs/rapidjson/prettywriter.h
        libs/rapidjson/rapidjson.h
        libs/rapidjson/reader.h
        libs/rapidjson/schema.h
        libs/rapidjson/stream.h
        libs/rapidjson/stringbuffer.h
        libs/rapidjson/writer.h
        tests/graph_tests/test.cpp
        src/graph.cpp
        src/graph_parser.cpp
        main.cpp)

include(libs/RapidJson.cmake)
include(libs/GTest.cmake)

set_target_properties(Rail_simulator PROPERTIES LINKER_LANGUAGE CXX)

