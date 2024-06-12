#pragma once 
#include "D3D12ThirdPart.h"

class RCommandQueue
{

};
class RCommandListManager
{
public:
private:
	ID3D12Device* m_Device; 
	RCommandQueue m_GrphicsQueue;
	RCommandQueue m_ComputeQueue;
	RCommandQueue m_CopyQueue;
};