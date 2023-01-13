package main

import (
	"log"
	"net"
	
	"google.golang.org/grpc"
	"google.golang.org/grpc/reflection"
	"google.golang.org/grpc/credentials"
	
	"tldemo/services"
)

var (
	crt_file = "certs/server.crt"
	key_file = "certs/server.key"
)

func main() {
	lis, err := net.Listen("tcp",":5050")
	if err != nil {
		log.Fatalf("failed to listen: %v", err)
	}
	
	// server-side tls
	// Create the TLS credentials
	creds, err := credentials.NewServerTLSFromFile(crt_file, key_file)
	if err != nil {
		log.Fatalf("could not load TLS keys: %s", err)
	}
	s := grpc.NewServer(grpc.Creds(creds))
	
	//register all server
	services.RegisterGreeterServer(s)
	
	reflection.Register(s)
	log.Printf("server listening at %v", lis.Addr())
	if err := s.Serve(lis); err != nil {
		log.Fatalf("failed to serve: %v", err)
	}	
}
