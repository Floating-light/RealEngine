#include "Adapter.h"

RAdapter::RAdapter(TRefCountPtr<IDXGIAdapter> Adapter)
{
    ASSERTDX(Adapter->QueryInterface(mAdapter.GetInitReference()));
}

void RAdapter::InitializeDevice()
{
}
