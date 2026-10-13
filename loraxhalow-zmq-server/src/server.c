#include <zmq.h>
#include <stdio.h>
#include <signal.h>

volatile sig_atomic_t keep_running = 1;
void handle_signal(int sig) { (void)sig; keep_running = 0; }

int main(void)
{
    struct sigaction sa = {0};
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    void *context = zmq_ctx_new();
    void *pull = zmq_socket(context, ZMQ_PULL);
    if (zmq_bind(pull, "ipc:///tmp/loraxhalow.ipc") != 0) {
        perror("zmq_bind");
        return 1;
    }

    while (keep_running) {
        zmq_msg_t msg;
        zmq_msg_init(&msg);
        if (zmq_msg_recv(&msg, pull, 0) == -1) {   /* interrupted by signal or error */
            zmq_msg_close(&msg);
            continue;
        }
        printf("%.*s\n", (int)zmq_msg_size(&msg), (char *)zmq_msg_data(&msg));
        fflush(stdout);
        zmq_msg_close(&msg);
    }

    zmq_close(pull);
    zmq_ctx_destroy(context);
    return 0;
}