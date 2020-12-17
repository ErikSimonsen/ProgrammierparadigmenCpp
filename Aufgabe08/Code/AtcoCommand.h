#ifndef AUFGABE7_ATCOCOMMAND_H
#define AUFGABE7_ATCOCOMMAND_H

#include <string>
#include "FileName.h"
#include "DynArray.h"

class AtcoCommand
{
public:

    /**
      * No copy constructor and assignment operator are needed because they are implemented in DynArray.cpp (which is the
      * only Class that allocates Heap Memory). In the default generated assignment operator or copy constructor of this class a flat copy
      * of each member var is made which trough the assignment of this->commands = copy.commands calls the assignment operator
      * of the DynArray<std::string>, which then correctly deletes the old memory and allocates the new array of strings.
    */
    /**
      * The default constructor is only needed so that in Resize() in DynArray.cpp we can create an array of AtcoCommands on the heap,
      * which will get filled afterwards with the contents of the old array.
      * This is why this class also has no setters, because the the default constructor is only used when
      * AtcoCmds allocate an Array (Heap Memory) of AtcoCommands (in its constructor and Resize())
     */
    AtcoCommand() = default;

    AtcoCommand(const FileName &filename, std::string const &wordSequence);

    void AddCommand(std::string const &command);

    FileName GetFileName() const;
    std::string GetWordSequence() const;
    void SetWordSequence(std::string const &wordSequence);
    void SetFileName(FileName const &fileName);

    std::string GetCommand(const int &i);

    const std::string *GetCommandsPtr() const;
    void SetCommand(const std::string &text, const int &i);

    int GetCmdCnt() const;

private:
    FileName filename;
    std::string wordSequence;

    DynArray<std::string> commands;
};

#endif //AUFGABE7_ATCOCOMMAND_H
