#include "TestRendererImpl.h"


TestRendererImpl::TestRendererImpl(void)
{
	pipe_=CreateFileA("\\\\.\\pipe\\myNamedPipe1",	GENERIC_WRITE ,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
}


TestRendererImpl::~TestRendererImpl(void)
{
	if ((pipe_ != NULL&& pipe_ != INVALID_HANDLE_VALUE))
    { 
		CloseHandle(pipe_);
	}
}

