#include <gtest/gtest.h>
#include <json.hpp>

using json = nlohmann::json;

TEST( json , usage ){
    bool val = true;
    bool nv = not val;
    json j2= {"pi", 3.141};
    j2 = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {
          {"everything", 42}
        }},
        {"list", {1, 0, 2}},
        {"object", {
          {"currency", "USD"},
          {"value", 42.99}
        }}
      };
}