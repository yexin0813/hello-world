#include "bx_ring_queue.h"
#include <string.h>
void *bx_enqueue_position(ring_queue_t *rq)
{
    uint8_t *ptr = rq->buf;
    return ptr + rq->wr_idx*rq->element_size;
}

void bx_enqueue_nocopy(ring_queue_t *rq)
{
    ++rq->count;
    ++rq->wr_idx;
    if(rq->wr_idx==rq->size)
    {
        rq->wr_idx = 0;
    }
}

void bx_enqueue(ring_queue_t *rq,void *element)
{
    uint8_t *ptr = bx_enqueue_position(rq);
    memcpy(ptr,element,rq->element_size);
    bx_enqueue_nocopy(rq);
}

void *bx_dequeue(ring_queue_t *rq)
{
    uint8_t *ptr = rq->buf;
    ptr += rq->rd_idx*rq->element_size;
    --rq->count;
    ++rq->rd_idx;
    if(rq->rd_idx == rq->size)
    {
        rq->rd_idx = 0;
    }
    return ptr;
}

/*
uint16_t bx_ring_queue_array_offset(ring_queue_t *rq,void *element)
{
    uint8_t *ptr =rq->buf;
    ptrdiff_t diff = (uint8_t *)element - ptr;
    return diff/rq->element_size;
}
*/

uint16_t bx_ring_queue_amount(ring_queue_t *rq)
{
        return rq->count;
}

void *bx_ring_queue_glance(ring_queue_t *rq,uint16_t idx)
{
    if(idx>=rq->count)
    {
        return NULL;
    }
    uint8_t *ptr = rq->buf;
    if(idx+rq->rd_idx < rq->size)
    {
        ptr += (idx + rq->rd_idx)*rq->element_size;
    }else
    {
        ptr += (idx + rq->rd_idx - rq->size)*rq->element_size;
    }
    return ptr;
}

bool bx_ring_queue_full(ring_queue_t *rq)
{
    return rq->count == rq->size;
}

bool bx_ring_queue_empty(ring_queue_t *rq)
{
    return rq->count == 0;
}

void bx_ring_queue_flush_all(ring_queue_t *rq)
{
    rq->rd_idx = rq->wr_idx;
    rq->count = 0;
}


