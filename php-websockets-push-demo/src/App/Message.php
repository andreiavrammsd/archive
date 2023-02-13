<?php

namespace App;

class Message implements MessageInterface
{
    /**
     * @var string
     */
    private $username;

    /**
     * @var string
     */
    private $message;

    /**
     * @param string $username
     * @return MessageInterface
     */
    public function setUsername($username) : MessageInterface
    {
        $this->username = $username;
        
        return $this;
    }

    /**
     * @param string $message
     * @return MessageInterface
     */
    public function setMessage($message) : MessageInterface
    {
        $this->message = $message;
        
        return $this;
    }

    /**
     * @return string
     */
    public function __toString(): string
    {
        $event = [
            'subscription' => 'update:' . $this->username,
            'payload' => [
                'message' => $this->message,
                'time' => date('Y-m-d H:i:s'),
            ],
        ];
        
        return json_encode($event);
    }
}
