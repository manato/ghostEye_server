#include <stdio.h>
#include <math.h>
#include "for_use_GPU.h"
#include "cutil.h"
#include "drvapi_error_string.h"
#include <cuda_runtime_api.h>
#include "switch_release.h"

#define MAX_CPU_THREAD 2

/*** for debug(windows) ***//*
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>*/

/*** for debug(Linux) ***/
#include <unistd.h>

/* declaration of global variables */


//extern CUdevice dev;
#define conv(arg) getCudaDrvErrorString(arg)
CUdevice *dev;
CUcontext *ctx;
//CUdevice dev, dev2;
//CUcontext ctx, ctx2;
CUfunction *func_process_root, *func_process_part, *func_dt1d_x, *func_dt1d_y, *func_calc_a_score, *func_inverse_Q, *func_calc_feature;
CUmodule *module;
int *NR_MAXTHREADS_X, *NR_MAXTHREADS_Y;
// �z�X�g������
int data[MAX_CPU_THREAD];

/*** for debug(windows) ***//*
#define _MAX_PATH 256
#define _MAX_DIR 256
#define _MAX_DRIVE 8 */
                  
//TCHAR szAppDir[_MAX_PATH];  // �A�v���P�[�V�������N�����ꂽ�f�B���N�g��
//TCHAR szFull[_MAX_PATH];    // �N�����ꂽ�A�v���P�[�V�����̃t���p�X��
//TCHAR szDrive[_MAX_DRIVE];  // �N�����ꂽ�A�v���P�[�V�����̃h���C�u��
//TCHAR szDir[_MAX_DIR];      // �N�����ꂽ�A�v���P�[�V�����̃f�B���N�g����
                  

