# 필요한 .proto 파일 이름 설정 (확장자 제외, 여러 개 가능)
set(PROTO_NAMES hello)

# 생성된 파일들의 경로 설정
set(PROTO_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../protos_build)
set(PROTO_SOURCES "")
set(PROTO_HEADERS "")

foreach(PROTO_NAME ${PROTO_NAMES})
    list(APPEND PROTO_SOURCES
        ${PROTO_BUILD_DIR}/${PROTO_NAME}.pb.cc
        ${PROTO_BUILD_DIR}/${PROTO_NAME}.grpc.pb.cc
    )
    list(APPEND PROTO_HEADERS
        ${PROTO_BUILD_DIR}/${PROTO_NAME}.pb.h
        ${PROTO_BUILD_DIR}/${PROTO_NAME}.grpc.pb.h
    )
endforeach()

# 출력 확인 (디버그용)
message(STATUS "Generated PROTO_SOURCES: ${PROTO_SOURCES}")
message(STATUS "Generated PROTO_HEADERS: ${PROTO_HEADERS}")

# 결과를 상위 CMake에서 사용할 수 있도록 설정 (필요시)
set(PROTO_SOURCES ${PROTO_SOURCES} PARENT_SCOPE)
set(PROTO_HEADERS ${PROTO_HEADERS} PARENT_SCOPE)

###########################################################
# 해당 라인부터 수정

# 실행 파일 생성
add_executable(hello main.cc)

# 프로토콜 버퍼 소스 파일 추가
target_sources(hello PRIVATE ${PROTO_SOURCES})

# include 디렉토리 설정
target_include_directories(hello PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../protos_build
)

add_library(hello_lib
  ${PROTO_SOURCES}
  ${PROTO_HEADERS})
target_link_libraries(hello_lib
  absl::check
  gRPC::grpc++_reflection
  gRPC::grpc++
  protobuf::libprotobuf)

# 필요시 라이브러리 링크
target_link_libraries(hello hello_lib)