#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

// Определение констант
const int Dimension = 2;    // Размерность точек (2D)
const int X = 0;            // Индекс X-координаты
const int Y = 1;            // Индекс Y-координаты
const double Inf = 2e9;     // Положительная бесконечность

// Глобальные переменные для данных кластеризации
double** cluster_centers;
int** cluster_points;
int** coordinats_of_points;
int* number_of_points_in_claster;
int clasters_number;
int points_number;

// Структура для передачи аргументов в функцию потока
typedef struct arguments
{
    int first_cluster;
    int last_cluster;
} arguments;

// Функция для инициализации матрицы целых чисел
int** matrix_initializer(int first_size, int second_size)
{
    int** matrix = (int**) malloc(first_size * sizeof(int*));
    
    for (int index = 0; index < first_size; ++index)
    {
        matrix[index] = (int*) calloc(second_size, sizeof(int));
    }
    
    return matrix;
}

// Функция для инициализации матрицы вещественных чисел
double** double_matrix_initializer(int first_size, int second_size)
{
    double** matrix = (double**) malloc(first_size * sizeof(double*));
    
    for (int index = 0; index < first_size; ++index)
    {
        matrix[index] = (double*) calloc(second_size, sizeof(double));
    }
    
    return matrix;
}

// Функция для копирования матрицы
double** matrix_copying(double** input_matrix, int first_size, int second_size)
{
    double** matrix_copy;
    matrix_copy = double_matrix_initializer(first_size, second_size);
    
    for (int line = 0; line < first_size; ++line)
    {
        for (int column = 0; column < second_size; ++column)
        {
            matrix_copy[line][column] = input_matrix[line][column];
        }
    }
    
    return matrix_copy;
}

// Функция для сравнения двух матриц
bool matrix_equality(double** first_matrix, double** second_matrix, int first_size, int second_size)
{
    for (int line = 0; line < first_size; ++line)
    {
        for (int column = 0; column < second_size; ++column)
        {
            if (first_matrix[line][column] != second_matrix[line][column])
            {
                return false;
            }
        }
    }
    
    return true;
}

// Функция для освобождения памяти, выделенной под матрицу целых чисел
void delete_int_matrix(int** matrix, int size)
{
    for (int index = 0; index < size; ++index)
    {
        free(matrix[index]);
    }
    
    free(matrix);
    matrix = NULL;
}

// Функция для освобождения памяти, выделенной под матрицу вещественных чисел
void delete_double_matrix(double** matrix, int size)
{
    for (int index = 0; index < size; ++index)
    {
        free(matrix[index]);
    }
    
    free(matrix);
    matrix = NULL;
}

// Функция потока для обновления центров кластеров
void* thread_update_cluster_centers(void* argument)
{
    arguments* thread_argument = (arguments*) argument;
    int start_cluster = thread_argument -> first_cluster;
    int last_cluster = thread_argument -> last_cluster;
    
    for (int claster = start_cluster; claster <= last_cluster; ++claster)
    {
        for (int coordinat = 0; coordinat < Dimension; ++coordinat)
        {
            double new_center = 0;
            int number_of_points = number_of_points_in_claster[claster];
            
            for (int point = 0; point < number_of_points; ++point)
            {
                int current_point = cluster_points[claster][point];
                new_center += coordinats_of_points[current_point][coordinat];
            }
            
            if (number_of_points != 0)
            {
                new_center = new_center / number_of_points;
            }
            
            cluster_centers[claster][coordinat] = new_center;
        }
    }
    
    return NULL;
}

// Функция для распределения точек по кластерам
void points_distribution()
{
    for (int cluster = 0; cluster < clasters_number; ++cluster)
    {
        number_of_points_in_claster[cluster] = 0;
    }
    
    for (int point = 0; point < points_number; ++point)
    {
        double minimal_distance = Inf;
        int temporary_claster = -1;
        
        for (int claster = 0; claster < clasters_number; ++claster)
        {
            double distance = 0; //Расстояние от точки до центра кластера
            
            distance += pow((coordinats_of_points[point][X] - cluster_centers[claster][X]), 2);
            distance += pow((coordinats_of_points[point][Y] - cluster_centers[claster][Y]), 2);
            
            distance = pow(distance, 0.5);
            
            if (distance < minimal_distance)
            {
                minimal_distance = distance;
                temporary_claster = claster;
            }
        }
        
        int number_of_points = number_of_points_in_claster[temporary_claster];
        cluster_points[temporary_claster][number_of_points] = point;
        ++number_of_points_in_claster[temporary_claster];
    }
}

