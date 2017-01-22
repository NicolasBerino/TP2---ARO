#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct object {
    int name;
    float weight;
    float value;
    float ratio;
} object;

typedef struct binaryDecisionTree {
    object* object;
    int is_taken;
    struct binaryDecisionTree* left;
    struct binaryDecisionTree* right;
} binaryDecisionTree;

void displayObject(object* object) {
    printf("%d\tpoid: %f\tvaleur: %f\tratio: %f\n",
        object->name, object->weight, object->value, object->ratio);
}

void freeTree(binaryDecisionTree* root) {
    if (root->left != NULL) freeTree(root->left);
    if (root->right != NULL) freeTree(root->right);
    free(root);
}

binaryDecisionTree* sortedArrayToBdt(object** objects, int start, int end, int is_taken) { //////////////////////////// IL FAUT FAIRE DES TRUCS ICICICICICICI
    if (start >= end) return NULL;
    binaryDecisionTree* node = (binaryDecisionTree*) malloc (sizeof(binaryDecisionTree));
    node->object = objects[start];
    node->is_taken = is_taken;
    start++;
    node->left = sortedArrayToBdt(objects, start, end, 0);
    node->right = sortedArrayToBdt(objects, start, end, 1);
    return node;
}

void displayBdt(binaryDecisionTree* root, int indent) {
    if (root->left) displayBdt(root->left, indent + 1);
    for (int i = 0; i < indent; ++i) printf("  ");
    //displayObject(root->object);
    printf("%d\n", root->object->name);
    if (root->right) displayBdt(root->right, indent + 1);
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

    // trouver une borne supérieure par glouton
    int taken[nb_object];
    for (i = 0; i < nb_object; ++i) taken[i] = 0;
    float consomated_weight = 0;
    i = 0;
    while (consomated_weight < max_weight) {
        taken[i] = 1;
        consomated_weight += objects[i]->weight;
        ++i;
    }

    // version qui ne coupe pas d'objet
    // for (i = 0; i < nb_object; ++i) {
    //     if (objects[i]->weight + consomated_weight <= max_weight) {
    //         taken[i] = 1;
    //         consomated_weight += objects[i]->weight;
    //     } else taken[i] = 0;
    // }
    printf("Par glouton:\tconso:%f avec\n", consomated_weight);
    for (i = 0; i < nb_object; ++i)
        if (taken[i] == 1)
            displayObject(objects[i]);


    printf("\n");
    printf("\n");
    // construction de l'arbre
    binaryDecisionTree* tree = sortedArrayToBdt(objects, 0, nb_object - 1, -1);

    displayBdt(tree, 0);

    // FREE --------------------------------------
    for (i = 0; i < nb_object; ++i) free(objects[i]);
    free(objects);
    freeTree(tree);
    return 0;
}
