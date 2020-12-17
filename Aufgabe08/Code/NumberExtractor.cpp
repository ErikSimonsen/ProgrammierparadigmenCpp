#include <sstream>
#include <algorithm>
#include <vector>
#include "NumberExtractor.h"
#include "Util.h"

NumberExtractor::NumberExtractor(std::string astr_WordSeq, int startIndexExtractionAfter) {
    std::string word;
    std::istringstream iss(astr_WordSeq);
  
    while(iss >> word){
        fullWordSeq.push_back(word);
    }

    this->startIndexExtractionAfter = startIndexExtractionAfter;
    FillDictionaryMap();
}

NumberExtractor::NumberExtractor(const std::vector<std::string> &ar_allWordsAsString, int startIndexExtractionAfter) {
    fullWordSeq = ar_allWordsAsString;
    this->startIndexExtractionAfter = startIndexExtractionAfter;
    FillDictionaryMap();
}
bool NumberExtractor::PerformFullExtraction(){
    
    NumberExtractor::ExtractedNumber exNum  = ExtractNextFullNumber();
    
    while(exNum.IsExtractedNumberValid()){
        m_extractedNumberSeq.push_back(exNum);
        exNum = ExtractNextFullNumber();
    }

    return true; //check for exceptions and then return false ?
}
NumberExtractor::ExtractedNumber NumberExtractor::ExtractNextFullNumber() {
    //check if all words are read already, so you cant go past the vector size
    int wordSeqCount = static_cast<int>(fullWordSeq.size());
    if(startIndexExtractionAfter == wordSeqCount)
        return NumberExtractor::ExtractedNumber();
    //always start from the word after (this is why 1 is added) startIndexExtractionAfter, not from the beginning of the vector.
    std::vector<std::string>::iterator start = fullWordSeq.begin() + (startIndexExtractionAfter + 1);
    std::vector<std::string>::iterator end = fullWordSeq.end();
    std::vector<std::string>::iterator firstNumber = fullWordSeq.end();
    //find the first element in the fullWordSeq vector which is also in the dictionary map
    //-> marks the point where the first number sequence starts
    for(auto it = start; it != end; ++it){
        std::string word = *it;
        if(dictionaryMap.find(word) != dictionaryMap.end()){
         firstNumber = it;
         break;
        }
    }
    if(firstNumber == fullWordSeq.end()){ //if no number was in the word sequence
        //set to the size of the wordSequence-Vector so that this function returns instantly when its called again after returning
        //an empty number/being at the end of the wordSequence (see condition above)
        startIndexExtractionAfter = std::distance(fullWordSeq.begin(), end);
        return NumberExtractor::ExtractedNumber();
    }
    //we know the start of the number sequence already 
    //-> just iterate until the end of the vector until the current element in the vector is not found in the dictionary anymore
    //-> marks the end of the number sequence
    std::vector<std::string> extractedWords;
    for(auto it = firstNumber; it != end; ++it){ //because the element that we start from is a number for sure
        //get the word the current iterator points to and try to find it in the dictionary. if the resulting iterator marks the end
        //of the dictionary -> the word was not found in it and we can break out of the loop
        std::string word = *it;
        auto dictionaryIter = dictionaryMap.find(word);
        if(dictionaryIter == dictionaryMap.end()){
            end = it; //overwrite the end iterator with the iterator that pointed to the last searched word
            break;
        }else{
            extractedWords.push_back(word);
        }
    }
    std::string extractedNumber = ProcessExtractedWords(extractedWords);

    //start the next extract function from the word that was the last one iterated
    startIndexExtractionAfter = std::distance(fullWordSeq.begin(), end);
    std::vector<std::string> seqOfNumbersVec(firstNumber, end); //get the whole word sequence that contained the number sequence
    std::string seqOfNumbers;
    for(auto &val : seqOfNumbersVec)
        seqOfNumbers.append(val + " ");
    seqOfNumbers = Trim(seqOfNumbers);   
    std::size_t separatorPos = extractedNumber.find(".");
    int number;
    if(separatorPos != std::string::npos){ //if there is a separator (.) get the part of the string before and after the separator
        number = std::stoi(extractedNumber.substr(0, separatorPos));
        int decimal = std::stoi(extractedNumber.substr(separatorPos+1, extractedNumber.length()));
        return NumberExtractor::ExtractedNumber(seqOfNumbers, number, decimal);
    }else{
        number = stoi(extractedNumber);
    }
    return NumberExtractor::ExtractedNumber(seqOfNumbers, number);
}

