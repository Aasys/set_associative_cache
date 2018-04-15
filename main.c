//@author Aasys Sresta
//https://github.com/Aasys
//N-way Set Associative Cache 
//----------------------------------

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "cache.h"

extern CACHE *create_cache(unsigned int size, unsigned int associativity, unsigned int line_size);

extern unsigned int logbase2(unsigned int);

void print_cache(CACHE *);

int access_memory_address(CACHE *cache, uint64_t memory_address);

void simulate_unified_cache(CACHE *u_cache, FILE *fp);

void simulate_i_d_cache(CACHE *i_cache, CACHE *d_cache, FILE *fp);

uint64_t extractBits(uint64_t address, unsigned startIndex, unsigned nBits);

int lookup_tag(CACHE *cache, uint64_t tag, uint64_t index);

void lru_update(LRU_MATRIX *lru_matrix, int hit_index, unsigned int associativity);

int lru_get(LRU_MATRIX *lru_matrix, unsigned int associativity);

//access a memory address
int access_memory_address(CACHE *cache, uint64_t memory_address) {
    uint64_t offset = extractBits(memory_address, 0, cache->cache_property->word_bits); // UNUSED
    uint64_t index = extractBits(memory_address, cache->cache_property->word_bits, cache->cache_property->set_bits);
    uint64_t tag = memory_address >> (cache->cache_property->word_bits + cache->cache_property->set_bits);

    return lookup_tag(cache, tag, index);
}

//look up tag bits from address
int lookup_tag(CACHE *cache, uint64_t tag, uint64_t index) {
    int i;
    CACHE_LINE *cache_line;

    //for each cache_way find the associated set on index
    for (i = 0; i < cache->cache_property->associativity; i++) {
        cache_line = cache->cache_ways[i]->cache_lines[index];
        if (cache_line->valid_bit == 1 && cache_line->tag == tag) {
            // tag found and is valid
            // update LRU matrix
            lru_update(cache->lru_matrices[index], i, cache->cache_property->associativity);
            return 1;
        }
    }

    // cache miss, find replacement using LRU matrix
    int replacement_block_index = lru_get(cache->lru_matrices[index], cache->cache_property->associativity);
    // add the tag to cache
    cache_line = cache->cache_ways[replacement_block_index]->cache_lines[index];
    cache_line->valid_bit = 1;
    cache_line->tag = tag;
    // update lru matrix
    lru_update(cache->lru_matrices[index], replacement_block_index, cache->cache_property->associativity);
    return 0;
}


//updates the LRU_MATRIX on cache hit
void lru_update(LRU_MATRIX *lru_matrix, int hit_index, unsigned int associativity) {
    int i;
    // set hit row to 1
    for (i = 0; i < associativity; i++) {
        lru_matrix->matrix[hit_index][i] = 1;
    }

    //set hit column to 0
    for (i = 0; i < associativity; i++) {
        lru_matrix->matrix[i][hit_index] = 0;
    }
}

//get LRU block index
int lru_get(LRU_MATRIX *lru_matrix, unsigned int associativity) {
    int min_sum = associativity + 1; //1 higher than the maximum possible sum
    int lru_block_index = -1;

    int i, j, row_sum;
    for (i = 0; i < associativity; i++) {
        //find the number of 1s in row
        row_sum = 0;
        for (j = 0; j < associativity; j++) {
            row_sum += lru_matrix->matrix[i][j];
        }

        //if it is lower than previous, keep track
        if (row_sum < min_sum) {
            min_sum = row_sum;
            lru_block_index = i;
        }
    }

    return lru_block_index;
}

uint64_t extractBits(uint64_t address, unsigned startIndex, unsigned nBits) {
    uint64_t mask = 0;
    unsigned i;
    for (i = startIndex; i < startIndex + nBits; i++)
        mask |= 1 << i;

    return (mask & address) >> startIndex;
}

int main(int argc, char **argv) {
   
    // TODO: supply {MEMORY_ADDRESS} and call
    hit = access_memory_address(u_cache, {MEMORY_ADDRESS});
}
