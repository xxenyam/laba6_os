#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_RANDOM_NUM 100 

// Структура для передачі даних у потоки
typedef struct {
    int thread_id;
} thread_data_t;

// Функція для генерації випадкових чисел
void* generate_random_numbers(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    int thread_id = data->thread_id;

    // Випадкове число у діапазоні [0, MAX_RANDOM_NUM]
    for (int i = 0; i < rand() % 10 + 1; i++) { 
        int random_number = rand() % MAX_RANDOM_NUM;
        printf("Потік %d: Випадкове число: %d\n", thread_id, random_number);
        sleep(1); 
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Використання: %s <кількість_потоків>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    srand(time(NULL));

    // Створення потоків
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i + 1;
        pthread_create(&threads[i], NULL, generate_random_numbers, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Всі потоки завершили свою роботу.\n");
    return 0;
}