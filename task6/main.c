#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Функція обробки очищення
void cleanup_handler(void *arg) {
    fprintf(stderr, "Потік завершився з ітерацією: %d\n", *(int *)arg);
}

// Функція потоку-нащадка
void *child_thread(void *arg) {
    int iteration = 1;
    pthread_cleanup_push(cleanup_handler, &iteration); // створюємо обробник очищення

    while (1) {
        printf("Ітерація %d\n", iteration++);
        sleep(1);
    }

    pthread_cleanup_pop(1); //  обробник очищення
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Використання: %s <час_затримки>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int delay_time = atoi(argv[1]);
    if (delay_time <= 0) {
        fprintf(stderr, "Час затримки має бути додатнім числом.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t thread;
    // Створення потоку
    if (pthread_create(&thread, NULL, child_thread, NULL) != 0) {
        perror("Не вдалося створити потік");
        return 1;
    }

    sleep(delay_time);
    pthread_cancel(thread);

    pthread_join(thread, NULL);
    printf("Потік був скасований.\n");

    return 0;
}