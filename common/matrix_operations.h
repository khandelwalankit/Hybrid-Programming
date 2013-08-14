/* Creator : Ankit Khandelwal
 * This is helper file for matrix operations. This header file has 
 * various functions which would help users to easily work with matrix.
 * TODO: Proper Casting. Currently it is working model for float type matrix
 */
#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include<iostream>

using namespace std;

template<typename T, typename U>
  struct is_same_type
  {
        static const bool value = false;
  };
template<typename T>
  struct is_same_type<T, T>
  {
        static const bool value = true;
  };



/*  mat is the matrix that needs to be printed. 
 *  xdim is xdimension 
 *  ydim is y dimension
 */

template<typename VARY, typename INT>
  void print_matrix(VARY* mat,INT xdim,INT ydim)
  { 
    INT i,j;
    for (i=0;i<xdim;i++)
    {
      for(j=0;j<ydim;j++)
      {
       if (is_same_type<VARY, int>::value) 
       {
            printf("%d ",mat[i*ydim+j]);
       }
       else
       {
            printf("%f ",mat[i*ydim+j]);
       }
      }
      printf("\n");
   }
 } 

/*  This function will initialize th e matrix
*   For now it can only take floating point numbers
*   type parameter initilizes matrix into 3 types 
*   c= initialize matrix with cholesky computable values 
*   r= initialize matrix with random values
*   s= initialize matrix with serial increment value of 1
*   n= initialize matrix with 0
*   xdim and ydim specifies the number of values need to be initilized in x and y dimension
*/

template<typename CHAR, typename INT, typename VARY>
  void initialize_matrix(CHAR type,INT xdim,INT ydim,INT number_of_arg, ...)
  {
      va_list list;
      INT j=0,i=0,k=0;
      va_start (list,number_of_arg); 
      for(j=0;j<number_of_arg;j++)
      {
          VARY* mat=va_arg(list,VARY*);
          //assign_type(var_type,1,mat);
          srand(2013);
          VARY count=1;
          for(i=0;i<xdim;i++)
          {
            for(k=0;k<ydim;k++)
            {
              if(type=='c')
              {
                 mat[i*ydim+k]=(1.0f/(1.0f+i+k)) + ((i == k)?1.0f*xdim:0.0f);
               }
              else if(type=='r')
              { 
                 mat[i*ydim+k]=drand48();
              }
              else if(type=='n')
              {
                 mat[i*ydim+k]=0;
              } 
              else if(type=='s')
              {
                 mat[i*ydim+k]=count++;
              } 
             }
           } 
       }
      va_end(list);
  } 

/* This function is used to swap the matrices
 */
template<typename VARY>
  void swap_matrix(VARY* mat1,VARY* mat2)
  {
    VARY* temp;
    temp=mat1;
    mat1=mat2;
    mat2=temp;
  }


/*void assign_type(type_t var_type,int number_of_arg, ...)
{
  va_list arg_list;
  int j=0;
  va_start(arg_list,number_of_arg);
  for(j=0;j<number_of_arg;j++)
  {
    void* mat=va_arg(arg_list,void*);
    switch(var_type)
    {
      case CHAR:
          (char*) mat;
          break;
      case INT:
          (int*) mat;
          break;
      case FLOAT:
          (float*) mat;
          break;
      case DOUBLE:
          (double*) mat;
          break;
    }
  }
  va_end(arg_list);
}*/

template<typename VARY, typename INT>               
  int GetMinor(VARY **src,VARY **dest,INT row,INT col,INT order)
  {  
    int colCount=0,rowCount=0;
    for(int i=0;i<order;i++)
    {
      if(i!=row)
       {
        colCount=0;
        for(int j=0;j<order;j++)
         {
          if(j!=col)
          {
            dest[rowCount][colCount]=src[i][j];
            //cout<<"Source is"<<src[i][j]<<"\n";
            colCount++;
          }
        }
        rowCount++;
      }
    }
    return 1;
  }

template<typename VARY,typename INT>
  VARY calculate_det(VARY **mat,INT order)
  {
    if(order==1)
    {
      return mat[0][0];
    }
    VARY det=0;
    VARY **minor;
    minor=new VARY* [order-1];
    for(int i=0;i<(order-1);i++)
      minor[i]=new VARY[order-1];

    for(int i=0;i<order;i++)
    {
      GetMinor<VARY,INT>(mat,minor,0,i,order);
      //cout<<"Value of Minor is "<<minor[0][i]<<"\n";
      //det+=pow( -1.0, 2.0 + i ) * mat[0][i] * calculate_det( minor,order-1 );
      det+=(i%2==1?-1.0:1.0) * mat[0][i] * calculate_det<VARY,INT>(minor,order-1);
    }
    for(int i=0;i<order-1;i++)
      delete[] minor[i];
    delete [] minor;
    return det;
  }

template<typename INT,typename VARY>
  void matrix_multiplycol(INT localx,INT localy, INT slicex,INT slicey,INT ydim,VARY *A,VARY *mat)
  { 
    VARY sum=0;
    for(int i=localx;i<(localx+slicex);i++)
     {
      int n=0;
      for(int j=localy;j<(localy+slicey);j++)
       {
        sum=0;
        int m=0;
        for(int k=localy;k<(localy+slicey);k++)
        {
          sum+=A[i*ydim+k]*mat[m*slicey+n];
          m++;
        }
        A[i*ydim+j]=sum;
        n++;
      }
    }
  }

