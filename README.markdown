# theseus.f

A minimalist game for 26th Ludum Dare 48h competition ([competition
entry](http://www.ludumdare.com/compo/ludum-dare-26/?action=preview&uid=21358)).
Built with C, SDL and love.

## building

### Unix

On Unix systems building should be straighforward. The game requires libSDL
v1.2 and libSDL-image v1.2, most GNU/Linux distributions provide those packages
through the system repositiories, on Debian based systems (like Ubuntu) you may
get them with apt:

    sudo apt-get install libsdl1.2-dev 
    sudo apt-get install libsdl-image1.2-dev

To build the game simply use make by typing:

    make

If SDL is installed correctly, the game will be compiled. The same goes for all
BSDs and Macs, although process installing SDL will differ.

The default compiler used in makefile is GCC. You may use any C99 compiler you
like, but you'll have to either edit makefile or compile it in a more manual
fashion.

### Windows

Code is written in C99 so Microsoft C++ Compier won't be a suitable choice as
it supports C90 only. The Windows version of _theseus.f_ was compiled using
MinGW and MSYS utilities, both can be installed by automatic installer found
[here](http://www.mingw.org/wiki/InstallationHOWTOforMinGW).

After installing the tools you'll probably need to edit your system PATH
variable to include MinGW and MSYS _bin_ directories.

Next thing you'll need to do is to install SDL and SDL-image, there are some
good resources online explaining this step by step, namely: [installing
SDL](http://lazyfoo.net/SDL_tutorials/lesson01/windows/mingw/index.php) and
[installing SDL extension
libraries](http://lazyfoo.net/SDL_tutorials/lesson03/windows/mingw/).

After everything has been installed all you need to do is open cmd.exe, cd into
directory with _theseus.f_ sources and type

    make windows

If you managed to do all previous steps correctly an .exe file will be created.

## license

All code is licensed under the MIT license:


The MIT License (MIT)

Copyright (c) 2013 Mateusz Belicki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

