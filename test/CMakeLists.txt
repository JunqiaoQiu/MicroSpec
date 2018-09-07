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
add_executable (specDFAcheck specDFAcheck.cpp)
target_link_libraries(specDFAcheck ${PROJECT_NAME})
