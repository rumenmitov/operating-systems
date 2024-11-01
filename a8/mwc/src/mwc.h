#ifndef __MWC_H
#define __MWC_H

#include <stdint.h>
#include <stdbool.h>


/**
 * @brief Error pretty-printing.
 */
#define RED   "\033[31m"
#define RESET "\033[0m"

#define ERROR "[" RED "ERROR" RESET "]: "


/**
 * @brief Matches wc formatting.
 */
#define WC_FMT "%8lu%8lu%8lu %s\n"


/**
 * @brief Program configuration and initialization.
 */
typedef struct mwc_config 
{
    char **files_to_parse;
    uint64_t file_count;
    
    uint64_t total_bytes;
    uint64_t total_words;
    uint64_t total_lines;
} mwc_config;

#define MWC_INIT { .files_to_parse = 0,		\
	    .file_count     = 0,		\
	    .total_bytes    = 0,		\
	    .total_words    = 0,		\
	    .total_lines    = 0 }


/**
 * @brief File metadata that is relevant to program.
 */
typedef struct mwc_file
{
    uint64_t bytes;
    uint64_t words;
    uint64_t lines;

    bool last_ascii;
    
} mwc_file;

#define MWC_FILE_INIT { .bytes = 0, .words = 0, .lines = 0, .last_ascii = 0 }


/**
 * @brief Parses CLI arguments and generates configuration.
 */
void parse_args(int argc, char *argv[], mwc_config *config);


/**
 * @brief Runs the program.
 */
void mwc(mwc_config *config);


#endif // __MWC_H
