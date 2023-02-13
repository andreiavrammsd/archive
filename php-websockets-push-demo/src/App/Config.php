<?php

namespace App;

class Config
{
    const IP = '0.0.0.0';
    const PORT = 666;
    const QUEUE_CONSUME = 'tcp://0.0.0.0:5555';
    const QUEUE_PUBLISH = 'tcp://socket-server:5555';
}
