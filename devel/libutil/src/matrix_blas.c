#include <stdbool.h>
#include <util.h>
#include <matrix.h>
#include <matrix_blas.h>


/*****************************************************************/
void  dgemm_(char * , char * , int * , int * , int * , double * , double * , int * , double * , int *  , double * , double * , int *);
void  dgemv_(char * , int * , int * , double * , double * , int * , double * , int * , double * , double * , int * );
/*****************************************************************/



/**
   y = alpha * op(A)*x + beta * y

   alpha,beta: scalars
   x,y       : vectors
   A         : matrix

   
   x and y are entered as (double * ).
*/


void matrix_dgemv(const matrix_type * A , const double *x , double * y, bool transA , double alpha , double beta) {
  int m    = matrix_get_rows( A );
  int n    = matrix_get_columns( A );
  int lda  = matrix_get_column_stride( A );
  int incx = 1;
  int incy = 1;

  char transA_c;
  if (transA) 
    transA_c = 'T';
  else
    transA_c = 'N';
 
  dgemv_(&transA_c , &m , &n , &alpha , matrix_get_data( A ) , &lda , x , &incx , &beta , y , &incy);
}


/**
   y = A*x
*/

void matrix_mul_vector(const matrix_type * A , const double * x , double * y) {
  matrix_dgemv(A , x , y , false , 1 , 0);
}



static void dgemm_debug(const matrix_type *C , const matrix_type *A , const matrix_type * B , bool transA, bool transB) {
  printf("A: [%d , %d]", matrix_get_rows( A ) , matrix_get_columns(A));
  if (transA)
    printf("^T");

  printf("\nB: [%d , %d]", matrix_get_rows( B ) , matrix_get_columns(B));
  if (transB)
    printf("^T");

  printf("\nC: [%d , %d]\n",matrix_get_rows( C ) , matrix_get_columns(C));
}



/**
   C = alpha * op(A) * op(B)  +  beta * C

   op(�) can either be unity or Transpose.
*/

void matrix_dgemm(matrix_type *C , const matrix_type *A , const matrix_type * B , bool transA, bool transB , double alpha , double beta) {
  int m   = matrix_get_rows( C );
  int n   = matrix_get_columns( C );
  int lda = matrix_get_column_stride( A );
  int ldb = matrix_get_column_stride( B );
  int ldc = matrix_get_column_stride( C );
  char transA_c;
  char transB_c;
  int  k , innerA, innerB , outerA , outerB;
  
  if (transA) 
    k = matrix_get_rows( A );
  else
    k = matrix_get_columns( A );
  
  
  if (transA) {
    innerA = matrix_get_rows(A);
    outerA = matrix_get_columns(A);
    transA_c = 'T';
  } else {
    innerA = matrix_get_columns(A);
    outerA = matrix_get_rows(A);
    transA_c = 'N';
  }

  
  if (transB) {
    innerB   = matrix_get_columns( B ); 
    outerB   = matrix_get_rows( B ); 
    transB_c = 'T';
  } else {
    transB_c = 'N';
    innerB = matrix_get_rows( B );
    outerB = matrix_get_columns( B ); 
  }
  
  /*
    This is the dimension check which must pass:

    --------------------------------------------------
          A   |         B   |  Columns(A) = Rows(B)
    Trans(A)  |   Trans(B)  |  Rows(A)    = Columns(B)
          A   |   Trans(B)  |  Columns(A) = Columns(B)
    Trans(A)  |         B   |  Rows(A)    = Rows(B)	  
    --------------------------------------------------

    --------------------------------------------------
              A         | Rows(A)    = Rows(C)
	Trans(A)        | Columns(A) = Rows(C)
	      B         | Columns(B) = Columns(C)
	Trans(B)        | Rows(B)    = Columns(B)
    --------------------------------------------------
    
  */
  
  if (innerA != innerB) {
    dgemm_debug(C,A,B,transA , transB);
    util_abort("%s: matrix size mismatch between A and B \n", __func__);
  }
  
  if (outerA != matrix_get_rows( C )) {
    dgemm_debug(C,A,B,transA , transB);
    util_abort("%s: matrix size mismatch between A and C \n",__func__);
  }

  
  if (outerB != matrix_get_columns( C )) {
    dgemm_debug(C,A,B,transA , transB);
    util_abort("%s: matrix size mismatch between B and C \n",__func__);
  }


  dgemm_(&transA_c , &transB_c , &m ,&n , &k , &alpha , matrix_get_data( A ) , &lda , matrix_get_data( B ) , &ldb , &beta , matrix_get_data( C ) , &ldc);
}




/* 
   This function does a general matrix multiply of A * B, and stores
   the result in C.
*/

void matrix_matmul(matrix_type * C, const matrix_type * A , const matrix_type * B) {
  matrix_dgemm( C , A , B , false , false , 1 , 0);
}
