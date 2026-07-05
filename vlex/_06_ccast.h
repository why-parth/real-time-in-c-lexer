

#ifndef _INC_V_CCAST
#define _INC_V_CCAST

#include "_05_fileio.h"



void CC_ccast_charclass_ASCII(void);

#ifdef DYNAMIC_CHARCLASS

void CC_ccast_charclass_ASCII_function(struct CHARCLASS_charclass_view _charclass);

#endif

void CC_ccast_VLU(void);

void CC_ccast_VLU_function(struct VLU_VLU_view _VLU);


void CC_ccast(void);


struct CC_append_charclass_calls {
    #ifdef DYNAMIC_CHARCLASS
    void (* Dynamic)(struct CHARCLASS_charclass_view _charclass);
    #else
    void (* Static)(void);
    #endif
};


struct CC_append_VLU_calls {
    #ifdef STATIC_VLU
    void (* Static)(void);
    #else
    void (* Dynamic)(struct VLU_VLU_view _VLU);
    #endif
};


struct CC_append_tokenlist_calls {
    void (* Static)(void);
    void (* Dynamic)(struct VLU_VLU_view _VLU);
};



#endif