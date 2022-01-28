#Windows

###Download and install msys2 from:
[msys2](https://repo.msys2.org/distrib/x86_64)

###Intall pkgs:
    pacman -Fy && pacman -Syuu
    pacman -S mingw-w64-x86_64-gcc
    pacman -S mingw-w64-x86_64-make
    pacman -S mingw-w64-x86_64-cmake
    pacman -S mingw-w64-x86_64-boost
    pacman -S mingw-w64-x86_64-ag
    pacman -S mingw-w64-x86_64-sqlite3
    pacman -S automake1.9 unrar vim
    ln -s /usr/bin/vim.exe /usr/bin/vi.exe
