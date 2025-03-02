# this file is a part of AES project which is under the MIT license; See LICENSE for more info

find_package(Catch2 3 REQUIRED)

add_executable(aestest
        cmm.cpp # component management
        lifecycle.cpp # entity lifecycles
)

# Sanitizer and debug flags
set(SANITIZER_FLAGS
        "-fsanitize=address,undefined"
        "-fno-omit-frame-pointer"
        "-fno-sanitize-recover=all"
)

# Debug configuration flags
set(DEBUG_FLAGS
        "-g"
        "-Wall"
        "-Wextra"
        "-Werror"
)

target_compile_options(aestest PRIVATE
        ${SANITIZER_FLAGS}
        ${DEBUG_FLAGS}
)

# Link sanitizer libraries
target_link_options(aestest PRIVATE
        ${SANITIZER_FLAGS}
)

target_link_libraries(aestest PRIVATE
        aes
        Catch2::Catch2WithMain
)