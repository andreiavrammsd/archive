<?php

declare(strict_types = 1);

require dirname(__DIR__) . '/vendor/autoload.php';

use App\Config;
use App\Message;

$message = new Message();
$message->setUsername($argv[1]);
$message->setMessage($argv[2]);

$context = new ZMQContext();
$socket = $context->getSocket(ZMQ::SOCKET_PUSH, 'Push');
$socket->connect(Config::QUEUE_PUBLISH);
$socket->send((string)$message);
