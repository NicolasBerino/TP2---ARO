#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct object {
    int name;
    float weight;
    float value;
    float ratio;
} object;

void displayObject(object* object) {
    printf("%d\tpoid: %f\tvaleur: %f\tratio: %f\n",
        object->name, object->weight, object->value, object->ratio);
}

float supPole(object** objects, int start int end, float max_weight) {
    float consomated_weight = 0;
    float value = 0;
    for (int i = start; i < end; ++i) {
        if (consomated_weight + objects[i]->weight >= max_weight) { // je coupe
            value += (objects[i]->value * (max_weight - consomated_weight)) / objects[i]->weight;
            consomated_weight = max_weight;
            break;
        } else { // je prends
            value += objects[i]->value;
            consomated_weight += objects[i]->weight;
        }
    }
    return value;
}

int* branchAndBound(object** objects, int start, int end, int* max_sollution, int* stock, int weight) {

}

// solution = tableau d'objet
int main(int argc, char const *argv[]) {
    if (argc != 2) exit(1);

    // REMPLIR --------------------------------------
    FILE* f = fopen(argv[1], "r");
    int nb_object = 0;
    char c;
    do {
        c = fgetc(f);
        if (c == '\n') nb_object ++;
    } while (! feof(f));
    printf("nb_object : %d\n", nb_object);

    object** objects = (object**) malloc(nb_object * sizeof(object*));

    rewind(f);
    float weight, value;
    int max_weight, i = 0;
    fscanf(f, "%d", &max_weight);
    do {
        fscanf(f, "%f %f", &weight, &value);
        object* obj = (object*) malloc(sizeof(object));
        obj->name = i;
        obj->weight = weight;
        obj->value = value;
        obj->ratio = value / weight;
        objects[i] = obj;
        ++i;
    } while (! feof(f));
    fclose(f);
    // SOLVE --------------------------------------

    // max (sum(value * is_taken))
    // avec sum(weight * is_taken) <= max_weight

    // trier le tableau (tri bulle :| )
    int j;
    i = 0;
    while(i < nb_object - 1) {
        j = nb_object - 1;
        while(j > i) {
            if (objects[i]->ratio < objects[j]->ratio) {
                object* tmp = objects[i];
                objects[i] = objects[j];
                objects[j] = tmp;
            }
            --j;
        }
        i++;
    }

    for (i = 0; i < nb_object; ++i)
        displayObject(objects[i]);
    printf("\n");
    printf("\n");


    printf("Par glouton:\tconso:%f avec\n", supPole(objects, 0, nb_object, max_weight));




    printf("\n");
    printf("\n");
    // construction de l'arbre


    // FREE --------------------------------------
    for (i = 0; i < nb_object; ++i) free(objects[i]);
    free(objects);
    return 0;
}