#ifndef AUFGABE7_NUMBEREXTRACTOR_H
#define AUFGABE7_NUMBEREXTRACTOR_H
#include <map>
#include <string>
#include <vector>
class NumberExtractor
{

public:
    //this inner class only responsibility is saving the extracted numbers and the string and exposing it as int, double. Also
    //exposes functions to check if the extracted number is valid and to get the string which was processed.
    //Note: The processing of the word sequence happens in the parent class NumberExtractor
    class ExtractedNumber
    {

    public:
        /**
         * @param astr_seqOfNumbers Only sequence of the string that contains the words that could be processed to numbers
         * @param numberToStore
         * @param numAfterDeci Which number comes after the decimal point
         */
        ExtractedNumber(std::string astr_seqOfNumbers = "", int numberToStore = -1, int numAfterDeci = -1);

        int GetExtractedNumberAsInt() const
        {
            return mi_number;
        }
        double GetExtractedNumberAsDouble() const;
        bool IsExtractedNumberValid() const
        {
            return mi_number >= 0;
        }
        std::string GetNumberStringSeq() const
        {
            return mstr_numberSeq;
        }
        bool IsExtractedNumberAnInt() const
        {
            return (mi_numberAfterDecimal < 0) && IsExtractedNumberValid();
        }
        //! number of words in mstr_numberSeq
        int GetNumberOfStringsForNumber() const;

    private:
        //!substring of mstr_numberSeq containing the words implementing the number
        std::string mstr_numberSeq;
        //!the extracted number
        int mi_number = -1;
        //!the extracted number after word "decimal" etc. if we have extracted a double
        int mi_numberAfterDecimal = -1;
    };
public:
    //! the numbers are extracted from astr_WordSeq,
    // starting after the word startIndexExtractionAfter, first word starts at index 0
    NumberExtractor(std::string astr_WordSeq, int startIndexExtractionAfter = -1);
    //! same as before, but the single are already extracted into a vector
    //! the numbers are extracted from ar_allWordsAsString,
    // starting after the word startIndexExtractionAfter, first word starts at index in vector 0
    NumberExtractor(const std::vector<std::string> &ar_allWordsAsString, int startIndexExtractionAfter);
    //! Extracts the next complete number from sequence of words provide in constructor
    ExtractedNumber ExtractNextFullNumber();

    bool PerformFullExtraction();

    int GetExtractedNumbersCnt() const{
        return static_cast<int>(m_extractedNumberSeq.size());
    }
    //these functions throw an exception if the index is invalid
    int GetNumberAsInt(int index) const{
        return m_extractedNumberSeq.at(index).GetExtractedNumberAsInt();
    }
    double GetNumberAsDouble(int index) const {
        return m_extractedNumberSeq.at(index).GetExtractedNumberAsDouble();
    }
    bool IsNumberValid(int index) const{
        return m_extractedNumberSeq.at(index).IsExtractedNumberValid();
    }
    bool IsNumberInt(int index) const {
        return IsNumberValid(index) && m_extractedNumberSeq.at(index).IsExtractedNumberAnInt();
    }
    bool IsNumberDouble(int index) const{
        return IsNumberValid(index) && !m_extractedNumberSeq.at(index).IsExtractedNumberAnInt();
    }
private:
    std::map<std::string, std::string> dictionaryMap; //read in from file or define in place ?
    std::vector<std::string> fullWordSeq; //could also use DynArray<std::string> here
    std::vector<ExtractedNumber> m_extractedNumberSeq;
    int startIndexExtractionAfter; //start after this index! -> always one lower than the index that you want to start from

    void FillDictionaryMap();
    std::string ProcessExtractedWords(std::vector<std::string> &extractedWords);
};
#endif