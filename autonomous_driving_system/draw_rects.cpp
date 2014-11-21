/**************************************************/
/* program to draw rectangles to the output image */
/**************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cv.h>
#include <highgui.h>

#include "data_for_shm.h"


main() {

  key_t shm_key_input, shm_key_output, shm_key_size;
  //  key_t shm_key_rbuf_dst, shm_key_rbuf, shm_key_rbuf_head, shm_key_rbuf_tail;
  key_t shm_key_rbuf, shm_key_rbuf_head, shm_key_rbuf_tail;
  int shrd_id_input, shrd_id_output, shrd_id_size;
  //  int shrd_id_rbuf_dst, shrd_id_rbuf, shrd_id_rbuf_head, shrd_id_rbuf_tail;
  int shrd_id_rbuf, shrd_id_rbuf_head, shrd_id_rbuf_tail;
  unsigned char *shrd_ptr_input, *shrd_ptr_output;
  //  int  *shrd_ptr_size, *shrd_ptr_rbuf_dst, *shrd_ptr_rbuf_head, *shrd_ptr_rbuf_tail;
  int  *shrd_ptr_size, *shrd_ptr_rbuf_head, *shrd_ptr_rbuf_tail;
  //  int **shrd_ptr_rbuf;
  obj_coordinate *shrd_ptr_rbuf;

    /* for semaphore control */
    key_t sem_key;
    int semid;


    /* key generation */
    shm_key_input = ftok(INPUT_SHM_PATH, 1);  // key for shared memory
    if(shm_key_input == -1) {                 // error semantics
        printf("key generation failed\n");
        return -1;
    }

    shm_key_output = ftok(OUTPUT_SHM_PATH, 1);  // key for shared memory
    if(shm_key_output == -1) {                  // error semantics
        printf("key generation failed\n");
        return -1;
    }

    shm_key_size = ftok(SIZE_SHM_PATH, 1);  // key for shared memory
    if(shm_key_size == -1) {                  // error semantics
        printf("key generation failed\n");
        return -1;
    }

    // shm_key_rbuf_dst = ftok(RBUF_DST_PATH, 1);  // key for shared memory
    // if(shm_key_rbuf_dst == -1) {                // error semantics
    //   printf("key generation failed\n");
    //   return -1;
    // }

    shm_key_rbuf = ftok(RBUF_PATH, 1);  // key for shared memory
    if(shm_key_rbuf == -1) {                // error semantics
      printf("key generation failed\n");
      return -1;
    }

    shm_key_rbuf_head = ftok(RBUF_HEAD_PATH, 1);  // key for shared memory
    if(shm_key_rbuf_head == -1) {                // error semantics
      printf("key generation failed\n");
      return -1;
    }

    shm_key_rbuf_tail = ftok(RBUF_TAIL_PATH, 1);  // key for shared memory
    if(shm_key_rbuf_tail == -1) {                // error semantics
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

    shrd_id_size = shmget(shm_key_size, sizeof(int), 0666);  // 0666 : readable and writable for all user
    if(shrd_id_size < 0) {  // error semantics
        printf("Can't Access to the Shared image memory...\n");
        return -1;
    }


    // shrd_id_rbuf_dst = shmget(shm_key_rbuf_dst, RBUF_ELEMENT_NUM*sizeof(int), 0666);
    // if(shrd_id_rbuf_dst < 0) {
    //     printf("Can't Access to the Shared image memory...\n");
    //     return -1;
    // }

    //    shrd_id_rbuf = shmget(shm_key_rbuf, MAX_OBJECT_NUM*sizeof(int), 0666);
    shrd_id_rbuf = shmget(shm_key_rbuf, MAX_OBJECT_NUM*sizeof(obj_coordinate), 0666);
    if(shrd_id_rbuf < 0) {
        printf("Can't Access to the Shared image memory...\n");
        return -1;
    }

    shrd_id_rbuf_head = shmget(shm_key_rbuf_head, sizeof(int), 0666);
    if(shrd_id_rbuf_head < 0) {
        printf("Can't Access to the Shared image memory...\n");
        return -1;
    }

    shrd_id_rbuf_tail = shmget(shm_key_rbuf_tail, sizeof(int), 0666);
    if(shrd_id_rbuf_tail < 0) {
        printf("Can't Access to the Shared image memory...\n");
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
    shrd_ptr_size = (int *)shmat(shrd_id_size, NULL, 0);
    //    shrd_ptr_rbuf_dst = (int *)shmat(shrd_id_rbuf_dst, NULL, 0);
    //    shrd_ptr_rbuf = (int **)shmat(shrd_id_rbuf, NULL, 0);
    shrd_ptr_rbuf = (obj_coordinate *)shmat(shrd_id_rbuf, NULL, 0);
    shrd_ptr_rbuf_head = (int *)shmat(shrd_id_rbuf_head, NULL, 0);
    shrd_ptr_rbuf_tail = (int *)shmat(shrd_id_rbuf_tail, NULL, 0);

    // int *tmpptr = shrd_ptr_rbuf_dst;
    // for(int i=0; i<MAX_OBJECT_NUM; i++) {
    //   shrd_ptr_rbuf[i] = tmpptr;
    //   tmpptr += CO_NUM;
    // }


    while(1) {
      
      My_sem_operation(semid, LOCK); // lock semaphorea
      int input_img_size = *shrd_ptr_size;
      My_sem_operation(semid, UNLOCK); // unlock semaphorea

      
      /* read input image from shared memory */      
      CvMat *buf = cvCreateMat(1, input_img_size, CV_8UC3);

      My_sem_operation(semid, LOCK); // lock semaphorea
      memcpy(buf->data.ptr, shrd_ptr_input, input_img_size);
      My_sem_operation(semid, UNLOCK); // unlock semaphorea


      /* absorb difference of file format */
      IplImage *iplimg = cvDecodeImage(buf, CV_LOAD_IMAGE_COLOR);


      /* define rectangle color*/
      CvScalar rects_color = CV_RGB(255, 0, 0); // red

      My_sem_operation(semid, LOCK); // lock semaphorea
      while( *shrd_ptr_rbuf_tail - *shrd_ptr_rbuf_head ) {    // read all coodinates from the ring buffer
        /* load coodinates of detected result */
        // int left = apGetCoordinate(shrd_ptr_rbuf, shrd_ptr_rbuf_head, shrd_ptr_rbuf_tail, LEFT);
        // int upper = apGetCoordinate(shrd_ptr_rbuf, shrd_ptr_rbuf_head, shrd_ptr_rbuf_tail, UPPER);
        // int right = apGetCoordinate(shrd_ptr_rbuf, shrd_ptr_rbuf_head, shrd_ptr_rbuf_tail, RIGHT);
        // int bottom = apGetCoordinate(shrd_ptr_rbuf, shrd_ptr_rbuf_head, shrd_ptr_rbuf_tail, BOTTOM);

        int left, upper, right, bottom, type;
        apGetCoordinate(shrd_ptr_rbuf,      // obj_coordinate *queue
                        shrd_ptr_rbuf_head, // int *head
                        shrd_ptr_rbuf_tail, // int *tail
                        &left,              // int *left
                        &upper,             // int *upper
                        &right,             // int *right
                        &bottom,            // int *bottom
                        &type               // int *type
                        );
                        

        CvPoint p1 = cvPoint(left, upper);
        CvPoint p2 = cvPoint(right, bottom);


        /* draw rectangles to the input image */
        cvRectangle(iplimg, p1, p2, rects_color, 3);
      }
#if 0
      /* write output image to the shared memory*/
      CvMat *buf_for_output = cvEncodeImage(".png", iplimg);
      memcpy(shrd_ptr_output, buf_for_output->data.ptr, (buf_for_output->rows)*(buf_for_output->cols)*sizeof(unsigned char));
      
      cvReleaseMat(&buf_for_output);
#endif  
      My_sem_operation(semid, UNLOCK); // unlock semaphorea


      cvShowImage("detection result", iplimg);
      cvMoveWindow("detection relult", 640, 0);

      cvWaitKey(10);


      cvReleaseImage(&iplimg);
      cvReleaseMat(&buf);
      


    }
    

}
