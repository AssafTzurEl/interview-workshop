#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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

enum class ArtistType
{
    Singer,
    Band,
    Composer
};

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

typedef std::shared_ptr<Artist> ArtistPtr;

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

typedef std::shared_ptr<Song> SongPtr;

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

typedef std::shared_ptr<Album> AlbumPtr;


// DB tables:
std::unordered_map<Song::id_t, SongPtr> SongsTable;
std::unordered_map<Artist::id_t, ArtistPtr> ArtistsTable;
std::unordered_map<Album::id_t, AlbumPtr> AlbumsTable;

// Links:
std::unordered_multimap<Song::id_t, Album::id_t> SongIdsToAlbumIdsTable;

// Indices:
std::unordered_multimap<Album::id_t, SongPtr> AlbumIdsToSongsIndex;
std::unordered_multimap<Artist::id_t, SongPtr> ArtistIdsToSongsIndex;
std::unordered_multimap<Album::id_t, ArtistPtr> AlbumIdsToArtistsIndex;

// Search requirements:

// Songs by year:
std::unordered_multimap<size_t, SongPtr> YearsToSongsIndex;

// Songs by year range: For each year, get all the songs published that year:
std::map<size_t, std::vector<SongPtr>> YearsToSongsIndex;

// Albums by year:
std::unordered_multimap<size_t, AlbumPtr> YearsToAlbumsIndex;

// Albums by year range:
std::map<size_t, std::vector<AlbumPtr>> YearsToAlbumsIndex;

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