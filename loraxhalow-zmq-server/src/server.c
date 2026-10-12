#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>

volatile sig_atomic_t keep_running = 1;

void handle_signal(int sig)
{
    keep_running = 0;
}

int main (void)
{
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "ipc://*:5000");
    assert (rc == 0);

    while (keep_running) {
        zmq_msg_t message;
        zmq_msg_init(&message);

        zmq_msg_recv(&message, &responder, 0);

        size_t msg_size = zmq_msg_size(&message);
        char *msg_data = zmq_msg_data(&message);

        char *buffer = (char *)malloc(msg_size + 1);
        if(buffer != NULL)
        {
            memcpy(buffer, msg_data, msg_size);
            buffer[msg_size] = '\0';

            for (int i = 0; i < msg_size; i++)
                printf(buffer[i]);

            free(buffer);
        }
        
        //callbacks here
        //zmq_send (responder, "World", 5, 0);
    }
    return 0;
}
