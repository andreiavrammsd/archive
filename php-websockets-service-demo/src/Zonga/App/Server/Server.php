<?php

namespace Zonga\App\Server;

use Ratchet\ConnectionInterface;
use Ratchet\MessageComponentInterface;
use Zonga\App\App\AppException;
use Zonga\App\App\AppInterface;

class Server implements MessageComponentInterface
{
    /**
     * @var AppInterface
     */
    private $app;

    /**
     * @var array
     */
    private $clients = [];

    /**
     * @param AppInterface $app
     */
    public function __construct(AppInterface $app)
    {
        $this->app = $app;
    }

    /**
     * @param ConnectionInterface $conn
     */
    public function onOpen(ConnectionInterface $conn)
    {
        /** @var \Guzzle\Http\QueryString $query */
        $query = $conn->WebSocket->request->getQuery();
        $session = $query->get('session');

        try {
            $this->app->setSession($session);
            $this->app->addClient($conn->resourceId);
        } catch (AppException $e) {
            $conn->send($e->getMessage());
            $conn->close();
        } catch (\Exception $e) {
            $conn->send('Internal Server Error');
            $conn->close();
        }

        $this->clients[$conn->resourceId] = $conn;
        
        echo "New connection! ({$conn->resourceId})\n";
    }

    /**
     * @param ConnectionInterface $from
     * @param string $msg
     */
    public function onMessage(ConnectionInterface $from, $msg)
    {
        try {
            $this->app->setConnection($from);
            $this->app->handle($msg);
            $response = $this->app->getResponse();
        } catch (\Exception $e) {
            $response = $e->getMessage();
        }

        $from->send($response);
    }

    /**
     * @param ConnectionInterface $conn
     */
    public function onClose(ConnectionInterface $conn)
    {
        $this->app->removeClient($conn->resourceId);
        unset($this->clients[$conn->resourceId]);

        echo "Connection {$conn->resourceId} has disconnected\n";
    }

    /**
     * @param ConnectionInterface $conn
     * @param \Exception $e
     */
    public function onError(ConnectionInterface $conn, \Exception $e)
    {
        $conn->send($e->getMessage());
        $conn->close();
        unset($this->clients[$conn->resourceId]);

        echo "An error has occurred: {$e->getMessage()}\n";
    }
}
