
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <assert.h>
#include "IDX1_DATA.h"
#include "IDX3_DATA.h"

#ifndef IDX_H
#define IDX_H



class idx {
    
public:
    int fread_idx1_file(char *, IDX1_DATA *);
    int fread_idx3_file(char *, IDX3_DATA *);
};




int idx::fread_idx1_file( char *slabelfname, IDX1_DATA *idxdata){
   FILE *f = fopen(slabelfname, "r");
   if(!f){
      return 0;
   }
   
   fread(&(idxdata->header),4,1,f);
   if( idxdata->header[0] != 0 ) return 0;
   if( idxdata->header[1] != 0 ) return 0;
   if( idxdata->header[2] != 8 ) return 0; // the kind of data should be an unsigned byte
   if( idxdata->header[3] != 1 ) return 0; // there should be only 1 dimension

   int x;
   fread(&x, 4,1,f); idxdata->length = ntohl( x );
      
   idxdata->data =(unsigned char*) calloc( idxdata->length, 1);
   if( !idxdata->data ) return 0;
   
   fread(idxdata->data, idxdata->length, 1, f);
   
   return 1;
}


int idx::fread_idx3_file( char *slabelfname, IDX3_DATA *idxdata){
   FILE *f = fopen(slabelfname, "r");
   if(!f){
      return 0;
   }
   
   fread(&(idxdata->header),4,1,f);
   if( idxdata->header[0] != 0 ) return 0;
   if( idxdata->header[1] != 0 ) return 0;
   if( idxdata->header[2] != 8 ) return 0; // the kind of data should be an unsigned byte
   if( idxdata->header[3] != 3 ) return 0; // there should be 3 dimensions (number of images, number of rows, number of columns)

   int x;
   fread(&x, 4,1,f); idxdata->nimages = ntohl( x );
   fread(&x, 4,1,f); idxdata->nrows   = ntohl( x );
   fread(&x, 4,1,f); idxdata->ncols   = ntohl( x );
               
   if( idxdata->nrows != 28 ) return 0;
   if( idxdata->ncols != 28 ) return 0;

   idxdata->length = idxdata->nimages * idxdata->nrows * idxdata->ncols ;
   
   idxdata->data =(unsigned char*) calloc( idxdata->length, 1);
   if( !idxdata->data ) return 0;
   
   fread(idxdata->data, idxdata->length, 1, f);
   
   return 1;
}


#endif  /* __IDX_H_ENTRY_H__ */
