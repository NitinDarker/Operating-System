#include <stdio.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

void produce_item(int item) {
    if (count < BUFFER_SIZE) {
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("Producer added item %d. Buffer count: %d\n", item, count);
    } else {
        printf("Producer: Buffer is full, cannot add item %d.\n", item);
    }
}

int consume_item() {
    int item = -1; 

    if (count > 0) {
        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("Consumer took item %d. Buffer count: %d\n", item, count);
    } else {
        printf("Consumer: Buffer is empty, no item to take.\n");
    }
    return item;
}

int main() {
    printf("Starting single-threaded producer-consumer simulation.\n");

    produce_item(10);
    produce_item(20);
    consume_item();
    produce_item(30);
    produce_item(40);
    produce_item(50);
    produce_item(60); // This one will find the buffer full
    consume_item();
    consume_item();
    consume_item();
    consume_item();
    consume_item(); // This one will find the buffer empty

    return 0;
}
