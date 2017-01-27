#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct object {
    int number;
    float weight;
    float value;
    float ratio;
    int taken;
} object;

int getNbObject(FILE* f) {
    rewind(f);
    int nb_object = 0;
    char buf[1000];
    while(! feof(f)) {
        fgets(buf, 100, f);
        nb_object++;
    }
    return nb_object - 2;
}

int getMaxWeight(FILE* f) {
    rewind(f);
    int max_weight;
    fscanf(f, "%d", &max_weight);
    return max_weight;
}

void fillObjects(FILE* f, object* objects) {
    float weight, value;
    int i = 0;
    while(1 < fscanf(f, "%f %f", &weight, &value)) {
        object o;
        o.number = i;
        o.weight = weight;
        o.value = value;
        o.ratio = value / weight;
        o.taken = 0;
        objects[i] = o;
        ++i;
    }
}

void swap(object* objects, int i, int j) {
    object tmp = objects[i];
    objects[i] = objects[j];
    objects[j] = tmp;
}

void pivot(object* objects, int left, int right, int *p) {
    int i;
    float pivot = objects[left].ratio;
    *p = left;
    for (i = left + 1; i <= right; i++) {
        if (objects[i].ratio > pivot) {
            (*p)++;
            if (i != *p) swap(objects, i, *p);
        }
    }
    swap(objects, *p, left);
}

void quickSort(object* objects, int left, int right) {
    int p;
    if (left < right) {
        pivot(objects, left, right, &p);
        quickSort(objects, left, p - 1);
        quickSort(objects, p + 1, right);
    }
}

void displayObjects(object* objects, int nb_object, int displaytaken) {
    int total_o = 0;
    float total_w = 0, total_v = 0;
    printf("\nObjet    Poids   Valeur     Ratio\n");
    printf("---------------------------------\n");
    for (int i = 0; i < nb_object; ++i) {
        if (! displaytaken || objects[i].taken){
            printf("%5d  %7.2f  %7.2f  %7f  %d\n",
                   objects[i].number + 1,
                   objects[i].weight,
                   objects[i].value,
                   objects[i].ratio,
                   objects[i].taken);
            total_o++;
            total_w += objects[i].weight;
            total_v += objects[i].value;
        }
    }
    printf("       -------  -------          \n");
    printf("       %7.2f  %7.2f     Total:%5d objets\n", total_w, total_v, total_o);
}

float bound(object* objects, int start, int end, float value, float max_weight) {
    float consomated_weight = 0;
    for (int i = start; i < end; ++i) {
        if (consomated_weight + objects[i].weight >= max_weight) { // je coupe
            value += (objects[i].value * (max_weight - consomated_weight) / objects[i].weight);
            consomated_weight = max_weight;
            break;
        } else { // je prends
            value += objects[i].value;
            consomated_weight += objects[i].weight;
        }
    }
    return value;
}

float branchAndBound(object* objects, int level, int nb_object, float weight, float value, float *best_value, int *nb_node) {
    (*nb_node)++;
    if (level >= nb_object || weight <= 0 || *best_value > bound(objects, level, nb_object, value, weight)) return value;
    float n_weight = weight - objects[level].weight;
    if (n_weight >= 0) {
        float left = branchAndBound(objects, level + 1, nb_object, n_weight, value + objects[level].value, best_value, nb_node);
        if (left > *best_value) {
            objects[level].taken = 1;
            printf("PASSE ! ! !\n");
            *best_value = left;
        }
    }
    float right = branchAndBound(objects, level + 1, nb_object, weight, value, best_value, nb_node);
    if (right > *best_value) {
        *best_value = right;
        objects[level].taken = 0;
    }
    return *best_value;
}

void knapsack(object* objects, int nb_object, int max_weight) {
    int nb_node = 0;
    float best_value = 0;
    float bnb = branchAndBound(objects, 0, nb_object, max_weight, 0, &best_value, &nb_node);
    printf("\nSac à dos:\n");
    printf("Optimum : %.2f\n", bnb);
    printf("Nombre de nœud : %d\n", nb_node);
    printf("Avec les objets suivants : \n");
    displayObjects(objects, nb_object, 1);
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("usage: %s chemin/du/sac\n", argv[0]);
        exit(1);
    }

    // REMPLIR --------------------------------------
    FILE* f = fopen(argv[1], "r");
    int nb_object = getNbObject(f);
    object* objects = (object*) malloc(nb_object * sizeof(object));
    int max_weight = getMaxWeight(f);
    fillObjects(f, objects);
    fclose(f);
    printf("Nombre d'objets : %d\n", nb_object);
    displayObjects(objects, nb_object, 0);

    // TRIER --------------------------------------
    quickSort(objects, 0, nb_object - 1);

    // RESOUDRE --------------------------------------
    knapsack(objects, nb_object, max_weight);

    // FREE --------------------------------------
    free(objects);
    return 0;
}
