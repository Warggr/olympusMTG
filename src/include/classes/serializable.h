#ifndef OLYMPUS_SERIALIZABLE_H
#define OLYMPUS_SERIALIZABLE_H

#include <condition_variable>

class ReaderVisitor; class WriterVisitor;

template<bool read>
using Visitor = typename std::conditional<read, ReaderVisitor, WriterVisitor>::type;

template<typename Host, bool read>
using ConstHost = typename std::conditional<read, Host, const Host>::type;

template<typename Host, bool read>
void visit(ConstHost<Host, read>& host, Visitor<read>& visitor);

#endif //OLYMPUS_SERIALIZABLE_H