template<typename INT,typename VARY>
  void calculate_inverse(INT localy,INT slicex,INT slicey,INT ydim,VARY *A,VARY **Inv)
  { 
    //Matrix to be calculated for inverse
    VARY **B;
    B=new VARY*[slicey];
    for(int i=0;i<slicey;i++)
      B[i]=new VARY[slicey];

    int m=0,n=0;
    INT order=slicey;
    for(int i=localy;i<(localy+slicex);i++)
    {
      n=0;
      for(int j=localy;j<(localy+slicey);j++)
      {
        B[m][n]=A[i*ydim+j];
        //printf("B[%d][%d]=%f\n",m,n,B[m][n]);
        n++;
       }
      m++;
     }

    double det =1.0/calculate_det<VARY,INT>(B,order);
    VARY *temp = new VARY[(order-1)*(order-1)];
    VARY **minor = new VARY*[order-1];
    for(int i=0;i<order-1;i++)
        minor[i] = temp+(i*(order-1));
    
    for(int j=0;j<order;j++)
    { 
      for(int i=0;i<order;i++)
       {
        GetMinor(B,minor,j,i,order);
        Inv[i][j] = det*calculate_det<VARY,INT>(minor,order-1);
        if( (i+j)%2 == 1)
          Inv[i][j] = -Inv[i][j];
      }
    }
    delete [] temp;
    delete [] minor;
    delete [] B;
   //cout<<"Determinant is "<<det<<"\n";
  }

template<typename INT,typename VARY>
  void calculate_transpose(INT localx,INT localy,INT slicex,INT slicey,INT ydim,VARY *A,VARY **Tran)
  { 
    int r=0;
    //printf("Value of localx and local y are Block[%d][%d]\n",localx,localy);
    for(int i=localx;i<(localx+slicex);i++)
     {
      int c=0;
      for(int j=localy;j<(localy+slicey);j++)
       {
        //cout<<"Ahiya";
        Tran[c][r]=A[i*ydim+j];
        //cout<<"Transpose Tran[c][r]"<<Tran[c][r]<<endl;
        c++;
      }
      r++;
    }
  }

template<typename INT, typename VARY>
  void matrix_multiplytri(INT localx,INT localy,INT slicex,INT slicey,INT ydim,VARY *A, VARY **mat,VARY **mult)
  { 
    VARY sum=0;
    int t=0;
    VARY *temp=(VARY *)malloc(slicex*slicey*sizeof(VARY));
    int m=0;
    for(int i=localx;i<(localx+slicex);i++)
    {
      int n=0;
      for(int j=localy;j<(localy+slicey);j++)
      {
        temp[m*slicey+n]=A[i*ydim+j];
        n++;
      }
      m++;
    }
    for(int i=0;i<slicex;i++)
     {
      for(int j=0;j<slicey;j++)
       {
        sum=0;
        for(int k=0;k<slicey;k++)
        {
          sum+=temp[i*slicey+k]*mat[k][j];
        }
        mult[i][j]=sum;
       }
     }
     delete [] temp;
  }  

template<typename INT, typename VARY>
  void matrix_subtract(INT localx,INT localy,INT slicex,INT slicey,INT ydim, VARY *A,VARY **mat)
  {
    int r=0;
    for(int i=localx;i<(localx+slicex);i++)
    {
      int c=0;
      for(int j=localy;j<(localy+slicey);j++)
      {
        A[i*ydim+j]=A[i*ydim+j]-mat[r][c];
        c++;
      }
      r++;
    }
  }
               
                 
//Concept implemented as shown in: http://users.erols.com/mdinolfo/matrix.htm                  
template<typename INT,typename VARY>
void invert(INT slicex,INT slicey, INT ydim, VARY *A)  {
    if (slicex <= 0) return;  // sanity check
    if (slicex == 1) return;  // must be of dimension >= 2
    for (int i=1; i < slicex; i++) A[i*ydim+0] /= A[0*ydim+0]; // normalize row 0
    for (int i =1; i < (slicex); i++)  
    { 
      for (int  j=i; j < (slicey); j++)  
      { // do a column of L
        VARY sum = 0.0;
        for (int k = 0; k < i; k++)  
            sum += A[j*ydim+k] * A[k*ydim+i];
        A[j*ydim+i] -= sum;
      }
      if (i == (slicex)-1) continue;
      for (int j=i+1; j < (slicex); j++)  
      {  // do a row of U
        VARY sum = 0.0;
        for (int k = 0; k < i; k++)
            sum += A[i*ydim+k]*A[k*ydim+j];
        A[i*ydim+j] = (A[i*ydim+j]-sum) / A[i*ydim+i];
      } 
    }
    for ( int i = 0; i < slicey; i++ )  // invert L
      for ( int j = i; j < (slicex); j++ )  
      {
        VARY x = 1.0;
        if ( i != j ) 
        { 
          x = 0.0;
          for ( int k = i; k < j; k++ ) 
              x -= A[j*ydim+k]*A[k*ydim+i];
        }
        A[j*ydim+i] = x / A[j*ydim+j];
      }
    for ( int i = 0; i < (slicex); i++ )   // invert U
      for ( int j = i; j < (slicey); j++ ) 
      {
        if ( i == j ) continue;
        VARY sum = 0.0;
        for ( int k = i; k < j; k++ )
            sum += A[k*ydim+j]*( (i==k) ? 1.0 : A[i*ydim+k] );
        A[i*ydim+j] = -sum;
      } 
    for ( int i = 0; i < (slicey); i++ )   // final inversion
      for ( int j = 0; j < (slicex); j++ )  
      {
        VARY sum = 0.0;
        for ( int k = ((i>j)?i:j); k < (slicex); k++ )  
            sum += ((j==k)?1.0:A[j*ydim+k])*A[k*ydim+i];
        A[j*ydim+i] = sum;
      }  
    }
