<?php

namespace Zonga\App\Session;

use Zonga\App\Storage\StorageInterface;

class Session implements SessionInterface
{
    const SESSION_KEY_PREFIX = 'session:';

    /**
     * @var StorageInterface
     */
    private $storage;

    /**
     * @var string
     */
    private $sessionId;
    
    /**
     * @var array
     */
    private $data;

    /**
     * @param StorageInterface $storage
     */
    public function setStorage(StorageInterface $storage)
    {
        $this->storage = $storage;
    }

    /**
     * @param string $sessionId
     * @throws SessionNotFoundException
     */
    public function setSession(string $sessionId)
    {
        $key = self::SESSION_KEY_PREFIX . $sessionId;
        $this->data = $this->storage->get($key);

        if (!$this->usernameExists()) {
            throw new SessionNotFoundException(sprintf('Session not found: %s', $sessionId));
        }

        $this->sessionId = $sessionId;
    }

    /**
     * @return bool
     */
    public function isValid() : bool
    {
        return $this->usernameExists();
    }

    /**
     * @return string
     */
    public function getSessionId() : string
    {
        return $this->sessionId;
    }

    /**
     * @return string
     */
    public function getUsername() : string
    {
        return $this->data['username'];
    }

    /**
     * @return bool
     */
    private function usernameExists()
    {
        return !empty($this->data['username']);
    }
}