// Функция для обновления центров кластеров с использованием нескольких потоков
void update_claster_centers(pthread_t* threads, int clusters_for_thread, int clusters_for_last_thread, int threads_number)
{
    arguments thread_argument;
    
    // Создание потоков для обновления центров кластеров
    for (int thread = 0; thread < threads_number - 1; ++thread)
    {
        int first_cluster = thread * clusters_for_thread;
        int last_cluster = first_cluster + clusters_for_thread - 1;
        
        thread_argument.first_cluster = first_cluster;
        thread_argument.last_cluster = last_cluster;
        
        if (pthread_create(&threads[thread], NULL, thread_update_cluster_centers, &thread_argument) != 0)
        {
            printf("Can't create thread\n");
            exit(-1);
        }
    }
    
    // Для последнего потока учитывается оставшееся количество кластеров
    thread_argument.first_cluster = clasters_number - clusters_for_last_thread;
    thread_argument.last_cluster = clasters_number - 1;
    pthread_create(&threads[threads_number - 1], NULL, thread_update_cluster_centers, &thread_argument);
    
    // Ожидание завершения всех потоков
    for (int thread = 0; thread < threads_number; ++thread)
    {
        if (pthread_join(threads[thread], NULL) != 0)
        {
            printf("Join error\n");
            exit(-1);
        }
    }
    
}

// Функция кластеризации
void clasterzation(int threads_number)
{
    // Инициализация центров кластеров начальными координатами точек
    for (int claster = 0; claster < clasters_number; ++claster)
    {
        cluster_centers[claster][X] = coordinats_of_points[claster][X];
        cluster_centers[claster][Y] = coordinats_of_points[claster][Y];
    }
    
    // Выделение памяти для массива потоков
    pthread_t* threads = (pthread_t*) calloc(threads_number, sizeof(pthread_t));
    
    if (threads == NULL)
    {
        printf("Can't allocate memory\n");
        exit(-1);
    }
    
    // Если количество потоков больше, чем количество кластеров, уменьшаем количество потоков
    if (threads_number > clasters_number)
    {
        threads_number = clasters_number;
    }
    
    // Вычисление числа кластеров для каждого потока
    int clusters_for_thread = clasters_number / threads_number;
    int clusters_for_last_thread = 0;
    clusters_for_last_thread = clasters_number - clusters_for_thread * (threads_number - 1);
    
    // Распределение точек по кластерам
    points_distribution();
    
    // Создание матрицы для хранения предыдущих центров кластеров
    double** previous_centers = matrix_copying(cluster_centers, clasters_number, Dimension);
    
    // Итерационный процесс обновления центров кластеров
    while(1)
    {
        // Обновление центров кластеров с использованием многопоточности
        update_claster_centers(threads, clusters_for_thread, clusters_for_last_thread, threads_number);
        points_distribution();
        
        // Проверка условия остановки: сравнение текущих и предыдущих центров кластеров
        if (matrix_equality(cluster_centers, previous_centers, clasters_number, Dimension))
        {
            break;
        }

        // Обновление предыдущих центров кластеров для следующей итерации
        previous_centers = matrix_copying(cluster_centers, clasters_number, Dimension);
    }
}

void print_result()
{
    for (int cluster = 0; cluster < clasters_number; ++cluster)
    {
        int points_number = number_of_points_in_claster[cluster];
        printf("%d cluster:\npoints:\n", cluster + 1);
        
        for (int point = 0; point < points_number; ++point)
        {
            int current_point = cluster_points[cluster][point];
            
            printf("(%d, %d);\n", coordinats_of_points[current_point][X], coordinats_of_points[current_point][Y]);
        }
        
        printf("----------------------------------\n");
    }
}

int main(int argc, char* argv[])
{
    // Проверка наличия аргумента командной строки (количество потоков)
    if (argc != 2)
    {
        printf("You have to enter Number of threads\n");
        printf("Example: ./a.out 2\n");
        exit(1);        
    }
    
    // Чтение количества потоков из аргумента командной строки
    int threads_number = atoi(argv[1]);
    
    printf("Enter the number of clasters\n");
    scanf("%d", &clasters_number);
    printf("Enter the number of points\n");
    scanf("%d", &points_number);
    
    // Выделение памяти для хранения координат точек
    coordinats_of_points = matrix_initializer(points_number, Dimension);
    
    for (int point_index = 0; point_index < points_number; ++point_index)
    {
       
        int coordinat;
        
        scanf("%d", &coordinat);
        coordinats_of_points[point_index][X] = coordinat;
        
        scanf("%d", &coordinat);
        coordinats_of_points[point_index][Y] = coordinat;
    }
    
    // Выделение памяти для хранения данных о кластерах
    number_of_points_in_claster = (int*) calloc(clasters_number, sizeof(int));
    cluster_centers = double_matrix_initializer(clasters_number, Dimension);
    cluster_points = matrix_initializer(clasters_number, points_number);
    
    // Засечение времени начала выполнения программы
    long double start, end;
    start = clock();
    
    // Запуск процесса кластеризации
    clasterzation(threads_number);
    
    end = clock();

    // Вывод времени выполнения (закомментировано, так как опционально)
    printf("Execution time %Lf ms\n", (end - start) / 1000.0);
    
    // Вывод результатов кластеризации
    print_result();
    
    // Освобождение выделенной памяти
    free(number_of_points_in_claster);
    delete_double_matrix(cluster_centers, clasters_number);
    delete_int_matrix(cluster_points,clasters_number);
    delete_int_matrix(coordinats_of_points, points_number);

    return 0;
}
