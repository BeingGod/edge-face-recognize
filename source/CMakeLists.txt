add_executable(edge_face_recognize main.cpp)

target_link_libraries(
    edge_face_recognize
    ${OpenCV_LIBRARIES}
)