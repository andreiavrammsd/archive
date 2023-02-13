<?php

namespace Zonga\App\Storage;

use Predis\Client;

class Redis implements StorageInterface
{
    /**
     * @var Client
     */
    private $client;

    /**
     * @param array $config
     */
    public function __construct(array $config)
    {
        $this->client = new Client($config);
        $this->client->connect();
    }

    /**
     * @param string $key
     * @param $value
     * @param int $ttl
     */
    public function set(string $key, $value, int $ttl = 0)
    {
        $this->client->set($key, json_encode($value));

        if ($ttl > 0) {
            $this->client->expire($key, $ttl);
        }
    }

    /**
     * @param string $key
     * @return array
     */
    public function get(string $key) : array
    {
        return (array)json_decode($this->client->get($key), true);
    }

    /**
     * @param string $key
     * @return array
     */
    public function getAll(string $key) : array
    {
        $keys = $this->client->keys($key);
        $data = [];

        foreach ($keys as $key) {
            $data [] = json_decode($this->client->get($key), true);
        }

        return $data;
    }

    /**
     * @param string $key
     */
    public function delete(string $key)
    {
        $this->client->del([$key]);
    }
}
