#include "codexion.h"

int main(int argc, char **argv)
{
    s_args *args;
    s_sim *sim;

    if (argc != 9)
    { 
        printf("ERROR : number of argument should be 9");
        return (1);
    }
    args = parser(argv);
    if (!args)
    {
        printf("Parsing error argument should be a valid integer\n");
        return 1;
    }
    sim = fill_it(args);
    simulation(sim);

    return 0;
}
