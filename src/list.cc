#include "../test/kv_store/disk.h"
#include "hestia.h"

std::vector<struct hestia::hsm_uint> hestia::list(const std::uint8_t tier)
{
    kv::Disk kv_store;

    return kv_store.list(tier);
}
