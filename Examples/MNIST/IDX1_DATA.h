/* 
 * File:   IDX1_DATA.h
 * Author: amind
 *
 * Created on May 7, 2015, 12:46 AM
 */

#ifndef IDX1_DATA_H
#define	IDX1_DATA_H
class IDX1_DATA {
private:
    
public:
    unsigned char header[4];
    int length;
    unsigned char *data;
} ;


#endif	/* IDX1_DATA_H */

