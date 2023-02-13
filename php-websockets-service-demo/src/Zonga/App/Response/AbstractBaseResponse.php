<?php

namespace Zonga\App\Response;

use Zonga\App\Serializer\SerializerInterface;

abstract class AbstractBaseResponse
{
    /**
     * @var SerializerInterface
     */
    private $serializer;

    /**
     * @var mixed
     */
    private $payload;

    /**
     * @var null|string
     */
    private $error;

    /**
     * @param SerializerInterface $serializer
     */
    public function __construct(SerializerInterface $serializer)
    {
        $this->serializer = $serializer;
    }

    /**
     * @param array $payload
     */
    public function setPayload(array $payload)
    {
        $this->payload = $payload;
    }

    /**
     * @param null|string $error
     */
    public function setError($error)
    {
        $this->error = $error;
    }

    /**
     * @return string
     */
    public function getContent() : string
    {
        $content = [
            'payload' => $this->payload,
            'error' => $this->error,
        ];

        return $this->serializer->serialize($content);
    }
}
