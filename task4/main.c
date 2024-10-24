#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define N 100000 

double pi_value = 0.0;

// Функція для обчислення числа π
void* calculate_pi(void* arg) {
    double sum = 0.0;

    for (int i = 0; i < N; i++) {
        // Обчислення доданка
        double term = ((i % 2 == 0) ? 1.0 : -1.0) / (2.0 * i + 1.0);
        
        sum += term; // Додаємо доданок до суми
        pi_value = sum * 4.0; // Обчислюємо π

        // Виводимо номер ітерації та поточне значення π
        printf("Ітерація %d: Поточне значення π: %.10f\n", i + 1, pi_value);

        // Перевірка на скасування
        if (pthread_kill(pthread_self(), 0) != 0) {
            break; // Виходимо з циклу, якщо потік скасовано
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <delay_in_seconds>\n", argv[0]);
        return 1;
    }

    int delay = atoi(argv[1]);
    pthread_t thread_id;

    // Створюємо потік
    pthread_create(&thread_id, NULL, calculate_pi, NULL);

    // Очікуємо заданий час
    sleep(delay);

    // Скасовуємо потік
    pthread_cancel(thread_id);

    // Чекаємо на завершення потоку
    void* result;
    pthread_join(thread_id, &result);

    // Перевіряємо, чому потік завершився
    if (result == PTHREAD_CANCELED) {
        printf("Потік скасовано.\n");
    } else {
        printf("Потік завершився нормально, розрахована π: %.10f\n", pi_value);
    }

    return 0;
}