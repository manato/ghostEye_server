#include<stdio.h>
#include<math.h>
#include"for_use_GPU.h"


/* declaration of texture memory */
//texture<FLOAT> A;
//texture<FLOAT> B;
texture<float, cudaTextureType1D, cudaReadModeElementType> A;
texture<float, cudaTextureType1D, cudaReadModeElementType> B;
texture<int2, cudaTextureType1D, cudaReadModeElementType> A_double;
texture<int2, cudaTextureType1D, cudaReadModeElementType> B_double;


//thread process
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// convolve A and B(non_symmetric)
//unsigned __stdcall process(void *thread_arg) {

/********************************************/
/* function for calculating root */
/********************************************/
extern "C"
__global__
void
process_root 
(
 //FLOAT *A,  
 //FLOAT *B, 
 FLOAT *C, 
 int *A_dims_array, 
 int *B_dims_array, 
 int len,
 int interval, 
 int L_MAX,
 int *error_array,
 int error_array_num,
 int pid,
 int device_number
) 
{
  int idx_x = blockIdx.x * blockDim.x + threadIdx.x;
  int idx_y = blockIdx.y * blockDim.y + threadIdx.y;
  int ii = blockIdx.z % len;
  int level = blockIdx.z / len;

  int A_dims[3] = { A_dims_array[level*3], A_dims_array[level*3+1], A_dims_array[level*3+2] };
  int B_dims[3] = { B_dims_array[ii*3], B_dims_array[ii*3+1], B_dims_array[ii*3+2] };
  int C_dims[2] = { A_dims[0] - B_dims[0] + 1, A_dims[1] - B_dims[1] + 1 };

  int C_x = C_dims[1]/device_number;
  
  if(C_dims[1]%device_number != 0){
    C_x++;
  }
 
  idx_x = idx_x + pid * C_x;
 
  if(idx_x < C_x * pid  ||  idx_x >=  C_x * (pid + 1)){
    return ;
  }  

  if(0 <= ii && ii < len && 0 <= idx_x && idx_x < C_dims[1] && 0 <= idx_y && idx_y < C_dims[0] && interval <= level && level < L_MAX ) { 


    int num_features = A_dims[2];
    const int A_SQ = A_dims[0]*A_dims[1];
    const int B_SQ = B_dims[0]*B_dims[1];
    FLOAT add_val = 0;
    
    int x = idx_x;
    int y = idx_y;
    int XA0 = A_dims[0]*x;

    
    /* apply loop condition */
    for(int i=0; i<error_array_num; i++){
      if(error_array[i] == level){
        return;
      }
    }
    
    
    
    /* adjust the location of pointer of C */
    FLOAT *dst;
    unsigned long long int pointer = (unsigned long long int)C;

    for(int a=interval; a<level; a++) {
      for(int b=0; b<len; b++) {
        int height = A_dims_array[a*3] - B_dims_array[b*3] + 1; 
        int width = A_dims_array[a*3 + 1] - B_dims_array[b*3 + 1] + 1;
        
        /* error semantics */
        if (height < 1 || width < 1){
          printf("Invalid input in GPU\n");
          return;
        }
        
        pointer += (unsigned long long int)(height*width*sizeof(FLOAT));
       
      }
    }

    for(int b=0; b<ii; b++){
      int height = A_dims_array[level*3] - B_dims_array[b*3] + 1;
      int width  = A_dims_array[level*3 + 1] - B_dims_array[b*3 + 1] + 1;

      /* error semantics */
      if (height < 1 || width < 1){
        printf("Invalid input in GPU\n");
        return;
      }
      
      pointer += (unsigned long long int)(height*width*sizeof(FLOAT));
    }
    
    dst = (FLOAT *)pointer;
    
    /* adjust the location of pointer of A */
    //unsigned long long int pointerA = (unsigned long long int)A;
    int A_index_ini = 0;
    for(int a=0; a<level; a++) {
      //      pointerA += (unsigned long long int)(A_dims_array[a*3]*A_dims_array[a*3 + 1]*A_dims_array[a*3 + 2]*sizeof(FLOAT));
      A_index_ini += A_dims_array[a*3]*A_dims_array[a*3 + 1]*A_dims_array[a*3 + 2];
    }
    
    
    /* adjust the location of pointer of B */
    //unsigned long long int pointerB = (unsigned long long int)B;
    int B_index_ini = 0;
    for(int b=0; b<ii; b++) {
      //      pointerB += (unsigned long long int)(B_dims_array[b*3]*B_dims_array[b*3 + 1]*B_dims_array[b*3 + 2]*sizeof(FLOAT));
      B_index_ini += B_dims_array[b*3]*B_dims_array[b*3 + 1]*B_dims_array[b*3 + 2];
    } 

            
    for(int f = 0; f < num_features; f++) // num_features = 31
      {  
        // FLOAT *A_src = (FLOAT *)pointerA + f*A_SQ;      
        int A_index = A_index_ini + f*A_SQ;
        // FLOAT *B_src = (FLOAT *)pointerB + f*B_SQ;     
        int B_index = B_index_ini + f*B_SQ;
        
        // FLOAT *A_src2 =A_src+XA0; 
        A_index += XA0;

        FLOAT val = 0;
        // FLOAT *A_off = A_src2+y;
        A_index += y;
        // FLOAT *B_off = B_src;
        
        for (int xp = 0; xp < B_dims[1]; xp++) 
          {
            // FLOAT *A_temp = A_off;						
            int A_index_tmp = A_index;
            // FLOAT *B_temp = B_off;
            int B_index_tmp = B_index;
	  
            for (int yp = 0; yp < B_dims[0]; yp++) 	  
              {
                // val += *(A_temp++) * *(B_temp++);
                if(sizeof(FLOAT) == sizeof(float)) // if configured to use single precision
                  {
                    FLOAT A_val = tex1Dfetch(A, A_index_tmp);
                    FLOAT B_val = tex1Dfetch(B, B_index_tmp);
                    val += A_val * B_val;
                  } 
                else
                  {      // if configured to use double precision
                    int2 A_val = tex1Dfetch(A_double, A_index_tmp);
                    int2 B_val = tex1Dfetch(B_double, B_index_tmp);
                    val += __hiloint2double(A_val.y, A_val.x) * __hiloint2double(B_val.y, B_val.x);
                  }
                
                A_index_tmp++;
                B_index_tmp++;
              }
            
            // A_off+=A_dims[0];
            A_index += A_dims[0];
            // B_off+=B_dims[0];
            B_index += B_dims[0];
            
          }
        
        add_val += val;
      }
    
    *(dst + (idx_x*C_dims[0] + idx_y)) += add_val;
  }
  
  
  return;
}



