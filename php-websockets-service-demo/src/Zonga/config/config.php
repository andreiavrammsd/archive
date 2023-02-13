<?php

$config = [];

$config['server_port'] = 666;

$config['routes'] = [
    'hello' => 'Main::main',
    'quit' => 'Main::quit',
];

$config['storage'] = [
    'scheme' => 'tcp',
    'host' => 'zonga-socks-storage',
    'port' => 6379,
];

$config['session'] = [
    'scheme' => 'tcp',
    'host' => 'zonga-socks-storage',
    'port' => 6379,
];

$config['services'] = [
    'request' => function () {
        $serializer = new \Zonga\App\Serializer\JsonSerializer();
        $request = new \Zonga\App\Request\Request($serializer);

        return $request;
    },
    'router' => function () use ($config) {
        return new \Zonga\App\Router\Router($config['routes']);
    },
    'response' => function () {
        $serializer = new \Zonga\App\Serializer\JsonSerializer();
        $response = new \Zonga\App\Response\Response($serializer);

        return $response;
    },
    'storage' => function () use ($config) {
        return new Zonga\App\Storage\Redis($config['storage']);
    },
    'session' => function () use ($config) {
        $session = new Zonga\App\Session\Session();
        $storage = new Zonga\App\Storage\Redis($config['session']);
        $session->setStorage($storage);

        return $session;
    },
];

return $config;
