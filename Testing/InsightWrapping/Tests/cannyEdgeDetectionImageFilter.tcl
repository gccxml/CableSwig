load ./libVXLNumericsTcl.dll
load ./libITKCommonTcl.dll
load ./libITKNumericsTcl.dll
load ./libITKBasicFiltersTcl.dll
load ./libITKAlgorithmsTcl.dll
load ./libITKIOTcl.dll

load ./libVXLNumericsTcl.so
load ./libITKCommonTcl.so
load ./libITKNumericsTcl.so
load ./libITKBasicFiltersTcl.so
load ./libITKAlgorithmsTcl.so
load ./libITKIOTcl.so



set inputFileName  "$env(ITK_DATA_ROOT)/Input/cthead1.png"
set outputFileName "$env(ITK_DATA_ROOT)/cthead1canny.png"

# Setup pipeline.

set reader   [ImageFileReaderF2_New]
set canny    [CannyEdgeDetectionImageFilterF2F2_New]
set rescaler [RescaleIntensityImageFilterF2US2_New]
set writer   [ImageFileWriterUS2_New]


$canny     SetInput [$reader    GetOutput]
$rescaler  SetInput [$canny     GetOutput]
$writer    SetInput [$rescaler  GetOutput]

$rescaler SetOutputMinimum 0
$rescaler SetOutputMaximum 65535

$reader SetFileName $inputFileName  
$writer SetFileName $outputFileName 

$writer SetImageIO [PNGImageIO_New]

$writer Update