/********************************************/
/* function for calculating part */
/********************************************/
extern "C"
__global__
void
process_part
(
 //FLOAT *A,  
 //FLOAT *B, 
 FLOAT *C, 
 int *A_dims_array, 
 int *B_dims_array, 
 int len,
 int interval, 
 int L_MAX,
 int *error_array,
 int error_array_num,
 int pid,
 int device_number
) 
{
  int idx_x = blockIdx.x * blockDim.x + threadIdx.x;
  int idx_y = blockIdx.y * blockDim.y + threadIdx.y;
  int ii = blockIdx.z % len;
  int level = blockIdx.z / len; 

  int A_dims[3] = { A_dims_array[level*3], A_dims_array[level*3+1], A_dims_array[level*3+2] };
  int B_dims[3] = { B_dims_array[ii*3], B_dims_array[ii*3+1], B_dims_array[ii*3+2] };
  int C_dims[2] = { A_dims[0] - B_dims[0] + 1, A_dims[1] - B_dims[1] + 1 };

  int C_x = C_dims[1]/device_number;

  if(C_dims[1]%device_number != 0){
    C_x++;
  }  
 
  idx_x = idx_x + pid * C_x;
 
  if(idx_x < C_x * pid  ||  idx_x >=  C_x * (pid + 1)){
    return ;
  }  

  if(0 <= ii && ii < len && 0 <= idx_x && idx_x < C_dims[1] && 0 <= idx_y && idx_y < C_dims[0] && 0 <= level && level < (L_MAX - interval) ) {
    int num_features = A_dims[2];
    const int A_SQ = A_dims[0]*A_dims[1];
    const int B_SQ = B_dims[0]*B_dims[1];
    FLOAT add_val = 0;

    int x = idx_x;
    int y = idx_y;
    int XA0 = A_dims[0]*x;
    
    /* apply loop condition */
    for(int i=0; i<error_array_num; i++){
      if(error_array[i] == level)
        return;
    }
    
    /* adjust the location of pointer of C */
    FLOAT *dst;
    unsigned long long int pointer = (unsigned long long int)C;
    for(int a=0; a<level; a++) {
      for(int b=0; b<len; b++){
        int height = A_dims_array[a*3] - B_dims_array[b*3] + 1;
        int width = A_dims_array[a*3 + 1] - B_dims_array[b*3 + 1] + 1;
        
        /* error semantics */
        if(height < 1 || width < 1){
          printf("Invalid input in GPU\n");
          return;
        }
        
        pointer += (unsigned long long int)(height*width*sizeof(FLOAT));
      }
    }

    for(int b=0; b<ii; b++){
      int height = A_dims_array[level*3] - B_dims_array[b*3] + 1;
      int width  = A_dims_array[level*3 + 1] - B_dims_array[b*3 + 1] + 1;

       /* error semantics */
        if(height < 1 || width < 1){
          printf("Invalid input in GPU\n");
          return;
        }

      pointer += (unsigned long long int)(height*width*sizeof(FLOAT));
    }
    

    dst = (FLOAT *)pointer;

    /* adjust the location of pointer of A */
    // unsigned long long int pointerA = (unsigned long long int)A;
    int A_index_ini = 0;
    for(int a=0; a<level; a++) {
      // pointerA += (unsigned long long int)(A_dims_array[a*3]*A_dims_array[a*3 + 1]*A_dims_array[a*3 + 2]*sizeof(FLOAT));
      A_index_ini += A_dims_array[a*3]*A_dims_array[a*3 + 1]*A_dims_array[a*3 + 2];
    }
    
    /* adjust the location of pointer of B */
    // unsigned long long int pointerB = (unsigned long long int)B;
    int B_index_ini = 0;
    for(int b=0; b<ii; b++) {
      // pointerB += (unsigned long long int)(B_dims_array[b*3]*B_dims_array[b*3 + 1]*B_dims_array[b*3 + 2]*sizeof(FLOAT));
      B_index_ini += B_dims_array[b*3]*B_dims_array[b*3 + 1]*B_dims_array[b*3 + 2];
    } 
    
    for(int f = 0; f < num_features; f++) // num_features = 31
      {  
        // FLOAT *A_src = (FLOAT *)pointerA + f*A_SQ;      
        int A_index = A_index_ini + f*A_SQ;
        // FLOAT *B_src = (FLOAT *)pointerB + f*B_SQ;     
        int B_index = B_index_ini + f*B_SQ;
        
        // FLOAT *A_src2 =A_src+XA0; 
        A_index += XA0;

        FLOAT val = 0;
        // FLOAT *A_off = A_src2+y;
        A_index += y;
        // FLOAT *B_off = B_src;
        
        for (int xp = 0; xp < B_dims[1]; xp++) 
          {
            // FLOAT *A_temp = A_off;						
            int A_index_tmp = A_index;
            // FLOAT *B_temp = B_off;	  
            int B_index_tmp = B_index;
 
            for (int yp = 0; yp < B_dims[0]; yp++) 	  
              {
                // val += *(A_temp++) * *(B_temp++);
                if(sizeof(FLOAT) == sizeof(float)) // if configured to use single precision
                  {
                    FLOAT A_val = tex1Dfetch(A, A_index_tmp);
                    FLOAT B_val = tex1Dfetch(B, B_index_tmp);
                    val += A_val * B_val;
                  }
                else            // if configured to use double precision
                  {
                    int2 A_val = tex1Dfetch(A_double, A_index_tmp);
                    int2 B_val = tex1Dfetch(B_double, B_index_tmp);
                    val += __hiloint2double(A_val.y, A_val.x) * __hiloint2double(B_val.y, B_val.x);
                  }
                
                A_index_tmp++;
                B_index_tmp++;
              }
            
            // A_off+=A_dims[0];
            A_index += A_dims[0];
            // B_off+=B_dims[0];
            B_index += B_dims[0];
            
          }
        add_val += val;
      }

    *(dst + (idx_x*C_dims[0] + idx_y)) += add_val;
  }
  
  return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
__global__
void
inverse_Q(
  FLOAT *src_start,
  int *size_array,
  int *error_array,
  int error_array_num,
  int NoP,
  int *PIDX_array,
  int *numpart,
  int NoC,
  int max_numpart,
  int interval,
  int L_MAX,
  int pid,
  int device_number
          )
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int kk = blockIdx.y * blockDim.y + threadIdx.y;
  int jj = threadIdx.z;
  int L = blockIdx.z;
  int numpart_jj;
  int C_y;


  if(0<=jj && jj<NoC)
    {
      numpart_jj = numpart[jj];
      C_y = numpart_jj/device_number;
      if(numpart_jj%device_number != 0){
        C_y++;
       }
      kk = kk + pid * C_y;
      if(kk < C_y * pid  ||  kk >=  C_y * (pid + 1)){
         return ;
       }
    } else return ;
   

  if(0<=L && L < (L_MAX-interval)) 
    {
  
      /* loop condition */
      for(int h=0; h<error_array_num; h++) {
        if(L==error_array[h]){ 
          return;
        }
      }
    
     
      if( 0<=kk && kk < numpart_jj )
        {
          int PIDX = PIDX_array[L*(NoC*max_numpart) + jj*max_numpart + kk];
          int dim0 = size_array[L*NoP*2 + PIDX*2];
          int dim1 = size_array[L*NoP*2 + PIDX*2+1]; 

          if( idx < 0 || dim0*dim1 <= idx) return;

              /* pointer adjustment */
          FLOAT *src;
          unsigned long long int ptr_adjuster = (unsigned long long int)src_start;
          for(int i=0; i<L; i++) {
                
                /* apply error condition */
            int error_flag=0;
            for(int h=0; h<error_array_num; h++) {
              if(i==error_array[h]){
                error_flag = 1;
              }
            }
            if(error_flag != 0) {
              continue;
            }

                
            for(int j=0; j<NoP; j++) {
              int height = size_array[i*NoP*2 + j*2];
              int width = size_array[i*NoP*2 + j*2+1];
              ptr_adjuster += (unsigned long long int)(height*width*sizeof(FLOAT));
                  
            }
          }
              
   
              
          for(int j=0; j<PIDX; j++) {
            int height = size_array[L*NoP*2 + j*2];
            int width = size_array[L*NoP*2 + j*2+1];
            ptr_adjuster += (unsigned long long int)(height*width*sizeof(FLOAT));
          }
              
          src = (FLOAT *)ptr_adjuster;  
                        
          *(src + idx) *= -1;
        
      }
    }       
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dt helper function
__device__
void 
dt_helper(FLOAT *src, FLOAT *dst, int *ptr, int step, int s1, int s2, int d1, int d2, FLOAT a, FLOAT b) 
{
  if (d2 >= d1) 
    {
      int d = (d1+d2) >> 1;
      int ds =d*step;
      int s = s1;
      FLOAT src_ss = *(src+s*step);
      for (int p = s1+1; p <= s2; p++)
        {
          int t1 = d-s;
          int t2 = d-p;
          if (src_ss + a*t1*t1 + b*t1 > *(src+p*step) + a*t2*t2 + b*t2) 
            {
              s = p;
              src_ss = *(src+s*step);
            }
        }
      int D = d-s;
      dst[ds] = *(src+s*step) + a*D*D + b*D;
      ptr[ds] = s;
      dt_helper(src, dst, ptr, step, s1, s, d1, d-1, a, b);
      dt_helper(src, dst, ptr, step, s, s2, d+1, d2, a, b);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//sub function of dt 
extern "C"
__global__
void 
dt1d_x(
  FLOAT *src_start,             // part_C_dev
  FLOAT *dst_start,             // tmpM_dev
  int *ptr_start,               // tmpIy_dev
  int *DID_4_array,             // DID_4_array_dev
  FLOAT *def_array,             // def_array_dev
  int *size_array,              // pm_size_array_dev
  int NoP,                      // NoP
  int *PIDX_array,              // PIDX_array_dev
  int *error_array,             // part_error_array_dev
  int error_array_num,          // part_error_array_num
  int *numpart,                 // numpart_jj
  int NoC,                      // NoC
  int max_numpart,              // max_numpart
  int interval,                 // interval
  int L_MAX,                     // L_MAX
  int pid,                       // pid
  int device_number              // device_number

       ) 
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int kk = blockIdx.y * blockDim.y + threadIdx.y;
  int jj = threadIdx.z;
  int L = blockIdx.z;
  int numpart_jj;
  int C_y;

  if(0<=jj && jj<NoC)
    {

      numpart_jj = numpart[jj];
      C_y = numpart_jj/device_number;

      if(numpart_jj%device_number != 0){
        C_y++;
       }
 
      kk = kk + pid * C_y;
 
      if(kk < C_y * pid  ||  kk >=  C_y * (pid + 1)){
         return ;
       }
    } else{
      return ;
    }


  if(0<=L && L<(L_MAX-interval)) 
    {
      /* loop condition */
      for(int h=0; h<error_array_num; h++) {
        if(L==error_array[h]){ 
          return;
        }
      }
                
      if(0<=kk && kk<numpart_jj)
        {
          int PIDX = PIDX_array[L*(NoC*max_numpart) + jj*max_numpart + kk];
          int dim1 = size_array[L*NoP*2 + PIDX*2+1]; 

          if( idx < 0 || dim1 <= idx ) return;

          int dim0 = size_array[L*NoP*2 + PIDX*2];
          int XD=0;
          int step = 1;
          int n = dim0;  
          int DID_4 = DID_4_array[L*(NoC*max_numpart) + jj*max_numpart + kk];
          FLOAT a = def_array[DID_4+2];
          FLOAT b = def_array[DID_4+3];
             
          /* pointer adjustment */
          unsigned long long int adj_src = (unsigned long long int)src_start;
          unsigned long long int adj_dst = (unsigned long long int)dst_start;
          unsigned long long int adj_ptr = (unsigned long long int)ptr_start;
          /* for src */
          for(int i=0; i<L; i++) {
                
            /* apply error condition */
            int error_flag=0;
            for(int h=0; h<error_array_num; h++) {
              if(i==error_array[h]){
                error_flag = 1;
              }
            }
            if(error_flag != 0) {
              continue;
            }
                
            for(int j=0; j<NoP; j++) {
              int height = size_array[i*NoP*2 + j*2];
              int width = size_array[i*NoP*2 + j*2+1];
              adj_src += (unsigned long long int)(height*width*sizeof(FLOAT));
                  
            }
          }
              
              
          for(int j=0; j<PIDX; j++) {
            int height = size_array[L*NoP*2 + j*2];
            int width = size_array[L*NoP*2 + j*2+1];
            adj_src += (unsigned long long int)(height*width*sizeof(FLOAT));
          }
              
              /* for dst, ptr */
              // adjust "dst" to tmpM[L][jj][kk]
              // adjust "ptr" to tmpIy[L][jj][kk]
          for(int i=0; i<L; i++) {
                
                /* apply error condition */
            int error_flag=0;
            for(int h=0; h<error_array_num; h++) {
              if(i==error_array[h]){
                error_flag = 1;
              }
            }
            if(error_flag != 0) {
              continue;
            }
                
            for(int j=0; j<NoC; j++) {
              for(int k=0; k<numpart[j]; k++) {
                int PIDX_tmp = PIDX_array[i*(NoC*max_numpart) + j*max_numpart + k];
                int dims0_tmp = size_array[i*NoP*2 + PIDX_tmp*2];
                int dims1_tmp = size_array[i*NoP*2 + PIDX_tmp*2+1];

                    
                adj_dst += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(FLOAT));
                adj_ptr += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(int));
                    
                    
              }
            }
          }
              

          for(int i=0; i<jj; i++) {
            for(int j=0; j<numpart[i]; j++) {
              int PIDX_tmp = PIDX_array[L*(NoC*max_numpart) + i*max_numpart + j]; // PIDX_array[L][i][j]
              int dims0_tmp = size_array[L*NoP*2 + PIDX_tmp*2]; // size_array[L][PIDX_tmp*2]
              int dims1_tmp = size_array[L*NoP*2 + PIDX_tmp*2+1]; // size_array[L][PIDX_tmp*2+1]
                  
              adj_dst += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(FLOAT));
              adj_ptr += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(int));
                  
            }
          }
              
          for(int j=0; j<kk; j++) {
            int PIDX_tmp = PIDX_array[L*(NoC*max_numpart) + jj*max_numpart + j]; // PIDX_array[L][jj][j]
            int dims0_tmp = size_array[L*NoP*2 + PIDX_tmp*2]; // size_array[L][PIDX_tmp*2]
            int dims1_tmp = size_array[L*NoP*2 + PIDX_tmp*2+1]; // size_array[L][PIDX_tmp*2+1]
                
            adj_dst += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(FLOAT));
            adj_ptr += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(int));
          }
              
              
          FLOAT *src = (FLOAT *)adj_src;
          FLOAT *dst = (FLOAT *)adj_dst;
          int *ptr = (int *)adj_ptr;
              
          /* main calculation of di1d_x */
          XD = idx*dim0;
          dt_helper(src+XD, dst+XD, ptr+XD, step, 0, n-1, 0, n-1, a, b);
            
        }
    }
}


