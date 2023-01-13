package services

import (
	"fmt"
	"context"
	"log"
	"time"

	"google.golang.org/grpc"
	pb "tldemo/services/Greeter"
)

type GreeterServer struct {
	pb.UnimplementedGreeterServiceServer
}

func (s *GreeterServer) Hello(ctx context.Context, in *pb.HelloRequest) (*pb.HelloResponse, error) {
	currentTime := time.Now()
	replyMessage := fmt.Sprintf("Hello %s, time is %s", in.Name, currentTime.Format("15:04:05"))
	log.Printf("[Hello User] %s", in.Name)
	result := &pb.HelloResponse{ReplyMessage: replyMessage}
	return result, nil
}


type TimeServer struct {
	pb.UnimplementedTimeServiceServer
}

func (s *TimeServer) Ticktok(in *pb.TicktokRequest, stream pb.TimeService_TicktokServer) error {
	ctx := stream.Context()
	response := &pb.NowResponse{}

	sendNow := func(counts int32) {
		response.Now = uint64(time.Now().UTC().UnixMilli())
		response.Counts = counts
		log.Printf("[Ticktok]send time %d/%d", response.Now, counts)
		if err := stream.Send(response); err != nil {
			return
		}
	}
	counts := int32(in.Counts)
	if(counts < 0) {
		counts=0
	}
	
	for {
		sendNow(counts)
		
		if(in.Counts>0) {
			counts-=1
			
			if(counts <= 0) {
				return nil
			}
		}
		
		select {
		case <-ctx.Done():
			return nil
		default:
			time.Sleep(1.0 * time.Second)
		}
	}
	return nil
}

func (s *TimeServer) Watch(stream pb.TimeService_WatchServer) error {
	ctx := stream.Context()
	response := &pb.NowResponse{}

	sendNow := func() error {
		response.Now = uint64(time.Now().UTC().UnixMilli())
		log.Printf("[Watch]send time %d", response.Now)
		return stream.Send(response)
	}

	for {
		select {
		case <-ctx.Done():
			return nil
		default:
		}

		if _, err := stream.Recv(); err != nil {
			return err
		} else if err := sendNow(); err != nil {
			return err
		}
	}
}


func RegisterGreeterServer(s *grpc.Server) {
	greeterServer := &GreeterServer{}
	pb.RegisterGreeterServiceServer(s, greeterServer)
	
	timeServer := &TimeServer{}
	pb.RegisterTimeServiceServer(s, timeServer)
}


