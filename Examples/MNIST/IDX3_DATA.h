/* 
 * File:   IDX3_DATA.h
 * Author: amind
 *
 * Created on May 7, 2015, 12:48 AM
 */

#ifndef IDX3_DATA_H
#define	IDX3_DATA_H


class IDX3_DATA{
private:
    
public:
    unsigned char header[4];
    int nimages;
    int nrows;
    int ncols;
    int length;
    unsigned char *data;
} ;

#endif	/* IDX3_DATA_H */