extern "C"
__global__
void 
dt1d_y(
  FLOAT *src_start,             // tmpM_dev
  FLOAT *dst_start,             // M_dev
  int *ptr_start,               // tmpIx_dev
  int *DID_4_array,             // DID_4_array_dev
  FLOAT *def_array,             // def_array_dev
  int NoP,                      // NoP
  int *size_array,              // pm_size_array_dev
  int *numpart,                 // numpart_jj
  int *PIDX_array,              // PIDX_array_dev
  int NoC,                      // NoC
  int max_numpart,              // max_numpart
  int interval,                 // interval
  int L_MAX,                    // L_MAX
  int *error_array,             // part_error_array_dev
  int error_array_num,           // part_error_array_num
  int pid,                       // pid
  int device_number              // device_number
       ) 
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int kk = blockIdx.y * blockDim.y + threadIdx.y;
  int jj = threadIdx.z;
  int L = blockIdx.z;
  int numpart_jj;
  int C_y;

  if(0<=jj && jj<NoC)
    {

      numpart_jj = numpart[jj];
      C_y = numpart_jj/device_number;

      if(numpart_jj%device_number != 0){
        C_y++;
       }
 
      kk = kk + pid * C_y;
 
      if(kk < C_y * pid  ||  kk >=  C_y * (pid + 1)){
         return ;
       }
    } else{
      return ;
    }


  if(0<=L && L<(L_MAX-interval)) 
    {
      /* loop condition */
      for(int h=0; h<error_array_num; h++) {
        if(L==error_array[h]){ 
          return;
        }
      }
      
      
      if( 0<=kk && kk<numpart_jj)
        {
          int PIDX = PIDX_array[L*(NoC*max_numpart) + jj*max_numpart + kk];
          int dim0 = size_array[L*NoP*2 + PIDX*2];

          if( idx < 0 || dim0 <= idx ) return;

          int dim1 = size_array[L*NoP*2 + PIDX*2+1];
          int step  = dim0;
          int n = dim1;
              
          int DID_4 = DID_4_array[L*(NoC*max_numpart) + jj*max_numpart + kk];
              
          FLOAT a = def_array[DID_4];   // ax
          FLOAT b = def_array[DID_4+1]; // bx
              
              /* pointer adjustment */
          unsigned long long int adj_src = (unsigned long long int)src_start;
          unsigned long long int adj_dst = (unsigned long long int)dst_start;
          unsigned long long int adj_ptr = (unsigned long long int)ptr_start;
              /* for src, dst, ptr */
              /* adjust "src" to tmpM[L][jj][kk] */
              /* adjust "dst" to M[L][jj][kk] */
              /* adjust "ptr" to tmpIx[L][jj][kk] */
          for(int i=0; i<L; i++) {

            /* apply error condition */
            int error_flag=0;
            for(int h=0; h<error_array_num; h++) {
              if(i==error_array[h]){
                error_flag = 1;
              }
            }
            if(error_flag != 0) {
              continue;
            }
                
            for(int j=0; j<NoC; j++) {
              for(int k=0; k<numpart[j]; k++) {
                    
                int PIDX_tmp = PIDX_array[i*(NoC*max_numpart) + j*max_numpart + k];
                int dims0_tmp = size_array[i*NoP*2 + PIDX_tmp*2];
                int dims1_tmp = size_array[i*NoP*2 + PIDX_tmp*2+1];
                    
                adj_src += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(FLOAT));
                adj_dst += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(FLOAT));
                adj_ptr += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(int));
                    
              }
            }
          }


          for(int i=0; i<jj; i++) {
            for(int j=0; j<numpart[i]; j++) {
              int PIDX_tmp = PIDX_array[L*(NoC*max_numpart) + i*max_numpart + j]; // PIDX_array[L][i][j]
              int dims0_tmp = size_array[L*NoP*2 + PIDX_tmp*2]; // size_array[L][PIDX_tmp*2]
              int dims1_tmp = size_array[L*NoP*2 + PIDX_tmp*2+1]; // size_array[L][PIDX_tmp*2+1]
                  
              adj_src += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(FLOAT));
              adj_dst += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(FLOAT));
              adj_ptr += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(int));
                  
            }
          }
              
          for(int j=0; j<kk; j++) {
            int PIDX_tmp = PIDX_array[L*(NoC*max_numpart) + jj*max_numpart + j];
            int dims0_tmp = size_array[L*NoP*2 + PIDX_tmp*2];
            int dims1_tmp = size_array[L*NoP*2 + PIDX_tmp*2+1];
                
            adj_src += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(FLOAT));
            adj_dst += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(FLOAT));
            adj_ptr += (unsigned long long int)(dims0_tmp*dims1_tmp*sizeof(int));
          }
              
              
              
          FLOAT *src = (FLOAT *)adj_src;
          FLOAT *dst = (FLOAT *)adj_dst;
          int *ptr = (int *)adj_ptr;
              
 
          dt_helper(src+idx, dst+idx, ptr+idx, step, 0, n-1, 0, n-1, a, b);
          
              
        }
    }
}

