#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Функція потоку-нащадка
void *child_thread(void *arg) {
    int iteration = 1;
    while (1) {
        printf("Ітерація %d\n", iteration++);
        sleep(1);
    }
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
    pthread_create(&thread, NULL, child_thread, NULL);

    sleep(delay_time);
    // Спроба скасувати потік
    pthread_cancel(thread);

    // Чекаємо завершення роботи потоку
    pthread_join(thread, NULL);
    printf("Потік був скасований.\n");

    return 0;
}