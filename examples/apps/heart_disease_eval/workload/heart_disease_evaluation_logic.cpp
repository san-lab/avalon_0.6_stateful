/* Copyright 2019 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <string>
#include <cmath>
#include "heart_disease_evaluation_logic.h"

// Calculate risk of having heart disease given input parameters.

HeartDiseaseEvalLogic::HeartDiseaseEvalLogic() {}

HeartDiseaseEvalLogic::~HeartDiseaseEvalLogic() {}

template<typename Out>
void split(const std::string &str, char delim, Out result) {
        std::size_t current, previous = 0;

        current = str.find(delim);
        while (current != std::string::npos) {
                std::string item = str.substr(previous, current - previous);
                if (item.compare("") != 0)
                        *(result++) = item;
                previous = current + 1;
                current = str.find(delim, previous);
        }

        std::string item = str.substr(previous, current - previous);
        if (item.compare("") != 0)
                *(result++) = item;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

// Process work order input for heart disease risk factors
std::string HeartDiseaseEvalLogic::executeWorkOrder(
        std::string decrypted_user_input_str) {
    std::string resultString;

    try {
        std::string dataString;
        std::vector<std::string> inputString =
            split(decrypted_user_input_str, ':');

        if (inputString[0] == "store"){
            dataString = inputString[1];
            std::vector<std::string> store_Data = split(dataString, ' ');
            if(store_Data.size() == 2){
                HeartDiseaseEvalLogic::keys.push_back(store_Data[0]);
                HeartDiseaseEvalLogic::values.push_back(store_Data[1]);
                return "Value correctly stored on SGX";
            }
            else {
                return "Incorrect number of parameters on store command.";
            } 
        }
        else if(inputString[0] == "retrieve"){
            std::string retrieveString = inputString[1];
            for (int i = 0; i < HeartDiseaseEvalLogic::keys.size(); i++){
                if(HeartDiseaseEvalLogic::keys.at(i) == retrieveString){
                    return "Value on the table: " + HeartDiseaseEvalLogic::values.at(i);
                }
            }
            return "Value not found on the table.";
        }
        else {
            return "Incorrect command. Only store and retrieve are available.";
        }
    } catch (...) {
        resultString = "Caught exception while processing workload data";
    }
    return resultString;
}

