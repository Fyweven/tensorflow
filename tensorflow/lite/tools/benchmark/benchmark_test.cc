/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include <iostream>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "tensorflow/lite/testing/util.h"
#include "tensorflow/lite/tools/benchmark/benchmark_tflite_model.h"
#include "tensorflow/lite/tools/benchmark/command_line_flags.h"

namespace {
const std::string* g_model_path = nullptr;
}

namespace tflite {
namespace benchmark {
namespace {

BenchmarkParams CreateParams() {
  BenchmarkParams params;
  params.AddParam("num_runs", BenchmarkParam::Create<int32_t>(2));
  params.AddParam("min_secs", BenchmarkParam::Create<float>(1.0f));
  params.AddParam("run_delay", BenchmarkParam::Create<float>(-1.0f));
  params.AddParam("num_threads", BenchmarkParam::Create<int32_t>(1));
  params.AddParam("benchmark_name", BenchmarkParam::Create<std::string>(""));
  params.AddParam("output_prefix", BenchmarkParam::Create<std::string>(""));
  params.AddParam("warmup_runs", BenchmarkParam::Create<int32_t>(1));
  params.AddParam("graph", BenchmarkParam::Create<std::string>(*g_model_path));
  params.AddParam("input_layer", BenchmarkParam::Create<std::string>(""));
  params.AddParam("input_layer_shape", BenchmarkParam::Create<std::string>(""));
  params.AddParam("use_nnapi", BenchmarkParam::Create<bool>(false));
  params.AddParam("warmup_min_secs", BenchmarkParam::Create<float>(0.5f));
  return params;
}

TEST(BenchmarkTest, DoesntCrash) {
  ASSERT_THAT(g_model_path, testing::NotNull());

  BenchmarkTfLiteModel benchmark(CreateParams());
  benchmark.Run();
}

}  // namespace
}  // namespace benchmark
}  // namespace tflite

int main(int argc, char** argv) {
  std::string model_path;
  std::vector<tflite::Flag> flags = {
      tflite::Flag::CreateFlag("graph", &model_path, "Path to model file.")};
  g_model_path = &model_path;
  const bool parse_result =
      tflite::Flags::Parse(&argc, const_cast<const char**>(argv), flags);
  if (!parse_result) {
    std::cerr << tflite::Flags::Usage(argv[0], flags);
    return 1;
  }

  ::tflite::LogToStderr();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