/*****************************************************************/
/* init_cuda

   initialization device to use CUDA function 
*/
/*****************************************************************/
void init_cuda(void)
{


    CUresult res;
    //const char file_name[43] = "./gccDebug/GPU_function.cubin";
#ifdef RELEASE
    const char file_name[256] = "/usr/local/geye/bin/car_detecter/GPU_function.cubin";
#else
    const char file_name[43] = "./gccRelease/GPU_function.cubin";
#endif  // ifdef RELEASE
    int i;
    /* initnialize GPU */
    res = cuInit(0);
    if(res != CUDA_SUCCESS){
      printf("\ncuInit failed: res = %s\n", conv(res));
      exit(1);
    }

  /* count the number of usable GPU */
    res = cuDeviceGetCount(&device_num);
    if(res != CUDA_SUCCESS) {
      printf("cuDeviceGetCount() failed: res = %s\n", conv(res));
      exit(1);
    }

    device_num = 2;

#ifdef PRINT_INFO
    printf("%d GPUs found\n", device_num);
#endif  // ifdef PRINT_INFO


  /* get device */
    dev = (CUdevice*)malloc(device_num*sizeof(CUdevice));
    for(int i=0; i<device_num; i++) {
    //    res = cuDeviceGet(&dev[i], 0);
      res = cuDeviceGet(&dev[i], i);
      if(res != CUDA_SUCCESS) {
      printf("cuDeviceGet(dev[%d]) failed: res = %s\n", i, conv(res));
      exit(1);
    }
  }

#if 0
  /* check whether peer-to-peer access between GPUs is possible */
  int canAccessPeer=0;
  cudaDeviceCanAccessPeer(&canAccessPeer, dev[0], dev[1]);
  if(canAccessPeer ==1 )
    printf("p2p access dev[0] -> dev[1] is ENable\n");
  else
    printf("p2p access dev[0] -> dev[1] is DISable\n"); 

  cudaDeviceCanAccessPeer(&canAccessPeer, dev[1], dev[0]);
  if(canAccessPeer ==1 )
    printf("p2p access dev[1] -> dev[0] is ENable\n");
  else
    printf("p2p access dev[1] -> dev[0] is DISable\n"); 
#endif


  ctx = (CUcontext*)malloc(device_num*sizeof(CUcontext));

  module = (CUmodule*)malloc(device_num*sizeof(CUmodule));

  func_process_root = (CUfunction*)malloc(device_num*sizeof(CUfunction));
  func_process_part = (CUfunction*)malloc(device_num*sizeof(CUfunction));
  func_dt1d_x = (CUfunction*)malloc(device_num*sizeof(CUfunction));
  func_dt1d_y = (CUfunction*)malloc(device_num*sizeof(CUfunction));
  func_calc_a_score = (CUfunction*)malloc(device_num*sizeof(CUfunction));
  func_inverse_Q = (CUfunction*)malloc(device_num*sizeof(CUfunction));
  func_calc_feature = (CUfunction*)malloc(device_num*sizeof(CUfunction));



  for(int i=0; i<device_num; i++) {

    res = cuCtxCreate(&ctx[i], 0, dev[i]);
    if(res != CUDA_SUCCESS) {
      printf("cuCtxCreate(ctx[%d]) failed: res = %s\n", i, conv(res));
      exit(1);
    }
  }


  for(int i=0; i<device_num; i++) {

    res = cuCtxSetCurrent(ctx[i]);
    if(res != CUDA_SUCCESS) {
       printf("cuCtxSetCurrent(ctx[%d]) failed: res = %s\n", i, conv(res));
       exit(1);
     }


    /* load .cubin file */
    res = cuModuleLoad(&module[i], file_name);
    if(res != CUDA_SUCCESS){
      printf("\ncuModuleLoad failed: res = %s\n", conv(res));
      /*** for debug(windows) ***//*  
                         // �N�����ꂽ�A�v���P�[�V�����̃t���p�X�����擾
                         ::GetModuleFileName(NULL, szFull, sizeof(szFull) / sizeof(TCHAR));
                         
                         // �t���p�X�����h���C�u����f�B���N�g���������ɕ���
                         _tsplitpath(szFull, szDrive, szDir, NULL, NULL);
                         
                         // �h���C�u���ƃf�B���N�g����������A��
                         _tmakepath(szAppDir, szDrive, szDir, NULL, NULL);
                         
                         MessageBox(NULL, szAppDir, (LPCWSTR)" ", MB_OK);*/
      /*** for debug(Linux) ***//*
           char pathname[512]="";
           // get current directory name
           getcwd(pathname, 512);
           // display current directory
           printf("current directory : %s\n", pathname);*/
      exit(1);
    }

    res = cuModuleGetFunction(&func_process_root[i], module[i], "process_root");
    if(res != CUDA_SUCCESS){
      printf("\ncuGetFunction(process_root) failed: res = %s\n", conv(res));
      exit(1);
    }

    res = cuModuleGetFunction(&func_process_part[i], module[i], "process_part");
    if(res != CUDA_SUCCESS){
      printf("\ncuGetFunction(process_part) failed: res = %s\n", conv(res));
      exit(1);
    }

    res = cuModuleGetFunction(&func_inverse_Q[i], module[i], "inverse_Q");
    if(res != CUDA_SUCCESS){
      printf("\ncuGetFunction(inverse_Q) failed: res = %s\n", conv(res));
      exit(1);
    }

    res = cuModuleGetFunction(&func_dt1d_x[i], module[i], "dt1d_x");
    if(res != CUDA_SUCCESS){
      printf("\ncuGetFunction(dt1d_x) failed: res = %s\n", conv(res));
      exit(1);
    }

    res = cuModuleGetFunction(&func_dt1d_y[i], module[i], "dt1d_y");
    if(res != CUDA_SUCCESS){
      printf("\ncuGetFunction(dt1d_y) failed: res = %s\n", conv(res));
      exit(1);
    }

    res = cuModuleGetFunction(&func_calc_a_score[i], module[i], "calc_a_score");
    if(res != CUDA_SUCCESS){
      printf("\ncuGetFunction(calc_a_score) failed: res = %s\n", conv(res));
      exit(1);
    }

    res = cuModuleGetFunction(&func_calc_feature[i], module[i], "calc_feature");
    if(res != CUDA_SUCCESS){
      printf("\ncuGetFunction(calc_feature) failed: res = %s\n", conv(res));
      exit(1);
    }

  }


  NR_MAXTHREADS_X = (int*)malloc(device_num*sizeof(int));
  NR_MAXTHREADS_Y = (int*)malloc(device_num*sizeof(int));


  for(int i=0; i<device_num; i++) {
    
    /* get max thread num per block */
    int max_threads_num = 0;
    res = cuDeviceGetAttribute(&max_threads_num, CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_BLOCK, dev[i]);
    if(res != CUDA_SUCCESS){
      printf("\ncuDeviceGetAttribute() failed: res = %s\n", conv(res));
      exit(1);
    }
    
    NR_MAXTHREADS_X[i] = (int)sqrt((double)max_threads_num);
    NR_MAXTHREADS_Y[i] = (int)sqrt((double)max_threads_num);

  }

    res = cuCtxSetCurrent(ctx[0]);
    if(res != CUDA_SUCCESS) {
       printf("cuCtxSetCurrent(ctx[%d]) failed: res = %s\n", i, conv(res));
       exit(1);
     }
#if 0
    /*** for debug ***/
    /* show device information */
    printf("************ device information ************\n");
    char devname[256];
    cuDeviceGetName(devname, sizeof(devname), dev);
    printf("Device Name : %s\n", devname);
    printf("--------------------------------------------\n");

    int pi;
    cuDeviceGetAttribute(&pi, CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_BLOCK, dev);
    printf("Max Threads per Block : %d\n", pi);
    printf("--------------------------------------------\n");

    cuDeviceGetAttribute(&pi, CU_DEVICE_ATTRIBUTE_INTEGRATED, dev);
    if(pi != 0) printf("device is integrated with the host memory system\n");
    else printf("device is NOT integrated with the host memory system\n");
    printf("--------------------------------------------\n");

    cuDeviceGetAttribute(&pi, CU_DEVICE_ATTRIBUTE_CAN_MAP_HOST_MEMORY, dev);
    if(pi != 0) printf("device can map host memory\n");
    else printf("device CANNOT map host memory\n");
    printf("--------------------------------------------\n");

    cuDeviceGetAttribute(&pi, CU_DEVICE_ATTRIBUTE_UNIFIED_ADDRESSING, dev);
    if(pi != 0) printf("device shares a unified address space\n");
    else printf("device DOES NOT share a unified address space\n");
    printf("--------------------------------------------\n");

    cuDeviceCanAccessPeer(&pi, dev2, dev);
    if(pi != 0) printf("dev are capable of directly accessing memory from dev2\n");
    else printf("dev are NOT capable of directly accessing memory from dev2\n");
    printf("--------------------------------------------\n");

    int major = 0, minor = 0;
    cuDeviceComputeCapability(&major, &minor, dev);
    printf("Compute Capability : major = %d, minor = %d\n", major, minor);
    printf("--------------------------------------------\n");

    cuDeviceGetCount(&pi);
    printf("Available device number : %d\n", pi);

    printf("********************************************\n");

    printf("if you want to exit, type 'q' then Push Enter key\n");
    char check_exit;
    check_exit = getchar();
    if(check_exit == 'q') exit(1);

#endif

}/* init_cuda */


/*****************************************************************/
/* clean_cuda

   cleaning up after using GPU
*/
/*****************************************************************/
void clean_cuda(void)
{
    CUresult res;

#if 0
    res = cuCtxPushCurrent(ctx);
    if(res != CUDA_SUCCESS){
      printf("cuCtxPushCurrent(ctx) failed: res = %s\n", conv(res));
      exit(1);
    }
#endif


  for(int i=0; i<device_num; i++){
    res = cuModuleUnload(module[i]);
    if(res != CUDA_SUCCESS){
        printf("\ncuModuleUnload failed: res = %s\n", conv(res));
        exit(1);
    }
 }


  for(int i=0; i<device_num; i++){
    res = cuCtxDestroy(ctx[i]);
    if(res != CUDA_SUCCESS){
        printf("\ncuCtxDestroy failed: res = %s\n", conv(res));
        exit(1);
    }
  }

    free(NR_MAXTHREADS_X);
    free(NR_MAXTHREADS_Y);
    free(func_process_root);
    free(func_process_part);
    free(func_dt1d_x); 
    free(func_dt1d_y);
    free(func_calc_a_score);
    free(func_inverse_Q);
    free(func_calc_feature);
    free(module);
    free(dev);
    free(ctx);

}/* clean_cuda */
