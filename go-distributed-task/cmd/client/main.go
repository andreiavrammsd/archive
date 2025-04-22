package main

import (
	"fmt"
	"log"
	"math/rand"
	"strings"
	"sync"
	"time"

	"github.com/andreiavrammsd/dtask/clients/golang"

	"github.com/andreiavrammsd/dtask/pkg/cluster"
	"github.com/andreiavrammsd/dtask/pkg/protocol/rpc"
)

func main() {
	const count = 1
	clients := make([]cluster.Client, count)
	lock := sync.RWMutex{}

	for i := 0; i < count; i++ {
		dTaskClient := &rpc.Client{Addr: fmt.Sprintf(":808%d", i)}
		if err := dTaskClient.Connect(); err != nil {
			log.Fatal("dialing:", err)
		}
		clients[i] = dTaskClient
	}

	go func() {
		for {
			for i := 0; i < count; i++ {
				go func(i int) {
					lock.RLock()
					dTaskClient := clients[i]
					lock.RUnlock()

					status, err := dTaskClient.Status()
					if err != nil {
						log.Fatal(err)
					}

					fmt.Println(strings.Repeat("-", 100))
					fmt.Println("number of nodes", len(status.Nodes))
					for _, n := range status.Nodes {
						fmt.Printf("Node %s has load: %d\n", n.Address, n.TaskCount)
					}
				}(i)
			}

			time.Sleep(time.Millisecond * 1000)
		}
	}()

	for {
		time.Sleep(time.Millisecond * 1000)
		for i := 0; i < 30; i++ {
			time.Sleep(time.Millisecond * 1000)
			go func(i int) {
				lock.RLock()
				rand.Seed(int64(i))
				dTaskClient := clients[rand.Intn(len(clients))]
				lock.RUnlock()

				if err := dTaskClient.Ping(); err != nil {
					log.Fatal(err)
				}

				t := golang.New(dTaskClient)

				// work
				n := rand.Intn(10)
				if n == 0 {
					n = 1
				}

				// Wrapper
				result, err := t.Fibonacci(n)
				if err != nil {
					log.Println(err)
					return
				}

				//// Async
				//call := dTaskClient.WorkAsync("fibonacci", n)
				//call.Wait()
				//
				//if call.Error() != nil {
				//	log.Println(call.Error())
				//	return
				//}
				//result := call.Result()

				//// Sync
				//result, err := dTaskClient.Work("fibonacci", n)
				//if err != nil {
				//	log.Fatal(err)
				//}

				fmt.Printf("fibonacci result for %d is %v\n", n, result)

			}(i)
		}
	}
}
