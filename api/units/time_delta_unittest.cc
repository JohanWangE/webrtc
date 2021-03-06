/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "api/units/time_delta.h"

#include "test/gtest.h"

namespace webrtc {
namespace test {

TEST(TimeDeltaTest, GetBackSameValues) {
  const int64_t kValue = 499;
  for (int sign = -1; sign <= 1; ++sign) {
    int64_t value = kValue * sign;
    EXPECT_EQ(TimeDelta::ms(value).ms(), value);
    EXPECT_EQ(TimeDelta::us(value).us(), value);
    EXPECT_EQ(TimeDelta::seconds(value).seconds(), value);
    EXPECT_EQ(TimeDelta::seconds(value).seconds(), value);
  }
  EXPECT_EQ(TimeDelta::Zero().us(), 0);
}

TEST(TimeDeltaTest, GetDifferentPrefix) {
  const int64_t kValue = 3000000;
  EXPECT_EQ(TimeDelta::us(kValue).seconds(), kValue / 1000000);
  EXPECT_EQ(TimeDelta::ms(kValue).seconds(), kValue / 1000);
  EXPECT_EQ(TimeDelta::us(kValue).ms(), kValue / 1000);

  EXPECT_EQ(TimeDelta::ms(kValue).us(), kValue * 1000);
  EXPECT_EQ(TimeDelta::seconds(kValue).ms(), kValue * 1000);
  EXPECT_EQ(TimeDelta::seconds(kValue).us(), kValue * 1000000);
}

TEST(TimeDeltaTest, IdentityChecks) {
  const int64_t kValue = 3000;
  EXPECT_TRUE(TimeDelta::Zero().IsZero());
  EXPECT_FALSE(TimeDelta::ms(kValue).IsZero());

  EXPECT_TRUE(TimeDelta::PlusInfinity().IsInfinite());
  EXPECT_TRUE(TimeDelta::MinusInfinity().IsInfinite());
  EXPECT_FALSE(TimeDelta::Zero().IsInfinite());
  EXPECT_FALSE(TimeDelta::ms(-kValue).IsInfinite());
  EXPECT_FALSE(TimeDelta::ms(kValue).IsInfinite());

  EXPECT_FALSE(TimeDelta::PlusInfinity().IsFinite());
  EXPECT_FALSE(TimeDelta::MinusInfinity().IsFinite());
  EXPECT_TRUE(TimeDelta::ms(-kValue).IsFinite());
  EXPECT_TRUE(TimeDelta::ms(kValue).IsFinite());
  EXPECT_TRUE(TimeDelta::Zero().IsFinite());
}

TEST(TimeDeltaTest, ComparisonOperators) {
  const int64_t kSmall = 450;
  const int64_t kLarge = 451;
  const TimeDelta small = TimeDelta::ms(kSmall);
  const TimeDelta large = TimeDelta::ms(kLarge);

  EXPECT_EQ(TimeDelta::Zero(), TimeDelta::ms(0));
  EXPECT_EQ(TimeDelta::PlusInfinity(), TimeDelta::PlusInfinity());
  EXPECT_EQ(small, TimeDelta::ms(kSmall));
  EXPECT_LE(small, TimeDelta::ms(kSmall));
  EXPECT_GE(small, TimeDelta::ms(kSmall));
  EXPECT_NE(small, TimeDelta::ms(kLarge));
  EXPECT_LE(small, TimeDelta::ms(kLarge));
  EXPECT_LT(small, TimeDelta::ms(kLarge));
  EXPECT_GE(large, TimeDelta::ms(kSmall));
  EXPECT_GT(large, TimeDelta::ms(kSmall));
  EXPECT_LT(TimeDelta::Zero(), small);
  EXPECT_GT(TimeDelta::Zero(), TimeDelta::ms(-kSmall));
  EXPECT_GT(TimeDelta::Zero(), TimeDelta::ms(-kSmall));

  EXPECT_GT(TimeDelta::PlusInfinity(), large);
  EXPECT_LT(TimeDelta::MinusInfinity(), TimeDelta::Zero());
}

TEST(TimeDeltaTest, CanBeInititializedFromLargeInt) {
  const int kMaxInt = std::numeric_limits<int>::max();
  EXPECT_EQ(TimeDelta::seconds(kMaxInt).us(),
            static_cast<int64_t>(kMaxInt) * 1000000);
  EXPECT_EQ(TimeDelta::ms(kMaxInt).us(), static_cast<int64_t>(kMaxInt) * 1000);
}

TEST(TimeDeltaTest, ConvertsToAndFromDouble) {
  const int64_t kMicros = 17017;
  const double kNanosDouble = kMicros * 1e3;
  const double kMicrosDouble = kMicros;
  const double kMillisDouble = kMicros * 1e-3;
  const double kSecondsDouble = kMillisDouble * 1e-3;

  EXPECT_EQ(TimeDelta::us(kMicros).seconds<double>(), kSecondsDouble);
  EXPECT_EQ(TimeDelta::seconds(kSecondsDouble).us(), kMicros);

  EXPECT_EQ(TimeDelta::us(kMicros).ms<double>(), kMillisDouble);
  EXPECT_EQ(TimeDelta::ms(kMillisDouble).us(), kMicros);

  EXPECT_EQ(TimeDelta::us(kMicros).us<double>(), kMicrosDouble);
  EXPECT_EQ(TimeDelta::us(kMicrosDouble).us(), kMicros);

  EXPECT_NEAR(TimeDelta::us(kMicros).ns<double>(), kNanosDouble, 1);

  const double kPlusInfinity = std::numeric_limits<double>::infinity();
  const double kMinusInfinity = -kPlusInfinity;

  EXPECT_EQ(TimeDelta::PlusInfinity().seconds<double>(), kPlusInfinity);
  EXPECT_EQ(TimeDelta::MinusInfinity().seconds<double>(), kMinusInfinity);
  EXPECT_EQ(TimeDelta::PlusInfinity().ms<double>(), kPlusInfinity);
  EXPECT_EQ(TimeDelta::MinusInfinity().ms<double>(), kMinusInfinity);
  EXPECT_EQ(TimeDelta::PlusInfinity().us<double>(), kPlusInfinity);
  EXPECT_EQ(TimeDelta::MinusInfinity().us<double>(), kMinusInfinity);
  EXPECT_EQ(TimeDelta::PlusInfinity().ns<double>(), kPlusInfinity);
  EXPECT_EQ(TimeDelta::MinusInfinity().ns<double>(), kMinusInfinity);

  EXPECT_TRUE(TimeDelta::seconds(kPlusInfinity).IsPlusInfinity());
  EXPECT_TRUE(TimeDelta::seconds(kMinusInfinity).IsMinusInfinity());
  EXPECT_TRUE(TimeDelta::ms(kPlusInfinity).IsPlusInfinity());
  EXPECT_TRUE(TimeDelta::ms(kMinusInfinity).IsMinusInfinity());
  EXPECT_TRUE(TimeDelta::us(kPlusInfinity).IsPlusInfinity());
  EXPECT_TRUE(TimeDelta::us(kMinusInfinity).IsMinusInfinity());
}

TEST(TimeDeltaTest, MathOperations) {
  const int64_t kValueA = 267;
  const int64_t kValueB = 450;
  const TimeDelta delta_a = TimeDelta::ms(kValueA);
  const TimeDelta delta_b = TimeDelta::ms(kValueB);
  EXPECT_EQ((delta_a + delta_b).ms(), kValueA + kValueB);
  EXPECT_EQ((delta_a - delta_b).ms(), kValueA - kValueB);

  const int32_t kInt32Value = 123;
  const double kFloatValue = 123.0;
  EXPECT_EQ((TimeDelta::us(kValueA) * kValueB).us(), kValueA * kValueB);
  EXPECT_EQ((TimeDelta::us(kValueA) * kInt32Value).us(), kValueA * kInt32Value);
  EXPECT_EQ((TimeDelta::us(kValueA) * kFloatValue).us(), kValueA * kFloatValue);

  EXPECT_EQ((delta_b / 10).ms(), kValueB / 10);
  EXPECT_EQ(delta_b / delta_a, static_cast<double>(kValueB) / kValueA);

  EXPECT_EQ(TimeDelta::us(-kValueA).Abs().us(), kValueA);
  EXPECT_EQ(TimeDelta::us(kValueA).Abs().us(), kValueA);
}
}  // namespace test
}  // namespace webrtc
