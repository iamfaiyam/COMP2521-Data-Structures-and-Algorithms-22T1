
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

int main(void) {
	Queue q = QueueNew();

    // enqueue 100000
    for (int i = 1; i <= 100000; i++) {
        QueueEnqueue(q, i);
    }
    
    // dequeue 1 to 50000, enqueue 100001 to 250000
    for (int i = 100001, j = 1; i <= 250000; i++, j++) {
        QueueEnqueue(q, i);
        QueueDequeue(q);
    }

	QueueFree(q);
}

