/* QEMU_VIRTIO_ALLEN_H */

#ifndef QEMU_VIRTIO_ALLEN_H
#define QEMU_VIRTIO_ALLEN_H

#include "standard-headers/linux/virtio_allen.h"
#include "hw/virtio/virtio.h"
#include "hw/pci/pci.h"

#define TYPE_VIRTIO_ALLEN "virtio-allen-device"
#define VIRTIO_ALLEN(obj) \
        OBJECT_CHECK(VirtIOAllen, (obj), TYPE_VIRTIO_ALLEN)

typedef struct VirtIOAllen {
    VirtIODevice parent_obj;
    VirtQueue *ivq;
    uint32_t set_config;
    uint32_t actual;
    VirtQueueElement *stats_vq_elem;
    size_t stats_vq_offset;
    QEMUTimer *stats_timer;
    uint32_t host_features;
    uint32_t event;
} VirtIOAllen;

#endif
