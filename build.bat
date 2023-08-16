set BUILD_FOLDER=build

if exist "%BUILD_FOLDER%" (
  del /q "%BUILD_FOLDER%"
) else (
  mkdir "%BUILD_FOLDER%"
)

cd "%BUILD_FOLDER%"
cmake ..
cd ..