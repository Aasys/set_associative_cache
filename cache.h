//@author Aasys Sresta
//https://github.com/Aasys
//N-way Set Associative Cache 
//----------------------------------


#include <inttypes.h>


// stores caches attributes
typedef struct cache_property_t {
    unsigned int size;  //size in bytes
    unsigned int associativity;
    unsigned int line_size;  //size in bytes
    unsigned int address_size;
    unsigned int num_cache_lines;
    unsigned int num_sets;
    unsigned int word_bits;
    unsigned int set_bits;
    unsigned int tag_bits;
} CACHE_PROPERTY;

// a single cache line
typedef struct cache_line_t {
    uint64_t tag;
    unsigned int valid_bit;
    uint64_t data;
} CACHE_LINE;

// all cache lines in a single way
typedef struct cache_way_t {
    CACHE_LINE **cache_lines;
} CACHE_WAY;

//lru matrix for pseudo lru implementation
typedef struct lru_matrix_t {
    unsigned int ** matrix;
} LRU_MATRIX;

// root cache structure
// contains all cache ways
typedef struct cache_t {
    CACHE_WAY **cache_ways;
    LRU_MATRIX ** lru_matrices;

    CACHE_PROPERTY *cache_property;
} CACHE;

