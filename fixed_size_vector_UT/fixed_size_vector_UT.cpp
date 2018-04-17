#include "stdafx.h"

#include <list>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace fixed_size_vector_UT {
struct NonCopyable {
  NonCopyable() = default;
  NonCopyable(const NonCopyable &) = delete;
  NonCopyable(NonCopyable &&) = default;
  NonCopyable &operator=(const NonCopyable &) = delete;
  NonCopyable &operator=(NonCopyable &&) = default;
};
struct ObjectCouter {
  static std::size_t constructed;
  static std::size_t arg_constructed;
  static std::size_t copy_constructed;
  static std::size_t move_constructed;
  static std::size_t copy_assigned;
  static std::size_t move_assigned;
  static std::size_t destructed;
  static void reset() {
    constructed = 0;
    arg_constructed = 0;
    copy_constructed = 0;
    move_constructed = 0;
    copy_assigned = 0;
    move_assigned = 0;
    destructed = 0;
  }
  static std::size_t sum() {
    return constructed + arg_constructed + copy_constructed + move_constructed +
           copy_assigned + move_assigned;
  }
  ObjectCouter() { ++constructed; }
  ObjectCouter(int) { ++arg_constructed; }
  ObjectCouter(const ObjectCouter &) { ++copy_constructed; }
  ObjectCouter(ObjectCouter &&) noexcept { ++move_constructed; }
  ObjectCouter &operator=(ObjectCouter &&) noexcept {
    ++move_assigned;
    return *this;
  }
  ObjectCouter &operator=(const ObjectCouter &) noexcept {
    ++copy_assigned;
    return *this;
  }
  ~ObjectCouter() { ++destructed; }
};
std::size_t ObjectCouter::constructed;
std::size_t ObjectCouter::arg_constructed;
std::size_t ObjectCouter::copy_constructed;
std::size_t ObjectCouter::move_constructed;
std::size_t ObjectCouter::copy_assigned;
std::size_t ObjectCouter::move_assigned;
std::size_t ObjectCouter::destructed;

