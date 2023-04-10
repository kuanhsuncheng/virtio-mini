#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/iov.h"
#include "qemu/timer.h"
#include "qemu-common.h"
#include "hw/virtio/virtio.h"

// allen test
#include "hw/virtio/virtio-allen.h"

#include "sysemu/kvm.h"
#include "sysemu/hax.h"
#include "exec/address-spaces.h"
#include "qapi/error.h"
#include "qapi/qapi-events-misc.h"
#include "qapi/visitor.h"
#include "qemu/error-report.h"

#include "hw/virtio/virtio-bus.h"
#include "hw/virtio/virtio-access.h"
#include "migration/migration.h"

static void virtio_allen_handle_output(VirtIODevice *vdev, VirtQueue *vq)
{
    //VirtIOAllen *s = VIRTIO_ALLEN(vdev);
    VirtQueueElement *elem;
    //MemoryRegionSection section;

    qemu_log("[%s][%d] \n",__FUNCTION__,__LINE__);

    for (;;) {
        size_t offset = 0;
        uint32_t pfn;

        elem = virtqueue_pop(vq, sizeof(VirtQueueElement));
        if (!elem) {
	    qemu_log("no element in VirtQueue ");
            return;
        }

        while (iov_to_buf(elem->out_sg, elem->out_num, offset, &pfn, 4) == 4) {
            int p = virtio_ldl_p(vdev, &pfn);

            offset += 4;
            qemu_log("=========get virtio num:%d \n", p);
        }

        virtqueue_push(vq, elem, offset);
        virtio_notify(vdev, vq);
        g_free(elem);
    }
}

static void virtio_allen_get_config(VirtIODevice *vdev, uint8_t *config_data)
{
    VirtIOAllen *dev = VIRTIO_ALLEN(vdev);
    struct virtio_allen_config config;
    qemu_log("[%s][%d] \n",__FUNCTION__,__LINE__);

    config.actual = cpu_to_le32(dev->actual);
    config.event = cpu_to_le32(dev->event);

    memcpy(config_data, &config, sizeof(struct virtio_allen_config));

}

static void virtio_allen_set_config(VirtIODevice *vdev,
                                    const uint8_t *config_data)
{
    VirtIOAllen *dev = VIRTIO_ALLEN(vdev);
    struct virtio_allen_config config;
    qemu_log("[%s][%d] \n",__FUNCTION__,__LINE__);

    memcpy(&config, config_data, sizeof(struct virtio_allen_config));
    dev->actual = le32_to_cpu(config.actual);
    dev->event = le32_to_cpu(config.event);
}

static uint64_t virtio_allen_get_features(VirtIODevice *vdev, uint64_t f,
                                            Error **errp)
{
    VirtIOAllen *dev = VIRTIO_ALLEN(vdev);
    f |= dev->host_features;
    virtio_add_feature(&f, VIRTIO_ALLEN_F_CAN_PRINT);
    qemu_log("[%s][%d] \n",__FUNCTION__,__LINE__);

    return f;
}

static int virtio_allen_post_load_device(void *opaque, int version_id)
{
    //VirtIOAllen *s = VIRTIO_ALLEN(opaque);
    qemu_log("[%s][%d] \n",__FUNCTION__,__LINE__);

    return 0;
}

static const VMStateDescription vmstate_virtio_allen_device = {
    .name = "virtio-allen-device",
    .version_id = 1,
    .minimum_version_id = 1,
    .post_load = virtio_allen_post_load_device,
    .fields = (VMStateField[]) {
        VMSTATE_UINT32(actual, VirtIOAllen),
        VMSTATE_END_OF_LIST()
    },
};

static void allen_stats_change_timer(VirtIOAllen *s, int64_t secs)
{
    timer_mod(s->stats_timer, qemu_clock_get_ms(QEMU_CLOCK_VIRTUAL) + secs * 1000);
}

static void allen_stats_poll_cb(void *opaque)
{
    VirtIOAllen *s = opaque;
    VirtIODevice *vdev = VIRTIO_DEVICE(s);
    qemu_log("[%s][%d]  set config (+1) :%d \n",__FUNCTION__,__LINE__, s->set_config++);

    virtio_notify_config(vdev);
    allen_stats_change_timer(s, 1);
}

static void virtio_allen_device_realize(DeviceState *dev, Error **errp)
{
    VirtIODevice *vdev = VIRTIO_DEVICE(dev);
    VirtIOAllen *s = VIRTIO_ALLEN(dev);
    //int ret;
    qemu_log("[%s][%d] \n",__FUNCTION__,__LINE__);

    // Allen test
    virtio_init(vdev, "virtio-allen", VIRTIO_ID_ALLEN,
                sizeof(struct virtio_allen_config));

    s->ivq = virtio_add_queue(vdev, 128, virtio_allen_handle_output);

    /* create a new timer */
    g_assert(s->stats_timer == NULL);
    s->stats_timer = timer_new_ms(QEMU_CLOCK_VIRTUAL, allen_stats_poll_cb, s);
    allen_stats_change_timer(s, 30);
}

static void virtio_allen_device_unrealize(DeviceState *dev, Error **errp)
{
    VirtIODevice *vdev = VIRTIO_DEVICE(dev);
    //VirtIOAllen *s = VIRTIO_ALLEN(dev);
    qemu_log("[%s][%d] \n",__FUNCTION__,__LINE__);

    virtio_cleanup(vdev);
}

static void virtio_allen_device_reset(VirtIODevice *vdev)
{
    //VirtIOAllen *s = VIRTIO_ALLEN(vdev);
    qemu_log("[%s][%d] \n",__FUNCTION__,__LINE__);

}

static void virtio_allen_set_status(VirtIODevice *vdev, uint8_t status)
{
    //VirtIOAllen *s = VIRTIO_ALLEN(vdev);
    qemu_log("[%s][%d] \n",__FUNCTION__,__LINE__);

}


static const VMStateDescription vmstate_virtio_allen_vm = {
    .name = "virtio-allen",
    .minimum_version_id = 1,
    .version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_VIRTIO_DEVICE,
        VMSTATE_END_OF_LIST()
    },
};

static Property virtio_allen_properties[] = {
    DEFINE_PROP_END_OF_LIST(),
};

static void virtio_allen_instance_init(Object *obj)
{
    //VirtIOAllen *s = VIRTIO_ALLEN(obj);

    qemu_log("[%s][%d] \n",__FUNCTION__,__LINE__);
}

static void virtio_allen_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    VirtioDeviceClass *vdc = VIRTIO_DEVICE_CLASS(klass);

    // allen
    dc->props = virtio_allen_properties;

    dc->vmsd = &vmstate_virtio_allen_vm;
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);


    // allen
    vdc->realize = virtio_allen_device_realize;
    vdc->unrealize = virtio_allen_device_unrealize;
    vdc->reset = virtio_allen_device_reset;
    vdc->get_config = virtio_allen_get_config;
    vdc->set_config = virtio_allen_set_config;

    vdc->get_features = virtio_allen_get_features;
    vdc->set_status = virtio_allen_set_status;
    vdc->vmsd = &vmstate_virtio_allen_device;
}


static const TypeInfo virtio_allen_info = {
    .name = TYPE_VIRTIO_ALLEN,
    .parent = TYPE_VIRTIO_DEVICE,
    .instance_size = sizeof(VirtIOAllen),
    .instance_init = virtio_allen_instance_init,
    .class_init = virtio_allen_class_init,
};

static void virtio_register_types(void)
{
    type_register_static(&virtio_allen_info);
}

type_init(virtio_register_types)

