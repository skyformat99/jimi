
#ifndef _JIMI_LANG_THROWABLE_H_
#define _JIMI_LANG_THROWABLE_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jimi/basic/stddef.h"
#include "jimi/io/Serializable.h"
#include "jimi/lang/ObjectBase.h"
#include "jimi/lang/String.h"

#include <string>

namespace jimi {

class Object;

class Throwable : public ObjectBase, public Serializable
{
public:
    Throwable();
    Throwable(const Throwable &cause);
    Throwable(const jimi::string &message);
    Throwable(const jimi::string &message, const Throwable &cause);
    Throwable(const std::string &message);
    Throwable(const std::string &message, const Throwable &cause);
    ~Throwable();

private:
    jimi::string detailMessage;
    Throwable *cause;
};

Throwable::Throwable()
: detailMessage()
, cause(NULL)
{

}

Throwable::Throwable(const Throwable &cause)
{
    this->cause = (Throwable *)&cause;
}

Throwable::Throwable(const jimi::string &message)
{
    detailMessage = message;
}

Throwable::Throwable(const jimi::string &message, const Throwable &cause)
{
    detailMessage = message;
    this->cause = (Throwable *)&cause;
}

Throwable::Throwable(const std::string &message)
{
    detailMessage = message;
}

Throwable::Throwable(const std::string &message, const Throwable &cause)
{
    detailMessage = message;
    this->cause = (Throwable *)&cause;
}

Throwable::~Throwable()
{
    this->cause = NULL;
}

}  /* namespace jimi */

#endif  /* _JIMI_LANG_THROWABLE_H_ */