/*************************************************************/
/*************************************************************/
/* original source of dt function loop */
// for (int x = 0; x < dims[1]; x++)
//   {
//     dt1d(vals+XD, tmpM+XD, tmpIy+XD, 1, dims[0], ay, by);
//     XD+=dims[0];
//   }
// for (int y = 0; y < dims[0]; y++)
//   {
//     dt1d(tmpM+y, M+y, tmpIx+y, dims[0], dims[1], ax, bx);
//   }
/*************************************************************/
/*************************************************************/



extern "C"
__global__
void
calc_a_score(
 int IWID,
 int IHEI,
 FLOAT scale,
 int padx_n,
 int pady_n,
 int *RX_array,
 int *RY_array,
 FLOAT *ac_score,
 FLOAT *score_array,
 int *ssize_array,
 int NoC,
 int *size_score_array
)
{
  int ii = blockIdx.x * blockDim.x + threadIdx.x;
  int jj = blockIdx.y * blockDim.y + threadIdx.y;

  int component_jj = threadIdx.z;

  if(0<=component_jj && component_jj < NoC) 
    {

      unsigned long long int pointer_score = (unsigned long long int)score_array;
      unsigned long long int pointer_ssize = (unsigned long long int)ssize_array;
      unsigned long long int pointer_RX = (unsigned long long int)RX_array;
      unsigned long long int pointer_RY = (unsigned long long int)RY_array;
      for(int k=0; k<component_jj; k++) {
        pointer_score += (unsigned long long int)size_score_array[k];
        pointer_ssize += (unsigned long long int)(sizeof(int));
        pointer_RX += (unsigned long long int)(sizeof(int));
        pointer_RY += (unsigned long long int)(sizeof(int));
      }

      FLOAT *score = (FLOAT *)pointer_score;
      int ssize0 = *((int *)pointer_ssize);
      int ssize1 = *((int *)pointer_ssize + sizeof(int));
      int RX = *((int *)pointer_RX);
      int RY = *((int *)pointer_RY);



      if(0<=ii && ii<IWID && 0<=jj && jj<IHEI)
        {
          int Xn = (int)((FLOAT)ii/scale+padx_n);
          int Yn = (int)((FLOAT)jj/scale+pady_n);

          
          if(Yn<ssize0 && Xn<ssize1)
            {
              FLOAT sc = score[Yn+Xn*ssize0];
              int Im_Y = jj+RY;
              int Im_X = ii+RX;
              if(Im_Y<IHEI && Im_X<IWID)
                {
                  FLOAT *PP = ac_score+Im_Y+Im_X*IHEI;
                  if(sc>*PP) *PP=sc;
                }
            }
        }
    }
  
  /*************************************************************/
  /*************************************************************/
  /* original source of calc_a_score loop */
  // for(int ii=0;ii<IWID;ii++)
  //   {
  //     int Xn=(int)((FLOAT)ii/scale+padx_n);
  
  //     for(int jj=0;jj<IHEI;jj++)
  //       {
  //         int Yn =(int)((FLOAT)jj/scale+pady_n);
  
  //         if(Yn<ssize[0] && Xn<ssize[1])
  //           {
  //             FLOAT sc = score[Yn+Xn*ssize[0]]; //get score of pixel
      
  //             int Im_Y = jj+RY;
  //             int Im_X = ii+RX;
  //             if(Im_Y<IHEI && Im_X<IWID)
  //               {
  //                 FLOAT *PP=ac_score+Im_Y+Im_X*IHEI; //consider root rectangle size
  //                 if(sc>*PP) *PP=sc;                 //save max score
  //               }
  //           }
  //       }
  //   }
  /*************************************************************/
  /*************************************************************/
  
}



