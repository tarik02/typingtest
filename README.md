# Typing speed test
Test your typing speed!

![Screenshot](/.github/assets/screenshot.png?raw=true "Screenshot")

## Dependencies
 - ncursesw (`# apt-get install libncursesw5-dev`)

## Building
```
$ make
```
or
```
$ make LOCALE=<your locale>
```
List of available locales:
 - en (English, default one)
 - ru (Russian)
 - uk (Ukrainian)

### Running
One you built it, you can run it with
```
$ ./typingtest
```

### Installing
Optionally you can install typingtest systemwide:
```
# make install
```
or to specified prefix:
```
$ make install PREFIX=<your prefix>
```
