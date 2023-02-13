<?php

namespace App;

interface MessageInterface
{
    /**
     * @param string $username
     */
    public function setUsername($username) : MessageInterface;

    /**
     * @param string $message
     */
    public function setMessage($message) : MessageInterface;

    /**
     * @return string
     */
    public function __toString(): string;
}
