//@author Aasys Sresta
//https://github.com/Aasys
//N-way Set Associative Cache 
//----------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cache.h"

#define ADDRESS_SIZE 64

CACHE *create_cache(unsigned int size, unsigned int associativity, unsigned int line_size);

void define_cache_property(CACHE *cache, unsigned int size, unsigned int associativity, unsigned int line_size);

void print_cache_property(CACHE_PROPERTY *);

void define_cache_ways(CACHE *);

void define_cache_way(CACHE_WAY *cache_way, unsigned int num_lines_in_set);

void define_lru_matrices(CACHE *cache);

unsigned int logbase2(unsigned int);

/* Creates and initializes the cache structure based on the size (KB), associativity, and 
 * line size (Bytes) parameters. 
 * */
CACHE *create_cache(unsigned int size, unsigned int associativity, unsigned int line_size) {
    CACHE *cache = (CACHE *) malloc(sizeof(CACHE));

    define_cache_property(cache, size, associativity, line_size);

    define_cache_ways(cache);

    define_lru_matrices(cache);

    return cache;
}

void define_cache_property(CACHE *cache, unsigned int size, unsigned int associativity, unsigned int line_size) {
    CACHE_PROPERTY *cache_property = (CACHE_PROPERTY *) malloc(sizeof(CACHE_PROPERTY));
    cache->cache_property = cache_property;

    // define cache specification
    cache_property->size = size;
    cache_property->associativity = associativity;
    cache_property->line_size = line_size;
    cache_property->address_size = ADDRESS_SIZE;
    cache_property->num_cache_lines = (cache_property->size * 1024) / cache_property->line_size;
    cache_property->num_sets = cache_property->num_cache_lines / cache_property->associativity;
    cache_property->word_bits = logbase2(cache_property->line_size);
    cache_property->set_bits = logbase2(cache_property->num_sets);
    cache_property->tag_bits = ADDRESS_SIZE - (cache_property->word_bits + cache_property->set_bits);

    print_cache_property(cache_property);
}

void print_cache_property(CACHE_PROPERTY *cache_property) {
    printf("***Cache Properties***\n");
    printf("Size:                    %d KB\n", cache_property->size);
    printf("Associativity            %d ways\n", cache_property->associativity);
    printf("Line size:               %d bytes\n", cache_property->line_size);
    printf("Address Size:            %d bits\n", cache_property->address_size);
    printf("Number of cache lines:   %d lines\n", cache_property->num_cache_lines);
    printf("Number of sets in cache: %d sets\n", cache_property->num_sets);
    printf("Word offset:             %d bits\n", cache_property->word_bits);
    printf("Set offset:              %d bits\n", cache_property->set_bits);
    printf("Tag bit:                 %d bits\n", cache_property->tag_bits);
}

// define cache ways based on associativity
void define_cache_ways(CACHE *cache) {
    CACHE_WAY **cache_ways = (CACHE_WAY **) malloc(cache->cache_property->associativity * sizeof(CACHE_WAY *));
    cache->cache_ways = cache_ways;

    int i;
    for (i = 0; i < cache->cache_property->associativity; i++) {
        cache_ways[i] = (CACHE_WAY *) malloc(sizeof(CACHE_WAY));
        define_cache_way(cache_ways[i], cache->cache_property->num_sets);
    }
}

// define CACHE_LINEs for each CACHE_WAY = number of sets
void define_cache_way(CACHE_WAY *cache_way, unsigned int num_sets) {
    CACHE_LINE **cache_lines = (CACHE_LINE **) malloc(num_sets * sizeof(CACHE_LINE *));
    cache_way->cache_lines = cache_lines;

    int i;
    for (i = 0; i < num_sets; i++) {
        cache_lines[i] = (CACHE_LINE *) malloc(sizeof(CACHE_LINE));

        //initialize cache line attributes to 0
        cache_lines[i]->tag = 0;
        cache_lines[i]->valid_bit = 0;
    }
}

//initialize LRU matrix for all sets
void define_lru_matrices(CACHE *cache) {
    LRU_MATRIX **lru_matrices = (LRU_MATRIX **) malloc(cache->cache_property->num_sets * sizeof(LRU_MATRIX *));
    cache->lru_matrices = lru_matrices;

    int i, j, k;
    LRU_MATRIX * lru_matrix;

    for (i = 0; i < cache->cache_property->num_sets; i++) {
        lru_matrix = (LRU_MATRIX *) malloc(sizeof(LRU_MATRIX));
        cache->lru_matrices[i] = lru_matrix;

        lru_matrix->matrix = (unsigned int **) malloc(cache->cache_property->associativity * sizeof(unsigned int *));

        // initialize matrix
        for (j = 0; j < cache->cache_property->associativity; j++) {
            lru_matrix->matrix[j] = (unsigned int *) malloc(cache->cache_property->associativity * sizeof(unsigned int));
            for (k = 0; k < cache->cache_property->associativity; k++) {
                lru_matrix->matrix[j][k] = 0;
            }
        }
    }
}

// calculate log base 2
unsigned int logbase2(unsigned int x) {
    double lg;
    lg = log10(x) / log10(2);
    return (unsigned int) round(lg);
}


