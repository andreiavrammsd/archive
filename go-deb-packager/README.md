# Go Deb Packager

## This is just an exercise

### Build Go daemons into Deb packages

* It will build your Go project into a Debian package.
* After you install the package, your app will be started and set up as daemon (with restart at reboot).
* It builds for amd64 architectures, on Ubuntu 16.10 with Go 1.7 (see Dockerfile).
* All builds are race condition tested.

### Dependencies

* Docker
* Docker compose

### Structure

* Config: any file/directory added here will be found, after install, at /etc/yourpackage
* Release: all packages will be stored here
* Template: the deb package template files
* .env.dist: Sample package config

### Usage

* Create package config: cp .env.dist .env
* Run: ./pkg

### Nice to have

* [Multi os/architecture builds](https://golang.org/doc/install/source#environment)
* Pull project from repository
* Deploy
* Create git tag

### A simple way to distribute your package

Save this as install.sh on your server:

```
#!/bin/sh

HOST=127.0.0.1
DIR=/
FILE=pkg_1.0.0.deb
TMP=/tmp/${FILE}
curl ${HOST}${DIR}${FILE} > ${TMP} && sudo dpkg -i ${TMP} && rm ${TMP}
```

Then:

```
curl https://127.0.0.1/install.sh | sh
```
