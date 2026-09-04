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

typedef struct s_queue{
    int coder_id;
    long long priorrity;
    int size;
}s_queue;

typedef struct s_dongle
{
    pthread_mutex_t	mutex;
    pthread_cond_t  cond;      
    int is_free;   
    int in_cooldown; 
    s_queue nodes[2];
    long long		last_released;  
}	s_dongle;

typedef struct s_coder
{
    int				id;               
    pthread_t		thread;            
    s_dongle	*first_dongle;      
    s_dongle	*second_dongle;      
    int				compiles_done;     
    long long		last_compile_time; 
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
    int             all_ready;
    long long       start_time;
    int				locks_ready;         
}	s_sim;


s_args *parser(char **argv);
s_sim *fill_it(s_args *args);
void freedom(s_sim *sim, s_args *args);
void simulation(s_sim *data);
void compile(s_coder *coder);
void debuge(s_coder *coder);
void refactor(s_coder *coder);
long long get_time();
void *monitor_routine(void *arg);
void init_start_time(s_sim *data);
void fill_dongles(s_coder *coder,s_sim *sim);
void print_coder_status(s_coder *coder, char *status);
void register_in_queue(s_dongle *dongle, s_coder *coder);
void release_dongles(s_dongle *d1, s_dongle *d2);
void pop_from_queue(s_dongle *dongle);
#endif