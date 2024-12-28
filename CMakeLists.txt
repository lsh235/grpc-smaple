cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름 및 언어 설정
project(ProtoBuild CXX)

# Protobuf 및 gRPC 패키지 찾기
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC REQUIRED)

# .proto 파일 및 include 디렉토리 설정
set(PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos)
set(PROTO_INCLUDE_DIRS 
    ${PROTO_SRC_DIR}
    /usr/local/include
)

# .proto 파일 탐색
file(GLOB_RECURSE PROTO_FILES "${PROTO_SRC_DIR}/*.proto")

# 생성된 파일들을 저장할 디렉토리 설정
set(GENERATED_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos_build)

# 출력 디렉토리가 존재하지 않을 경우 생성
file(MAKE_DIRECTORY ${GENERATED_SRC_DIR})

# Include 디렉토리를 위한 -I 옵션 설정
set(PROTO_INCLUDE_FLAGS "")
foreach(dir ${PROTO_INCLUDE_DIRS})
    list(APPEND PROTO_INCLUDE_FLAGS "-I${dir}")
endforeach()

message(status "include path : ${PROTO_INCLUDE_FLAGS}")

# protoc 경로를 명시적으로 찾기
find_program(PROTOC_EXECUTABLE protoc REQUIRED)

# grpc_cpp_plugin 경로 찾기
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin REQUIRED)

# 생성된 파일 리스트 초기화
set(GENERATED_SRCS "")
set(GENERATED_GRPC_SRCS "")

# 각 .proto 파일에 대해 add_custom_command 호출
foreach(PROTO_FILE ${PROTO_FILES})
    # 파일명과 경로 처리
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE) # 확장자 없는 파일명
    set(PB_CC_FILE "${GENERATED_SRC_DIR}/${PROTO_NAME}.pb.cc")
    set(GRPC_PB_CC_FILE "${GENERATED_SRC_DIR}/${PROTO_NAME}.grpc.pb.cc")
    
    # 생성 파일 리스트 업데이트
    list(APPEND GENERATED_SRCS ${PB_CC_FILE})
    list(APPEND GENERATED_GRPC_SRCS ${GRPC_PB_CC_FILE})

    # .proto 파일에 대해 add_custom_command 설정
    add_custom_command(
        OUTPUT ${PB_CC_FILE} ${GRPC_PB_CC_FILE}
        COMMAND ${PROTOC_EXECUTABLE}
        ARGS ${PROTO_INCLUDE_FLAGS}
             --cpp_out=${GENERATED_SRC_DIR}
             --grpc_out=${GENERATED_SRC_DIR}
             --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
             ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
    )
endforeach()

# 프로토콜 버퍼 파일 빌드 타겟 추가
add_custom_target(generate_protos ALL DEPENDS ${GENERATED_SRCS} ${GENERATED_GRPC_SRCS})

# 출력 디렉토리를 인클루드 디렉토리로 설정 (필요 시)
include_directories(${GENERATED_SRC_DIR})

###########################################################
# 해당 라인부터 수정

add_subdirectory(hello)