#include "base/package_api.h"
#include <event2/buffer.h>       // for evbuffer_freeze, etc
#include <event2/bufferevent.h>  // for bufferevent_enable, etc
#include "net/websocket.h"

#ifdef F_RECEIVE_BINARY
void  f_receive_binary (void)
{
    if (sp->type == T_STRING) {
        if (current_object->interactive) {
            int len = SVALUE_STRLEN(sp);

            if (len > LARGEST_PRINTABLE_STRING)
                error("Printable strings limited to length of %d.\n", LARGEST_PRINTABLE_STRING);
            if(current_object->interactive->connection_type == PORT_TYPE_WEBSOCKET)
                websocket_send_text(current_object->interactive->lws, sp->u.string, len);
            else
                bufferevent_write(current_object->interactive->ev_buffer, sp->u.string, len);                
        }
        free_string_svalue(sp);
    }
#ifndef NO_BUFFER_TYPE
    else {
        if (current_object->interactive) {
            if(current_object->interactive->connection_type == PORT_TYPE_WEBSOCKET)
                websocket_send_text(current_object->interactive->lws, (char *)sp->u.buf->item, sp->u.buf->size);
            else            
                bufferevent_write(current_object->interactive->ev_buffer, (char *)sp->u.buf->item, sp->u.buf->size);
        }
        free_buffer((sp)->u.buf);
    }
#endif
}
#endif

/*
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
*/