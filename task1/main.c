#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int *array;               
int current_index = 0;    
pthread_mutex_t mutex;

typedef struct {
    int thread_num;
    int num_threads;
} thread_data;

void* thread_func(void* arg) {
    thread_data* data = (thread_data*)arg;
    int thread_num = data->thread_num;
    int num_threads = data->num_threads;

    sleep(thread_num);

    int random_value = rand() % 10 + 1;

    // Захист доступу до загального масиву
    pthread_mutex_lock(&mutex);

    // Додаємо згенероване число в масив, якщо є місце
    if (current_index < num_threads) {
        array[current_index] = random_value;
        current_index++;

        printf("Потік %d прокинувся і згенерував число: %d\n", thread_num, random_value);
        printf("Значення у масиві: ");
        for (int i = 0; i < num_threads; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
    }

    pthread_mutex_unlock(&mutex);

    free(data); 

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Використання: %s <кількість потоків>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    array = malloc(num_threads * sizeof(int));  // Динамічний масив для чисел від потоків
    if (array == NULL) {
        perror("Не вдалося виділити пам'ять");
        return 1;
    }

    pthread_t threads[num_threads];
    pthread_mutex_init(&mutex, NULL);
    srand(time(NULL));  

    // Створення потоків
    for (int i = 0; i < num_threads; i++) {
        thread_data *data = malloc(sizeof(thread_data));  // Виділяємо окрему пам'ять для кожного потоку
        data->thread_num = i;
        data->num_threads = num_threads;
        if (pthread_create(&threads[i], NULL, thread_func, data) != 0) {
            perror("Не вдалося створити потік");
            free(array);
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    free(array);  

    printf("Основна програма завершена.\n");
    return 0;
}