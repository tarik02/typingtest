# Typing speed test
Test your typing speed!

![Screenshot](/.github/assets/screenshot.png?raw=true "Screenshot")

## Dependencies
 - Linux: ncursesw (`# apt-get install libncursesw5-dev`)
 - macOS: ncurses (`$ brew install ncurses`)

## Building
```
$ mkdir build
$ cd build
$ cmake [-DLOCALE=en/ru/uk] ..
$ cmake --build .
```
or
List of available locales:
 - en (English, default)
 - ru (Russian)
 - uk (Ukrainian)

### Running
One you built it, you can run it with
```
$ ./typingtest
```

### Installing
Optionally you can install typingtest system-wide:
```
# cmake --build . --target install
```
