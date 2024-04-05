# OperatingSystems-Project1 CS33211
Classic Producer-Consumer Problem
My submission includes the table.h, producer.h, consumer.h, and an example.txt of the output in the terminal. I used semaphores to help solve the problem. Prodcuer.c contains the code for the producer process. It generates two random data items and places them into the shared memory buffer. Consumer.c contains the code for the consumer process. It consumer the data from the shared memory buffer. The process continues to run for a total of 10 iterations. The table.h contains the header file that defines the shared memory structure and constants. 

Use the following to compile and run the program.

gcc producer.c -pthread -lrt -o producer

gcc consumer.c -pthread -lrt -o consumer

./producer & ./consumer
