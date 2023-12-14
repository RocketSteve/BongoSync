#ifndef MESSAGECREATOR_H
#define MESSAGECREATOR_H

#include "MessageBuilder.h"

class MessageCreator {
public:
    static MessageBuilder create() {
        return MessageBuilder();
    }
};

#endif // MESSAGECREATOR_H
