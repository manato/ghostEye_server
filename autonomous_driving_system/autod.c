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


    /* if(argc != 5) { */
    /*     printf("/\*===============================================================*\/\n"); */
    /*     printf("USAGE : \n %s <shared input file path> <shared output file path> <shared height file path> <shared width file path>\n", argv[0]); */
    /*     printf("NOTE : parameter files must be readable and writable for all user\n"); */
    /*     printf("/\*===============================================================*\/\n"); */
    /*     printf("program exits...\n"); */
    /*     return; */
    /* }  */


    /*************************************************************************************/
    /* key generation */
    /*************************************************************************************/
    /* pathname_input = argv[1]; */
    /* printf("shared memory for input will create to %s\n", pathname_input); */
    /* shm_key_input = ftok(pathname_input, 1);  // key for input file */
    shm_key_input = ftok(INPUT_SHM_PATH, 1);  // key for input file
    if(shm_key_input == -1) {  // error semantics
        printf("generate key for input failed\n");
        return;
    }

    /* pathname_output = argv[2]; */
    /* printf("shared memory for output will create to %s\n", pathname_output); */
    /* shm_key_output = ftok(pathname_output, 1);  // key for output file */
    shm_key_output = ftok(OUTPUT_SHM_PATH, 1);  // key for output file
    if(shm_key_output == -1) {  // error semantics
        printf("generate key for output failed\n");
        return;
    }

    /* pathname_height = argv[3]; */
    /* printf("shared memory for height will create to %s\n", pathname_height); */
    /* shm_key_height = ftok(pathname_height, 1);  // key for height file */
    shm_key_height = ftok(HEIGHT_SHM_PATH, 1);  // key for height file
    if(shm_key_height == -1) {  // error semantics
        printf("generate key for height failed\n");
        return;
    }

    /* pathname_width = argv[4]; */
    /* printf("shared memory for width will create to %s\n", pathname_width); */
    /* shm_key_width = ftok(pathname_width, 1);  // key for width file */
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
    shrd_id_input = shmget(shm_key_input, IMAGE_SIZE, IPC_CREAT|0666);  // 0666 : readable and writable for all user
    if(shrd_id_input < 0) {  // error semantics
        printf("Can't Create Shared memory for input...\n");
        return;
    }

    shrd_id_output = shmget(shm_key_output, IMAGE_SIZE, IPC_CREAT|0666);  // 0666 : readable and writable for all user
    if(shrd_id_output < 0) {  // error semantics
        printf("Can't Create Shared memory for output...\n");
        return;
    }

    shrd_id_height = shmget(shm_key_height, sizeof(int), IPC_CREAT|0666);  // 0666 : readable and writable for all user
    if(shrd_id_height < 0) {  // error semantics
        printf("Can't Create Shared memory for height...\n");
        return;
    }

    shrd_id_width = shmget(shm_key_width, sizeof(int), IPC_CREAT|0666);  // 0666 : readable and writable for all user
    if(shrd_id_width < 0) {  // error semantics
        printf("Can't Create Shared memory for width...\n");
        return;
    }

    shrd_id_size = shmget(shm_key_size, sizeof(int), IPC_CREAT|0666);  // 0666 : readable and writable for all user
    if(shrd_id_size < 0) {  // error semantics
        printf("Can't Create Shared memory for size...\n");
        return;
    }


