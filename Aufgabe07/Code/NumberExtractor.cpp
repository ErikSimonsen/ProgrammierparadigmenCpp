#include <sstream>
#include "NumberExtractor.h"

NumberExtractor::NumberExtractor(std::string astr_WordSeq, int startIndexExtractionAfter) {
    //
}

NumberExtractor::NumberExtractor(const std::vector<std::string> &ar_allWordsAsString, int startIndexExtractionAfter) {

}

NumberExtractor::~NumberExtractor() {

}

NumberExtractor::ExtractedNumber NumberExtractor::ExtractNextFullNumber() {
    return NumberExtractor::ExtractedNumber();
}


//ExtractedNumber methods - Inner Class of NumberExtractor
NumberExtractor::ExtractedNumber::ExtractedNumber(std::string astr_seqOfNumbers, int numberToStore, int numAfterDeci) {
    this->mstr_numberSeq = astr_seqOfNumbers;
    this->mi_number = numberToStore;
    this->mi_numberAfterDecimal = numAfterDeci;
}

int NumberExtractor::ExtractedNumber::GetNumberOfStringsForNumber() const {
    std::istringstream iss(this->mstr_numberSeq);
    std::string word;
    int counter;
    while(iss >> word){ counter++;}
    return counter;
}

