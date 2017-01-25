#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct object {
    int name;
    float weight;
    float value;
    float ratio;
} object;

typedef struct soluce
{
    int* taken;
    float value;    
} soluce;

void displayObject(object* object) {
    printf("%d\tpoid: %f\tvaleur: %f\tratio: %f\n",
        object->name, object->weight, object->value, object->ratio);
}

float supPole(object** objects, int start, int end, float max_weight) {
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

soluce* branchAndBound(object** objects, int start, int end,soluce* max_solution, int* stock, float weight) {

    if(start>=end) return max_solution;


    int i;

    float value_stock = 0;

    soluce* solution_gauche = (soluce*)malloc(sizeof(soluce*));
    solution_gauche->taken= (int*)malloc(sizeof(int)*end);
    soluce* solution_droite = (soluce*)malloc(sizeof(soluce*));
    solution_droite->taken= (int*)malloc(sizeof(int)*end);
    for(i=0;i<end;i++){
        solution_gauche->taken[i]=0;
        solution_droite->taken[i]=0;
    }
    solution_gauche->value=0;
    solution_droite->value=0;

    for(i=0;i<end;i++){
        if(stock[i]==0){
            value_stock+=objects[i]->value;
        }
    }
    
    float t = supPole(objects,start,end,weight) + value_stock;

    if(t>max_solution->value){

           
    
        if(objects[start]->weight<weight){
            // printf("%f %f\n", objects[start]->weight,weight );
            stock[start]=0;
            solution_gauche=branchAndBound(objects,start+1,end,max_solution,stock,weight-objects[start]->weight);
        }

        for(i=0;i<end;i++){
            if(solution_gauche->taken[i]==1) solution_gauche->value+=objects[i]->value;
        }

        if(solution_gauche->value>max_solution->value){
            for(i=0;i<end;i++){
                max_solution->taken[i]=solution_gauche->taken[i];
            }
            max_solution->value=solution_gauche->value;
        }

        stock[start]=1;
        solution_droite=branchAndBound(objects,start+1,end,max_solution,stock,weight);

    }
    else{
        return max_solution;
    }

    for(i=0;i<end;i++){
            if(solution_droite->taken[i]==1) solution_droite->value+=objects[i]->value;
    }    

    if(solution_droite->value>max_solution->value){
        return solution_droite;
    }
    return max_solution;

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


    soluce* solution = (soluce*)malloc(sizeof(soluce*));
    solution->taken= (int*)malloc(sizeof(int)*nb_object);
    int* stock = (int*)malloc(sizeof(int)*nb_object);

    for(i=0;i<nb_object;i++){
        solution->taken[i]=0;
        stock[i]=1;
    }

    soluce* result = branchAndBound(objects,0, nb_object,solution,stock,max_weight);

    for(i=0;i<nb_object;i++){
        if(result->taken[i])displayObject(objects[i]);
    }

    printf("\n");
    printf("\n");
    // construction de l'arbre


    // FREE --------------------------------------
    for (i = 0; i < nb_object; ++i) free(objects[i]);
    free(objects);
    return 0;
}
