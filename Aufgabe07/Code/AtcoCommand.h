#ifndef AUFGABE6_ATCOCOMMAND_H
#define AUFGABE6_ATCOCOMMAND_H

#include <string>
#include "FileName.h"

class AtcoCommand
{
public:
    /**
     * this is only needed so that in Resize() in AtcoCmds.cpp we can create an array of AtcoCommands on the heap,
     * which will get filled afterwards with the contents of the old array.
     * This is why this class also has no setters, because the the default constructor is only used when 
     * AtcoCmds allocate an Array (Heap Memory) of AtcoCommands (in its constructor and Resize())
    */
    //ToDo: Refactor?
    AtcoCommand() = default;

    AtcoCommand(const FileName &filename, std::string const &wordSequence);

    AtcoCommand(AtcoCommand const &copy);
    AtcoCommand &operator=(const AtcoCommand &copy);
    ~AtcoCommand();

    void AddCommand(std::string const &command);

    FileName GetFileName() const;

    void SetWordSequence(std::string const &wordSequence);
    void SetFileName(FileName const &fileName);

    std::string GetWordSequence() const;

    std::string GetCommand(const int &i) const;

    std::string *GetCommands() const;

    int GetCmdCnt() const;

private:
    FileName filename;
    std::string wordSequence;
    int maxCommands = 5;
    int cmdCnt = 0;
    std::string *commands = nullptr;

    void CheckAgainstCmdCnt(const int &i) const;

    void Resize();

    //Diese Methode existiert, da ich nicht standardmäßig beim Erstellen von AtcoCommand-Objekten Heap Speicher für die this->commands
    //Membervariable allokieren möchte, welcher dann vielleicht garnicht genutzt wird. Daher ist this->commands standardmäßig ein nullptr
    //und allokiert erst Heap-Speicher wenn ein Kommando (String) hinzugefügt wird.
    void InitCommands();

    void FreeCommands();
};

#endif //AUFGABE6_ATCOCOMMAND_H