TEST_CLASS(fixed_size_vector){
    TEST_METHOD(DefaultConstructor){utils::fixed_size_vector<int, 10> sut;
}  // namespace fixed_size_vector_UT
TEST_METHOD(initializerListConstructor) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(1, sut[0]);
  Assert::AreEqual(2, sut[1]);
  Assert::AreEqual(3, sut[2]);
  Assert::AreEqual(std::size_t(3), sut.size());
}
TEST_METHOD(capacity) {
  utils::fixed_size_vector<int, 10> sut;
  Assert::AreEqual(std::size_t(10u), sut.capacity());
}
TEST_METHOD(max_size) {
  utils::fixed_size_vector<int, 10> sut;
  Assert::AreEqual(std::size_t(10u), sut.max_size());
}
TEST_METHOD(SizeOnEmptyVector) {
  utils::fixed_size_vector<int, 10> sut;
  Assert::AreEqual(std::size_t(0u), sut.size());
}
TEST_METHOD(pushBack) {
  utils::fixed_size_vector<int, 10> sut;
  sut.push_back(42);
  Assert::AreEqual(std::size_t(1u), sut.size());
}
TEST_METHOD(arrayOperator) {
  utils::fixed_size_vector<int, 10> sut;
  sut.push_back(42);
  Assert::AreEqual(42, sut[0]);
}
TEST_METHOD(begin) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(&sut[0], sut.begin());
}
TEST_METHOD(cbegin) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(&sut[0], sut.cbegin());
}
TEST_METHOD(begin_on_const) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(&sut[0], sut.begin());
}
TEST_METHOD(end) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(&sut[3], sut.end());
}
TEST_METHOD(end_on_const) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(&sut[3], sut.end());
}
TEST_METHOD(cend) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(&sut[3], sut.cend());
}
TEST_METHOD(constArrayOperator) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(1, sut[0]);
  Assert::AreEqual(2, sut[1]);
  Assert::AreEqual(3, sut[2]);
  constexpr std::size_t expected{3};
  Assert::AreEqual(expected, sut.size());
}
TEST_METHOD(Membertypes_value_type) {
  using sutType = utils::fixed_size_vector<int, 10>;
  Assert::IsTrue(std::is_same_v<sutType::value_type, int>);
}
TEST_METHOD(Membertypes_size_type) {
  using sutType = utils::fixed_size_vector<int, 10>;
  Assert::IsTrue(std::is_same_v<sutType::size_type, std::size_t>);
}
TEST_METHOD(Membertypes_iterator) {
  using sutType = utils::fixed_size_vector<int, 10>;
  Assert::IsTrue(std::is_same_v<sutType::iterator, int *>);
}
TEST_METHOD(Membertypes_const_iterator) {
  using sutType = utils::fixed_size_vector<int, 10>;
  Assert::IsTrue(std::is_same_v<sutType::const_iterator, const int *>);
}
TEST_METHOD(Membertypes_reference) {
  using sutType = utils::fixed_size_vector<int, 10>;
  Assert::IsTrue(std::is_same_v<sutType::reference, int &>);
}
TEST_METHOD(Membertypes_const_reference) {
  using sutType = utils::fixed_size_vector<int, 10>;
  Assert::IsTrue(std::is_same_v<sutType::const_reference, const int &>);
}
TEST_METHOD(Membertypes_pointer) {
  using sutType = utils::fixed_size_vector<int, 10>;
  Assert::IsTrue(std::is_same_v<sutType::pointer, int *>);
}
TEST_METHOD(Membertypes_const_pointer) {
  using sutType = utils::fixed_size_vector<int, 10>;
  Assert::IsTrue(std::is_same_v<sutType::const_pointer, const int *>);
}
TEST_METHOD(range_based_for_loop) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  int sum{0};
  for (auto &i : sut) sum += i;
  Assert::AreEqual(6, sum);
}
TEST_METHOD(range_based_for_loop_on_const) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  int sum{0};
  for (const auto &i : sut) sum += i;
  Assert::AreEqual(6, sum);
}
TEST_METHOD(std_begin) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(std::begin(sut), sut.begin());
}
TEST_METHOD(std_begin_on_const) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(std::begin(sut), sut.begin());
}
TEST_METHOD(std_cbegin) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(std::cbegin(sut), sut.cbegin());
}
TEST_METHOD(std_end) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(std::end(sut), sut.end());
}
TEST_METHOD(std_end_on_const) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(std::end(sut), sut.end());
}
TEST_METHOD(std_cend) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(std::cend(sut), sut.cend());
}
TEST_METHOD(front) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(1, sut.front());
}
TEST_METHOD(front_on_const) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(1, sut.front());
}
TEST_METHOD(back) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(3, sut.back());
}
TEST_METHOD(back_on_const) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(3, sut.back());
}
TEST_METHOD(at) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(3, sut.at(2));
}
TEST_METHOD(at_on_const) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(2, sut.at(1));
}
TEST_METHOD(at_throws) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::ExpectException<std::out_of_range>([&]() { sut.at(3); });
}
TEST_METHOD(at_on_const_throws) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::ExpectException<std::out_of_range>([&]() { sut.at(3); });
}
TEST_METHOD(empty_on_empty_vector) {
  utils::fixed_size_vector<int, 10> sut;
  Assert::IsTrue(sut.empty());
}
TEST_METHOD(empty_on_non_empty_vector) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::IsFalse(sut.empty());
}
TEST_METHOD(arrayOperatorWithNonTrivialType) {
  utils::fixed_size_vector<std::string, 10> sut{"a", "b", "c"};
  Assert::IsFalse(sut.empty());
}
TEST_METHOD(data) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  auto data_ptr = sut.data();
  Assert::AreEqual(&sut[0], data_ptr);
}
TEST_METHOD(insert_in_end) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  auto pos = sut.end();
  sut.insert(pos, 10);
  Assert::AreEqual(std::size_t(4), sut.size());
  Assert::AreEqual(1, sut[0]);
  Assert::AreEqual(2, sut[1]);
  Assert::AreEqual(3, sut[2]);
  Assert::AreEqual(10, sut[3]);
}
TEST_METHOD(insert_in_begin) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  auto pos = sut.begin();
  sut.insert(pos, 10);
  Assert::AreEqual(std::size_t(4), sut.size());
  Assert::AreEqual(10, sut[0]);
  Assert::AreEqual(1, sut[1]);
  Assert::AreEqual(2, sut[2]);
  Assert::AreEqual(3, sut[3]);
}

