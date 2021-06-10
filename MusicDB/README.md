# MusicDB

Design a C++ in-memory database of songs with the following details:
* A song has a name, an artist (singer, band, etc.), a release year, and might belong to an album
* An album has a title, genre, artist (a single band/singer or multiple performers), release year, and a list of songs
* An artist has a name and a type (singer/band/composer)

The database should be persistable; that is, we should be able to read and write it to disk.

The DB should support the following searches:
* Song by year
* Song by year range (e.g. all songs between 2018-2020)
* Album by year
* Album by year range
