//
// Created by Neel Bhave on 4/15/21.
//
#include "arrow_example.h"
#include "s3select.h"
#include <arrow/api.h>
#include <arrow/io/api.h>
//#include <arrow/array/array_base.h>
//#include <arrow/array/builder_primitive.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arrow/result.h>
#include <arrow/status.h>
#include <arrow/table.h>
#include <arrow/ipc/api.h>

using arrow::Int64Builder;
using arrow::StringBuilder;
using arrow::schema;
using arrow::Table;
using arrow::Status;
using arrow::ipc::RecordBatchWriter;

int main(int argc, char** argv)
{
    const char* arrow_filename = "test.arrow";

    arrow::Int64Builder i64builder;
    i64builder.Append(1);
    i64builder.Append(2);
    i64builder.Append(3);
    std::shared_ptr<arrow::Array> i64array;
    i64builder.Finish(&i64array);
//    std::cout<<*(i64array.get());

    arrow::StringBuilder strbuilder;
    strbuilder.Append("one");
    strbuilder.Append("two");
    strbuilder.Append("three");
    strbuilder.Append("four");
    std::shared_ptr<arrow::Array> strarray;
    strbuilder.Finish(&strarray);
//    std::cout<<*(strarray.get());

    auto schema = arrow::schema(
            {arrow::field("int", arrow::int64()),
             arrow::field("str", arrow::utf8())});

    auto data_table = arrow::Table::Make(schema, {i64array, strarray});

    std::cout<<(data_table.get())->ToString();

    auto output_file = arrow::io::FileOutputStream::Open(arrow_filename);

    auto batch_writer= arrow::ipc::MakeFileWriter(output_file,data_table->schema());

    batch_writer->WriteTable(*data_table);

    batch_writer->Close();
}