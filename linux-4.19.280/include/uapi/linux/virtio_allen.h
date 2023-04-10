#ifndef _LINUX_VIRTIO_ALLEN_H
#define _LINUX_VIRTIO_ALLEN_H

#include "linux/types.h"
#include "linux/virtio_types.h"
#include "linux/virtio_ids.h"
#include "linux/virtio_config.h"

/* The feature bitmap for virtio balloon */
#define VIRTIO_ALLEN_F_CAN_PRINT        0

struct virtio_allen_config {
        uint32_t num_pages;
        uint32_t actual;
        uint32_t event;
};

struct virtio_allen_stat {
        __virtio16 tag;
        __virtio64 val;
} __attribute__((packed));

#endif /* _LINUX_VIRTIO_ALLEN_H */

