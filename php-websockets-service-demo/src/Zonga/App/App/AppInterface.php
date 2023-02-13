<?php

namespace Zonga\App\App;

use Ratchet\ConnectionInterface;

interface AppInterface
{
    /**
     * @param string $sessionId
     */
    public function setSession(string $sessionId);

    /**
     * @param int $resourceId
     */
    public function addClient(int $resourceId);

    /**
     * @param int $resourceId
     */
    public function removeClient(int $resourceId);

    /**
     * @param ConnectionInterface $connection
     */
    public function setConnection(ConnectionInterface $connection);

    /**
     * @param string $message
     */
    public function handle(string $message);

    /**
     * @return string
     */
    public function getResponse(): string;
}
