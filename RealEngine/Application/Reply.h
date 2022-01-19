#pragma once 

struct Reply
{
public:
    Reply(bool InIsHandled)
        : bIshandled(InIsHandled)
    {}

    bool IsHandled() const ;
    static Reply Unhandled();
    static Reply Handled();
private:
    bool bIshandled;
};
