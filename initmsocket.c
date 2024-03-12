#include "msocket.h"
#include <pthread.h>
#include <sys/select.h>

// Shared Memory for two MTP sockets
Shared_Memo SM[MAX_MTP_SOCKETS];

// Mutex and Condition Variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data_available = PTHREAD_COND_INITIALIZER;

// Thread R Function
void *thread_R(void *arg)
{
    while (1)
    {
        // Implement R thread logic
        // Wait for data to be available in the receive buffer
        pthread_mutex_lock(&mutex);

        fd_set readfds, copy_readfds;
        FD_ZERO(&readfds);
        select(sockfd + 1, &readfds, NULL, NULL, NULL);


        // Process data from the receive buffer
        // Update rwnd, send ACK, etc.

        pthread_mutex_unlock(&mutex);
    }
}

// Thread S Function
void *thread_S(void *arg)
{
    while (1)
    {
        // Implement S thread logic
        // Check for pending messages in the sender-side message buffer
        // Send messages through UDP socket using sendto()

        // Sleep for a short period
        usleep(100000); // Sleep for 100 milliseconds
    }
}

int main()
{
    // Initialize MTP sockets, bind, etc.

    // Create threads R and S
    pthread_t tid_R, tid_S;
    pthread_create(&tid_R, NULL, thread_R, NULL);
    pthread_create(&tid_S, NULL, thread_S, NULL);

    // Initialize shared memory, bind sockets, etc.

    // Wait for threads to finish (which they won't)
    pthread_join(tid_R, NULL);
    pthread_join(tid_S, NULL);

    return 0;
}