TEST_METHOD(insert_in_end_with_counter) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut{1, 2, 3};
  Assert::AreEqual(std::size_t(6), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  // TODO: check if it can be done better
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  auto pos = sut.end();
  ObjectCouter oc;
  Assert::AreEqual(std::size_t(7), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  sut.insert(pos, oc);
  Assert::AreEqual(std::size_t(8), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  Assert::AreEqual(std::size_t(1), ObjectCouter::copy_assigned);
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
}
TEST_METHOD(insert_in_end_with_counter_and_rvalue) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut{1, 2, 3};
  Assert::AreEqual(std::size_t(6), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  // TODO: check if it can be done better
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  auto pos = sut.end();
  sut.insert(pos, ObjectCouter{});
  Assert::AreEqual(std::size_t(8), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  Assert::AreEqual(std::size_t(1), ObjectCouter::move_assigned);
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
}
TEST_METHOD(insert_in_the_middle_with_counter) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut{1, 2, 3};
  Assert::AreEqual(std::size_t(6), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  // TODO: check if it can be done better
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  auto pos = sut.begin() + 1;
  ObjectCouter oc;
  Assert::AreEqual(std::size_t(7), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  sut.insert(pos, oc);
  Assert::AreEqual(std::size_t(10), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  Assert::AreEqual(std::size_t(2), ObjectCouter::move_assigned);
  Assert::AreEqual(std::size_t(1), ObjectCouter::copy_assigned);
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
}
TEST_METHOD(insert_in_the_middle_with_counter_with_rvalue) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut{1, 2, 3};
  Assert::AreEqual(std::size_t(6), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  // TODO: check if it can be done better
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  auto pos = sut.begin() + 1;
  sut.insert(pos, ObjectCouter{});
  Assert::AreEqual(std::size_t(10), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  Assert::AreEqual(std::size_t(3), ObjectCouter::move_assigned);
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
}
TEST_METHOD(insert_throws_bad_alloc) {
  utils::fixed_size_vector<int, 3> sut{1, 2, 3};
  Assert::ExpectException<std::bad_alloc>([&]() { sut.insert(sut.end(), 10); });
}
TEST_METHOD(insert_in_the_middle) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  auto pos = sut.begin() + 1;
  sut.insert(pos, 10);
  Assert::AreEqual(std::size_t(4), sut.size());
  Assert::AreEqual(1, sut[0]);
  Assert::AreEqual(10, sut[1]);
  Assert::AreEqual(2, sut[2]);
  Assert::AreEqual(3, sut[3]);
}
TEST_METHOD(erase_from_back) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  auto pos = sut.end() - 1;
  auto retVal = sut.erase(pos);
  Assert::AreEqual(retVal, sut.end());
  Assert::AreEqual(std::size_t(2), sut.size());
  Assert::AreEqual(1, sut[0]);
  Assert::AreEqual(2, sut[1]);
}
TEST_METHOD(erase_from_begin) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  auto pos = sut.begin();
  auto retVal = sut.erase(pos);
  Assert::AreEqual(retVal, sut.begin());
  Assert::AreEqual(std::size_t(2), sut.size());
  Assert::AreEqual(2, sut[0]);
  Assert::AreEqual(3, sut[1]);
}
TEST_METHOD(erase_from_the_middle) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  auto pos = sut.begin() + 1;
  auto retVal = sut.erase(pos);
  Assert::AreEqual(retVal, sut.begin() + 1);
  Assert::AreEqual(std::size_t(2), sut.size());
  Assert::AreEqual(1, sut[0]);
  Assert::AreEqual(3, sut[1]);
}
TEST_METHOD(data_on_const) {
  const utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  auto data_ptr = sut.data();
  Assert::AreEqual(&sut[0], data_ptr);
}
TEST_METHOD(pushBackNonCopyable) {
  utils::fixed_size_vector<NonCopyable, 10> sut;
  sut.push_back(NonCopyable{});
  Assert::AreEqual(std::size_t(1), sut.size());
}
TEST_METHOD(pushBackObjectCounterWithRValue) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  Assert::AreEqual(std::size_t(0), ObjectCouter::sum());
  sut.push_back(ObjectCouter{});
  Assert::AreEqual(std::size_t(1), sut.size());
  Assert::AreEqual(std::size_t(2), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::move_constructed);
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
}
TEST_METHOD(pushBackObjectCounterWithLValueRef) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  Assert::AreEqual(std::size_t(0), ObjectCouter::sum());
  ObjectCouter oc;
  Assert::AreEqual(std::size_t(1), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
  sut.push_back(oc);
  Assert::AreEqual(std::size_t(1), sut.size());
  Assert::AreEqual(std::size_t(2), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
  Assert::AreEqual(std::size_t(1), ObjectCouter::copy_constructed);
}
TEST_METHOD(emplaceBackObjectCounterWithCtorArgs) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  Assert::AreEqual(std::size_t(0), ObjectCouter::sum());

  sut.emplace_back(1);
  Assert::AreEqual(std::size_t(1), sut.size());
  Assert::AreEqual(std::size_t(1), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::arg_constructed);
}
TEST_METHOD(emplaceBackObjectCounterWithRValue) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  Assert::AreEqual(std::size_t(0), ObjectCouter::sum());

  sut.emplace_back(ObjectCouter{});
  Assert::AreEqual(std::size_t(1), sut.size());
  Assert::AreEqual(std::size_t(2), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
  Assert::AreEqual(std::size_t(1), ObjectCouter::move_constructed);
}
TEST_METHOD(emplaceBackObjectCounterWithLValueRef) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  Assert::AreEqual(std::size_t(0), ObjectCouter::sum());
  ObjectCouter oc;
  Assert::AreEqual(std::size_t(1), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);

  sut.emplace_back(oc);
  Assert::AreEqual(std::size_t(1), sut.size());
  Assert::AreEqual(std::size_t(2), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(1), ObjectCouter::constructed);
  Assert::AreEqual(std::size_t(1), ObjectCouter::copy_constructed);
}
TEST_METHOD(destructor) {
  ObjectCouter::reset();
  {
    utils::fixed_size_vector<ObjectCouter, 10> sut;
    Assert::AreEqual(std::size_t(0), ObjectCouter::sum());
    sut.emplace_back(1);
    sut.emplace_back(2);
    sut.emplace_back(3);
    Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
    Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  }
  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);

  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::destructed);
}

