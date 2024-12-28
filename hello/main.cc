#include "hello.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>

class helloImpl final : public hello::Hello::Service {
  public:
    grpc::Status SayHello(::grpc::ServerContext *context, const ::google::protobuf::Empty *request, ::hello::helloRes *response) override {
        response->set_msg("Hi my name lsh");
        return grpc::Status::OK;
    }
};

// 서버 실행 함수
void RunServer() {
    std::string server_address("0.0.0.0:50051");
    helloImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main() {
    RunServer();
    return 0;
}
