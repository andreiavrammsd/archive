<?php

declare(strict_types = 1);

use Ratchet\Http\HttpServer;
use Ratchet\Server\IoServer;
use Ratchet\WebSocket\WsServer;
use Zonga\App\App\App as ZongaApp;
use Zonga\App\Server\Server as ZongaServer;

require dirname(__DIR__) . '/vendor/autoload.php';
$config = require dirname(__DIR__) . '/src/Zonga/config/config.php';

$server = IoServer::factory(
    new HttpServer(
        new WsServer(
            new ZongaServer(
                new ZongaApp($config)
            )
        )
    ),
    $config['server_port']
);

$server->run();
