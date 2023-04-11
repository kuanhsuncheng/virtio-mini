#ifndef _LINUX_VIRTIO_ALLEN_H
#define _LINUX_VIRTIO_ALLEN_H
#include "standard-headers/linux/types.h"
#include "standard-headers/linux/virtio_types.h"
#include "standard-headers/linux/virtio_ids.h"
#include "standard-headers/linux/virtio_config.h"

/* The feature bitmap for virtio balloon */
#define VIRTIO_ALLEN_F_CAN_PRINT	0

struct virtio_allen_config {
	uint32_t num_pages;
	uint32_t actual;
	uint32_t event;
};

struct virtio_allen_stat {
	__virtio16 tag;
	__virtio64 val;
} QEMU_PACKED;

#endif /* _LINUX_VIRTIO_ALLEN_H */
