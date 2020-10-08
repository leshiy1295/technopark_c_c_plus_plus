#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_SEND_SIZE 80
#define PROCESSES_COUNT 10

struct mymsgbuf {
    long mtype;
    char mtext[MAX_SEND_SIZE];
};

void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text) {
    qbuf->mtype = type;
    strcpy(qbuf->mtext, text);
    if (msgsnd(qid, (struct msgbuf *)qbuf, strlen(qbuf->mtext) + 1, 0) == -1) {
        printf("Failed to send message\n");
    }
}

void read_message(int qid, struct mymsgbuf *qbuf, long type) {
    qbuf->mtype = type;
    if (msgrcv(qid, (struct msgbuf *)qbuf, MAX_SEND_SIZE, type, 0) < 0) {
        printf("Failed to read message from queue\n");
    } else {
        printf("Received %s\n", qbuf->mtext);
    }
}

int main() {
    int qtype = 1, pid[PROCESSES_COUNT], msgqid;
    struct mymsgbuf qbuf;
    
    key_t key = ftok("014_messages_queue.c", 'b');
    if (key == -1) {
        perror("Failed to create key\n");
        return 1;
    }
    printf("Created key %d\n", key);

    if ((msgqid = msgget(key, IPC_CREAT|0660)) == -1) {
        perror("Failed to create queue\n");
        return 1;
    }

    /*
    // можно ещё создать идентификатор очереди автоматически с помощью ОС
    if ((msgqid = msgget(IPC_PRIVATE, IPC_CREAT|0660)) == -1) {
        perror("Failed to create queue\n");
        return 1;
    }
    */

    for (int i = 1; i < PROCESSES_COUNT; ++i) {
        pid[i] = fork();
        if (pid[i] == -1) {
            printf("Failed to create process\n");
            continue;
        }

        if (pid[i] == 0) {
            char str[40];
            sprintf(str, "Hello world from %d", getpid());
            send_message(msgqid, &qbuf, qtype, str);
            exit(0);
        }
    }

    for (int stat, i = 1; i < PROCESSES_COUNT; ++i) {
        int status = waitpid(pid[i], &stat, 0);
        if (pid[i] == status) {
            printf("Child process %d exited\n", pid[i]);
        }
    }

    for (int i = 1; i < PROCESSES_COUNT; ++i) {
        read_message(msgqid, &qbuf, qtype);
    }

    if (msgctl(msgqid, IPC_RMID, NULL) < 0) {
        printf("Failed to remove queue\n");
    }
}
