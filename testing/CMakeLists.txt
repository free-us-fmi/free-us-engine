project(test)

add_executable(test main.cpp
)


target_include_directories(test PUBLIC ../testing )
target_include_directories(test PUBLIC ../ )

target_link_libraries(test engine)
target_link_libraries(test serializer)




