# ConLog

Connection logger - Network process monitoring (developed in 2018)

### Base structure and flow

* Agent (client) collects connection info from host and sends to Collector (server)
    * collect network processes and send diffs to Collector
    * determine host persistent unique id
    * grpc + protobuf
    * api key
    * ssl client auth
    * installer

* Collector
    * validates data sent from Agent
    * writes data to Queue (message queue)

* Queue
    * RabbitMQ cluster

* Persister
     * writes in LogsDB data Collector sent to Queue

* Analyzer
    * gets data from Queue, analyzes it, and sends message notifications to Notifier
    
* Notifier
    * sends notifications (db, email, push, sms etc)

* LogsDB
    * NoSql database

* CLAPI
    * REST API for LogsDB browse

* Viewer
    * UI client for CLAPI
        * View logs (all recorded processes)
        * View current map (current processes)
        * View Analyzer notifications
    * configure Agent installer (generate /etc/conlog/agent.yml)
        * API KEY
        * protocols
        * delay

### Development

#### Setup
* Install:
    * Docker
    * docker-compose

#### Run in Docker container
* ./dev/conlog up