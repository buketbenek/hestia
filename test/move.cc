#include <catch2/catch.hpp>
#include <hestia.h>
#include <string>

#include "common.h"

SCENARIO(
    "Move interfaces correctly with the object store backend",
    "[object][store]")
{
    GIVEN("an object within the object store")
    {
        auto data_vec = GENERATE(
            chunk(max_data_size, take(max_data_size, random(' ', 'z'))));

        std::string data(data_vec.begin(), data_vec.end());

        auto hsm_uint_parts = GENERATE(chunk(
            2, take(
                   2, random(
                          std::numeric_limits<std::uint64_t>::min(),
                          std::numeric_limits<std::uint64_t>::max()))));
        struct hestia::hsm_uint oid(hsm_uint_parts[0], hsm_uint_parts[1]);
        struct hestia::hsm_obj obj;

        hestia::put(oid, &obj, false, data.data(), 0, data.size(), 0);

        const auto json_attrs =
            nlohmann::json::parse(hestia::get_attrs(oid, "tier"));
        const int dest_tier = json_attrs["tier"];

        WHEN("the data is moved to a different tier")
        {
            std::string original_data;
            original_data.resize(data.size());
            hestia::get(
                oid, &obj, &original_data[0], 0, original_data.size(), 0,
                dest_tier);

            hestia::set_attrs(
                oid,
                R"({"trigger_migration": {"operation":"move", "src_tier":0,"tgt_tier":1}})");


            THEN(
                "The data retrieved from the original object above matches the data retrieved from the move")
            {

                const auto json_attrs =
                    nlohmann::json::parse(hestia::get_attrs(oid, "tier"));
                const int tgt_tier = json_attrs["tier"];
                std::string move_data;
                move_data.resize(data.size());
                hestia::get(
                    oid, &obj, &move_data[0], 0, move_data.size(), 0, tgt_tier);
                REQUIRE(original_data == move_data);
            }
        }
    }
    /*
    WHEN("offset data is sent to the object store")
    {
        auto hsm_uint_parts = GENERATE(chunk(
            2, take(
                   2, random(
                          std::numeric_limits<std::uint64_t>::min(),
                          std::numeric_limits<std::uint64_t>::max()))));
        struct hestia::hsm_uint oid(hsm_uint_parts[0], hsm_uint_parts[1]);
        struct hestia::hsm_obj obj;

        const std::size_t offset = GENERATE_COPY(take(
            1,
            random(
                std::numeric_limits<std::size_t>::min() + 1, data.size() - 1)));
        const std::size_t length = GENERATE_COPY(take(
            1, random(
                   std::numeric_limits<std::size_t>::min() + 1,
                   data.size() - offset)));

        hestia::put(oid, &obj, false, data.data(), offset, length, 0);

        const auto json_attrs =
            nlohmann::json::parse(hestia::get_attrs(oid, "tier"));
        const int dest_tier = json_attrs["tier"];

        THEN("a hsm_obj is created")
        {
            REQUIRE(obj.oid == oid);
            REQUIRE(!obj.meta_data.empty());
        }
        WHEN("the data is retrieved")
        {
            std::string recv_data;
            recv_data.resize(data.size());
            hestia::get(oid, &obj, &recv_data[0], offset, length, 0, dest_tier);

            THEN("the stored data matches the retrieved data")
            {
                REQUIRE(
                    recv_data.substr(offset, length)
                    == data.substr(offset, length));
            }
        }
    }
}*/
}