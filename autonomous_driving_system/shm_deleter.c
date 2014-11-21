/**********************************************/
/* daemon program to make posix shared memory */
/**********************************************/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

#include <sys/sem.h>

#include "data_for_shm.h"
#include "switch_release.h"

main(int argc, char *argv[]) {

    char *pathname_input, *pathname_output, *pathname_height, *pathname_width;
    key_t shm_key_input, shm_key_output, shm_key_height, shm_key_width, shm_key_size;
    key_t shm_key_rbuf, shm_key_rbuf_head, shm_key_rbuf_tail;
    int shrd_id_input, shrd_id_output, shrd_id_height, shrd_id_width, shrd_id_size;
    int shrd_id_rbuf, shrd_id_rbuf_head, shrd_id_rbuf_tail;
    unsigned char *shrd_ptr_input, *shrd_ptr_output;
    int *shrd_ptr_height, *shrd_ptr_width, *shrd_ptr_size;
    int *shrd_ptr_rbuf_head, *shrd_ptr_rbuf_tail;
    obj_coordinate *shrd_ptr_rbuf;
    int check_end_shm;

    /* for semaphore control */
    union semun ctl_arg;
    key_t sem_key;
    int semid;

    /* for reader-writer lock */
    key_t shm_key_rwlock;
    int shrd_id_rwlock;
    pthread_rwlock_t *shrd_ptr_rwlock;
    pthread_rwlockattr_t lockattr;

    /* for image update checker */
    key_t shm_key_imgupd;
    int shrd_id_imgupd;
    char *shrd_ptr_imgupd;


    int i=0;


    /*************************************************************************************/
    /* key generation */
    /*************************************************************************************/
    shm_key_input = ftok(INPUT_SHM_PATH, 1);  // key for input file
    if(shm_key_input == -1) {  // error semantics
        printf("generate key for input failed\n");
        return;
    }

    shm_key_output = ftok(OUTPUT_SHM_PATH, 1);  // key for output file
    if(shm_key_output == -1) {  // error semantics
        printf("generate key for output failed\n");
        return;
    }

    shm_key_height = ftok(HEIGHT_SHM_PATH, 1);  // key for height file
    if(shm_key_height == -1) {  // error semantics
        printf("generate key for height failed\n");
        return;
    }

    shm_key_width = ftok(WIDTH_SHM_PATH, 1);  // key for width file
    if(shm_key_width == -1) {  // error semantics
        printf("generate key for width failed\n");
        return;
    }


    shm_key_size = ftok(SIZE_SHM_PATH, 1);  // key for size file
    if(shm_key_size == -1) {  // error semantics
        printf("generate key for size failed\n");
        return;
    }


    shm_key_rbuf = ftok(RBUF_PATH, 1);  // key for rbuf file
    if(shm_key_rbuf == -1) {  // error semantics
        printf("generate key for rbuf failed\n");
        return;
    }

    shm_key_rbuf_head = ftok(RBUF_HEAD_PATH, 1);  // key for rbuf_head file
    if(shm_key_rbuf_head == -1) {  // error semantics
        printf("generate key for rbuf_head failed\n");
        return;
    }

    shm_key_rbuf_tail = ftok(RBUF_TAIL_PATH, 1);  // key for rbuf_tail file
    if(shm_key_rbuf_tail == -1) {  // error semantics
        printf("generate key for rbuf_tail failed\n");
        return;
    }





    /* key generation for semaphore */
    sem_key = ftok(SEM_PATH, 1);  // key for semaphore
    if(sem_key == -1) {  // error semantics         
        printf("generate key for semaphore failed\n");
        return;
    }


    /* key generation for reader-writer lock */
    shm_key_rwlock = ftok(RWLOCK_SHM_PATH, 1);  // key for rwlock
    if(shm_key_rwlock == -1) {  // error semantics
        printf("generation key for reader-writer lock failed\n");
        return;
    }


    /* key generation for image update checker */
    shm_key_imgupd = ftok(IMGUPD_SHM_PATH, 1); // key for image update checker
    if(shm_key_imgupd == -1) {  // error semantics
        printf("generation key for image update checker failed\n");
        return;
    }




    /*************************************************************************************/
    /* create shared memory */
    /*************************************************************************************/
    shrd_id_input = shmget(shm_key_input, IMAGE_SIZE, 0666);  // 0666 : readable and writable for all user
    if(shrd_id_input < 0) {  // error semantics
        printf("Can't Access Shared memory for input...\n");
        return;
    }

    shrd_id_output = shmget(shm_key_output, IMAGE_SIZE, 0666);  // 0666 : readable and writable for all user
    if(shrd_id_output < 0) {  // error semantics
        printf("Can't Access Shared memory for output...\n");
        return;
    }

    shrd_id_height = shmget(shm_key_height, sizeof(int), 0666);  // 0666 : readable and writable for all user
    if(shrd_id_height < 0) {  // error semantics
        printf("Can't Access Shared memory for height...\n");
        return;
    }

    shrd_id_width = shmget(shm_key_width, sizeof(int), 0666);  // 0666 : readable and writable for all user
    if(shrd_id_width < 0) {  // error semantics
        printf("Can't Access Shared memory for width...\n");
        return;
    }

    shrd_id_size = shmget(shm_key_size, sizeof(int), 0666);  // 0666 : readable and writable for all user
    if(shrd_id_size < 0) {  // error semantics
        printf("Can't Access Shared memory for size...\n");
        return;
    }


    shrd_id_rbuf = shmget(shm_key_rbuf, MAX_OBJECT_NUM*sizeof(obj_coordinate), 0666);  // 0666 : readable and writable for all user
    if(shrd_id_rbuf < 0) {  // error semantics
        printf("Can't Access Shared memory for rbuf...\n");
        return;
    }

    shrd_id_rbuf_head = shmget(shm_key_rbuf_head, sizeof(int), 0666);  // 0666 : readable and writable for all user
    if(shrd_id_rbuf_head < 0) {  // error semantics
        printf("Can't Access Shared memory for rbuf_head...\n");
        return;
    }

    shrd_id_rbuf_tail = shmget(shm_key_rbuf_tail, sizeof(int), 0666);  // 0666 : readable and writable for all user
    if(shrd_id_rbuf_tail < 0) {  // error semantics
        printf("Can't Access Shared memory for rbuf_tail...\n");
        return;
    }



    /* create semaphore */
    semid = semget(sem_key, 1, 0666);
    if(semid == -1) {  // error semantics
        printf("Can't Access Semaphore...\n");
        return;
    }     


    /* create shared reader-writer lock */
    shrd_id_rwlock = shmget(shm_key_rwlock, sizeof(pthread_rwlock_t), 0666);
    if(shrd_id_rwlock < 0) {  // error semantics
        printf("Can't Access Shared memory for reader-writer lock...\n");
        return;
    }


    /* create shared image update checker */
    shrd_id_imgupd = shmget(shm_key_imgupd, sizeof(char)*256, 0666);
    if(shrd_id_imgupd < 0) {  // error semantics
        printf("Can't Create Shared memory for image update checker...\n");
        return;
    }



    /*************************************************************************************/
    /* delete shared memory */
    /*************************************************************************************/
    shmctl(shrd_id_input, IPC_RMID, 0);
    shmctl(shrd_id_output, IPC_RMID, 0);
    shmctl(shrd_id_height, IPC_RMID, 0);
    shmctl(shrd_id_width, IPC_RMID, 0);
    shmctl(shrd_id_size, IPC_RMID, 0);
    shmctl(shrd_id_rbuf, IPC_RMID, 0);
    shmctl(shrd_id_rbuf_head, IPC_RMID, 0);
    shmctl(shrd_id_rbuf_tail, IPC_RMID, 0);

    /* delete semaphore */
    semctl(semid, 0, IPC_RMID, ctl_arg);

    /* delete readre-writer lock */
    pthread_rwlock_destroy(shrd_ptr_rwlock);
    shmctl(shrd_id_rwlock, IPC_RMID, 0);

    /* delete image update checker */
    shmctl(shrd_id_imgupd, IPC_RMID, 0);

    printf("/*** shared memory deleted!! ***/\n");


}
