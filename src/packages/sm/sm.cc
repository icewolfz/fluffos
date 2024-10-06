#include "base/package_api.h"
#include <event2/buffer.h>       // for evbuffer_freeze, etc
#include <event2/bufferevent.h>  // for bufferevent_enable, etc
#include "net/websocket.h"
#include "thirdparty/libtelnet/libtelnet.h"
#include "net/ws_telnet.h"

#ifdef F_RECEIVE_BINARY
void  f_receive_binary (void)
{
    interactive_t *ip = current_object->interactive;
    if(!ip) return;
    if (sp->type == T_STRING) {     
        int len = SVALUE_STRLEN(sp);  
        if (len > LARGEST_PRINTABLE_STRING)
            error("Printable strings limited to length of %d.\n", LARGEST_PRINTABLE_STRING);
        if(ip->connection_type == PORT_TYPE_WEBSOCKET)  {
            if(lws_get_protocol(ip->lws)->id == PROTOCOL_WS_TELNET)
                telnet_send(ip->telnet, sp->u.string, len);
            else
                websocket_send_text(ip->lws, sp->u.string, len);
        }
        else if(ip->connection_type == PORT_TYPE_TELNET)
            telnet_send(ip->telnet, sp->u.string, len);
        else
            bufferevent_write(ip->ev_buffer, sp->u.string, len);                
        free_string_svalue(sp--);
    }
    else if (sp->type == T_BUFFER) {
        if(ip->connection_type == PORT_TYPE_WEBSOCKET) {
            if(lws_get_protocol(ip->lws)->id == PROTOCOL_WS_TELNET)
                telnet_send(ip->telnet, reinterpret_cast<char *>(sp->u.buf->item), sp->u.buf->size);
            else
                websocket_send_text(ip->lws, reinterpret_cast<char *>(sp->u.buf->item), sp->u.buf->size);
        }
        else if(ip->connection_type == PORT_TYPE_TELNET)
            telnet_send(ip->telnet, reinterpret_cast<char *>(sp->u.buf->item), sp->u.buf->size);
        else            
            bufferevent_write(ip->ev_buffer, reinterpret_cast<char *>(sp->u.buf->item), sp->u.buf->size);
       free_buffer((sp--)->u.buf);
    }
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