#include<stdio.h>
#include<pthread.h>

void * message_function(void *message) {
    printf("%s\n", ((char *) message));
}

int main() {
    pthread_t t1, t2;

    char *m1 = "Thread 1: PDC";
    char *m2 = "Thread 2: Lab";

    pthread_create(&t1, NULL, message_function, (void *) m1);
    pthread_create(&t2, NULL, message_function, (void *) m2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
