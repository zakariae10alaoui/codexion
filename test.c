// // // #include "src/codexion.h"
// // // #include <pthread.h>
// // // #include <stdio.h>
// // // #include <unistd.h>
// // // #include <string.h>

// // // void *hello(void *)
// // // {
// // //     printf("hhhhhh\n");
// // //     printf("tttttttt\n");

// // // }

// // // // int main()
// // // // {
// // // //     // pthread_t t1;
// // // //     // pthread_t t2;
// // // //     // pthread_create(&t1, NULL,&hello,NULL);
// // // //     // pthread_create(&t2, NULL,&hello,NULL);
// // // //     // pthread_join(t1,NULL);
// // // //     // pthread_join(t2,NULL);
// // // //     // printf("The new thread ID is: %lu\n and  %lu\n", (unsigned long) t1,t2);
// // // //     struct timeval tv;
// // // //     gettimeofday(&tv,NULL);

// // // //     return 0;
// // // // }

// // // #include <stdio.h>
// // // #include <sys/time.h>

// // // int main(void)
// // // {
// // //     struct timeval tv;

// // //     gettimeofday(&tv, NULL);

// // //     printf("Seconds: %ld\n", tv.tv_sec * 1000);
// // //     printf("Microseconds: %ld\n", tv.tv_usec);

// // //     return 0;
// // // }

// // #include <stdio.h>
// // int main()
// // {
// //     int nn = 2147483647;
// //     nn = nn + 1;
// //     printf("ttttt%d\n",nn);
// // }
// #include <stdlib.h>
// #include <stdio.h>
// #include <pthread.h>

// int mails = 0;
// pthread_mutex_t mutex;

// void* routine(void*) {
//     for (int i = 0; i < 10000000; i++) {
//         // pthread_mutex_lock(&mutex);
//         mails++;
//         pthread_mutex_unlock(&mutex);
//     }
// }

// int main(int argc, char* argv[]) {
//     pthread_t th[8];
//     int i;
//     pthread_mutex_init(&mutex, NULL);
//     for (i = 0; i < 8; i++) {
//         if (pthread_create(th + i, NULL, &routine, NULL) != 0) {
//             perror("Failed to create thread");
//             return 1;
//         }
//         printf("Thread %d has started\n", i);
//     }
//     for (i = 0; i < 8; i++) {
//         if (pthread_join(th[i], NULL) != 0) {
//             return 2;
//         }
//         printf("Thread %d has finished execution\n", i);
//     }
//     pthread_mutex_destroy(&mutex);
//     printf("Number of mails: %d\n", mails);
//     return 0;
// }
// deadlock
// conduit