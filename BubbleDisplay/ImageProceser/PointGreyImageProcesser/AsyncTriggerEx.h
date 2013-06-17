//=============================================================================
// Project Includes
//=============================================================================
#include <pgrflycapture.h>
#include <string>

void configureExternalTriggerCamera(FlyCaptureContext  context);
FlyCaptureError checkSoftwareTriggerPresence( FlyCaptureContext  context,unsigned int       uiRegister );
FlyCaptureError checkTriggerReady(FlyCaptureContext context );
void _HANDLE_ERROR(FlyCaptureError error, std::string function ) ;