string(REPLACE "/" ";" CURRENT_NAME_LIST ${CPP_NAME})
list(REVERSE CURRENT_NAME_LIST)
list(GET CURRENT_NAME_LIST 0 CURRENT_NAME_WITH_CPP)
string(REPLACE "." ";" CURRENT_NAME_LIST2 ${CURRENT_NAME_WITH_CPP})
list(GET CURRENT_NAME_LIST2 0 CURRENT_NAME)	