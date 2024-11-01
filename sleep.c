#include <stdio.h>          // For printf
#include <stdlib.h>         // For srand, rand, and NULL
#include <unistd.h>         // For sleep
#include <pthread.h>        // For pthreads (thread creation, mutexes)
#include <semaphore.h>      // For semaphores
#include <time.h>           // For time (to seed rand)

// Define the number of student threads
#define NUM_STUDENTS 5

// Global variables to keep track of waiting students and available chairs
int waiting_students = 0;  // Number of students waiting for the TA
int chairs_available = 3;  // Total chairs available outside TA's office

// Semaphore and mutex declarations
sem_t sem_ta;              // Semaphore used to wake up the TA
pthread_mutex_t mutex_waiting; // Mutex to control access to shared variables

// TA's behavior function
void *ta_function(void *arg) {
    while (1) { // Run indefinitely
        // Wait until a student wakes up the TA by posting to the semaphore
        sem_wait(&sem_ta);

        // Lock the mutex to safely modify shared variables
        pthread_mutex_lock(&mutex_waiting);
        if (waiting_students > 0) { // If there are students waiting
            waiting_students--;  // TA is now helping a student, so reduce waiting count
            chairs_available++;  // A chair is freed up for other students
            printf("TA finished helping a student. Chairs available: %d\n", chairs_available);
        }
        // Unlock the mutex after updating shared variables
        pthread_mutex_unlock(&mutex_waiting);

        // Simulate the TA helping a student by sleeping for a random amount of time
        printf("TA is helping a student.\n");
        sleep(rand() % 3 + 1); // Random time (1 to 3 seconds)
    }
}

// Student's behavior function
void *student_function(void *arg) {
    while (1) { // Run indefinitely
        // Simulate student programming before seeking help
        printf("Student is programming.\n");
        sleep(rand() % 5 + 1); // Random programming time (1 to 5 seconds)

        // Try to sit in a chair to wait for the TA
        pthread_mutex_lock(&mutex_waiting);
        if (waiting_students < chairs_available) { // If there are chairs available
            waiting_students++;  // Increment the number of waiting students
            chairs_available--;  // Occupy a chair
            printf("Student is sitting in a chair. Chairs available now: %d\n", chairs_available);

            // Unlock the mutex after modifying shared variables
            pthread_mutex_unlock(&mutex_waiting);

            // Notify the TA that a student needs help
            printf("Student is waking up the TA.\n");
            sem_post(&sem_ta);

            // Simulate time spent receiving help from the TA
            sleep(rand() % 3 + 1); // Random time (1 to 3 seconds)
        } else { // If no chairs are available
            // Unlock the mutex since no updates were made
            pthread_mutex_unlock(&mutex_waiting);
            printf("No chairs available, student will come back later.\n");
        }
    }
}

int main() {
    // Initialize random seed for sleep duration randomness
    srand(time(NULL));

    // Declare threads for the TA and students
    pthread_t ta;
    pthread_t students[NUM_STUDENTS];

    // Initialize the semaphore with an initial value of 0 (TA starts asleep)
    sem_init(&sem_ta, 0, 0);

    // Initialize the mutex for managing access to shared variables
    pthread_mutex_init(&mutex_waiting, NULL);

    // Create the TA thread
    pthread_create(&ta, NULL, ta_function, NULL);

    // Create student threads
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_create(&students[i], NULL, student_function, NULL);
    }

    // Join threads to wait for their completion (though they run indefinitely here)
    pthread_join(ta, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }

    // Clean up resources after program ends (though unreachable here due to infinite loop)
    sem_destroy(&sem_ta);
    pthread_mutex_destroy(&mutex_waiting);

    return 0;
}