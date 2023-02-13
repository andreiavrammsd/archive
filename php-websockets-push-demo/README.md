# Client subscribe to socket

## PHP web sockets demo (Send messages to subscribed clients)

#### Requires

* Git
* Docker
* Docker compose

#### Setup

* cd docker
* docker-compose up -d

#### Client

* Open client/index.html in a browser with a hash as username (client/index.html#johndoe)
* You can open multiple clients at the same time

#### Send push to client

* cd docker
* docker-compose run --rm push-server php bin/push.php johndoe "Something has changed"

#### Other

* composer update: docker-compose run --rm socket-server composer update
* supervisor reload: docker-compose exec socket-server supervisorctl -c /etc/supervisor/conf.d/supervisord.conf reload
* enter container: docker-compose run socket-server bash
