#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct object {
    int number;
    float weight;
    float value;
    float ratio;
} object;

typedef struct soluce {
    int* taken;
    float value;
} soluce;

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

void sortObjects(object** objects, int nb_object) {
    int j, i = 0;
    while(i < nb_object - 1) {
        j = nb_object - 1;
        while(j > i) {
            if (objects[i]->ratio < objects[j]->ratio) {
                object* tmp = objects[i];
                objects[i] = objects[j];
                objects[j] = tmp;
            }
            j--;
        }
        i++;
    }
}

void displayObject(object* object) {
    printf("%d\tpoid: %f\tvaleur: %f\tratio: %f\n",
        object->number, object->weight, object->value, object->ratio);
}

float supBound(object** objects, int start, int end, float current_value, float max_weight) {
    float consomated_weight = 0;
    for (int i = start; i < end; ++i) {
        if (consomated_weight + objects[i]->weight >= max_weight) { // je coupe
            current_value += (objects[i]->value * (max_weight - consomated_weight)) / objects[i]->weight;
            consomated_weight = max_weight;
            break;
        } else { // je prends
            current_value += objects[i]->value;
            consomated_weight += objects[i]->weight;
        }
    }
    return current_value;
}

//parcourt profondeur
// il faut, une liste d'element pris, un niveau, le poid libre restant, maintenir le meilleur coup
// pensé a un compteur de nœud
float branchAndBound(object** objects, int level, int nb_object, float weight, float current_value, int* nb_node) {
    if (level >= nb_object) return current_value;
    (*nb_node)++;
    float right = -1, left = -1, n_weight = weight - objects[level]->weight;
    if (n_weight >= 0)
        right = branchAndBound(objects, level + 1, nb_object, n_weight, current_value + objects[level]->value, nb_node);
    left = branchAndBound(objects, level + 1, nb_object, weight, current_value, nb_node);

    if (right > left) return right;
    return left;

}

// solution = tableau d'objet
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

    // TRIER (bulle :| ) --------------------------------------
    sortObjects(objects, nb_object);
    // for (int i = 0; i < nb_object; ++i) displayObject(objects[i]);

    // RESOUDRE --------------------------------------
    int nb_node = 0;
    float bnb = branchAndBound(objects, 0, nb_object, max_weight, 0,&nb_node);
    printf("optimum: %f node: %d\n", bnb, nb_node);

    // FREE --------------------------------------
    for (int i = 0; i < nb_object; ++i) free(objects[i]);
    free(objects);
    return 0;
}
