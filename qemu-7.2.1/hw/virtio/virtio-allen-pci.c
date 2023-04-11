/*
 * Virtio allen PCI Bindings
 *
 * Copyright IBM, Corp. 2007
 * Copyright (c) 2009 CodeSourcery
 *
 * Authors:
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *  Paul Brook        <paul@codesourcery.com>
 *
 * Contributions after 2012-01-13 are licensed under the terms of the
 * GNU GPL, version 2 or (at your option) any later version.
 */

#include "qemu/osdep.h"

#include "hw/virtio/virtio-pci.h"
#include "hw/qdev-properties.h"
#include "hw/virtio/virtio-allen.h"
#include "qapi/error.h"
#include "qemu/module.h"
#include "qom/object.h"

typedef struct VirtIOAllenPCI VirtIOAllenPCI;

/*
 * virtio-allen-pci: This extends VirtioPCIProxy.
 */
#define TYPE_VIRTIO_ALLEN_PCI "virtio-allen-pci-base"
DECLARE_INSTANCE_CHECKER(VirtIOAllenPCI, VIRTIO_ALLEN_PCI,
                         TYPE_VIRTIO_ALLEN_PCI)

struct VirtIOAllenPCI {
    VirtIOPCIProxy parent_obj;
    VirtIOAllen vdev;
};

static void virtio_allen_pci_realize(VirtIOPCIProxy *vpci_dev, Error **errp)
{
    VirtIOAllenPCI *dev = VIRTIO_ALLEN_PCI(vpci_dev);
    DeviceState *vdev = DEVICE(&dev->vdev);

    vpci_dev->class_code = PCI_CLASS_OTHERS;
    qdev_realize(vdev, BUS(&vpci_dev->bus), errp);
}

static void virtio_allen_pci_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    VirtioPCIClass *k = VIRTIO_PCI_CLASS(klass);
    PCIDeviceClass *pcidev_k = PCI_DEVICE_CLASS(klass);
    k->realize = virtio_allen_pci_realize;
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
    pcidev_k->vendor_id = PCI_VENDOR_ID_REDHAT_QUMRANET;
    pcidev_k->device_id = PCI_DEVICE_ID_VIRTIO_ALLEN;
    pcidev_k->revision = VIRTIO_PCI_ABI_VERSION;
    pcidev_k->class_id = PCI_CLASS_OTHERS;
}

static void virtio_allen_pci_instance_init(Object *obj)
{
    VirtIOAllenPCI *dev = VIRTIO_ALLEN_PCI(obj);

    virtio_instance_init_common(obj, &dev->vdev, sizeof(dev->vdev),
                                TYPE_VIRTIO_ALLEN);
}

static const VirtioPCIDeviceTypeInfo virtio_allen_pci_info = {
    .base_name             = TYPE_VIRTIO_ALLEN_PCI,
    .generic_name          = "virtio-allen-pci",
    .transitional_name     = "virtio-allen-pci-transitional",
    .non_transitional_name = "virtio-allen-pci-non-transitional",
    .instance_size = sizeof(VirtIOAllenPCI),
    .instance_init = virtio_allen_pci_instance_init,
    .class_init    = virtio_allen_pci_class_init,
};

static void virtio_allen_pci_register(void)
{
    virtio_pci_types_register(&virtio_allen_pci_info);
}

type_init(virtio_allen_pci_register)
