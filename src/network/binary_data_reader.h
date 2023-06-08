#ifndef OLYMPUS_BINARY_DATA_READER_H
#define OLYMPUS_BINARY_DATA_READER_H

#include <string_view>
#include <cassert>

template<typename T>
class BinaryDataArrayView {
    const T* _begin;
    const T* _end;
public:
    BinaryDataArrayView(std::string_view data)
    : _begin(reinterpret_cast<const T*>(data.data()))
    , _end(reinterpret_cast<const T*>(data.data() + data.size()))
    {
        assert(data.size() % sizeof(T) == 0);
    }

    const T* begin() const { return _begin; }
    const T* end() const { return _end; }
};

class BinaryDataReader {
    const char* cursor;
    const char* end;
public:
    BinaryDataReader(std::string_view data): cursor(data.data()), end(data.data() + data.size()) {}

    template<typename T> const T& get(){
        const char* result = cursor;
        cursor += sizeof(T);
        assert(cursor <= end);
        return *reinterpret_cast<const T*>(result);
    }

    bool empty() const { assert(cursor <= end); return cursor == end; }

    std::string_view remainder() {
        auto result = std::string_view(cursor, end);
        cursor = end;
        return result;
    }
};

template<typename DataReader>
class OwningReader: public DataReader {
    std::string data;
public:
    OwningReader(std::string&& data): DataReader(data), data(std::move(data)) {}
};

#endif //OLYMPUS_BINARY_DATA_READER_H
