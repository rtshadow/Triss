/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef FIELDS_H_
#define FIELDS_H_

template <class T>
class Field {
    public:
    T value;
    int nextFieldId;

    bool operator<(const Field<T>& other) const {
        return value < other.value;
    }

    virtual void updateNextFieldUsingMapping(int* currentColumnMapping, int* nextColumnMapping) {
        nextFieldId = nextColumnMapping[nextFieldId];
    }

    virtual bool isLastListElement() {
        return true;
    }
};

template <class T>
class ListField : public Field<T> {
    public:
    bool isLastElement;

    virtual void updateNextFieldUsingMapping(int* currentColumnMapping, int* nextColumnMapping) {
        if(isLastElement) {
            this -> nextFieldId = nextColumnMapping[this -> nextFieldId];
        }
        else {
            this -> nextFieldId = currentColumnMapping[this -> nextFieldId];
        }
    }

    bool isLastListElement() {
        return isLastElement;
    }
};


#endif /* FIELDS_H_ */