#ifndef _CUDA_H
#define _CUDA_H
#include <cuda.h>
#endif

#ifndef _SWITCH_FLOAT_H
#define _SWITCH_FLOAT_H
#include "switch_float.h"
#endif

#ifndef _MODEL_INFO
#define _MODEL_INFO
//struct for model component information
typedef struct {
    
    //basic information
    //from xxxcomp.csv
    int numcomponent;           //number of component
    int sbin;                   //cell size
    int interval;               //interval (for hierachical detection)
    int max_X;
    int max_Y;
    //from calculation
    int padx;                   //pad information
    int pady;
    int max_scale;
    //image size information
    int IM_WIDTH;
    int IM_HEIGHT;
    
//per root
    int *ridx;                  //root index information
    int *oidx;                  //offsetindex information
    FLOAT *offw;                //offset weight
    int *rsize;                 //root size
    int *numpart;               //number of part filter per component

//per part
    int **pidx;                 //part index information
    int **didx;                 //define index of part
    int **psize;

//defs
    FLOAT *def;                 //defs
    int *anchor;                //anchor

//least_square info
    FLOAT **x1;
    FLOAT **y1;
    FLOAT **x2;
    FLOAT **y2;

    bool ini;                   //flag for initialization
    FLOAT ratio;                //ratio of zooming image 

}Model_info;

//struct for root_filter_information
typedef struct {
    int NoR;                    //number of root filter
    int **root_size;            //size of root filter
    FLOAT **rootfilter;         //weight of root filter
    int *rootsym;               //symmetric information
}Rootfilters;

//struct for part_filter_information
typedef struct {
    int NoP;                    //number of part filter
    int **part_size;            //size of part filter
    FLOAT **partfilter;         //weight of root filter
    int *part_partner;          //symmetric-partner information
    int *part_sym;              //symmetric information of part filter
}Partfilters;


//model information
typedef struct {
    Model_info *MI;
    Rootfilters *RF;
    Partfilters *PF;
}MODEL;
#endif



#ifdef __cplusplus
extern "C" {
#endif


struct thread_data {
    FLOAT *A;
    FLOAT *B;
    FLOAT *C;
    FLOAT *F;
    FLOAT *T;
    int A_dims[3];
    int B_dims[3];
    int C_dims[2];
};
    
        
/* define variables for using GPU */

extern CUdevice *dev;
extern CUcontext *ctx;
extern CUfunction *func_process_root, *func_process_part, *func_dt1d_x, *func_dt1d_y, *func_calc_a_score, *func_inverse_Q;
extern CUmodule *module;
extern int *NR_MAXTHREADS_X, *NR_MAXTHREADS_Y;

extern CUdeviceptr *part_C_dev;
extern CUdeviceptr *part_error_array_dev;
extern int part_error_array_num;
extern CUdeviceptr *pm_size_array_dev;
extern CUdeviceptr *PIDX_array_dev;
extern CUdeviceptr *def_array_dev;
extern int sum_size_def_array;
extern CUdeviceptr *DID_4_array_dev;   
extern CUdeviceptr *numpart_dev;
extern int max_numpart;
extern int max_RL_S;
extern int device_num;
extern int *part_error_array;
extern size_t SUM_SIZE_C;
extern FLOAT *dst_C;

/* functions for using GPU and to calculate on GPU */
extern void init_cuda(void);

extern void clean_cuda(void);
    
/* function to convert CUDA error to string */
extern char *conv(unsigned int res);

/* function for GPU execution correspond to fconvsMT */
extern 
FLOAT ***fconvsMT_GPU(
    FLOAT **featp2,
    size_t SUM_SIZE_feat, 
    FLOAT **filter,
    int *sym_info,
    int start,
    int end,
    int *A_SIZE, 
    int **B_SIZE,
    int **M_size_array, 
    int L_MAX, 
    int interval, 
    int *FSIZE, 
    int padx, 
    int pady, 
    int max_X,
    int max_Y, 
    int calc_flag
    );
    
/* definition of calc_flag */ 
#define ROOT 0
#define PART 1

extern 
FLOAT ****dt_GPU(
    int ****Ix_array,
    int ****Iy_array,
    int ***PIDX_array,
    int **size_array,
    int NoP,
    const int *numpart,
    int NoC,
    int interval,
    int L_MAX,
    int *FSIZE,
    int padx,
    int pady,
    int max_X,
    int max_Y,
    FLOAT *def,
    int tmp_array_size,
    int *dst_PIDX,
    int *dst_DID_4
    );



/* switch define sentence  which use original source or GPU function */
//#define ORIGINAL

//#define SEPARETE_MEM


    
#ifdef __cplusplus
}
#endif

