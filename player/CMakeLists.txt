project(player)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

pkg_check_modules(NCURSES REQUIRED IMPORTED_TARGET ncurses)
pkg_check_modules(VLC REQUIRED IMPORTED_TARGET libvlc)

add_executable(player src/main.c src/player.c)

target_link_libraries(${PROJECT_NAME}
    logging
    PkgConfig::NCURSES
    PkgConfig::VLC
    Threads::Threads
)