TEST_METHOD(clear) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  Assert::AreEqual(std::size_t(0), ObjectCouter::sum());
  sut.emplace_back(1);
  sut.emplace_back(2);
  sut.emplace_back(3);
  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  sut.clear();
  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);

  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::destructed);
}
TEST_METHOD(copy_ctor_trivial_type) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  auto copy{sut};
  Assert::AreEqual(1, copy[0]);
  Assert::AreEqual(2, copy[1]);
  Assert::AreEqual(3, copy[2]);
}
TEST_METHOD(copy_ctor_object_counter) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  sut.emplace_back(1);
  sut.emplace_back(2);
  sut.emplace_back(3);
  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  auto copy{sut};
  Assert::AreEqual(std::size_t(6), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
}
TEST_METHOD(move_ctor_trivial_type) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  auto copy{std::move(sut)};
  Assert::AreEqual(1, copy[0]);
  Assert::AreEqual(2, copy[1]);
  Assert::AreEqual(3, copy[2]);
}
TEST_METHOD(move_ctor_object_counter) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  sut.emplace_back(1);
  sut.emplace_back(2);
  sut.emplace_back(3);
  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  auto copy{std::move(sut)};
  Assert::AreEqual(std::size_t(6), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::move_constructed);
}
TEST_METHOD(copy_ctor_std_string) {
  utils::fixed_size_vector<std::string, 10> sut{"123", "456", "789"};
  auto copy(sut);
  copy[0] = "abc";
  Assert::AreEqual("123", sut[0].c_str());
}
TEST_METHOD(copy_assign_trivial_type) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  utils::fixed_size_vector<int, 10> copy;
  copy = sut;
  Assert::AreEqual(1, sut[0]);
  Assert::AreEqual(2, sut[1]);
  Assert::AreEqual(3, sut[2]);
  Assert::AreEqual(1, copy[0]);
  Assert::AreEqual(2, copy[1]);
  Assert::AreEqual(3, copy[2]);
}
TEST_METHOD(move_assign_trivial_type) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  utils::fixed_size_vector<int, 10> copy;
  copy = std::move(sut);
  Assert::AreEqual(1, copy[0]);
  Assert::AreEqual(2, copy[1]);
  Assert::AreEqual(3, copy[2]);
}
TEST_METHOD(copy_assign_object_counter) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  sut.emplace_back(1);
  sut.emplace_back(2);
  sut.emplace_back(3);
  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  utils::fixed_size_vector<ObjectCouter, 10> copy;
  copy = sut;
  Assert::AreEqual(std::size_t(6), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
}
TEST_METHOD(move_assign_object_counter) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  sut.emplace_back(1);
  sut.emplace_back(2);
  sut.emplace_back(3);
  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  utils::fixed_size_vector<ObjectCouter, 10> copy;
  copy = std::move(sut);
  Assert::AreEqual(std::size_t(6), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::move_constructed);
}
TEST_METHOD(copy_assign_std_string) {
  utils::fixed_size_vector<std::string, 10> sut{"123", "456", "789"};
  utils::fixed_size_vector<std::string, 10> copy;
  copy = sut;
  copy[0] = "abc";
  Assert::AreEqual("123", sut[0].c_str());
}
TEST_METHOD(pop_back_trivial_type) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  sut.pop_back();
  Assert::AreEqual(std::size_t(2), sut.size());
  Assert::AreEqual(1, sut[0]);
  Assert::AreEqual(2, sut[1]);
}
TEST_METHOD(pop_back_object_counter) {
  ObjectCouter::reset();
  utils::fixed_size_vector<ObjectCouter, 10> sut;
  sut.emplace_back(1);
  sut.emplace_back(2);
  sut.emplace_back(3);
  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  sut.pop_back();
  Assert::AreEqual(std::size_t(1), ObjectCouter::destructed);
}
TEST_METHOD(std_size) {
  utils::fixed_size_vector<int, 10> sut{1, 2, 3};
  Assert::AreEqual(std::size_t(3), std::size(sut));
}
TEST_METHOD(iterator_ctor_trivial_type) {
  std::list<int> v{1, 2, 3};
  utils::fixed_size_vector<int, 10> sut{v.begin(), v.end()};
  Assert::AreEqual(std::size_t(3), sut.size());
  Assert::AreEqual(1, sut[0]);
  Assert::AreEqual(2, sut[1]);
  Assert::AreEqual(3, sut[2]);
}
TEST_METHOD(iterator_ctor_object_counter) {
  ObjectCouter::reset();
  std::list<ObjectCouter> v;
  v.emplace_back(1);
  v.emplace_back(2);
  v.emplace_back(3);
  Assert::AreEqual(std::size_t(3), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  utils::fixed_size_vector<ObjectCouter, 10> sut{v.begin(), v.end()};
  Assert::AreEqual(std::size_t(6), ObjectCouter::sum());
  Assert::AreEqual(std::size_t(3), ObjectCouter::arg_constructed);
  Assert::AreEqual(std::size_t(3), ObjectCouter::copy_constructed);
  Assert::AreEqual(std::size_t(3), sut.size());
}
}
;
}  // namespace fixed_size_vector_UT
