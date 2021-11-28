#include <param_helper/params.hpp>
#include "catch2/catch.hpp"

// ToDo: To be continued...

using namespace param_helper::params;


TEST_CASE( "add_and_get_entry", "[parameters]" ){
  Parameters params = Parameters::create_by_params(json {{"a", 0}, {"vec", std::vector<double> {0.0, 1.0}}});

  REQUIRE(params.get_entry<double>("a") == 0);

  params.add_entry("c", "test_c");

  REQUIRE(params.get_entry<std::string>("c") == "test_c");
}