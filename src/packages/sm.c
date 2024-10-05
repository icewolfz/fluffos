#include "../lpc_incl.h"
#include "../comm.h"
#include "../file_incl.h"
#include "../file.h"
#include "../backend.h"
#include "../compiler.h"
#include "../main.h"
#include "../eoperators.h"
#include "../efuns_main.h"
#include "../efun_protos.h"
#include "../simul_efun.h"
#include "../add_action.h"
#include "../array.h"
#include "../master.h"
#include "../port.h"
#include "../array.h"

#ifdef F_RECEIVE_BINARY
void 
f_receive_binary (void)
{
    if (sp->type == T_STRING) {
        if (current_object->interactive) {
            int len = SVALUE_STRLEN(sp);

            if (len > LARGEST_PRINTABLE_STRING)
                error("Printable strings limited to length of %d.\n",
                      LARGEST_PRINTABLE_STRING);
            add_binary_message(current_object, sp->u.string, len);
        }
        free_string_svalue(sp--);
    }
#ifndef NO_BUFFER_TYPE
    else {
        if (current_object->interactive)
            add_binary_message(current_object, (char *)sp->u.buf->item, sp->u.buf->size);
        free_buffer((sp--)->u.buf);
    }
#endif
}
#endif

#ifdef F_SMRANDOM
#include "pcg_basic.h"
//store as only want to init it once
pcg32_random_t smRng;

void f_smrandom (void)
{
    if (sp->u.number <= 0) {
        sp->u.number = 0;
        return;
    }
    static char called = 0;
    //init generator only once
    if (!called) {
        pcg32_srandom_r(&smRng, time(NULL));
        called = 1;
    }
    //return bounded random number
    sp->u.number = pcg32_boundedrand_r(&smRng, sp->u.number);  
}
#endif