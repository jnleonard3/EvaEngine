CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_TEST(EvaSimpleDynamicArrayTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/EvaSimpleDynamicArrayTest)
ADD_EXECUTABLE(EvaSimpleDynamicArrayTest ${TEST_MAIN} TestEvaSimpleDynamicArray.cpp)
TARGET_LINK_LIBRARIES(EvaSimpleDynamicArrayTest EvaEngineLib ${CPPUNIT_LIBRARIES})

ADD_TEST(EvaBlockArrayTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/EvaBlockArrayTest)
ADD_EXECUTABLE(EvaBlockArrayTest ${TEST_MAIN} TestEvaBlockArray.cpp)
TARGET_LINK_LIBRARIES(EvaBlockArrayTest EvaEngineLib ${CPPUNIT_LIBRARIES})