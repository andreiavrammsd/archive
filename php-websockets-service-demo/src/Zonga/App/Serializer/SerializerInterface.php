<?php

namespace Zonga\App\Serializer;

interface SerializerInterface
{
    /**
     * @param array $data
     * @return string
     */
    public function serialize(array $data): string;

    /**
     * @param string $data
     * @return array
     */
    public function unserialize(string $data);
}
