FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.i)
FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.swg)
INSTALL(FILES ${__files1} ${__files2}
  DESTINATION ${CableSwig_INSTALL_ROOT_CM24}/lib/CableSwig/SWIGLib/java
  COMPONENT Development)
