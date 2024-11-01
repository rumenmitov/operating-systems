#include <stdlib.h>

#include "xargs.h"
#include "async_xargs.h"

int main(int argc, char * argv[]) 
{
    config xargs_config;
    arena arena_config;
    
    xargs_init(argc, argv, &xargs_config);

    arena_config.workers = xargs_config.jobs;
    arena_config.tasks_done = 0;
    
    async_xarg_exec(
	&arena_config,
	&xargs_config);

    xargs_free(&xargs_config);
    
	
    return EXIT_SUCCESS;
}
