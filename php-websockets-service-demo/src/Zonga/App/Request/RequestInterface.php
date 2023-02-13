<?php

namespace Zonga\App\Request;

use Zonga\App\Serializer\SerializerInterface;

interface RequestInterface
{
    /**
     * @param SerializerInterface $serializer
     */
    public function __construct(SerializerInterface $serializer);

    /**
     * @param string $message
     * @return mixed
     */
    public function setRequest(string $message);

    /**
     * @throws BadRequestException
     */
    public function validateRequest();
    
    /**
     * @return string
     */
    public function getEvent() : string;

    /**
     * @param array
     */
    public function getPayload() : array;
}
