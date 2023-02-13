# Basic RabbitMQ Wrapper

### Common usage of queues (publish, consume, acknowledge)

#### Example

* You need to set up a RabbitMQ Docker container:
    * Install Docker: https://docs.docker.com/engine/getstarted/step_one/
    * Run in your terminal: docker run -tid --name go-rabbit -p 5672:5672 -p 15672:15672 rabbitmq:3-management
     
* go run [example/main.go](example/main.go)

* You can can see Rabbit connections and channels in your browser at http://127.0.0.1:15672/ 

* After you're done and want to clean up: docker rm -f go-rabbit
