<?php

namespace Zonga\App\Storage;

interface StorageInterface
{
    /**
     * @param string $key
     * @param $value
     * @param int $ttl Seconds
     */
    public function set(string $key, $value, int $ttl = 0);

    /**
     * @param string $key
     * @return array
     */
    public function get(string $key) : array;

    /**
     * @param string $key
     * @return array
     */
    public function getAll(string $key) : array;

    /**
     * @param string $key
     */
    public function delete(string $key);
}
