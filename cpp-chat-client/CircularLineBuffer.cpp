
#include "CircularLineBuffer.h"

bool CircularLineBuffer::_writeChars(const char *chars, size_t nchars) {
    if (this->freeSpace() < nchars) {
        return false;
    }

    for(int i = 0; i < nchars; i++) {
        const int index = nextFreeIndex();
        if (index >= 0) {
            buffer[index] = chars[i];
            count++;
        } else {
            return false;
        }
    }

    return true;
}

int CircularLineBuffer::freeSpace() {
    return bufferSize - count;
}

bool CircularLineBuffer::isFull() {
    return bufferSize == count;
}

bool CircularLineBuffer::isEmpty() {
    return count == 0;
}

std::string CircularLineBuffer::_readLine() {
    std::string result;

    if (hasLine()) {
        int lineSize = (findNewline() - start + 1 + bufferSize) % bufferSize;

        while(result.size() < lineSize) {
            result += buffer[start];
            start = (start + 1)%bufferSize;
            count--;
        }
    }

    return result;
}

int CircularLineBuffer::nextFreeIndex() {
    if(isFull())
        return -1;

    return (start + count)%bufferSize;
}

int CircularLineBuffer::findNewline() {
    for(int i = 0; i < count; i++) {
        const int index = (start + i)%bufferSize;
        if (buffer[index] == '\n')
            return index;
    }

    return -1;
}

bool CircularLineBuffer::hasLine() {
    return findNewline() != -1;
}