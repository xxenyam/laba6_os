#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Функція потоку-нащадка
void *child_thread(void *arg) {
    int delay_time = *(int *)arg;
    int iterations = delay_time * 2; 

    // Встановлюємо заборону на скасування потоку
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    for (int i = 1; i <= iterations; i++) {
        printf("Ітерація %d\n", i);
        sleep(1); 
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Використання: %s <час_затримки>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int delay_time = atoi(argv[1]);

    pthread_t thread;

    // Створення потоку
    pthread_create(&thread, NULL, child_thread, &delay_time);

    // Основний потік засинає на заданий час
    sleep(delay_time);

    // Спроба скасувати потік (не матиме ефекту)
    pthread_cancel(thread);

    // Чекаємо завершення роботи потоку
    pthread_join(thread, NULL);

    printf("Потік завершився.\n");

    return 0;
}