<?php

namespace Zonga\App\Session;

use Zonga\App\Storage\StorageInterface;

interface SessionInterface
{
    /**
     * @param StorageInterface $storage
     */
    public function setStorage(StorageInterface $storage);

    /**
     * @param string $sessionId
     * @throws SessionNotFoundException
     */
    public function setSession(string $sessionId);

    /**
     * @return bool
     */
    public function isValid() : bool;

    /**
     * @return string
     */
    public function getSessionId() : string;

    /**
     * @return string
     */
    public function getUsername() : string;
}
