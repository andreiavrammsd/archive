<?php

namespace Zonga\App\Request;

use Zonga\App\Serializer\SerializerInterface;

abstract class AbstractBaseRequest
{
    /**
     * @var SerializerInterface
     */
    private $serializer;

    /**
     * @var array
     */
    private $request;

    /**
     * @param SerializerInterface $serializer
     */
    public function __construct(SerializerInterface $serializer)
    {
        $this->serializer = $serializer;
    }

    /**
     * @param string $message
     */
    public function setRequest(string $message)
    {
        $this->request = $this->serializer->unserialize($message);
    }

    /**
     * @throws BadRequestException
     */
    public function validateRequest()
    {
        if (!$this->request) {
            throw new BadRequestException('Invalid request body');
        }

        if (empty($this->request['event'])) {
            throw new BadRequestException('Missing event');
        }

        if (array_key_exists('payload', $this->request) && !is_array($this->request['payload'])) {
            throw new BadRequestException('Invalid payload');
        }
    }

    /**
     * @return string
     */
    public function getEvent() : string
    {
        return $this->request['event'];
    }

    /**
     * @return array
     */
    public function getPayload() : array
    {
        return array_key_exists('payload', $this->request) ? $this->request['payload'] : [];
    }
}
