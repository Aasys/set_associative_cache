N-way Set Associative Cache with pseudo LRU replcaement policy

## DATA STRUCTURE:

    CACHE *|
    
           |→CACHE_WAY ** |
        
           |              |→CACHE_LINE** |

           |                             |→uint64_t tag

           |                             |→unsigned int valid_bit
        
           |→LRU_MATRIX ** |
           |               |→ unsigned int ** matrix
           |→CACHE_PROPERTY

struct CACHE stores array of CACHE_WAY with length n of given n-way set associative. struct CHACHE_WAY is a collection of blocks for that particular way. Its length is equal to the total number of sets. Each block is implemented in struct CACHE_LINE with attribute tag and valid_bit

struct CACHE also store collection of LRU_MATRIX, one matrix for each set, used for implementation of pseudo-LRU policy. struct LRU_MATRIX is implemented a simple array of array of unsigned int.

Lastly, struct CACHE_PROPERTY is included in CACHE as a convenient store of cache attributes like size, associativity, number of sets, number of set bits, number of tag bits.

## ALGORITHM:

create_cache() initializes CACHE with all its parameters. 

access_cache() has been modified to lookup tag in cache, return a hit if a valid one is found and update the associated LRU_MATRIX. On cache miss, the implemented psuedo-LRU policy uses the LRU_matrix to determine the block to replace.

Pseudo-LRU Policy:

    https://en.wikipedia.org/wiki/Pseudo-LRU
            
