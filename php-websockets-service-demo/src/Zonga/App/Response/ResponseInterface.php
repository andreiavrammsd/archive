<?php

namespace Zonga\App\Response;

use Zonga\App\Serializer\SerializerInterface;

interface ResponseInterface
{
    /**
     * @param SerializerInterface $serializer
     */
    public function __construct(SerializerInterface $serializer);

    /**
     * @param array $payload
     */
    public function setPayload(array $payload);

    /**
     * @param null|string $error
     */
    public function setError($error);

    /**
     * @return string
     */
    public function getContent() : string;
}
