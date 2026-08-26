#ifndef CODEXION_H
#define CODEXION_H

#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

typedef struct args
{
    int				number_of_coders;          
    long long		time_to_burnout;           
    long long		time_to_compile;           
    long long		time_to_debug;             
    long long		time_to_refactor;          
    int				number_of_compiles_required;
    long long		dongle_cooldown;           
    char			*scheduler;  
}s_args;

typedef struct s_dongle
{
    pthread_mutex_t	mutex;          
    long long		last_released;  
}	s_dongle;

typedef struct s_coder
{
    int				id;               
    pthread_t		thread;            
    pthread_mutex_t	*left_dongle;      
    pthread_mutex_t	*right_dongle;      
    int				compiles_done;     
    long long		last_compile_start; 
    struct s_sim	*sim;               
}	s_coder;

typedef struct s_sim
{
    int				number_of_coders;          
    long long		time_to_burnout;           
    long long		time_to_compile;           
    long long		time_to_debug;             
    long long		time_to_refactor;          
    int				number_of_compiles_required;
    long long		dongle_cooldown;           
    char			*scheduler;              
    
    s_dongle		*dongles;                   
    s_coder			*coders;                  
    
    pthread_mutex_t	print_lock;                
    pthread_mutex_t	sim_lock; 
                     
    int				simulation_ended; 
    int				locks_ready;         
}	s_sim;

typedef struct s_task
{
    int coder_id;
    long long arrival_time;
    long long deadline;
}s_task;

s_args *parser(char **argv);
s_sim *fill_it(s_args *args);
void freedom(s_sim *sim, s_args *args);
void simulation(s_sim *data);
void compile(s_coder *coder);
void debuge(s_coder *coder);
void refactor(s_coder *coder);
long long get_time();

#endif