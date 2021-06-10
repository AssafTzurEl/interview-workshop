# Music DB - design

## Song

A song comprises the following properties:

```C++
std::string name;
Artist *artist;
size_t releaseYear;
std::list<Album *> albums;
```

Since the DB should be written to a file, we cannot use pointers. Instead, each entity (song, artist, album) has a unique ID. This is the ID generator:

```C++
class IdGenerator
{
public:
    IdGenerator()
        : m_lastId(0)
    {}

    size_t next()
    {
        ++m_lastId;
        return m_lastId;
    }

private:
    size_t m_lastId;
};
```

Each entity class contains a static `IdGenerator` member, and its constructor assigns it its unique ID. The ID is constant and cannot be modified. For example:

```C++
class Song
{
public:
    Song()
        : m_id(s_idGenerator.next())
    {}

    size_t GetId() const
    {
        return m_id;
    }

private:
    static IdGenerator s_idGenerator;

    const size_t m_id;
};
```

We're going to have a lot of IDs going around: A song is linked to an album using the album's ID; it is also linked to an artist using the artist's ID; an album is linked to its songs by their song IDs, etc. To prevent confusion, we `typedef` the various IDs. For example:

```C++
class Song
{
public:
    typedef size_t id_t;

    Song()
        : m_id(s_idGenerator.next())
    {}

    id_t GetId() const
    {
        return m_id;
    }

private:
    static IdGenerator s_idGenerator;

    const id_t m_id;
};
```

To the outside world, this new type is seen as `Song::id_t`.

**Note:** `typedef` doesn't prevent you from assigning one ID type to another; it's just a visual aid. To prevent accidental assignment of song ID to an album ID, you'll have to declare a special type (classe) for each ID. This is left as an exercise to the reader. :)

Let's add the song's name:

```C++
class Song
{
public:
    typedef size_t id_t;

    Song(const std::string &name)
        : m_id(s_idGenerator.next()), m_name(name)
    {}

    id_t GetId() const
    {
        return m_id;
    }

    void SetName(const std::string newName)
    {
        m_name = newName;
    }

    const std::string &GetName() const
    {
        return m_name;
    }

private:
    static IdGenerator s_idGenerator;

    const id_t m_id;
    std::string m_name;
};
```

The name may be updated, so we have a setter method for it, unlike the song ID. From now on, we'll skip all the setters and getters (as well as constructors), to make things shorter.

Finally, we add the song's year and artist. We use the artist's ID type, which will be decalre later.

```C++
class Song
{
public:
    typedef size_t id_t;

    // ctor, setters, getters

private:
    static IdGenerator s_idGenerator;

    const id_t m_id;
    std::string m_name;
    size_t m_year;
    Artist::id_t m_artistId;
};
```

Since a song may be published in multiple albums and an album contains many songs, the linkage between them is maintained in a different data structure.

## Artist

Artists can be singers, bands, orchestras, composers. etc. We can use a string for artist type, but if we have a closed list of types, we can use `enum` (never use an `enum` for a modifiable list of items, or you'll violate the sacred [open/closed principle](https://en.wikipedia.org/wiki/Open%E2%80%93closed_principle)!).

```C++
class Artist
{
public:
    typedef size_t id_t;

    // ctor, setters, getters

private:
    static IdGenerator s_idGenerator;

    const id_t m_id;
    std::string m_name;
    ArtistType m_type;
};
```

## Album

```C++
class Album
{
public:
    typedef size_t id_t;

    // ctor, setters, getters

private:
    static IdGenerator s_idGenerator;

    const id_t m_id;
    std::string m_name;
    size_t m_year;
};
```

## Basic tables

To store entities, we need database tables. In C++, we can simply use vectors or lists, but we want better performance. Since most searches on a table is for a specific ID, we want the container to be sorted (or indexed) this way. Since IDs are discrete, there's no need to sort (and get O(n) complexity for fetching) - we can use an `unordered_map`, which promises O(1) complexity (possibly using a hash table, but the C++ standard forces complexity requirements and doesn't state implementation).

So here's the first shot at the DB tables:
```C+++
std::unordered_map<Song::id_t, Song> SongsTable;
std::unordered_map<Artist::id_t, Artist> ArtistsTable;
std::unordered_map<Album::id_t, Album> AlbumsTable;
```