std::string NumberExtractor::ProcessExtractedWords(std::vector<std::string> &extractedWords){
    std::string extractedNumber;
    for(auto it = extractedWords.begin(); it < extractedWords.end(); ++it){
        std::string word = *it;
        auto dictionaryIter = dictionaryMap.find(word);
        auto nextIter = std::next(it);
        //if there no next word just concatenate the value from the dictionary for this key (word), e.g. for a thousand just add 000
        if (nextIter == extractedWords.end()) 
        {
            extractedNumber += dictionaryIter->second;
            break;
        }
        //get value for the next word from the dictionaryMap to check how the value for the current word should be displayed
        std::string nextWord = *nextIter;
        auto nextDictionaryIter = dictionaryMap.find(nextWord);
        if(dictionaryIter->first == "thousand"){ //tausender
            //man muss auch das übernächste wort prüfen : three thousand five hundred; 3500
            auto afterNextIter = std::next(nextIter);
            if(afterNextIter != extractedWords.end()){
                std::string afterNextWord = *afterNextIter;
                auto afterNextDictionaryIter = dictionaryMap.find(afterNextWord);
                if(afterNextDictionaryIter->first == "hundred"){ //if word after next word is hundred -> dont concatenate anything
                    continue;
                }
            }
            //always ignore zeros after thousand, because its inconsistent in the test examples: 
            //one thousand twenty eight = 1028; one thousand zero two nine = 1029
            if(nextDictionaryIter->first == "zero"){ 
                ++it;//überspringe zero für die nächsten Schleifeniteration
                //überspringe zero für die weitere Verarbeitung und betrachte das darauffolgende Wort (falls es eins gibt)
                //in Abhängigkeit zu diesem Wort siehe nächstes if - else conditional
                nextIter = std::next(nextIter); 
                if(nextIter != extractedWords.end()){
                    nextWord = *nextIter;
                    nextDictionaryIter = dictionaryMap.find(nextWord);
                }
            }
            
            if(nextDictionaryIter->second.size() == 2){//zehner nach tausender: three thousand forty: 3040 -> füge nur eine 0 hinzu
                extractedNumber += dictionaryIter->second.substr(0,1);
            }else{ //das nächste wort muss einstellig sein
                //falls das wort danach (also das übernächste wort) auch einstellig ist werden diese zweistellig:
                //one thousand zero two nine: 1029 -> das zero wird geskipped und dann ist die nächste zahl 2 
                //falls die darauffolgende zahl auch einstellig -> werden diese beiden zusammengefügt und man kann nur eine 0 von der tausend konkatenieren
                nextIter = std::next(nextIter); 
                if(nextIter != extractedWords.end()){
                    nextWord = *nextIter;
                    nextDictionaryIter = dictionaryMap.find(nextWord);
                    if(nextDictionaryIter->second.size() == 1){
                        extractedNumber += dictionaryIter->second.substr(0,1);
                        continue;
                    }
                }
                extractedNumber += dictionaryIter->second.substr(0,2);
            }
        }

        else if(dictionaryIter->first == "hundred"){ //hunderter
            //always ignore zeros after hundreds, because its inconsistent in the test examples: 
            //one hundred zero two = 102; one hundred two = 102;
            if(nextDictionaryIter->first == "zero"){
                ++it;//überspringe zero für die nächsten Schleifeniteration
                //überspringe zero für die weitere Verarbeitung und betrachte das darauffolgende Wort in Abhängigkeit zu diesem Wort
                //siehe nächstes if - else conditional
                nextIter = std::next(nextIter); 
                if(nextDictionaryIter->second.size() == 1){
                    nextWord = *nextIter;
                    nextDictionaryIter = dictionaryMap.find(nextWord);
                }
            } 
           if(nextDictionaryIter->second.size() == 2){
               continue;
           }else{
               extractedNumber += dictionaryIter->second.substr(0,1);
           }
        }
        else if(dictionaryIter->second.size() == 2 ){ //aktuelles wort ist zehner
            if(nextIter != extractedWords.end()){ //prüfung hinzufügen ob nächstes element zwei oder einstellig ist ?
                extractedNumber += dictionaryIter->second.substr(0, 1);
            }
        }else{ //einer
            extractedNumber += dictionaryIter->second;
        }
    }
    return extractedNumber;
}
void NumberExtractor::FillDictionaryMap(){
    dictionaryMap.insert(std::make_pair("thousand", "000"));
    dictionaryMap.insert(std::make_pair("hundred", "00"));
    dictionaryMap.insert(std::make_pair("ninety", "90"));
    dictionaryMap.insert(std::make_pair("eighty", "80"));
    dictionaryMap.insert(std::make_pair("seventy", "70"));
    dictionaryMap.insert(std::make_pair("sixty", "60"));
    dictionaryMap.insert(std::make_pair("fifty", "50"));
    dictionaryMap.insert(std::make_pair("fourty", "40"));
    dictionaryMap.insert(std::make_pair("thirty", "30"));
    dictionaryMap.insert(std::make_pair("twenty", "20"));
    dictionaryMap.insert(std::make_pair("ten", "10"));
    dictionaryMap.insert(std::make_pair("nine", "9"));
    dictionaryMap.insert(std::make_pair("eight", "8"));
    dictionaryMap.insert(std::make_pair("seven", "7"));
    dictionaryMap.insert(std::make_pair("six", "6"));
    dictionaryMap.insert(std::make_pair("five", "5"));
    dictionaryMap.insert(std::make_pair("four", "4"));
    dictionaryMap.insert(std::make_pair("three", "3"));
    dictionaryMap.insert(std::make_pair("two", "2"));
    dictionaryMap.insert(std::make_pair("one", "1"));
    dictionaryMap.insert(std::make_pair("zero", "0"));
    dictionaryMap.insert(std::make_pair("point", "."));
    dictionaryMap.insert(std::make_pair("decimal", "."));
}

double NumberExtractor::ExtractedNumber::GetExtractedNumberAsDouble() const{
    std::string concatenatedNum = std::to_string(mi_number) + "." + std::to_string(mi_numberAfterDecimal);
    return std::stod(concatenatedNum);
    
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
    int counter = 0;
    while(iss >> word){ counter++;}
    return counter;
}

