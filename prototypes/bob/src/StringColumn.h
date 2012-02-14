#ifndef PROTOTYPES_BOB_SRC_STRINGCOLUMN_H_
#define PROTOTYPES_BOB_SRC_STRINGCOLUMN_H_

#include <string>
#include "Column.h"

class StringColumn : public Column {
    protected:
    class StringField : public Field {
        public:
        std :: string value;

        virtual int compare(Field* other);
    };

    public:
    void addField(const std :: string& value, int nextFieldId);
};

#endif  // PROTOTYPES_BOB_SRC_STRINGCOLUMN_H_