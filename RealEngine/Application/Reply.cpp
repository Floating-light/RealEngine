#include "Reply.h"


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
