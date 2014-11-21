/*************************************************/
/* program to display image in shared memory */
/*************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cv.h>
#include <highgui.h>

#include "data_for_shm.h"

using namespace cv;

main(){

  key_t shm_key_input, shm_key_output, shm_key_height, shm_key_width, shm_key_size;
  int shrd_id_input, shrd_id_output, shrd_id_height, shrd_id_width, shrd_id_size;
  unsigned char *shrd_ptr_input, *shrd_ptr_output;
  int  *shrd_ptr_height, *shrd_ptr_width, *shrd_ptr_size;
  
  /* for semaphore control */
  key_t sem_key;
  int semid;
  
  
  /* key generation */
  shm_key_input = ftok(INPUT_SHM_PATH, 1);  // key for shared memory
  if(shm_key_input == -1) {  // error semantics
    printf("key generation failed\n");
    return -1;
  }

  shm_key_output = ftok(OUTPUT_SHM_PATH, 1);  // key for shared memory
  if(shm_key_output == -1) {  // error semantics
    printf("key generation failed\n");
    return -1;
  }


  shm_key_height = ftok(HEIGHT_SHM_PATH, 1);  // key for shared memory
  if(shm_key_height == -1) {  // error semantics
    printf("key generation failed\n");
    return -1;
  }

  shm_key_width = ftok(WIDTH_SHM_PATH, 1);  // key for shared memory
  if(shm_key_width == -1) {  // error semantics
    printf("key generation failed\n");
    return -1;
  }

  shm_key_size = ftok(SIZE_SHM_PATH, 1);  // key for shared memory
  if(shm_key_size == -1) {  // error semantics
    printf("key generation failed\n");
    return -1;
  }


  /* key generation for semaphore */
  sem_key = ftok(SEM_PATH, 1);  // key for semaphore
  if(sem_key == -1) {  // error semantics
    printf("generation key for semaphore failed\n");
    return -1;
  }


  /* open shared memory */
  shrd_id_input = shmget(shm_key_input, IMAGE_SIZE, 0666);  // 0666 : readable and writable for all user
  if(shrd_id_input < 0) {  // error semantics
    printf("Can't Access to the Shared image memory...\n");
    return -1;
  }

  shrd_id_output = shmget(shm_key_output, IMAGE_SIZE, 0666);  // 0666 : readable and writable for all user
  if(shrd_id_output < 0) {  // error semantics
    printf("Can't Access to the Shared image memory...\n");
    return -1;
  }


  shrd_id_height = shmget(shm_key_height, sizeof(int), 0666);  // 0666 : readable and writable for all user
  if(shrd_id_height < 0) {  // error semantics
    printf("Can't Access to the Shared height memory...\n");
    return -1;
  }

  shrd_id_width = shmget(shm_key_width, sizeof(int), 0666);  // 0666 : readable and writable for all user
  if(shrd_id_width < 0) {  // error semantics
    printf("Can't Access to the Shared width memory...\n");
    return -1;
  }


  shrd_id_size = shmget(shm_key_size, sizeof(int), 0666);  // 0666 : readable and writable for all user
  if(shrd_id_size < 0) {  // error semantics
    printf("Can't Access to the Shared size memory...\n");
    return -1;
  }


  /* open semaphore */
  semid = semget(sem_key, 1, 0666);
  if(semid == -1) {
    printf("Can't Access to the semaphore\n");
    return -1;
  }

  /* get address of shared memory */
  shrd_ptr_input = (unsigned char*)shmat(shrd_id_input, NULL, 0);
  shrd_ptr_output = (unsigned char*)shmat(shrd_id_output, NULL, 0);
  shrd_ptr_height = (int*)shmat(shrd_id_height, NULL, 0);
  shrd_ptr_width = (int*)shmat(shrd_id_width, NULL, 0);
  shrd_ptr_size = (int*)shmat(shrd_id_size, NULL, 0);

  printf("image in the shared memory displaying.....\n");

  while(1){
      
    /* read image from buffer */
    
    CvMat *buf_input = cvCreateMat(1, *shrd_ptr_size, CV_8UC3);
    CvMat *buf_output = cvCreateMat(1, *shrd_ptr_size, CV_8UC3);
      
    My_sem_operation(semid, LOCK);  // lock semaphore
    memcpy(buf_input->data.ptr, shrd_ptr_input, *shrd_ptr_size);
    //    memcpy(buf_output->data.ptr, shrd_ptr_output, *shrd_ptr_size);
    My_sem_operation(semid, UNLOCK);  // unlock semaphore
      
    /* by using this function, difference of file format is absorbed */
    IplImage *iplimg_input = cvDecodeImage(buf_input, CV_LOAD_IMAGE_COLOR);
    ///    IplImage *iplimg_output = cvDecodeImage(buf_output, CV_LOAD_IMAGE_COLOR);
      
    /* output image */
    cvShowImage("display shared memory INPUT", iplimg_input);
    cvMoveWindow("display shared memory INPUT", 0, 0);
    // cvShowImage("display shared memory OUTPUT", iplimg_output);
    // cvMoveWindow("display shared memory OUTPUT", 640, 0);


#if 0
    printf("height: %d, width: %d\n", *shrd_ptr_height, *shrd_ptr_width);
    printf("file size %d bytes\n", *shrd_ptr_size);
#endif
 
    int in_key = cvWaitKey(10);
#if 0
    printf("key waiting in 1ms...\n");
#endif
    if(in_key == 1048603)  // if 'Esc' is typed
      break;
      
    /* release image */
    cvReleaseImage(&iplimg_input);
    //    cvReleaseImage(&iplimg_output);
      
  }
    

}
