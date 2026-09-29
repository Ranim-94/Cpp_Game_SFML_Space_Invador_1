# Cmake Notes

## Installation

```
sudo snap install cmake --classic
```

Verify version:
```
cmake --version
```


## Context

This document contains the information I learned for `cmake` whilie building my project

## Version number meaning

VERSION 1.0.0

- 3rd digit: bug fixes, something doesn't work and now it works, so I update the number
- 2nd digit: new features, and not craching <-> all is working with the new feature
- 1st digit: new iteration is going to the software


## CMake Benifits

- Cross platfrom between all OS (Linux, Window,...)
- generate several build files for make, ninja,Visual studio,...
- work with several compiler like GCC,Clang,MSVc,...
- work with external libraries like SDL,OpeGl,...