#include "../../src/object_store.h"

namespace hestia {
namespace obj {

/// @copydoc Object_store
class Disk : Object_store {
  public:
    /// @copydoc Object_store::Object_store
    Disk(){};
    /// @copydoc Object_store::Object_store
    ~Disk(){};

    /// @copydoc Object_store::put
    int put(
        const struct hsm_uint& oid,
        const void* buf,
        const std::size_t length,
        const std::uint8_t target_tier = 0);

    /// @copydoc Object_store::get
    int get(
        const struct hsm_uint& oid,
        void* buf,
        const std::size_t length,
        const std::uint8_t src_tier = 0);

    /// @copydoc Object_store::remove
    int remove(const struct hsm_uint& oid, const std::uint8_t tier = 0);

  private:
    std::string get_filename_from_oid(
        const struct hsm_uint& oid, const std::uint8_t tier = 0) const
    {
        return std::to_string(tier) + '-' + std::to_string(oid.higher)
               + std::to_string(oid.lower) + ".data";
    }
};

}  // namespace obj
}  // namespace hestia
