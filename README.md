Key Components:
1.	Student Threads: Each student tries to program and, at random intervals, seeks help from the TA.
2.	TA Thread: The TA waits (sleeps) until a student needs help, then assists them.
3.	Chairs: Limited to three, these chairs allow up to three students to wait for the TA. If all chairs are occupied, a student leaves and will try again later.
4.	Semaphores and Mutexes: These control the synchronization between students and the TA, ensuring that shared resources like waiting chairs are correctly managed.
Program Flow:
1.	TA Behavior (ta_function):
o	The TA waits until a student posts to the semaphore sem_ta, indicating that help is needed.
o	Once woken, the TA checks if there are waiting students. If so:
	The TA decrements the waiting_students count and increments the chairs_available count (a student is being helped, so a chair is freed).
	The TA then simulates helping a student by sleeping for a random time (1-3 seconds).

2.	Student Behavior (student_function):
o	Each student thread simulates programming for a random amount of time (1-5 seconds).
o	When a student seeks help, they attempt to sit in a chair by acquiring the mutex_waiting lock:
	If there are available chairs, the student sits (updates waiting_students and chairs_available), posts to sem_ta to notify the TA, and waits to be helped.
	If no chairs are available, the student unlocks the mutex and leaves, planning to try again later.

3.	Main Function:
o	Initializes the semaphore and mutex.
o	Creates one TA thread and five student threads.
o	Each thread runs indefinitely, simulating a continuous environment.
•	TA Output:
o	When woken by a student, the TA indicates they are "helping a student."
o	After helping, the TA displays how many chairs are now available.
•	Student Output:
o	Students indicate they are programming.
o	When seeking help, if chairs are available, they sit down and notify the TA.
o	If no chairs are available, students display a message that they will come back later.
Example Output Sequence:
1.	A student programs and then tries to sit down.
2.	If there is a free chair, they sit and wake up the TA.
3.	The TA helps the student, freeing up a chair once done.
4.	If all chairs are occupied, students who come later will leave and return later.

Infinite Loop: This program runs indefinitely. The pthread_join() statements in main() would only allow cleanup if the loops terminated.

