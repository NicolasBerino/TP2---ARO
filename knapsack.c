#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct object {
    int number;
    float weight;
    float value;
    float ratio;
} object;

int getNbObject(FILE* f) {
    int nb_object = 0;
    while (! feof(f)) if (fgetc(f) == '\n') nb_object++;
    return nb_object;
}

int getMaxWeight(FILE* f) {
    rewind(f);
    int max_weight;
    fscanf(f, "%d", &max_weight);
    return max_weight;
}

void fillObjects(FILE* f, object** objects) {
    float weight, value;
    int i = 0;
    do {
        fscanf(f, "%f %f", &weight, &value);
        object* obj = (object*) malloc(sizeof(object));
        obj->number = i;
        obj->weight = weight;
        obj->value = value;
        obj->ratio = value / weight;
        objects[i] = obj;
        ++i;
    } while (! feof(f));
}

void swap(object** objects, int i, int j) {
    object* tmp = objects[i];
    objects[i] = objects[j];
    objects[j] = tmp;
}

void pivot(object** objects, int left, int right, int *p) {
    int i;
    float pivot = objects[left]->ratio;
    *p = left;
    for (i = left + 1; i <= right; i++) {
        if (objects[i]->ratio > pivot) {
            (*p)++;
            if (i != *p) swap(objects, i, *p);
        }
    }
    swap(objects, *p, left);
}

void quickSort(object** objects, int left, int right) {
    int p;
    if (left < right) {
        pivot(objects, left, right, &p);
        quickSort(objects, left, p - 1);
        quickSort(objects, p + 1, right);
    }
}

void displayObject(object* object) {
    printf("%d\tpoid: %f\tvaleur: %f\tratio: %f\n",
        object->number, object->weight, object->value, object->ratio);
}

float bound(object** objects, int start, int end, float value, float max_weight) {
    float consomated_weight = 0;
    for (int i = start; i < end; ++i) {
        if (consomated_weight + objects[i]->weight >= max_weight) { // je coupe
            value += (objects[i]->value * (max_weight - consomated_weight) / objects[i]->weight);
            consomated_weight = max_weight;
            break;
        } else { // je prends
            value += objects[i]->value;
            consomated_weight += objects[i]->weight;
        }
    }
    return value;
}

float branchAndBound(object** objects, int level, int nb_object, float weight, float value, float *best_value, int *nb_node) {
    if (level >= nb_object) return value;
    (*nb_node)++;
    float left = -1, right = -1, n_weight = weight - objects[level]->weight;
    if (*best_value <= bound(objects, level, nb_object, value, weight)) {
        if (n_weight >= 0) {
            left = branchAndBound(objects, level + 1, nb_object, n_weight, value + objects[level]->value, best_value, nb_node);
            if (left > *best_value) *best_value = left;
        }
        right = branchAndBound(objects, level + 1, nb_object, weight, value, best_value, nb_node);
        if (right > *best_value) *best_value = right;
    }
    if (left > right) return left;
    return right;
}

float knapsack(object** objects, int nb_object, int max_weight) {
    int nb_node = 0;
    float best_value = 0;
    float bnb = branchAndBound(objects, 0, nb_object, max_weight, 0, &best_value, &nb_node);
    printf("optimum: %f\n", bnb);
    printf("nombre de nœud: %d\n", nb_node);
    return bnb;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("usage: %s chemin/du/sac\n", argv[0]);
        exit(1);
    }

    // REMPLIR --------------------------------------
    FILE* f = fopen(argv[1], "r");
    int nb_object = getNbObject(f);
    object** objects = (object**) malloc(nb_object * sizeof(object*));
    int max_weight = getMaxWeight(f);
    fillObjects(f, objects);
    fclose(f);
    printf("nb_object : %d\n", nb_object);

    // TRIER --------------------------------------
    quickSort(objects, 0, nb_object - 1);
    // for (int i = 0; i < nb_object; ++i) displayObject(objects[i]);

    // RESOUDRE --------------------------------------
    knapsack(objects, nb_object, max_weight);

    // FREE --------------------------------------
    for (int i = 0; i < nb_object; ++i) free(objects[i]);
    free(objects);
    return 0;
}
