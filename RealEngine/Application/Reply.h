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

Reply Reply::Unhandled()
{
    return Reply(false);
}
Reply Reply::Handled()
{
    return Reply(true);
}

bool Reply::IsHandled() const 
{
    return bIshandled;
}
