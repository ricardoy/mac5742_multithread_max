#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdlib.h>

using namespace std;

int max_value = 0;
int *v;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *worker1(void *pointer) {
    int *i = (int*) pointer;
    if (pthread_mutex_lock(&mutex) != 0) {
        cerr << "Error locking mutex." << endl;
    }

    if (v[*i] > max_value) {
        max_value = v[*i];
    }

    if (pthread_mutex_unlock(&mutex) != 0) {
        cerr << "Error unlocking mutex." << endl;
    }

    free(i);
    pthread_exit(NULL);
}

void *worker2(void *pointer) {
    int *i = (int*) pointer;
    
    if (v[*i] > max_value) {
        if (pthread_mutex_lock(&mutex) != 0) {
            cerr << "Error locking mutex." << endl;
        }

        if (v[*i] > max_value) {
            max_value = v[*i];
        }        

        if (pthread_mutex_unlock(&mutex) != 0) {
            cerr << "Error unlocking mutex." << endl;
        }
    }
    
    free(i);
    pthread_exit(NULL);
}

void *worker3(void *pointer) {
    int *i = (int*) pointer;
    
    if (v[*i] > max_value) {
        if (v[*i] > max_value) {
            if (pthread_mutex_lock(&mutex) != 0) {
                cerr << "Error locking mutex." << endl;
            }

            if (v[*i] > max_value) {
                max_value = v[*i];
            }        

            if (pthread_mutex_unlock(&mutex) != 0) {
                cerr << "Error unlocking mutex." << endl;
            }
        }
    }
    
    free(i);
    pthread_exit(NULL);
}


void initialize_vector(int n) {
    v = (int*) malloc(n * sizeof(int));
    if (v == NULL) {
        cerr << "Error allocating " << n << " integers." << endl;
        exit(1);
    }
    int max_value = -1;
    for (int i=0; i < n; i++) {
        v[i] = rand();
        if (v[i] > max_value) {
            max_value = v[i];
        }
    }
    cout << "valor máximo: " << max_value << "." << endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "$ max <vector size>" << endl;
        exit(1);
    }

    // srand(time(NULL));
    int n = atoi(argv[1]);    
    initialize_vector(n);
    pthread_t *workers = new pthread_t[n];

    for (int i = 0; i < n; i++) {
        int *x = (int*) malloc(sizeof(int));
        *x = i;
        int ret = pthread_create(&workers[i], NULL, worker3, (void*) x);
        if (ret != 0) {
            cerr << "Error creating thread." << endl;
        }
    }

    for (int i = 0; i < n; i++) {
        pthread_join(workers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    delete[] workers;
    free(v);

    cout << "Max: " << max_value << endl;
}