from VXLNumericsTcl import *
from ITKCommonTcl import *
from ITKNumericsTcl import *
from ITKBasicFiltersTcl import *
from ITKAlgorithmsTcl import *
from ITKIOTcl import *


reader = ImageFileReaderF2_New()
canny  = CannyEdgeDetectionImageFilterF2F2_New()
rescaler = RescaleIntensityImageFilterF2US2_New()
writer = ImageFileWriterUS2_New()
canny.SetInput(reader.GetOutput())
rescaler.SetInput(canny.GetOutput())
writer.SetInput(rescaler.GetOutput())

rescaler.SetOutputMinimum(0)
rescaler.SetOutputMaximum(65535)

reader.SetFileName("/home/hoffman/Insight/Testing/Data/Input/cthead1.png")
writer.SetFileName("./testout.png")
writer.SetImageIO(PNGImageIO_New())
writer.Update()

