xcopy ".\sdk\*.dll" "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64"  /y/s
xcopy ".\sdk\include\*.*" "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Source\ThirdParty\IVRLowLevelSDK\include"  /y /s
xcopy ".\sdk\lib\*.lib" "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Source\ThirdParty\IVRLowLevelSDK\lib\Win64"  /y /s

mkdir "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64\iconengines"
mkdir "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64\imageformats"
mkdir "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64\platforms"
mkdir "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64\styles"
mkdir "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64\translations"

xcopy ".\iconengines\" "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64\iconengines" /y /S
xcopy ".\imageformats" "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64\imageformats" /y /S
xcopy ".\platforms" "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64\platforms" /y /S
xcopy ".\styles" "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64\styles" /y /S
xcopy ".\translations" "..\..\..\IVRCameraManRecorder\Plugins\IVRCameraMan\Binaries\ThirdParty\IVRLowLevelSDK\Win64\translations" /y /S
