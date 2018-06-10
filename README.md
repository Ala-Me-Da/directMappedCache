# DirectMappedCache
ECS 154a project to implement a 256-byte Direct mapped cache with 8 byte cache lines. 

# Compiling & Executing
Requires C++11 or greater to compile. 
Open command prompt and type the folloiwng to complie the project: 
```
g++ -std=c++11 DirectMappedCache.h DirectMappedCache.cpp dmcache2.cpp
```
Next, use run the resulting a.out executable with one of the test files.
```
./a.out test-file.txt 
```

This will output a text file named ```dm-out.txt``` containing all the data read from the Direct mapped cache represented in hexadecimal, followed by the value a dirty bit, and the the hit bit. 

# Documentation 

