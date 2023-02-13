<?php

namespace Zonga\App\Serializer;

class JsonSerializer implements SerializerInterface
{
    /**
     * @param mixed $data
     * @return string
     */
    public function serialize(array $data): string
    {
        return json_encode($data);
    }

    /**
     * @param string $data
     * @return array
     */
    public function unserialize(string $data)
    {
        return json_decode($data, true);
    }
}
