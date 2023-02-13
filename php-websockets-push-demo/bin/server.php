<?php

declare(strict_types = 1);

require dirname(__DIR__) . '/vendor/autoload.php';

use App\Config;
use App\Server;

$loop = React\EventLoop\Factory::create();
$server = new Server();

$context = new React\ZMQ\Context($loop);
$pull = $context->getSocket(ZMQ::SOCKET_PULL);
$pull->bind(Config::QUEUE_CONSUME);
$pull->on('message', array($server, 'onEventPush'));

$webSock = new React\Socket\Server($loop);
$webSock->listen(Config::PORT, Config::IP);
$webServer = new Ratchet\Server\IoServer(
    new Ratchet\Http\HttpServer(
        new Ratchet\WebSocket\WsServer(
            new Ratchet\Wamp\WampServer(
                $server
            )
        )
    ),
    $webSock
);

$loop->run();
