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
using arrow::Int64Builder;


int main(int argc, char** argv)
{
    arrow::Int64Builder i64builder;
    i64builder.Append(1);
    i64builder.Append(2);
    i64builder.Append(3);
    std::shared_ptr<arrow::Array> i64array;
    i64builder.Finish(&i64array);

    std::cout<<*(i64array.get());
}