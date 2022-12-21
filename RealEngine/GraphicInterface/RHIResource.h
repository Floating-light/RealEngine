#pragma once 
#include <string_view>
#include <cassert>

class RRHIResource
{
protected:
	RRHIResource(std::string_view Name) 
        : mReferenceCount(0)
        , mName{Name}{}
public:
    virtual ~RRHIResource()
    {
        assert(mReferenceCount == 0);
    }
    inline size_t AddRef() 
    {
        ++mReferenceCount;
        return mReferenceCount;
    } 
    inline size_t Release()
    {
        const size_t NewRefCount = --mReferenceCount;
        assert(mReferenceCount >=0);
        return NewRefCount;
    }
    inline size_t GetRefCount() const 
    {
        return mReferenceCount;
    }
private:
    size_t mReferenceCount;
    std::string mName;
};

