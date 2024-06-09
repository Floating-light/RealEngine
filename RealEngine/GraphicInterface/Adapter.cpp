#include "Adapter.h"

RAdapter::RAdapter(TRefCountPtr<IDXGIAdapter> Adapter)
{
    ASSERT(Adapter->QueryInterface(mAdapter.GetInitReference()));
}

void RAdapter::InitializeDevice()
{
}
