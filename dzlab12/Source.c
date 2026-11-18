#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>

#define COUNT 3
#define FILENAME "routes.txt"

struct Busroute {
    int number;
    char startroad[30];
    char stoproad[30];
    double length;
    int stopcount;
};

double calculate_distance(double length, int stopcount);
void print_route(struct Busroute r, double distance);
void process_routes(struct Busroute routes[], int count, double threshold);
int writefile(const char* fname, struct Busroute* routes, int size, const char* mode);

int main() {
    struct Busroute routes[COUNT];
    double treshholdvalue;
    int i;
    int choice;

    setlocale(LC_ALL, "RUS");

    printf("Программа для учёта автобусных маршрутов\n\n");

    for (i = 0; i < COUNT; i++) {
        printf("Ввод данных для маршрута %d:\n", i + 1);
        printf("Введите номер маршрута: ");
        scanf("%d", &routes[i].number);

        printf("Введите начальную остановку (без пробелов): ");
        scanf("%29s", routes[i].startroad);

        printf("Введите конечную остановку (без пробелов): ");
        scanf("%29s", routes[i].stoproad);

        printf("Введите длину маршрута (км): ");
        scanf("%lf", &routes[i].length);

        printf("Введите количество остановок: ");
        scanf("%d", &routes[i].stopcount);
        printf("\n");
    }

    printf("Введите значение среднего расстояния в км: ");
    scanf("%lf", &treshholdvalue);
    printf("\n");

    process_routes(routes, COUNT, treshholdvalue);

    printf("\nВыберите режим работы с файлом:\n");
    printf("1 - Запись в новый файл (перезапись)\n");
    printf("2 - Дозапись в существующий файл\n");
    printf("Ваш выбор (1 или 2): ");

    if (scanf("%d", &choice) != 1) {
        printf("Ошибка ввода!\n");
        return 1;
    }

    if (choice == 1) {
        if (writefile(FILENAME, routes, COUNT, "w")) {
            printf("\nДанные успешно записаны в новый файл %s\n", FILENAME);
        }
        else {
            printf("\nНе удалось записать данные в файл %s\n", FILENAME);
            return 1;
        }
    }
    else if (choice == 2) {
        if (writefile(FILENAME, routes, COUNT, "a")) {
            printf("\nДанные успешно дописаны в файл %s\n", FILENAME);
        }
        else {
            printf("\nНе удалось дописать данные в файл %s\n", FILENAME);
            return 1;
        }
    }
    else {
        printf("Неверный выбор!\n");
        return 1;
    }

    return 0;
}

double calculate_distance(double length, int stopcount) {
    if (stopcount <= 1) {
        return 0.0;
    }

    return length / (double)(stopcount - 1);
}

void print_route(struct Busroute r, double distance) {
    printf("  Номер маршрута: %d\n", r.number);
    printf("  Начальная остановка: %s\n", r.startroad);
    printf("  Конечная остановка: %s\n", r.stoproad);
    printf("  Длина маршрута: %.2lf км\n", r.length);
    printf("  Количество остановок: %d\n", r.stopcount);
    printf("  Среднее расстояние между остановками: %.2lf км\n", distance);
}

void process_routes(struct Busroute routes[], int count, double threshold) {
    int i;
    int found = 0;
    double dist;

    printf("--- Все записи маршрутов ---\n");
    for (i = 0; i < count; i++) {
        dist = calculate_distance(routes[i].length, routes[i].stopcount);
        printf("\nМаршрут %d:\n", i + 1);
        print_route(routes[i], dist);
    }

    printf("\n--- Маршруты, где среднее расстояние меньше %.2lf км ---\n", threshold);
    for (i = 0; i < count; i++) {
        dist = calculate_distance(routes[i].length, routes[i].stopcount);

        if (dist < threshold && routes[i].stopcount > 1) {
            printf("\nМаршрут %d:\n", i + 1);
            print_route(routes[i], dist);
            found++;
        }
    }

    if (found == 0) {
        printf("Маршрутов, удовлетворяющих заданному условию, не найдено.\n");
    }
}

int writefile(const char* fname, struct Busroute* routes, int size, const char* mode) {
    FILE* out;
    int i;
    double dist;

    if ((out = fopen(fname, mode)) == NULL) {
        printf("Ошибка открытия файла для записи\n");
        return 0;
    }

    for (i = 0; i < size; i++) {
        dist = calculate_distance(routes[i].length, routes[i].stopcount);
        if (fprintf(out, "Номер:%d;\n Начало:%s;\n Конец:%s;\n Длина:%.2lf;\n Остановки:%d;\n Среднее расстояние:%.2lf\n\n",
            routes[i].number, routes[i].startroad, routes[i].stoproad,
            routes[i].length, routes[i].stopcount, dist) < 0) {
            printf("Ошибка записи в файл\n");
            fclose(out);
            return 0;
        }
    }

    fclose(out);
    return 1;
}