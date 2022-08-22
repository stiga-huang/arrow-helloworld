// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include <arrow/json/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>
#include <arrow/pretty_print.h>
#include <arrow/result.h>
#include <arrow/status.h>
#include <arrow/table.h>
#include <arrow/memory_pool.h>

#include <iostream>
#include <vector>

using arrow::Status;

namespace {

Status RunMain(int argc, char** argv) {
  const char* json_filename = "test.json";
  const char* arrow_filename = "test.arrow";

  std::vector<std::shared_ptr<arrow::Field>> fields_list = {};
  fields_list.push_back(arrow::field("id", arrow::int32()));
  fields_list.push_back(arrow::field("str", arrow::utf8()));
  fields_list.push_back(arrow::field("price", arrow::decimal128(9, 2)));
  std::shared_ptr<arrow::Schema> schema;
  schema = arrow::schema(fields_list);
  auto parseOptions = arrow::json::ParseOptions::Defaults();
  parseOptions.explicit_schema = schema;

  std::cerr << "* Reading JSON file '" << json_filename << "' into table" << std::endl;
  ARROW_ASSIGN_OR_RAISE(auto input_file, arrow::io::ReadableFile::Open(json_filename));
  ARROW_ASSIGN_OR_RAISE(auto json_reader, arrow::json::TableReader::Make(
                                             arrow::default_memory_pool(), input_file,
                                             arrow::json::ReadOptions::Defaults(),
                                             parseOptions));
  ARROW_ASSIGN_OR_RAISE(auto table, json_reader->Read());

  std::cerr << "* Read table:" << std::endl;
  ARROW_RETURN_NOT_OK(arrow::PrettyPrint(*table, {}, &std::cerr));

  std::cerr << "* Writing table into Arrow IPC file '" << arrow_filename << "'"
            << std::endl;
  ARROW_ASSIGN_OR_RAISE(auto output_file,
                        arrow::io::FileOutputStream::Open(arrow_filename));
  ARROW_ASSIGN_OR_RAISE(auto batch_writer,
                        arrow::ipc::MakeFileWriter(output_file, table->schema()));
  ARROW_RETURN_NOT_OK(batch_writer->WriteTable(*table));
  ARROW_RETURN_NOT_OK(batch_writer->Close());

  return Status::OK();
}

}  // namespace

int main(int argc, char** argv) {
  Status st = RunMain(argc, argv);
  if (!st.ok()) {
    std::cerr << st << std::endl;
    return 1;
  }
  return 0;
}
