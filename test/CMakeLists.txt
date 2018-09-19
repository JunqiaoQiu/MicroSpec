# Test 1
add_executable (mappingrulescheck mappingrulescheck.cpp)
target_link_libraries(mappingrulescheck ${PROJECT_NAME})

# Test 2
add_executable (InputAndTableCheck InputAndTableCheck.cpp)
target_link_libraries(InputAndTableCheck ${PROJECT_NAME})

# Test 3
add_executable (predictorCheck predictorCheck.cpp)
target_link_libraries(predictorCheck ${PROJECT_NAME})

# Test 4
add_executable (DFAcheckOnSingleCore DFAcheckOnSingleCore.cpp)
target_link_libraries(DFAcheckOnSingleCore ${PROJECT_NAME})

# Test 5
add_executable (DFAcheckOnMultipleCores DFAcheckOnMultipleCores.cpp)
target_link_libraries(DFAcheckOnMultipleCores ${PROJECT_NAME})