Using this implementation, each write and read to/from a table involves copying the entity. We could use pointers, but then we'd run into ownership issues. Luckily, C++11 (or, if you're using an older compiler, the [Boost library](https://www.boost.org/)) added smart pointers. Let's use them:

```C++
typedef std::shared_ptr<Song> SongPtr;
typedef std::shared_ptr<Album> AlbumPtr;
typedef std::shared_ptr<Artist> ArtistPtr;

std::unordered_map<Song::id_t, SongPtr> SongsTable;
std::unordered_map<Artist::id_t, ArtistPtr> ArtistsTable;
std::unordered_map<Album::id_t, AlbumPtr> AlbumsTable;
```

The `*Ptr typedef`s can be set inside the entity classes, but some people prefer to declare them globally, right after the class declaration.

## Links

### Songs to albums

To link songs with their albums, we can use a `multimap` - a sorted key-value container that can contain the same key multiple times. Again, our map will be unordered (O(1), remember?).

```C++
std::unordered_multimap<Song::id_t, Album::id_t> SongIdsToAlbumIdsTable;
```

Searching for all the albums that contain a song looks like this (integrity checks etc. omitted):
```C++
std::vector<AlbumPtr> FindAlbumsBySong(Song::id_t songId)
{
    // Get iterators pointing at the beginning and end of album IDs linked
    // to the song:
    auto range = SongIdsToAlbumIdsTable.equal_range(songId);
    std::vector<AlbumPtr> result;

    // For each album ID, fetch the album object and add it to the result
    // collection:
    std::for_each(range.first, range.second,
        [&result](Album::id_t *pAlbumId) { result.push_back(AlbumsTable[*pAlbumId]); });

    return result;
}
```

Remember that this table contains critical data, and so must be included in any serialization/deserialization we do (persistance to disk).

To allow fast searching in the other direction, we create an index that looks almost exactly the opposite:

```C++
std::unordered_multimap<Album::id_t, SongPtr> AlbumIdsToSongsIndex;
```

Each time we create a song-to-album link in `SongIdsToAlbumIdsTable`, we must also add a complementary album-to-song link to `AlbumIdsToSongIdsIndex`. Since the data exists elsewhere (albeit not efficiently), it doesn't have to be persistent and so we can use pointers to actual song objects instead of song IDs.

We can now search for all the songs in a single album.

### Artists to songs

While each song has a single artist, linked to it using a data member, an artist may have multiple songs. We can get this information by searching the songs table, but to get better performance, we create yet another index:

```C++
std::unordered_multimap<Artist::id_t, SongPtr> ArtistIdsToSongsIndex;
```

### Albums to artists

An album may have one artist or more, depending on its songs. We can extract that information by moving from the album to its songs to their artists, but since we have a requirement for direct album-to-artist link, we have to create yet another index:

```C++
std::unordered_multimap<Album::id_t, ArtistPtr> AlbumIdsToArtistsIndex;
```

Remember that any change to the albums or songs tables that involves adding or removing an artist requires an update to `AlbumIdsToArtistIdsIndex`.

## Searches

Search requirements manifest in more indices, to improve performance of these searches:

### Songs by year

```C++
std::unordered_multimap<size_t, SongPtr> YearsToSongIdsIndex;
```

### Songs by year range

We can use the `YearsToSongIdsIndex`, iterating through the years and accumulating songs. But if there's a special requirement for this search, we better optimize it:
```C++
std::map<size_t, std::vector<SongPtr>> YearsToSongsIndex;
```

Each song is pushed to the appropriate vector inside the map. We can get an iterator that points to the first year in the range (using, for exmaple, `YearsToSongsIndex.lower_bound(2018)`), another one that points beyond the last year (e.g. `YearsToSongsIndex.upper_bound(2020)`), and then go through the iterators and accumulate the songs in the vectors.

### Albums by year

```C++
std::unordered_multimap<size_t, AlbumPtr> YearsToAlbumsIndex;
```

### Albums by year range

```C++
std::map<size_t, std::vector<AlbumPtr>> YearsToAlbumsIndex;
```

You can find all the code in one go in [MusicDB.cpp](MusicDB.cpp).