#define max_i(x, y) ((x)>=(y) ? (x) : (y))
#define min_i(x, y) ((x)<=(y) ? (x) : (y))

/* atomic function dealing with double precision */
__device__ 
double 
atomicAdd_double
(
 double *address,
 double val
 )
{
  unsigned long long int *address_as_ull = (unsigned long long int *)address;
  unsigned long long int old = *address_as_ull, assumed;
  do {
    assumed = old;
    old = atomicCAS(address_as_ull, assumed, __double_as_longlong(val + __longlong_as_double(assumed)));
  }while(assumed != old);
  return __longlong_as_double(old);
}  
  
extern "C"
__global__
void
calc_feature
(
 FLOAT *SRC,
 int *ISIZE, 
 FLOAT *HHist,
 int vis_R1,
 int vis_R0,
  int sbin
 )
{
  
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;
  
  const FLOAT Hcos[9]={1.0000,0.9397,0.7660,0.5000,0.1736,-0.1736,-0.5000,-0.7660,-0.9397};
  const FLOAT Hsin[9]={0.0000,0.3420,0.6428,0.8660,0.9848,0.9848,0.8660,0.6428,0.3420};


  if(1<=x && x<vis_R1 && 1<=y && y<vis_R0)
    {
      
      //input size 
      const int height=ISIZE[0]; //{268,268,134,67,233,117,203,203,177,154,89,203,154,77}
      const int width=ISIZE[1];  //{448,112,224,390,195,340,170,296,257,148,340,257,129}
      const int dims[2]={height,width};
      
      //size of Histgrams and Norm calculation space size 
      const int blocks[2] = {(int)floor(double(height)/double(sbin)+0.5),(int)floor(double(width)/double(sbin)+0.5)};//{67,112}....sbine=4
      const int BLOCK_SQ = blocks[0]*blocks[1];//{7504}...
      
      const int vp0=dims[0]-2;
      const int vp1=dims[1]-2;
      const int SQUARE =dims[0]*dims[1];
      const FLOAT SBIN = FLOAT(sbin);
      



      // for(int x=1;x<vis_R[1];x++)
      //   {
      //parameters for interpolation
      FLOAT xp=((FLOAT)x+0.5)/SBIN-0.5;
      int ixp=(int)floor(xp);
      int ixpp=ixp+1;
      int ixp_b  = ixp * blocks[0];	
      int ixpp_b = ixp_b + blocks[0];
      FLOAT vx0=xp-(FLOAT)ixp;	
      FLOAT vx1=1.0-vx0;
      bool flag1=true,flag2=true,flagX=true;
  
      if(ixp<0) 
        {
          flag1=false;
          flagX=false;
        }
      if(ixpp>=blocks[1])
        {
          flag2=false;
          flagX=false;
        }
  
      int YC=min_i(x,vp1)*dims[0];
      FLOAT *SRC_YC = SRC+YC;
  
      //      for(int y=1;y<vis_R[0];y++)
      //        {
      //first color channel
      FLOAT *s=SRC_YC+min_i(y,vp0);
      FLOAT dy=*(s+1)-*(s-1);
      FLOAT dx=*(s+dims[0])-*(s-dims[0]);
      FLOAT v=dx*dx+dy*dy;
      
      //second color channel
      s+=SQUARE;
      FLOAT dy2=*(s+1)-*(s-1);
      FLOAT dx2=*(s+dims[0])-*(s-dims[0]);
      FLOAT v2=dx2*dx2+dy2*dy2;
      
      //third color channel
      s+=SQUARE;
      FLOAT dy3=*(s+1)-*(s-1);
      FLOAT dx3=*(s+dims[0])-*(s-dims[0]);
      FLOAT v3=dx3*dx3+dy3*dy3;
      
      //pick channel with strongest gradient
      if(v2>v)
        {
          v=v2;
          dx=dx2;
          dy=dy2;
        }
      
      if(v3>v)
        {
          v=v3;
          dx=dx3;
          dy=dy3;
        }
      
      FLOAT best_dot=0.0;
      int best_o=0;
      
      //snap to one of 18 orientations
      for(int o=0;o<9;o++)
        {
          FLOAT dot=Hcos[o]*dx+Hsin[o]*dy;
          if(dot>best_dot)
            {
              best_dot=dot;
              best_o=o;
            }
          else if(-dot>best_dot)
            {
              best_dot=-dot;
              best_o=o+9;
            }
        }
      
      //Add to 4 histgrams around pixel using linear interpolation
      FLOAT yp=((FLOAT)y+0.5)/SBIN-0.5;
      int iyp=(int)floor(yp);
      int iypp=iyp+1;
      FLOAT vy0=yp-(FLOAT)iyp;
      FLOAT vy1=1.0-vy0;
      v=sqrt(v);
      int ODim=best_o*BLOCK_SQ;
      FLOAT *Htemp = HHist+ODim;
      FLOAT vx1Xv =vx1*v;
      FLOAT vx0Xv = vx0*v;
      
      if(flagX)
        {
          if(iyp>=0)
            {
              // *(Htemp+ ixp_b+iyp)+=vy1*vx1Xv; //1-少数をxyでかけたものにエッジ強度の2乗をかけたもの
              // *(Htemp+ ixpp_b+iyp)+=vy1*vx0Xv;
              if(sizeof(FLOAT) == sizeof(float)) {
                atomicAdd((float*)(Htemp + ixp_b + iyp), (float)(vy1*vx1Xv));
                atomicAdd((float*)(Htemp + ixpp_b + iyp), (float)(vy1*vx0Xv));
              }else{
                atomicAdd_double((double*)(Htemp + ixp_b + iyp), (double)(vy1*vx1Xv));
                atomicAdd_double((double*)(Htemp + ixpp_b + iyp), (double)(vy1*vx0Xv));
              }
            }
          if (iypp<blocks[0])
            {
              // *(Htemp+ ixp_b+iypp)+=vy0*vx1Xv;
              // *(Htemp+ ixpp_b+iypp)+=vy0*vx0Xv;
              if(sizeof(FLOAT) == sizeof(float)) {
                atomicAdd((float*)(Htemp + ixp_b + iypp), (float)(vy0*vx1Xv));
                atomicAdd((float*)(Htemp + ixpp_b + iypp), (float)(vy0*vx0Xv));
              }else{
                atomicAdd_double((double*)(Htemp + ixp_b + iypp), (double)(vy0*vx1Xv));
                atomicAdd_double((double*)(Htemp + ixpp_b + iypp), (double)(vy0*vx0Xv));
              }
            }
        }
      else if(flag1)
        {
          if (iyp>=0) {
             // *(Htemp+ixp_b+iyp)+=vy1*vx1Xv;
            if(sizeof(FLOAT) == sizeof(float)) {
              atomicAdd((float*)(Htemp + ixp_b + iyp), (float)(vy1*vx1Xv));
            }else{
              atomicAdd_double((double*)(Htemp + ixp_b + iyp), (double)(vy1*vx1Xv));
            }
          }
          if (iypp<blocks[0]) {
             // *(Htemp+ixp_b+iypp)+=vy0*vx1Xv;
            if(sizeof(FLOAT) == sizeof(float)) {
              atomicAdd((float*)(Htemp + ixp_b + iypp), (float)(vy0*vx1Xv));
            }else{
              atomicAdd_double((double*)(Htemp + ixp_b + iypp), (double)(vy0*vx1Xv));
            }
          }
        }
      else if(flag2)
        {
          if(iyp>=0) {
             // *(Htemp+ixpp_b+iyp)+=vy1*vx0Xv;
            if(sizeof(FLOAT)==sizeof(float)) {
              atomicAdd((float*)(Htemp + ixpp_b + iyp), (float)(vy1*vx0Xv));
            }else{
              atomicAdd_double((double*)(Htemp + ixpp_b + iyp), (double)(vy1*vx0Xv));
            }
          }
          if(iypp<blocks[0]) {
             // *(Htemp+ixpp_b+iypp)+=vy0*vx0Xv;
            if(sizeof(FLOAT)==sizeof(float)) {
              atomicAdd((float*)(Htemp + ixpp_b + iypp), (float)(vy0*vx0Xv));
            }else{
              atomicAdd_double((double*)(Htemp + ixpp_b + iypp), (double)(vy0*vx0Xv));
            }
          }
        }
      //    }
      //}




      
      
    }
  
  /*************************************************************/
  /*************************************************************/
  /* original source of calc_feature loop */
  // for(int x=1;x<vis_R[1];x++)
  //   {
  //     //parameters for interpolation
  //     FLOAT xp=((FLOAT)x+0.5)/SBIN-0.5;
  //     int ixp=(int)floor(xp);
  //     int ixpp=ixp+1;
  //     int ixp_b  = ixp * blocks[0];	
  //     int ixpp_b = ixp_b + blocks[0];
  //     FLOAT vx0=xp-(FLOAT)ixp;	
  //     FLOAT vx1=1.0-vx0;
  //     bool flag1=true,flag2=true,flagX=true;
  
  //     if(ixp<0) 
  //       {
  //         flag1=false;
  //         flagX=false;
  //       }
  //     if(ixpp>=blocks[1])
  //       {
  //         flag2=false;
  //         flagX=false;
  //       }
  
  //     int YC=min_i(x,vp1)*dims[0];
  //     FLOAT *SRC_YC = SRC+YC;
  
  //     for(int y=1;y<vis_R[0];y++)
  //       {
  //         //first color channel
  //         FLOAT *s=SRC_YC+min_i(y,vp0);
  //         FLOAT dy=*(s+1)-*(s-1);
  //         FLOAT dx=*(s+dims[0])-*(s-dims[0]);
  //         FLOAT v=dx*dx+dy*dy;
  
  //         //second color channel
  //         s+=SQUARE;
  //         FLOAT dy2=*(s+1)-*(s-1);
  //         FLOAT dx2=*(s+dims[0])-*(s-dims[0]);
  //         FLOAT v2=dx2*dx2+dy2*dy2;
  
  //         //third color channel
  //         s+=SQUARE;
  //         FLOAT dy3=*(s+1)-*(s-1);
  //         FLOAT dx3=*(s+dims[0])-*(s-dims[0]);
  //         FLOAT v3=dx3*dx3+dy3*dy3;
  
  //         //pick channel with strongest gradient
  //         if(v2>v)
  //           {
  //             v=v2;
  //             dx=dx2;
  //             dy=dy2;
  //           }
  
  //         if(v3>v)
  //           {
  //             v=v3;
  //             dx=dx3;
  //             dy=dy3;
  //           }
  
  //         FLOAT best_dot=0.0;
  //         int best_o=0;
			
  //         //snap to one of 18 orientations
  //         for(int o=0;o<9;o++)
  //           {
  //             FLOAT dot=Hcos[o]*dx+Hsin[o]*dy;
  //             if(dot>best_dot)
  //               {
  //                 best_dot=dot;
  //                 best_o=o;
  //               }
  //             else if(-dot>best_dot)
  //               {
  //                 best_dot=-dot;
  //                 best_o=o+9;
  //               }
  //           }
  
  //         //Add to 4 histgrams around pixel using linear interpolation
  //         FLOAT yp=((FLOAT)y+0.5)/SBIN-0.5;
  //         int iyp=(int)floor(yp);
  //         int iypp=iyp+1;
  //         FLOAT vy0=yp-(FLOAT)iyp;
  //         FLOAT vy1=1.0-vy0;
  //         v=sqrt(v);
  //         int ODim=best_o*BLOCK_SQ;
  //         FLOAT *Htemp = HHist+ODim;
  //         FLOAT vx1Xv =vx1*v;
  //         FLOAT vx0Xv = vx0*v;

  //         if(flagX)
  //           {
  //             if(iyp>=0)
  //               {
  //                 *(Htemp+ ixp_b+iyp)+=vy1*vx1Xv; //1-少数をxyでかけたものにエッジ強度の2乗をかけたもの
  //                 *(Htemp+ ixpp_b+iyp)+=vy1*vx0Xv;
  //               }
  //             if (iypp<blocks[0])
  //               {
  //                 *(Htemp+ ixp_b+iypp)+=vy0*vx1Xv;
  //                 *(Htemp+ ixpp_b+iypp)+=vy0*vx0Xv;
  //               }
  //           }
  //         else if(flag1)
  //           {
  //             if (iyp>=0) *(Htemp+ixp_b+iyp)+=vy1*vx1Xv;
  //             if (iypp<blocks[0]) *(Htemp+ixp_b+iypp)+=vy0*vx1Xv;
  //           }
  //         else if(flag2)
  //           {
  //             if(iyp>=0) *(Htemp+ixpp_b+iyp)+=vy1*vx0Xv;
  //             if(iypp<blocks[0]) *(Htemp+ixpp_b+iypp)+=vy0*vx0Xv;
  //           }
  //       }
  //   }
  /*************************************************************/
  /*************************************************************/
  
  
}