//    shrd_id_rbuf = shmget(shm_key_rbuf, MAX_OBJECT_NUM*sizeof(int*), IPC_CREAT|0666);  // 0666 : readable and writable for all user
    shrd_id_rbuf = shmget(shm_key_rbuf, MAX_OBJECT_NUM*sizeof(obj_coordinate), IPC_CREAT|0666);  // 0666 : readable and writable for all user
    if(shrd_id_rbuf < 0) {  // error semantics
        printf("Can't Create Shared memory for rbuf...\n");
        return;
    }

    shrd_id_rbuf_head = shmget(shm_key_rbuf_head, sizeof(int), IPC_CREAT|0666);  // 0666 : readable and writable for all user
    if(shrd_id_rbuf_head < 0) {  // error semantics
        printf("Can't Create Shared memory for rbuf_head...\n");
        return;
    }

    shrd_id_rbuf_tail = shmget(shm_key_rbuf_tail, sizeof(int), IPC_CREAT|0666);  // 0666 : readable and writable for all user
    if(shrd_id_rbuf_tail < 0) {  // error semantics
        printf("Can't Create Shared memory for rbuf_tail...\n");
        return;
    }



    /* create semaphore */
    semid = semget(sem_key, 1, IPC_CREAT|0666);
    if(semid == -1) {  // error semantics
        printf("Can't Create Semaphore...\n");
        return;
    }     

    /* initialized semaphore by 1 */
    ctl_arg.val = 1;
    if(semctl(semid, 0, SETVAL, ctl_arg) == -1) {
        printf("semctl error\n");
        return;
    }


    /* create shared reader-writer lock */
    shrd_id_rwlock = shmget(shm_key_rwlock, sizeof(pthread_rwlock_t), IPC_CREAT|0666);
    if(shrd_id_rwlock < 0) {  // error semantics
        printf("Can't Create Shared memory for reader-writer lock...\n");
        return;
    }



    /* create shared image update checker */
    shrd_id_imgupd = shmget(shm_key_imgupd, sizeof(char)*256, IPC_CREAT|0666);
    if(shrd_id_imgupd < 0) {  // error semantics
        printf("Can't Create Shared memory for image update checker...\n");
        return;
    }



    /*************************************************************************************/
    /* get address of shared memory */
    /*************************************************************************************/
    shrd_ptr_input = (unsigned char*)shmat(shrd_id_input, NULL, 0);
    shrd_ptr_output = (unsigned char*)shmat(shrd_id_output, NULL, 0);

    shrd_ptr_height = (int*)shmat(shrd_id_height, NULL, 0);
    shrd_ptr_width = (int*)shmat(shrd_id_width, NULL, 0);
    shrd_ptr_size = (int*)shmat(shrd_id_size, NULL, 0);
//    shrd_ptr_rbuf = (int**)shmat(shrd_id_rbuf, NULL, 0);
    shrd_ptr_rbuf = (obj_coordinate *)shmat(shrd_id_rbuf, NULL, 0);
    shrd_ptr_rbuf_head = (int*)shmat(shrd_id_rbuf_head, NULL, 0);
    shrd_ptr_rbuf_tail = (int*)shmat(shrd_id_rbuf_tail, NULL, 0);

    shrd_ptr_imgupd = (char*)shmat(shrd_id_imgupd, NULL, 0);


    /* zero clear shared memory */
    memset(shrd_ptr_input, 0, IMAGE_SIZE);
    memset(shrd_ptr_output, 0, IMAGE_SIZE);
    memset(shrd_ptr_height, 0, sizeof(int));
    memset(shrd_ptr_width, 0, sizeof(int));
    memset(shrd_ptr_size, 0, sizeof(int));
    memset(shrd_ptr_rbuf_head, 0, sizeof(int));
    memset(shrd_ptr_rbuf_tail, 0, sizeof(int));



    /* initialization of ring buffer */
    for(i=0; i<MAX_OBJECT_NUM; i++) {
        /* shrd_ptr_rbuf[i] = tmpptr; */
        /* tmpptr += CO_NUM; */
        shrd_ptr_rbuf[i].left = -1;
        shrd_ptr_rbuf[i].upper = -1;
        shrd_ptr_rbuf[i].right = -1;
        shrd_ptr_rbuf[i].bottom = -1;
        shrd_ptr_rbuf[i].obj_type = UNKNOWN_OBJECT;
    }


    /* initialization of reader-writer lock */
    shrd_ptr_rwlock = (pthread_rwlock_t *)shmat(shrd_id_rwlock, NULL, 0);

    pthread_rwlockattr_init(&lockattr);
    pthread_rwlockattr_setpshared(&lockattr, PTHREAD_PROCESS_SHARED);
