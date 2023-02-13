# Go live reload

## Auto reload code while developing Go apps in a Docker container

## Usage
* cd dev
* Start app: docker-compose up -d
* Rebuild after Docker file changes: docker-compose up -d --build
* See apps status: docker-compose ps
* Watch apps logs: docker-compose logs -f
* Watch single app logs: docker-compose logs -f app1

Just update your code, save, and watch the logs.
