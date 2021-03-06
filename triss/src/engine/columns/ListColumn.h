/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_COLUMNS_LISTCOLUMN_H_
#define TRISS_ENGINE_SRC_COLUMNS_LISTCOLUMN_H_

#include <algorithm>
#include <list>
#include <vector>
#include "TypedColumn.h"

template <class T>
    class ListColumn : public TypedColumn<T> {
        private:
        TypedListColumnQueryState<T>* getTypedListState(ColumnQueryState* state) const {
            return static_cast<TypedListColumnQueryState<T>*>(state);
        }

        protected:
        bool hasBeenVisited(int valueIndex, int startPoint, TypedListColumnQueryState<T>* state) const {
            return valueIndex < startPoint && state -> constraintRangeSet.isInRange(valueIndex);
        }

        public:
        void updateNextFieldIdsUsingMapping(std::vector<int>& current, std::vector<int>& next, int indicesShift) {
            for(unsigned int i = 0; i < this -> fields.size(); ++i) {
                if(this -> fields[i].isLastElement) {
                    this -> fields[i].nextFieldId = next[this -> fields[i].nextFieldId] + indicesShift;
                }
                else {
                    this -> fields[i].nextFieldId = current[this -> fields[i].nextFieldId];
                }
            }
        }

        void add(const Row& row, int nextFieldId) {
            if(row.isNull(this -> columnId)) {
                this -> addNull(nextFieldId);
            }
            else {
                std::list<T>& ls = row.get<std::list<T> >(this -> columnId);
                
                if(ls.empty()) {
                    this -> addEmptyListField(nextFieldId);
                }
                else {
                    typename std::list<T>::iterator left = ls.begin(), right = ls.begin();
                    right++;
                    for(;right != ls.end(); left++, right++) {
                        this -> addField(*left, this -> fields.size() + 1, false);
                    }
                    this -> addField(*left, nextFieldId, true);
                }
            }
        }

        ColumnQueryState* prepareColumnForQuery() const {
            TypedListColumnQueryState<T>* typedListState = new TypedListColumnQueryState<T>();
            typedListState -> isMainColumn = false;
            return typedListState;
        }

        void markAsMainQueryColumn(ColumnQueryState* state) const {
            TypedListColumnQueryState<T>* typedListState = getTypedListState(state);
            typedListState -> isMainColumn = true;
            
            if(typedListState -> valueRangeSet != NULL &&
                    typedListState -> valueRangeSet -> hasAnyNonExcludingConstraint() &&
                    this -> numberOfEmptyListFields > 0) {
                state -> constraintRangeSet.ranges.pop_back();
            }
        }

        bool isFieldInvalid(TypedListColumnQueryState<T>* typedListState, int valueIndex, int startPoint) const {
            return (typedListState -> isMainColumn && hasBeenVisited(valueIndex, startPoint, typedListState))
                        || (typedListState -> valueRangeSet != NULL && typedListState -> valueRangeSet -> isExcluded(this -> fields[valueIndex].value));
        }
        
        void addToResultIfNeeded(std::list<T>& result, int valueIndex, bool fill) const {
            if(fill && valueIndex < this -> fields.size() - this -> numberOfEmptyListFields) {
                result.push_back(this -> fields[valueIndex].value);
            }
        }
        
        int fillRowWithValueAndGetNextFieldId(int valueIndex, int startPoint, Row* row, ColumnQueryState* state, const std::vector<ColumnDesc>& schema, bool fill) const {
            TypedListColumnQueryState<T>* typedListState = getTypedListState(state);
            if(isFieldInvalid(typedListState, valueIndex, startPoint)) {
                return -1;
            }

            std::list<T> result;
            bool hasAnyFieldInRange = false;
            while(this -> fields[valueIndex].nextFieldId < this -> fields.size()) {
                hasAnyFieldInRange |= state -> constraintRangeSet.isInRange(valueIndex);
                addToResultIfNeeded(result, valueIndex, fill);
                valueIndex = this -> fields[valueIndex].nextFieldId;
                if(isFieldInvalid(typedListState, valueIndex, startPoint)) {
                    return -1;
                }
            }
            hasAnyFieldInRange |= state -> constraintRangeSet.isInRange(valueIndex);
            addToResultIfNeeded(result, valueIndex, fill);

            if(hasAnyFieldInRange == false) {
                return -1;
            }

            if(fill) {
                for(int i = 0; i < state -> positionsInResult.size(); ++i)
                row -> set<std::list<T> >(state -> positionsInResult[i], result, schema);
            }
            return this -> fields[valueIndex].nextFieldId;
        }
    };
#endif /* TRISS_ENGINE_SRC_COLUMNS_LISTCOLUMN_H_ */
