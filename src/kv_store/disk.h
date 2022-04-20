#include "../kv_store.h"

namespace hestia {
namespace kv {

/// @copydoc Kv_store
class Disk : Kv_store {
  public:
    /// @copydoc Kv_store::Kv_store
    Disk(){};
    /// @copydoc Kv_store::Kv_store
    ~Disk(){};

    /// @copydoc Kv_store::object_exists
    bool object_exists(const struct hsm_uint& oid);

    /// @copydoc Kv_store::put_meta_data
    int put_meta_data(const struct hsm_obj& obj);
};

}  // namespace kv
}  // namespace hestia