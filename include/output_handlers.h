//
// Created by Neel Bhave on 4/24/21.
//

#ifndef S3SELECT_OUTPUT_HANDLERS_H
#define S3SELECT_OUTPUT_HANDLERS_H

enum class output_mode{
    ARROW,
    STRING
};

class base_output_handler{
protected:
    std::string file_name;
    int column_count;
    std::vector<arrow::ArrayBuilder *> builderArray;

public:
    base_output_handler(std::string file, int col_count):file_name(file),column_count(col_count)
    {
        std::cout<<"file_name "<<file_name;
        std::cout<<" column_count "<<column_count<<std::endl;
    }

};


class arrow_output_handler: base_output_handler{
private:
    std::shared_ptr<arrow::Table> outputTable;

public:
    arrow_output_handler(std::string file, int col_count): base_output_handler(file, col_count)
    {
        std::cout<<"file_name1 "<<file_name;
        std::cout<<" column_count1 "<<column_count<<std::endl;
    }

    void setOutputTable(std::shared_ptr<arrow::Table> opTable){
        this->outputTable=opTable;
    }

   void appendResultRow(std::vector<s3selectEngine::value> result){
       for (int i = 0; i < result.size(); ++i) {
           s3selectEngine::value v = result[i];
           if(builderArray.size()<=i){
               if(v.is_number()){
                   if(builderArray.size()<=i) {
                       arrow::Int64Builder i64builder;
                       std::cout<<"Length "<<i64builder.length();
                       builderArray.push_back(&i64builder);

                   }else {
                       arrow::Int64Builder *int64Builder = (arrow::Int64Builder *) &builderArray[i];
//                   std::cout<<"Length "<<int64Builder->length();
                       int64Builder->Append(v.i64());
                   }
                }
              else if(v.is_string()){
                   if(builderArray.size()<=i) {
                       builderArray.push_back(new arrow::StringBuilder());
                   }
                   arrow::StringBuilder *stringBuilder = (arrow::StringBuilder *) &builderArray[i];
                   stringBuilder->Append(v.str());
              }
              else if(v.is_bool()){
                   if(builderArray.size()<=i) {
                       builderArray.push_back(new arrow::BooleanBuilder());
                   }
                   arrow::BooleanBuilder *booleanBuilder = (arrow::BooleanBuilder *) &builderArray[i];
                   booleanBuilder->Append(v.is_true());
              }
           }
                std::cout<<builderArray[i]->length()<<" ";

       }
       std::cout<<builderArray.size()<<"\n";

    }
};

#endif //S3SELECT_OUTPUT_HANDLERS_H
