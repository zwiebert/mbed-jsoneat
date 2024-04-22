#include <unity.h>

#include <jsmn/jsmn_iterate.hh>
char json[] = R"({"json":{"auto":{"adapter.0":{"name":"Neutral","flags":{"exists":1,"neutral":1,"read_only":1},"temp":0.06,"wind":0.001,"humi":0.01,"clouds":0.01}}}})";
char json3[] = R"({"json":{}})";

void test_td() {
  auto jsmn = Jsmn<64, char *>(json);
  TEST_ASSERT_FALSE(!jsmn);
  auto it = jsmn.begin();

++it;
TEST_ASSERT_TRUE(it.keyStartsWith("jso"));
TEST_ASSERT_FALSE(it.keyStartsWith("gso"));
TEST_ASSERT_FALSE(!it);
it.skip_key_and_value();
TEST_ASSERT_TRUE(!it);
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_td);

  return UNITY_END();
}

