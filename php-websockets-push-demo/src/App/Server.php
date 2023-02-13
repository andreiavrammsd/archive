<?php

namespace App;

use Ratchet\ConnectionInterface;
use Ratchet\Wamp\WampServerInterface;

class Server implements WampServerInterface
{
    protected $subscriptions = [];

    public function onSubscribe(ConnectionInterface $conn, $topic)
    {
        $this->subscriptions[$topic->getId()] = $topic;
    }

    public function onUnSubscribe(ConnectionInterface $conn, $topic)
    {
        unset($this->subscriptions[$topic->getId()]);
    }

    public function onOpen(ConnectionInterface $conn)
    {
    }

    public function onClose(ConnectionInterface $conn)
    {
    }

    public function onCall(ConnectionInterface $conn, $id, $topic, array $params)
    {
        $conn->callError($id, $topic, 'You are not allowed to make calls')->close();
    }

    public function onPublish(ConnectionInterface $conn, $topic, $event, array $exclude, array $eligible)
    {
        $conn->close();
    }

    public function onError(ConnectionInterface $conn, \Exception $e)
    {
    }

    /**
     * @param string $message
     */
    public function onEventPush(string $message)
    {
        $event = json_decode($message, true);

        if (array_key_exists($event['subscription'], $this->subscriptions)) {
            $topic = $this->subscriptions[$event['subscription']];
            $topic->broadcast($event['payload']);
        }
    }
}