//    if( (pthread_rwlock_init(shrd_ptr_rwlock, NULL)) != 0) {
    if( (pthread_rwlock_init(shrd_ptr_rwlock, &lockattr )) != 0) {
        printf("pthread_rwlock_init() failed\n");
        return;
    }
    pthread_rwlockattr_destroy(&lockattr);



    /* initialization of image update checker */
    memset(shrd_ptr_imgupd, 0, sizeof(char)*256);

    /*************************************************************************************/
    /* load dummy image */
    /*************************************************************************************/
    FILE *src_fp;
    long filesize=0;
    /* get file size */
#ifdef RELEASE
    if( (src_fp = fopen("/usr/local/geye/bin/shm_creater/lena.jpg", "rb")) == NULL)
#else
    if( (src_fp = fopen("./org_image/lena.jpg", "rb")) == NULL)
#endif  /* ifdef RELEASE */
    {
        printf("source file open error !!\n");
    }
    fseek( src_fp, 0, SEEK_END );
    filesize = ftell( src_fp );
    fseek( src_fp, 0, SEEK_SET );
    
#ifndef RELEASE
    printf("filesize %d bytes\n", (int)filesize);
#endif  /* ifndef RELEASE */
    
    /* write to shared memory */
    unsigned char *pointer_input = shrd_ptr_input;
    long read_size = 0;
    
    My_sem_operation(semid, LOCK); // lock semaphore
    /* load image to input memory */
    while( feof( src_fp ) == 0 )
      {
          read_size = fread( pointer_input, sizeof( unsigned char ), 1, src_fp );
          if( read_size < 1 )
          {
              break;
          }
          pointer_input += read_size;
      }
    My_sem_operation(semid, UNLOCK); // unlock semaphore
    
    *shrd_ptr_height = HEIGHT_OF_IMAGE;  // defined in data_for_shm.h
    *shrd_ptr_width = WIDTH_OF_IMAGE;
    *shrd_ptr_size = (int)filesize;



#ifndef RELEASE
    /*************************************************************************************/
    /* notification */
    /*************************************************************************************/
    printf("/*** shared memory created!! ***/\n");
    printf("Prease type \"Esc\" and \"Enter\" key to delete shared memory\n");
#endif  /* ifndef RELEASE */

    /*************************************************************************************/
    /* loop to keep shared memory */
    /*************************************************************************************/
    /* while((check_end_shm = getchar())) { */
    /*     if(check_end_shm == 0x1b) {  // if "Esc" key is typed, loop is breaked */
    /*         break; */
    /*     } */
        
    /* } */

    /*************************************************************************************/
    /* delete shared memory */
    /*************************************************************************************/
    /* shmctl(shrd_id_input, IPC_RMID, 0); */
    /* shmctl(shrd_id_output, IPC_RMID, 0); */
    /* shmctl(shrd_id_height, IPC_RMID, 0); */
    /* shmctl(shrd_id_width, IPC_RMID, 0); */
    /* shmctl(shrd_id_size, IPC_RMID, 0); */
    /* shmctl(shrd_id_rbuf, IPC_RMID, 0); */
    /* shmctl(shrd_id_rbuf_head, IPC_RMID, 0); */
    /* shmctl(shrd_id_rbuf_tail, IPC_RMID, 0); */

    /* delete semaphore */
    /* semctl(semid, 0, IPC_RMID, ctl_arg); */

    /* delete readre-writer lock */
    /* pthread_rwlock_destroy(shrd_ptr_rwlock); */
    /* shmctl(shrd_id_rwlock, IPC_RMID, 0); */

    /* printf("/\*** shared memory deleted!! ***\/\n"); */


}
