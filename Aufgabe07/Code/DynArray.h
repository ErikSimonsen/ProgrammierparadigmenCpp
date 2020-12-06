#ifndef AUFGABE7_DYNARRAY_H
#define AUFGABE7_DYNARRAY_H
template <class T>
class DynArray
{
    T *arrPtr = nullptr;
    int cntElements = 0;
    int limit;
    int resize = 5; //increase size by this number each time the limit is hit; Make configurable?
public:
    DynArray(const int &limit = 5);
    DynArray(const DynArray<T> &copy);
    ~DynArray();
    DynArray<T> &operator=(const DynArray<T> &copy);

    void Add(const T &element);
    void Set(const T &element, const int &pos);

    T Get(const int &index);
    const T *GetArrPtr() const;
    int GetCntElements() const;
    int GetLimit() const;

private:
    void Resize();
    //Diese Methode existiert, da ich nicht standardmäßig beim Erstellen von DynArray-Objekten Heap Speicher für this->arrPtr
    //allokieren möchte, welcher dann vielleicht garnicht genutzt wird. Daher ist this->arrPointer standardmäßig ein nullptr
    //und allokiert erst Heap-Speicher wenn ein Objekt vom Typ T hinzugefügt wird.
    void InitArr();
    void Free();
    void CheckAgainstLimit(const int &pos);
};
#include "DynArray.cpp"
#endif //AUFGABE7_DYNARRAY_H