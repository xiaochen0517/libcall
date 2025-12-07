#include <catch2/catch_test_macros.hpp>

#include "adapter/runner.hpp"

TEST_CASE("Test base type literal calls", "[method]")
{
    SECTION("Load file and parse")
    {
        // 创建配置
        RunParam param;
        param.json_path = "./test_configs/base_type/literal_int_call.json";
        // 运行配置
        Runner runner = Runner::create();
        runner.run(param);
    }
}
