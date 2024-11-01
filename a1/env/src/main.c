#include "env.h"

int main(int argc, char *argv[])
{
    parse_opts(argc, argv);
    parse_args(argc, argv);
    env_print();
    return Sucess;
}
