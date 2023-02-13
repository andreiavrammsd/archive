# Web sockets service

## PHP web sockets demo

#### Requires

* Docker
* Docker compose

#### Setup

* cd docker
* docker-compose up -d
* Set up demo session data: docker-compose exec zonga-socks-storage redis-cli SET session:80dbfe846e8cdcc7d4fcc95d7a4b81ec "{\"username\": \"testusername\"}"

#### Client

* Open client/index.html in a browser

#### Useful

* Rebuild: docker-compose up -d --build
* Reload app after code change: docker-compose exec zonga-socks /usr/bin/supervisorctl -c /etc/supervisor/conf.d/supervisord.conf reload
* Update composer: docker-compose exec zonga-socks composer update
* Run unit tests: docker-compose exec zonga-socks /zonga/vendor/bin/phpunit -c src/Zonga/Tests/phpunit.xml
