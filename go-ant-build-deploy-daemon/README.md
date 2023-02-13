# Go Build & Deploy with Apache Ant

## This is just an exercise, not considered totally reliable

### A tool for building and deploying GO projects as daemons, developed for Ubuntu 16.04

#### Dependencies

* [Go](https://golang.org/)
* Environment variable: GOPATH
* [Java](https://www.java.com/)
* [Apache Ant](http://ant.apache.org/manual/install.html)
* [JSch](http://www.jcraft.com/jsch/index.html) (provided in this repository)
* [Git](https://git-scm.com/)
* Public ssh key added to remote host

#### Configure projects

* Make a new properties file for each project: cp projects/project.properties.sample projects/myproject.properties
* Properties:
    * project.repository=ssh or http url to repository
    * project.branch=name of branch
    * project.path=path from repository

    * deploy.host=remote host to deploy binary to
    * deploy.port=remote host port
    * deploy.user=remote host user
    * deploy.ssh_private_key_file=path to local private ssh key
    * deploy.path=deploy path on remote host
    * deploy.filename=binary name on remote host
    * deploy.env=environment variables to send to remote host

#### Usage

* ./run.sh myproject

#### Deploy logs

* All deploys will be logged to the ./logs directory

#### Workflow

* Git clone specified branch from project repository into temporary dir in GOPATH
* Get go dependencies and build binary
* Run tests
* Copy binary to remote host
* Set up binary as daemon and run it (restart at system boot)
* Remove temporary file used during deploy
* On remote host you'll find a log file at: /var/log/<binaryname>.log

#### Nice to have

* Execute cleanup even if build failed
