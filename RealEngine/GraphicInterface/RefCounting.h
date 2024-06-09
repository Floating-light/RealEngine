#pragma once 

template<typename ReferencedType>
class TRefCountPtr
{
    using ReferenceType = ReferencedType*;
public:
    TRefCountPtr():
        mReference(nullptr)
    {}

    TRefCountPtr(ReferencedType* InReference)
    {
        mReference = InReference;
        if(mReference)
        {
            mReference->AddRef();
        }
    }

    TRefCountPtr(const TRefCountPtr& Copy)
    {
        mReference = Copy.mReference;
        if(mReference)
        {
            mReference->AddRef();
        }
    }

    TRefCountPtr(TRefCountPtr&& Moved)
    {
        mReference = Moved.mReference;
        Moved.mReference = nullptr;
    }

    ~TRefCountPtr()
    {
        if(mReference)
        {
            mReference->Release();
        }
    }

    TRefCountPtr& operator=(ReferencedType* InReference)
    {
        if(mReference != InReference)
        {
            if(mReference)
            {
                mReference->Release();
            }
            mReference = InReference;
            if(mReference)
            {
                mReference->AddRef();
            }
        }
        return *this;
    }

    TRefCountPtr& operator=(const TRefCountPtr& InPtr)
    {
        return *this = InPtr.mReference;
    }

    TRefCountPtr& operator=(TRefCountPtr&& InPtr)
    {
        if(this != &InPtr)
        {
            ReferencedType* OldReference = mReference;
            mReference = InPtr.mReference;
            InPtr.mReference = nullptr;
            if(OldReference)
            {
                OldReference->Release();
            }
        }
        return *this;
    }

    ReferencedType* operator->() const 
    {
        return mReference;
    }
    operator ReferenceType() const 
    {
        return mReference;
    }
    // Can be passed to com interface 
    ReferencedType** GetInitReference()
    {
        *this = nullptr;
        return &mReference;
    }

    ReferencedType* GetReference() const 
    {
        return mReference;
    }

    bool IsValid() const 
    {
        return mReference != nullptr;
    }
private:
    ReferencedType* mReference;
};