#include <stdlib.h>
#include "mwc.h"

int main(int argc, char *argv[])
{
    mwc_config config = MWC_INIT;
    parse_args(argc, argv, &config);
    
    mwc(&config);
    
    return EXIT_SUCCESS;
}
