#include "GlipTcp.h"

extern "C" {

void* glip_tcp_create(int tcp_port, int width) {
    GlipTcp *obj = new GlipTcp(tcp_port, width);

    return (void*) obj;
}

int glip_tcp_reset(void* obj) {
    GlipTcp *inst = (GlipTcp*) obj;
    return inst->reset();
}

int glip_tcp_connected(void* obj) {
    GlipTcp *inst = (GlipTcp*) obj;
    return inst->connected() ? 1 : 0;
}

uint32_t glip_tcp_next_cycle(void* obj) {
    GlipTcp *inst = (GlipTcp*) obj;
    return inst->next_cycle();
}

uint32_t glip_tcp_control_msg(void* obj) {
    GlipTcp *inst = (GlipTcp*) obj;
    return inst->control_msg();
}

uint64_t glip_tcp_read(void* obj) {
    GlipTcp *inst = (GlipTcp*) obj;
    return inst->readData();
}

void glip_tcp_read_ack(void* obj) {
    GlipTcp *inst = (GlipTcp*) obj;
    inst->readAck();
}

void glip_tcp_write(void* obj, uint64_t data) {
    GlipTcp *inst = (GlipTcp*) obj;
    inst->writeData(data);
}

}
