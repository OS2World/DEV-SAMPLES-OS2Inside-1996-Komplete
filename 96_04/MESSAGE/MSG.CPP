
#include <ocl.hpp>
#include <OMessage.hpp> 


void main(void)
{
 APIRET  rc[4] = { ERROR_FILE_NOT_FOUND,
                   ERROR_ACCESS_DENIED,
                   ERROR_NOT_READY,
                   ERROR_INVALID_FUNCTION };

 for (ULONG i = 0; i < 4; i++)
  {
   cerr << "Return Code : " << rc[i] << endl
        << "Message     : " << OMessage(rc[i])
        << endl;
  }
}
