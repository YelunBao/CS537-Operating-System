#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include "mapreduce.h"

pthread_mutex_t mlock;
Mapper mymap;
Reducer myreduce;

struct kv {
    char* key;
    char* value;
};

struct kv_list {
    struct kv** elements;
    size_t num_elements;
    size_t size;
};

struct kv_list kvl;
size_t kvl_counter;
int *counter;
int *partition_count;
struct kv_list *bucket;

void init_kv_list(struct kv_list* kvl, size_t size) {
    kvl->elements = (struct kv**) malloc(size * sizeof(struct kv*));
    kvl->num_elements = 0;
    kvl->size = size;
}

void add_to_list(struct kv_list* kvl, struct kv* elt) {
    if (kvl->num_elements == kvl->size) {
	kvl->size *= 2;
	kvl->elements = realloc(kvl->elements, kvl->size * sizeof(struct kv*));
    }
    kvl->elements[kvl->num_elements++] = elt;
}

char* get_func(char* key, int partition_number) {
    if (counter[partition_number] == partition_count[partition_number]) {
	return NULL;
    }
    struct kv *curr_elt = bucket[partition_number].elements[counter[partition_number]];
    if (!strcmp(curr_elt->key, key)) {
	counter[partition_number]++;
	return curr_elt->value;
    }
    return NULL;
}

int cmp(const void* a, const void* b) {
    char* str1 = (*(struct kv **)a)->key;
    char* str2 = (*(struct kv **)b)->key;
    return strcmp(str1, str2);
}

void MR_Emit(char* key, char* value)
{
    struct kv *elt = (struct kv*) malloc(sizeof(struct kv));
    if (elt == NULL) {
	printf("Malloc error! %s\n", strerror(errno));
	exit(1);
    }
    elt->key = strdup(key);
    elt->value = strdup(value);
    pthread_mutex_lock(&mlock);
    add_to_list(&kvl, elt);
    pthread_mutex_unlock(&mlock);
    return;
}

void *Reduce_wrapper(void *arg) {
    //int counter = 0;
    int *partitionsize = ((void **) arg)[0];
    // int *indexpartition = ((void **) arg)[0];
    // printf("thread %d start\n", *indexpartition);
    struct kv_list *list = ((void **) arg)[1];
    int *partition_number = ((void **) arg)[2];
    // Reduce(char *key, Getter get_next, int partition_number)
    // for (int i=0; i<*partition_keys; i++) {
        // printf("*indexpartition: %d, *partitionsize: %d\n", *indexpartition, *partitionsize );
        // printf("list[%d]->key: %s", i, list->elements);
    while (counter[*partition_number] < *partitionsize) {
        // (*reduce)((kvl.elements[kvl_counter])->key, get_func, 0);
        myreduce(list->elements[counter[*partition_number]]->key, get_func, *partition_number);
        // gettercvs[*indexpartition] = NULL;
    }
    // printf("thread %d end\n", *indexpartition);
    return NULL;
}

void *Map_wrapper(void *arg) {
    mymap((char *) arg);
    return NULL;
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % num_partitions;
}

void MR_Run(int argc, char *argv[], Mapper map, int num_mappers, Reducer reduce, int num_reducers, Partitioner partition){    
    if (pthread_mutex_init(&mlock, NULL) != 0) {
        printf("mutex init failed\n");
        return;
    }
    init_kv_list(&kvl, 10);
    mymap = map;
    myreduce = reduce;

    // printf("Flag 1!\n");
    // map threads
    pthread_t mappers[num_mappers];
    for (int i=1; i<argc; i+=num_mappers) {
        // create map threads
        for (int j=0; j<num_mappers && i+j<argc; j++) {
            pthread_create(&mappers[j], NULL, Map_wrapper, argv[i+j]);
        }
        // pthread_join would wait p to be finished
        for (int j=0; j<num_mappers && i+j<argc; j++) {
            pthread_join(mappers[j], NULL);
        }
    }

    // printf("Flag 2!\n");
    qsort(kvl.elements, kvl.num_elements, sizeof(struct kv*), cmp);

    // printf("Flag 3!\n");
    // get the acutal size for each partition
    partition_count = malloc(sizeof(int)*num_reducers);
    bucket = malloc(sizeof(struct kv_list)*num_reducers);
    counter = malloc(sizeof(int)*num_reducers);
    for (int i=0; i<num_reducers; i++) partition_count[i] = 0; // initialize stp[i] to 0
    for (int i=0; i<kvl.num_elements; i++) {
        unsigned long index = partition(kvl.elements[i]->key, num_reducers);
        partition_count[index]++;
    }

    // struct kv_list bucket[num_reducers];
    // printf("Flag 4!\n");
    for (int i=0; i<num_reducers; i++) {
        init_kv_list(&bucket[i], 10);
        // unsigned long index = partition(kvl.elements[i]->key, num_reducers);
        // add_to_list(bucket[i], kvl.elements[i]);
    }

    // printf("Flag 5!\n");
    for (int i=0; i<kvl.num_elements; i++) {
        unsigned long index =  partition(kvl.elements[i]->key, num_reducers);
        // pt[index][stp[index]] = pairorder[i];
        // printf("i is %d\n", i);
        // printf("index is %ld\n", index);
        add_to_list(&bucket[index], kvl.elements[i]);
        // printf("iteration done!\n");
        // stp[index] ++;
    }
    // int partition_keys[num_reducers];
    // for (int i=0; i<num_reducers; i++) {
    //     partition_keys[i] = 0;
    //     for (int j=0; j<bucket[i].num_elements; j++){
    //         if (j == 0) partition_keys[i]++;
    //         else if (!strcmp(bucket[i].elements[j-1]->key, bucket[i].elements[j]->key)) {
    //             partition_keys[i]++;
    //         }
    //     }
    // }

    // printf("Flag 6!\n");
    // lock threads
    pthread_t reducers[num_reducers];
    // int subindex[num_reducers];
    // gettercvs = malloc(sizeof(value *)*num_reducers);
    void **args[num_reducers];
    int index_copy[num_reducers];

    // create reduce threads
    for (int i=0; i<num_reducers; i++) {
        args[i] = malloc(sizeof(void *)*3);
        // subindex[i] = i;
        // subarg[i][0] = &subindex[i];
        // subarg[i][1] = pt[i];
        // subarg[i][2] = &stp[i];
        // gettercvs[i] = NULL;
        index_copy[i] = i;
        args[i][0] = &bucket[i].num_elements;;
        args[i][1] = &bucket[i];
        args[i][2] = &index_copy[i];
        pthread_create(&reducers[i], NULL, Reduce_wrapper, args[i]);
    }

    // pthread_join would wait p to be finished
    for (int i=0; i<num_reducers; i++) {
        pthread_join(reducers[i], NULL);
        free(args[i]);
    }

    // kvl_counter = 0;
    // while (kvl_counter < kvl.num_elements) {
	// (*reduce)((kvl.elements[kvl_counter])->key, get_func, 0);
    // }
}
